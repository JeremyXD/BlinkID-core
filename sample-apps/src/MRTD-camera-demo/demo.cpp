#include <iostream>
#include <ctime>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"

#include "RecognizerApi.h"

/* specifier for correct printf of size_t on 32-bit and 64-bit architectures */
#if defined(_MSC_VER)
#define JL_SIZE_T_SPECIFIER    "%Iu"
#define JL_SSIZE_T_SPECIFIER   "%Id"
#define JL_PTRDIFF_T_SPECIFIER "%Id"
#elif defined(__GNUC__)
#define JL_SIZE_T_SPECIFIER    "%zu"
#define JL_SSIZE_T_SPECIFIER   "%zd"
#define JL_PTRDIFF_T_SPECIFIER "%zd"
#endif

#define KEY_SPACE	32
#define KEY_ESCAPE	27

/* function that converts RecognizerImage to cv::Mat */
cv::Mat createImageFromRecognizerImage(const RecognizerImage* ri) {
	int width;
	int height;
	int bpr;
	const unsigned char *data;
	RawImageType rawType;

	bpr = recognizerImageGetBytesPerRow(ri);
	width = recognizerImageGetWidth(ri);
	height = recognizerImageGetHeight(ri);
	data = recognizerImageGetRawBytes(ri);
	rawType = recognizerImageGetRawImageType(ri);
	
	switch (rawType) {
	case RAW_IMAGE_TYPE_BGRA:
		return cv::Mat(height, width, CV_8UC4, (void*)data, bpr);
		break;
	case RAW_IMAGE_TYPE_BGR:
		return cv::Mat(height, width, CV_8UC3, (void*)data, bpr);
		break;
	case RAW_IMAGE_TYPE_GRAY:
		return cv::Mat(height, width, CV_8UC1, (void*)data, bpr);
		break;
	case RAW_IMAGE_TYPE_NV21:
		return cv::Mat(height + height / 2, width, CV_8UC1, (void*)data, bpr);
		break;
	default:
		return cv::Mat();
	}
}


void onDetectionStarted() {
	printf("Detection has started!\n");
}

int onDetectedObject(const MBPoint* points, const size_t pointsSize, MBSize imageSize, MBDetectionStatus ds) {
	const char* detStatusDesc = "";

	printf("Detection on image of size %dx%d has finished\n", imageSize.width, imageSize.height);
	printf("Total " JL_SIZE_T_SPECIFIER " points have been found\n", pointsSize);
	if (pointsSize>0 && points != NULL) {
		size_t i;
		for (i = 0; i<pointsSize; ++i) {
			printf("Point " JL_SIZE_T_SPECIFIER " location: (%d, %d)\n", i, points[i].x, points[i].y);
		}
	}
	switch (ds) {
	case DETECTION_STATUS_CAMERA_AT_ANGLE:
		detStatusDesc = "camera is at angle";
		break;
	case DETECTION_STATUS_CAMERA_TOO_HIGH:
		detStatusDesc = "camera is too high";
		break;
    case DETECTION_STATUS_CAMERA_TOO_NEAR:
        detStatusDesc = "camera is too near";
        break;
	case DETECTION_STATUS_FAIL:
		detStatusDesc = "detection has failed";
		break;
	case DETECTION_STATUS_PARTIAL_OBJECT:
		detStatusDesc = "some parts of object are not visible";
		break;	
	case DETECTION_STATUS_SUCCESS:
		detStatusDesc = "detection has succeeded";
		break;
	}
	printf("Detection status: %s\n", detStatusDesc);
	return 1;
}

void onDetectionFailed() {
	printf("Detection has completely failed\n");
}

void onRecognitionStarted() {
	printf("Object recognition has started\n");
}

void onRecognitionFinished() {
	printf("Object recognition has finished\n");
}

cv::Mat dewarpedFrame;
int hasDewarped = 0;

/* callback function used to retrieve dewarped images from recognition process */
void onShowImage(const RecognizerImage* image, const ShowImageType showType, const char* name) {	
	/* If we got dewarped image clone it and store it for later use. 
	Image MUST be cloned because it gets deleted after this callback returns */
	if (showType == ShowImageType::SHOW_IMAGE_TYPE_DEWARPED) {
		dewarpedFrame = createImageFromRecognizerImage(image).clone();
		hasDewarped = 1;
	}
}

RecognizerCallback buildRecognizerCallback() {
	RecognizerCallback cb;
	/* onDetectedObject is called when recognizer detects an object in image */
	cb.onDetectedObject = onDetectedObject;
	/* onDetectionStarted is called when recognizer begins object detection process */
	cb.onDetectionStarted = onDetectionStarted;
	/* onDetectionFailed is called when all enabled recognizers do not detect anything on the image */
	cb.onDetectionFailed = onDetectionFailed;
	/* onRecognitionStarted is called when recognition of detected object begins */
	cb.onRecognitionStarted = onRecognitionStarted;
	/* onRecognitionFinished is called when recognition of detected object finishes */
	cb.onRecognitionFinished = onRecognitionFinished;
    /* onDetectionMidway is called when part of object is already detected, but detection is not finished yet */
    cb.onDetectionMidway = NULL;
	/* onShowImage is called during recognition process and allows for additional image processing */
	cb.onShowImage = onShowImage;
	return cb;
}

/* helper function for converting Machine Readable Zone date string to more human readable date */
const char* dateString( MBDate const& mrzdate )
{
	/* write to static string buffer and return it */
	static char stringBuff[12];
	sprintf_s(stringBuff, "%d.%d.%d.", mrzdate.year, mrzdate.month, mrzdate.day);

	return stringBuff;
}

int main(int argc, char** argv)
{
	/* path will contain path to image being recognized */
	const char* path = argv[1];
	/* this variable will contain all recognition settings (which recognizers are enabled, etc.) */
	RecognizerSettings* settings;
	/* this variable will contain MRTD recognition specific settings */
	MRTDSettings mrtdSettings;	
	/* this variable is the global recognizer that internally contains a list of different recognizers.
	Each recognizer is an object that can perform object recognitions. For example, there are PDF417 barcode
	recognizer (Microblink's implementation for PDF417 barcodes), ZXing barcode recognizer (supports everything ZXing supports),
	Microblink's 1D barcode recognizer, etc. */
	Recognizer* recognizer;
	/* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
	RecognizerErrorStatus status;
	/* recoginzer callback structure contains pointers to functions that will be called during the recognition process */
	RecognizerCallback recognizerCallback;
	/* this variable will contain list of scan results obtained from image scanning process. */
	RecognizerResultList resultList;
	/* this variable holds barreled image sent that will be debarreled */
	RecognizerImage* image;
	/* barrelDewarper object used to debarrel images */
	RecognizerBarrelDewarper *barrelDewarper;
	/* this variable holds debarreled image that will be sent to scanning process */
	RecognizerImage* debarreledImage = NULL;

	/* create recognizer settings object. Do not forget to delete it after usage. */
	recognizerSettingsCreate(&settings);

    /* set path to resources folder */
    recognizerSettingsSetResourcesLocation( settings, "resources" );
	
	/* Enable providing the image of full document. Option detectMachineReadableZonePosition must be on in order for this to work! */
	mrtdSettings.showFullDocument = 1; // enabled
	/* Enable providing the image of machine readable zone. Option detectMachineReadableZonePosition must be on in order for this to work! */
	mrtdSettings.showMachineReadableZone = 0; // disabled
	/* add Machine Readable Travel Document recognizer settings to global recognizer settings object */
	recognizerSettingsSetMRTDSettings(settings, &mrtdSettings);

    /* insert license key and licensee */
    recognizerSettingsSetLicenseKeyForLicensee( settings, "Add licensee here", "Add license key here" );
    /* OR insert license key for licensee obtained with LicenseRequestTool (**DO NOT USE BOTH**) */
    recognizerSettingsSetLicenseKey( settings, "Add license key here" );

	/* Create BarrelDewarper object used to debarrel images. 
		Parameters k1, k2, p1, p2, k3, scale must be set
		corresponding to camera geometry and resolution.
		
		recognizerBarrelDewarperCreate((&barrelDewarper, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)
		should be used for cameras with no barrel distortion!*/
	recognizerBarrelDewarperCreate(&barrelDewarper, -3.6e-7f, -7.0e-14f, 0.0f, 0.0f, 0.f, 0.9f);

	/* create global recognizer with settings */
	status = recognizerCreate(&recognizer, settings);
	/* as said earlier, all API functions return RecognizerErrorStatus. You can check the status for error, or you can simply ignore it like earlier in this example. */
	if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
		std::cout << "Error creating recognizer: " << recognizerErrorToString(status) << std::endl;
		return -1;
	}

	/* build recognizer callback structure */
	recognizerCallback = buildRecognizerCallback();

	/* open video capture stream from system default camera using OpenCV */
	cv::VideoCapture camera(0);
	if (!camera.isOpened()) {			
		std::cout << "Could not open camera video stream" << std::endl;
		return -1;
	}
	/* set camera resolution to 1279X723. This should be set to native camera resolution */
	cv::Size resolution(1279, 723);
	camera.set(CV_CAP_PROP_FRAME_WIDTH, resolution.width);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT, resolution.height);	

	/* open camera display window and console window for displaying text using OpenCV */
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display of barreled frames.
	cv::namedWindow("Display debarreled window", cv::WINDOW_AUTOSIZE); // Create a window for display of debarreled frames.
	cv::namedWindow("Text window", cv::WINDOW_AUTOSIZE); // Create a window for results.	

	/* variable for storing user key presses */
    char keystroke = '\0';

	do {
		/* image that will be displayed on the text console window */
		cv::Mat console(480, 640, CV_8UC3, cv::Scalar(40, 40, 40));

		/* image for holding current video capture frame */
		cv::Mat frame;

		/* obtain current frame from camera */
		camera >> frame;

        if( frame.cols == 0 || frame.rows == 0 ) {
            printf( "Skipping empty frame\n" );
            continue;
        }

		/* create the recognizer image object from video capture frame so we can debarrel it*/
		status = recognizerImageCreateFromRawImage(&image, frame.data, frame.cols, frame.rows, frame.step, frame.channels() == 3 ? RAW_IMAGE_TYPE_BGR : RAW_IMAGE_TYPE_BGRA);
		if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
			std::cout << "Error creating image from frame: " << recognizerErrorToString(status) << std::endl;
			return -1;
		}

		/* barrel dewarp the image */
		status = recognizerBarrelDewarperDewarp(barrelDewarper, image, &debarreledImage);

		/* Perform MRTD recognition on a recognizer image(video frame). Remember to set imageIsVideoFrame to true.
		if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
		insert non-NULL function pointers only to those events you are interested in */
		recognizerRecognizeFromImage(recognizer, &resultList, debarreledImage, 1, &recognizerCallback);		

		/* boolean flag that indicates that a valid result has been found */
		int foundResult = 0;

		if (resultList.resultsCount == 1u) {

            /* obtain the first (and only) result from list */
            RecognizerResult* result = resultList.results[ 0 ];

			/* check if it is a MRTD result */
			if ( recognizerResultIsMRTDResult( result ) ) {
				if ( recognizerResultIsResultValid( result ) ) {
					/* valid result has been found so set flag to true */
					foundResult = 1;

                    MRTDResult mrtdResult;
                    recognizerResultGetMRTDResult( result, &mrtdResult );

					/* display results on console window image using OpenCV */
					cv::putText(console, "SUCCESS", cv::Point(200, 100), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(20, 250, 20), 1, CV_AA, false);

					cv::putText(console, mrtdResult.primaryID, cv::Point(200, 145), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, mrtdResult.secondaryID, cv::Point(200, 160), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, strcmp("M", mrtdResult.sex) == 0 ? "MALE" : "FEMALE", cv::Point(200, 175), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, dateString(mrtdResult.dateOfBirth), cv::Point(200, 190), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, mrtdResult.nationality, cv::Point(200, 205), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);

					cv::putText(console, mrtdResult.documentCode, cv::Point(200, 235), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, mrtdResult.documentNumber, cv::Point(200, 250), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, dateString(mrtdResult.dateOfExpiry), cv::Point(200, 265), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, mrtdResult.issuer, cv::Point(200, 280), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, mrtdResult.opt1, cv::Point(200, 295), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, mrtdResult.opt2, cv::Point(200, 310), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);

					cv::putText(console, "Press SPACE to reset scanning", cv::Point(200, 345), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(20, 20, 220), 1, CV_AA, false);
				}
			}
		}

		/* if result is not found display default message on console */
		if (!foundResult) {
			cv::putText(console, "Result Not Found", cv::Point(200, 200), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 250), 1, CV_AA, false);
			cv::putText(console, "Press ESCAPE to exit demo", cv::Point(200, 250), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(250, 250, 250), 1, CV_AA, false);
		}

		/* must delete recognizer image*/
		recognizerImageDelete(&image);
		/* free result list */
		recognizerResultListDelete(&resultList);

		/* convert RecognizerImage to cv::Mat so we can display it */
		cv::Mat debarreledFrame = createImageFromRecognizerImage(debarreledImage);
		/* show camera frame and console image in respective windows */		
		cv::imshow("Display window", frame);
		cv::imshow("Text window", console);
		/* if we have dewarped image show it, if not show debarreled image instead */
		if (foundResult && hasDewarped) {
			cv::imshow("Display debarreled window", dewarpedFrame);
		} else {
			cv::imshow("Display debarreled window", debarreledFrame);
		}		

		/* read user key presses and delay for 10ms */
		keystroke = (char)cv::waitKey(10); // Wait for a keystroke in the window

		/* reset recognizer if user presses SPACE */
		if (keystroke == KEY_SPACE) {
			recognizerReset(recognizer);
			hasDewarped = 0;
		}
	} while (keystroke != KEY_ESCAPE); // exit loop if user presses ESCAPE

	/* cleanup memory */	
	recognizerSettingsDelete(&settings);
	recognizerDelete(&recognizer);		
	recognizerImageDelete(&debarreledImage);
	recognizerBarrelDewarperDelete(&barrelDewarper);

	/* release camera */
	camera.release();

	return 0;
}
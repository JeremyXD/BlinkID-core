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

void onDetectionStarted() {
	printf("Detection has started!\n");
}

int onDetectedObject(const PPPoint* points, const size_t pointsSize, PPSize imageSize, PPDetectionStatus ds) {
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
	case DETECTION_STATUS_FAIL:
		detStatusDesc = "detection has failed";
		break;
	case DETECTION_STATUS_PARTIAL_OBJECT:
		detStatusDesc = "some parts of object are not visible";
		break;
	case DETECTION_STATUS_QR_SUCCESS:
		detStatusDesc = "QR code has been successfuly detected";
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
	/* onProgress is called in some recognizers to provide progress information. Here we are not interested in this callback. */
	cb.onProgress = NULL;
	/* onShouldStopRecognition is called multiple times from some recognizers to check if recognition should be canceled. */
	cb.onShouldStopRecognition = NULL;
	/* onShowImage is called during recognition process and allows for additional image processing */
	cb.onShowImage = NULL;
	return cb;
}

/* helper function for converting Machine Readable Zone date string to more human readable date */
const char* dateString(const char* mrzdate, int future = 0)
{
	/* obtain current year */
	time_t t = time(0);
	struct tm now;
	localtime_s(&now, &t);
	int nowYear = 1900 + now.tm_year;

	/* read year(last 2 digits), month & day from MRZ date string*/
	int yearShort;
	int month;
	int day;
	sscanf_s(mrzdate, "%2d%2d%2d", &yearShort, &month, &day);

	/* get full year from last 2 digits */
	int year;
	if (future) {
		if (yearShort > 80) {
			year = 1900 + yearShort;
		}
		else {
			year = 2000 + yearShort;
		}
	}
	else {
		if (yearShort + 2000 > nowYear) {
			year = 1900 + yearShort;
		}
		else {
			year = 2000 + yearShort;
		}
	}

	/* write to static string buffer and return it */
	static char stringBuff[12];
	sprintf_s(stringBuff, "%d.%d.%d.", year, month, day);

	return stringBuff;
}

int main(int argc, char** argv)
{
	/* path will contain path to image being recognized */
	const char* path = argv[1];
	/* this buffer will contain OCR model */
	char* ocrModel;
	/* this variable will contain OCR model buffer length in bytes */
	int ocrModelLength;
	/* this variable will contain all recognition settings (which recognizers are enabled, etc.) */
	RecognizerSettings* settings;
	/* this variable will contain MRTD recognition specific settings */
	MRTDSettings mrtdSettings;
	/* this variable will contain device information. On Mac/PC this is not usually necessary, but
	can information about available processor cores. If more than 1 processor is available, recognizers
	will try to use parallel algorithms as much as possible. */
	RecognizerDeviceInfo* deviceInfo;
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
	RecognizerResultList* resultList;
	/* this variable will contain number of scan results obtained from image scanning process. */
	size_t numResults;

	/* load OCR model from file */
	status = recognizerLoadFileToBuffer("ocr_model.zzip", &ocrModel, &ocrModelLength);
	if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
		std::cout << "Could not load file ocr_model.zzip" << std::endl;
		return -1;
	}

	/* create recognizer settings object. Do not forget to delete it after usage. */
	recognizerSettingsCreate(&settings);

	/* create device info object. Do not forget to delete it after usage. */
	recognizerDeviceInfoCreate(&deviceInfo);
	/* define that device has 4 processors (you can use any number here - this is used to define number
	of threads library will use for its parallel operations */
	recognizerDeviceInfoSetNumberOfProcessors(deviceInfo, 4);
	/* add device info object to recognizer settings object */
	recognizerSettingsSetDeviceInfo(settings, deviceInfo);
	/* set OCR model to recognizer settings object */
	recognizerSettingsSetZicerModel(settings, ocrModel, ocrModelLength);

	/* enable ID card position detection. Note that card position detection will not work with passport recognition. */
	mrtdSettings.detectCardPosition = 1;
	/* add Machine Readable Travel Document recognizer settings to global recognizer settings object */
	recognizerSettingsSetMRTDSettings(settings, &mrtdSettings);

	/* insert license key and licensee */
	recognizerSettingsSetLicenseKey(settings, "Add licensee here", "Add license key here");	

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

	/* open camera display window and console window for displaying text using OpenCV */
	cv::namedWindow("Display window", cv::WINDOW_AUTOSIZE); // Create a window for display.
	cv::namedWindow("Text window", cv::WINDOW_AUTOSIZE); // Create a window for results.	

	/* variable for storing user key presses */
	char keystroke;

	do {
		/* image that will be displayed on the text console window */
		cv::Mat console(480, 640, CV_8UC3, cv::Scalar(40, 40, 40));

		/* image for holding current video capture frame */
		cv::Mat frame;

		/* obtain current frame from camera */
		camera >> frame;

		/* Perform MRTD recognition on a current video frame. Remember to set imageIsVideoFrame to true.
		if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
		insert non-NULL function pointers only to those events you are interested in */
		recognizerRecognizeFromRawImage(recognizer, &resultList, frame.data, frame.cols, frame.rows, frame.step, frame.channels() == 3 ? RAW_IMAGE_TYPE_BGR : RAW_IMAGE_TYPE_BGRA, 1, &recognizerCallback);

		recognizerResultListGetNumOfResults(resultList, &numResults);

		/* boolean flag that indicates that a valid result has been found */
		int foundResult = 0;

		if (numResults == 1u) {

			RecognizerResult* result;
			/* obtain the first (and only) result from list */
			recognizerResultListGetResultAtIndex(resultList, 0u, &result);


			int isMrtd = 0;
			/* check if it is a MRTD result */
			status = recognizerResultIsMRTDResult(result, &isMrtd);
			if (status == RECOGNIZER_ERROR_STATUS_SUCCESS && isMrtd) {
				int valid = 0;
				/* check if MRTD result is valid */
				status = recognizerResultIsResultValid(result, &valid);
				if (status == RECOGNIZER_ERROR_STATUS_SUCCESS && valid) {
					/* valid result has been found so set flag to true */
					foundResult = 1;

					const char* doe;
					const char* issuer;
					const char* docNum;
					const char* docCode;
					const char* dob;
					const char* primID;
					const char* secID;
					const char* sex;
					const char* nat;
					const char* opt1;
					const char* opt2;
					/* obtain all fields from result */
					recognizerResultGetMRTDDateOfExpiry(result, &doe);
					recognizerResultGetMRTDIssuer(result, &issuer);
					recognizerResultGetMRTDDocumentNumber(result, &docNum);
					recognizerResultGetMRTDDocumentCode(result, &docCode);
					recognizerResultGetMRTDDateOfBirth(result, &dob);
					recognizerResultGetMRTDPrimaryID(result, &primID);
					recognizerResultGetMRTDSecondaryID(result, &secID);
					recognizerResultGetMRTDSex(result, &sex);
					recognizerResultGetMRTDNationality(result, &nat);
					recognizerResultGetMRTDOpt1(result, &opt1);
					recognizerResultGetMRTDOpt2(result, &opt2);

					const char* raw;
					/* obtain raw lines from result */
					recognizerResultGetMRTDRawStringData(result, &raw);

					/* display results on console window image using OpenCV */
					cv::putText(console, "SUCCESS", cv::Point(200, 100), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(20, 250, 20), 1, CV_AA, false);

					cv::putText(console, primID, cv::Point(200, 145), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, secID, cv::Point(200, 160), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, strcmp("M", sex) == 0 ? "MALE" : "FEMALE", cv::Point(200, 175), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, dateString(dob), cv::Point(200, 190), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);
					cv::putText(console, nat, cv::Point(200, 205), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(220, 220, 220), 1, CV_AA, false);

					cv::putText(console, docCode, cv::Point(200, 235), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, docNum, cv::Point(200, 250), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, dateString(doe, 1), cv::Point(200, 265), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, issuer, cv::Point(200, 280), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, opt1, cv::Point(200, 295), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);
					cv::putText(console, opt2, cv::Point(200, 310), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200, 200, 200), 1, CV_AA, false);

					cv::putText(console, "Press SPACE to reset scanning", cv::Point(200, 345), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(20, 20, 220), 1, CV_AA, false);
				}
			}
		}

		/* if result is not found display default message on console */
		if (!foundResult) {
			cv::putText(console, "Result Not Found", cv::Point(200, 200), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(0, 0, 250), 1, CV_AA, false);
			cv::putText(console, "Press ESCAPE to exit demo", cv::Point(200, 250), cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(250, 250, 250), 1, CV_AA, false);
		}

		/* show camera frame and console image in respective windows */
		cv::imshow("Display window", frame);
		cv::imshow("Text window", console);

		/* read user key presses and delay for 10ms */
		keystroke = (char)cv::waitKey(10); // Wait for a keystroke in the window

		/* reset recognizer if user presses SPACE */
		if (keystroke == KEY_SPACE) {
			recognizerReset(recognizer);
		}
	} while (keystroke != KEY_ESCAPE); // exit loop if user presses ESCAPE

	/* cleanup memory */
	recognizerResultListDelete(&resultList);
	recognizerDeviceInfoDelete(&deviceInfo);
	recognizerSettingsDelete(&settings);
	recognizerDelete(&recognizer);
	recognizerFreeFileBuffer(&ocrModel);

	return 0;
}
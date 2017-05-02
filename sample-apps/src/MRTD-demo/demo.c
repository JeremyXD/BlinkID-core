#include "RecognizerImageWrapper.h"
#include "LicenceKey.h"
#include <RecognizerApi.h>

#include <stdio.h>
#include <stdlib.h>

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
	/* onDetectionMidway is called when some part of object is detected, but detection is not finished yet */
    cb.onDetectionMidway = NULL;
	/* onShowImage is called during recognition process and allows for additional image processing */
	cb.onShowImage = NULL;
	return cb;
}

int main(int argc, char* argv[]) {
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
	/* this variable holds the image sent for image scanning process*/
	RecognizerImageWrapper image;
	/* first result obtained from resultList */
	RecognizerResult* result;
    /* loop counter for images */
    int i;

	if (argc < 3) {
		printf("usage %s <resources_path> <img_path1> <img_path2> ...\n", argv[0]);
		return -1;
	}

	/* create recognizer settings object. Do not forget to delete it after usage. */
	recognizerSettingsCreate(&settings);

	/* define location where resources will be loaded from */
    recognizerSettingsSetResourcesLocation(settings, argv[1]);
	
    /* if you do not plan to use templatingSettings field to scan non-MRZ data, initialize that field to NULL
       (C++ does that automatically) */
	mrtdSettings.templatingSettings = NULL;

	/* add Machine Readable Travel Document recognizer settings to global recognizer settings object */
	recognizerSettingsSetMRTDSettings(settings, &mrtdSettings);

#ifdef LICENCE_KEY
    recognizerSettingsSetLicenseKey( settings, LICENCE_KEY );
#else
    /* insert license key and licensee */
    recognizerSettingsSetLicenseKeyForLicensee( settings, "Add licensee here", "Add license key here" );
    /* OR insert license key for licensee obtained with LicenseRequestTool */
    recognizerSettingsSetLicenseKey( settings, "Add license key here" );
#endif
        
	/* create global recognizer with settings */
	status = recognizerCreate(&recognizer, settings);
    
    /* after recognizer has been created, it is safe to cleanup settings object */
    recognizerSettingsDelete(&settings);
    
	/* as said earlier, all API functions return RecognizerErrorStatus. You can check the status for error, or you can simply ignore it like earlier in this example. */
	if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
		printf("Error creating recognizer: %s\n", recognizerErrorToString(status));
		return -1;
	}

	/* build recognizer callback structure */
	recognizerCallback = buildRecognizerCallback();

    for( i = 2; i < argc; ++i ) {
        image = loadImageFromFile( argv[ i ] ) ;
        if( image.recognizerImage == NULL ) return -1;
        
        /* before passing it to recognizerRecognizeFromImage, you must initialize resultList to empty (C++ does that automatically) */
        resultList.results = NULL;
        resultList.resultsCount = 0;

        /* if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
        insert non-NULL function pointers only to those events you are interested in */
        status = recognizerRecognizeFromImage(recognizer, &resultList, image.recognizerImage, 0, NULL);
        if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
            printf("Error recognizing file %s: %s\n", argv[ i ], recognizerErrorToString(status));
            return -1;
        }

        if (resultList.resultsCount != 1u) {
            /* number of results should be 1 as there is only one recognizer configured */
            printf("Wrong number of recognizer results:" JL_SIZE_T_SPECIFIER "\n", resultList.resultsCount);
            return -1;
        }

        result = resultList.results[0];

        if( recognizerResultIsMRTDResult( result ) ) {
            if( recognizerResultIsResultValid( result ) ) {
                MRTDResult mrtdResult;
                recognizerResultGetMRTDResult( result, &mrtdResult );

                /* display obtained fields */
                printf( "ID is of type %s issued by %s.\nExpiration date is %d.%d.%d.\n", mrtdResult.documentCode, mrtdResult.issuer, mrtdResult.dateOfExpiry.day, mrtdResult.dateOfExpiry.month, mrtdResult.dateOfExpiry.year );
                printf( "ID number is %s.\n", mrtdResult.documentNumber );
                printf( "ID holder is %s %s.\nGender is %s.\n", mrtdResult.primaryID, mrtdResult.secondaryID, mrtdResult.sex );
                printf( "Date of birth is %d.%d.%d\n",  mrtdResult.dateOfBirth.day,  mrtdResult.dateOfBirth.month, mrtdResult.dateOfBirth.year );
                printf( "Nationality is %s.\n", mrtdResult.nationality );
                printf( "Optional fields are:\nOPT1: %s\nOPT2: %s\n", mrtdResult.opt1, mrtdResult.opt2 );

                printf( "Raw result lines:\n%s\n", mrtdResult.rawMRZString );
            }
            else {
                printf( "Invalid result!\n" );
            }
        }
        else {
            /* this should never happen as there is only MRTD recognizer configured */
            printf( "Invalid result type!\n" );
        }

        /* cleanup memory */	
        terminateImageWrapper(&image);
        recognizerResultListDelete(&resultList);
    }
    
    /* cleanup recognizer object */
	recognizerDelete(&recognizer);

	return 0;
}

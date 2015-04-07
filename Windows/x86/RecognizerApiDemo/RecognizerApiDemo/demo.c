#include <stdio.h>
#include <stdlib.h>

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
	return cb;
}

int main(int argc, char* argv[]) {
	/* path will contain path to image being recognized */
	const char* path = argv[1];
	/* this buffer will contain OCR model */
	char* ocrModel;
	/* this variable will contain OCR model buffer length in bytes */
	size_t ocrModelLength;
	/* this variable will contain all recognition settings (which recognizers are enabled, etc.) */
	RecognizerSettings* settings;
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

	if (argc < 2) {
		printf("usage %s <img_path>\n", argv[0]);
		return -1;
	}

	/* load OCR model from file */
	status = recognizerLoadFileToBuffer("ocr_model.zzip", &ocrModel, &ocrModelLength);

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

	/* enable Machine Readable Travel Document recognizer and add it to global recognizer settings object */
	recognizerSettingsEnableMRTD(settings);

	/* insert license key and licensee */
	recognizerSettingsSetLicenseKey(settings, "Add licensee here", "Add license key here");

	/* create global recognizer with settings */
	status = recognizerCreate(&recognizer, settings);
	/* as said earlier, all API functions return RecognizerErrorStatus. You can check the status for error, or you can simply ignore it like earlier in this example. */
	if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
		printf("Error creating recognizer: %s\n", recognizerErrorToString(status));
		return -1;
	}

	/* build recognizer callback structure */
	recognizerCallback = buildRecognizerCallback();
	/* if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
	insert non-NULL function pointers only to those events you are interested in */
	status = recognizerRecognizeFromFile(recognizer, &resultList, path, NULL);
	if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
		printf("Error recognizing file %s: %s\n", path, recognizerErrorToString(status));
		return -1;
	}

	recognizerResultListGetNumOfResults(resultList, &numResults);

	if (numResults != 1u) {
		/* number of results should be 1 as there is only one recognizer configured */
		printf("Wrong number of recognizer results:" JL_SIZE_T_SPECIFIER "\n", numResults);
		return -1;
	}

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
			/* display obtained fields */
			printf("ID is of type %s issued by %s.\nExpiration date is %s.\n", docCode, issuer, doe);
			printf("ID number is %s.\n", docNum);
			printf("ID holder is %s %s.\nDate of birth is %s.\nSex is %s.\n", primID, secID, dob, sex);
			printf("Nationality is %s.\n", nat);
			printf("Optional fields are:\nOPT1: %s\nOPT2: %s\n", opt1, opt2);

			const char* raw;
			/* obtain raw lines from result */
			recognizerResultGetMRTDRawStringData(result, &raw);
			printf("Raw result lines:\n%s\n", raw);
		}
		else {
			printf("Invalid result!\n");
		}
	}
	else {
		/* this should never happen as there is only MRTD recognizer configured */
		printf("Invalid result type!\n");
	}

	/* cleanup memory */
	recognizerResultListDelete(&resultList);
	recognizerDeviceInfoDelete(&deviceInfo);
	recognizerSettingsDelete(&settings);
	recognizerDelete(&recognizer);

	return 0;
}

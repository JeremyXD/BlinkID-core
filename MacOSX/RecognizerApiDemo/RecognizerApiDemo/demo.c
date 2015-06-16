#include <stdio.h>
#include <stdlib.h>

#include "RecognizerApi.h"

// specifier for correct prinf of size_t on 32-bit and 64-bit architectures
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

    if (pointsSize > 0 && points != NULL) {
        for (size_t i = 0; i < pointsSize; ++i) {
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
        case DETECTION_STATUS_CAMERA_TOO_NEAR:
            detStatusDesc = "camera is too near";
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

    /* path to ocr model */
    const char* ocrModelPath = argv[1];

    /* path will contain path to image being recognized */
    const char* path = argv[2];

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

    /* this object will contain image being recognized */
    RecognizerImage* image;

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* recoginzer callback structure contains pointers to functions that will be called during the recognition process */
    RecognizerCallback recognizerCallback;

    /* this variable will contain list of scan results obtained from image scanning process. */
    RecognizerResultList* resultList;

    /* this variable will contain number of scan results obtained from image scanning process. */
    size_t numResults;

    /* this is a for loop counter for iteration over result list */
    size_t i;

    /* buffer with ocr model */
    char* ocrModel;

    // size of the ocr model */
    int bufferSize;
    
    if (argc < 3) {
        printf("usage %s <ocr_model_path> <img_path>\n", argv[0]);
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

    {
        /* create settings for MRTD recognizer */
        MRTDSettings mrtdSettings;

        /* add MRTD recognizer settings to global recognizer settings object */
        recognizerSettingsSetMRTDSettings(settings, &mrtdSettings);
    }

    status = recognizerImageLoadFileToBuffer(ocrModelPath, &ocrModel, &bufferSize);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot load ocr model file at location %s\n", ocrModelPath);
    }

    recognizerSettingsSetZicerModel(settings, ocrModel, bufferSize);

    /* insert license key and licensee */
    /* this specific key is valid until 2015-07-15 */
    recognizerSettingsSetLicenseKey(settings, "MicroBlink", "E4CKM73N-MIAARXO6-T3EIOCPN-2M2A7OZ3-7UVWJQEZ-WAEN3XU6-ZGPMTHWJ-T3EVYEML");
    
    /* create global recognizer with settings */
    status = recognizerCreate(&recognizer, settings);

    /* as said earlier, all API functions return RecognizerErrorStatus. You can check the status for error, or you can simply ignore it like earlier in this example. */
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error creating recognizer: %s\n", recognizerErrorToString(status));
        return -1;
    }

    /* build recognizer callback structure */
    recognizerCallback = buildRecognizerCallback();

    /* create image from file */
    status = recognizerImageCreateFromFile(&image, path);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot create image from file %s\n", path);
        return -1;
    }

    /* if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
       insert non-NULL function pointers only to those events you are interested in */
    status = recognizerRecognizeFromImage(recognizer, &resultList, image, 0, &recognizerCallback);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error recognizing file %s: %s\n", path, recognizerErrorToString(status));
        return -1;
    }

    /* free the image */
    recognizerImageDelete(&image);
    
    recognizerResultListGetNumOfResults(resultList, &numResults);
    
    for(i = 0; i< numResults; ++i) {
        RecognizerResult* result;
        int isMrtd = 0;
        
        /* obtain i-th result from list */
        recognizerResultListGetResultAtIndex(resultList, i, &result);

        status = recognizerResultIsMRTDResult(result, &isMrtd);

        if (status == RECOGNIZER_ERROR_STATUS_SUCCESS && isMrtd) {
            int valid = 0;

            recognizerResultIsResultValid(result, &valid);

            if (valid) {
                const char* primaryId;
                const char* secondaryId;
                const char* dateOfBirth;
                const char* dateOfExpiry;
                const char* documentCode;
                const char* documentNumber;
                const char* issuer;
                const char* nationality;
                const char* opt1;
                const char* opt2;
                const char* sex;

                recognizerResultGetMRTDPrimaryID(result, &primaryId);
                recognizerResultGetMRTDSecondaryID(result, &secondaryId);
                recognizerResultGetMRTDDateOfBirth(result, &dateOfBirth);
                recognizerResultGetMRTDDateOfExpiry(result, &dateOfExpiry);
                recognizerResultGetMRTDDocumentCode(result, &documentCode);
                recognizerResultGetMRTDDocumentNumber(result, &documentNumber);
                recognizerResultGetMRTDIssuer(result, &issuer);
                recognizerResultGetMRTDNationality(result, &nationality);
                recognizerResultGetMRTDOpt1(result, &opt1);
                recognizerResultGetMRTDOpt2(result, &opt2);
                recognizerResultGetMRTDSex(result, &sex);

                printf("MRTD result is:\n\n");
                printf("%s %s\n", primaryId, secondaryId);
                printf("Date of birth: %s\n", dateOfBirth);
                printf("Date of expiry: %s\n", dateOfExpiry);
                printf("Document code %s\n", documentCode);
                printf("Document number %s\n", documentNumber);
                printf("Issuer %s\n", issuer);
                printf("Nationality %s\n", nationality);
                printf("Opt1 %s\n", opt1);
                printf("Opt2 %s\n", opt2);
            } else {
                printf("Mrtd result is not valid!");
            }
        } else {
            printf("Did not get mrtd result!");
        }
        
    }

    recognizerImageFreeFileBuffer(&ocrModel);

    /* cleanup memory */
    recognizerResultListDelete(&resultList);
    recognizerDeviceInfoDelete(&deviceInfo);
    recognizerSettingsDelete(&settings);
    recognizerDelete(&recognizer);

    return 0;
}

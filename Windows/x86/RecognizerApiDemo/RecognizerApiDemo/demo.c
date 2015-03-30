#include <stdio.h>
#include <stdlib.h>

#include "RecognizerApi.h"

/* specifier for correct prinf of size_t on 32-bit and 64-bit architectures */
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
    if(pointsSize>0 && points!=NULL) {
        size_t i;
        for(i=0; i<pointsSize; ++i) {
            printf("Point " JL_SIZE_T_SPECIFIER " location: (%d, %d)\n", i, points[i].x, points[i].y);
        }
    }
    switch(ds) {
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
    /* this is a for loop counter for iteration over result list */
    size_t i;
    
    if (argc < 2) {
        printf("usage %s <img_path>\n", argv[0]);
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
        /* create settings for PDF417 barcode recognizer */
        Pdf417Settings pdf417Sett;
        /* enable the usage of autoscale to automatically determine best parameters for high resolution images */
        pdf417Sett.useAutoScale = 1;
        /* allow scanning of damaged and incomplete PDF417 barcodes */
        pdf417Sett.shouldScanUncertain = 1;
        /* add PDF417 barcode recognizer settings to global recognizer settings object */
        recognizerSettingsSetPdf417Settings(settings, &pdf417Sett);
    }
    
    {
        /* create settings for ZXing barcode recognizer */
        ZXingSettings zxingSett;
        /* activate scanning of QR codes */
        zxingSett.scanQRCode = 1;
        /* add ZXing barcode recognizer settings to global recognizer settings object */
        recognizerSettingsSetZXingSettings(settings, &zxingSett);
    }
    
    {
        /* create settings for US Driver's License barcode recognizer */
        UsdlSettings usdlSett;
        /* enable the usage of autoscale to automatically determine best parameters for high resolution images */
        usdlSett.useAutoScale = 1;
        /* add US Driver's License barcode recognizer settings to global recognizer settings object */
        recognizerSettingsSetUsdlSettings(settings, &usdlSett);
    }
    
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
    status = recognizerRecognizeFromFile(recognizer, &resultList, path, &recognizerCallback);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error recognizing file %s: %s\n", path, recognizerErrorToString(status));
        return -1;
    }
    
    recognizerResultListGetNumOfResults(resultList, &numResults);
    
    for(i = 0; i< numResults; ++i) {
        RecognizerResult* result;
        int isUsdl = 0;
        
        /* obtain i-th result from list */
        recognizerResultListGetResultAtIndex(resultList, i, &result);
        
        /* check if result is obtained from US Driver's License recognizer */
        status = recognizerResultIsUSDLResult(result, &isUsdl);
        if(status == RECOGNIZER_ERROR_STATUS_SUCCESS && isUsdl) {
            int valid = 0;
            
            /* first determine if USDL result is valid */
            recognizerResultIsResultValid(result, &valid);
            if(valid) {
                const char* firstName;
                const char* lastName;
                /* obtain first and last name of the driver */
                recognizerResultGetUSDLField(result, &firstName, USDLFieldKeys.kCustomerFirstName);
                recognizerResultGetUSDLField(result, &lastName, USDLFieldKeys.kCustomerFamilyName);
                
                printf("Driver's license belongs to %s %s\n", firstName, lastName);
            } else {
                printf("Invalid driver license result!\n");
            }
            
        } else {
            const char* str;
            BarcodeType barcodeType;
            
            /* obtain the type of the barcode */
            status = recognizerResultGetBarcodeType(result, &barcodeType);
            
            if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
                printf("Error obtaining barcode type: %s\n", recognizerErrorToString(status));
                return -1;
            }
            
            /* obtain string version of data in barcode */
            status = recognizerResultGetBarcodeStringData(result, &str);
            
            if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
                printf("Error obtaining barcode string data: %s\n", recognizerErrorToString(status));
                return -1;
            }
            
            printf("%s Result: %s\n", barcodeTypeToString(barcodeType), str);
        }
    }

    /* cleanup memory */
    recognizerResultListDelete(&resultList);
    recognizerDeviceInfoDelete(&deviceInfo);
    recognizerSettingsDelete(&settings);
    recognizerDelete(&recognizer);

    return 0;
}

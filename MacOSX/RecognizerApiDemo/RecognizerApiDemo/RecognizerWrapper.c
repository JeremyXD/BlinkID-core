//
//  RecognizerWrapper.c
//  RecognizerApiDemo
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 DoDo. All rights reserved.
//

#include "RecognizerWrapper.h"

#include "RecognizerCallback.h"

RecognizerErrorStatus recognizerWrapperInit(RecognizerWrapper* wrapper, const char* ocrModelPath) {

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* create recognizer settings object. Do not forget to delete it after usage. */
    status = recognizerSettingsCreate(&wrapper->settings);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot create recognizer settings: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* create device info object. Do not forget to delete it after usage. */
    status = recognizerDeviceInfoCreate(&wrapper->deviceInfo);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot create recognizer device info: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* define that device has 4 processors (you can use any number here - this is used to define number
     of threads library will use for its parallel operations */
    status = recognizerDeviceInfoSetNumberOfProcessors(wrapper->deviceInfo, 4);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot set number of processors: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* add device info object to recognizer settings object */
    status = recognizerSettingsSetDeviceInfo(wrapper->settings, wrapper->deviceInfo);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot set device info: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* Load OCR engine model */
    status = recognizerLoadFileToBuffer(ocrModelPath, &wrapper->ocrModel, &wrapper->ocrModelSize);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot load ocr model file at location %s: %s\n", ocrModelPath, recognizerErrorToString(status));
        return status;
    }

    /** Set the OCR engine model. Without OCR model, text cannot be processed */
    status = recognizerSettingsSetZicerModel(wrapper->settings, wrapper->ocrModel, wrapper->ocrModelSize);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot set ocr model: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* insert license key and licensee */
    status = recognizerSettingsSetLicenseKeyForLicensee(wrapper->settings, "add licensee here", "add license key here");
    /* OR insert license key for licensee obtained with LicenseRequestTool (**DO NOT USE BOTH**) */
    status = recognizerSettingsSetLicenseKey(wrapper->settings, "add license key here");
    
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot set license key: %s\n", recognizerErrorToString(status));
        return status;
    }

    /********* INITIALIZE RECOGNIZER SETTINGS ***********/
    /* This determines what will be scanned on images ***/

    /* create settings for MRTD recognizer */
    MRTDSettings mrtdSettings;

    /* add MRTD recognizer settings to global recognizer settings object */
    status = recognizerSettingsSetMRTDSettings(wrapper->settings, &mrtdSettings);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot set MRTD recognizer settings: %s\n", recognizerErrorToString(status));
        return status;
    }

    /********* END INITIALIZE RECOGNIZER SETTINGS *******/

    /* create global recognizer with settings */
    status = recognizerCreate(&wrapper->recognizer, wrapper->settings);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot create recognizer: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* build recognizer callback structure */
    wrapper->recognizerCallback = buildRecognizerCallback();

    /* return success */
    return RECOGNIZER_ERROR_STATUS_SUCCESS;
}

RecognizerErrorStatus recognizerWrapperTerm(RecognizerWrapper* wrapper) {

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    status = recognizerFreeFileBuffer(&wrapper->ocrModel);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot delete ocr model: %s\n", recognizerErrorToString(status));
        return status;
    }
    wrapper->ocrModel = NULL;

    /* cleanup device info */
    recognizerDeviceInfoDelete(&wrapper->deviceInfo);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot delete device info: %s\n", recognizerErrorToString(status));
        return status;
    }
    wrapper->deviceInfo = NULL;

    /* Cleanup recognizer settings */
    recognizerSettingsDelete(&wrapper->settings);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot delete recognizer settings: %s\n", recognizerErrorToString(status));
        return status;
    } else {
        printf("Successfully deleted recognizer settings!\n");
    }
    wrapper->settings = NULL;

    /* Cleanup recognizer */
    recognizerDelete(&wrapper->recognizer);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot delete recognizer: %s\n", recognizerErrorToString(status));
        return status;
    }
    wrapper->recognizer = NULL;

    /* return success */
    return RECOGNIZER_ERROR_STATUS_SUCCESS;
}

RecognizerErrorStatus recognizerWrapperProcessImage(const RecognizerWrapper* wrapper, const RecognizerImage* image,
                                                    RecognizerErrorStatus (*processRecognizerResult)(const RecognizerResult* result)) {

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* this variable will contain list of scan results obtained from image scanning process. */
    RecognizerResultList* resultList;

    /* this variable will contain number of scan results obtained from image scanning process. */
    size_t numResults;

    /* this is a for loop counter for iteration over result list */
    size_t i;

    /* if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
     insert non-NULL function pointers only to those events you are interested in */
    status = recognizerRecognizeFromImage(wrapper->recognizer, &resultList, image, 0, &wrapper->recognizerCallback);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error recognizing image: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* get number of results */
    status = recognizerResultListGetNumOfResults(resultList, &numResults);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error recognizing getting number of results: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* Iterate over results */
    for (i = 0; i < numResults; ++i) {

        RecognizerResult* result;

        /* obtain i-th result from list */
        status = recognizerResultListGetResultAtIndex(resultList, i, &result);
        if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
            printf("Error recognizing getting %d-th result: %s\n", (int) i, recognizerErrorToString(status));
            return status;
        }

        /* process it */
        processRecognizerResult(result);
    }

    /** Clear result list */
    status = recognizerResultListDelete(&resultList);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error deleting result list: %s\n", recognizerErrorToString(status));
        return status;
    }

    return RECOGNIZER_ERROR_STATUS_SUCCESS;
}

/**
 * Method processes image defined at path "path", using provided recognizer and recognizerCallback
 *
 *  @param recognizer         recognizer used for recognition
 *  @param recognizerCallback recognizer callback
 *  @param path               path to image
 */
RecognizerErrorStatus recognizerWrapperProcessImageFromFile(const RecognizerWrapper* wrapper, const char* imagePath,
                                                            RecognizerErrorStatus (*processRecognizerResult)(const RecognizerResult* result)) {

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* result status */
    RecognizerErrorStatus resultStatus = RECOGNIZER_ERROR_STATUS_SUCCESS;

    /* this object will contain image being recognized */
    RecognizerImage* image;

    /* create image from file */
    status = recognizerImageCreateFromFile(&image, imagePath);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot create image from file %s: %s\n", imagePath, recognizerErrorToString(status));
        return status;
    }

    /* perform recognition */

    status = recognizerWrapperProcessImage(wrapper, image, processRecognizerResult);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot process image from file %s: %s\n", imagePath, recognizerErrorToString(status));
        resultStatus = status;
    }

    /* free the image */
    status = recognizerImageDelete(&image);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error freeing the image: %s\n", recognizerErrorToString(status));
        return status;
    }

    return resultStatus;
}

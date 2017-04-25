//
//  RecognizerWrapper.c
//  RecognizerApiDemo
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 DoDo. All rights reserved.
//

#include "RecognizerWrapper.h"
#include "RecognizerImageWrapper.h"
#include "RecognizerCallback.h"


RecognizerErrorStatus recognizerWrapperInit(RecognizerWrapper* wrapper, const char* resourcesPath) {

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* create recognizer settings object. Do not forget to delete it after usage. */
    status = recognizerSettingsCreate(&wrapper->settings);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot create recognizer settings: %s\n", recognizerErrorToString(status));
        return status;
    }
    
    /* define location where resources will be loaded from */
    recognizerSettingsSetResourcesLocation(wrapper->settings, resourcesPath);
    
    /* insert license key and licensee */
    recognizerSettingsSetLicenseKeyForLicensee(wrapper->settings, "add licensee here", "add license key here");
    /* OR insert license key for licensee obtained with LicenseRequestTool (**DO NOT USE BOTH**) */
    recognizerSettingsSetLicenseKey(wrapper->settings, "add license key here");

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

    /* Cleanup recognizer settings */
    status = recognizerSettingsDelete(&wrapper->settings);
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
    RecognizerResultList resultList;

    /* this is a for loop counter for iteration over result list */
    size_t i;

    /* initialise RecognizerResultList's fields (C++ does that automatically) */
    resultList.results = NULL;
    resultList.resultsCount = 0;
    
    /* if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
     insert non-NULL function pointers only to those events you are interested in */
    status = recognizerRecognizeFromImage(wrapper->recognizer, &resultList, image, 0, &wrapper->recognizerCallback);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error recognizing image: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* Iterate over results */
    for (i = 0; i < resultList.resultsCount; ++i) {

        /* obtain i-th result from list */
        RecognizerResult* result = resultList.results[ i ];

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

    /* first use Apple's Image I/O for loading the image from given path */
    RecognizerImageWrapper imageWrapper = loadImageFromFile( imagePath );
    
    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status = RECOGNIZER_ERROR_STATUS_SUCCESS;

    /* perform recognition */

    status = recognizerWrapperProcessImage(wrapper, imageWrapper.recognizerImage, processRecognizerResult);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot process image from file %s: %s\n", imagePath, recognizerErrorToString(status));
    }

    /* free the image */
    terminateImageWrapper( &imageWrapper );

    return status;
}

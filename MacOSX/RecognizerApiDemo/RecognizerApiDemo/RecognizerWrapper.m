//
//  RecognizerWrapper.c
//  RecognizerApiDemo
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 DoDo. All rights reserved.
//

#include "RecognizerWrapper.h"

#include "RecognizerCallback.h"

#import <CoreGraphics/CoreGraphics.h>
#import <Foundation/Foundation.h>

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
    status = recognizerSettingsSetResourcesLocation(wrapper->settings, resourcesPath);
    if( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Failed to set resources location\n" );
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

// taken from https://developer.apple.com/library/content/documentation/GraphicsImaging/Conceptual/ImageIOGuide/imageio_source/ikpg_source.html
static CGImageRef MyCreateCGImageFromFile (NSString* path)
{
    // Get the URL for the pathname passed to the function.
    NSURL *url = [NSURL fileURLWithPath:path];
    CGImageRef        myImage = NULL;
    CGImageSourceRef  myImageSource;
    CFDictionaryRef   myOptions = NULL;
    CFStringRef       myKeys[2];
    CFTypeRef         myValues[2];
    
    // Set up options if you want them. The options here are for
    // caching the image in a decoded form and for using floating-point
    // values if the image format supports them.
    myKeys[0] = kCGImageSourceShouldCache;
    myValues[0] = (CFTypeRef)kCFBooleanTrue;
    myKeys[1] = kCGImageSourceShouldAllowFloat;
    myValues[1] = (CFTypeRef)kCFBooleanTrue;
    // Create the dictionary
    myOptions = CFDictionaryCreate(NULL, (const void **) myKeys,
                                   (const void **) myValues, 2,
                                   &kCFTypeDictionaryKeyCallBacks,
                                   & kCFTypeDictionaryValueCallBacks);
    // Create an image source from the URL.
    myImageSource = CGImageSourceCreateWithURL((CFURLRef)url, myOptions);
    CFRelease(myOptions);
    // Make sure the image source exists before continuing
    if (myImageSource == NULL){
        fprintf(stderr, "Image source is NULL.");
        return  NULL;
    }
    // Create an image from the first item in the image source.
    myImage = CGImageSourceCreateImageAtIndex(myImageSource,
                                              0,
                                              NULL);
    
    CFRelease(myImageSource);
    // Make sure the image exists before continuing
    if (myImage == NULL){
        fprintf(stderr, "Image not created from image source.");
        return NULL;
    }
    
    return myImage;
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
    CGImageRef appleImage = MyCreateCGImageFromFile( [NSString stringWithUTF8String:imagePath] );
    int image_stride = (int) CGImageGetBytesPerRow( appleImage );
    int image_width  = (int) CGImageGetWidth( appleImage );
    int image_height = (int) CGImageGetHeight( appleImage );
    
    // obtaining pixel data from CGImage: http://stackoverflow.com/a/10412292
    CFDataRef rawData = CGDataProviderCopyData( CGImageGetDataProvider( appleImage ) );
    const void* buffer = CFDataGetBytePtr( rawData );
    
    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* result status */
    RecognizerErrorStatus resultStatus = RECOGNIZER_ERROR_STATUS_SUCCESS;

    /* this object will contain image being recognized */
    RecognizerImage* image;

    /* create image from file */
    status = recognizerImageCreateFromRawImage(&image, buffer, image_width, image_height, image_stride, RAW_IMAGE_TYPE_BGRA);
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
    
    CFRelease( rawData );
    CGImageRelease( appleImage );

    return resultStatus;
}

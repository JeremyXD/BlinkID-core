//
//  RecognizerWrapper.c
//  RecognizerApiDemo
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 DoDo. All rights reserved.
//

#include "RecognizerWrapper.h"

#include "RecognizerCallback.h"

#include <wincodec.h>


RecognizerErrorStatus recognizerWrapperInit(RecognizerWrapper* wrapper, const char* resourcesPath) {

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* create recognizer settings object. Do not forget to delete it after usage. */
    status = recognizerSettingsCreate(&wrapper->settings);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot create recognizer settings: %s\n", recognizerErrorToString(status));
        return status;
    }

    /* define path to resource directory */
    recognizerSettingsSetResourcesLocation( wrapper->settings, resourcesPath );

    /* insert license key and licensee */
    status = recognizerSettingsSetLicenseKeyForLicensee(wrapper->settings, "Add licensee here", "Add license key here");
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

    /* required for obtaining WinAPI result codes */
    HRESULT hr = S_OK;
    /* Imaging factory required to create image decoder */
    IWICImagingFactory* pImagingFactory = NULL;
    /* WIC image decoder that will be used for decoding the image */
    IWICBitmapDecoder *pIDecoder = NULL;
    /* WIC image frame - result of image decoding */
    IWICBitmapFrameDecode *pIDecoderFrame = NULL;
    /* wide path that is required for WIC */
    WCHAR* pathString = NULL;
    /* Width and height of the loaded image */
    UINT wicWidth = 0;
    UINT wicHeight = 0;
    /* Image format converter */
    IWICFormatConverter *pIFormatConverter = NULL;
    BYTE* imageBuffer = NULL;

    /* create image from file */
    CoInitialize( NULL );

    /* Use Windows Imaging Component API to load the image from file */
    hr = CoCreateInstance(
        &CLSID_WICImagingFactory,
        NULL,
        CLSCTX_INPROC_SERVER,
        &IID_IWICImagingFactory,
        ( LPVOID* )&pImagingFactory
    );

    if( FAILED( hr ) ) {
        printf( "Failed to load Windows Image Component API\n" );
        return -1;
    }

    /* convert normal path to wide path */
    size_t pathLen = strlen( imagePath );
    pathString = ( WCHAR* )malloc( ( pathLen + 1 ) * sizeof( wchar_t ) );
    mbstowcs( pathString, imagePath, pathLen );
    pathString[ pathLen ] = 0;

    hr = pImagingFactory->lpVtbl->CreateDecoderFromFilename(
        pImagingFactory,
        pathString,                     // Image to be decoded
        NULL,                           // Do not prefer a particular vendor
        GENERIC_READ,                   // Desired read access to the file
        WICDecodeMetadataCacheOnDemand, // Cache metadata when needed
        &pIDecoder                      // Pointer to the decoder
    );

    if( SUCCEEDED( hr ) ) {
        hr = pIDecoder->lpVtbl->GetFrame( pIDecoder, 0, &pIDecoderFrame );
    }
    else {
        printf( "Failed to create decoder from filename!\n" );
        return -1;
    }

    hr = pImagingFactory->lpVtbl->CreateFormatConverter( pImagingFactory, &pIFormatConverter );
    if( SUCCEEDED( hr ) ) {
        hr = pIFormatConverter->lpVtbl->Initialize(
            pIFormatConverter,
            ( IWICBitmapSource * )pIDecoderFrame,                  // Input bitmap to convert
            &GUID_WICPixelFormat32bppPBGRA,  // Destination pixel format
            WICBitmapDitherTypeNone,         // Specified dither pattern
            NULL,                            // Specify a particular palette 
            0.f,                             // Alpha threshold
            WICBitmapPaletteTypeCustom       // Palette translation type
        );
    }
    else {
        printf( "Failed to create pixel format converter\n" );
        return -1;
    }

    /* Obtain image size */
    hr = pIDecoderFrame->lpVtbl->GetSize( pIDecoderFrame, &wicWidth, &wicHeight );

    /* Allocate buffer that will hold decoded pixels */
    imageBuffer = ( BYTE* )malloc( wicWidth * 4 * wicHeight );

    /* Now decode the image */
    hr = pIDecoderFrame->lpVtbl->CopyPixels(
        pIDecoderFrame,
        NULL,
        wicWidth * 4,
        wicWidth * 4 * wicHeight,
        imageBuffer
    );

    if( FAILED( hr ) ) {
        printf( "Failed to decode image\n" );
        return -1;
    }

    status = recognizerImageCreateFromRawImage( &image, imageBuffer, wicWidth, wicHeight, wicWidth * 4, RAW_IMAGE_TYPE_BGRA );

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

    free( pathString );
    free( imageBuffer );
    pIDecoderFrame->lpVtbl->Release( pIDecoderFrame );
    pIDecoder->lpVtbl->Release( pIDecoder );
    pImagingFactory->lpVtbl->Release( pImagingFactory );

    return resultStatus;
}

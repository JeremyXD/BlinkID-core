#include <RecognizerApi.h>

#include <wincodec.h>

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
    printf( "Detection has started!\n" );
}

int onDetectedObject( const MBPoint* points, const size_t pointsSize, MBSize imageSize, MBDetectionStatus ds ) {
    const char* detStatusDesc = "";

    printf( "Detection on image of size %dx%d has finished\n", imageSize.width, imageSize.height );
    printf( "Total " JL_SIZE_T_SPECIFIER " points have been found\n", pointsSize );
    if( pointsSize>0 && points != NULL ) {
        size_t i;
        for( i = 0; i<pointsSize; ++i ) {
            printf( "Point " JL_SIZE_T_SPECIFIER " location: (%d, %d)\n", i, points[ i ].x, points[ i ].y );
        }
    }
    switch( ds ) {
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
    printf( "Detection status: %s\n", detStatusDesc );
    return 1;
}

void onDetectionFailed() {
    printf( "Detection has completely failed\n" );
}

void onRecognitionStarted() {
    printf( "Object recognition has started\n" );
}

void onRecognitionFinished() {
    printf( "Object recognition has finished\n" );
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
    cb.onShowImage = NULL;
    return cb;
}

int main( int argc, char* argv[] ) {
    /* path will contain path to image being recognized */
    const char* path = argv[ 1 ];
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
    RecognizerResultList* resultList;
    /* this variable will contain number of scan results obtained from image scanning process. */
    size_t numResults;
    /* this variable holds the image sent for image scanning process*/
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

    if( argc < 2 ) {
        printf( "usage %s <img_path>\n", argv[ 0 ] );
        return -1;
    }

    /* create recognizer settings object. Do not forget to delete it after usage. */
    recognizerSettingsCreate( &settings );

    /* define path to resources folder */
    status = recognizerSettingsSetResourcesLocation( settings, "res" );
    if( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Could not set path to resources\n" );
        return -1;
    }

    /* add Machine Readable Travel Document recognizer settings to global recognizer settings object */
    recognizerSettingsSetMRTDSettings( settings, &mrtdSettings );

    /* insert license key and licensee */
    recognizerSettingsSetLicenseKeyForLicensee(settings, "Add licensee here", "Add license key here");
    /* OR insert license key for licensee obtained with LicenseRequestTool (**DO NOT USE BOTH**) */
    recognizerSettingsSetLicenseKey(settings, "Add license key here");

    /* create global recognizer with settings */
    status = recognizerCreate( &recognizer, settings );
    /* as said earlier, all API functions return RecognizerErrorStatus. You can check the status for error, or you can simply ignore it like earlier in this example. */
    if( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Error creating recognizer: %s\n", recognizerErrorToString( status ) );
        return -1;
    }

    /* build recognizer callback structure */
    recognizerCallback = buildRecognizerCallback();

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
    size_t pathLen = strlen( path );
    pathString = ( WCHAR* )malloc( ( pathLen + 1 ) * sizeof( wchar_t ) );
    mbstowcs( pathString, path, pathLen );
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

    if( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Error creating image from buffer: %s\n", recognizerErrorToString( status ) );
        return -1;
    }

    /* if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
    insert non-NULL function pointers only to those events you are interested in */
    status = recognizerRecognizeFromImage( recognizer, &resultList, image, 0, NULL );
    if( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Error recognizing file %s: %s\n", path, recognizerErrorToString( status ) );
        return -1;
    }

    recognizerResultListGetNumOfResults( resultList, &numResults );

    if( numResults != 1u ) {
        /* number of results should be 1 as there is only one recognizer configured */
        printf( "Wrong number of recognizer results:" JL_SIZE_T_SPECIFIER "\n", numResults );
        return -1;
    }

    RecognizerResult* result;
    /* obtain the first (and only) result from list */
    recognizerResultListGetResultAtIndex( resultList, 0u, &result );


    if( recognizerResultIsMRTDResult( result ) ) {
        if( recognizerResultIsResultValid( result ) ) {
            MRTDResult mrtdResult;
            recognizerResultGetMRTDResult( result, &mrtdResult );

            /* display obtained fields */
            printf( "ID is of type %s issued by %s.\nExpiration date is %s.\n", mrtdResult.documentCode, mrtdResult.issuer, mrtdResult.dateOfExpiry );
            printf( "ID number is %s.\n", mrtdResult.documentNumber );
            printf( "ID holder is %s %s.\nDate of birth is %s.\nSex is %s.\n", mrtdResult.primaryID, mrtdResult.secondaryID, mrtdResult.dateOfBirth, mrtdResult.sex );
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
    recognizerImageDelete( &image );
    recognizerResultListDelete( &resultList );
    recognizerSettingsDelete( &settings );
    recognizerDelete( &recognizer );

    free( pathString );
    free( imageBuffer );
    pIDecoderFrame->lpVtbl->Release( pIDecoderFrame );
    pIDecoder->lpVtbl->Release( pIDecoder );
    pImagingFactory->lpVtbl->Release( pImagingFactory );

    return 0;
}

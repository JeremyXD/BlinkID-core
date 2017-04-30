/**
 * This demo shows how to use BlinkID Core Components SDK to perform recognition
 * of various documents, which are not supported natively by the SDK. This example
 * will show you how to use Templating API to recognize any card-like document on
 * example of front side Croatian National ID. The example will also show how to
 * setup MRTD recognizer to also perform recognition of non-MRZ fields. As an example,
 * we will use the back side of Croatian National ID.
 */

#include "RecognizerImageWrapper.h"

/** 
 * This header will provide function for configuring BlinkInputSettings to perform 
 * of front side of Croatian National IDs.
 */
#include "TemplateAPICroIDFrontDemo.h"

/** 
 * This header will provide function for configuring MRTDSettings to perform 
 * of back side of Croatian National IDs.
 */
#include "TemplateAPICroIDBackDemo.h"

#include <RecognizerApi.h>

#include <stdio.h>
#include <stdlib.h>

int main( int argc, const char * argv[] ) {
    /*****************  0. Declare and initialize variables ****************/
    
    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;
    
    /* Recognizer that will perform recognition */
    Recognizer* recognizer;
    
    /* Settings object that will configure the Recognizer */
    RecognizerSettings* settings;
        
    /* Settings for BlinkInput recognizer - will be used to configure scanning of front side of the ID */
    BlinkInputSettings blinkInputSettings;

    /* Settings for MRTD recognizer - will be used to configure scanning of back side of the ID */
    MRTDSettings mrtdSettings;
    
    /* image wrapper which knows how to load image from file */
    RecognizerImageWrapper image;
    
    /* this variable will contain list of scan results obtained from image scanning process. */
    RecognizerResultList resultList;
    
    /* this is a for loop counter for iteration over images */
    int i;
    
    
    /*****************************  1. check app usage ********************/
    
    if ( argc < 3 ) {
        printf( "usage %s <resource_path> <img_path1> <img_path2> ...\n", argv[ 0 ] );
        return -1;
    }
    
    /*****************************  2. Init recognizer ********************/
    
    /* create recognizer settings object. Do not forget to delete it after usage. */
    status = recognizerSettingsCreate( &settings );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot create recognizer settings: %s\n", recognizerErrorToString( status ) );
        return -1;
    }
    
    /* define location where resources will be loaded from */
    recognizerSettingsSetResourcesLocation( settings, argv[ 1 ] );
    
    /* insert license key and licensee */
    recognizerSettingsSetLicenseKeyForLicensee( settings, "Add licensee here", "Add license key here" );
    /* OR insert license key for licensee obtained with LicenseRequestTool (**DO NOT USE BOTH**) */
    recognizerSettingsSetLicenseKey( settings, "Add license key here" );
    
    /********* INITIALIZE RECOGNIZER SETTINGS ***********/
    /*** This determines what will be scanned on images ***/

    /* setup BlinkInputSettings for recognition of front side of ID */
    setupBlinkInputSettings( &blinkInputSettings );
    
    /* add BlinkInput recognizer settings to global recognizer settings object */
    status = recognizerSettingsSetBlinkInputSettings( settings, &blinkInputSettings );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot set BlinkInputSettings: %s\n", recognizerErrorToString( status) );
        return -1;
    }
    
    /* BlinkInput recognizer settings can now be freed */
    blinkInputSettingsTerminate( &blinkInputSettings );

    /* setup MRTDSettings for recognition of back side of ID */
    setupMRTDSettings( &mrtdSettings );

    status = recognizerSettingsSetMRTDSettings( settings, &mrtdSettings );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot set MRTDSettings: %s\n", recognizerErrorToString( status) );
        return -1;
    }

    /* MRTD recognizer settings can now be freed */
    mrtdSettingsTerminate( &mrtdSettings );
    /********* END INITIALIZE RECOGNIZER SETTINGS *******/
    
    /* create global recognizer with settings */
    status = recognizerCreate( &recognizer, settings );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot create recognizer: %s\n", recognizerErrorToString( status ) );
        return status;
    }
    
    /* settings object is not required anymore */
    recognizerSettingsDelete( &settings );
    
    for ( i = 2; i < argc; ++i )
    {
        /* loop counter for iteration over results */
        size_t j;
        
        printf( "Performing recognition of %s\n", argv[ i ] );
        
        /* load image using Apple's Image I/O API */
        image = loadImageFromFile( argv[ i ] );
        
        /* 
           initialise RecognizerResultList's fields (C++ does that automatically) - without initialisation, recognizerRecognizeFromImage may crash
           since it tries to free results pointer if it is not NULL.
        */
        resultList.results = NULL;
        resultList.resultsCount = 0;
        
        status = recognizerRecognizeFromImage( recognizer, &resultList, image.recognizerImage, 0, 0 );
        if (status != RECOGNIZER_ERROR_STATUS_SUCCESS)
        {
            printf( "Error recognizing image %s: %s\n", argv[ i ], recognizerErrorToString( status ) );
            continue;
        }
        
        /* Iterate over results */
        for ( j = 0; j < resultList.resultsCount; ++j )
        {
            /* obtain j-th result from list */
            RecognizerResult* result = resultList.results[ j ];
                    
            /* process it */

            /* check if result is valid */
            if ( !recognizerResultIsResultValid( result ) )
            {
                printf( "Result is not valid!\n" );
                continue;
            }
            
            /* check if result is indeed MRTD result */
            if ( recognizerResultIsBlinkInputResult( result ) )
            {
                /* here we will obtain the BlinkInputResult */
                BlinkInputResult blinkInputResult;

                printf( "Result is BlinkInput result!\n" );

                /* obtain BlinkInput result */
                status = recognizerResultGetBlinkInputResult( result, &blinkInputResult );
                if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS )
                {
                    printf( "Error obtaining BlinkInput result: %s\n", recognizerErrorToString( status ) );
                    continue;
                }

                printBlinkInputResult( &blinkInputResult );
            }
            else if ( recognizerResultIsMRTDResult( result ) )
            {
                /* here we will obtain the MRTDResult */
                MRTDResult mrtdResult;

                printf( "Result is MRTD result!\n" );

                /* obtain MRTD result */
                status = recognizerResultGetMRTDResult( result, &mrtdResult );
                if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS )
                {
                    printf( "Error obtaining MRTD result: %s\n", recognizerErrorToString( status ) );
                    continue;
                }

                printMRTDResult( &mrtdResult );
            }
            else
            {
                printf( "Unknown result type\n" );
            }
        }
        
        /* Clear result list */
        status = recognizerResultListDelete( &resultList );
        if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS )
        {
            printf( "Error deleting result list: %s\n", recognizerErrorToString( status ) );
            return -1;
        }
        
        printf( "\n\n" );
        
        /* delete the image */
        terminateImageWrapper( &image );
    }
        
    /* terminate the Recognizer object */
    recognizerDelete( &recognizer );
    
    return 0;
}

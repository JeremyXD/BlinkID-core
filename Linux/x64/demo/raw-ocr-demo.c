#include "RecognizerImageWrapper.h"

#include <RecognizerApi.h>

#include <stdio.h>
#include <stdlib.h>

int main( int argc, const char * argv[] ) {
    /*****************  0. Declare and initialize variables ****************/
    
    /* path will contain path to image being recognized */
    const char* imagePath = argv[1];
    
    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;
    
    /* Recognizer that will perform recognition */
    Recognizer* recognizer;
    
    /* Settings object that will configure the Recognizer */
    RecognizerSettings* settings;
    
    /* Raw parser that will simply convert OCR result to string */
    RawParserSettings rawSett;
    
    /* OCR Engine settings */
    OcrEngineSettings ocrSett;
    
    /* Settings for BlinkInput recognizer */
    BlinkInputSettings blinkInputSettings;
    
    /* image wrapper which knows how to load image from file */
    RecognizerImageWrapper image;
    
    /* this variable will contain list of scan results obtained from image scanning process. */
    RecognizerResultList resultList;
    
    /* this is a for loop counter for iteration over result list */
    size_t i;
    
    /*****************************  1. check app usage ********************/
    
    if ( argc < 2 ) {
        printf( "usage %s <img_path>\n", argv[ 0 ] );
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
    recognizerSettingsSetResourcesLocation( settings, "../libRecognizerApi/res" );
    
    /* insert license key and licensee */
    recognizerSettingsSetLicenseKeyForLicensee( settings, "add licensee here", "add license key here" );
    /* OR insert license key for licensee obtained with LicenseRequestTool (**DO NOT USE BOTH**) */
    recognizerSettingsSetLicenseKey( settings, "add licence key here" );
    
    /********* INITIALIZE RECOGNIZER SETTINGS ***********/
    /* This determines what will be scanned on images ***/
    
    /* create settings for BlinkInput recognizer */

    /* initializing the BlinkInputSettings with blinkInputSettingsInitialize expects all pointers
       to be NULL - if they are not they will first be freed. Since C does not have constructors,
       we need to manually initialize the structure. */
    blinkInputSettings.templatingSettings = NULL;
    blinkInputSettings.detectorSettings = NULL;
    
    /* initialize BlinkInputSettings to be able to add parsers to templatingSettings field */
    status = blinkInputSettingsInitialize( &blinkInputSettings );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot initialize BlinkInputSettings: %s\n", recognizerErrorToString( status ) );
        return -1;
    }
    
    /* setup RawParser with its OCR engine settings */
    
    /* OcrEngineSettings contains lots of fields, so it is best to initialize them to default values */
    ocrEngineSettingsSetDefaultValues( &ocrSett );
    /* disabling colour dropout makes it possible to scan coloured text */
    ocrSett.colourDropoutEnabled = 0;
    /* associate OCR engine settings with raw parser settings */
    rawSett.engineSettings = &ocrSett;
    /* add raw parser to default parser group under name "rawParser" - this name is required to obtain parser result (if required) */
    status = templatingSettingsAddRawParserToDefaultGroup( blinkInputSettings.templatingSettings, "rawParser", &rawSett );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot add raw parser: %s\n", recognizerErrorToString( status ) );
        return -1;
    }
    
    /* add BlinkInput recognizer settings to global recognizer settings object */
    status = recognizerSettingsSetBlinkInputSettings( settings, &blinkInputSettings );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot set BlinkInputSettings: %s\n", recognizerErrorToString( status) );
        return -1;
    }
    
    /* BlinkInput recognizer settings can now be freed */
    blinkInputSettingsTerminate( &blinkInputSettings );
    /********* END INITIALIZE RECOGNIZER SETTINGS *******/
    
    /* create global recognizer with settings */
    status = recognizerCreate( &recognizer, settings );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Cannot create recognizer: %s\n", recognizerErrorToString( status ) );
        return status;
    }
    
    /* settings object is not required anymore */
    recognizerSettingsDelete( &settings );
    
    /* load image using Apple's Image I/O API */
    image = loadImageFromFile( imagePath );
    
    /* initialise RecognizerResultList's fields (C++ does that automatically) - without initialisation, recognizerRecognizeFromImage may crash */
    resultList.results = NULL;
    resultList.resultsCount = 0;
    
    status = recognizerRecognizeFromImage( recognizer, &resultList, image.recognizerImage, 0, 0 );
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error recognizing image: %s\n", recognizerErrorToString(status));
        return status;
    }
    
    /* Iterate over results */
    for (i = 0; i < resultList.resultsCount; ++i) {
        
        /* obtain i-th result from list */
        RecognizerResult* result = resultList.results[ i ];
        
        /* here we will obtain the BlinkInputResult */
        BlinkInputResult blinkInputResult;
        
        /* raw OCR result of default parser group */
        OcrResult rawOcr;
        
        /* loop counters for traversing OCR result */
        size_t blockNum, lineNum, charNum;
        
        /* variable to hold the parsed result of RawOcrParser */
        const char* parsedResult;
        
        /* process it */
        
        /* check if result is indeed MRTD result */
        if ( !recognizerResultIsBlinkInputResult( result ) ) {
            printf( "Result is not BlinkInput result!\n" );
            continue;
        }
        
        /* check if result is valid */
        if ( !recognizerResultIsResultValid( result ) ) {
            printf( "BlinkInput result is not valid!\n" );
            continue;
        }
        
        /* obtain BlinkInput result */
        status = recognizerResultGetBlinkInputResult( result, &blinkInputResult );
        if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
            printf( "Error obtaining BlinkInput result: %s\n", recognizerErrorToString( status ) );
            continue;
        }
        
        /* obtain the raw OCR result */
        status = templatingResultGetOcrResultForDefaultParserGroup( blinkInputResult.templatingResult, &rawOcr );
        if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
            printf( "Error obtaining OCR result for default parser group: %s\n", recognizerErrorToString( status ) );
            continue;
        }
        
        /* print the OCR result */
        printf( "Number of blocks: %d\n", (int) rawOcr.blocksCount );
        for( blockNum = 0; blockNum < rawOcr.blocksCount; ++blockNum ) {
            /* obtain the current block */
            OcrBlock block = rawOcr.blocks[ blockNum ];

            printf( "Block %d\n", (int) blockNum + 1 );
            printf( "Position: (%f, %f, %f, %f)\n", block.position.x, block.position.y, block.position.width, block.position.height );
            printf( "Number of lines: %d\n", (int) block.linesCount );
            for( lineNum = 0; lineNum < block.linesCount; ++lineNum ) {
                OcrLine line = block.lines[ lineNum ];
                
                printf( "Line %d\n", (int) lineNum + 1 );
                printf( "Position: (%f, %f, %f, %f)\n", line.position.x, line.position.y, line.position.width, line.position.height );
                printf( "Number of chars: %d\n", (int) line.charsCount );
                for( charNum = 0; charNum < line.charsCount; ++charNum ) {
                    OcrChar ch = line.chars[ charNum ];
                    printf( "%c", (char) ch.value );
                }
                printf( "\n" );
            }
        }
        
        /* cleanup the OcrResult structure */
        ocrResultDelete( &rawOcr );
        
        /* obtain the parsed result */
        status = templatingResultGetParsedStringFromDefaultParserGroup( blinkInputResult.templatingResult, "rawParser", &parsedResult );
        if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
            printf( "Error obtaining parsed string from default parser group: %s\n", recognizerErrorToString( status ) );
            continue;
        }
        printf( "Parsed result:\n%s\n", parsedResult );
    }
    
    /* Clear result list */
    status = recognizerResultListDelete( &resultList );
    if ( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Error deleting result list: %s\n", recognizerErrorToString( status ) );
        return -1;
    }
    
    /* delete the image */
    terminateImageWrapper( &image );
    
    /* terminate the Recognizer object */
    recognizerDelete( &recognizer );
    
    return 0;
}

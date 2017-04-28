#include "TemplateAPICroIDFrontDemo.h"
#include "TemplateAPIUtils.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * The implementation of the function follows the Templating API
 * whitepaper available here: https://github.com/BlinkID/blinkid-android/blob/master/templatingAPI/templatingAPI.md
 */

/** 
 * helper functions that define required decoding infos and parsers for data extraction
 */

/** function to setup location of first/last name on front side of the ID document and parser which will extract them */
static void setupName( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOld, MBDecodingInfo* decodingInfoNew, int firstName );

/** function to setup location from which gender, citizenship and date of birth will be extracted */
static void setupGenderCitizenshipAndDateOfBirth( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOld, MBDecodingInfo* decodingInfoNew );

/** function to setup locations of document number on both old and new ID - this will aid classification of the document type and also extract the info */
static void setupDocumentNumber( TemplatingSettings* sett, MBDecodingInfo classificationDecodingInfos[2] );

/** function that will be used to classify whether front side of ID document is old or new version of Croatian National ID */
static const char* blinkInputClassifierFunction( const RecognizerResult* result );

/**
 * string constants that will be used for class, parser and parser group names
 */

/** name of both parser group, decoding location and parser that will extract last name */
static const char* ID_LAST_NAME = "LastName";

/** name of both parser group, decoding location and parser that will extract first name */
static const char* ID_FIRST_NAME = "FirstName";

/** name of parser group and decoding location from which gender, citizenship and date of birth will be extracted */
static const char* ID_GENDER_CITIZENSHIP_DOB = "GenderCitizenshipDob";

/** name of parser that will extract gender */
static const char* ID_GENDER = "Gender";

/** name of parser that will extract citizenship */
static const char* ID_CITIZENSHIP = "Citizenship";

/** name of parser that will extract date of birth */
static const char* ID_DATE_OF_BIRTH = "DateOfBirth";

/** name of parser that will extract document number */
static const char* ID_DOCUMENT_NUMBER = "DocumentNumber";

/** name of parser group and decoding location from which document number will be extracted on old ID 
 *  On new IDs, extraction will simply fail and we will take advantage of that in blinkInputClassifierFunction.
 */
static const char* ID_DOCUMENT_NUMBER_OLD = "DocumentNumberOld";

/** name of parser group and decoding location from which document number will be extracted on new ID 
 *  On old IDs, extraction will simply fail and we will take advantage of that in blinkInputClassifierFunction.
 */
static const char* ID_DOCUMENT_NUMBER_NEW = "DocumentNumberNew";

/** name of class denoting the old version of Croatian National ID card */
static const char* CLASS_OLD_ID = "oldCroId";

/** name of class denoting the new version of Croatian National ID card */
static const char* CLASS_NEW_ID = "newCroId";

/**
 * This function will configure given BlinkInputSettings to perform
 * recognition of front side of both old and new version of Croatian
 * National IDs.
 * The implementation of the function follows the Templating API
 * whitepaper available here: https://github.com/BlinkID/blinkid-android/blob/master/templatingAPI/templatingAPI.md
 */
void setupBlinkInputSettings( BlinkInputSettings * blinkInputSett )
{
	/** required for API error handling */
	RecognizerErrorStatus status;

	/** array of decoding infos for old version of ID - allocated on static storage for simplicity, as it must outlive this function's scope */
	static MBDecodingInfo decodingInfosOld[ 3 ];
	/** array of decoding infos for new version of ID - allocated on static storage for simplicity, as it must outlive this function's scope */
	static MBDecodingInfo decodingInfosNew[ 3 ];

    /* set these pointers to NULL to prevent having blinkInputSettingsInitialize freeing them before allocating a new one
       ( this is done automatically in c++ )
     */
    blinkInputSett->detectorSettings   = NULL;
    blinkInputSett->templatingSettings = NULL;
    
	status = blinkInputSettingsInitialize( blinkInputSett );
	if( status != RECOGNIZER_ERROR_STATUS_SUCCESS )
    {
		printf( "Failed to initialize BlinkInput settings\n" );
		exit( -1 );
	}

	/* setup decoding infos, parsers and parser groups */
	setupName( blinkInputSett->templatingSettings, &decodingInfosOld[ 0 ], &decodingInfosNew[ 0 ], 0 );
	setupName( blinkInputSett->templatingSettings, &decodingInfosOld[ 1 ], &decodingInfosNew[ 1 ], 1 );

    setupGenderCitizenshipAndDateOfBirth( blinkInputSett->templatingSettings, &decodingInfosOld[ 2 ], &decodingInfosNew[ 2 ] );

	/* setup document detector */
	{
		/** array of decoding infos used for classification of document */
		/* can be local array, since those decoding infos will be associated with detector settings,
		   and function detectorSettingsCreateFromDocumentDetectorSettings will make internal copies
		   of decoding infos, so the array does not have to outlive its scope 
		 */
		MBDecodingInfo classificationDecodingInfos[ 2 ];
		/** document specification which defines how a specific document should be detected */
		DocumentSpecification documentSpecification;
		/** settings for document detector */
		DocumentDetectorSettings documentDetector;

		/** initialize documentSpecification with default values for detecting card-like documents */
		documentSpecificationInitFromPreset( &documentSpecification, DOCUMENT_SPECIFICATION_PRESET_ID1_CARD );

		/** setup parser decoding infos for classification */
		setupDocumentNumber( blinkInputSett->templatingSettings, classificationDecodingInfos );

		/** override default decoding infos with our classification decoding infos */
        documentSpecification.decodingInfos      = classificationDecodingInfos;
        documentSpecification.decodingInfosCount = 2;

        /** associate document specification with detector settings */
        documentDetector.documentSpecifications      = &documentSpecification;
        documentDetector.documentSpecificationsCount = 1;

        /* since we are recognizing single image, we must set the stable detections threshold to 1,
           otherwise detector will expect multiple video frames to perform detection
           This field is default-initialised in c++ to 1, but not in C.
         */
        documentDetector.numStableDetectionsThreshold = 1;

        /* create DetectorSettings and set it to BlinkInputSettings structure */
        status = detectorSettingsCreateFromDocumentDetectorSettings( &blinkInputSett->detectorSettings, &documentDetector );
		if( status != RECOGNIZER_ERROR_STATUS_SUCCESS )
        {
			printf( "Failed to initialize Detector settings\n" );
			exit( -1 );
		}

		/* now that detector settings have been initialized, let's also allow the flipped recognition - this is useful
		   when dealing with detector which cannot determine correct orientation of document, such as card detector
		   used in this example
		 */
		blinkInputSett->allowFlippedRecognition = 1;
	}

	/* setup classes - function returns status of the operation, but we ignore this here for simplicity */
	templatingSettingsSetParserDecodingInfosForClass( blinkInputSett->templatingSettings, decodingInfosOld, 3, CLASS_OLD_ID );
	templatingSettingsSetParserDecodingInfosForClass( blinkInputSett->templatingSettings, decodingInfosNew, 3, CLASS_NEW_ID );

	/* setup classifier function, also ignore status for simplicity */
	templatingSettingsSetClassifierFunction( blinkInputSett->templatingSettings, &blinkInputClassifierFunction );
}

/**
 * This function will print out all recognized fields from BlinkInput
 * recognition result.
 */
void printBlinkInputResult( BlinkInputResult* blinkInputResult )
{
	/** Will be used for obtaining parsed string values */
	const char* stringVal = NULL;
	/** Will be used for obtaining parsed date values */
	MBDate      dateVal;

    templatingResultGetParsedStringFromParserGroup( blinkInputResult->templatingResult, ID_LAST_NAME, ID_LAST_NAME, &stringVal );
    printf( "Last name: %s\n", stringVal );

    templatingResultGetParsedStringFromParserGroup( blinkInputResult->templatingResult, ID_FIRST_NAME, ID_FIRST_NAME, &stringVal );
    printf( "First name: %s\n", stringVal );

    templatingResultGetParsedStringFromParserGroup( blinkInputResult->templatingResult, ID_GENDER, ID_GENDER_CITIZENSHIP_DOB, &stringVal );
    printf( "Gender: %s\n", stringVal );

    templatingResultGetParsedStringFromParserGroup( blinkInputResult->templatingResult, ID_CITIZENSHIP, ID_GENDER_CITIZENSHIP_DOB, &stringVal );
    printf( "Citizenship: %s\n", stringVal );

    templatingResultGetParsedStringFromParserGroup( blinkInputResult->templatingResult, ID_DOCUMENT_NUMBER, ID_DOCUMENT_NUMBER_OLD, &stringVal );
    printf( "Document number: %s\n", stringVal );

    templatingResultGetParsedDateFromParserGroup( blinkInputResult->templatingResult, ID_DATE_OF_BIRTH, ID_GENDER_CITIZENSHIP_DOB, &dateVal );
    printf( "Date of birth: %d.%d.%d\n", dateVal.day, dateVal.month, dateVal.year );
}

/*** implementation of helper functions, based on android example: https://github.com/BlinkID/blinkid-android/blob/master/BlinkIDDemo/BlinkIDDemo/src/main/java/com/microblink/util/templating/CroatianIDFrontSide.java ***/

/**
 * This function will setup first and last name parser and locations of first and last
 * names on front side of Croatian ID cards.
 *
 * The Croatian ID card has width of 85mm and height of 54mm. If we take a ruler and measure
 * the locations of address field, we get the following measurements:
 * 
 * on old croatian ID card, last name is located in following rectangle:
 * left = 23 mm
 * right = 50 mm
 * top = 11 mm
 * bottom = 17 mm
 * 
 * MBDecodingInfo requires converting this into relative coordinates, so we
 * get the following:
 * 
 * x = 23mm / 85mm = 0.271
 * y = 11mm / 54mm = 0.204
 * width = (50mm - 23mm) / 85mm = 0.318
 * height = (17mm - 11mm) / 54mm = 0.111
 * 
 * on new croatian ID card, last name is located in following rectangle:
 * left = 23 mm
 * right = 54 mm
 * top = 11 mm
 * bottom = 20 mm
 * 
 * MBDecodingInfo requires converting this into relative coordinates, so we
 * get the following:
 * 
 * x = 23mm / 85mm = 0.271
 * y = 11mm / 54mm = 0.204
 * w = (54mm - 23mm) / 85mm = 0.365
 * h = (20mm - 11mm) / 54mm = 0.167
 * 
 * In the same manner we can measure the locations of first name on both old and new ID cards.
 * 
 * Both first and last name can hold a single line of text, but both on new and old ID card
 * first name is printed with smaller font than last name. Therefore, we will require that
 * dewarped image for last names will be of height 100 pixels and for first names of height 150
 * pixels.
 * The width of the image will be automatically determined to keep the original aspect ratio.
 *
 */
static void setupName( TemplatingSettings * sett, MBDecodingInfo * decodingInfoOld, MBDecodingInfo * decodingInfoNew, int firstName )
{
    /**
     * id will be used both for parser name and parser group name. The reason for this is that only one parser
     * will be used on image obtained from required location.
     *
     * An example where multiple parsers are used on same dewarped location can be seen in method
     * setupGenderCitizenshipAndDateOfBirth.
     */
	char const * id = firstName ? ID_FIRST_NAME : ID_LAST_NAME;

	/* setup parser */
	{
		/** settings for parser that will extract first and last name from OCR result */
		RegexParserSettings regexSettings;
		/** settings for OCR engine when performing OCR of names */
		OcrEngineSettings engineSettings;
		/** array of allowed characters in OCR */
		OcrWhitelistEntry* 	whitelist;
		/** size of the whitelist array */
		size_t 				whitelistSize;

		/* initialize engineSettings to default values */
		ocrEngineSettingsSetDefaultValues( &engineSettings );

		whitelist = croatianLettersWhitelist( &whitelistSize, 0 );

        /*
         * To ensure optimal OCR results, we will allow only uppercase letters in OCR. Thus, OCR
         * will not return lowercase letters nor digits, reducing the possibility of misreads.
         */
		engineSettings.whitelist            = whitelist;
		engineSettings.whitelistCount       = whitelistSize;
		engineSettings.colourDropoutEnabled = 0;
        engineSettings.minimumCharHeight    = 20;

        regexSettings.engineSettings = &engineSettings;
        /**
         * For extracting first and last names, we will use regex parser with regular expression which
         * attempts to extract as may uppercase words as possible from single line.
         */
        regexSettings.regex = "([A-ZŠĐŽČĆ]+ ?)+";

        /* also set other values to default (done automatically in c++ constructor) */
        regexSettings.useSieve                = 0;
        regexSettings.mustStartWithWhitespace = 0;
        regexSettings.mustEndWithWhitespace   = 0;

        /** finally, add parser to parser group */
        templatingSettingsAddRegexParserToGroup( sett, id, id, &regexSettings );
	}

	/* setup decoding infos */
	{
        /**
         * Since first name is printed with smaller font, the height of dewarped image containing first name
         * should be larger than height of dewarped image containing last name.
         */
		const int dewarpHeight = firstName ? 150 : 100;

        decodingInfoOld->position.x      = 0.271f;
        decodingInfoOld->position.y      = firstName ? 0.333f : 0.204f;
        decodingInfoOld->position.width  = 0.318f;
        decodingInfoOld->position.height = firstName ? 0.167f : 0.111f;
        decodingInfoOld->dewarpHeight    = dewarpHeight;
        decodingInfoOld->name            = id;

        decodingInfoNew->position.x      = 0.271f;
        decodingInfoNew->position.y      = firstName ? 0.389f : 0.204f;
        decodingInfoNew->position.width  = 0.365f;
        decodingInfoNew->position.height = 0.167f;
        decodingInfoNew->dewarpHeight    = dewarpHeight;
        decodingInfoNew->name            = id;
	}
}

/**
 * This function will setup single location containing gender, citizenship and date of birth and three parsers
 * for these three fields.
 * 
 * Firstly, we need to take a ruler and measure the location from which all these fields will be extracted.
 * On old croatian ID cards, the location containing both gender, citizenship and date of birth is in following
 * rectangle:
 * 
 * left = 35 mm
 * right = 57 mm
 * top = 27 mm
 * bottom = 43 mm
 * 
 * MBDecodingInfo requires converting this into relative coordinates, so we
 * get the following:
 * 
 * x = 35mm / 85mm = 0.412
 * y = 27 mm / 54mm = 0.500
 * w = (57mm - 35mm) / 85mm = 0.259
 * h = (43mm - 27mm) / 54mm = 0.296
 * 
 * Similarly, on new croatian ID card, rectangle holding same information is the following:
 * 
 * left = 33 mm
 * right = 57 mm
 * top = 27 mm
 * bottom = 43 mm
 * 
 * MBDecodingInfo requires converting this into relative coordinates, so we
 * get the following:
 * 
 * x = 33mm / 85mm = 0.388
 * y = 27mm / 54mm = 0.556
 * w = (57mm - 33mm) / 85mm = 0.282
 * h = (43mm - 27mm) / 54mm = 0.296
 * 
 * This location contains three fields in three lines of text. So we will set the height of
 * dewarped image to 300 pixels.
 * The width of the image will be automatically determined to keep the original aspect ratio.
 *
 */
static void setupGenderCitizenshipAndDateOfBirth( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOld, MBDecodingInfo* decodingInfoNew )
{
	/* setup decoding info */
	{
        decodingInfoOld->dewarpHeight    = 300;
        decodingInfoOld->name            = ID_GENDER_CITIZENSHIP_DOB;
        decodingInfoOld->position.x      = 0.407f;
        decodingInfoOld->position.y      = 0.500f;
        decodingInfoOld->position.width  = 0.259f;
        decodingInfoOld->position.height = 0.296f;

        decodingInfoNew->dewarpHeight    = 300;
        decodingInfoNew->name            = ID_GENDER_CITIZENSHIP_DOB;
        decodingInfoNew->position.x      = 0.388f;
        decodingInfoNew->position.y      = 0.500f;
        decodingInfoNew->position.width  = 0.282f;
        decodingInfoNew->position.height = 0.296f;
	}

	/* setup parsers in parser group */

	/* gender parser */
	{
        /**
         * for parsing gender we will use regex parser configured with simple regular expression
         */
        RegexParserSettings genderParser;
		/** settings for OCR engine when performing OCR of gender */
		OcrEngineSettings engineSettings;
		/** array of allowed characters in OCR */
		OcrWhitelistEntry 	whitelist[ 4 ];

        genderParser.regex                   = "[MŽ]/[MF]";
        genderParser.mustEndWithWhitespace   = 1;
        genderParser.mustStartWithWhitespace = 1;
        genderParser.useSieve 				 = 0;

 		/**
         * add possible chars to whitelist.
         *
         * Note that since this parser will be in same parser group with other parsers,
         * final whitelist for OCR will be obtained by merging all whitelists of all
         * parsers in same parser group.
         */
        whitelist[ 0 ].charValue = ( int ) 'M';
        whitelist[ 0 ].charFont  = OCR_FONT_ANY;
        whitelist[ 1 ].charValue = ( int ) 'F';
        whitelist[ 1 ].charFont  = OCR_FONT_ANY;
        whitelist[ 2 ].charValue = ( int ) '/';
        whitelist[ 2 ].charFont  = OCR_FONT_ANY;
        whitelist[ 3 ].charValue = ( int ) L'Ž';
        whitelist[ 3 ].charFont  = OCR_FONT_ANY;

        
        /* initialize engineSettings to default values */       
        ocrEngineSettingsSetDefaultValues( &engineSettings );

        /* set the whitelist */
        engineSettings.whitelist      = whitelist;
        engineSettings.whitelistCount = 4;

        /* associate engine settings with regex parser */
        genderParser.engineSettings = &engineSettings;

        /* this line will add gender parser to parser group: note that parser group
         * name is ID_GENDER_CITIZENSHIP_DOB (same as name od MBDecodingInfo) and name of
         * the parser is ID_GENDER
         */
        templatingSettingsAddRegexParserToGroup( sett, ID_GENDER, ID_GENDER_CITIZENSHIP_DOB, &genderParser );
	}

    /* citizenship parser */
    {
        /**
         * for parsing citizenship we will use regex parser configured with simple regular expression
         */
        RegexParserSettings citizenshipParser;
		/** settings for OCR engine when performing OCR of citizenship */
		OcrEngineSettings citizenshipEngineSettings;
		/** array of allowed characters in OCR */
		OcrWhitelistEntry* 	whitelist;
		/** size of the whitelist array */
		size_t 				whitelistSize;

        whitelist = croatianLettersWhitelist( &whitelistSize, 0 );
        
        ocrEngineSettingsSetDefaultValues( &citizenshipEngineSettings );
        citizenshipEngineSettings.whitelist            = whitelist;
        citizenshipEngineSettings.whitelistCount       = whitelistSize;
        citizenshipEngineSettings.colourDropoutEnabled = 0;

        citizenshipParser.engineSettings          = &citizenshipEngineSettings;
        citizenshipParser.regex                   = "[A-Z]{3}";
        citizenshipParser.mustEndWithWhitespace   = 1;
        citizenshipParser.mustStartWithWhitespace = 1;
        citizenshipParser.useSieve 				  = 0;

        templatingSettingsAddRegexParserToGroup( sett, ID_CITIZENSHIP, ID_GENDER_CITIZENSHIP_DOB, &citizenshipParser );
    }

    /* date of birth parser */
    {
        DateParserSettings dateParser;

        /* initialise structure to default values (done automatically in c++ constructor) */
        dateParser.dateFormats          = NULL;
        dateParser.dateFormatsCount     = 0;
        dateParser.separatorChars       = NULL;
        dateParser.separatorCharsCount  = 0;

        templatingSettingsAddDateParserToGroup( sett, ID_DATE_OF_BIRTH, ID_GENDER_CITIZENSHIP_DOB, &dateParser );
    }

    /**
     * So, all parsers in parser group ID_GENDER_CITIZENSHIP_DOB will be run on OCR result obtained
     * from image taken from location defined by decoding info of the same name
     */
}

/**
 * This function will show how to setup scanning of document number from MBDecodingInfo objects
 * inherent to detector.
 * 
 * First, we will add two possible locations of document number - one location on old Croatian IDs
 * and one location on new Croatian IDs. After that when card-like document will be detected,
 * both locations will be dewarped and OCR and data extraction will be performed. After that,
 * only one parser will succeed in parsing document number - either one on location for old
 * Croatian ID or one on location for new Croatian ID. This information will then be used
 * in blinkInputClassifierFunction to determine whether the scanned document is old or new
 * Croatian ID and appropriate array of MBDecodingInfo objects can then be used
 * to extract other data as set up in functions setupName and setupGenderCitizenshipAndDateOfBirth.
 */
static void setupDocumentNumber( TemplatingSettings* sett, MBDecodingInfo classificationDecodingInfos[2] )
{
	/* setup decoding infos */
	{
        classificationDecodingInfos[ 0 ].dewarpHeight    = 150;
        classificationDecodingInfos[ 0 ].name            = ID_DOCUMENT_NUMBER_OLD;
        classificationDecodingInfos[ 0 ].position.x      = 0.047f;
        classificationDecodingInfos[ 0 ].position.y      = 0.500f;
        classificationDecodingInfos[ 0 ].position.width  = 0.224f;
        classificationDecodingInfos[ 0 ].position.height = 0.130f;

        classificationDecodingInfos[ 1 ].dewarpHeight    = 150;
        classificationDecodingInfos[ 1 ].name            = ID_DOCUMENT_NUMBER_NEW;
        classificationDecodingInfos[ 1 ].position.x      = 0.047f;
        classificationDecodingInfos[ 1 ].position.y      = 0.685f;
        classificationDecodingInfos[ 1 ].position.width  = 0.224f;
        classificationDecodingInfos[ 1 ].position.height = 0.111f;
	}
	/* document number parser */
	{
       /**
         * Document number on Croatian ID is 9-digit number. We will extract that with simple
         * regex parser which only allows digits in OCR engine settings.
         */
        RegexParserSettings documentNumberParser;
		/** settings for OCR engine when performing OCR of document number */
		OcrEngineSettings documentNumberEngineSettings;
		/** array of allowed characters in OCR */
		OcrWhitelistEntry 	whitelist[ 10 ];

        for ( char c = '0'; c <= '9'; ++c ) {
            whitelist[ c ].charValue = ( int ) c;
            whitelist[ c ].charFont  = OCR_FONT_ANY;
        }

        ocrEngineSettingsSetDefaultValues( &documentNumberEngineSettings );
        documentNumberEngineSettings.whitelist            = whitelist;
        documentNumberEngineSettings.whitelistCount       = 10;
        documentNumberEngineSettings.colourDropoutEnabled = 0;

        documentNumberParser.engineSettings          = &documentNumberEngineSettings;
        documentNumberParser.regex                   = "\\d{9}";
        documentNumberParser.mustEndWithWhitespace   = 1;
        documentNumberParser.mustStartWithWhitespace = 1;
        documentNumberParser.useSieve				 = 0;

        /**
         * It is important to add that parser to both parser groups associated with both decoding infos set above.
         */
        templatingSettingsAddRegexParserToGroup( sett, ID_DOCUMENT_NUMBER, ID_DOCUMENT_NUMBER_OLD, &documentNumberParser );
        templatingSettingsAddRegexParserToGroup( sett, ID_DOCUMENT_NUMBER, ID_DOCUMENT_NUMBER_NEW, &documentNumberParser );

	}
}

/** function that will be used to classify whether front side of ID document is old or new version of Croatian National ID */
static const char* blinkInputClassifierFunction( const RecognizerResult* result )
{
    if ( recognizerResultIsBlinkInputResult( result ) )
    {
        BlinkInputResult blinkInputResult;
        const char* documentNumber = NULL;

        recognizerResultGetBlinkInputResult( result, &blinkInputResult );      

 		/* we first check if document number parser has succeeded in
         * parsing document number from location on old Croatian ID (Decoding Info object with
         * name ID_DOCUMENT_NUMBER_OLD defined in method setupDocumentNumber above).
         */
        templatingResultGetParsedStringFromParserGroup( blinkInputResult.templatingResult, ID_DOCUMENT_NUMBER, ID_DOCUMENT_NUMBER_OLD, &documentNumber );
        if ( documentNumber != NULL && strcmp( "", documentNumber) != 0 )
        {
         	/* if document number has been successfully parsed from location unique to old
             * Croatian ID, then classify the document as old Croatian ID
             */
        	return CLASS_OLD_ID;
        }
        else
        {
            templatingResultGetParsedStringFromParserGroup( blinkInputResult.templatingResult, ID_DOCUMENT_NUMBER, ID_DOCUMENT_NUMBER_NEW, &documentNumber );
            if ( documentNumber != NULL && strcmp( "", documentNumber) != 0 )
            {
                /* if document number has been successfully parsed from location unique to new
                 * Croatian ID, then classify the document as new Croatian ID
                 */
            	return CLASS_NEW_ID;
            }
            else
            {
            	/* if this line is reached, then classifier cannot correctly classify the document */
            	return NULL;
            }
        }
    }
    else
    {
    	return NULL;
    }
}

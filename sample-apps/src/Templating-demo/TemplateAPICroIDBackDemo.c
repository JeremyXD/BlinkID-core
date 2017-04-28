#include "TemplateAPICroIDBackDemo.h"
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

/** function to setup location of address on back side of the ID document and parser which will extract the address */
static void setupAddress( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOldID, MBDecodingInfo* decodingInfoNewID );

/** function to setup location of 'issued by' field on back side of the ID document and parser which will extract it */
static void setupIssuedBy( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOldID, MBDecodingInfo* decodingInfoNewID );

/** function to setup location of 'date of issue' field on back side of the ID document and parser which will extract it */
static void setupDateOfIssue( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOldID, MBDecodingInfo* decodingInfoNewID );

/** function that will be used to classify whether back side of ID document is old or new version of Croatian National ID */
static const char* mrtdClassifierFunction( const RecognizerResult* result );

/**
 * string constants that will be used for class, parser and parser group names
 */

/** name of both parser group, decoding location and parser that will extract address */
static const char* ID_ADDRESS = "Address";

/** name of both parser group, decoding location and parser that will extract 'issued by' field */
static const char* ID_ISSUED_BY = "IssuedBy";

/** name of both parser group, decoding location and parser that will extract date of issue */
static const char* ID_DATE_OF_ISSUE = "DateOfIssue";

/** name of class denoting the old version of Croatian National ID card */
static const char* CLASS_OLD_ID = "oldCroId";

/** name of class denoting the new version of Croatian National ID card */
static const char* CLASS_NEW_ID = "newCroId";

/**
 * This function will configure given MRTDSettings to perform
 * recognition of the back side of both old and new version of Croatian
 * National IDs.
 * The implementation of the function follows the Templating API
 * whitepaper available here: https://github.com/BlinkID/blinkid-android/blob/master/templatingAPI/templatingAPI.md
 */
void setupMRTDSettings( MRTDSettings* mrtdSett )
{
	/** required for API error handling */
	RecognizerErrorStatus status;

	/** array of decoding infos for old version of ID - allocated on static storage for simplicity, as it must outlive this function's scope */
	static MBDecodingInfo decodingInfosOld[ 3 ];
	/** array of decoding infos for new version of ID - allocated on static storage for simplicity, as it must outlive this function's scope */
	static MBDecodingInfo decodingInfosNew[ 3 ];

    /* set templating settings to NULL to prevent freeing the pointer in mrtdSettingsInitialize - this is done automatically in c++ */
    mrtdSett->templatingSettings = NULL;
    
    status = mrtdSettingsInitialize( mrtdSett );
	if( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
		printf( "Failed to initialize MRTD settings\n" );
		exit( -1 );
	}

	/* setup decoding infos, parsers and parser groups */
	setupAddress    ( mrtdSett->templatingSettings, &decodingInfosOld[ 0 ], &decodingInfosNew[ 0 ] );
	setupIssuedBy   ( mrtdSett->templatingSettings, &decodingInfosOld[ 1 ], &decodingInfosNew[ 1 ] );
	setupDateOfIssue( mrtdSett->templatingSettings, &decodingInfosOld[ 2 ], &decodingInfosNew[ 2 ] );

	/* setup classes - function returns status of the operation, but we ignore this here for simplicity */
	templatingSettingsSetParserDecodingInfosForClass( mrtdSett->templatingSettings, decodingInfosOld, 3, CLASS_OLD_ID );
	templatingSettingsSetParserDecodingInfosForClass( mrtdSett->templatingSettings, decodingInfosNew, 3, CLASS_NEW_ID );

	/* setup classifier function, also ignore status for simplicity */
	templatingSettingsSetClassifierFunction( mrtdSett->templatingSettings, &mrtdClassifierFunction );
}

/**
 * This function will print out all recognized fields from MRTD
 * recognition result.
 */
void printMRTDResult( MRTDResult* mrtdResult )
{
	/** Will be used for obtaining non-MRZ string data */
	const char* stringVal = NULL;
    /** Will be used for obtainin non-MRZ date of issue */
	MBDate      dateOfIssue;

	/* first obtain the result of classification function */
	templatingResultGetDocumentClassification( mrtdResult->templatingResult, &stringVal );
	printf
	(
		"Document classification result: %s\n",
		stringVal
	);

	/* now print classical part of MRTD result */
    printf
    ( 
    	"ID is of type %s issued by %s.\n"
    	"Expiration date is %d.%d.%d.\n", 
    	mrtdResult->documentCode,
    	mrtdResult->issuer,
    	mrtdResult->dateOfExpiry.day, mrtdResult->dateOfExpiry.month, mrtdResult->dateOfExpiry.year
	);
    printf
    (
    	"ID number is %s.\n", 
    	mrtdResult->documentNumber
    );
    printf
    ( 
    	"ID holder is %s %s.\n"
    	"Gender is %s.\n", 
    	mrtdResult->primaryID, mrtdResult->secondaryID,
    	mrtdResult->sex
    );
    printf
    (
    	"Date of birth is %d.%d.%d\n", 
    	mrtdResult->dateOfBirth.day,  mrtdResult->dateOfBirth.month, mrtdResult->dateOfBirth.year
    );
    printf
    (
    	"Nationality is %s.\n", 
    	mrtdResult->nationality
    );
    printf
    (
    	"Optional fields are:\n"
    	"OPT1: %s\n"
    	"OPT2: %s\n", 
    	mrtdResult->opt1,
    	mrtdResult->opt2
    );
    printf
    (
    	"Raw result lines:\n"
    	"%s\n", 
    	mrtdResult->rawMRZString
    );

    /* finally, print recognizer non-MRZ fields */
    templatingResultGetParsedStringFromParserGroup( mrtdResult->templatingResult, ID_ADDRESS, ID_ADDRESS, &stringVal );
    printf
    (	"ID holder's address is:\n"
    	"%s\n", 
    	stringVal
    );

    templatingResultGetParsedStringFromParserGroup( mrtdResult->templatingResult, ID_ISSUED_BY, ID_ISSUED_BY, &stringVal );
    printf
    (	"ID issued by:%s\n",
    	stringVal
    );

    templatingResultGetParsedDateFromParserGroup( mrtdResult->templatingResult, ID_DATE_OF_ISSUE, ID_DATE_OF_ISSUE, &dateOfIssue );
    printf
    (
    	"Date of issue is %d.%d.%d\n", 
    	dateOfIssue.day,  dateOfIssue.month, dateOfIssue.year
    );
}

/*** implementation of helper functions, based on android example: https://github.com/BlinkID/blinkid-android/blob/master/BlinkIDDemo/BlinkIDDemo/src/main/java/com/microblink/util/templating/CroatianIDBackSide.java ***/

/**
 * This function will setup address parser and location on back side of Croatian ID cards.
 * 
 * The Croatian ID card has width of 85mm and height of 54mm. If we take a ruler and measure
 * the locations of address field, we get the following measurements:
 * 
 * on old croatian ID card, address field is located in following rectangle:
 * left = 21 mm
 * right = 60 mm
 * top = 3 mm
 * bottom = 11 mm
 * 
 * MBDecodingInfo requires converting this rectangle into relative coordinates so we get
 * the following:
 * 
 * x = 21mm / 85mm = 0.247
 * y = 3mm / 54mm = 0.056
 * width = (60mm - 21mm) / 85mm = 0.459
 * height = (11mm - 3mm) / 54mm = 0.148
 * 
 * The address field on old croatian ID cards can hold up to two lines of text. Therefore, we
 * will require that dewarped image from this location has height of 200 pixels.
 * The width of the image will be automatically determined to keep the original aspect ratio.
 * 
 * Similarly, on new croatian ID card, address field is located in following rectangle:
 * left = 21 mm
 * right = 60 mm
 * top = 3 mm
 * bottom = 13 mm
 * 
 * After converting this to relative coordinates, we get the following:
 * 
 * x = 21mm / 8mm5 = 0.247
 * y = 3mm / 54mm = 0.056
 * width = (60mm - 21mm) / 85mm = 0.459
 * height = (13mm - 3mm) / 54mm = 0.185
 * 
 * The address field on new croatian ID cards can hold up to three lines of text. Therefore, we
 * will require that dewarped image from this location has height of 300 pixels.
 * The width of the image will be automatically determined to keep the original aspect ratio.
 */
static void setupAddress( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOldID, MBDecodingInfo* decodingInfoNewID )
{
    /* setup decoding location on both old and new version of document */
    {
	    /* setup decoding info for old ID - see above why these values are chosen */
	    decodingInfoOldID->dewarpHeight    = 200;
	    decodingInfoOldID->name            = ID_ADDRESS;
	    decodingInfoOldID->position.x      = 0.247f;
	    decodingInfoOldID->position.y      = 0.056f;
	    decodingInfoOldID->position.width  = 0.459f;
	    decodingInfoOldID->position.height = 0.148f;

	    /* setup decoding info for new ID - see above why these values are chosen */
	    decodingInfoNewID->dewarpHeight    = 300;
	    decodingInfoNewID->name            = ID_ADDRESS;
	    decodingInfoNewID->position.x      = 0.247f;
	    decodingInfoNewID->position.y      = 0.056f;
	    decodingInfoNewID->position.width  = 0.459f;
	    decodingInfoNewID->position.height = 0.185f;
	}
	/* setup RegexParser which will be used to extract address */
	{
		/** settings for OCR engine when performing OCR of address field */
		OcrEngineSettings 	engineSettings;
		/** settings for parser that will extract address from OCR result */
		RegexParserSettings addressParser;
		/** array of allowed characters in OCR */
		OcrWhitelistEntry* 	whitelist;
		/** size of the whitelist array */
		size_t 				whitelistSize;

		/* initialize engineSettings to default values */
		ocrEngineSettingsSetDefaultValues( &engineSettings );

		whitelist = croatianLettersWhitelist( &whitelistSize, 1 );

        /*
         * To ensure optimal OCR results, we will allow only uppercase letters and digits in OCR. Thus, OCR
         * will not return lowercase letters, reducing the possibility of misreads.
         */
		engineSettings.whitelist            = whitelist;
		engineSettings.whitelistCount       = whitelistSize;
		engineSettings.colourDropoutEnabled = 0;
        engineSettings.minimumCharHeight    = 35;

        addressParser.engineSettings = &engineSettings;
        /*
         * For parsing address, we will use regex parser which expects one or more words in first line (the name of city),
         * and one or more words and a number in second line (street name and number).
         */
        addressParser.regex          = "([A-ZŠĐŽČĆ]+,? ?)+\n([A-ZŠĐŽČĆ]+ ?)+\\d+";

        /* also set other values to default (done automatically in c++ constructor) */
        addressParser.useSieve                = 0;
        addressParser.mustStartWithWhitespace = 0;
        addressParser.mustEndWithWhitespace   = 0;

        /** finally, add parser to parser group */
        templatingSettingsAddRegexParserToGroup( sett, ID_ADDRESS, ID_ADDRESS, &addressParser );
	}
}

/**
 * This function will setup issuing authority parser and location on back side of Croatian ID cards.
 * 
 * The setting up is same as shown in setupAddress,except different regular expression and locations are used.
 */
static void setupIssuedBy( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOldID, MBDecodingInfo* decodingInfoNewID )
{
	/* setup decoding location on both old and new version of document */
	{
		/* setup decoding info for old ID */
        decodingInfoOldID->dewarpHeight    = 100;
        decodingInfoOldID->name            = ID_ISSUED_BY;
        decodingInfoOldID->position.x      = 0.247f;
        decodingInfoOldID->position.y      = 0.204f;
        decodingInfoOldID->position.width  = 0.459f;
        decodingInfoOldID->position.height = 0.111f;

        /* setup decoding info for new ID */
        decodingInfoNewID->dewarpHeight    = 100;
        decodingInfoNewID->name            = ID_ISSUED_BY;
        decodingInfoNewID->position.x      = 0.247f;
        decodingInfoNewID->position.y      = 0.241f;
        decodingInfoNewID->position.width  = 0.459f;
        decodingInfoNewID->position.height = 0.130f;
	}
	/* setup regex parser which will be used to extract 'issued by' field */
	{
		/** settings for OCR engine when performing OCR of 'issued by' field */
		OcrEngineSettings 	engineSettings;
		/** settings for parser that will extract 'issued by' field from OCR result */
		RegexParserSettings issuedByParser;
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

        issuedByParser.engineSettings = &engineSettings;
        /*
         * For parsing address, we will use regex parser which expects one or more words in first line (the name of city),
         * and one or more words and a number in second line (street name and number).
         */
        issuedByParser.regex          = "P[PU] ([A-ZŠĐŽČĆ]+ ?)+";

        /* also set other values to default (done automatically in c++ constructor) */
        issuedByParser.useSieve                = 0;
        issuedByParser.mustStartWithWhitespace = 0;
        issuedByParser.mustEndWithWhitespace   = 0;

        /** finally, add parser to parser group */
        templatingSettingsAddRegexParserToGroup( sett, ID_ISSUED_BY, ID_ISSUED_BY, &issuedByParser );
	}
}

/**
 * This function will setup date of issue parser and location on back side of Croatian ID cards.
 * 
 * The setting up is almost the same as shown in setupAddress, except DateParserSettings is used for 
 * setting up date parser, instead of using Regular Expression parser.
 */
static void setupDateOfIssue( TemplatingSettings* sett, MBDecodingInfo* decodingInfoOldID, MBDecodingInfo* decodingInfoNewID )
{
	/* setup decoding location on both old and new version of document */
	{
        decodingInfoOldID->dewarpHeight    = 100;
        decodingInfoOldID->name            = ID_DATE_OF_ISSUE;
        decodingInfoOldID->position.x      = 0.247f;
        decodingInfoOldID->position.y      = 0.315f;
        decodingInfoOldID->position.width  = 0.282f;
        decodingInfoOldID->position.height = 0.111f;

        decodingInfoNewID->dewarpHeight    = 100;
        decodingInfoNewID->name            = ID_DATE_OF_ISSUE;
        decodingInfoNewID->position.x      = 0.247f;
        decodingInfoNewID->position.y      = 0.370f;
        decodingInfoNewID->position.width  = 0.282f;
        decodingInfoNewID->position.height = 0.093f;
	}
    /* date of issue parser */
    {
        DateParserSettings dateParser;
        /* we need to initialise dateParser to default values - this is done automatically in c++ */
        dateParser.dateFormats 		   = NULL;
        dateParser.dateFormatsCount    = 0;
        dateParser.separatorChars	   = NULL;
        dateParser.separatorCharsCount = 0;

        /** finally, add parser to parser group */
        templatingSettingsAddDateParserToGroup( sett, ID_DATE_OF_ISSUE, ID_DATE_OF_ISSUE, &dateParser );
    }
}

/**
 * This function must decide from RecognizerResult containing data parsed from machine
 * readable zone whether the document being scanned is old or new croatian ID. If document cannot
 * be classified, an empty string or NULL can be returned.
 */
static const char* mrtdClassifierFunction( const RecognizerResult* result )
{
    if( recognizerResultIsMRTDResult( result ) )
    {
        MRTDResult mrtdResult;
        recognizerResultGetMRTDResult( result, &mrtdResult );

        /* if MRZ data is from Croatian ID card, we can discriminate new and old ID card
         * by opt1 field - old croatian ID cards have this field empty,
         * i.e. the value of that field is '<<<<<<<<<<<<<<<'.
         * New ID cards contain personal identification number in this field.
         */

        if( strcmp( mrtdResult.opt1, "<<<<<<<<<<<<<<<" ) == 0 )
        {
            return CLASS_OLD_ID;
        }
        else
        {
            return CLASS_NEW_ID;
        }

    }
    else
    {
        return NULL;
    }
}

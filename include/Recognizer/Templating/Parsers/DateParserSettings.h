/**
 * @file DateParserSettings.h
 *
 * Copyright (c)2017 Microblink Ltd. All rights reserved.
 *
 * ANY UNAUTHORIZED USE OR SALE, DUPLICATION, OR DISTRIBUTION
 * OF THIS PROGRAM OR ANY OF ITS PARTS, IN SOURCE OR BINARY FORMS,
 * WITH OR WITHOUT MODIFICATION, WITH THE PURPOSE OF ACQUIRING
 * UNLAWFUL MATERIAL OR ANY OTHER BENEFIT IS PROHIBITED!
 * THIS PROGRAM IS PROTECTED BY COPYRIGHT LAWS AND YOU MAY NOT
 * REVERSE ENGINEER, DECOMPILE, OR DISASSEMBLE IT.
 */

#ifndef DATE_PARSER_SETTINGS_INCLUDED
#define DATE_PARSER_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/OCR/OcrEngineSettings.h"
#include "Recognizer/Templating/TemplatingSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum DateFormat
 * @brief Available date formats for date parser.
 */
enum DateFormat {
    /** Accepts dates like 18.04.2017. */
    DATE_FORMAT_DDMMYYYY = 0,
    /** Accepts dates like 18.04.17. */
    DATE_FORMAT_DDMMYY,
    /** Accepts dates like 04.18.2017. */
    DATE_FORMAT_MMDDYYYY,
    /** Accepts dates like 04.18.17. */
    DATE_FORMAT_MMDDYY,
    /** Accepts dates like 2017.04.18. */
    DATE_FORMAT_YYYYMMDD,
    /** Accepts dates like 17.04.18. */
    DATE_FORMAT_YYMMDD,
    /** Accepts dates like 18 April 2017 */
    DATE_FORMAT_DDMONTHYYYY,
    /** Accepts dates like 18 April 17 */
    DATE_FORMAT_DDMONTHYY,
    /** Accepts dates like April 18 2017 */
    DATE_FORMAT_MONTHDDYYYY,
    /** Accepts dates like April 18 17 */
    DATE_FORMAT_MONTHDDYY,
    /** Accepts dates like 2017 April 18 */
    DATE_FORMAT_YYYYMONTHDD,
    /** Accepts dates like 2017 April 18 */
    DATE_FORMAT_YYMONTHDD,
    /** denotes number of available date formats */
    DATE_FORMAT_COUNT
};

/**
 * @brief Typedef for DateFormat enumeration
 */
typedef MB_EXPORTED_TYPE enum DateFormat DateFormat;

/**
 * @struct DateParserSettings
 * @brief Structure that represents a parser which can parse date from OCR result.
 */
struct DateParserSettings {
    /**
     * @brief Array of allowed date formats which date parser will take into consideration while parsing the OCR result.
     * If set to NULL, default formats will be taken into consideration, which may lead to slow performance and false positives.
     * Default formats include all formats which use numerical representation of month.
     */
    DateFormat* dateFormats;

    /**
     * @brief Size of dateFormats array.
     */
    size_t dateFormatsCount;

    /**
     * @brief Array of allowed unicode characters which can be interpreted as separators between date parts.
     * If set to NULL, default separators will be used.
     * Default separators are '.', '/' and '-'.
     */
    int* separatorChars;

    /**
     * @brief Size of the separatorChars array.
     */
    size_t separatorCharsCount;

#ifdef __cplusplus
    DateParserSettings() :
        dateFormats( NULL ),
        dateFormatsCount( 0 ),
        separatorChars( NULL ),
        separatorCharsCount( 0 )
    {}
#endif
};

/**
 * @brief Typedef for DateParserSettings structure
 */
typedef MB_EXPORTED_TYPE struct DateParserSettings DateParserSettings;

/**
 * @memberof TemplatingSettings
 * @brief Adds the date parser to the default parser group.
 * After calling this method, it is safe to delete RawParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserSett            Date parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddDateParserToDefaultGroup( TemplatingSettings* templatingSettings, const char* parserName, const DateParserSettings* parserSett );

/**
 * @memberof TemplatingSettings
 * @brief Adds the date parser to the given parser group.
 * After calling this method, it is safe to delete RawParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserGroup           Name of the parser group.
 * @param parserSett            Raw parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddDateParserToGroup( TemplatingSettings* templatingSettings, const char* parserName, const char* parserGroup, const DateParserSettings* parserSett );

#ifdef __cplusplus
}
#endif

#endif

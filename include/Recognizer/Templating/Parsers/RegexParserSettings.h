/**
 * @file RegexParserSettings.h
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

#ifndef REGEX_PARSER_SETTINGS_INCLUDED
#define REGEX_PARSER_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/OCR/OcrEngineSettings.h"
#include "Recognizer/Templating/TemplatingSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct RegexParserSettings
 * @brief Structure that represents a parser which parses OCR result according to given regular expression.
 * Regular expression parsing is performed with custom regex engine which can work with multiple char recognition alternatives
 * (if enabled in OcrEngineSettings). Due to differences between parsing normal strings
 * and OCR results, this parser does not support some regex features found in usual regex engines,
 * like back references.
 */
struct RegexParserSettings {
    /**
     * @brief Defines regex that will be used to parse OCR data. Must not be NULL!
     * Note that not all usual regex features are available, such as back references
     * and '^' and '$' meta-character. '.' meta-character that represents any character
     * and '^' inside brackets representing all characters except those in brackets are
     * available only if alphabet is defined, i.e. engineSettings has non-empty whitelist.
     * Important NOTE: if invalid regex is set, no exception nor error will be risen. Regex expression
     * will be compiled in native code after native library is initialized. Please make sure you
     * watch the contents of log because this is the only place you will find regex parser errors.
     */
    const char* regex;

    /**
     * @brief Pointer to engine options that will be used when processing image.
     * Set to NULL to use default options.
     */
    OcrEngineSettings* engineSettings;

    /**
     * @brief Enable the usage of algorithm for combining consecutive OCR results between video frames for improving OCR quality.
     * By default this is turned off. This has effect only when processing video frames.
     */
    int useSieve;

    /**
     * @brief Defines whether a whitespace is required before matched regular expression.
     * Whitespace is not returned in parsed result, since it is not part of regular expression.
     * It only helps the parsing process.
     */
    int mustStartWithWhitespace;

    /**
     * @brief Defines whether a whitespace is required after matched regular expression.
     * Whitespace is not returned in parsed results, since it is not part of regular expression.
     * It only helps the parsing process.
     */
    int mustEndWithWhitespace;

#ifdef __cplusplus
    RegexParserSettings() :
        regex( NULL ),
        engineSettings( NULL ),
        useSieve( 0 ),
        mustStartWithWhitespace( 0 ),
        mustEndWithWhitespace( 0 )
    {}
#endif
};

/**
 * @brief Typedef for RegexParserSettings structure
 */
typedef MB_EXPORTED_TYPE struct RegexParserSettings RegexParserSettings;

/**
 * @memberof TemplatingSettings
 * @brief Adds the regex parser to the default parser group.
 * After calling this method, it is safe to delete RegexParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserSett            Raw parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddRegexParserToDefaultGroup( TemplatingSettings* templatingSettings, const char* parserName, const RegexParserSettings* parserSett );

/**
 * @memberof TemplatingSettings
 * @brief Adds the regex parser to the given parser group.
 * After calling this method, it is safe to delete RegexParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserGroup           Name of the parser group.
 * @param parserSett            Raw parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddRegexParserToGroup( TemplatingSettings* templatingSettings, const char* parserName, const char* parserGroup, const RegexParserSettings* parserSett );

#ifdef __cplusplus
}
#endif

#endif

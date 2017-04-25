/**
 * @file RawParserSettings.h
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

#ifndef RAW_PARSER_SETTINGS_INCLUDED
#define RAW_PARSER_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/OCR/OcrEngineSettings.h"
#include "Recognizer/Templating/TemplatingSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct RawParserSettings
 * @brief Structure that represents a parser which simply returns the string version of OCR result, without performing any smarter parse operations.
 */
struct RawParserSettings {
    /**
     * @brief Enable the usage of algorithm for combining consecutive OCR results between video frames for improving OCR quality.
     * By default this is turned off. This has effect only when processing video frames.
     */
    int useSieve;
    /**
     * @brief Pointer to engine options that will be used when processing image.
     * Set to NULL to use default options.
     */
    OcrEngineSettings* engineSettings;

#ifdef __cplusplus
    RawParserSettings() :
        useSieve( 0 ),
        engineSettings( NULL ) {}
#endif
};

/**
 * @brief Typedef for RawParserSettings structure.
 */
typedef MB_EXPORTED_TYPE struct RawParserSettings RawParserSettings;

/**
 * @memberof TemplatingSettings
 * @brief Adds the raw parser to the default parser group.
 * After calling this method, it is safe to delete RawParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserSett            Raw parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddRawParserToDefaultGroup( TemplatingSettings* templatingSettings, const char* parserName, const RawParserSettings* parserSett );

/**
 * @memberof TemplatingSettings
 * @brief Adds the raw parser to the given parser group.
 * After calling this method, it is safe to delete RawParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserGroup           Name of the parser group.
 * @param parserSett            Raw parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddRawParserToGroup( TemplatingSettings* templatingSettings, const char* parserName, const char* parserGroup, const RawParserSettings* parserSett );

#ifdef __cplusplus
}
#endif

#endif

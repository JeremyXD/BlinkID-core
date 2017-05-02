/**
 * @file IbanParserSettings.h
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

#ifndef IBAN_PARSER_SETTINGS_INCLUDED
#define IBAN_PARSER_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/OCR/OcrEngineSettings.h"
#include "Recognizer/Templating/TemplatingSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct IbanParserSettings
 * @brief Structure that represents a parser which can extract IBAN (International Bank Account Number) from OCR result.
 */
struct IbanParserSettings {
    /**
     * @brief Array of strings representing allowed country codes. If set to NULL, all IBANs can be extracted.
     * You can use this whitelist to restrict which IBANs (from which countries) can be extracted from the OCR
     * result. Having a restriction can improve recognition quality, as it reduces the possibility of mis-recognising
     * the IBAN.
     */
    const char** countryCodesWhitelist;

    /**
     * @brief Size of the countryCodesWhitelist array.
     */
    size_t countryCodesWhitelistCount;

#ifdef __cplusplus
    IbanParserSettings() :
        countryCodesWhitelist( NULL ),
        countryCodesWhitelistCount( 0 )
    {}
#endif
};

/**
 * @brief Typedef for IbanParserSettings structure
 */
typedef MB_EXPORTED_TYPE struct IbanParserSettings IbanParserSettings;

/**
 * @memberof TemplatingSettings
 * @brief Adds the IBAN parser to the default parser group.
 * After calling this method, it is safe to delete IbanParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserSett            Raw parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddIbanParserToDefaultGroup( TemplatingSettings* templatingSettings, const char* parserName, const IbanParserSettings* parserSett );

/**
 * @memberof TemplatingSettings
 * @brief Adds the IBAN parser to the given parser group.
 * After calling this method, it is safe to delete IbanParserSettings, since internal copy of the settings will be made.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of the parser.
 * @param parserGroup           Name of the parser group.
 * @param parserSett            Raw parser settings.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsAddIbanParserToGroup( TemplatingSettings* templatingSettings, const char* parserName, const char* parserGroup, const IbanParserSettings* parserSett );

#ifdef __cplusplus
}
#endif

#endif

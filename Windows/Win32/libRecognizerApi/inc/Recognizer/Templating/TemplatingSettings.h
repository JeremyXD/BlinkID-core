/**
 * @file TemplatingSettings.h
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

#ifndef TEMPLATING_SETTINGS_INCLUDED
#define TEMPLATING_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/Detector/DecodingInfo.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerResultFwd.h"
#include "Recognizer/RecognizerSettingsFwd.h"
#include "TemplatingSettingsFwd.h"

#include "Parsers/RawParserSettings.h"
#include "Parsers/DateParserSettings.h"
#include "Parsers/RegexParserSettings.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @memberof TemplatingSettings
 * @brief                           Remove parser with given name from parser group of given name.
 * @param templatingSettings        Pointer to templating settings structure that will be modified.
 * @param parserName                Name of parser to be removed.
 * @param parserGroupName           Name of parser group from which parser should be removed.
 * @return                          status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsRemoveParserFromParserGroup( TemplatingSettings* templatingSettings, const char* parserName, const char* parserGroupName );

/**
 * @memberof TemplatingSettings
 * @brief                       Remove parser with given name from default parser group.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserName            Name of parser to be removed.
 * @return                      status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsRemoveParserFromDefaultGroup( TemplatingSettings* templatingSettings, const char* parserName );

/**
 * @memberof TemplatingSettings
 * @brief                       Remove all parsers from parser group of given name - effectively remove the parser group.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param parserGroupName       Name of parser group to be removed.
 * @return                      status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsRemoveAllParsersFromParserGroup( TemplatingSettings* templatingSettings, const char* parserGroupName );

/**
 * @memberof TemplatingSettings
 * @brief                       Remove all parsers from all parser groups.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @return                      status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsRemoveAllParserGroups( TemplatingSettings* templatingSettings );

/**
 * @memberof TemplatingSettings
 * @brief                       Sets the list of decoding infos for default class.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param decodingInfos         Array of decoding infos that will be set. The actual decoding infos are copied, so it is safe to delete the array afterwards.
 * @param decodingInfosSize     Size of decodingInfos array.
 * @return status               Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsSetParserDecodingInfos( TemplatingSettings* templatingSettings, const MBDecodingInfo* decodingInfos, size_t decodingInfosSize );

/**
 * @memberof TemplatingSettings
 * @brief                       Sets the list of decoding infos for given class.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param decodingInfos         Array of decoding infos that will be set. The actual decoding infos are copied, so it is safe to delete the array afterwards.
 * @param decodingInfosSize     Size of decodingInfos array.
 * @param className             Name of the class for which decoding infos will be set.
 * @return status               Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsSetParserDecodingInfosForClass( TemplatingSettings* templatingSettings, const MBDecodingInfo* decodingInfos, size_t decodingInfosSize, const char* className );

/**
 * @memberof TemplatingSettings
 * @brief                       Removes the list of decoding infos for given class.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param className             Name of the class for which decoding infos will be removed.
 * @return status               Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsRemoveParserDecodingInfosForClass( TemplatingSettings* templatingSettings, const char* className );

/**
 * @memberof TemplatingSettings
 * @brief                       Removes all decoding infos for all classes.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @return status               Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsRemoveAllParserDecodingInfos( TemplatingSettings* templatingSettings );

/**
 *  Document classifier function which can be used to classify document based on classification decoding infos (either given or inherent to detector).
 *  For each document type that you want to support, returned result string has to be equal to the name of the corresponding class of MBDecodingInfo
 *  objects which are defined for that document type. Named decoding info classes should be defined using templatingSettingsSetParserDecodingInfosForClass function.
 */
typedef const char* (*ClassifierFunction)( const RecognizerResult* recognitionResult );

/**
 * @memberof TemplatingSettings
 * @brief                       Sets the classifier function used to classify document.
 * This functions should be used if multiple document types having same physical size are expected
 * and additional information (elements) of interest should be recognized. In addition, decoding
 * infos for each document type should be set with function templatingSettingsSetParserDecodingInfosForClass.
 * For each document type, name of the decoding info set must be equal to corresponding classifier result.
 * @param templatingSettings    Pointer to templating settings structure that will be modified.
 * @param classifier            Classifier function. Set to NULL do disable already existing classifier function.
 * @return status               Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingSettingsSetClassifierFunction( TemplatingSettings* templatingSettings, ClassifierFunction classifier );

#ifdef __cplusplus
}
#endif

#endif

/**
 * @file MRTDSettings.h
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

#ifndef MRTD_SETTINGS_INCLUDED
#define MRTD_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerSettingsFwd.h"
#include "Recognizer/Templating/TemplatingSettings.h"

#ifdef __cplusplus
#include <cstdlib>
extern "C" {
#endif

/**
* @struct MRTDSettings
* @brief Settings for configuring Machine Readable Travel Document recognizer.
* MRTD recognizer can detect machine readable zone in the image and extract data from it.
* Besides that, it can also perform recognition of non-MRZ data on known locations
* relative to document location.
* For more information about how to use that, check [Android documentation](https://github.com/BlinkID/blinkid-android#extracting-additional-fields-of-interest-from-machine-readable-travel-documents-templating-api) -
* the only difference is that MRTDDocumentClassifier in C API is actually a ClassifierFunction defined in TemplatingSettings.h
* To learn how to use Templating API to also scan non-MRZ data on example, check [this article](https://github.com/BlinkID/blinkid-android/blob/master/templatingAPI/templatingAPI.md#-templating-api-for-mrtd)
* and also check demo apps provided with the SDK.
*/
struct MRTDSettings {
    /**
    * Turns on calling the showImage callback with image of dewarped MRZ.
    * You can use this and showFullDocument simultaneously.
    */
    int showMachineReadableZone;

    /**
    * Turns on calling the showImage callback with image of dewarped document.
    * You can use this and showMachineReadableZone simultaneously.
    */
    int showFullDocument;

    /**
     * Turns on encoding of full document image into MRTDResult. If enabled,
     * fields fullDocumentJpegBuffer and fullDocumentBufferSize can be used
     * to obtain the image data.
     */
    int encodeFullDocumentImage;

    /**
     * Turns on encoding of machine readable zone image into MRTDResult. If enabled,
     * fields mrzImageJpegBuffer and mrzImageBufferSize can be used to obtain the
     * image data.
     */
    int encodeMachineReadableZoneImage;

    /**
     * If enabled, raw MRZ text will be returned for MRZ which does not have
     * standardised form of three lines each with 30 characters
     * or two lines each with 36 or 44 characters
     */
    int allowUnparsedResults;

    /**
     * Allow processing of mrz in which one or more check digits aren't correct.
     */
    int allowUnverifiedResults;

    /**
     * Pointer to additional templating settings that can be used to perform recognition
     * of non-MRZ data. If you are only interested in MRZ data, set this to NULL, othervise
     * use mrtdSettingsInitialize to initialize it.
     * When done, you must terminate it with mrtdSettingsTerminate.
     */
    TemplatingSettings* templatingSettings;

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    MRTDSettings() :
        showMachineReadableZone(0),
        showFullDocument(0),
        encodeFullDocumentImage(0),
        encodeMachineReadableZoneImage(0),
        allowUnparsedResults(0),
        allowUnverifiedResults(0),
        templatingSettings( NULL ) {}
#endif
};

/**
 * @brief Typedef for MRTDSettings structure.
 */
MB_EXPORTED_TYPE typedef struct MRTDSettings MRTDSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set settings for Machine Readable Travel Document recognizer.
 * A copy of MRTDSettings will be made, so it is safe to terminate the MRTDSettings structure with mrtdSettingsTerminate function.
 * @param settings pointer to MRTDSettings object that holds MRTD settings information
 * @param mrtdSettings pointer to MRTD recognizer settings object that contains information how MRTD recognizer should behave or NULL to disable MRTD recognizer
 * @return status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetMRTDSettings(RecognizerSettings* settings, const MRTDSettings* mrtdSettings);

/**
 * @memberof MRTDSettings
 * @brief Creates and initializes TemplatingSettings inside MRTDSettings.
 * TemplatingSettings can be used to define parameters for performing recognition of non-MRZ data on the document.
 * @param mrtdSettings MRTDSettings where TemplatingSettings will be initialised.
 * @return status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL mrtdSettingsInitialize( MRTDSettings* mrtdSettings );

/**
 * @memberof MRTDSettings
 * @brief Frees resources allocated with mrtdSettingsInitialize. You can safely call this function after recognizerSettingsSetMRTDSettings.
 * @param mrtdSettings MRTDSettings where TemplatingSettings will be terminated.
 * @return status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL mrtdSettingsTerminate( MRTDSettings* mrtdSettings );

#ifdef __cplusplus
}
#endif

#endif

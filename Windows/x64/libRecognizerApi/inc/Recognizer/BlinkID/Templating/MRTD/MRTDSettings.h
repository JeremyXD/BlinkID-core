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

#ifdef __cplusplus
extern "C" {
#endif

/**
* @struct MRTDSettings
* @brief Settings for configuring Machine Readable Travel Document recognizer.
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
        allowUnverifiedResults(0) {}
#endif
};

/**
 * @brief Typedef for MRTDSettings structure.
 */
MB_EXPORTED_TYPE typedef struct MRTDSettings MRTDSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set settings for Machine Readable Travel Document recognizer.
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param mrtdSettings pointer to MRTD recognizer settings object that contains information how MRTD recognizer should behave or NULL to disable MRTD recognizer
 * @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetMRTDSettings(RecognizerSettings* settings, const MRTDSettings* mrtdSettings);

#ifdef __cplusplus
}
#endif

#endif

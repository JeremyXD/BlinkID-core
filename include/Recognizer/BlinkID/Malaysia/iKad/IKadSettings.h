/**
 * @file IkadSettings.h
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

#ifndef IKAD_SETTINGS_INCLUDED
#define IKAD_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @struct IKadSettings
* @brief Settings for configuring IKad recognizer. */
struct IKadSettings {

    /**
    * Turns on calling the showImage callback with image of dewarped document.
    */
    int showFullDocument;

    /**
     * Turns on calling the showImage callback with image of the document holder's face.
     */
    int showFaceImage;

    /**
     * Turns on extraction of passport number.
     */
    int extractPassportNumber;

    /**
     * Turns on expiry date extraction
     */
    int extractExpiryDate;

    /**
     * Turns on sector extraction.
     */
    int extractSector;

    /**
     * Turns on employer extraction.
     */
    int extractEmployer;

    /**
     * Turns on address extraction.
     */
    int extractAddress;

    /**
     * Turns on nationality extraction.
     */
    int extractNationality;

    /**
     * Turns on sex extraction.
     */
    int extractSex;

#ifdef __cplusplus
    /**
    * Default constructor for c++.
    */
    IKadSettings() :
        showFullDocument( 0 ),
        showFaceImage( 0 ),
        extractPassportNumber( 1 ),
        extractExpiryDate( 1 ),
        extractSector( 1 ),
        extractEmployer( 1 ),
        extractAddress( 1 ),
        extractNationality( 1 ),
        extractSex( 1 )
    { }

#endif
};

/**
 * @brief Typedef for IKadSettings structure.
 */
MB_EXPORTED_TYPE typedef struct IKadSettings IKadSettings;

/**
* @memberof RecognizerSettings
* @brief Set settings for Malaysian immigrator ID card recognizer.
* @param settings pointer to RecognizerSettings object that holds all settings information
* @param ikadSettings pointer to IKad recognizer settings object that contains information how IKad recognizer should behave or NULL to disable IKad recognizer
* @return status of the operation.
*/
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetIKadSettings(RecognizerSettings* settings, const IKadSettings* ikadSettings);

#ifdef __cplusplus
}
#endif

#endif

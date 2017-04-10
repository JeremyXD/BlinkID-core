/**
 * @file MyKadSettings.h
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

#ifndef MYKAD_SETTINGS_INCLUDED
#define MYKAD_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
* @struct MyKadSettings
* @brief Settings for configuring MyKad recognizer. */
struct MyKadSettings {

    /**
    * Turns on calling the showImage callback with image of dewarped document.
    */
    int showFullDocument;

    /**
     * Turns on calling the showImage callback with image of the document holder's face.
     */
    int showFaceImage;

#ifdef __cplusplus
    /**
    * Default constructor for c++.
    */
    MyKadSettings() : showFullDocument( 0 ), showFaceImage( 0 ) { }

#endif
};

/**
 * @brief Typedef for MyKadSettings structure.
 */
MB_EXPORTED_TYPE typedef struct MyKadSettings MyKadSettings;

/**
* @memberof RecognizerSettings
* @brief Set settings for Malaysian ID card recognizer.
* @param settings pointer to RecognizerSettings object that holds all settings information
* @param mykadSettings pointer to MyKad recognizer settings object that contains information how MyKad recognizer should behave or NULL to disable MyKad recognizer
* @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
*/
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetMyKadSettings(RecognizerSettings* settings, const MyKadSettings* mykadSettings);

#ifdef __cplusplus
}
#endif

#endif

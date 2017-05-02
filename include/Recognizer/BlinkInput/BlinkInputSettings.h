/**
 * @file BlinkInputSettings.h
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
#ifndef BLINK_INPUT_SETTINGS_INCLUDED
#define BLINK_INPUT_SETTINGS_INCLUDED

#include "Recognizer/Detector/DetectorSettings.h"
#include "Recognizer/Export.h"
#include "Recognizer/Templating/TemplatingSettings.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct BlinkInputSettings
 * @brief Settings for configuring BlinkInput recognizer.
 * BlinkInput recognizer can be used to perform generic OCR of images,
 * OCR+data extraction from images or detection+OCR+data extraction from images of documents.
 *
 * For more information about parser groups, check [Android documentation](https://github.com/BlinkID/blinkid-android#blinkOCR)
 * (it describes the same concepts, but using Java API, which is transcribed in this C API).
 *
 * For more information about scanning templated documents, also check
 * [Android documentation](https://github.com/BlinkID/blinkid-android#blinkOCR_templating) - the only difference is
 * that DocumentClassifier in C API is actually a ClassifierFunction defined in TemplatingSettings.h
 * To learn how to use Templating API to scan generic documents on example, check [Android documentation](https://github.com/BlinkID/blinkid-android/blob/master/templatingAPI/templatingAPI.md#-templating-api-for-generic-documents)
 * and also check demo apps provided with the SDK.
 *
 */
struct BlinkInputSettings {
    /**
     * Pointer to TemplatingSettings part of BlinkInputSettings. You must initialise this field
     * by calling blinkInputSettingsInitialize.
     */
    TemplatingSettings* templatingSettings;

    /**
     * @brief Settings for detector to be used before performing the OCR and data extraction.
     * Sets detector settings that will define which detector will be used.
     * If set to NULL, no detector will be used and OCR will be performed on entire
     * scanning region.
     * If set to valid detector, first this detector will be used to detect an object and
     * then its MBDecodingInfo objects will be consulted
     * to perform dewarping of defined locations and parsers from parser group having the same
     * name as dewarped location will be used to extract OCR data.
     */
    DetectorSettings* detectorSettings;

    /**
     * @brief Allow recognition of flipped detection (only if detectorSettings is not NULL).
     * Set this to non-zero to enable recognition of document also in upside down direction.
     * This is useful if detector being used cannot determine correct orientation of detected
     * document (for example DocumentDetector represented with DocumentDetectorSettings structure).
     * Keep in mind that enabling this feature will make recognition two times slower
     * and possibly less accurate. By default, this is turned off.
     */
    int allowFlippedRecognition;
#ifdef __cplusplus
    BlinkInputSettings() :
        templatingSettings( NULL ),
        detectorSettings( NULL ),
        allowFlippedRecognition( 0 )
    {}
#endif
};

/**
 * @brief Typedef for BlinkInputSettings structure.
 */
typedef MB_EXPORTED_TYPE struct BlinkInputSettings BlinkInputSettings;

/**
 * @memberof BlinkInputSettings
 * @brief Creates and initialises TemplatingSettings inside BlinkInputSettings and initialises all other fields to default values.
 * NOTE: This method will initialise detectorSettings field to NULL, so make sure to set DetectorSettings *AFTER* intialising the
 * structure with this function.
 * @param blinkInputSett    Pointer to BlinkInputSettings object that holds recognition information.
 * @return status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL blinkInputSettingsInitialize( BlinkInputSettings* blinkInputSett );

/**
 * @memberof BlinkInputSettings
 * @brief Frees resources allocated with blinkInputSettingsInitialize. You can safely call this function after recognizerSettingsSetBlinkInputSettings.
 * NOTE: This function will also release resources associated with detectorSettings pointer, so you are not required to explicitely release it with
 * detectorSettingsTerminate function.
 * @param blinkInputSett    Pointer to BlinkInputSettings object that holds recognition information.
 * @return status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL blinkInputSettingsTerminate( BlinkInputSettings* blinkInputSett );

/**
 * @memberof RecognizerSettings
 * @brief Set settings for BlinkInput recognizer.
 * A copy of BlinkInputSettings will be made, so it is safe to terminate the BlinkInputSettings structure with blinkInputSettingsTerminate function.
 * @param recognizerSettings    pointer to RecognizerSettings object that holds all settings information.
 * @param blinkInputSettings    pointer to BlinkInputSettings object that contains information how BlinkInput recognizer should behave or NULL to disable BlinkInput recognizer.
 * @return status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetBlinkInputSettings( RecognizerSettings* recognizerSettings, const BlinkInputSettings* blinkInputSettings );

#ifdef __cplusplus
}
#endif

#endif

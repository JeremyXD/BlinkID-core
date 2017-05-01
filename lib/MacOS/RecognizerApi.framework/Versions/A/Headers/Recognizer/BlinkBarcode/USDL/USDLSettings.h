/**
 * @file USDLSettings.h
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

#ifndef USDL_SETTINGS_INCLUDED
#define USDL_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct UsdlSettings
 * @brief Settings for configuring US Driver's License recognizer.
 */
struct UsdlSettings {
    /**
     * Allow scanning barcodes which don't have quiet zone surrounding it (e.g. text
     * concatenated with barcode). This option can drastically reduce recognition time and
     * can even reduce the recognition quality of barcodes that do have quiet zone
     * surrounding it.
     */
    int nullQuietZoneAllowed;
    /**
     * Allow scanning of uncertain PDF417 barcodes. Uncertain scanning refers to returning result even on barcodes
     * which checksum does not match. Ideal for incomplete or damaged barcodes.
     */
    int shouldScanUncertain;
    /**
     * Allow enabling the autodetection of image scale when scanning barcodes. Enabling this can improve recognition
     * quality of high resolution images, at the cost of additional processing time.
     */
    int useAutoScale;

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    UsdlSettings() :
        nullQuietZoneAllowed(0),
        shouldScanUncertain(0),
        useAutoScale(0) {}
#endif
};

/**
 * @brief Typedef for UsdlSettings structure.
 */
MB_EXPORTED_TYPE typedef struct UsdlSettings UsdlSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set US Driver's License recognizer settings to global settings object.
 * USDL recognizer settings structure defines how the US Driver's license recognition
 * will be performed. Set NULL to disable US Driver's License recognizer.
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param usdlSettings pointer to USDL recognizer settings object that contains information about USDL recognition or NULL to disable US Driver's License recognizer
 * @return  status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetUsdlSettings(RecognizerSettings* settings, const UsdlSettings* usdlSettings);


#ifdef __cplusplus
}
#endif

#endif

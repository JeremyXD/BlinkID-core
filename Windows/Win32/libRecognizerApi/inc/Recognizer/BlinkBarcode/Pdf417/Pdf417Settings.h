/**
 * @file Pdf417Settings.h
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

#ifndef PDF417_SETTINGS_INCLUDED
#define PDF417_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct Pdf417Settings
 * @brief Settings for configuring PDF417 recognizer.
 */
struct Pdf417Settings {
    /**
     * Enables scanning of barcodes with inverse intensity values (e.g. white barcode on black background)
     */
    int shouldScanInverse;
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

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    Pdf417Settings() :
        shouldScanInverse(0),
        nullQuietZoneAllowed(0),
        shouldScanUncertain(0) {}
#endif
};

/**
 * @brief Typedef for Pdf417Settings structure.
 */
MB_EXPORTED_TYPE typedef struct Pdf417Settings Pdf417Settings;

/**
 * @memberof RecognizerSettings
 * @brief Set PDF417 recognizer settings to global settings object.
 * PDF417 recognizer settings structure defines how the PDF417 recognition will be performed. Set NULL to disable PDF417 recognizer.
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param pdf417Settings pointer to PDF417 recognizer settings object that contains information about PDF417 recognition or NULL to disable PDF417 recognizer
 * @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetPdf417Settings(RecognizerSettings* settings, const Pdf417Settings* pdf417Settings);

#ifdef __cplusplus
}
#endif

#endif

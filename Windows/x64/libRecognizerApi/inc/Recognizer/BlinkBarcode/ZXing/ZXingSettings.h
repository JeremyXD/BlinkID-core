/**
 * @file ZXingSettings.h
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

#ifndef ZXING_SETTINGS_INCLUDED
#define ZXING_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct ZXingSettings
 * @brief Settings for configuring ZXing implementation of barcode scanners.
 */
struct ZXingSettings {
    /**
     * Enables scanning of barcodes with inverse intensity values (e.g. white barcode on black background)
     */
    int shouldScanInverse;
    /**
     * Use this flag to enable reading of Aztec 2D barcodes.
     */
    int scanAztec;
    /**
     * Use this flag to enable reading of Code 128 1D barcodes.
     */
    int scanCode128;
    /**
     * Use this flag to enable reading of Code 39 1D barcodes.
     */
    int scanCode39;
    /**
     * Use this flag to enable reading of DataMatrix 2D barcodes.
     */
    int scanDataMatrix;
    /**
     * Use this flag to enable reading of EAN 13 1D barcodes.
     */
    int scanEAN13;
    /**
     * Use this flag to enable reading of EAN 8 1D barcodes.
     */
    int scanEAN8;
    /**
     * Use this flag to enable reading of ITF 1D barcodes.
     */
    int scanITF;
    /**
     * Use this flag to enable reading of QR codes.
     */
    int scanQRCode;
    /**
     * Use this flag to enable reading of UPC A 1D barcodes.
     */
    int scanUPCA;
    /**
     * Use this flag to enable reading of UPC E 1D barcodes.
     */
    int scanUPCE;
    /**
     * Use this flag to use slower, but more thorough algorithm for scanning barcodes.
     */
    int slowThoroughScan;

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    ZXingSettings() :
        shouldScanInverse(0),
        scanAztec(0),
        scanCode128(0),
        scanCode39(0),
        scanDataMatrix(0),
        scanEAN13(0),
        scanEAN8(0),
        scanITF(0),
        scanQRCode(0),
        scanUPCA(0),
        scanUPCE(0),
        slowThoroughScan(1) {}
#endif
};

/**
 * @brief Typedef for ZXingSettings structure.
 */
MB_EXPORTED_TYPE typedef struct ZXingSettings ZXingSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set settings for ZXing barcode recognizer.
 * You should use ZXing barcode recognizer for all barcodes that Microblink does not have implementation for. ZXingSettings structure
 * defines how the ZXing barcode recognizer should behave. Set NULL to disable ZXing barcode recognizer.
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param zxingSettings pointer to ZXing barcode recognizer settings object that contains information how ZXing barcode recognizer should behave or NULL to disable ZXing barcode recognizer
 * @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetZXingSettings(RecognizerSettings* settings, const ZXingSettings* zxingSettings);

#ifdef __cplusplus
}
#endif

#endif

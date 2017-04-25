/**
 * @file BarDecoderSettings.h
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

#ifndef BARDECODER_SETTINGS_INCLUDED
#define BARDECODER_SETTINGS_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerSettingsFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct BarDecoderSettings
 * @brief Settings for configuring Microblink's implementation of 1D barcode scanner.
 */
struct BarDecoderSettings {
    /**
     * Enables scanning of barcodes with inverse intensity values (e.g. white barcode on black background)
     */
    int shouldScanInverse;
    /**
     * Use this flag to use more processing time for barcode detection and decoding. With this flag enabled, better recognition quality can be expected.
     */
    int tryHarder;
    /**
     * Allow enabling the autodetection of image scale when scanning barcodes. Enabling this can improve recognition
     * quality of high resolution images, at the cost of additional processing time.
     */
    int useAutoScale;
    /**
     * Use this flag to enable reading of Code 39 1D barcodes.
     */
    int scanCode39;
    /**
     * Use this flag to enable reading of Code 128 1D barcodes.
     */
    int scanCode128;

#ifdef __cplusplus
    /** Default constructor for c++ */
    BarDecoderSettings() :
        shouldScanInverse(0),
        tryHarder(0),
        useAutoScale(0),
        scanCode39(0),
        scanCode128(0) {}
#endif
};

/**
 * @brief Typedef for BarDecoderSettings structure.
 */
MB_EXPORTED_TYPE typedef struct BarDecoderSettings BarDecoderSettings;

/**
 * @memberof RecognizerSettings
 * @brief Set settings for 1D barcode recognizer (Microblink's implementation).
 * BarDecodeSettings structure defines how the 1D barcode recognizer (Microblink's implementation) will behave.
 * Set NULL to disable 1D barcode recognizer (Microblink's implementation).
 * @param settings pointer to RecognizerSettings object that holds all settings information
 * @param barDecoderSettings pointer to 1D barcode recognizer (Microblink's implementation) settings object that contains information how 1D barcode recognizer will behave or NULL
 *                           to disable 1D barcode recognizer (Microblink's implementation)
 * @return status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsSetBarDecoderSettings(RecognizerSettings* settings, const BarDecoderSettings* barDecoderSettings);

#ifdef __cplusplus
}
#endif

#endif

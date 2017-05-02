/**
 * @file RecognizerSettingsFwd.h
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
#ifndef RECOGNIZERSETTINGSFWD_H
#define RECOGNIZERSETTINGSFWD_H

#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct RecognizerSettings
 * @brief Recognizer settings data structure
 * A Recognizer is object which orchestrates image recognition. It combines detection of objects (documents, barcodes, etc.) with
 * OCR/barcode decoding and data extraction. Each Recognizer is specific to extracting single type of information from the image.
 * For example, USDL recognizer (@see USDLSettings.h) is specialized for searching PDF417 barcodes on US driver's licenses and
 * extracting data from them. On the other hand, Machine Readable Travel Document (MRTD) recognizer (@see MRTDSettings.h) is
 * specialized for searching Machine Readable Zone in the image and extracting data from it. Please browse the documentation
 * to see all specific available recognizers and how to configure them.
 * @see Recognizer.h
 */
struct RecognizerSettings;

/**
 * @typedef RecognizerSettings
 * @brief Typedef for RecognizerSettings structure.
 */
typedef MB_EXPORTED_TYPE struct RecognizerSettings RecognizerSettings;

#ifdef __cplusplus
}
#endif

#endif

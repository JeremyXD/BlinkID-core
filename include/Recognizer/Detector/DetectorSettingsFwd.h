/**
 * @file DetectorSettingsFwd.h
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
#ifndef DETECTORSETTINGSFWD_H
#define DETECTORSETTINGSFWD_H

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct DetectorSettings
 * @brief Structure that defines settings for a detector.
 * A detector is object that can detect one or more objects in the image and present them
 * to recognizer which then uses the detection result to perform OCR and data extraction.
 */
struct DetectorSettings;

/**
 * @brief Typedef for DetectorSettings structure
 */
typedef MB_EXPORTED_TYPE struct DetectorSettings DetectorSettings;

#ifdef __cplusplus
}
#endif

#endif

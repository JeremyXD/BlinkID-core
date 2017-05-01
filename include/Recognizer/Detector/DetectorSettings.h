/**
 * @file DetectorSettings.h
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
#ifndef DETECTORSETTINGS_H
#define DETECTORSETTINGS_H

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "DetectorSettingsFwd.h"

#include "DocumentDetector/DocumentDetectorSettings.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @memberof DetectorSettings
 * @brief Releases resources associated with given DetectorSettings structure and sets given pointer to NULL.
 * @param detectorSettings Pointer to pointer to DetectorSettings structure which will be terminated.
 * @return status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL detectorSettingsTerminate( DetectorSettings** detectorSettings );

#ifdef __cplusplus
}
#endif

#endif

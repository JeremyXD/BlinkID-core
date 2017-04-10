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
 @struct RecognizerSettings
 @brief Recognizer settings data structure

 Used for initializing the Recognizer object.
 @see Recognizer.h
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

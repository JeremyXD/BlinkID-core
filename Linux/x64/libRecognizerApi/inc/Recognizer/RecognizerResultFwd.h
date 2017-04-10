/**
 * @file RecognizerResultFwd.h
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
#ifndef RECOGNIZERRESULTFWD_H
#define RECOGNIZERRESULTFWD_H

#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 @struct RecognizerResult
 @brief RecognizerResult data structure. Holds the results of the recognition.

 You can use various getter methods to obtain results of the recognition or to inspect the type of recognizer result.
 */
struct RecognizerResult;

/**
 * @brief Typedef for RecognizerResult structure.
 */
typedef MB_EXPORTED_TYPE struct RecognizerResult RecognizerResult;

#ifdef __cplusplus
}
#endif

#endif

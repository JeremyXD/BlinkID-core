/**
 * @file BarDecoderResult.h
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

#ifndef BARDECODER_RESULT_INCLUDED
#define BARDECODER_RESULT_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/BlinkBarcode/BarcodeResult.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @memberof RecognizerResult
 * @brief Returns non-zero value if given result is generated by 1D barcode recognizer (Microblink's implementation).
 * @param result RecognizerResult object
 * @return non-zero value if given result is generated by 1D barcode recognizer, zero otherwise
 */
MB_API int MB_CALL recognizerResultIsBardecoderResult( const RecognizerResult* result );

#ifdef __cplusplus
}
#endif

#endif

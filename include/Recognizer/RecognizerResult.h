/**
 * @file RecognizerResult.h
 *
 * Copyright (c)2015 MicroBlink Ltd. All rights reserved.
 *
 * ANY UNAUTHORIZED USE OR SALE, DUPLICATION, OR DISTRIBUTION
 * OF THIS PROGRAM OR ANY OF ITS PARTS, IN SOURCE OR BINARY FORMS,
 * WITH OR WITHOUT MODIFICATION, WITH THE PURPOSE OF ACQUIRING
 * UNLAWFUL MATERIAL OR ANY OTHER BENEFIT IS PROHIBITED!
 * THIS PROGRAM IS PROTECTED BY COPYRIGHT LAWS AND YOU MAY NOT
 * REVERSE ENGINEER, DECOMPILE, OR DISASSEMBLE IT.
 */

#ifndef RECOGNIZERRESULT_H_
#define RECOGNIZERRESULT_H_

#include "RecognizerError.h"
#include "Export.h"
#include "RecognizerResultFwd.h"

#include "Recognizer/BlinkBarcode/Pdf417/Pdf417Result.h"
#include "Recognizer/BlinkBarcode/BarDecoder/BarDecoderResult.h"
#include "Recognizer/BlinkBarcode/ZXing/ZXingResult.h"
#include "Recognizer/BlinkBarcode/USDL/USDLResult.h"
#include "Recognizer/BlinkID/Malaysia/MyKad/MyKadResult.h"
#include "Recognizer/BlinkID/Malaysia/iKad/IKadResult.h"
#include "Recognizer/BlinkID/MRTD/MRTDResult.h"
#include "Recognizer/BlinkInput/BlinkInputResult.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @memberof RecognizerResult
 * @brief Obtain whether given recognizer result is empty.
 * Example:
 * @code
RecognizerResult *result;
int empty = recognizerResultIsResultEmpty( result );
if(empty) {
    // handle empty data
} else {
    // handle non-empty data
}
 * @endcode
 *
 * @param   result          RecognizerResult object which stores the values of the results.
 * @return  Non-zero value if result is empty, zero if result is not empty.
 *
 */
MB_API int MB_CALL recognizerResultIsResultEmpty( const RecognizerResult* result );

/**
* @memberof RecognizerResult
* @brief Obtain whether given recognizer result is valid.
* Example:
* @code
RecognizerResult *result;
int valid = recognizerResultIsResultValid( result );
if(valid) {
    // handle valid data
} else {
    // handle invalid data
}
* @endcode
*
* @param result RecognizerResult object which stores the values of the results.
* @return Non-zero value if result is valid, zero if result is non-valid
*
*/
MB_API int MB_CALL recognizerResultIsResultValid( const RecognizerResult* result );

#ifdef __cplusplus
}
#endif

#endif

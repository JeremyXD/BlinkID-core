/**
 * @file RecognizerError.h
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

#ifndef RECOGNIZERERROR_H_
#define RECOGNIZERERROR_H_

#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 @enum RecognizerErrorStatus
 @brief Enumeration of all possible error statuses
 */
enum RecognizerErrorStatus {
    /** Operation was successfully completed */
    RECOGNIZER_ERROR_STATUS_SUCCESS = 0,
    
    /** Operation failed */
    RECOGNIZER_ERROR_STATUS_FAIL = 1,
    
    /** Unknown key was used for the operation */
    RECOGNIZER_ERROR_STATUS_UNKNOWN_KEY = 2,
    
    /** Invalid type was used in the operation */
    RECOGNIZER_ERROR_STATUS_INVALID_TYPE = 3,

    /** Invalid or expired license key was provided */
    RECOGNIZER_ERROR_STATUS_INVALID_LICENSE_KEY = 4,

    /** One of required pointer inputs is NULL */
    RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL = 5,

    /** Index is out of range */
    RECOGNIZER_ERROR_STATUS_INDEX_OUT_OF_RANGE = 6
    
};

/**
 * @brief Typedef for RecognizerErrorStatus enum.
 */
typedef MB_EXPORTED_TYPE enum RecognizerErrorStatus RecognizerErrorStatus;

/**
 Helper method for fetching string representation of the error status. Usually used in debugging.
 
 @param errorStatus   status for which the string representation is required
 @return null terminated string for required status
 */
MB_API const char* MB_CALL recognizerErrorToString(RecognizerErrorStatus errorStatus);

#ifdef __cplusplus
}
#endif

#endif

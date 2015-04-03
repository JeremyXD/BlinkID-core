/**
 *  @file RecognizerResultList.h
 *  Created on: 23/05/2014
 *      Author: dodo
 *
 * Copyright (c)2014 Racuni.hr d.o.o. All rights reserved.
 *
 * ANY UNAUTHORIZED USE OR SALE, DUPLICATION, OR DISTRIBUTION
 * OF THIS PROGRAM OR ANY OF ITS PARTS, IN SOURCE OR BINARY FORMS,
 * WITH OR WITHOUT MODIFICATION, WITH THE PURPOSE OF ACQUIRING
 * UNLAWFUL MATERIAL OR ANY OTHER BENEFIT IS PROHIBITED!
 * THIS PROGRAM IS PROTECTED BY COPYRIGHT LAWS AND YOU MAY NOT
 * REVERSE ENGINEER, DECOMPILE, OR DISASSEMBLE IT.
 */

#ifndef RECOGNIZERRESULTLIST_HPP
#define RECOGNIZERRESULTLIST_HPP

#include <stdlib.h>

#include "RecognizerResult.h"
#include "RecognizerError.h"
#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct RecognizerResultList
 * RecognizerResultList data structure. Holds all results of the recognition.
 * Some images may contain more than one recognizable objects, e.g. two types of barcodes.
 * To obtain specific RecognizerResult, use the function recognizerResultListGetResultAtIndex(int).
 * To get the number of results in the list, use function recognizerResultListGetNumOfResults().
 * Results are sorted by quality - the best result is the first (index 0). Note that there is no
 * defined order for results of same quality.
 **/
typedef PP_EXPORTED_TYPE struct RecognizerResultList RecognizerResultList;

/**
  @memberof RecognizerResultList
  @brief Gets the number of results stored in RecognizerResultList.

  Example:
  @code
    int numElements;
    recognizerResultListGetNumOfResults(resultList, &numElements);
  @endcode

  @param     resultList     Result list that will be queried for number of elements.
  @param     numElements    [out] destination where result will be stored
  @return    errorStatus    status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs. Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if any of inputs is NULL.
*/
PP_API RecognizerErrorStatus PP_CALL recognizerResultListGetNumOfResults(const RecognizerResultList* resultList, size_t* numElements);

/**
  * @memberof RecognizerResultList
  * @brief Returns the result stored at given index in RecognizerResultList.
  * Example:
  * @code
  * RecognizerResult* result = NULL;
  * RecognizerErrorStatus status = recognizerResultListGetResultAtIndex(resultList, 0, &result);
  * if(status == RECOGNIZER_ERROR_STATUS_SUCCESS) {
  *     // use the data
  * } else {
  *     // something bad has happened
  * }
  * @endcode
  *
  * @param      resultList     Result list that will be queried.
  * @param      index           Index of the element to be returned.
  * @param      result          [out] destination where result will be stored. On failure, result will be set to NULL.
  * @return     errorStatus     status of the operation. On success, RECOGNIZER_ERROR_STATUS_SUCCESS will be returned. If resultList or result is NULL,
  *                             RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL will be returned. If index is negative or out of range, RECOGNIZER_ERROR_STATUS_INDEX_OUT_OF_RANGE
  *                             will be returned.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerResultListGetResultAtIndex(const RecognizerResultList* resultList, size_t index, RecognizerResult** result);

/**
 @memberof RecognizerResultList
 @brief Deletes the RecognizerResultList object, together with all internal results, and sets the pointer to it to NULL.
 Example:
 @code
    RecognizerResultList *resultList = NULL;
    RecognizerErrorStatus status = recognizerRecognizeFromEncodedImage(recognizer, &resultList, file);
    recognizerResultListDelete(&resultList);
 @endcode

 @param     resultList    Double Pointer to the RecognizerResultList object which is to be deleted
 @return    errorStatus status of the operation. If resultList is NULL, return RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise return RECOGNIZER_ERROR_STATUS_SUCCESS.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerResultListDelete(RecognizerResultList** resultList);

#ifdef __cplusplus
}
#endif

#endif

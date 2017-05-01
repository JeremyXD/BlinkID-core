/**
 *  @file RecognizerResultList.h
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

#ifndef RECOGNIZERRESULTLIST_HPP
#define RECOGNIZERRESULTLIST_HPP

#include "Export.h"
#include "RecognizerResultFwd.h"
#include "RecognizerError.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct RecognizerResultList
 * @brief RecognizerResultList data structure. Holds all results of the recognition.
 *
 * Some images may contain more than one recognizable objects, e.g. two types of barcodes.
 * The results array contains all results that were recognized and is allocated by the function
 * recognizerRecognizeFromImage and should be released with function recognizerResultListDelete.
 * Results are sorted by quality - the best result is the first (index 0). Note that there is no
 * defined order for results of same quality.
 **/
struct RecognizerResultList {
    /**
     * @brief Array of results returned by recognition process. If NULL, nothing was recognized.
     */
    RecognizerResult** results;

    /**
     * @brief Size of results array.
     */
    size_t resultsCount;

#ifdef __cplusplus
    RecognizerResultList() :
        results( NULL ),
        resultsCount( 0 )
    {}
#endif
};

/**
 * @brief Typedef for RecognizerResultList structure.
 */
typedef MB_EXPORTED_TYPE struct RecognizerResultList RecognizerResultList;

/**
 @memberof RecognizerResultList
 @brief Deletes all internal results inside RecognizerResultList structure.
 @param     resultList    Pointer to the RecognizerResultList object which is to be deleted
 @return    status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerResultListDelete( RecognizerResultList* resultList );

#ifdef __cplusplus
}
#endif

#endif

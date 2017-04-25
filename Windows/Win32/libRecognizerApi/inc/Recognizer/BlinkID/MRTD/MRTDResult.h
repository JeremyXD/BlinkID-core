/**
 * @file MRTDResult.h
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

#ifndef MRTD_RESULT_INCLUDED
#define MRTD_RESULT_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerResultFwd.h"
#include "Recognizer/Templating/TemplatingResult.h"

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
* @memberof RecognizerResult
* @brief Returns non-zero value if given result is generated by Machine Readable Travel Document recognizer.
* @param result RecognizerResult object
* @return non-zero value if result is generated by Machine Readable Travel Document recognizer, zero if not
*/
MB_API int MB_CALL recognizerResultIsMRTDResult( const RecognizerResult* result );

/**
 * @enum MRTDDocumentType
 * @brief Possible types of Machine Readable Travel Documents.
 */
enum MRTDDocumentType {
    /** Represents unknown MRTD document */
    MRTD_TYPE_UNKNOWN,
    /** Represents Identity card */
    MRTD_TYPE_IDENITY_CARD,
    /** Represents passport */
    MRTD_TYPE_PASSPORT,
    /** Represents visa */
    MRTD_TYPE_VISA,
    /** Represents green card */
    MRTD_TYPE_GREEN_CARD
};

/**
 * @brief Typedef for MRTDDocumentType enum.
 */
MB_EXPORTED_TYPE typedef enum MRTDDocumentType MRTDDocumentType;

/**
* @struct MRTDResult
* @brief Result for storing MRTD recognition data
*/
struct MRTDResult {
    /** Document code string */
    const char* documentCode;

    /** Issuer string */
    const char* issuer;

    /** Document number */
    const char* documentNumber;

    /** Optional string 1 */
    const char* opt1;

    /** Date of birth */
    MBDate dateOfBirth;

    /** Sex of the document holder */
    const char* sex;

    /** Date of expiry of the document */
    MBDate dateOfExpiry;

    /** Three letter code of the document holder's nationality */
    const char* nationality;

    /** Optional string 2 */
    const char* opt2;

    /** Primary ID of the document holder (this is usually a last name) */
    const char* primaryID;

    /** Secondary ID of the document holder (this is usually a first name) */
    const char* secondaryID;

    /** Alien number (exists only on U.S. green card) */
    const char* alienNumber;

    /** Application receipt number (exists only on U.S. green card) */
    const char* applicationReceiptNumber;

    /** Immigrant case number (exists only on U.S. green card) */
    const char* immigrantCaseNumber;

    /** Defines whether MRTD data has been verified */
    int mrtdVerified;

    /** Type of document that was scanned */
    MRTDDocumentType documentType;

    /** Raw MRZ string that was used for parsing other data */
    const char* rawMRZString;

    /** Pointer to buffer containing JPEG encoded image of full document.
     *  By default, this is set to NULL, unless encodeFullDocumentImage property
     *  is enabled in MRTDSettings.
     */
    const unsigned char* fullDocumentJpegBuffer;

    /**
     * Size of buffer containing JPEG encoded image of full document.
     * By default, this is set to 0, unless encodeFullDocumentImage property is
     * enabled in MRTDSettings.
     */
    size_t fullDocumentBufferSize;

    /**
     * Pointer to buffer containing JPEG encoded image of machine readable zone.
     * By default, this is set to NULL, unless encodeMachineReadableZoneImage property
     * is enabled in MRTDSettings.
     */
    const unsigned char* mrzImageJpegBuffer;

    /**
     * Size of buffer containing JPEG encoded image of machine readable zone.
     * By default, this is set to 0, unless encodeMachineReadableZoneImage property
     * is enabled in MRTDSettings.
     */
    size_t mrzImageBufferSize;

    /**
     * Pointer to TemplatingResult structure containing any non-MRZ data that was potentionally
     * scanned.
     */
    const TemplatingResult* templatingResult;
};

/**
 * @brief Typedef for MRTDResult structure.
 */
MB_EXPORTED_TYPE typedef struct MRTDResult MRTDResult;

/**
 * @brief Returns a string representation of MRTDDocumentType enum. Returned pointer is a string constant.
 * @param mrtd MRTDDocumentType value.
 * @return String representation of mrtd.
 */
MB_API const char* MB_CALL mrtdDocumentTypeToString(MRTDDocumentType mrtd);

/**
* @memberof RecognizerResult
* @brief Obtains the scanned data from Machine Readable Zone.
* You can use the obtained strings inside MRTDResult structure until recognizerResultDelete is called on the RecognizerResult object.
* If you want to use the any string later, you must copy it.
* All strings are guaranteed to be zero-terminated.
* Example:
* @code
MRTDResult mrtdResult;
RecognizerErrorStatus status = recognizerResultGetMRTDResult(result, &mrtdResult);

if (status == RECOGNIZER_ERROR_STATUS_SUCCESS) {
    // use data from mrtdResult
    // all pointers are valid until recognizerResultDelete is called with 'result' as parameter
} else {
    const char* statusString = recognizerErrorToString(status);
    printf("Error status: %s\n", statusString);

    // handle error
}
* @endcode
*
* @param result RecognizerResult object which stores the values of the results
* @param mrtdResult Pointer to MRTDResult structure which will be filled with scanned data.
* @return status of the operation
*/
MB_API RecognizerErrorStatus MB_CALL recognizerResultGetMRTDResult(const RecognizerResult* result, MRTDResult* mrtdResult);

#ifdef __cplusplus
}
#endif

#endif

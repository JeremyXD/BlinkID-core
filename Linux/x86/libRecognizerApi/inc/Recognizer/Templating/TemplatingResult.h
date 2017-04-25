/**
 * @file TemplatingResult.h
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

#ifndef TEMPLATING_RESULT_INCLUDED
#define TEMPLATING_RESULT_INCLUDED

#include "Recognizer/Export.h"
#include "Recognizer/OCR/OcrResult.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerResultFwd.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct TemplatingResult
 * @brief Recognition result of Templating Recognizer.
 */
struct TemplatingResult;

/**
 * @brief Typedef for TemplatingResult structure.
 */
typedef MB_EXPORTED_TYPE struct TemplatingResult TemplatingResult;

/**
 * @memberof TemplatingResult
 * @brief Returns the parsed string (if any) produced by parser of given name from default parser group.
 * You can use obtained string until recognizerResultDelete is called on the RecognizerResult object that
 * owns the TemplatingResult from which string will be obtained.
 * If you want to use string later, you must copy it.
 * String is guaranteed to be zero-terminated.
 * If parser of given name produced the date as the result, this function will return the original string
 * from which date was parsed. If you need the entire date structure, please use templateResultGetParsedDate function.
 * If no result exists in default parser group under requested name, destination pointer will be set to NULL and RECOGNIZER_ERROR_STATUS_INVALID_ARGUMENT will be returned.
 * @param templatingResult  Pointer to TemplatingResult structure containing the result information.
 * @param parserName        Name of the parser that produced the result.
 * @param destination       Pointer to char pointer that will be set to internal array holding the string data.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingResultGetParsedStringFromDefaultParserGroup( const TemplatingResult* templatingResult, const char* parserName, const char** destination );

/**
 * @memberof TemplatingResult
 * @brief Returns the parsed string (if any) produced by parser of given name from given parser group.
 * You can use obtained string until recognizerResultDelete is called on the RecognizerResult object that
 * owns the TemplatingResult from which string will be obtained.
 * If you want to use string later, you must copy it.
 * String is guaranteed to be zero-terminated.
 * If parser of given name produced the date as the result, this function will return the original string
 * from which date was parsed. If you need the entire date structure, please use templateResultGetParsedDate function.
 * If no result exists in given parser group under requested name, destination pointer will be set to NULL and RECOGNIZER_ERROR_STATUS_INVALID_ARGUMENT will be returned.
 * @param templatingResult  Pointer to TemplatingResult structure containing the result information.
 * @param parserName        Name of the parser that produced the result.
 * @param parserGroupName   Name of the parser group where parser is located.
 * @param destination       Pointer to char pointer that will be set to internal array holding the string data.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingResultGetParsedStringFromParserGroup( const TemplatingResult* templatingResult, const char* parserName, const char* parserGroupName, const char** destination );

/**
 * @memberof TemplatingResult
 * @brief Returns the parsed date (if any) produced by parser of given name from default parser group.
 * If no result exists in default parser group under requested name or if result is not date, destination pointer will be unchanged and RECOGNIZER_ERROR_STATUS_INVALID_ARGUMENT will be returned.
 * @param templatingResult  Pointer to TemplatingResult structure containing the result information.
 * @param parserName        Name of the parser that produced the result.
 * @param date              Pointer to MBDate structure that will be filled with parsed date data.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingResultGetParsedDateFromDefaultParserGroup( const TemplatingResult* templatingResult, const char* parserName, MBDate* date );

/**
 * @memberof TemplatingResult
 * @brief Returns the parsed date (if any) produced by parser of given name from given parser group.
 * If no result exists in given parser group under requested name or if result is not date, destination pointer will be unchanged and RECOGNIZER_ERROR_STATUS_INVALID_ARGUMENT will be returned.
 * @param templatingResult  Pointer to TemplatingResult structure containing the result information.
 * @param parserName        Name of the parser that produced the result.
 * @param parserGroupName   Name of the parser group where parser is located.
 * @param date              Pointer to MBDate structure that will be filled with parsed date data.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingResultGetParsedDateFromParserGroup( const TemplatingResult* templatingResult, const char* parserName, const char* parserGroupName, MBDate* date );

/**
 * @memberof TemplatingResult
 * @brief Returns the document classification result, if classifier was used (see ClassifierFunction). If no classifier was used, returns NULL.
 * @param templatingResult  Pointer to TemplatingResult structure containing the result information.
 * @param destination       Pointer to char pointer which will be set to classification result (the output of ClassifierFunction), or NULL if no classification was performed.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingResultGetDocumentClassification( const TemplatingResult* templatingResult, const char** destination );

/**
 * @memberof TemplatingResult
 * @brief Returns the OcrResult for default parser group. If no OCR was performed, default-initialises the OcrResult structure.
 * Returned OcrResult has its own lifetime, i.e. it is not bound to owning RecognizerResult. When you do not need OcrResult anymore,
 * clean it up with ocrResultDelete function.
 * @param templatingResult  Pointer to TemplatingResult structure containing the result information.
 * @param destination       Pointer to OcrResult structure that will be initialised with OCR result.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingResultGetOcrResultForDefaultParserGroup( const TemplatingResult* templatingResult, OcrResult* destination );

/**
 * @memberof TemplatingResult
 * @brief Returns the OcrResult for given parser group. If no OCR was performed or if parser group does not exist, default-initialises the OcrResult structure.
 * Returned OcrResult has its own lifetime, i.e. it is not bound to owning RecognizerResult. When you do not need OcrResult anymore,
 * clean it up with ocrResultDelete function.
 * @param templatingResult  Pointer to TemplatingResult structure containing the result information.
 * @param parserGroupName   Name of the parser group for which OCR result should be obtained.
 * @param destination       Pointer to OcrResult structure that will be initialised with OCR result.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL templatingResultGetOcrResultForParserGroup( const TemplatingResult* templatingResult, const char* parserGroupName, OcrResult* destination );

#ifdef __cplusplus
}
#endif

#endif

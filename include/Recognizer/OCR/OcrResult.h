/**
 * @file OcrResult.h
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
#ifndef OCR_RESULT_INCLUDED
#define OCR_RESULT_INCLUDED

#include "OcrFont.h"
#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/Rectangle.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Typedef for OcrChar structure.
 */
typedef MB_EXPORTED_TYPE struct OcrChar OcrChar;

/**
 * @struct OcrChar
 * @brief Structure that describes single recognised character.
 */
struct OcrChar {
    /**
     * @brief Unicode value of the character.
     */
    int value;

    /**
     * @brief Height of the character, in pixels.
     */
    int height;

    /**
     * @brief Indicates whether character is bold or not.
     */
    int bold;

    /**
     * @brief Indicates whether character is italic or not.
     */
    int italic;

    /**
     * @brief Indicates whether character is uncertain or not.
     */
    int uncertain;

    /**
     * @brief Recognition quality of the character. Max quality is 100 and min quality is 0.
     */
    int quality;

    /**
     * @brief Position of character in image.
     */
    MBRectangle position;

    /**
     * @brief Font of the character.
     */
    OcrFont font;

    /**
     * @brief Array of recognition alternatives for this char.
     * This array is valid until owning OcrResult object is alive.
     * If there are no recognition variants, this is set to NULL.
     */
    OcrChar* recognitionVariants;

    /**
     * @brief Size of recognition alternatives array.
     */
    size_t recognitionVariantsCount;
};

/**
 * @struct OcrLine
 * @brief Structure that describes line inside OcrResult.
 */
struct OcrLine {
    /**
     * @brief Position of the line in image.
     */
    MBRectangle position;

    /**
     * @brief Array of characters contained in OcrLine.
     * This array is valid until owning OcrReult object is alive.
     * If there are no characters in OcrLine, this is set to NULL.
     */
    OcrChar* chars;

    /**
     * @brief Size of chars array.
     */
    size_t charsCount;
};

/**
 * @brief Typedef for OcrLine structure.
 */
typedef MB_EXPORTED_TYPE struct OcrLine OcrLine;

/**
 * @struct OcrBlock
 * @brief Structure that describes block inside OcrResult.
 */
struct OcrBlock {
    /**
     * @brief Position of the block in image.
     */
    MBRectangle position;

    /**
     * @brief Array of lines contained in OcrBlock.
     * This array is valid until owning OcrResult object is alive.
     * If there are no lines in OcrBlock, this is set to NULL.
     */
    OcrLine* lines;

    /**
     * @brief Size of lines array.
     */
    size_t linesCount;
};

/**
 * @brief Typedef for OcrBlock structure.
 */
typedef MB_EXPORTED_TYPE struct OcrBlock OcrBlock;

/**
 * @struct OcrResult
 * @brief Structure that describes OCR recognition result.
 * OCR recognition result is a list of blocks (see OcrBlock), each block having a list of lines (see OcrLine) and each line having a list of characters (see OcrChar).
 * Each OcrChar contains all information that OCR engine has returned.
 * OcrResult structure has its own lifetime, i.e. it can outlive RecognizerResult object it originated from. To delete the OcrResult structure and cleanup the memory,
 * you should use function ocrResultDelete.
 */
struct OcrResult {
    /**
     * @brief Transformation matrix of the OCR result.
     * All coordinates of OcrBlock, OcrLine and OcrChar are given in coordinate system of the image the OCR was performed on. However,
     * in some cases this is not the same image that was given as input. You can use this 3x3 matrix to calculate convert the coordinates
     * of each MBRectangle from coordinate system of the image the OCR was performed on to coordinate system of the input image. If no
     * internal transformations took place (e.g. perspective correction), then this holds an identity matrix.
     */
    float transformation[ 9 ];

    /**
     * @brief Array of blocks contained in OcrResult.
     * The pointer is valid until ocrResultDelete is called with this OcrResult as parameter.
     */
    OcrBlock* blocks;

    /**
     * @brief Size of the blocks array.
     */
    size_t blocksCount;
};

/**
 * @brief Typedef for OcrResult structure.
 */
typedef MB_EXPORTED_TYPE struct OcrResult OcrResult;

/**
 * @memberof OcrResult
 * @brief Deletes the OcrResult object and releases all memory required by it.
 * After this method is called, all pointers in OcrResult, OcrBlock, OcrLine and OcrChar structures are not valid anymore.
 * @param ocrResult OcrResult to be deleted.
 * @return Status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL ocrResultDelete( OcrResult* ocrResult );

#ifdef __cplusplus
}
#endif

#endif

/**
 * @file OcrEngineSettings.h
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
#ifndef OCR_ENGINE_SETTINGS_INCLUDED
#define OCR_ENGINE_SETTINGS_INCLUDED

#include "OcrFont.h"
#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"

#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum OcrDocumentType
 * Specifies available OCR document types. Every OCR document type has specific set of characters and
 * settings for ocr engine.
 */
enum OcrDocumentType {
    /**
     * Supports latin alphabet, numbers, punctuation marks, currency symbols and common country
     * specific characters for large number of commonly used fonts.
     * This document type should be used in most cases.
     */
    OCR_DOCUMENT_TYPE_GENERIC,
    /**
     *  Supports the recognition of MICR (Magnetic Ink Character Recognition) font characters
     *  which are used mainly by the banking industry.
     */
    OCR_DOCUMENT_TYPE_MICR,
    /** Supports the recognition of Arabic-Indic numerals and punctuation marks used in amounts. */
    OCR_DOCUMENT_TYPE_ARABIC
};

/**
 * @brief Typedef for OcrDocumentType enum
 */
typedef MB_EXPORTED_TYPE enum OcrDocumentType OcrDocumentType;

/**
 * @struct OcrWhitelistEntry
 * @brief Structure defining the allowed character-font combination.
 */
struct OcrWhitelistEntry {
    /**
     * @brief Value of the character inside the whitelist.
     */
    int charValue;

    /**
     * @brief Font of the charValue (use OCR_FONT_ANY to specify all available fonts).
     */
    OcrFont charFont;
};

/**
 * @brief Typedef for OcrWhitelistEntry structure.
 */
typedef MB_EXPORTED_TYPE struct OcrWhitelistEntry OcrWhitelistEntry;

/**
 * @struct OcrEngineSettings
 * @brief Represents the settings for performing the Optical Character Recognition (OCR) process on image.
 */
struct OcrEngineSettings {
    /**
     * @brief Defines the maximum allowed number of char recognition variants OCR engine will produce for each OcrChar.
     * By default, OCR engine will not produce char recognition variants
     * (i.e. maximum allowed char recognition variants is 0). By setting this number to higher
     * values, you can create more flexible OCR postprocessing algorithm because more char
     * recognition variants will be available to you. However, setting it too high can lead
     * to significant performance problems.
     */
    int maximumAllowedCharRecognitionVariants;

    /**
     * @brief Defines the minimum height of the line of text given in pixels.
     * All lines smaller than this value are ignored. Line height is defined as
     * height of largest character in line.
     */
    int minimumLineHeight;

    /**
     * @brief Defines the maximum height of the line of text given in pixels.
     * All chars larger than this value are ignored.
     */
    int maximumLineHeight;

    /**
     * @brief Defines the minimum height of the char that can be classified.
     * All chars smaller than this value are ignored. Unlike ink minimumLineHeight,
     * this value refers to individual chars, while minimumLineHeight refers to line
     * height, which equals to height of largest char in line.
     */
    int minimumCharHeight;

    /**
     * @brief Defines the maximum number of chars expected in the image.
     * Setting this value can speed up the OCR processing because all images with more chars than specified will be ignored
     * (becuase in most cases they are noise). Please note that noise characters are also counted. This means that if you
     * expect 17 characters and usually get 25 characters (8 of which are noise), setting this to 17 will cause to return
     * empty OCR result (because 25 characters is more than expected 17, even though 8 characters are noise).
     * Use this option solely to increase OCR speed. This option will not increase OCR accuracy.
     */
    size_t maximumCharsExpected;

    /**
     * @brief Specifies if the additional image processing which drops the background colors should be performed.
     * Use this if you have black text on color backgrounds.
     * If you have black text on white background, using this is not needed as it slows down processing.
     * If you have color text, don't use it at all because color dropout will remove the text.
     */
    int colourDropoutEnabled;

    /**
     * @brief Specifies if additional advanced image processing is performed on image.
     * By default it's set to true, as this is required for general-purpose use case, such
     * as when OCR is performed on colour or grayscale image.
     * You should disable this only if you perform your own image processing and provide already
     * binarised image to the OCR engine.
     */
    int advancedImageProcessingEnabled;

    /**
     * @brief Specifies the OCR document type that will be used to adjust the ocr engine settings.
     * Every document type has specific set of characters and ocr engine settings.
     * By default it is set to OCR_DOCUMENT_TYPE_GENERIC.
     * Default OCR document type is the best option in most cases and should not be changed unless
     * the specific type is required.
     */
    OcrDocumentType documentType;

    /**
     * @brief Enable word processing.
     * If the option is enabled, words in the OCR result will be
     * processed and possibly classified as numbers, lower case
     * words, upper case, ... .
     */
    int wordProcessingEnabled;

    /**
     * @brief Specifies whether flipped text detection is enabled.
     * If enabled, OCR will produce result for the input image and for
     * input image rotated by 180 degrees. Better results of the two will
     * be returned. Note that enabling this option requires two OCR passes,
     * therefore double time will be required.
     */
    int detectFlippedTextEnabled;

    /**
     * @brief Enable character case post-processing.
     * If enabled, classification will be aided by character position in line.
     * In other words, character height and baseline position will be used to
     * possibly improve the result. Use this option only when expecting
     * structured text.
     */
    int casePostProcessorEnabled;

    /**
     * @brief Enable noisy lines removal.
     * If enabled noisy lines will be removed from the result. Noisy lines
     * are short or with high percentage of punctuniation, ... . Use this
     * option only when expecting structured text.
     */
    int noisePostprocessingEnabled;

    /**
     * @brief Enable char ratio post-processing.
     * If enabled, char quality will be influenced by character ratio difference
     * from the original character in font specified by classification.
     */
    int ratioPostprocessorEnabled;

    /**
     * @brief Enable removal of chars that exceed image bounds.
     * If enabled, all chars that are classified as cutoff, in other words chars
     * that exceed image bounds, will be removed from the result.
     */
    int cutoffCharFilterEnabled;

    /**
     * @brief Enable line grouping.
     * If line grouping is enabled, chars are grouped into lines.
     * If disabled every char is in it's own line/block.
     * Line grouping should be enabled when structured text is expected and
     * disabled otherwise.
     */
    int lineGroupingEnabled;

    /**
     * @brief Array of char+font combinations that are allowed during recognition.
     * If set to NULL, all supported char+font combinations are allowed.
     * If set to non-NULL, make sure pointer is valid at the time of using OcrEngineSettings structure.
     */
    OcrWhitelistEntry* whitelist;

    /**
     * @brief Size of whitelist array.
     */
    size_t whitelistCount;
};

/**
 * @brief Typedef for OcrEngineSettings structure
 */
typedef MB_EXPORTED_TYPE struct OcrEngineSettings OcrEngineSettings;

/**
 * @memberof OcrEngineSettings
 * @brief Initializes all fields of OcrEngineSettings structure to default values.
 * @param settings Pointer to OcrEngineSettings
 */
MB_API void MB_CALL ocrEngineSettingsSetDefaultValues( OcrEngineSettings* settings );

#ifdef __cplusplus
}
#endif

#endif

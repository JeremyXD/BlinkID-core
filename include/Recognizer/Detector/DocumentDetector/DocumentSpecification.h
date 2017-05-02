/**
 * @file DocumentSpecification.h
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
#ifndef DOCUMENT_SPECIFICATION_INCLUDED
#define DOCUMENT_SPECIFICATION_INCLUDED

#include "Recognizer/Detector/DecodingInfo.h"
#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum ScanningMode
 * @brief Defines the scanning mode for DocumentSpecification.
 */
enum ScanningMode {
    /* Documents will be detected only when frame orientation is landscape. */
    SCANNING_MODE_LANDSCAPE,
    /* Documents will be detected only when frame orientation is portrait. */
    SCANNING_MODE_PORTRAIT,
    /* Documents will be detected in all orientations and scale will be selected based on current orientation. */
    SCANNING_MODE_AUTO
};

/**
 * @brief Typedef for ScanningMode enumeration.
 */
typedef MB_EXPORTED_TYPE enum ScanningMode ScanningMode;

/**
 * @struct ScaleAndTolerance
 * @brief Defines the scale and scale tolerance that will be used when detecting documents.
 */
struct ScaleAndTolerance {
    /**
     * @brief Ideal scale defined as percentage of frame dimension at which document should be detected.
     */
    float scale;
    /**
     * @brief Scale tolerance is number that together with scale defines scale interval at which document can be detected.
     * Scale interval is [scale-tolerance, scale+tolerance]
     */
    float tolerance;
};

/**
 * @brief Typedef for ScaleAndTolerance structure
 */
typedef MB_EXPORTED_TYPE struct ScaleAndTolerance ScaleAndTolerance;

/**
 * @struct XRange
 * @brief Defines the range of possible document offsets on x axis.
 */
struct XRange {
    /**
     * @brief possible left offset of centre. Value must be in range [-1, 0].
     */
    float leftRange;
    /**
     * @brief possible right offset of centre. Value must be in range [0, 1]
     */
    float rightRange;
};

/**
 * @brief Typedef for XRange structure
 */
typedef MB_EXPORTED_TYPE struct XRange XRange;

/**
 * @struct YRange
 * @brief Defines the range of possible document offsets on y axis.
 */
struct YRange {
    /**
     * @brief possible offset above centre. Value must be in range [-1, 0].
     */
    float aboveRange;
    /**
     * @brief possible offset below centre. Value must be in range [0, 1]
     */
    float belowRange;
};

/**
 * @brief Typedef for YRange structure
 */
typedef MB_EXPORTED_TYPE struct YRange YRange;

/**
 * @struct DocumentSpecification
 * @brief Defines the basic specification for scanning a document.
 */
struct DocumentSpecification {
    /**
     * @brief Aspect ratio of the document that will be detected.
     * The number must be positive (i.e. aspectRatio >= 0 )
     */
    float aspectRatio;

    /**
     * @brief The array of decoding infos that control how detection will be dewarped.
     * Must not be NULL nor empty.
     */
    MBDecodingInfo* decodingInfos;

    /**
     * @brief Size of the decodingInfos array.
     */
    size_t decodingInfosCount;

    /**
     * @brief The scanning mode that defines in which orientations can this document be detected.
     */
    ScanningMode scanningMode;

    /**
     * @brief Scale and tolerance that will be used when detecting document in landscape orientation.
     */
    ScaleAndTolerance landscapeScale;

    /**
     * @brief Scale and tolerance that will be used when detecting document in portrait orientation.
     */
    ScaleAndTolerance portraitScale;

    /**
     * @brief Maximum angle for document detection.
     * Allowed values are in interval [0, 35]
     */
    float maxAngle;

    /**
     * @brief percentage of possible document offset on x axis.
     */
    XRange xRange;

    /**
     * @brief percentage of possible document offset on y axis.
     */
    YRange yRange;
};

/**
 * @brief Typedef for DocumentSpecification structure.
 */
typedef MB_EXPORTED_TYPE struct DocumentSpecification DocumentSpecification;

/**
 * @enum DocumentSpecificationPreset
 * @brief Available presets for initialising the DocumentSpecification structure with function documentSpecificationInitFromPreset.
 */
enum DocumentSpecificationPreset {
    /**
     * @brief Uses detection settings for detecting ID1 card format as explained <a href="https://en.wikipedia.org/wiki/ISO/IEC_7810#ID-1">here</a>.
     */
    DOCUMENT_SPECIFICATION_PRESET_ID1_CARD,

    /**
     * @brief Uses detection settings for detecting ID1 card format as explained <a href="https://en.wikipedia.org/wiki/ISO/IEC_7810#ID-2">here</a>.
     */
    DOCUMENT_SPECIFICATION_PRESET_ID2_CARD,

    /** @brief Uses detections settings for detecting cheques */
    DOCUMENT_SPECIFICATION_PRESET_CHEQUE,

    /** @brief Uses detection settings for detection of A4 documents in portrait orientation (for example invoices) */
    DOCUMENT_SPECIFICATION_PRESET_A4_PORTRAIT,

    /** @brief Uses detection settings for detection of A4 documents in landscape orientation (for example printed presentation slides) */
    DOCUMENT_SPECIFICATION_PRESET_A4_LANDSCAPE,

    /** @brief Number of presets */
    DOCUMENT_SPECIFICATION_PRESET_COUNT
};

/**
 * @brief Typedef for DocumentSpecificationPreset enumeration
 */
typedef MB_EXPORTED_TYPE enum DocumentSpecificationPreset DocumentSpecificationPreset;

/**
 * @memberof DocumentSpecification
 * @brief Initializes the DocumentSpecification structure with default values for given preset.
 * After initialisation, MBDecodingInfo pointer points to array in static storage, so user is not
 * required to perfom any memory deallocation of this pointer.
 * @param specification     Pointer to DocumentSpecification structure which will be initialised
 * @param preset            Preset which will be used for initialisation.
 * @return status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL documentSpecificationInitFromPreset( DocumentSpecification* specification, const DocumentSpecificationPreset preset );

#ifdef __cplusplus
}
#endif

#endif

/**
 * @file DecodingInfo.h
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
#ifndef DECODINGINFO_H
#define DECODINGINFO_H

#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/Rectangle.h"

#ifdef __cplusplus
#include <cstdlib>
extern "C" {
#endif

/**
 * @struct MBDecodingInfo
 * @brief Represents a part of detection that will be used later.
 *
 * Each part is represented by rectangle defining the position of interest relative
 * to detection and dewarpHeight defining the height of dewarped image obtained
 * from this position.
 */
struct MBDecodingInfo {
    /**
     * Position inside detection this decoding info refers to. The position should be
     * relative rectangle.
     */
    MBRectangle position;
    /**
     * Height in pixels of the dewarped image that will be created from above position in
     * original image.
     */
    unsigned int dewarpHeight;
    /**
     * Name of this decoding info. This is used to match decoding info with parser group
     * in Templating API.
     */
    const char* name;

#ifdef __cplusplus
    MBDecodingInfo() :
        dewarpHeight( 0 ),
        name( NULL ) {}
#endif
};

/**
 * @brief Typedef for MBDecodingInfo structure.
 */
typedef MB_EXPORTED_TYPE struct MBDecodingInfo MBDecodingInfo;

#ifdef __cplusplus
}
#endif

#endif

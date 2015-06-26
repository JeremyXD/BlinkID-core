//
//  RecognizerWrapper.h
//  RecognizerApiDemo
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 DoDo. All rights reserved.
//

#ifndef __RecognizerApiDemo__RecognizerWrapper__
#define __RecognizerApiDemo__RecognizerWrapper__

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "RecognizerApi.h"

/**
 * This structure contains everything related to Recognizer objects and recognition processing
 */
typedef struct RecognizerWrapper {

    /* this member will contain all recognition settings (which recognizers are enabled, etc.) */
    RecognizerSettings* settings;

    /* this member will contain device information. On Mac/PC this is not usually necessary, but
     can information about available processor cores. If more than 1 processor is available, recognizers
     will try to use parallel algorithms as much as possible. */
    RecognizerDeviceInfo* deviceInfo;

    /* this member is the global recognizer that internally contains a list of different recognizers.
     Each recognizer is an object that can perform object recognitions. For example, there are PDF417 barcode
     recognizer (Microblink's implementation for PDF417 barcodes), ZXing barcode recognizer (supports everything ZXing supports),
     Microblink's 1D barcode recognizer, etc. */
    Recognizer* recognizer;

    /* recoginzer callback structure contains pointers to functions that will be called during the recognition process */
    RecognizerCallback recognizerCallback;

    /* buffer with ocr model */
    char* ocrModel;

    /* size of the ocr model */
    int ocrModelSize;

} RecognizerWrapper;

/**
 * Method initializes the recognizer wrapper with settings for MRTD scanning
 *
 *  @param wrapper      wrapper
 *  @param ocrModelPath path to ocr model. Without OCR model file, OCR cannot be performed.
 *
 *  @return status of the operation
 */
RecognizerErrorStatus recognizerWrapperInit(RecognizerWrapper* wrapper, const char* ocrModelPath);

/**
 * Method terminates the recognizer wrapper and frees all taken resources
 *
 *  @param wrapper wrapper
 *
 *  @return status of the operation
 */
RecognizerErrorStatus recognizerWrapperTerm(RecognizerWrapper* wrapper);

/**
 * Method processes image using Recognizer inside RecognizerWraper
 *
 *  @param wrapper   wrapper
 *  @param image     image object
 *
 *  @return status of processing
 */
RecognizerErrorStatus recognizerWrapperProcessImage(const RecognizerWrapper* wrapper, const RecognizerImage* image,
                                                    RecognizerErrorStatus (*processRecognizerResult)(const RecognizerResult* result));

/**
 * Method processes image defined with a path using Recognizer inside RecognizerWraper
 *
 *  @param wrapper   wrapper
 *  @param filePath  path to the image
 *
 *  @return status of processing
 */
RecognizerErrorStatus recognizerWrapperProcessImageFromFile(const RecognizerWrapper* wrapper, const char* filePath,
                                                            RecognizerErrorStatus (*processRecognizerResult)(const RecognizerResult* result));

#ifdef __cplusplus
}
#endif

#endif

//
//  RecognizerCallback.h
//  RecognizerApiDemo
//
//  Created by Jura on 24/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#ifndef RecognizerApiDemo_RecognizerCallback_h
#define RecognizerApiDemo_RecognizerCallback_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "RecognizerApi.h"

void onDetectionStarted();

int onDetectedObject(const MBPoint* points, const size_t pointsSize, MBSize imageSize, MBDetectionStatus ds);

void onDetectionFailed();

void onRecognitionStarted();

void onRecognitionFinished();

RecognizerCallback buildRecognizerCallback();

#ifdef __cplusplus
}
#endif

#endif

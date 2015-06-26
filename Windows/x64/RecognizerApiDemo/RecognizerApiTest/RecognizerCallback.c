//
//  RecognizerCallback.c
//  RecognizerApiDemo
//
//  Created by Jura on 24/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#include "RecognizerCallback.h"

// specifier for correct prinf of size_t on 32-bit and 64-bit architectures
#if defined(_MSC_VER)
#define JL_SIZE_T_SPECIFIER    "%Iu"
#define JL_SSIZE_T_SPECIFIER   "%Id"
#define JL_PTRDIFF_T_SPECIFIER "%Id"
#elif defined(__GNUC__)
#define JL_SIZE_T_SPECIFIER    "%zu"
#define JL_SSIZE_T_SPECIFIER   "%zd"
#define JL_PTRDIFF_T_SPECIFIER "%zd"
#endif

void onDetectionStarted() {
    printf("Detection has started!\n");
}

int onDetectedObject(const PPPoint* points, const size_t pointsSize, PPSize imageSize, PPDetectionStatus ds) {
    const char* detStatusDesc = "";

    printf("Detection on image of size %dx%d has finished\n", imageSize.width, imageSize.height);
    printf("Total " JL_SIZE_T_SPECIFIER " points have been found\n", pointsSize);

    if (pointsSize > 0 && points != NULL) {
        for (size_t i = 0; i < pointsSize; ++i) {
            printf("Point " JL_SIZE_T_SPECIFIER " location: (%d, %d)\n", i, points[i].x, points[i].y);
        }
    }

    switch (ds) {
        case DETECTION_STATUS_CAMERA_AT_ANGLE:
            detStatusDesc = "camera is at angle";
            break;
        case DETECTION_STATUS_CAMERA_TOO_HIGH:
            detStatusDesc = "camera is too high";
            break;
        case DETECTION_STATUS_FAIL:
            detStatusDesc = "detection has failed";
            break;
        case DETECTION_STATUS_PARTIAL_OBJECT:
            detStatusDesc = "some parts of object are not visible";
            break;
        case DETECTION_STATUS_QR_SUCCESS:
            detStatusDesc = "QR code has been successfuly detected";
            break;
        case DETECTION_STATUS_SUCCESS:
            detStatusDesc = "detection has succeeded";
            break;
        case DETECTION_STATUS_CAMERA_TOO_NEAR:
            detStatusDesc = "camera is too near";
            break;
    }

    printf("Detection status: %s\n", detStatusDesc);

    return 1;
}

void onDetectionFailed() {
    printf("Detection has completely failed\n");
}

void onRecognitionStarted() {
    printf("Object recognition has started\n");
}

void onRecognitionFinished() {
    printf("Object recognition has finished\n");
}

RecognizerCallback buildRecognizerCallback() {

    RecognizerCallback cb;

    /* onDetectedObject is called when recognizer detects an object in image */
    cb.onDetectedObject = onDetectedObject;

    /* onDetectionStarted is called when recognizer begins object detection process */
    cb.onDetectionStarted = onDetectionStarted;

    /* onDetectionFailed is called when all enabled recognizers do not detect anything on the image */
    cb.onDetectionFailed = onDetectionFailed;

    /* onRecognitionStarted is called when recognition of detected object begins */
    cb.onRecognitionStarted = onRecognitionStarted;

    /* onRecognitionFinished is called when recognition of detected object finishes */
    cb.onRecognitionFinished = onRecognitionFinished;

    /* onProgress is called in some recognizers to provide progress information. Here we are not interested in this callback. */
    cb.onProgress = NULL;

    /* onShouldStopRecognition is called multiple times from some recognizers to check if recognition should be canceled. */
    cb.onShouldStopRecognition = NULL;

    cb.onShowImage = NULL;
    
    return cb;
}

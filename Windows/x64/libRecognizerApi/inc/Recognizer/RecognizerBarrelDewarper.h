/**
 * @file RecognizerBarrelDewarper.h
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

#ifndef RECOGNIZERBARRELDEWARPER_HPP
#define RECOGNIZERBARRELDEWARPER_HPP

#include "RecognizerError.h"
#include "RecognizerImage.h"
#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @struct RecognizerBarrelDewarper
  @brief RecognizerBarrelDewarper data structure.

  RecognizerBarrelDewarper is an object that can be initialized with barrel undistortion parameters and then used later for quickly undistort images.
  */
struct RecognizerBarrelDewarper;

/**
 * @brief Typedef for RecognizerBarrelDewarper structure.
 */
typedef MB_EXPORTED_TYPE struct RecognizerBarrelDewarper RecognizerBarrelDewarper;

/**
  @memberof RecognizerBarrelDewarper
  @brief Allocates and creates RecognizerBarrelDewarper with given parameters.

  Distortion parameters are split into two categories:
    - k1, k2, k3    Radial distorion parameters which are used for correcting barrel distortion.
    - p1 and p2     Tangential distortion parameters which are used to correct distortions ocurring when camera plane is not paralel with image plane.

  @param    dewarper        Pointer to pointer referencing the created RecognizerBarrelDewarper object, set to NULL if error occureed.
  @param    k1              Radial distortion parameter, has the most influence. If k1 = 0 it has no influence, for k1 < 0 image is expanded thoward the edges
                            and for k1 > 0 image is shrinked towards the edges.
  @param    k2              Radial distortion parameter. If k2 = 0 it has no influence, for k2 < 0 image is expanded thoward the edges
                            and for k2 > 0 image is shrinked towards the edges.
  @param    p1              Tangential distortion parameter. It is used when squared objects in real world appear like trapezoids on image. If p1 = 0 it has no influence,
                            for p1 < 0 botom of image is expanded and top of image is shrinked, for p1 > 0 botom of image is shrinked and top of image is expanded.
  @param    p2              Tangential distortion parameter. It is used when squared objects in real world appear like trapezoids on image. If p2 = 0 it has no influence,
                            for p2 < 0 left side of image is shrinked and right side of image is expanded, for p2 > 0 left side of image is expanded and right side of image is shrinked.
  @param    k3              Radial distortion parameer, has least influence. If k3 = 0 it has no influence, for k3 < 0 image is expanded thoward the edges
                            and for k3 > 0 image is shrinked towards the edges.
  @param    scale           Additional scaling of transformed image. If scale = 1 no scaling is applied, for scale < 1 image is decreased and for scale > 1 image is increased.
  @return   errorStatus     Status of the operation. Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                             Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerBarrelDewarperCreate(RecognizerBarrelDewarper** dewarper,
            const float k1, const float k2, const float p1, const float p2, const float k3, const float scale);

/**
  @memberof RecognizerBarrelDewarper
  @brief Deletes RecognizerBarrelDewarper object and sets pointer to NULL

  @param    dewarper        Double pointer to the barrel dewarper object which is to be deleted.
  @return   errorStatus     Status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS. If NULL pointer was given,
                                status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerBarrelDewarperDelete(RecognizerBarrelDewarper** dewarper);

/**
  @memberof RecognizerBarrelDewarper
  @brief Applies barrel distortion transformation on input image.

  @param    dewarper        Object which performs transformation of input image.
  @param    input           RecognitionImage object which holds image on which transformation will be performed.
  @param    output          Pointer to pointer referencing the created transformed image obtained by applying parameters given when creating RecognizerBarrelDewarper object.
                            If value of given pointer is NULL new RecognizerImage structure is created, othervise given structure is reused. In case of allocation ::recognizerImageDelete
                            must be called if structure is not going to be used any more. If error occured it will be set to NULL.
  @return   errorStatus     Status of the operation. Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                             Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  @see RecognizerImage to see details on supported image formats.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerBarrelDewarperDewarp(RecognizerBarrelDewarper* dewarper, const RecognizerImage* input, RecognizerImage** output);

#ifdef __cplusplus
}
#endif

#endif

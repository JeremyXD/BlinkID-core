/**
 * @file RecognizerImageProcessor.h
 *
 *  Created on: Jun 6, 2015
 *      Author: boris
 */

#ifndef RECOGNIZERIMAGEPROCESSOR_HPP
#define RECOGNIZERIMAGEPROCESSOR_HPP

#include "RecognizerError.h"
#include "RecognizerImage.h"
#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @brief Mirrors the input image.
  Example:
  @code
    RecognizerImage* output = NULL;
    recognizerImageProcessorMirrorImage(image, &output);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* statusString = recognizerErrorToString(status);
        printf("ImageProcessorMirrorImage method returned status: %s\n", statusString);

        // handle error
    } else {
        // use mirrored image
    }

    recognizerImageDelete(&outout);
  @endcode

  @param    input           Pointer to RecognizerImage object which holds original image.
  @param    output          Pointer to pointer referencing the crated mirrored image. If value of given pointer is NULL new RecognizerImage structure is created,
                            othervise given structure is reused. In case of allocation RecognizerImageDelete must be called if structure is not going to be used any more.
  @return   errorStatus     Status of the operation. Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                             Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerImageProcessorMirrorImage(const RecognizerImage* input, RecognizerImage** output);

/**
  @brief Applies barrel distortion transformation on input image.

  Distortion parameters are split into two categories:
    - k1, k2, k3    Radial distorion parameters which are used for correcting barrel distortion.
    - p1 and p2     Tangential distortion parameters which are used to correct distortions ocurring when camera plane is not paralel with image plane.

  @param    input           Pointer to RecognizerImage object which holds original image.
  @param    output          Pointer to pointer referencing the crated transformed image. If value of given pointer is NULL new RecognizerImage structure is created,
                            othervise given structure is reused. In case of allocation RecognizerImageDelete must be called if structure is not going to be used any more.
                            If error occured it will be set to NULL;
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
PP_API RecognizerErrorStatus PP_CALL recognizerImageProcessorBarrelDewarp(const RecognizerImage* input, RecognizerImage** output,
            const float k1, const float k2, const float p1, const float p2, const float k3, const float scale);

#ifdef __cplusplus
}
#endif

#endif // RECOGNIZERIMAGEPROCESSOR_HPP

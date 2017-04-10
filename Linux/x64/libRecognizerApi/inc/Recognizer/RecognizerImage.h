/**
 * @file RecognizerImage.h
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

#ifndef RECOGNIZERIMAGE_HPP
#define RECOGNIZERIMAGE_HPP

#include "RecognizerError.h"
#include "Export.h"
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  @struct RecognizerImage
  @brief RecognizerImage data structure. Holds image on which recognition will bw performed.
  */
struct RecognizerImage;

/**
 * @brief Typedef for RecognizerImage structure.
 */
typedef MB_EXPORTED_TYPE struct RecognizerImage RecognizerImage;

/**
 * @enum RawImageType
 * @brief Enumeration of all possible raw image types
 */
enum RawImageType {

    /** 4 channel raw image with 32 bit per pixel, 8 bit per channel, channel order is BGRA */
    RAW_IMAGE_TYPE_BGRA,

    /** 2 channel raw image with 24 bit per pixel, 8 bit per channel, channel order is BGR */
    RAW_IMAGE_TYPE_BGR,

    /** 1 channel grayscale raw image with 8 bit per pixel */
    RAW_IMAGE_TYPE_GRAY,

    /**
     Raw YUV 4:2:0 image with a plane of 8 bit Y samples followed by an interleaved
     V/U plane containing 8 bit 2x2 subsampled chroma samples. Typically, frames from Android
     video capture are obtained in this format.
     */
    RAW_IMAGE_TYPE_NV21

};

/**
 * @brief Typedef for RawImageType enum.
 */
typedef MB_EXPORTED_TYPE enum RawImageType RawImageType;

/**
 * @enum ImageOrientation
 * @brief Defined orientation in which image should be interpreted.
 */
enum ImageOrientation {
    /** Image orientation is portrait (as if taken by mobile phone in portrait mode - phone's home button is below screen) */
    IMAGE_ORIENTATION_PORTRAIT,
    /** Image orientation is right landscape (as if taken by mobile phone in landscape right mode - phone's home button is right of screen) */
    IMAGE_ORIENTATION_LANDSCAPE_RIGHT,
    /** Image orientation is reverse portrait (as if taken by mobile phone in reverse portrait mode - phone's home button is above screen) */
    IMAGE_ORIENTATION_PORTRAIT_UPSIDE,
    /** Image orientation is left landscape (as if taken by mobile phone in landscape left mode - phone's home button is left of screen) */
    IMAGE_ORIENTATION_LANDSCAPE_LEFT
};

/**
 * @brief Typedef for ImageOrientation enum.
 */
typedef MB_EXPORTED_TYPE enum ImageOrientation ImageOrientation;

/**
  @memberof RecognizerImage
  @brief Allocates and creates RecognizerImage object from raw image.
  Example:
  @code
    int image_width, image_height, image_stride;
    void* image_buffer;

    // populate above variables (i.e. by loading image file or capturing image with camera)

    RecognizerImage *image;
    RecognizerErrorStatus status = recognizerImageCreateFromRawImage(&img, image_buffer, image_width, image_height, image_stride, RAW_IMAGE_TYPE_BGRA);
    // make sure last parameter correctly describes image format, otherwise a crash will occur later while processing the image. There is no way to
    // detect whether this parameter was given correctly while creating the image, so function may return RECOGNIZER_ERROR_STATUS_SUCCESS even if
    // this parameter is wrong.

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* status_string = recognizerErrorToString(status);
        printf("CreateFromRawImage method returned status: %s\n", status_string);

        // handle error
    } else {
        // use RecognizerImage object
    }

    recognizerImageDelete(image);
  @endcode

  Raw image type is the type without any encoding. List of supported raw image types is given in enum RawImageType.
  @see RawImageType

  @param     image               Pointer to pointer referencing the created RecognizerImage object, set to NULL if error occured.
  @param     input               Pointer to a buffer with raw image pixels
  @param     width               Width of the image, in pixels
  @param     height              Height of the image, in pixels
  @param     bytesPerRow         Number of bytes contained in every row of the image
  @param     rawType             Type of the image. @see RawImageType
  @return    errorStatus         Status of the operation. RECOGNIZER_ERROR_STATUS_SUCCESS for success, RECOGNIZER_ERROR_STATUS_INVALID_TYPE if rawType is invalid.
                                    Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerImageCreateFromRawImage( RecognizerImage** image, const void* input, int width, int height, size_t bytesPerRow, RawImageType rawType );

/**
  @memberof RecognizerImage
  @brief Allocates and creates RecognizerImage object from raw image.
  Example:
  @code
    int image_width, image_height, image_stride;
    void* image_buffer;

    // populate above variables (i.e. by loading image file or capturing image with camera)

    RecognizerImage *image;

    // create image with recognizerImageCreateFromRawImage

    // now, update the image with new buffer
    RecognizerErrorStatus status = recognizerImageSetNewRawBuffer(img, image_buffer, image_width, image_height, image_stride, RAW_IMAGE_TYPE_BGRA);
    // make sure last parameter correctly describes image format, otherwise a crash will occur later while processing the image. There is no way to
    // detect whether this parameter was given correctly while creating the image, so function may return RECOGNIZER_ERROR_STATUS_SUCCESS even if
    // this parameter is wrong.

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* status_string = recognizerErrorToString(status);
        printf("CreateFromRawImage method returned status: %s\n", status_string);

        // handle error
    } else {
        // use RecognizerImage object
    }

    recognizerImageDelete(image);
  @endcode

  Raw image type is the type without any encoding. List of supported raw image types is given in enum RawImageType.
  @see RawImageType

  @param     image               Pointer referencing the created RecognizerImage object, must be non-NULL.
  @param     input               Pointer to a buffer with raw image pixels
  @param     width               Width of the image, in pixels
  @param     height              Height of the image, in pixels
  @param     bytesPerRow         Number of bytes contained in every row of the image
  @param     rawType             Type of the image. @see RawImageType
  @return    errorStatus         Status of the operation. RECOGNIZER_ERROR_STATUS_SUCCESS for success, RECOGNIZER_ERROR_STATUS_INVALID_TYPE if rawType is invalid.
                                    Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerImageSetNewRawBuffer( RecognizerImage* image, const void* input, int width, int height, size_t bytesPerRow, RawImageType rawType );

/**
 * @memberof RecognizerImage
 * @brief Allocates and creates RecognizerImage by copying another RecognizerImage. This method will copy pixel data, so it may be slow.
 * @param   image           Pointer to pointer referencing the created RecognizerImage object, set to NULL if error occured.
 * @param   original        Pointer to original RecognizerImage object that should be copied.
 * @return  errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs. Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerImageCreateCopyFromImage( RecognizerImage** image, const RecognizerImage* original );

/**
  @memberof RecognizerImage
  @brief Deletes the image object and sets pointer to NULL

  @param    image           Double pointer to image object which is to be deleted.
  @return   errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                            Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerImageDelete(RecognizerImage** image);

/**
 * @memberof RecognizerImage
 * @brief Sets the orientation of the image.
 * When created, each image has default orientation IMAGE_ORIENTATION_LANDSCAPE_RIGHT, which corresponds to 'natural' orientation of all images. However, if image was taken
 * in different orientation, you should set correct orientation with this method to ensure best recognition quality.
 * @param   image         Image to which orientation will be set.
 * @param   orientation   Orientation that will be set.
 * @return  errorStatus   Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
 *                        Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerImageSetImageOrientation( RecognizerImage* image, ImageOrientation orientation );

/**
 * @memberof RecognizerImage
 * @brief Gets the orientation of the image.
 * @param   image         Image from which orientation will be obtained.
 * @param   orientation   Pointer to store image orientation.
 * @return  errorStatus   Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
 *                        Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerImageGetImageOrientation( RecognizerImage* image, ImageOrientation* orientation );

/**
  @memberof RecognizerImage
  @brief Gets buffer containing raw image bytes

  @param    image           Pointer to RecognizerImage object of interest.
  @param    data            Double pointer whose value is set to a buffer with raw image pixels. Youu can use the obtained buffer until recognizerImageDelete
                            is called on the RecognizerImage object. If you want to use the buffer later, you must copy it.
  @return   errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                            Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerImageGetRawBytes(const RecognizerImage* image, void** data);

/**
  @memberof RecognizerImage
  @brief Gets width of image in number of pixels

  @param    image           Pointer to RecognizerImage object of interest.
  @param    width           Pointer to store given image width in pixels.
  @return   errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                            Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerImageGetWidth(const RecognizerImage* image, int* width);

/**
  @memberof RecognizerImage
  @brief Gets height of image in number of pixels

  @param    image           Pointer to RecognizerImage object of interest.
  @param    height          Pointer to store given image height in pixels.
  @return   errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                            Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerImageGetHeight(const RecognizerImage* image, int* height);

/**
  @memberof RecognizerImage
  @brief Gets bytes per row for given image

  @param    image           Pointer to RecognizerImage object of interest.
  @param    bytesPerRow     Pointer to store bytes per row of given image.
  @return   errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                            Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerImageGetBytesPerRow(const RecognizerImage* image, int* bytesPerRow);

/**
  @memberof RecognizerImage
  @brief Gets raw image type for given image

  @param    image           Pointer to RecognizerImage object of interest.
  @param    rawImageType    Pointer to store raw image type of given image.
  @return   errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                            Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerImageGetRawImageType(const RecognizerImage* image, RawImageType* rawImageType);

#ifdef __cplusplus
}
#endif

#endif

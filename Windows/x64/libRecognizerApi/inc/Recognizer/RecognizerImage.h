/**
 * @file RecognizerImage.h
 *
 *  Created on: Jun 6, 2015
 *      Author: boris
 */

#ifndef RECOGNIZERIMAGE_HPP
#define RECOGNIZERIMAGE_HPP

#include "RecognizerError.h"
#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
  @struct RecognizerImage
  @brief RecognizerImage data structure. Holds image on which recognition will bw performed.
  */
typedef PP_EXPORTED_TYPE struct RecognizerImage RecognizerImage;

/**
 * @enum RawImageType
 * @brief Enumeration of all possible raw image types
 */
typedef PP_EXPORTED_TYPE enum RawImageType {

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
    RAW_IMAGE_TYPE_NV21,

} RawImageType;

/**
  @memberof RecognizerImage
  @brief Alocates and creates RecognizerImage object from raw image.
  Example:
  @code
    RawImageType imageType = RAW_IMAGE_TYPE_GRAY;
    void *imageBytes = getImageBytes(image);

    RecognizerImage *image;
    RecognizerErrorStatus status = recognizerImageCreateFromRawImage(&image, imageBytes, image.width, image.height,
                                                                       image.bytesPerRow);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* statusString = recognizerErrorToString(status);
        printf("CreateFromRawImage method returned status: %s\n", statusString);

        // handle error
    } else {
        // use RecognizerImage object
    }

    recognizerImageDelete(image);
  @endcode

  Raw image type is the type without any encoding. List of supported raw image types is given in enum RawImageType.
  @see RawImageType

  @param     image               Pointer to pointer referencing the created RecognizerImage object, set to NULL if error occureed.
  @param     input               Pointer to a buffer with raw image pixels
  @param     width               Width of the image, in pixels
  @param     height              Height of the image, in pixels
  @param     bytesPerRow         Number of bytes contained in every row of the image
  @param     rawType             Type of the image. @see RawImageType
  @return    errorStatus         Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                                    Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerImageCreateFromRawImage(RecognizerImage** image, const void* input, int width, int height,
        size_t bytesPerRow, RawImageType rawType);

/**
  @memberof RecognizerImage
  @brief Alocates and creates RecognizerImage object from encoded image
  Image is loaded into memory and in the one of the following encodings:
    - Windows bitmaps - *.bmp, *.dib
    - JPEG files - *.jpeg, *.jpg, *.jpe
    - JPEG 2000 files - *.jp2
    - Portable Network Graphics - *.png
    - TIFF files - *.tiff, *.tif

  @code
    void *image;
    size_t size;
    getImage(&imageBytes, &size);

    RecognizerImage *image;
    RecognizerErrorStatus status = recognizerImageCreateFromEncodedImage(&image, size);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* statusString = recognizerErrorToString(status);
        printf("CreateFromEncodedImage method returned status: %s\n", statusString);

        // handle error
    } else {
        // use RecognizerImage object
    }

    recognizerImageDelete(image);
  @endcode

  @param     image           Pointer to pointer referencing the created RecognizerImage object, set to NULL if error occureed.
  @param     input           Pointer to a buffer where the image is loaded
  @param     size            Size of the image buffer, in bytes
  @return    errorStatus     Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                              Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerImageCreateFromEncodedImage(RecognizerImage** image, const void* input, size_t size);

/**
  @memberof RecognizerImage
  @brief Alocates and creates RecognizerImage object from file
 Image must be encoded in one of the following encodings:
    - Windows bitmaps - *.bmp, *.dib
    - JPEG files - *.jpeg, *.jpg, *.jpe
    - JPEG 2000 files - *.jp2
    - Portable Network Graphics - *.png
    - TIFF files - *.tiff, *.tif

  @code
    const char *file = "Image.png";

    RecognizerImage *image;
    RecognizerErrorStatus status = recognizerImageCreateFromFile(&image, file);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* statusString = recognizerErrorToString(status);
        printf("CreateFromFile method returned status: %s\n", statusString);

        // handle error
    } else {
        // use RecognizerImage object
    }

    recognizerImageDelete(image);
  @endcode

  @param     image          Pointer to pointer referencing the created RecognizerImage object, set to NULL if error occureed.
  @param     filename       Null terminated string, name of file to be loaded.
  @return    errorStatus    Status of the operation. This method should always return RECOGNIZER_ERROR_STATUS_SUCCESS for non-NULL inputs.
                             Returns RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL pointer is given.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerImageCreateFromFile(RecognizerImage** image, const char* filename);

/**
  @memberof RecognizerImage
  @brief Deletes the image object and sets pointer to NULL

  @param    image           Double pointer to image object which is to be deleted.
  @return   errorStatus     Status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS.
                                If NULL pointer was given, status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerImageDelete(RecognizerImage** image);

/**
  @memberof RecognizerImage
  @brief Gets buffer containing raw image bytes

  @param    image       Pointer to RecognizerImage object of interest.
  @param    bytesPerRow Double pointer whose value is set to a buffer with raw image pixels. Youu can use the obtained buffer until recognizerImageDelete
                            is called on the RecognizerImage object. If you want to use the buffer later, you must copy it.
  @return   errorStatus Status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS.
                            If NULL pointer was given, status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerImageGetRawBytes(const RecognizerImage* image, void** data);

/**
  @memberof RecognizerImage
  @brief Gets width of image in number of pixels

  @param    image       Pointer to RecognizerImage object of interest.
  @param    width       Pointer to store given image width in pixels.
  @return   errorStatus Status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS.
                            If NULL pointer was given, status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerImageGetWidth(const RecognizerImage* image, int* width);

/**
  @memberof RecognizerImage
  @brief Gets height of image in number of pixels

  @param    image       Pointer to RecognizerImage object of interest.
  @param    height      Pointer to store given image height in pixels.
  @return   errorStatus Status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS.
                            If NULL pointer was given, status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerImageGetHeight(const RecognizerImage* image, int* height);

/**
  @memberof RecognizerImage
  @brief Gets bytes per row for given image

  @param    image       Pointer to RecognizerImage object of interest.
  @param    bytesPerRow Pointer to store bytes per row of given image.
  @return   errorStatus Status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS.
                            If NULL pointer was given, status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerImageGetBytesPerRow(const RecognizerImage* image, int* bytesPerRow);

/**
  @memberof RecognizerImage
  @brief Gets raw image type for given image

  @param    image       Pointer to RecognizerImage object of interest.
  @param    bytesPerRow Pointer to store raw image type of given image.
  @return   errorStatus Status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS.
                            If NULL pointer was given, status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerImageGetRawImageType(const RecognizerImage* image, RawImageType* rawImageType);

#ifdef __cplusplus
}
#endif

#endif // RECOGNIZERIMAGE_HPP

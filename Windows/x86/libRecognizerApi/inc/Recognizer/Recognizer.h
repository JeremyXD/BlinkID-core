/**
 * @file Recognizer.h
 *
 *  Created on: Oct 9, 2013
 *      Authors: mkorpar, dodo
 */

#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_

#include <stdlib.h>

#include "RecognizerError.h"
#include "RecognizerResultList.h"
#include "RecognizerSettings.h"
#include "RecognizerImage.h"
#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct Recognizer
 * @brief Recognizer data structure.
 *
 * Recognizer object specializes in finding elements (e.g. barcodes) on the image. Which elements are being
 * found is specified by RecognizerSettings object
 */
typedef PP_EXPORTED_TYPE struct Recognizer Recognizer;

/**
  * @struct PPPoint
  * @brief This structure represents the 2D point.
  */
typedef PP_EXPORTED_TYPE struct PPPoint {
    /** x-coordinate of the point */
    int x;
    /** y-coordinate of the point */
    int y;
} PPPoint;

/**
  * @struct PPSize
  * @brief This structure represents the two-dimensional size of an object.
  */
typedef PP_EXPORTED_TYPE struct PPSize {
    /** width */
    int width;
    /** height */
    int height;
} PPSize;

/**
  * @struct PPRectangle
  * @brief This structure represents the rectangle.
  */
typedef PP_EXPORTED_TYPE struct PPRectangle {
    /** horizontal position of the rectangle */
    float x;
    /** vertical position of the rectangle */
    float y;
    /** width of the rectangle */
    float width;
    /** height of the rectangle */
    float height;
} PPRectangle;

/**
  * @enum PPDetectionStatus
  * @brief Defines a status of the object detection process.
  */
typedef PP_EXPORTED_TYPE enum PPDetectionStatus {
    /** Object has been detected successfully */
    DETECTION_STATUS_SUCCESS = 1 << 0,
    /** Detection failed, nothing detected */
    DETECTION_STATUS_FAIL = 1 << 1,
    /** Object detected, but the camera is too far above it */
    DETECTION_STATUS_CAMERA_TOO_HIGH = 1 << 2,
    /** QR code detected */
    DETECTION_STATUS_QR_SUCCESS = 1 << 3,
    /** Object has been detected, but some parts of it are not in image */
    DETECTION_STATUS_PARTIAL_OBJECT = 1 << 6,
    /** Object has been detected, but camera is at too big angle */
    DETECTION_STATUS_CAMERA_AT_ANGLE = 1 << 7,
    /** Object detected, but the camera is too close to the object  */
    DETECTION_STATUS_CAMERA_TOO_NEAR = 1 << 8
} PPDetectionStatus;

/**
* @enum ShowImageType
* @brief Enumeration of types of images returned via onShowImage callback function
*/
typedef PP_EXPORTED_TYPE enum ShowImageType {
    /** original image passed to recognizer */
    SHOW_IMAGE_TYPE_ORIGINAL,
    /** image with position and orientation adjusted and cropped for further processing */
    SHOW_IMAGE_TYPE_DEWARPED,
    /** grayscale image */
    SHOW_IMAGE_TYPE_GRAYSCALE,
    /** image with all color information droped */
    SHOW_IMAGE_TYPE_COLOR_DROP,
    /** final image resulting from a successful scan */
    SHOW_IMAGE_TYPE_SUCCESSFUL_SCAN,
} ShowImageType;

/**
 * @struct RecognizerCallback
 * @brief The RecognizerCallback struct
 *
 * This structure contains pointer to functions that can be called in various parts
 * of recognition process. Functions will be called only if non-NULL function pointer
 * is given. All functions except onDetectionFailed may be called multiple times whilst
 * processing a single image - each recognizer object will call them separately.
 */
typedef PP_EXPORTED_TYPE struct RecognizerCallback {
    /** Called when object detection on image starts. */
    void (*onDetectionStarted)();
    /** Called when object detection on image finishes. If processing should proceed to
     *  recognition of the detected object, method must return non-zero.
     *  @param  points array of points that represent the detected object (usually 4 points, but can be less or
     *          more for some specific object (for example: QR code)). Point coordinates are
     *          in image-based coordinate system, i.e. (0,0) is the coordinate of upper left corner
     *          and (width, height) is the coordinate of lower right corner of the image.
     *          If no points are given, this pointer is NULL.
     *  @param  pointsSize size of the array points
     *  @param  imageSize size of the image given to recognition (convenience in case you want to convert
     *          points to different coordinate system)
     *  @param  detectionStatus status of the object detection. Can be any from the DetectionStatus enum.
     *  @return Non-zero if recognition should proceed. Zero if recognition should not proceed.
     */
    int (*onDetectedObject)(const PPPoint* points, const size_t pointsSize, PPSize imageSize, PPDetectionStatus detectionStatus);
    /** Called when whole chain of recognizers failed and nothing has been detected on image.
     *  Note that this method is called only if all recognizers in recognizer chain fail to
     *  detect anything, while onDetectedObject will be called for each recognizer separately.
     *
     *  For example, if the image contains PDF417 code and recognizer settings say that both QR code
     *  recognizer and PDF417 recognizer should be enabled, then QR code recognizer will call onDetectedObject with
     *  detection status DETECTION_STATUS_FAIL, while PDF417 recognizer will call onDetectedObject with DETECTION_STATUS_SUCCESS
     *  and this method will not be called. On the other hand, if image doesn't contain anything, both QR code recognizer
     *  and PDF417 recognizer will call onDetectedObject with DETECTION_STATUS_FAIL and then this method will be called.
     */
    void (*onDetectionFailed)();
    /** Called when recognition process starts */
    void (*onRecognitionStarted)();
    /** Called when recognition process ends, just before returning from any of the recognizerRecognizeFrom* methods */
    void (*onRecognitionFinished)();


    /** Called inside recognition process. If non-zero is returned, recognition process will stop immediately. */
    int (*onShouldStopRecognition)();
    /** Called inside recognition process and reports the current recognition progress. This method will not
     *  called from all recognizers. */
    void (*onProgress)(int progress);
    /** Called when recognition process produces an image in various stages of recognition. showType parameter
     *  can be used to differentiate between image types so only images that are needed are handled.
     *  @param  data pointer to raw image data
     *  @param  width image width in pixels
     *  @param  height image height in pixels
     *  @param  bytesPerRow number of bytes per row of image pixels
     *  @param  rawType image type. @see RawImageType
     *  @param  showType type of showed image. @see ShowImageType for more information of what kinds of images are available
     *  @param  name image name. Can be NULL.
     */
    void(*onShowImage)(const void* data, int width, int height, size_t bytesPerRow, RawImageType rawType, const ShowImageType showType, const char* name);

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    RecognizerCallback() :
        onDetectionStarted(NULL),
        onDetectedObject(NULL),
        onDetectionFailed(NULL),
        onRecognitionStarted(NULL),
        onRecognitionFinished(NULL),
        onProgress(NULL),
        onShowImage(NULL) {}
#endif
} RecognizerCallback;


/**
 @memberof Recognizer
 @brief Allocates and initializes the recognizer object.
 Example:
 @code
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);

    // configure settings object

    const char* licenseOwner = "here_goes_my_name";
    const char* licenseKey = "here_goes_my_key";
    recognizerSettingsSetLicenseKey(licenseOwner, licenseKey);

    Recognizer* recognizer;
    RecognizerErrorStatus status = recognizerCreate(&recognizer, settings);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* statusString = recognizerErrorToString(status);
        printf("Recognizer create returned status: %s\n", statusString);

        // handle error
    }
 @endcode

 @param     recognizer    Pointer to pointer referencing the created recognizer object
 @param     settings      Required for initializing the recognizer
 @return    errorStatus   status of the operation. The operation might fail, so please
                            check the returned status for possible errors
 */
PP_API RecognizerErrorStatus PP_CALL recognizerCreate(Recognizer** recognizer, const RecognizerSettings* settings);

/**
 @memberof Recognizer
 @brief Deletes the recognizer object and sets a pointer to it to NULL.

 @param     recognizer  Double Pointer to the recognizer object which is to be deleted
 @return    errorStatus status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS. If NULL pointer was given,
                        status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerDelete(Recognizer** recognizer);

/**
  * @memberof Recognizer
  * Sets the scanning region of interest. All subsequent calls to recognizerRecognizeFromRawImage, recognizerRecognizeFromEncodedImage or
  * recognizerRecognizeFromFile will perform scans only in given ROI. ROI must be given with relative dimensions, i.e. all dimensinos of
  * given rectangle must be from interval [0.f, 1.f], where for x coordinate and rectangle width dimension represents the percentage of
  * image widht, and for y coordinate and rectangle height dimension represents the percentage of image height.
  * Call this function with NULL to disable current ROI settings. If any dimension in ROI is larger than 1.f or smaller than 0.f, they
  * will be clamped to interval [0.f, 1.f].
  *
  * @param      recognizer      Pointer to the recognizer object to which ROI will be set
  * @param      roi             Pointer to rectangle that represents the ROI, or NULL to disable ROI.
  * @return     errorStatus     Status of the operation. On success it is RECOGNIZER_ERROR_STATUS_SUCCESS, in case of given NULL pointer for recognizer it is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerSetROI(Recognizer* recognizer, const PPRectangle* roi);


/**
 @memberof Recognizer
 @brief Performs recognition process on a raw image.
 Example:
 @code
    RecognizerResultList *resultList;
    RecognizerErrorStatus status = recognizerRecognizeFromRawImage(recognizer, &resultList, image, 0, NULL);

    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        const char* statusString = recognizerErrorToString(status);
        printf("Recognize method returned status: %s\n", statusString);

        // handle error
    } else {
        // obtain results from RecognizerResultList object
    }

    recognizerResultListDelete(&resultList);
 @endcode

 @param     recognizer          object which performs recognition. Only elements set in the initialization
                                method of recognizer object will be recognized on the image
 @param     resultList          RecognizerResultList object in which the results of the recognition will be stored. This object
                                is allocated and initialized inside this method. On error, resultList is set to NULL.
                                @see RecognizerResultList for description how to obtain results from list and
                                @see RecognizerResult for description how to obtain results from RecognizerResult object.
 @param     image               RecognitionImage object which holds image on which recognition will be performed.
                                @see RecognizerImage to see details on supported image formats.
 @param     imageIsVideoFrame   If non-zero is given, image is treated as video frame. When treating image as video frame,
                                multiple consecutive frame may be combined to yield better recognition result. Note that if
                                consecutive calls to this method with this parameter set as non-zero expect the consecutive
                                frames obtained from camera - all these frames should contain the same object that needs to be
                                recognised. If one of this frames contain different object, total recognition result may be
                                corrupted. To reset recognizer to initial setting, call recognizerReset (@see recognizerReset).
                                Also note that returned recognizer result may be NULL or not valid for frames for which library
                                concludes are too poor to be processed. If you want to force library to process every frame, set
                                this parameter to zero. In that case time redundancy information from consecutive frames will not
                                be used for recognition improvement.
 @param     callback            Pointer to structure that contains pointer to callback functions. If given NULL,
                                no callback will be called. If given non-NULL, only non-NULL function pointers will be called.
                                @see RecognizerCallback
 @return    errorStatus         status of the operation. You should check if it's RECOGNIZER_ERROR_STATUS_SUCCESS before
                                obtaining result values
 */
PP_API RecognizerErrorStatus PP_CALL recognizerRecognizeFromImage(const Recognizer* recognizer, RecognizerResultList** resultList,
        const RecognizerImage* image, int imageIsVideoFrame, const RecognizerCallback* callback);

/**
 * @memberof Recognizer
 * @brief Resets the recognizer to default state.
 *
 *  When using method recognizerRecognizerFromRawImage with parameter imageIsVideoFrame set to non-zero, multiple consecutive video frames
 *  are used to obtain time redundancy information and yield better recognition result. To be able to do that, library will cache some
 *  information from each frame that is later used for boosting the recognition quality. This method will purge that cache. In mobile
 *  version of library this method is usually called when first object is to be recognised or when device gets shaken.
 *
 *  If you are not using video frames for recognition, this method is not neccessary.
 *
 * @param recognizer object which performs recognition.
 * @return status of the operation.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerReset(const Recognizer* recognizer);

/**
@memberof Recognizer
@brief Utility function for loading files to memory buffers.
Loaded buffers MUST be freed after use. @see recognizerFreeFileBuffer
@param     filename        Null terminated string, name of file to be loaded.
@param     buffer          Byte buffer in which to store the file contents. On error, buffer is set to NULL.
@param     bufferSize      Variable that will be set to buffer size in bytes. On error, bufferSize is set to -1.
@return    errorStatus     Status of the operation. You should check if it's RECOGNIZER_ERROR_STATUS_SUCCESS before
using returned buffer.
*/
PP_API RecognizerErrorStatus PP_CALL recognizerLoadFileToBuffer(const char* filename, char** buffer, int* bufferSize);

/**
@memberof Recognizer
@brief Utility function for freeing previously loaded buffers (@see recognizerLoadFileToBuffer) from memory.
@param      buffer          Byte buffer which was alocated with recognizerImageLoadFileToBuffer.
@return     errorStatus     Status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS
*/
PP_API RecognizerErrorStatus PP_CALL recognizerFreeFileBuffer(char** buffer);

#ifdef __cplusplus
}
#endif

#endif

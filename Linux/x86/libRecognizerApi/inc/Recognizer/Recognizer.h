/**
 * @file Recognizer.h
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

#ifndef RECOGNIZER_H_
#define RECOGNIZER_H_

#include "RecognizerError.h"
#include "RecognizerResultList.h"
#include "Recognizer/RecognizerSettingsFwd.h"
#include "RecognizerImage.h"
#include "Export.h"

#include <stdlib.h>

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
struct Recognizer;

/**
 * @brief Typedef for Recognizer structure.
 */
typedef MB_EXPORTED_TYPE struct Recognizer Recognizer;

/**
  * @struct MBPoint
  * @brief This structure represents the 2D point.
  */
struct MBPoint {
    /** x-coordinate of the point */
    int x;
    /** y-coordinate of the point */
    int y;

#ifdef __cplusplus
    MBPoint() : x( 0 ), y( 0 ) {}
#endif
};

/**
 * @brief Typedef for MBPoint structure.
 */
typedef MB_EXPORTED_TYPE struct MBPoint MBPoint;

/**
  * @struct MBSize
  * @brief This structure represents the two-dimensional size of an object.
  */
struct MBSize {
    /** width */
    int width;
    /** height */
    int height;

#ifdef __cplusplus
    MBSize() : width( 0 ), height( 0 ) {}
#endif
};

/**
 * @brief Typedef for MBSize structure.
 */
typedef MB_EXPORTED_TYPE struct MBSize MBSize;

/**
  * @struct MBRectangle
  * @brief This structure represents the rectangle.
  */
struct MBRectangle {
    /** horizontal position of the rectangle */
    float x;
    /** vertical position of the rectangle */
    float y;
    /** width of the rectangle */
    float width;
    /** height of the rectangle */
    float height;

#ifdef __cplusplus
    MBRectangle() : x( 0 ), y( 0 ), width( 0 ), height( 0 ) {}
#endif
};

/**
 * @brief Typedef for MBRectangle structure.
 */
typedef MB_EXPORTED_TYPE struct MBRectangle MBRectangle;

/**
  * @enum MBDetectionStatus
  * @brief Defines a status of the object detection process.
  */
enum MBDetectionStatus {
    /** Object has been detected successfully */
    DETECTION_STATUS_SUCCESS = 1 << 0,
    /** Detection failed, nothing detected */
    DETECTION_STATUS_FAIL = 1 << 1,
    /** Object detected, but the camera is too far above it */
    DETECTION_STATUS_CAMERA_TOO_HIGH = 1 << 2,
    /** Object has been detected, but some parts of it are not in image */
    DETECTION_STATUS_PARTIAL_OBJECT = 1 << 6,
    /** Object has been detected, but camera is at too big angle */
    DETECTION_STATUS_CAMERA_AT_ANGLE = 1 << 7,
    /** Object detected, but the camera is too close to the object  */
    DETECTION_STATUS_CAMERA_TOO_NEAR = 1 << 8
};

/**
 * @brief Typedef for MBDetectionStatus enum.
 */
typedef MB_EXPORTED_TYPE enum MBDetectionStatus MBDetectionStatus;

/**
* @enum ShowImageType
* @brief Enumeration of types of images returned via onShowImage callback function
*/
enum ShowImageType {
    /** original image passed to recognizer */
    SHOW_IMAGE_TYPE_ORIGINAL,
    /** image with position and orientation adjusted and cropped for further processing */
    SHOW_IMAGE_TYPE_DEWARPED,
    /** final image resulting from a successful scan */
    SHOW_IMAGE_TYPE_SUCCESSFUL_SCAN,
};

/**
 * @brief Typedef for ShowImageType enum.
 */
typedef MB_EXPORTED_TYPE enum ShowImageType ShowImageType;

/**
 * @struct RecognizerCallback
 * @brief The RecognizerCallback struct
 *
 * This structure contains pointer to functions that can be called in various parts
 * of recognition process. Functions will be called only if non-NULL function pointer
 * is given. All functions except onDetectionFailed may be called multiple times whilst
 * processing a single image - each recognizer object will call them separately.
 */
struct RecognizerCallback {
    /** Called when object detection on image starts. */
    void (*onDetectionStarted)();
    /**
     * Called when the recognizer is midway the detection. At that point
     * some points could be drawn on to the frame as a sign of something happening.
     *
     *  @param  points array of points that represent the detected object (usually 4 points, but can be less or
     *          more for some specific object (for example: QR code)). Point coordinates are
     *          in image-based coordinate system, i.e. (0,0) is the coordinate of upper left corner
     *          and (width, height) is the coordinate of lower right corner of the image.
     *          If no points are given, this pointer is NULL.
     *  @param  pointsSize size of the array points
     *  @param  imageSize size of the image given to recognition (convenience in case you want to convert
     *          points to different coordinate system)
     *
     */
    void (*onDetectionMidway)( const MBPoint* points, const size_t pointsSize, MBSize imageSize );
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
    int (*onDetectedObject)(const MBPoint* points, const size_t pointsSize, MBSize imageSize, MBDetectionStatus detectionStatus);
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

    /** Called when recognition process produces an image in various stages of recognition. showType parameter
     *  can be used to differentiate between image types so only images that are needed are handled.
     *	@param	image       returned image. Image pointer is valid only during the scope of the function. If you need it later, you should
     *                      copy the image with ::recognizerImageCreateCopyFromImage.
     *  @param  showType    type of shown image.
     *  @param  name        image name. Can be NULL.
     *
     *  @see ShowImageType for more information of what kinds of images are available
     */
    void (*onShowImage)(const RecognizerImage* image, const ShowImageType showType, const char* name);

#ifdef __cplusplus
    /**
     * Default constructor for c++.
     */
    RecognizerCallback() :
        onDetectionStarted(NULL),
        onDetectionMidway(NULL),
        onDetectedObject(NULL),
        onDetectionFailed(NULL),
        onRecognitionStarted(NULL),
        onRecognitionFinished(NULL),
        onShowImage(NULL) {}
#endif
};

/**
 * @brief Typedef for RecognizerCallback structure.
 */
typedef MB_EXPORTED_TYPE struct RecognizerCallback RecognizerCallback;


/**
 * @brief Returns the library version string.
 * @returns library version string
 */
MB_API const char* MB_CALL recognizerGetVersionString();

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
MB_API RecognizerErrorStatus MB_CALL recognizerCreate(Recognizer** recognizer, const RecognizerSettings* settings);

/**
  @memberof Recognizer
  @brief Updates recognizer object's settings.
  You can use this method to change what recognizers are active. You cannot use this method to change options
  like device info, OCR model etc.

  @param    recognizer  recognizer object that will be updated with new settings
  @param    settings    settings that will be applied
  @return   errorStatus status of the operation. The operation might fail, so please check returned status for possible errors.
  */
MB_API RecognizerErrorStatus MB_CALL recognizerUpdateSettings(Recognizer* recognizer, const RecognizerSettings* settings);

/**
 @memberof Recognizer
 @brief Deletes the recognizer object and sets a pointer to it to NULL.

 @param     recognizer  Double Pointer to the recognizer object which is to be deleted
 @return    errorStatus status of the operation. If deletion was successfuly, status will be RECOGNIZER_ERROR_STATUS_SUCCESS. If NULL pointer was given,
                        status will be RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerDelete(Recognizer** recognizer);

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
MB_API RecognizerErrorStatus MB_CALL recognizerSetROI(Recognizer* recognizer, const MBRectangle* roi);


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
                                See RecognizerResultList for description how to obtain results from list and
                                See RecognizerResult for description how to obtain results from RecognizerResult object.
 @param     image               RecognizerImage object which holds image on which recognition will be performed.
                                See RecognizerImage to see details on supported image formats.
 @param     imageIsVideoFrame   If non-zero is given, image is treated as video frame. When treating image as video frame,
                                multiple consecutive frame may be combined to yield better recognition result. Note that if
                                consecutive calls to this method with this parameter set as non-zero expect the consecutive
                                frames obtained from camera - all these frames should contain the same object that needs to be
                                recognised. If one of this frames contain different object, total recognition result may be
                                corrupted. To reset recognizer to initial setting, call ::recognizerReset.
                                Also note that returned recognizer result may be NULL or not valid for frames for which library
                                concludes are too poor to be processed. If you want to force library to process every frame, set
                                this parameter to zero. In that case time redundancy information from consecutive frames will not
                                be used for recognition improvement.
 @param     callback            Pointer to structure that contains pointer to callback functions. If given NULL,
                                no callback will be called. If given non-NULL, only non-NULL function pointers will be called.
 @return    errorStatus         status of the operation. You should check if it's RECOGNIZER_ERROR_STATUS_SUCCESS before
                                obtaining result values
 @see RecognizerCallback for details about what callbacks can be installed
 @see RecognizerResult for description how to obtain results from RecognizerResult object
 @see RecognizerResultList for description how to obtain results from list
 @see RecognizerImage to see details on supported image formats.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerRecognizeFromImage(const Recognizer* recognizer, RecognizerResultList** resultList,
        const RecognizerImage* image, int imageIsVideoFrame, const RecognizerCallback* callback);

/**
 * @memberof Recognizer
 * @brief Cancels the current recognition process (if any).
 * If there is a ongoing recognition process (i.e. ongoing call to recognizerRecognizeFromImage), this function will cancel the process.
 * The ongoing recognition will return the results it had at the time of cancelling (or nothing if nothing was recognized yet).
 * If there is no ongoing recognition process, this function does nothing (it WILL NOT prevent recognition of next image given with call
 * to recognizerRecognizeFromImage that follows at any time after calling this function).
 * @param recognizer Recognizer object whose recognition process should be cancelled.
 * @return RECOGNIZER_ERROR_STATUS_SUCCESS if process was cancelled, RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL if NULL is given as parameter
 */
MB_API RecognizerErrorStatus MB_CALL recognizerCancelCurrentRecognition( const Recognizer* recognizer );

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
MB_API RecognizerErrorStatus MB_CALL recognizerReset(const Recognizer* recognizer);

#ifdef __cplusplus
}
#endif

#endif

/**
 * @file DocumentDetectorSettings.h
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
#ifndef DOCUMENT_DETECTOR_SETTINGS_INCLUDED
#define DOCUMENT_DETECTOR_SETTINGS_INCLUDED

#include "DocumentSpecification.h"
#include "Recognizer/Detector/DetectorSettingsFwd.h"
#include "Recognizer/Export.h"
#include "Recognizer/RecognizerError.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct DocumentDetectorSettings
 * @brief Settings for document detector.
 * Document detector can be used to detect card documents, cheques, papers etc.
 */
struct DocumentDetectorSettings {
    /**
     * @brief Array of document specifications that define which documents can be detected.
     * This array must not be NULL nor empty.
     */
    DocumentSpecification* documentSpecifications;

    /**
     * @brief Size of documentSpecifications array.
     */
    size_t documentSpecificationsCount;

    /**
     * @brief minimum number of stable detections to return detector result
     * Available only when performing recognition of video stream. The detector
     * will not return the detection result until at least this number of
     * detections have been found in consecutive video frames. When performing
     * detection on still image, this is always interpreted as 1, regardless
     * of this setting.
     */
    unsigned int numStableDetectionsThreshold;
#ifdef __cplusplus
    DocumentDetectorSettings() :
        documentSpecifications( NULL ),
        documentSpecificationsCount( 0 ),
        numStableDetectionsThreshold( 1 )
    {}
#endif
};

/**
 * @brief Typedef for DocumentDetectorSettings structure
 */
typedef MB_EXPORTED_TYPE struct DocumentDetectorSettings DocumentDetectorSettings;

/**
 * @memberof DetectorSettings
 * @brief Creates the DetectorSettings structure from given DocumentDetectorSettings.
 * The DetectorSettings is suitable for setting into BlinkInputSettings structure and contains
 * internal copies of all elements - after calling this function you can free the DocumentDetectorSettings
 * pointer.
 * The DetectorSettings structure should be released after usage with detectorSettingsTerminate function.
 * In case of error (invalid settings given or invalid DocumentSpecification inside documentSpecifications array),
 * detectorSettings parameter will be set to NULL and RECOGNIZER_ERROR_STATUS_INVALID_ARGUMENT will be returned.
 * @param detectorSettings          Output parameter which will point to created DetectorSettings or to NULL on error.
 * @param documentDetectorSettings  DocumentDetectorSettings structure from which native DetectorSettings object will be created.
 * @return  status of the operation
 */
MB_API RecognizerErrorStatus MB_CALL detectorSettingsCreateFromDocumentDetectorSettings( DetectorSettings** detectorSettings, const DocumentDetectorSettings* documentDetectorSettings );

#ifdef __cplusplus
}
#endif

#endif

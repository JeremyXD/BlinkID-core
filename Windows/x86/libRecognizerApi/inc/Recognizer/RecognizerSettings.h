/**
 * @file RecognizerSettings.h
 *
 *  Created on: Oct 9, 2013
 *      Authors: mkorpar, dodo
 */

#ifndef RECOGNIZERSETTINGS_H_
#define RECOGNIZERSETTINGS_H_

#include "RecognizerError.h"
#include "RecognizerDeviceInfo.h"
#include "Export.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 @struct RecognizerSettings
 @brief Recognizer settings data structure
 
 Used for initializing the Recognizer object.
 @see Recognizer.h
 */
typedef PP_EXPORTED_TYPE struct RecognizerSettings RecognizerSettings;

/**
 @memberof RecognizerSettings
 @brief Allocates and initializes the RecognizerSettings object.
 Example:
 @code
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);
 @endcode

 @param     settings    Pointer to pointer referencing the created RecognizerSettings object
 @return    errorStatus status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsCreate(RecognizerSettings** settings);
    
/**
 @memberof RecognizerSettings
 @brief Deletes the RecognizerSettings object and sets the settings pointer to NULL.
 Example:
 @code
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);
    recognizerSettingsDelete(&settings);
 @endcode
 
 @param     settings    Double Pointer to the RecognizerSettings object which is to be deleted
 @return    errorStatus status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS, except when settings is NULL pointer when status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsDelete(RecognizerSettings** settings);

/**
  @memberof RecognizerSettings
  @brief Sets the license key required for initialization of Recognizer object.

  @param     settings      Pointer to the RecognizerSettings object that holds all settings information.
  @param     licenseOwner  You should enter here the licensee name to which license key is bound.
  @param     licenseKey    License key used for unlocking the library.
  @return    errorStatus   Status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS, except when settings is NULL pointer when status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
                            License check will be perfomed while creating recognizer object with recognizerCreate method, not here.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetLicenseKey(RecognizerSettings* settings, const char* licenseOwner, const char* licenseKey);

/**
  * @memberof RecognizerSettings
  * @brief Sets whether or not outputting of multiple scan results from same image is allowed.
  *
  * If that is true, it is possible to return multiple recognition results from same image (e.g. both PDF417 and QR code from same image can be decoded). By default, this
  * option is true. If you turn it off, then RecognizerResultList outputted by recognizerRecognize* functions will contain at most 1 result.
  * The upside of turning that option off is the speed - if you enable lots of recognizers, as soon as the first recognizer succeeds in
  * recognizing the object, function will return and other recognizers will not get a chance to analyze the image. The downside is that
  * you are then unable to obtain multiple results from single image.
  *
  * @param     settings      Pointer to the RecognizerSettings object that holds all settings information.
  * @param     outputMulti   true for enabling output of multiple results, false otherwise
  * @return    errorStatus   Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetOutputMultipleResults(RecognizerSettings* settings, int outputMulti);


/**
  * @memberof RecognizerSettings
  * @brief Sets the device information.
  * Device information holds information about name of the device, number of processors for optimal multithreaded support, CPU frequency etc.
  * This method replaces old recognizerSettingsSetNumberOfProcessors function.
  * Example:
  * @code
  * RecognizerSettings* settings;
  * RecognizerDeviceInfo* deviceInfo;
  * recognizerSettingsCreate(&settings);
  * recognizerDeviceInfoCreate(&deviceInfo);
  * recognizerDeviceSetNumberOfProcessors(deviceInfo, 2);
  * recognizerSettingsSetDeviceInfo(settings, deviceInfo);
  * @endcode
  *
  * @param      settings    pointer to RecognizerSettings object that holds all settings information
  * @param      deviceInfo  pointer to RecognizerDeviceInfo object that holds all device information
  * @return     errorStatus status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetDeviceInfo(RecognizerSettings* settings, const RecognizerDeviceInfo* deviceInfo);



/**
 @memberof RecognizerSettings
 @brief Sets the byte array for ocr model used in character recognition.
 Example:
 @code
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);
    recognizerSettingsSetZicerModel(settings, data, size);
 @endcode
 
 @param     settings    pointer to RecognizerSettings object that holds all settings information
 @param     data        pointer to byte array with loaded ocr model data
 @param     size        size of the data
 @return    errorStatus status of the operation. Here it's always RECOGNIZER_ERROR_STATUS_SUCCESS, except when settings is NULL pointer when status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL.
 */
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetZicerModel(RecognizerSettings* settings, const void* data, size_t size);













/**
* @struct MRTDSettings
* @brief Settings for configuring Machine Readable Travel Document recognizer.
*/
struct MRTDSettings {
	/**
	* Enables detection of Machine Readable Zone position
	*/
    int detectMachineReadableZonePosition;

	/**
	* Turns on calling the showImage callback with image of dewarped MRZ.
	* IMPORTANT detectMachineReadableZonePosition MUST be enabled if you
	* want to turn this feature on!
	* You can use this and showFullDocument simultaneously.
	*/
	int showMachineReadableZone;

	/**
	* Turns on calling the showImage callback with image of dewarped document.
	* IMPORTANT detectMachineReadableZonePosition MUST be enabled if you
	* want to turn this feature on!
	* You can use this and showMachineReadableZone simultaneously.
	*/
	int showFullDocument;

#ifdef __cplusplus
	/**
	* Default constructor for c++.
	*/
	MRTDSettings() :
        detectMachineReadableZonePosition(0), showMachineReadableZone(0), showFullDocument(0) {}
#endif
};

PP_EXPORTED_TYPE typedef struct MRTDSettings MRTDSettings;

/**
* @memberof RecognizerSettings
* @brief Set settings for Machine Readable Travel Document recognizer.
* @param settings pointer to RecognizerSettings object that holds all settings information
* @param mrtdSettings pointer to MRTD recognizer settings object that contains information how MRTD recognizer should behave or NULL to disable MRTD recognizer
* @return status of the operation. If settings is NULL, status is RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL, otherwise RECOGNIZER_ERROR_STATUS_SUCCESS.
*/
PP_API RecognizerErrorStatus PP_CALL recognizerSettingsSetMRTDSettings(RecognizerSettings* settings, const MRTDSettings* mrtdSettings);






#ifdef __cplusplus
}
#endif

#endif

/**
 * @file RecognizerSettings.h
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

#ifndef RECOGNIZERSETTINGS_H_
#define RECOGNIZERSETTINGS_H_

#include "RecognizerError.h"
#include "Export.h"
#include "Recognizer.h"
#include "Recognizer/RecognizerSettingsFwd.h"

#include "Recognizer/BlinkBarcode/Pdf417/Pdf417Settings.h"
#include "Recognizer/BlinkBarcode/BarDecoder/BarDecoderSettings.h"
#include "Recognizer/BlinkBarcode/ZXing/ZXingSettings.h"
#include "Recognizer/BlinkBarcode/USDL/USDLSettings.h"
#include "Recognizer/BlinkID/Malaysia/MyKad/MyKadSettings.h"
#include "Recognizer/BlinkID/Malaysia/iKad/IKadSettings.h"
#include "Recognizer/BlinkID/MRTD/MRTDSettings.h"
#include "Recognizer/BlinkInput/BlinkInputSettings.h"

#ifdef __cplusplus
extern "C" {
#endif

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
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsCreate(RecognizerSettings** settings);
    
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
 @return    errorStatus status of the operation.
 */
MB_API RecognizerErrorStatus MB_CALL recognizerSettingsDelete(RecognizerSettings** settings);

/**
  @memberof RecognizerSettings
  @brief Sets the license key and licensee required for initialization of Recognizer object.
  NOTE: License check will be performed while creating Recognizer object with recognizerCreate function, not here.

  @param     settings      Pointer to the RecognizerSettings object that holds all settings information.
  @param     licensee      You should enter here the licensee name to which license key is bound. Make sure pointer is valid at the time recognizerCreate is called.
  @param     licenseKey    License key used for unlocking the library. Make sure pointer is valid at the time recognizerCreate is called.
  */
MB_API void MB_CALL recognizerSettingsSetLicenseKeyForLicensee(RecognizerSettings* settings, const char* licensee, const char* licenseKey);

/**
  @memberof RecognizerSettings
  @brief Sets the license key required for initialization of Recognizer object.
  NOTE: License check will be performed while creating Recognizer object with recognizerCreate function, not here.

  @param     settings      Pointer to the RecognizerSettings object that holds all settings information.
  @param     licenseKey    License key used for unlocking the library. Make sure pointer is valid at the time recognizerCreate is called.
  */
MB_API void MB_CALL recognizerSettingsSetLicenseKey(RecognizerSettings* settings, const char* licenseKey);

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
  */
MB_API void MB_CALL recognizerSettingsSetOutputMultipleResults(RecognizerSettings* settings, int outputMulti);

/**
 * @memberof RecognizerSettings
 * @brief Gets whether or not outputting of multiple scan results from scan image is allowed.
 * @see recognizerSettingsSetOutputMultipleResults
 * @param settings Pointer to RecognizerSettings object that holds all settings information.
 * @return zero if outputting of multiple results is not allowed, non-zero otherwise
 */
MB_API int MB_CALL recognizerSettingsGetOutputMultipleResults( const RecognizerSettings* settings );

/**
 @memberof RecognizerSettings
 @brief Sets the location where required resources will be loaded from. If not set, "res" folder will be used.
 NOTE: This function will not make a copy of given string, it will only save the pointer to it. Make sure the given
       char buffer is alive during whole recognition process, since resources are lazy loaded during recognition.
 Example:
 @code
    RecognizerSettings* settings;
    recognizerSettingsCreate(&settings);
    recognizerSettingsSetResourcesLocation(settings, "/path/to/res");
 @endcode
 
 @param     settings     pointer to RecognizerSettings object that holds all settings information
 @param     resourcePath path to resource folder
 */
MB_API void MB_CALL recognizerSettingsSetResourcesLocation(RecognizerSettings* settings, const char* resourcePath);


#ifdef __cplusplus
}
#endif



#endif

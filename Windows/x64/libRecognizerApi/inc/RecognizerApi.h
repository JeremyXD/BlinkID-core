/**
 * @file RecognizerApi.h
 *
 * @mainpage %Recognizer API documentation
 *
 * @section Introduction
 *
 * This documentation gives an overview how to use %Recognizer scanning library. %Recognizer library is supported
 * on several platforms: Android, iOS, Mac OS X, Linux and Windows. This documentation covers the usage of
 * %Recognizer API to perform scanning of images and retrieving the scan results. For information about platform
 * specific integration, refer to platform specific README document.
 *
 * @section quickStart Quick start
 *
 * In order to perform recognition, you first need to initialize a master Recognizer object. Master Recognizer object
 * internally contains list of multiple recognizer objects (i.e. recognizers) that are specific for each object scanned.
 * Each recognizer has its own settings object that configures its behaviour. This section only briefly covers the simplest
 * usage. For more elaborate example, see the demo app provided with the library.
 *
 * -# Initialize the RecognizerSettings object and add license key to it
 *    @code
 *      RecognizerSettings* settings;
 *      recognizerSettingsCreate(&settings);
 *      recognizerSettingsSetLicenseKeyForLicensee(settings, "add licensee here", "add license key here");
 *    @endcode
 *    Please note that license key evaluation is done when creating the Recognizer, not when inserting it into RecognizerSettings object.
 * -# Define settings for each recognizer you want to use and add those settings to RecognizerSettings object. To see all possible recognizer
 *    settings and documentation for them, refer to RecognizerSettings.h file.
 *    For example, to enable PDF417 recognizer, QR code scanning via ZXing recognizer and US Driver's License recognizer, do the following
 *    @code
 *      Pdf417Settings pdf417Sett;
 *      pdf417Sett.useAutoScale = 1;
 *      pdf417Sett.shouldScanUncertain = 1;
 *      recognizerSettingsSetPdf417Settings(settings, &pdf417Sett);
 *
 *      ZXingSettings zxingSett;
 *      zxingSett.scanQRCode = 1;
 *      recognizerSettingsSetZXingSettings(settings, &zxingSett);
 *
 *      UsdlSettings usdlSett;
 *      usdlSett.useAutoScale = 1;
 *      recognizerSettingsSetUsdlSettings(settings, &usdlSett);
 *    @endcode
 *    Please note that some setting might be ignored if recognizers they configured are disabled by license key.
 *    You can disable certain recognizer by setting its settings to NULL. For example, to disable ZXing recognizer
 *    do the following:
 *    @code
 *      recognizerSettingsSetZXingSettings(settings, NULL);
 *    @endcode
 * -# Create the Recognizer object
 *    @code
 *      Recognizer* recognizer;
 *      RecognizerErrorStatus status = recognizerCreate(&recognizer, settings);
 *
 *      // Delete settings object
 *      recognizerSettingsDelete(settings);
 *
 *      if (status == RECOGNIZER_ERROR_STATUS_SUCCESS) {
 *          // perform the scan
 *
 *      } else {
 *          const char* statusString = recognizerErrorToString(status);
 *          printf("Recognizer create returned status: %s\n", statusString);
 *
 *          // handle error
 *      }
 *    @endcode
 *    Creation of Recognizer might fail. For that reason you should check the status of ::recognizerCreate method.
 * -# Perform the scan on the image. Image first needs to be created from file or from memory. To create image from file,
 *    use ::recognizerImageCreateFromFile. To create image from memory use either ::recognizerImageCreateFromRawImage or ::recognizerImageCreateFromEncodedImage.
 *    @code
 *      RecognizerImage* img;
 *      RecognizerErrorStatus status = recognizerImageCreateFromFile(&img, "barcode.jpg");
 *      if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
 *          printf("Failed to load file. Reason: %s", recognizerErrorToString(status));
 *      }
 *    @endcode
 * -# Once you have created an image, you can perform recognition using method ::recognizerRecognizeFromImage.
 *    @code
 *      RecognizerResultList* resultList;
 *      RecognizerErrorStatus status = recognizerRecognizeFromImage(recognizer, &resultList, img, 0, NULL);
 *
 *      if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
 *          printf("Recognizer error %s", recognizerErrorToString(status));
 *      }
 *    @endcode
 *    The output of recognition methods is list of recognition results. List can contain one or more recognition results, depending on the setting
 *    defined with method ::recognizerSettingsSetOutputMultipleResults of RecognizerSettings object.
 * -# Iterate over list of recognition results and obtain information from each recognition result. Note that methods for information retrieval
 *    may depend on the type of the recognition result. For all available information retrieval methods and information about when certain
 *    method is applicable, see the complete reference for RecognizerResult data structure.
 *    The following example iterates over all elements in recognition result list and checks if RecognitionResult is generated by US Driver's License
 *    recognizer. If yes and result is valid, output driver's first and last name. If RecognitionResult is not generated by US Driver's License
 *    recognizer, assume it was generated by any barcode recognizer (@ref Pdf417Settings "Pdf417", @ref ZXingSettings "ZXing" or @ref BarDecoderSettings "BarDecoder",
 *    also see RecognizerSettings) and output barcode type and barcode string contents.
 *    @code
 *      size_t numResults;
 *      recognizerResultListGetNumOfResults(resultList, &numResults);
 *
 *      for(i = 0; i< numResults; ++i) {
 *          RecognizerResult* result;
 *          int isUsdl = 0;
 *
 *          recognizerResultListGetResultAtIndex(resultList, i, &result);
 *
 *          status = recognizerResultIsUSDLResult(result, &isUsdl);
 *          if(status == RECOGNIZER_ERROR_STATUS_SUCCESS && isUsdl) {
 *              int valid = 0;
 *
 *              recognizerResultIsResultValid(result, &valid);
 *              if(valid) {
 *                  const char* firstName;
 *                  const char* lastName;
 *                  recognizerResultGetUSDLField(result, &firstName, USDLFieldKeys.kCustomerFirstName);
 *                  recognizerResultGetUSDLField(result, &lastName, USDLFieldKeys.kCustomerFamilyName);
 *
 *                  printf("Driver's license belongs to %s %s\n", firstName, lastName);
 *              } else {
 *                  printf("Invalid driver license result!\n");
 *              }
 *
 *          } else {
 *              const char* str;
 *              BarcodeType barcodeType;
 *
 *              status = recognizerResultGetBarcodeType(result, &barcodeType);
 *
 *              if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
 *                  printf("Error obtaining barcode type: %s\n", recognizerErrorToString(status));
 *                  return -1;
 *              }
 *
 *              status = recognizerResultGetBarcodeStringData(result, &str);
 *
 *              if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
 *                  printf("Error obtaining barcode string data: %s\n", recognizerErrorToString(status));
 *                  return -1;
 *              }
 *
 *              printf("%s Result: %s\n", barcodeTypeToString(barcodeType), str);
 *          }
 *      }
 *    @endcode
 * -# Finally, when done, clean the memory. Each structure has method for releasing it.
 *    @code
 *      recognizerImageDelete(&img);
 *      recognizerResultListDelete(&resultList);
 *      recognizerSettingsDelete(&settings);
 *      recognizerDelete(&recognizer);
 *    @endcode
 *
 * @section demo Demo application
 *
 * Each platform contains its own demo application specific for that platform. Refer to its source code and documentation for more information.
 *
 * @section info Additional info
 *
 * For any inquiries, additional information or instructions please contact us at <a href="http://help.microblink.com">help.microblink.com</a>.
 * When contacting, please state which product and which platform you are using so we can help you more quickly. Also, please state that you are using
 * core components RecognizerAPI and state the version you are using. You can obtain the library version with function ::recognizerGetVersionString.
 */

#ifndef RECOGNIZERAPI_H_
#define RECOGNIZERAPI_H_

#include "Recognizer/Recognizer.h"
#include "Recognizer/RecognizerImage.h"
#include "Recognizer/RecognizerResult.h"
#include "Recognizer/RecognizerSettings.h"
#include "Recognizer/RecognizerError.h"
#include "Recognizer/RecognizerImageProcessor.h"
#include "Recognizer/RecognizerBarrelDewarper.h"

#endif

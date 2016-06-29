/**
 *  @file RecognizerDeviceInfo.h
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

#ifndef RECOGNIZERDEVICEINFO_HPP
#define RECOGNIZERDEVICEINFO_HPP

#include "RecognizerError.h"
#include "Export.h"
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
  * @struct RecognizerDeviceInfo
  * @brief Holds the information about current device.
  *
  * RecognizerDeviceInfo structure holds the information about current device. This can help recognizer library to better choose recognition parameters, i.e. if library
  * recognizes specific device, it can adjust its recognition parameters specifically for that device. If device is not recognized, default parameters will be used.
  */
typedef PP_EXPORTED_TYPE struct RecognizerDeviceInfo RecognizerDeviceInfo;

/** defines the name for Android operating system */
extern PP_API const char* RECOGNIZER_OS_ANDROID;
/** defines the name for iOS operating system */
extern PP_API const char* RECOGNIZER_OS_IOS;
/** defines the name for Windows Phone operating system */
extern PP_API const char* RECOGNIZER_OS_WINDOWS_PHONE;
/** defines the name for Windows operating system */
extern PP_API const char* RECOGNIZER_OS_WINDOWS;
/** defines the name for Linux operating system */
extern PP_API const char* RECOGNIZER_OS_LINUX;
/** defines the name for MacOS X operating system */
extern PP_API const char* RECOGNIZER_OS_MACOSX;


/**
  * @memberof RecognizerDeviceInfo
  * @brief Creates new recognizer device info structure that will hold information about current device.
  *
  * @param  deviceInfo  [out] Pointer to pointer referencing the created recognizer device info object.
  * @return errorStatus       Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoCreate(RecognizerDeviceInfo** deviceInfo);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Deletes the recognizer device info object and sets its pointer to NULL.
  *
  * @param  deviceInfo      Pointer to pointer referencing the recognizer device info object that is to be deleted.
  * @return errorStatus     Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoDelete(RecognizerDeviceInfo** deviceInfo);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets the operating system name.
  * Name must not be NULL, but can be any string. Recognized strings are string constants RECOGNIZER_OS_ANDROID,
  * RECOGNIZER_OS_IOS, RECOGNIZER_OS_WINDOWS_PHONE, RECOGNIZER_OS_WINDOWS, RECOGNIZER_OS_LINUX, RECOGNIZER_OS_MACOSX.
  * Example:
      @code
       RecognizerDeviceInfo* di;
       recognizerDeviceInfoCreate(&di);
       recognizerDeviceInfoSetOperatingSystem(di, RECOGNIZER_OS_ANDROID);
      @endcode
  *
  * @param      deviceInfo          Pointer to device info object that hold device information.
  * @param      operatingSystemName Name of the operating system of the device. Must not be NULL.
  * @return     errorStatus         Status of the operation.
  *
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetOperatingSystem(RecognizerDeviceInfo* deviceInfo, const char* operatingSystemName);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets the operating system name that is stored in device info structure.
  *
  * @param      deviceInfo              Pointer to device info object that hold device information.
  * @param      operatingSystemName    [out]   Pointer to string where name will be stored. No memory will be allocated. Make sure you keep the device info object
  *                                             alive as long as you use the string returned from this function.
  * @return     errorStatus             Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoGetOperatingSystem(const RecognizerDeviceInfo* deviceInfo, const char** operatingSystemName);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets the operating system version string. String must not be NULL, but can be any string. Version will be recognized if string is in format major.minor.
  * Example:
  * @code
  * RecognizerDeviceInfo* di;
  * recognizerDeviceInfoCreate(&di);
  * recognizerDeviceInfoSetOperatingSystem(di, RECOGNIZER_OS_ANDROID);
  * recognizerDeviceInfoSetOperatingSystemVersion(di, "4.4.2");
  * @endcode
  *
  * @param      deviceInfo      Pointer to device info object that hold device information.
  * @param      osVersion       Version string of the operating system.
  * @return     errorStatus     Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetOperatingSystemVersion(RecognizerDeviceInfo* deviceInfo, const char* osVersion);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets the operating system version string that is stored in device info structure.
  *
  * @param      deviceInfo      Pointer to device info object that hold device information.
  * @param      osVersion       [out]   Pointer to string where version string will be stored. No memory will be allocated. Make sure you keep the device info object
  *                                     alive as long as you use the string returned from this function.
  * @return     errorStatus     Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoGetOperatingSystemVersion(const RecognizerDeviceInfo* deviceInfo, const char** osVersion);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets the device name. String must not be NULL.
  * Example:
  * @code
  * RecognizerDeviceInfo* di;
  * recognizerDeviceInfoCreate(&di);
  * recognizerDeviceInfoSetOperatingSystem(di, RECOGNIZER_OS_ANDROID);
  * recognizerDeviceInfoSetOperatingSystemVersion(di, "4.4.2");
  * recognizerDeviceInfoSetDeviceName(di, "mako");
  * @endcode
  *
  * @param      deviceInfo      Pointer to device info object that hold device information.
  * @param      deviceName      Name of the device.
  * @return     errorStatus     Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetDeviceName(RecognizerDeviceInfo* deviceInfo, const char* deviceName);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets the device name stored in device info structure.
  *
  * @param      deviceInfo      Pointer to device info object that hold device information.
  * @param      deviceName      [out]   Pointer to string where device name will be stored. No memory will be allocated. Make sure you keep the device info object
  *                                     alive as long as you use the string returned from this function.
  * @return     errorStatus     Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoGetDeviceName(const RecognizerDeviceInfo* deviceInfo, const char** deviceName);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets the device model. String must not be NULL.
  * Example:
  * @code
  * RecognizerDeviceInfo* di;
  * recognizerDeviceInfoCreate(&di);
  * recognizerDeviceInfoSetOperatingSystem(di, RECOGNIZER_OS_ANDROID);
  * recognizerDeviceInfoSetOperatingSystemVersion(di, "4.4.2");
  * recognizerDeviceInfoSetDeviceName(di, "mako");
  * recognizerDeviceInfoSetDeviceModel(di, "Nexus 4");
  * @endcode
  *
  * @param      deviceInfo      Pointer to device info object that hold device information.
  * @param      deviceModel     Model of the device.
  * @return     errorStatus     Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetDeviceModel(RecognizerDeviceInfo* deviceInfo, const char* deviceModel);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets the device model stored in device info structure.
  *
  * @param      deviceInfo      Pointer to device info object that hold device information.
  * @param      deviceModel     [out]   Pointer to string where device model will be stored. No memory will be allocated. Make sure you keep the device info object
  *                                     alive as long as you use the string returned from this function.
  * @return     errorStatus     Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoGetDeviceModel(const RecognizerDeviceInfo* deviceInfo, const char** deviceModel);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets the device manufacturer. String must not be NULL.
  * Example:
  * @code
  * RecognizerDeviceInfo* di;
  * recognizerDeviceInfoCreate(&di);
  * recognizerDeviceInfoSetOperatingSystem(di, RECOGNIZER_OS_ANDROID);
  * recognizerDeviceInfoSetOperatingSystemVersion(di, "4.4.2");
  * recognizerDeviceInfoSetDeviceName(di, "mako");
  * recognizerDeviceInfoSetDeviceModel(di, "Nexus 4");
  * recognizerDeviceInfoSetDeviceManufacturer(di, "LG");
  * @endcode
  *
  * @param      deviceInfo          Pointer to device info object that hold device information.
  * @param      deviceManufacturer  Manufacturer of the device.
  * @return     errorStatus         Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetDeviceManufacturer(RecognizerDeviceInfo* deviceInfo, const char* deviceManufacturer);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets the device manufacturer stored in device info structure.
  *
  * @param      deviceInfo          Pointer to device info object that hold device information.
  * @param      deviceManufacturer  [out]   Pointer to string where device manufacturer will be stored. No memory will be allocated.
  *                                         Make sure you keep the device info object alive as long as you use the string returned from this function.
  * @return     errorStatus         Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoGetDeviceManufacturer(const RecognizerDeviceInfo* deviceInfo, const char** deviceManufacturer);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets whether the device is of high quality. Set this to true if device has got HD camera.
  *
  * @param      deviceInfo  Pointer to device info object that hold device information.
  * @param      isHQ        Whether or not device is of high quality.
  * @return     errorStatus Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetHQ(RecognizerDeviceInfo* deviceInfo, int isHQ);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets whether the device is of high quality.
  *
  * @param      deviceInfo  Pointer to device info object that hold device information.
  * @param      isHQ        [out] Whether or not device is of high quality.
  * @return     errorStatus Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoIsDeviceHQ(const RecognizerDeviceInfo* deviceInfo, int* isHQ);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets the number of processors available on the device. Must be positive integer.
  *
  * @param      deviceInfo          Pointer to device info object that hold device information.
  * @param      numberOfProcessors  Number of processors.
  * @return     errorStatus         Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetNumberOfProcessors(RecognizerDeviceInfo* deviceInfo, unsigned int numberOfProcessors);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets the number of processors available on the device.
  *
  * @param      deviceInfo          Pointer to device info object that hold device information.
  * @param      numberOfProcessors  [out] Number of processors.
  * @return     errorStatus         Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoGetNumberOfProcessors(const RecognizerDeviceInfo* deviceInfo, unsigned int* numberOfProcessors);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Sets the maximum CPU frequency of the processors. Must be positive integer.
  *
  * @param      deviceInfo          Pointer to device info object that hold device information.
  * @param      maxCPUFrequency     Maximum frequency of the processors.
  * @return     errorStatus         Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoSetMaxCPUFrequency(RecognizerDeviceInfo* deviceInfo, unsigned int maxCPUFrequency);

/**
  * @memberof RecognizerDeviceInfo
  * @brief Gets the maximum CPU frequency of the processors.
  *
  * @param      deviceInfo          Pointer to device info object that hold device information.
  * @param      maxCPUFrequency     [out] Maximum frequency of the processors.
  * @return     errorStatus         Status of the operation.
  */
PP_API RecognizerErrorStatus PP_CALL recognizerDeviceInfoGetMaxCPUFrequency(const RecognizerDeviceInfo* deviceInfo, unsigned int* maxCPUFrequency);

#ifdef __cplusplus
}
#endif

#endif

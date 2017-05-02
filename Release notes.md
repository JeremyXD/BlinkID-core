# Release notes

## 3.1.0
- added support for IBAN parser
    - IBAN parser can be used to parse International Bank Account Numbers from documents
- added 32-bit slice to MacOS binaries
- reduced Linux binaries size

## 3.0.0
- added support for Templating API and performing raw OCR
	- refer to updated demo apps and [documentation](https://blinkid.github.io/blinkid-core/) for more information
- removed `RECOGNIZER_ERROR_STATUS_POINTER_IS_NULL`
	- all functions require non-null pointers, except if otherwise is noted. Failure to provide will result with a crash.
- introduced `RECOGNIZER_ERROR_STATUS_MALLOC_FAIL` which can be returned if memory allocation failed
	- previous to that, a c++ exception was thrown, which is not correct, since the API should be C90-compatible
	- this version of SDK catches all C++ exceptions that try to escape the API boundary and return either `RECOGNIZER_ERROR_STATUS_FAIL` (in case of general exception) or `RECOGNIZER_ERROR_STATUS_MALLOC_FAIL` (in case of `bad_alloc`)
- fixed linker errors when iKad scanning was enabled
- in `MRTDResult`, dates are now returned as `MBDate` objects
	- this now removes confusion about which date format was used - this differed between use cases
- `RecognizerResultList` structure is now public, so getters like `recognizerResultListGetNumOfResults` or `recognizerResultListGetResultAtIndex` are not required anymore
- `RECOGNIZER_ERROR_STATUS_INVALID_TYPE` has been renamed to `RECOGNIZER_ERROR_STATUS_INVALID_ARGUMENT`, since this better describes its intention and usage
- fixed bug where `recognizerImageSetImageOrientation` was not implemented properly, resulting with all images being treated as in `IMAGE_ORIENTATION_LANDSCAPE_RIGHT`

## 2.0.0
- new API for obtaining scanning results - refer to documentation and code samples for more information.
- Major API changes:
	- functions like `recognizerResultGetMRTDPrimaryID` have been removed. Please use `recognizerResultGetMRTDResult` to fill `MRTDResult` structure with all scanned data.
	- Similar change is also for obtaining results of PDF417 barcodes, QR codes, 1D barcodes and Malaysian MyKad documents
	- functions like `recognizerResultIsMRTD` now return value and do not use output parameters anymore
		- such functions could not fail, so returning status was an overkill
- added support for scanning Malaysian iKad documents
- removed functions `recognizerImageCreateFromFile` and `recognizerImageCreateFromEncodedImage`
	- the library does not support decoding TIFF, JPEG and PNG files anymore. You should use external libraries to perform image decoding and then present the raw image to BlinkID library for recognition.
	- we decided to remove that support due to security reasons - since we are focusing on improving recognition quality, we do not keep up with latest security patches from libtiff, libjpeg and libpng. This made our library vulnerable. For that matter we decided not to support those formats internally at all.
- added utility function `recognizerImageSetNewRawBuffer` which is more efficient than calling `recognizerImageDelete` and `recognizerImageCreateFromRawImage` respectively
	- this reduces memory reallocation overhead when recognizing bunch of video frames with single `Recognizer`
- `PPPoint` renamed to `MBPoint`
- `PPSize` renamed to `MBSize`
- `PPRectangle` renamed to `MBRectangle`
- `PPDetectionStatus` renamed to `MBDetectionStatus`
- added support for returning MyKad address parts
- added support for performing OCR of non-standard Machine Readable Zones
	- only OCR will be performed, data will not be extracted
- added support for parsing Machine Readable Zone which does not have correct check digits
	- there is no guarantee that returned data will be correct in that case
- added support for returning JPEG-encoded images of Machine Readable Zone and MRZ document
	- the encoded buffer is part of `MRTDResult` structure and available only if asked for (not by default)
	- the same images can still be retrieved in raw form as before via `showImage` callback function

## 1.4.0
- added support for scanning US Driver's Licenses, PDF417 barcodes, QR codes, Code39 and Code128 1D barcodes and Malaysian MyKad documents
- added support for scanning Green Cards' Machine Readable Zone
- added build for Linux and Mac OS X

## 1.3.0

- MRTD recognizer improvements
- Fixed bug which caused document number to have zeros and Os misrecognized, even if OCR was 100% correct in MRTD recognizer

## 1.2.0

- major library upgrade for Windows
- fixed license key library versioning on Windows

## 1.1.2

- fixed missing opencv dll files on Windows

## 1.1.1

- ShowFullDocument now returns 250 ppi color images (onShowImage)
- improved parsing of certain documents
- fixed some problems with "0" and "O" confusion

## 1.1.0

- MRZ position detection and BarrelDewarper implemented

## 1.0.0

- Initial version that supports only Machine Readable Zones (MRZ) of Machine Readable Travel Documents (MRTDs)
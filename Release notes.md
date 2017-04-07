# Release notes

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
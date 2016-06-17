# Release notes

## 1.4.0
- added support for scanning US Driver's Licenses, PDF417 barcodes, QR codes and Code39 and Code128 1D barcodes
- added build for Linux

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
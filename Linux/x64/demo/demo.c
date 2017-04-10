#include <RecognizerApi.h>

#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>

/* specifier for correct printf of size_t on 32-bit and 64-bit architectures */
#if defined(_MSC_VER)
#define JL_SIZE_T_SPECIFIER    "%Iu"
#define JL_SSIZE_T_SPECIFIER   "%Id"
#define JL_PTRDIFF_T_SPECIFIER "%Id"
#elif defined(__GNUC__)
#define JL_SIZE_T_SPECIFIER    "%zu"
#define JL_SSIZE_T_SPECIFIER   "%zd"
#define JL_PTRDIFF_T_SPECIFIER "%zd"
#endif

void onDetectionStarted() {
	printf("Detection has started!\n");
}

int onDetectedObject(const MBPoint* points, const size_t pointsSize, MBSize imageSize, MBDetectionStatus ds) {
	const char* detStatusDesc = "";

	printf("Detection on image of size %dx%d has finished\n", imageSize.width, imageSize.height);
	printf("Total " JL_SIZE_T_SPECIFIER " points have been found\n", pointsSize);
	if (pointsSize>0 && points != NULL) {
		size_t i;
		for (i = 0; i<pointsSize; ++i) {
			printf("Point " JL_SIZE_T_SPECIFIER " location: (%d, %d)\n", i, points[i].x, points[i].y);
		}
	}
	switch (ds) {
	case DETECTION_STATUS_CAMERA_AT_ANGLE:
		detStatusDesc = "camera is at angle";
		break;
	case DETECTION_STATUS_CAMERA_TOO_HIGH:
		detStatusDesc = "camera is too high";
		break;
    case DETECTION_STATUS_CAMERA_TOO_NEAR:
        detStatusDesc = "camera is too near";
        break;
	case DETECTION_STATUS_FAIL:
		detStatusDesc = "detection has failed";
		break;
	case DETECTION_STATUS_PARTIAL_OBJECT:
		detStatusDesc = "some parts of object are not visible";
		break;	
	case DETECTION_STATUS_SUCCESS:
		detStatusDesc = "detection has succeeded";
		break;
	}
	printf("Detection status: %s\n", detStatusDesc);
	return 1;
}

void onDetectionFailed() {
	printf("Detection has completely failed\n");
}

void onRecognitionStarted() {
	printf("Object recognition has started\n");
}

void onRecognitionFinished() {
	printf("Object recognition has finished\n");
}

RecognizerCallback buildRecognizerCallback() {
	RecognizerCallback cb;
	/* onDetectedObject is called when recognizer detects an object in image */
	cb.onDetectedObject = onDetectedObject;
	/* onDetectionStarted is called when recognizer begins object detection process */
	cb.onDetectionStarted = onDetectionStarted;
	/* onDetectionFailed is called when all enabled recognizers do not detect anything on the image */
	cb.onDetectionFailed = onDetectionFailed;
	/* onRecognitionStarted is called when recognition of detected object begins */
	cb.onRecognitionStarted = onRecognitionStarted;
	/* onRecognitionFinished is called when recognition of detected object finishes */
	cb.onRecognitionFinished = onRecognitionFinished;
	/* onDetectionMidway is called when some part of object is detected, but detection is not finished yet */
    cb.onDetectionMidway = NULL;
	/* onShowImage is called during recognition process and allows for additional image processing */
	cb.onShowImage = NULL;
	return cb;
}

int main(int argc, char* argv[]) {
	/* path will contain path to image being recognized */
	const char* path = argv[1];
	/* this variable will contain all recognition settings (which recognizers are enabled, etc.) */
	RecognizerSettings* settings;
	/* this variable will contain MRTD recognition specific settings */
	MRTDSettings mrtdSettings;
	/* this variable is the global recognizer that internally contains a list of different recognizers.
	Each recognizer is an object that can perform object recognitions. For example, there are PDF417 barcode
	recognizer (Microblink's implementation for PDF417 barcodes), ZXing barcode recognizer (supports everything ZXing supports),
	Microblink's 1D barcode recognizer, etc. */
	Recognizer* recognizer;
	/* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
	RecognizerErrorStatus status;
	/* recoginzer callback structure contains pointers to functions that will be called during the recognition process */
	RecognizerCallback recognizerCallback;
	/* this variable will contain list of scan results obtained from image scanning process. */
	RecognizerResultList* resultList;
	/* this variable will contain number of scan results obtained from image scanning process. */
	size_t numResults;
	/* this variable holds the image sent for image scanning process*/
	RecognizerImage* image;
	/* buffer that will contain the decompressed image */
	unsigned char* decompressedBuffer = NULL;

	if (argc < 2) {
		printf("usage %s <img_path>\n", argv[0]);
		return -1;
	}

	/* create recognizer settings object. Do not forget to delete it after usage. */
	recognizerSettingsCreate(&settings);

	/* define location where resources will be loaded from */
    recognizerSettingsSetResourcesLocation(settings, "../libRecognizerApi/res");
	
	/* add Machine Readable Travel Document recognizer settings to global recognizer settings object */
	recognizerSettingsSetMRTDSettings(settings, &mrtdSettings);

	/* insert license key and licensee */	
	recognizerSettingsSetLicenseKeyForLicensee(settings, "Add licensee here", "Add license key here");
	/* OR insert license key for licensee obtained with LicenseRequestTool (**DO NOT USE BOTH**) */
	recognizerSettingsSetLicenseKey(settings, "Add license key here");
        
	/* create global recognizer with settings */
	status = recognizerCreate(&recognizer, settings);
	/* as said earlier, all API functions return RecognizerErrorStatus. You can check the status for error, or you can simply ignore it like earlier in this example. */
	if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
		printf("Error creating recognizer: %s\n", recognizerErrorToString(status));
		return -1;
	}

	/* build recognizer callback structure */
	recognizerCallback = buildRecognizerCallback();

	/* read the image using libjpeg */
	/* code based on example from Github: https://github.com/ellzey/libjpeg/blob/master/example.c ) */
	{
		struct jpeg_decompress_struct cinfo;
		struct jpeg_error_mgr jerr;
		/* source image file */
		FILE * infile;
		JSAMPARRAY buffer;	/* Output row buffer */
		int row_stride;		/* physical row width in output buffer */
		unsigned char r,g,b;
		unsigned char* dummy;

		if ((infile = fopen(argv[1], "rb")) == NULL) {
			fprintf(stderr, "can't open %s\n", argv[1]);
			return -1;
		}

		cinfo.err = jpeg_std_error(&jerr);
		jpeg_create_decompress(&cinfo);
		jpeg_stdio_src(&cinfo, infile);
		(void) jpeg_read_header(&cinfo, TRUE);
		(void) jpeg_start_decompress(&cinfo);
		
		decompressedBuffer = ( unsigned char* )malloc( cinfo.output_width * cinfo.output_height * 3 );
		dummy = decompressedBuffer;

		row_stride = cinfo.output_width * cinfo.output_components;
		buffer = (*cinfo.mem->alloc_sarray)
			((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

		while (cinfo.output_scanline < cinfo.output_height) {
			int x; /* for loop counter (this example is C89-compatible) */

			(void) jpeg_read_scanlines(&cinfo, buffer, 1);

			/* RecognizerAPI accepts BGR and BGRA images, and libjpeg returns RGB order, so we need to revert the pixel order */

		    for (x = 0; x < cinfo.output_width; x++) {
		     	r = buffer[0][cinfo.output_components * x];
		      	if (cinfo.output_components > 2) {
		        	g = buffer[0][cinfo.output_components * x + 1];
		        	b = buffer[0][cinfo.output_components * x + 2];
		      	} else {
		        	g = r;
		        	b = r;
		      	}
		      *(dummy++) = b;
		      *(dummy++) = g;
		      *(dummy++) = r;
		    }
		}

		(void) jpeg_finish_decompress(&cinfo);

		/* now create RecognizerImage */
		recognizerImageCreateFromRawImage( &image, decompressedBuffer, cinfo.output_width, cinfo.output_height, row_stride, RAW_IMAGE_TYPE_BGR );

		jpeg_destroy_decompress(&cinfo);
		fclose(infile);
	}

	/* if you do not want to receive callbacks during simply set NULL as last parameter. If you only want to receive some callbacks,
	insert non-NULL function pointers only to those events you are interested in */
	status = recognizerRecognizeFromImage(recognizer, &resultList, image, 0, NULL);
	if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
		printf("Error recognizing file %s: %s\n", path, recognizerErrorToString(status));
		return -1;
	}

	recognizerResultListGetNumOfResults(resultList, &numResults);

	if (numResults != 1u) {
		/* number of results should be 1 as there is only one recognizer configured */
		printf("Wrong number of recognizer results:" JL_SIZE_T_SPECIFIER "\n", numResults);
		return -1;
	}

	RecognizerResult* result;
	/* obtain the first (and only) result from list */
	recognizerResultListGetResultAtIndex(resultList, 0u, &result);

    if( recognizerResultIsMRTDResult( result ) ) {
        if( recognizerResultIsResultValid( result ) ) {
            MRTDResult mrtdResult;
            recognizerResultGetMRTDResult( result, &mrtdResult );

            /* display obtained fields */
            printf( "ID is of type %s issued by %s.\nExpiration date is %s.\n", mrtdResult.documentCode, mrtdResult.issuer, mrtdResult.dateOfExpiry );
            printf( "ID number is %s.\n", mrtdResult.documentNumber );
            printf( "ID holder is %s %s.\nDate of birth is %s.\nSex is %s.\n", mrtdResult.primaryID, mrtdResult.secondaryID, mrtdResult.dateOfBirth, mrtdResult.sex );
            printf( "Nationality is %s.\n", mrtdResult.nationality );
            printf( "Optional fields are:\nOPT1: %s\nOPT2: %s\n", mrtdResult.opt1, mrtdResult.opt2 );

            printf( "Raw result lines:\n%s\n", mrtdResult.rawMRZString );
        }
        else {
            printf( "Invalid result!\n" );
        }
    }
    else {
        /* this should never happen as there is only MRTD recognizer configured */
        printf( "Invalid result type!\n" );
    }

	/* cleanup memory */	
	recognizerImageDelete(&image);
	recognizerResultListDelete(&resultList);
	recognizerSettingsDelete(&settings);
	recognizerDelete(&recognizer);

	free( decompressedBuffer );

	return 0;
}

#include <stdio.h>
#include <stdlib.h>

#include "RecognizerWrapper.h"

/**
 * Method processes RecognizerResults
 *
 *  @param result result
 */
RecognizerErrorStatus processRecognizerResult(const RecognizerResult* result) {

    // declare variables
    int isMrtd = 0;
    int isValid = 0;
    const char* primaryId;
    const char* secondaryId;
    const char* dateOfBirth;
    const char* dateOfExpiry;
    const char* documentCode;
    const char* documentNumber;
    const char* issuer;
    const char* nationality;
    const char* opt1;
    const char* opt2;
    const char* sex;
    RecognizerErrorStatus status;

    // check if result is indeed MRTD result
    status = recognizerResultIsMRTDResult(result, &isMrtd);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot check result type: %s\n", recognizerErrorToString(status));
        return status;
    }
    if (!isMrtd) {
        printf("Result is not MRTD result!\n");
        return RECOGNIZER_ERROR_STATUS_SUCCESS;
    }

    // check if result is valid
    status = recognizerResultIsResultValid(result, &isValid);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot check if result is valid: %s\n", recognizerErrorToString(status));
        return status;
    }

    if (!isValid) {
        printf("MRTD result is not valid!\n");
        return RECOGNIZER_ERROR_STATUS_SUCCESS;
    }

    // Collect all parsed data. Ignore RECOGNIZER_ERROR_STATUS here, although it's not wrong to check it.
    recognizerResultGetMRTDPrimaryID(result, &primaryId);
    recognizerResultGetMRTDSecondaryID(result, &secondaryId);
    recognizerResultGetMRTDDateOfBirth(result, &dateOfBirth);
    recognizerResultGetMRTDDateOfExpiry(result, &dateOfExpiry);
    recognizerResultGetMRTDDocumentCode(result, &documentCode);
    recognizerResultGetMRTDDocumentNumber(result, &documentNumber);
    recognizerResultGetMRTDIssuer(result, &issuer);
    recognizerResultGetMRTDNationality(result, &nationality);
    recognizerResultGetMRTDOpt1(result, &opt1);
    recognizerResultGetMRTDOpt2(result, &opt2);
    recognizerResultGetMRTDSex(result, &sex);

    // print the data
    printf("MRTD result is:\n\n");
    printf("%s %s\n", primaryId, secondaryId);
    printf("Date of birth: %s\n", dateOfBirth);
    printf("Date of expiry: %s\n", dateOfExpiry);
    printf("Document code %s\n", documentCode);
    printf("Document number %s\n", documentNumber);
    printf("Issuer %s\n", issuer);
    printf("Nationality %s\n", nationality);
    printf("Opt1 %s\n", opt1);
    printf("Opt2 %s\n\n", opt2);

    return RECOGNIZER_ERROR_STATUS_SUCCESS;
}


int main(int argc, char* argv[]) {

    /*****************  0. Declare and initialize variables ****************/

    /* path will contain path to image being recognized */
    const char* imagePath = argv[1];

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* Helper structure with all things related to recognition */
    RecognizerWrapper wrapper;

    /*****************************  1. check app usage ********************/

    if (argc < 2) {
        printf("usage %s <img_path>\n", argv[0]);
        return -1;
    }

    /*****************************  2. Init recognizer ********************/
    status = recognizerWrapperInit(&wrapper, "ocr_model.zzip");
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error initializing recognizer wrapper: %s\n\n", recognizerErrorToString(status));
        return status;
    } else {
        printf("Successfully initialized recognizer wrapper!\n\n");
    }

    /*****************************  3. Perform recognition on image ********************/
    status = recognizerWrapperProcessImageFromFile(&wrapper, imagePath, &processRecognizerResult);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error processing image at path %s: %s\n\n", imagePath, recognizerErrorToString(status));
    } else {
        printf("Successfully processed image at path %s!\n\n", imagePath);
    }

    /*****************************  4. Terminate recognizer ********************/
    status = recognizerWrapperTerm(&wrapper);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error terminating recognizer wrapper: %s\n\n", recognizerErrorToString(status));
    } else {
        printf("Successfully terminated recognizer wrapper!\n\n");
    }

    return 0;
}

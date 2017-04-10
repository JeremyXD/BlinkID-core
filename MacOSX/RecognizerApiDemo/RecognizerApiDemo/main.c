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
    RecognizerErrorStatus status;

    // check if result is indeed MRTD result
    if ( !recognizerResultIsMRTDResult( result ) ) {
        printf("Result is not MRTD result!\n");
        return RECOGNIZER_ERROR_STATUS_SUCCESS;
    }

    // check if result is valid
    if ( !recognizerResultIsResultValid( result ) ) {
        printf("MRTD result is not valid!\n");
        return RECOGNIZER_ERROR_STATUS_SUCCESS;
    }

    // Collect all parsed data.
    MRTDResult mrtdResult;
    status = recognizerResultGetMRTDResult( result, &mrtdResult );
    
    if( status != RECOGNIZER_ERROR_STATUS_SUCCESS ) {
        printf( "Failed to obtain MRTD result\n" );
        return status;
    }

    // print the data
    printf("MRTD result is:\n\n");
    printf("%s %s\n", mrtdResult.primaryID, mrtdResult.secondaryID);
    printf("Date of birth: %s\n",  mrtdResult.dateOfBirth);
    printf("Date of expiry: %s\n",  mrtdResult.dateOfExpiry);
    printf("Document code %s\n",  mrtdResult.documentCode);
    printf("Document number %s\n",  mrtdResult.documentNumber);
    printf("Issuer %s\n",  mrtdResult.issuer);
    printf("Nationality %s\n",  mrtdResult.nationality);
    printf("Opt1 %s\n",  mrtdResult.opt1);
    printf("Opt2 %s\n\n",  mrtdResult.opt2);

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
    status = recognizerWrapperInit(&wrapper, "RecognizerApi.framework/Resources");
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

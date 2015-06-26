//
//  LocalStats.c
//  RecognizerApiTest
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#include "LocalStats.h"

void localStatsCreate(LocalStats* stats, const RecognizerResult* result) {

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
        return;
    }
    if (!isMrtd) {
        printf("Result is not MRTD result!\n");
        return;
    }

    // check if result is valid
    status = recognizerResultIsResultValid(result, &isValid);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Cannot check if result is valid: %s\n", recognizerErrorToString(status));
        return;
    }

    if (!isValid) {
        printf("MRTD result is not valid!\n");
        return;
    }

    stats->valid = true;

    // Collect all parsed data. Ignore RECOGNIZER_ERROR_STATUS here, although it's not wrong to check it.
    recognizerResultGetMRTDPrimaryID(result, &primaryId);
    stats->primaryId = std::string(primaryId);

    recognizerResultGetMRTDSecondaryID(result, &secondaryId);
    stats->secondaryId = std::string(secondaryId);

    recognizerResultGetMRTDDateOfBirth(result, &dateOfBirth);
    stats->dateOfBirth = std::string(dateOfBirth);

    recognizerResultGetMRTDDateOfExpiry(result, &dateOfExpiry);
    stats->dateOfExpiry = std::string(dateOfExpiry);

    recognizerResultGetMRTDDocumentCode(result, &documentCode);
    stats->documentCode = std::string(documentCode);

    recognizerResultGetMRTDDocumentNumber(result, &documentNumber);
    stats->documentNumber = std::string(documentNumber);

    recognizerResultGetMRTDIssuer(result, &issuer);
    stats->issuer = std::string(issuer);

    recognizerResultGetMRTDNationality(result, &nationality);
    stats->nationality = std::string(nationality);

    recognizerResultGetMRTDOpt1(result, &opt1);
    stats->opt1 = std::string(opt1);

    recognizerResultGetMRTDOpt2(result, &opt2);
    stats->opt2 = std::string(opt2);

    recognizerResultGetMRTDSex(result, &sex);
    stats->sex = std::string(sex);
}

void localStatsSave(LocalStats* stats, const std::string filename) {

    FILE *f;
     
    if (fopen_s(&f, filename.c_str(), "wt+") == -1) {
        printf("Error opening file!\n");
    }

    fprintf(f, "Image filename: %s\n", stats->filename.c_str());
    fprintf(f, "Processing duration: %.5lf milliseconds\n", stats->duration);
    fprintf(f, "Data is valid: %s\n\n", stats->valid ? "YES" : "NO");

    if (stats->primaryId.length() > 0) {
        fprintf(f, "primaryId: %s\n", stats->primaryId.c_str());
    }

    if (stats->secondaryId.length() > 0) {
        fprintf(f, "secondaryId: %s\n", stats->secondaryId.c_str());
    }

    if (stats->dateOfBirth.length() > 0) {
        fprintf(f, "dateOfBirth: %s\n", stats->dateOfBirth.c_str());
    }

    if (stats->dateOfExpiry.length() > 0) {
        fprintf(f, "dateOfExpiry: %s\n", stats->dateOfExpiry.c_str());
    }

    if (stats->documentCode.length() > 0) {
        fprintf(f, "documentCode: %s\n", stats->documentCode.c_str());
    }

    if (stats->documentNumber.length() > 0) {
        fprintf(f, "documentNumber: %s\n", stats->documentNumber.c_str());
    }

    if (stats->issuer.length() > 0) {
        fprintf(f, "issuer: %s\n", stats->issuer.c_str());
    }

    if (stats->nationality.length() > 0) {
        fprintf(f, "nationality: %s\n", stats->nationality.c_str());
    }

    if (stats->opt1.length() > 0) {
        fprintf(f, "opt1: %s\n", stats->opt1.c_str());
    }

    if (stats->opt2.length() > 0) {
        fprintf(f, "opt2: %s\n", stats->opt2.c_str());
    }

    if (stats->sex.length() > 0) {
        fprintf(f, "sex: %s\n", stats->sex.c_str());
    }
    
    fclose(f);
}

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
    
    MRTDResult mrtdResult;
    RecognizerErrorStatus status;

    // check if result is indeed MRTD result
    isMrtd = recognizerResultIsMRTDResult(result);
    
    if (!isMrtd) {
        printf("Result is not MRTD result!\n");
        return;
    }

    // check if result is valid
    isValid = recognizerResultIsResultValid(result);
  
    if (!isValid) {
        printf("MRTD result is not valid!\n");
        return;
    }

    stats->valid = true;

    recognizerResultGetMRTDResult( result, &mrtdResult );

    stats->primaryId = std::string(mrtdResult.primaryID);
    stats->secondaryId = std::string( mrtdResult.secondaryID);
    stats->dateOfBirth = std::string( mrtdResult.dateOfBirth);
    stats->dateOfExpiry = std::string( mrtdResult.dateOfExpiry);
    stats->documentCode = std::string( mrtdResult.documentCode);
    stats->documentNumber = std::string( mrtdResult.documentNumber);
    stats->issuer = std::string( mrtdResult.issuer);
    stats->nationality = std::string( mrtdResult.nationality);
    stats->opt1 = std::string( mrtdResult.opt1);
    stats->opt2 = std::string( mrtdResult.opt2);
    stats->sex = std::string( mrtdResult.sex);
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

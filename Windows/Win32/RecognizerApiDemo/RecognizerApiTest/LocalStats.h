//
//  LocalStats.h
//  RecognizerApiTest
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#ifndef __RecognizerApiTest__LocalStats__
#define __RecognizerApiTest__LocalStats__

#include <stdio.h>

#include <string>

#include "RecognizerApi.h"

typedef struct LocalStats {
    std::string filename;
    double duration;

    bool valid;

    std::string primaryId;
    std::string secondaryId;
    std::string dateOfBirth;
    std::string dateOfExpiry;
    std::string documentCode;
    std::string documentNumber;
    std::string issuer;
    std::string nationality;
    std::string opt1;
    std::string opt2;
    std::string sex;
} LocalStats;

void localStatsCreate(LocalStats* stats, const RecognizerResult* result);

void localStatsSave(LocalStats* stats, const std::string filename);

#endif /* defined(__RecognizerApiTest__LocalStats__) */

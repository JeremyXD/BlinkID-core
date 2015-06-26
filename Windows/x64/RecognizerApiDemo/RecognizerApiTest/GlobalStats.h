//
//  GlobalStats.h
//  RecognizerApiTest
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#ifndef __RecognizerApiTest__GlobalStats__
#define __RecognizerApiTest__GlobalStats__

#include <stdio.h>

#include <string>

typedef struct GlobalStats {
    int numScans;
    double sumDurations;
    int numValidScans;
} GlobalStats;

void globalStatsSave(GlobalStats* stats, const std::string filename);

#endif

//
//  GlobalStats.c
//  RecognizerApiTest
//
//  Created by Jura on 26/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#include "GlobalStats.h"

void globalStatsSave(GlobalStats* stats, const std::string filename)  {

    FILE *f = fopen(filename.c_str(), "w+");

    if (f == NULL) {
        printf("Error opening file!\n");
    }

    fprintf(f, "Total scans: %d\n", stats->numScans);
    fprintf(f, "Total successful scans: %d\n", stats->numValidScans);
    fprintf(f, "Total processing duration: %.5lf\n", stats->sumDurations);
    fprintf(f, "Average processing duration per image: %.5lf\n", stats->sumDurations / stats->numScans);
    
    fclose(f);
}
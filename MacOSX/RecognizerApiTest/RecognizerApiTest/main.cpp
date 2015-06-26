//
//  main.cpp
//  RecognizerApiTest
//
//  Created by Jura on 24/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#include <iostream>
#include <dirent.h>
#include <sys/stat.h>

#include "RecognizerWrapper.h"
#include "GlobalStats.h"
#include "LocalStats.h"

LocalStats localStats;
GlobalStats globalStats;

/**
 * Method processes RecognizerResults
 *
 *  @param result result
 */
RecognizerErrorStatus processRecognizerResult(const RecognizerResult* result) {
    localStatsCreate(&localStats, result);
    return RECOGNIZER_ERROR_STATUS_SUCCESS;
}

void processFolder(const RecognizerWrapper* wrapper, std::string folderName) {
    RecognizerErrorStatus status;
    DIR* d;
    struct dirent* dirent;
    struct stat fstat;

    d = opendir(folderName.c_str());
    if (d) {
        while ((dirent = readdir(d)) != NULL) {
            std::string filename(dirent->d_name);
            if (filename == "." || filename == "..") continue;

            filename = folderName + "/" + filename;
            if (stat(filename.c_str(), &fstat) < 0) continue;

            if (!S_ISDIR(fstat.st_mode)) {
                localStats.filename = filename;

                double duration = 1.0;
                status = recognizerWrapperProcessImageFromFile(wrapper, filename.c_str(), &processRecognizerResult);
                if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
                    printf("Error processing image at path %s: %s\n\n", filename.c_str(), recognizerErrorToString(status));
                } else {
                    localStats.duration = duration;
                    globalStats.sumDurations += duration;

                    localStatsSave(&localStats, filename + ".txt");

                    globalStats.numScans++;
                    globalStats.numValidScans += (localStats.valid ? 1 : 0);
                }
            } else {
                processFolder(wrapper, filename);
            }
        }

        closedir(d);
    }
}

int main(int argc, char* argv[]) {

    /*****************  1. Declare and initialize variables ****************/

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* Helper structure with all things related to recognition */
    RecognizerWrapper wrapper;

    /*****************************  2. Init recognizer ********************/
    status = recognizerWrapperInit(&wrapper, "ocr_model.zzip");
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error initializing recognizer wrapper: %s\n\n", recognizerErrorToString(status));
        return status;
    } else {
        printf("Successfully initialized recognizer wrapper!\n\n");
    }

    /*****************************  3. Perform recognition on image ********************/
    processFolder(&wrapper, ".");

    /*****************************  4. Terminate recognizer ********************/
    status = recognizerWrapperTerm(&wrapper);
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error terminating recognizer wrapper: %s\n\n", recognizerErrorToString(status));
    } else {
        printf("Successfully terminated recognizer wrapper!\n\n");
    }

    globalStatsSave(&globalStats, "report.txt");
    
    return 0;
}

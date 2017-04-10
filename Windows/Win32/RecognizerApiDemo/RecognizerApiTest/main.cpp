//
//  main.cpp
//  RecognizerApiTest
//
//  Created by Jura on 24/06/15.
//  Copyright (c) 2015 MicroBlink. All rights reserved.
//

#include "RecognizerWrapper.h"
#include "GlobalStats.h"
#include "LocalStats.h"

#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>

static long long frequency = 0;

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
    HANDLE hFind = INVALID_HANDLE_VALUE;
    WIN32_FIND_DATA ffd;
    std::stringstream ss;
    ss << folderName << "\\*";
    
    hFind = FindFirstFile(ss.str().c_str(), &ffd);
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            std::string filename(ffd.cFileName);
            if (filename == "." || filename == "..") continue;

            filename = folderName + "\\" + filename;

            long long end;
            long long start;
            if (frequency == 0) {
                QueryPerformanceFrequency((PLARGE_INTEGER)&frequency);
            }
            QueryPerformanceCounter((PLARGE_INTEGER)&start);

            if (!(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                localStats.filename = filename;

                double duration = 1.0;
                status = recognizerWrapperProcessImageFromFile(wrapper, filename.c_str(), &processRecognizerResult);
                if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
                    printf("Error processing image at path %s: %s\n\n", filename.c_str(), recognizerErrorToString(status));
                }
                else {
                    QueryPerformanceCounter((PLARGE_INTEGER)&end);                    

                    duration = ((end - start) * 1000) / (double)frequency;                    

                    localStats.duration = duration;
                    globalStats.sumDurations += duration;

                    localStatsSave(&localStats, filename + ".txt");

                    globalStats.numScans++;
                    globalStats.numValidScans += (localStats.valid ? 1 : 0);
                }                   
            } else {
                processFolder(wrapper, filename);
            }
        } while (FindNextFile(hFind, &ffd) != 0);
    }
    FindClose(hFind);
}

int main(int argc, char* argv[]) {

    /*****************  1. Declare and initialize variables ****************/

    /* all API functions return RecognizerErrorStatus indicating the success or failure of operations */
    RecognizerErrorStatus status;

    /* Helper structure with all things related to recognition */
    RecognizerWrapper wrapper;

    /*****************************  2. Init recognizer ********************/
    status = recognizerWrapperInit(&wrapper, "res");
    if (status != RECOGNIZER_ERROR_STATUS_SUCCESS) {
        printf("Error initializing recognizer wrapper: %s\n\n", recognizerErrorToString(status));
        return status;
    } else {
        printf("Successfully initialized recognizer wrapper!\n\n");
    }

    /*****************************  3. Perform recognition on image ********************/
    processFolder(&wrapper, "Images");

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

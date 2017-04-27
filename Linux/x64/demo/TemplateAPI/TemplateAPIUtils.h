#ifndef TEMPLATE_API_UTILS_INCLUDED
#define TEMPLATE_API_UTILS_INCLUDED

#include <RecognizerApi.h>

#include <stdlib.h>

/**
 * This function will return pointer to statically allocated array of char+font
 * combinations used for 
 */
OcrWhiteListEntry* croatianLettersWhitelist( size_t* whiteListSize, int alsoAddDigits );

#endif
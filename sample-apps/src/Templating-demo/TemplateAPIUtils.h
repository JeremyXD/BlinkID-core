#ifndef TEMPLATE_API_UTILS_INCLUDED
#define TEMPLATE_API_UTILS_INCLUDED

#include <RecognizerApi.h>

#include <stdlib.h>

#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

/**
 * This function will return pointer to statically allocated array of char+font
 * combinations used for 
 */
OcrWhitelistEntry * croatianLettersWhitelist( size_t * whiteListSize, int alsoAddDigits );

#endif

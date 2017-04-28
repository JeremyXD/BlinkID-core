#ifndef TEMPLATE_API_CRO_FRONT_DEMO_INCLUDED
#define TEMPLATE_API_CRO_FRONT_DEMO_INCLUDED

#include <RecognizerApi.h>

/**
 * This function will configure given BlinkInputSettings to perform
 * recognition of front side of both old and new version of Croatian
 * National IDs.
 * The implementation of the function follows the Templating API
 * whitepaper available here: https://github.com/BlinkID/blinkid-android/blob/master/templatingAPI/templatingAPI.md
 */
void setupBlinkInputSettings( BlinkInputSettings* blinkInputSett );

/**
 * This function will print out all recognized fields from BlinkInput
 * recognition result.
 */
void printBlinkInputResult( BlinkInputResult* blinkInputResult );

#endif
#ifndef TEMPLATE_API_CRO_BACK_DEMO_INCLUDED
#define TEMPLATE_API_CRO_BACK_DEMO_INCLUDED

#include <RecognizerApi.h>

/**
 * This function will configure given MRTDSettings to perform
 * recognition of the back side of both old and new version of Croatian
 * National IDs.
 * The implementation of the function follows the Templating API
 * whitepaper available here: https://github.com/BlinkID/blinkid-android/blob/master/templatingAPI/templatingAPI.md
 */
void setupMRTDSettings( MRTDSettings* mrtdSett );

/**
 * This function will print out all recognized fields from MRTD
 * recognition result.
 */
void printMRTDResult( MRTDResult* mrtdResult );

#endif
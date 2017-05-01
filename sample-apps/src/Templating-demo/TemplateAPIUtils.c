#include "TemplateAPIUtils.h"

OcrWhitelistEntry * croatianLettersWhitelist( size_t * whiteListSize, int alsoAddDigits )
{
	static OcrWhitelistEntry whitelist[ 'Z' - 'A' + 16 ];
	if ( whitelist[ 0 ].charValue == 0 ) {
		/* initialize static whitelist */
        {
            wchar_t c;
            for ( c = L'A'; c <= 'Z'; ++c )
            {
                whitelist[ c - 'A' ].charValue = ( int ) c;
                whitelist[ c - 'A' ].charFont  = OCR_FONT_ANY;
            }
        }

		whitelist[ 'Z' - 'A' + 1 ].charValue = ( int ) L'Š';
		whitelist[ 'Z' - 'A' + 1 ].charFont  = OCR_FONT_ANY;

		whitelist[ 'Z' - 'A' + 2 ].charValue = ( int ) L'Đ';
		whitelist[ 'Z' - 'A' + 2 ].charFont  = OCR_FONT_ANY;

		whitelist[ 'Z' - 'A' + 3 ].charValue = ( int ) L'Ž';
		whitelist[ 'Z' - 'A' + 3 ].charFont  = OCR_FONT_ANY;

		whitelist[ 'Z' - 'A' + 4 ].charValue = ( int ) L'Č';
		whitelist[ 'Z' - 'A' + 4 ].charFont  = OCR_FONT_ANY;

		whitelist[ 'Z' - 'A' + 5 ].charValue = ( int ) L'Ć';
		whitelist[ 'Z' - 'A' + 5 ].charFont  = OCR_FONT_ANY;

        {
            char c;
            for ( c = '0'; c <= '9'; ++ c)
            {
                whitelist[ 'Z' - 'A' + 6 + ( c - '0' ) ].charValue = ( int ) c;
                whitelist[ 'Z' - 'A' + 6 + ( c - '0' ) ].charFont  = OCR_FONT_ANY;
            }
        }
	}
	*whiteListSize = 'Z' - 'A' + ( alsoAddDigits ? 16 : 6 );
	return whitelist;
}

/*
  CharType.cpp
  Copyright (C) 2007 David M. Anderson

  Character classification and conversion functions.
*/


#include "CharType.hpp"
#include "CodePointData.hpp"
#include "StdInt.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <cctype>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool 
IsAlpha( char ch )
{
    return IsAlpha( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsUpper( char ch )
{
    return IsUpper( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsLower( char ch )
{
    return IsLower( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsDigit( char ch )
{
    return IsDigit( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsXDigit( char ch )
{
    return IsXDigit( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsAlNum( char ch )
{
    return IsAlNum( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsBlank( char ch )
{
    return IsBlank( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsSpace( char ch )
{
    return IsSpace( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsPunct( char ch )
{
    return IsPunct( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsGraph( char ch )
{
    return IsGraph( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsPrint( char ch )
{
    return IsPrint( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

bool 
IsCntrl( char ch )
{
    return IsCntrl( (wchar_t)(uint8_t)ch );
}

//=============================================================================

char 
ToUpper( char ch )
{
    return (char)ToUpper( (wchar_t )(uint8_t)ch );
}

//-----------------------------------------------------------------------------

char 
ToLower( char ch )
{
    return (char)ToLower( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

char 
ToASCII( char ch )
{
    return (char)ToASCII( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

int 
DigitValue( char ch )
{
    return DigitValue( (wchar_t)(uint8_t)ch );
}

//-----------------------------------------------------------------------------

int 
HexDigitValue( char ch )
{
    return HexDigitValue( (wchar_t)(uint8_t)ch );
}

//=============================================================================
   
bool 
IsAlpha( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Alpha) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsUpper( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Upper) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsLower( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Lower) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsDigit( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_DecDigit) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsXDigit( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_HexDigit) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsAlNum( wchar_t ch )
{
    return (IsAlpha( ch ) || IsDigit( ch ));
}

//-----------------------------------------------------------------------------

bool 
IsBlank( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Blank) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsSpace( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Space) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsPunct( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Punctuation) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsGraph( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Graph) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsPrint( wchar_t ch )
{
    return ( IsGraph( ch ) || (IsBlank( ch ) && (ch != '\t')) );
}

//-----------------------------------------------------------------------------

bool 
IsCntrl( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return false;
    return ((codePointCharTypes[ ch ] & CTB_Ctrl) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsASCII( wchar_t ch )
{
    return (ch < 128 );
}

//=============================================================================

wchar_t 
ToUpper( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return ch;
    return codePointToUpper[ ch ];
}

//-----------------------------------------------------------------------------

wchar_t 
ToLower( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return ch;
    return codePointToLower[ ch ];
}

//-----------------------------------------------------------------------------

char 
ToASCII( wchar_t ch )
{
    Assert( ch <= MaximumCodePoint );
    if ( ch > MaximumCodePoint )
        return '_';
    return codePointToASCII[ ch ];
}

//-----------------------------------------------------------------------------

int 
DigitValue( wchar_t ch )
{
    if ( (ch >= '0') && (ch <= '9') )
        return (int)(ch - '0');
    if ( (ch >= 0xFF10) && (ch <= 0xFF19) ) //full-width digits
        return (int)(ch - 0xFF10);
    Assert( 0 && "Not a digit" );
    return 0;
}

//-----------------------------------------------------------------------------

int 
HexDigitValue( wchar_t ch )
{
    if ( (ch >= '0') && (ch <= '9') )
        return (int)(ch - '0');
    if ( (ch >= 'A') && (ch <= 'F') )
        return 10 + (int)(ch - 'A');
    if ( (ch >= 'a') && (ch <= 'f') )
        return 10 + (int)(ch - 'a');
    if ( (ch >= 0xFF10) && (ch <= 0xFF19) ) //full-width digits
        return (int)(ch - 0xFF10);
    if ( (ch >= 0xFF21) && (ch <= 0xFF26) ) //full-width A-F
        return 10 + (int)(ch - 0xFF21);
    if ( (ch >= 0xFF41) && (ch <= 0xFF46) ) //full-width a-f
        return 10 + (int)(ch - 0xFF41);
    Assert( 0 && "Not a hex digit" );
    return 0;
}

//=============================================================================

#ifdef DEBUG

bool 
TestCharType( )
{
    bool ok = true;
    cout << "Testing CharType" << endl;

    cout << "Checking range 0-127 for consistency with std functions." << endl;
    for ( char c = 0; (c >= 0) && (c < 128); ++c )
    {
        TESTCHECKD( IsAlpha( c ), (bool)isalpha( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsUpper( c ), (bool)isupper( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsLower( c ), (bool)islower( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsDigit( c ), (bool)isdigit( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsXDigit( c ), (bool)isxdigit( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsAlNum( c ), (bool)isalnum( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsBlank( c ), (bool)isblank( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsSpace( c ), (bool)isspace( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsPunct( c ), (bool)ispunct( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsGraph( c ), (bool)isgraph( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsPrint( c ), (bool)isprint( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsCntrl( c ), (bool)iscntrl( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( ToUpper( c ), (char)toupper( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( ToLower( c ), (char)tolower( c ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( ToASCII( c ), c, &ok, TestCheck::PrintFailure );
    }

    for ( wchar_t wc = 0; wc < 128; ++wc )
    {
        TESTCHECKD( IsAlpha( wc ), (bool)isalpha( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsUpper( wc ), (bool)isupper( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsLower( wc ), (bool)islower( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsDigit( wc ), (bool)isdigit( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsXDigit( wc ), (bool)isxdigit( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsAlNum( wc ), (bool)isalnum( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsBlank( wc ), (bool)isblank( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsSpace( wc ), (bool)isspace( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsPunct( wc ), (bool)ispunct( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsGraph( wc ), (bool)isgraph( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsPrint( wc ), (bool)isprint( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( IsCntrl( wc ), (bool)iscntrl( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( ToUpper( wc ), (char)toupper( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( ToLower( wc ), (char)tolower( wc ), &ok,
                    TestCheck::PrintFailure );
        TESTCHECKD( ToASCII( wc ), (char)wc, &ok, TestCheck::PrintFailure );
    }

    if ( ok )
        cout << "CharType PASSED." << endl << endl;
    else
        cout << "CharType FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

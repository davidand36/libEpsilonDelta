/*
  CharType.cpp
  Copyright (C) 2007 David M. Anderson

  Character classification and conversion functions.
*/


#include "CharType.hpp"
#include "StdInt.hpp"
#include "Assert.hpp"
#include <cctype>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool 
IsUpper( char ch )
{
    return IsUpper(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsLower( char ch )
{
    return IsLower(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsAlpha( char ch )
{
    return IsAlpha(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsDigit( char ch )
{
    return IsDigit(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsXDigit( char ch )
{
    return IsXDigit(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsSpace( char ch )
{
    return IsSpace(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsPrint( char ch )
{
    return IsPrint(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsGraph( char ch )
{
    return IsGraph(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsBlank( char ch )
{
    return IsBlank(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsCntrl( char ch )
{
    return IsCntrl(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsPunct( char ch )
{
    return IsPunct(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//-----------------------------------------------------------------------------

bool 
IsAlNum( char ch )
{
    return IsAlNum(
        static_cast< wchar_t >( static_cast< unsigned char >( ch ) ) );
}

//=============================================================================

char 
ToUpper( char ch )
{
    return static_cast< char >( ToUpper( static_cast< wchar_t >(
                                      static_cast< unsigned char >( ch ) ) ) );
}

//-----------------------------------------------------------------------------

char 
ToLower( char ch )
{
    return static_cast< char >( ToLower( static_cast< wchar_t >(
                                      static_cast< unsigned char >( ch ) ) ) );
}

//-----------------------------------------------------------------------------

char 
ToASCII( char ch )
{
    return static_cast< char >( ToASCII( static_cast< wchar_t >(
                                      static_cast< unsigned char >( ch ) ) ) );
}


//=============================================================================


const int WCharTypeTableLimit = 0x0180;  //Latin-1, Latin Extended A


//=============================================================================

namespace
{                                                                   //namespace

enum ECharTypeBit
{
    UpperCTBit      = (1 << 0),
    LowerCTBit      = (1 << 1),
    OtherAlphaCTBit = (1 << 2), 
    PunctCTBit      = (1 << 3),
    ControlCTBit    = (1 << 4)
};
const uint8_t WCharTypeTable[ WCharTypeTableLimit - 128 ]
= { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, //0080-0087
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, //0088-008F
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, //0090-0097
    0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, //0098-009F
    0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, //00A0-00A7
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, //00A8-00AF
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, //00B0-00B7
    0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, //00B8-00BF
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //00C0-00C7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, //00C8-00CF
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x08, //00D0-00D7
    0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, //00D8-00DF
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, //00E0-00E7
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, //00E8-00EF
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x08, //00F0-00F7
    0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, //00F8-00FF
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0100-0107
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0108-010F
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0110-0117
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0118-011F
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0120-0127
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0128-012F
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0130-0137
    0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, //0138-013F
    0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, //0140-0147
    0x02, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0148-014F
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0150-0157
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0158-015F
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0160-0167
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0168-016F
    0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, //0170-0177
    0x01, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x02  //0178-017F
};
const wchar_t ToUpperTable[ WCharTypeTableLimit - 128 ]
= {
         0,      0,      0,      0,      0,      0,      0,      0, //0080-0087
         0,      0,      0,      0,      0,      0,      0,      0, //0088-008F
         0,      0,      0,      0,      0,      0,      0,      0, //0090-0097
         0,      0,      0,      0,      0,      0,      0,      0, //0098-009F
         0,      0,      0,      0,      0,      0,      0,      0, //00A0-00A7
         0,      0,      0,      0,      0,      0,      0,      0, //00A8-00AF
         0,      0,      0,      0,      0,      0,      0,      0, //00B0-00B7
         0,      0,      0,      0,      0,      0,      0,      0, //00B8-00BF
         0,      0,      0,      0,      0,      0,      0,      0, //00C0-00C7
         0,      0,      0,      0,      0,      0,      0,      0, //00C8-00CF
         0,      0,      0,      0,      0,      0,      0,      0, //00D0-00D7
         0,      0,      0,      0,      0,      0,      0,      0, //00D8-00DF
    0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, //00E0-00E7
    0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, //00E8-00EF
    0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6,      0, //00F0-00F7
    0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x0178, //00F8-00FF
         0, 0x0100,      0, 0x0102,      0, 0x0104,      0, 0x0106, //0100-0107
         0, 0x0108,      0, 0x010A,      0, 0x010C,      0, 0x010E, //0108-010F
         0, 0x0110,      0, 0x0112,      0, 0x0114,      0, 0x0116, //0110-0117
         0, 0x0118,      0, 0x011A,      0, 0x011C,      0, 0x011E, //0118-011F
         0, 0x0120,      0, 0x0122,      0, 0x0124,      0, 0x0126, //0120-0127
         0, 0x0128,      0, 0x012A,      0, 0x012C,      0, 0x012E, //0128-012F
         0, 0x0130,      0, 0x0132,      0, 0x0134,      0, 0x0136, //0130-0137
         0,      0, 0x0139,      0, 0x013B,      0, 0x013D,      0, //0138-013F
    0x013F,      0, 0x0141,      0, 0x0143,      0, 0x0145,      0, //0140-0147
    0x0147,      0,      0, 0x014A,      0, 0x014C,      0, 0x014E, //0148-014F
         0, 0x0150,      0, 0x0152,      0, 0x0154,      0, 0x0156, //0150-0157
         0, 0x0158,      0, 0x015A,      0, 0x015C,      0, 0x015E, //0158-105F
         0, 0x0160,      0, 0x0162,      0, 0x0164,      0, 0x0166, //0160-0167
         0, 0x0168,      0, 0x016A,      0, 0x016C,      0, 0x016E, //0168-016F
         0, 0x0170,      0, 0x0172,      0, 0x0174,      0, 0x0176, //0170-0177
         0,      0, 0x0179,      0, 0x017B,      0, 0x177D, 0x0053  //0178-017F
};
const wchar_t ToLowerTable[ WCharTypeTableLimit - 128 ]
= {
         0,      0,      0,      0,      0,      0,      0,      0, //0080-0087
         0,      0,      0,      0,      0,      0,      0,      0, //0088-008F
         0,      0,      0,      0,      0,      0,      0,      0, //0090-0097
         0,      0,      0,      0,      0,      0,      0,      0, //0098-009F
         0,      0,      0,      0,      0,      0,      0,      0, //00A0-00A7
         0,      0,      0,      0,      0,      0,      0,      0, //00A8-00AF
         0,      0,      0,      0,      0,      0,      0,      0, //00B0-00B7
         0,      0,      0,      0,      0,      0,      0,      0, //00B8-00BF
    0x00E0, 0x00E1, 0x00E2, 0x00E3, 0x00E4, 0x00E5, 0x00E6, 0x00E7, //00C0-00C7
    0x00E8, 0x00E9, 0x00EA, 0x00EB, 0x00EC, 0x00ED, 0x00EE, 0x00EF, //00C8-00CF
    0x00F0, 0x00F1, 0x00F2, 0x00F3, 0x00F4, 0x00F5, 0x00F6,      0, //00D0-00D7
    0x00F8, 0x00F9, 0x00FA, 0x00FB, 0x00FC, 0x00FD, 0x00FE,      0, //00D8-00DF
         0,      0,      0,      0,      0,      0,      0,      0, //00E0-00E7
         0,      0,      0,      0,      0,      0,      0,      0, //00E8-00EF
         0,      0,      0,      0,      0,      0,      0,      0, //00F0-00F7
         0,      0,      0,      0,      0,      0,      0,      0, //00F8-00FF
    0x0101,      0, 0x0103,      0, 0x0105,      0, 0x0107,      0, //0100-0107
    0x0109,      0, 0x010B,      0, 0x010D,      0, 0x010F,      0, //0108-010F
    0x0111,      0, 0x0113,      0, 0x0115,      0, 0x0117,      0, //0110-0117
    0x0119,      0, 0x011B,      0, 0x011D,      0, 0x011F,      0, //0118-011F
    0x0121,      0, 0x0123,      0, 0x0125,      0, 0x0127,      0, //0120-0127
    0x0129,      0, 0x012B,      0, 0x012D,      0, 0x012F,      0, //0128-012F
    0x0131,      0, 0x0133,      0, 0x0135,      0, 0x0137,      0, //0130-0137
         0, 0x013A,      0, 0x013C,      0, 0x013E,      0, 0x0140, //0138-013F
         0, 0x0142,      0, 0x0144,      0, 0x0146,      0, 0x0148, //0140-0147
         0,      0, 0x014B,      0, 0x014D,      0, 0x014F,      0, //0148-014F
    0x0151,      0, 0x0153,      0, 0x0155,      0, 0x0157,      0, //0150-0157
    0x0159,      0, 0x015B,      0, 0x015D,      0, 0x015F,      0, //0158-015F
    0x0161,      0, 0x0163,      0, 0x0165,      0, 0x0167,      0, //0160-0167
    0x0169,      0, 0x016B,      0, 0x016D,      0, 0x016F,      0, //0168-016F
    0x0171,      0, 0x0173,      0, 0x0175,      0, 0x0177,      0, //0170-0177
    0x00FF, 0x017A,      0, 0x017C,      0, 0x017E,      0,      0  //0178-017F
};
const char ToASCIITable[ WCharTypeTableLimit - 128 ]
= {
    '\b', '\b', '\b', '\b', '\b', '\n', '\b', '\b', //0080-0087
    '\b', '\t', '\b', '\b', '\b', '\b', '\b', '\b', //0088-008F
    '\b', '\b', '\b', '\b', '\b', '\b', '\b', '\b', //0090-0097
    '\b', '\b', '\b', '\b', '\b', '\b', '\b', '\b', //0098-009F
     ' ',  '!',  '$',  '$',  '$',  '$',  '|',  '#', //00A0-00A7
     '"',  'C',  'a',  '"',  '~',  '-',  'R',  '-', //00A8-00AF
     'o',  '+',  '2',  '3', '\'',  'm',  '#',  '.', //00B0-00B7
     ',',  '1',  'o',  '"',  '#',  '#',  '#',  '?', //00B8-00BF
     'A',  'A',  'A',  'A',  'A',  'A',  'A',  'C', //00C0-00C7
     'E',  'E',  'E',  'E',  'I',  'I',  'I',  'I', //00C8-00CF
     'D',  'N',  'O',  'O',  'O',  'O',  'O',  'x', //00D0-00D7
     'O',  'U',  'U',  'U',  'U',  'Y',  'P',  's', //00D8-00DF
     'a',  'a',  'a',  'a',  'a',  'a',  'a',  'c', //00E0-00E7
     'e',  'e',  'e',  'e',  'i',  'i',  'i',  'i', //00E8-00EF
     'd',  'n',  'o',  'o',  'o',  'o',  'o',  '/', //00F0-00F7
     'o',  'u',  'u',  'u',  'u',  'y',  'p',  'y', //00F8-00FF
     'A',  'a',  'A',  'a',  'A',  'a',  'C',  'c', //0100-0107
     'C',  'c',  'C',  'c',  'C',  'c',  'D',  'd', //0108-010F
     'D',  'd',  'E',  'e',  'E',  'e',  'E',  'e', //0110-0117
     'E',  'e',  'E',  'e',  'G',  'g',  'G',  'g', //0118-011F
     'G',  'g',  'G',  'g',  'H',  'h',  'H',  'h', //0120-0127
     'I',  'i',  'I',  'i',  'I',  'i',  'I',  'i', //0128-012F
     'I',  'i',  'I',  'i',  'J',  'j',  'K',  'k', //0130-1037
     'k',  'L',  'l',  'L',  'l',  'L',  'l',  'L', //0138-013F
     'l',  'L',  'l',  'N',  'n',  'N',  'n',  'N', //0140-0147
     'n',  'n',  'N',  'n',  'O',  'o',  'O',  'o', //0148-014F
     'O',  'o',  'O',  'o',  'R',  'r',  'R',  'r', //0150-0157
     'R',  'r',  'S',  's',  'S',  's',  'S',  's', //0158-015F
     'S',  's',  'T',  't',  'T',  't',  'T',  't', //0160-0167
     'U',  'u',  'U',  'u',  'U',  'u',  'U',  'u', //0168-016F
     'U',  'u',  'U',  'u',  'W',  'w',  'Y',  'y', //0170-0177
     'Y',  'Z',  'z',  'Z',  'z',  'Z',  'z',  's'  //0178-017F
};

}                                                                   //namespace

//=============================================================================

bool 
IsUpper( wchar_t ch )
{
    if ( ch < 128 )
        return ( isupper( ch ) != 0 );
    Assert( ch < WCharTypeTableLimit );
    if ( ch >= WCharTypeTableLimit )
        return false;
    return ((WCharTypeTable[ ch - 128 ] & UpperCTBit) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsLower( wchar_t ch )
{
    if ( ch < 128 )
        return ( islower( ch ) != 0 );
    Assert( ch < WCharTypeTableLimit );
    if ( ch >= WCharTypeTableLimit )
        return false;
    return ((WCharTypeTable[ ch - 128 ] & LowerCTBit) != 0);
}

//-----------------------------------------------------------------------------
   
bool 
IsAlpha( wchar_t ch )
{
    if ( ch < 128 )
        return ( isalpha( ch ) != 0 );
    Assert( ch < WCharTypeTableLimit );
    if ( ch >= WCharTypeTableLimit )
        return false;
    return ((WCharTypeTable[ ch - 128 ]
             & (UpperCTBit | LowerCTBit | OtherAlphaCTBit)) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsDigit( wchar_t ch )
{
    if ( ch < 128 )
        return ( isdigit( ch ) != 0 );
    return false;
}

//-----------------------------------------------------------------------------

bool 
IsXDigit( wchar_t ch )
{
    if ( ch < 128 )
        return ( isxdigit( ch ) != 0 );
    return false;
}

//-----------------------------------------------------------------------------

bool 
IsSpace( wchar_t ch )
{
    if ( ch < 128 )
        return ( isspace( ch ) != 0 );
    return false;
}

//-----------------------------------------------------------------------------

bool 
IsPrint( wchar_t ch )
{
    return ( IsGraph( ch ) || (ch == ' ') );
}

//-----------------------------------------------------------------------------

bool 
IsGraph( wchar_t ch )
{
    return ( IsAlNum( ch ) || IsPunct( ch ) );
}

//-----------------------------------------------------------------------------

bool 
IsBlank( wchar_t ch )
{
    return ( (ch == ' ') || (ch == '\t') );
}

//-----------------------------------------------------------------------------

bool 
IsCntrl( wchar_t ch )
{
    return ! IsPrint( ch );
}

//-----------------------------------------------------------------------------

bool 
IsPunct( wchar_t ch )
{
    if ( ch < 128 )
        return ( ispunct( ch ) != 0 );
    Assert( ch < WCharTypeTableLimit );
    if ( ch >= WCharTypeTableLimit )
        return false;
    return ((WCharTypeTable[ ch - 128 ] & PunctCTBit) != 0);
}

//-----------------------------------------------------------------------------

bool 
IsAlNum( wchar_t ch )
{
    return (IsAlpha( ch ) || IsDigit( ch ));
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
    if ( ch < 128 )
        return static_cast< wchar_t >( toupper( ch ) );
    Assert( ch < WCharTypeTableLimit );
    if ( ch >= WCharTypeTableLimit )
        return ch;
    wchar_t u = ToUpperTable[ ch - 128 ];
    return ( u  ?  u  :  ch );
}

//-----------------------------------------------------------------------------

wchar_t 
ToLower( wchar_t ch )
{
    if ( ch < 128 )
        return static_cast< wchar_t >( tolower( ch ) );
    Assert( ch < WCharTypeTableLimit );
    if ( ch >= WCharTypeTableLimit )
        return ch;
    wchar_t l = ToLowerTable[ ch - 128 ];
    return ( l  ?  l  :  ch );
}

//-----------------------------------------------------------------------------

char 
ToASCII( wchar_t ch )
{
    if ( ch < 128 )
        return static_cast< char >( ch );
    Assert( ch < WCharTypeTableLimit );
    if ( ch >= WCharTypeTableLimit )
        return '_';
    return ToASCIITable[ ch - 128 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

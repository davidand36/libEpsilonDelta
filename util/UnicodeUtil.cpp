/*
  UnicodeUtil.cpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for Unicode strings.
*/


#include "UnicodeUtil.hpp"
#include "StringUtil.hpp"
#include "FixEndian.hpp"
#include "Exception.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Array.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


wstring 
DecodeUTF8( const string & utf8 )
{
    wstring unicode;

    for ( string::const_iterator p = utf8.begin(); p != utf8.end(); ++p )
    {
        uint32_t uni;
        if ( *p & (1 << 7) )    //byte begins with 1: lead byte of 
        {                       // multibyte sequence
            if ( (*p & (1 << 6)) == 0 ) //lead bytes don't begin with 10
                throw Exception( "Invalid UTF-8 string" );
            if ( *p & (1 << 5) )    //byte begins with 111
            {
                if ( *p & (1 << 4) )    //byte begins with 1111
                {
                    if ( *p & (1 << 3) )    //byte can't begin 11111
                        throw Exception( "Invalid UTF-8 string" );
                    uni = (*p++ & 0x07) << 6;
                    if ( p == utf8.end() )
                        throw Exception( "Invalid UTF-8 string" );
                    uni |= (*p++ & 0x3F);
                    if ( p == utf8.end() )
                        throw Exception( "Invalid UTF-8 string" );
                }
                else    //byte begins with 1110
                {
                    uni = (*p++ & 0x0F);
                    if ( p == utf8.end() )
                        throw Exception( "Invalid UTF-8 string" );
                }
                uni = (uni << 6) | (*p++ & 0x3F);
                if ( p == utf8.end() )
                    throw Exception( "Invalid UTF-8 string" );
            }
            else    //byte begins with 110
            {
                uni = (*p++ & 0x1F);
                if ( p == utf8.end() )
                    throw Exception( "Invalid UTF-8 string" );
            }
            uni = (uni << 6) | (*p & 0x3F);
        }
        else    //byte begins with 0: ASCII (single byte)
        {
            uni = *p;
        }

        if ( (sizeof( wchar_t ) > 2) || (uni <= 0xFFFF) )
        {
            unicode += static_cast< wchar_t >( uni );
        }
        else    //encoding to multibyte UTF-16 sequence
        {
            uni -= 0x10000;
            wchar_t wc = static_cast< wchar_t >( 0xD800
                                                 | ((uni >> 10) & 0x03FF) );
            unicode += wc;
            wc = static_cast< wchar_t >( 0xDC00 | (uni & 0x03FF) );
            unicode += wc;
        }
    }

    return unicode;
}

//-----------------------------------------------------------------------------

string 
EncodeUTF8( const wstring & unicode )
{
    string utf8;

    for ( wstring::const_iterator p = unicode.begin();
          p != unicode.end(); ++p )
    {
        uint32_t uni;
        if ( (*p >= 0xD800) && (*p <= 0xDFFF) ) //UTF-16 multibyte
        {
            if ( *p >= 0xDC00 )
                throw Exception( "Invalid UTF-16 string" );
            uni = (*p++ & 0x3FF) << 10;
            if ( p == unicode.end() )
                throw Exception( "Invalid UTF-16 string" );
            if ( (*p < 0xDC00) || (*p > 0xDFFF) )
                throw Exception( "Invalid UTF-16 string" );
            uni |= (*p & 0x3FF);
            uni += 0x10000;
        }
        else
            uni = *p;

        //if multibyte, append lead byte
        if ( uni >= 0x10000 )
            utf8 += static_cast< char >( ((uni >> 18) & 0x07) | 0xF0 );
        else if ( uni >= 0x800 )
            utf8 += static_cast< char >( ((uni >> 12) & 0x0F) | 0xE0 );
        else if ( uni >= 0x80 )
            utf8 += static_cast< char >( ((uni >> 6) & 0x1F) | 0xC0 );
        //now add tail surrogates or the whole ASCII value
        if ( uni >= 0x10000 )
            utf8 += static_cast< char >( ((uni >> 12) & 0x3F) | 0x80 );
        if ( uni >= 0x800 )
            utf8 += static_cast< char >( ((uni >> 6) & 0x3F) | 0x80 );
        if ( uni >= 0x80 )
            utf8 += static_cast< char >( (uni & 0x3F) | 0x80 );
        else
            utf8 += static_cast< char >( uni );
    }

    return utf8;
}

//=============================================================================

EUnicodeBOM 
IdentifyBOM( const char * str )
{
    unsigned char s0 = static_cast< unsigned char >( str[0] );
    unsigned char s1 = static_cast< unsigned char >( str[1] );
    unsigned char s2 = static_cast< unsigned char >( str[2] );
    unsigned char s3 = static_cast< unsigned char >( str[3] );
    switch ( s0 )
    {
    case 0xEF:
    {
        if ( (s1 == 0xBB) && (s2 == 0xBF) )
            return UTF8_BOM;
        break;
    }
    case 0xFE:
    {
        if ( s1 == 0xFF )
            return UTF16BE_BOM;
        break;
    }
    case 0xFF:
    {
        if ( s1 == 0xFE )
        {
            if ( (s2 == 0x00) && (s3 == 0x00) )
                return UTF32LE_BOM;
            return UTF16LE_BOM;
        }
        break;
    }
    case 0x00:
    {
        if ( (s1 == 0x00) && (s2 == 0xFE) && (s3 == 0xFF) )
            return UTF32BE_BOM;
        break;
    }
    default:
    {
        if ( (s0 == 0x0E) && (s1 == 0xFE) && (s2 == 0xFF) )
            return OtherBOM;    //SCSU
        if ( (s0 == 0x2B) && (s1 == 0x2F) && (s2 == 0x76)
            && ((s3 == 0x38) || (s3 == 0x39) || (s3 == 0x2B) || (s3 == 0x2F)) )
            return OtherBOM;    //UTF-7
        if ( (s0 == 0xDD) && (s1 == 0x73) && (s2 == 0x66)
             && (str[3] == 0x73) )
            return OtherBOM;    //UTF-EBCDIC
        if ( (s0 == 0xFB) && (s1 == 0xEE) && (s2 == 0x28) )
            return OtherBOM;    //BOCU-1
        break;
    }
    }
    return NoBOM;
}

//=============================================================================

std::string * 
RemoveBOM( std::string * pStr )
{
    return &(*pStr = RemoveBOM( *pStr ));
}

//-----------------------------------------------------------------------------

std::string 
RemoveBOM( const std::string & str )
{
    if ( str.length() < 3 )
        return str;
    if ( IdentifyBOM( str.c_str() ) != UTF8_BOM )
        return str;
    return str.substr( 3, string::npos );
}

//=============================================================================

std::wstring 
DecodeUnicodeWithBOM( const std::vector< char > encodedBuffer )
{
    const char * buffer = &encodedBuffer[0];
    int bufferSize = encodedBuffer.size();
    if ( encodedBuffer.size() < 4 )
        return DecodeUTF8( std::string( buffer, bufferSize ) );
    EUnicodeBOM bom = IdentifyBOM( buffer );
    switch ( bom )
    {
    case UTF8_BOM:
    case NoBOM:
        return DecodeUTF8( RemoveBOM( string( buffer, bufferSize ) ) );
    case UTF16BE_BOM:
    case UTF16LE_BOM:
    {
        bool bigEndian = (bom == UTF16BE_BOM);
        const uint16_t * pU16
                = reinterpret_cast< const uint16_t * >( buffer + 2 );
        int numWChars = (bufferSize - 2) / sizeof( uint16_t );
        wstring unicode;
        for ( int i = 0; i < numWChars; ++i )
        {
            uint16_t u16 = *pU16++;
            if ( bigEndian )
                FixBigEndian( &u16 );
            else
                FixLittleEndian( &u16 );
            if ( (sizeof( wchar_t ) > 2)
                 && (u16 >= 0xD800) && (u16 <= 0xDFFF) ) //UTF-16 multibyte
            {
                if ( u16 >= 0xDC00 )
                    throw Exception( "Invalid UTF-16 string" );
                uint32_t uni = (u16++ & 0x3FF) << 10;
                u16 = *pU16++;
                if ( bigEndian )
                    FixBigEndian( &u16 );
                else
                    FixLittleEndian( &u16 );
                if ( ++i >= numWChars )
                    throw Exception( "Invalid UTF-16 string" );
                if ( (u16 < 0xDC00) || (u16 > 0xDFFF) )
                    throw Exception( "Invalid UTF-16 string" );
                uni |= (u16 & 0x3FF);
                uni += 0x10000;
                unicode += static_cast< wchar_t >( uni );
            }
            else
            {
                unicode += static_cast< wchar_t >( u16 );
            }
        }
        return unicode;
    }
    case UTF32BE_BOM:
    case UTF32LE_BOM:
    {
        bool bigEndian = (bom == UTF32BE_BOM);
        const uint32_t * pU32
                = reinterpret_cast< const uint32_t * >( buffer + 4 );
        int numWChars = (bufferSize - 4) / sizeof( uint32_t );
        wstring unicode;
        for ( int i = 0; i < numWChars; ++i )
        {
            uint32_t u32 = *pU32++;
            if ( bigEndian )
                FixBigEndian( &u32 );
            else
                FixLittleEndian( &u32 );
            if ( (sizeof( wchar_t ) > 2) || (u32 <= 0xFFFF) )
            {
                unicode += static_cast< wchar_t >( u32 );
            }
            else    //encoding to multibyte UTF-16 sequence
            {
                u32 -= 0x10000;
                wchar_t wc = static_cast< wchar_t >( 0xD800
                                                    | ((u32 >> 10) & 0x03FF) );
                unicode += wc;
                wc = static_cast< wchar_t >( 0xDC00 | (u32 & 0x03FF) );
                unicode += wc;
            }
        }
        return unicode;
    }
    default:
    {
        Assert( 0 && "Unsupported Unicode BOM." );
        return L"";
    }
    }
}

//=============================================================================

#ifdef DEBUG

bool 
TestUnicodeUtil( )
{
    bool ok = true;
    cout << "Testing UnicodeUtil" << endl;

    unsigned char u8_1[] = { 0x41, 0xE2, 0x89, 0xA2, 0xCE, 0x91, 0x2E };
    string utf8_1 = string( reinterpret_cast< char * >( u8_1 ),
                            ARRAYSIZE( u8_1 ) );
    wchar_t u16_1[] = { 0x0041, 0x2262, 0x0391, 0x002E };
    wstring utf16_1 = wstring( u16_1, ARRAYSIZE( u16_1 ) );
    TESTCHECK( DecodeUTF8( utf8_1 ) == utf16_1, true, &ok );
    TESTCHECK( EncodeUTF8( utf16_1 ) == utf8_1, true, &ok );
    unsigned char u8_2[] = { 0xED, 0x95, 0x9C, 0xEA, 0xB5, 0xAD, 0xEC, 0x96,
                             0xB4 };
    string utf8_2 = string( reinterpret_cast< char * >( u8_2 ),
                            ARRAYSIZE( u8_2 ) );
    wchar_t u16_2[] = { 0xD55C, 0xAD6D, 0xC5B4 };
    wstring utf16_2 = wstring( u16_2, ARRAYSIZE( u16_2 ) );
    TESTCHECK( DecodeUTF8( utf8_2 ) == utf16_2, true, &ok );
    TESTCHECK( EncodeUTF8( utf16_2 ) == utf8_2, true, &ok );
    unsigned char u8_3[] = { 0xE6, 0x97, 0xA5, 0xE6, 0x9C, 0xAC, 0xE8, 0xAA,
                             0x9E };
    string utf8_3 = string( reinterpret_cast< char * >( u8_3 ),
                            ARRAYSIZE( u8_3 ) );
    wchar_t u16_3[] = { 0x65E5, 0x672C, 0x8A9E };
    wstring utf16_3 = wstring( u16_3, ARRAYSIZE( u16_3 ) );
    TESTCHECK( DecodeUTF8( utf8_3 ) == utf16_3, true, &ok );
    TESTCHECK( EncodeUTF8( utf16_3 ) == utf8_3, true, &ok );
    unsigned char u8_4[] = { 0xEF, 0xBB, 0xBF, 0xF0, 0xA3, 0x8E, 0xB4 };
    string utf8_4 = string( reinterpret_cast< char * >( u8_4 ),
                            ARRAYSIZE( u8_4 ) );
    wchar_t u16_4[] = { 0xFEFF, 0xD84C, 0xDFB4 };
    wstring utf16_4 = wstring( u16_4, ARRAYSIZE( u16_4 ) );
    if ( sizeof( wchar_t ) <= 2 )
        TESTCHECK( DecodeUTF8( utf8_4 ) == utf16_4, true, &ok );
    TESTCHECK( EncodeUTF8( utf16_4 ) == utf8_4, true, &ok );
    if ( sizeof( wchar_t ) > 2 )
    {
        //This will produce a warning if sizeof(wchar_t)<=2;
        // it can safely be ignored.
        wchar_t u32_4[] = { 0xFEFF, static_cast< wchar_t >( 0x233B4 ) };
        wstring utf32_4 = wstring( u32_4, ARRAYSIZE( u32_4 ) );
        TESTCHECK( DecodeUTF8( utf8_4 ) == utf32_4, true, &ok );
        TESTCHECK( EncodeUTF8( utf32_4 ) == utf8_4, true, &ok );
    }

    unsigned char u8_5[] = { 0xEF, 0xBB, 0xBF, 0x54, 0x65, 0x73, 0x74, 0x20,
                             0x66, 0x69, 0x6C, 0x65, 0x0D, 0x0A, 0xC3, 0x87,
                             0xC4, 0xB1, 0xC4, 0x9F };
    const char * c8_5 = reinterpret_cast< char * >( u8_5 );
    TESTCHECK( IdentifyBOM( c8_5 ), UTF8_BOM, &ok );
    string utf8_5( c8_5, ARRAYSIZE( u8_5 ) );
    TESTCHECK( RemoveBOM( utf8_5 ) ==
               string( (c8_5 + 3), ARRAYSIZE( u8_5 ) - 3 ), true, &ok );
    vector< char > charVec1( c8_5, c8_5 + ARRAYSIZE( u8_5 ) );
    wchar_t u16_5[] = { 0x0054, 0x0065, 0x0073, 0x0074, 0x0020, 0x0066,
                        0x0069, 0x006C, 0x0065, 0x000D, 0x000A, 0x00C7,
                        0x0131, 0x011F };
    wstring utf16_5( u16_5, ARRAYSIZE( u16_5 ) );
    TESTCHECK( DecodeUnicodeWithBOM( charVec1 ) == utf16_5, true, &ok );
    unsigned char u8_6[] = { 0xFE, 0xFF, 0x00, 0x54, 0x00, 0x65, 0x00, 0x73,
                             0x00, 0x74, 0x00, 0x20, 0x00, 0x66, 0x00, 0x69,
                             0x00, 0x6C, 0x00, 0x65, 0x00, 0x0D, 0x00, 0x0A,
                             0x00, 0xC7, 0x01, 0x31, 0x01, 0x1F };
    const char * c8_6 = reinterpret_cast< char * >( u8_6 );
    TESTCHECK( IdentifyBOM( c8_6 ), UTF16BE_BOM, &ok );
    vector< char > charVec2( c8_6, c8_6 + ARRAYSIZE( u8_6 ) );
    TESTCHECK( DecodeUnicodeWithBOM( charVec2 ) == utf16_5, true, &ok );
    unsigned char u8_7[] = { 0xFF, 0xFE, 0x54, 0x00, 0x65, 0x00, 0x73, 0x00,
                             0x74, 0x00, 0x20, 0x00, 0x66, 0x00, 0x69, 0x00,
                             0x6C, 0x00, 0x65, 0x00, 0x0D, 0x00, 0x0A, 0x00,
                             0xC7, 0x00, 0x31, 0x01, 0x1F, 0x01 };
    const char * c8_7 = reinterpret_cast< char * >( u8_7 );
    TESTCHECK( IdentifyBOM( c8_7 ), UTF16LE_BOM, &ok );
    vector< char > charVec3( c8_7, c8_7 + ARRAYSIZE( u8_7 ) );
    TESTCHECK( DecodeUnicodeWithBOM( charVec3 ) == utf16_5, true, &ok );
    
    if ( ok )
        cout << "UnicodeUtil PASSED." << endl << endl;
    else
        cout << "UnicodeUtil FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

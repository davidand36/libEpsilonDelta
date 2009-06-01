#ifndef UNICODEUTIL_HPP
#define UNICODEUTIL_HPP
/*
  UnicodeUtil.hpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for Unicode strings.
  NOTES:
  1. DecodeUTF8 and EncodeUTF8 convert between UTF-8 and either UTF-16 or
     UTF-32, depending on sizeof(wchar_t), which is typically 4 (32 bits) on
     Unix/Linux, and 2 (16 bits) on Windows. For most purposes these are
     equivalent, since 16-bits is enough for the Basic Multilingual Plane,
     which covers nearly all important languages and symbols. (UTF-8 leaves
     ASCII text unchanged.)
  2. BOMs (byte-order marks) are sometimes prepended to files to indicate the
     encoding and byte order, especially by Windows software. Routines are
     provided here to identify the BOM, remove it from UTF-8 strings, and to
     decode a buffer to Unicode.
  3. If no BOM is found, DecodeUnicodeWithBOM() assumes UTF-8.
*/


#include <string>
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


std::wstring DecodeUTF8( const std::string & utf8 );
std::string EncodeUTF8( const std::wstring & unicode );

enum EUnicodeBOM
{
    NoBOM,
    UTF8_BOM, UTF16BE_BOM, UTF16LE_BOM, UTF32BE_BOM, UTF32LE_BOM,
    OtherBOM
};

EUnicodeBOM IdentifyBOM( const char * str );

std::string * RemoveBOM( std::string * pStr );
std::string RemoveBOM( const std::string & str );

std::wstring DecodeUnicodeWithBOM( const std::vector< char > encodedBuffer );

#ifdef DEBUG
bool TestUnicodeUtil( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //UNICODEUTIL_HPP

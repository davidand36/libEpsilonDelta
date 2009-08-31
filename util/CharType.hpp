#ifndef CHARTYPE_HPP
#define CHARTYPE_HPP
/*
  CharType.hpp
  Copyright (C) 2007 David M. Anderson

  Character classification and conversion functions.
  NOTES:
  1. These are intended as a short-term solution to handling Unicode characters
     outside the ASCII range.
  2. Only values less than WCharTypeTableLimt are handled by the wchar_t
     routines. Larger values result in an Assert.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool IsUpper( char ch );
bool IsLower( char ch );
bool IsAlpha( char ch );
bool IsDigit( char ch );
bool IsXDigit( char ch );
bool IsSpace( char ch );
bool IsPrint( char ch );
bool IsGraph( char ch );
bool IsBlank( char ch );
bool IsCntrl( char ch );
bool IsPunct( char ch );
bool IsAlNum( char ch );
char ToUpper( char ch );
char ToLower( char ch );
char ToASCII( char ch );

bool IsUpper( wchar_t ch );
bool IsLower( wchar_t ch );
bool IsAlpha( wchar_t ch );
bool IsDigit( wchar_t ch );
bool IsXDigit( wchar_t ch );
bool IsSpace( wchar_t ch );
bool IsPrint( wchar_t ch );
bool IsGraph( wchar_t ch );
bool IsBlank( wchar_t ch );
bool IsCntrl( wchar_t ch );
bool IsPunct( wchar_t ch );
bool IsAlNum( wchar_t ch );
wchar_t ToUpper( wchar_t ch );
wchar_t ToLower( wchar_t ch );
char ToASCII( wchar_t ch );

extern const int WCharTypeTableLimit;


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CHARTYPE_HPP

#ifndef STRINGUTIL_HPP
#define STRINGUTIL_HPP
/*
  StringUtil.hpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for strings (C- and C++-style).
  NOTES:
  1. There are two versions each of ToUpper(), ToLower(), Trim...(),
     AddQuotes(), RemoveQuotes(), and Replace(). The first versions modify the
     contents of their arguments. They also return a pointer to the modified
     result, so that such calls can be nested. The second does not modify the
     argument, which is therefore const, but returns the resulting string,
     also allowing nesting.
  2. AddQuotes() surrounds the text in quotes.
     The AddQuotes( pStr, quote ) version first replaces any instances of
     quote in the original text with a pair of quotes.
     The AddQuotes( pStr, quote, escape ) version prepends the escape
     character to any instances of escape or quote in the original text.
     RemoveQuotes() reverses the effect of AddQuotes(), but does not
     assume that the text begins or ends with quotes.
  3. Split() always returns a string, possibly empty, on each side of each
     delimiter found. So the size of the returned vector is always
     one plus the number of occurrences of the delimiter.
  4. (i) The versions of Split() with a quote parameter mean that any text
     included between two quotes is taken literally. In particular, any
     delimiter characters within quotes are not used to split the string.
     Within quoted text, a pair of quotes is taken to represent a single quote.
     (ii)The version with an escape parameter means that any character
     following the escape character is taken literally, allowing substrings
     that contain the delimeter, quote, or escape characters.
     (iii) If the escape character is the same as the quote character, then it
     only affects a subsequent quote character.
     (iv) By default, the quote and escape characters are themselves not
     retained in the resulting substrings, but this behavior can be modified
     with the retainQuotes and retainEscapes parameters.
     (v) See CSV.hpp for a working example.
  5. In the routines IntToString() and RealToString(), the width argument
     specifies the minimum length of the resulting string; more characters
     will be used if needed. zeroFill specifies whether leading zeros are
     supplied to fill out the width; otherwise spaces are used. showSign
     specifies whether '+' is prepended to positive values. point and comma
     are the characters to be used for the decimal point and, if nonzero,
     for the separator between thousands, millions, etc.
*/


#include "CharType.hpp"
#include "Platform.hpp"
#include "Assert.hpp"
#include <string>
#include <vector>
#include <cctype>
#include <cmath>

#ifdef OS_ANDROID //Required until NDK improves
namespace std
{
typedef basic_string< wchar_t >  wstring;
}
#define WCOUT_AVAILABLE 0
#else
#define WCOUT_AVAILABLE 1
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename Ch >
Ch * 
ToUpper( Ch * str );

template < typename Ch >
Ch * 
ToLower( Ch * str );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
ToUpper( std::basic_string< Ch, Tr, A > * pStr );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
ToUpper( const std::basic_string< Ch, Tr, A > & str );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > *
ToLower( std::basic_string< Ch, Tr, A > * pStr );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
ToLower( const std::basic_string< Ch, Tr, A > & str );

template < typename Ch >
int
CompareNoCase( const Ch * lhs, const Ch * rhs );

template < typename Ch, typename Tr, typename A >
int 
CompareNoCase( const std::basic_string< Ch, Tr, A > & lhs,
               const std::basic_string< Ch, Tr, A > & rhs );

template < typename Ch >
Ch * 
TrimLeading( Ch * str );

template < typename Ch >
Ch * 
TrimTrailing( Ch * str );

template < typename Ch >
Ch * 
Trim( Ch * str );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
TrimLeading( std::basic_string< Ch, Tr, A > * pStr );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
TrimLeading( const std::basic_string< Ch, Tr, A > & str );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
TrimTrailing( std::basic_string< Ch, Tr, A > * pStr );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
TrimTrailing( const std::basic_string< Ch, Tr, A > & str );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
Trim( std::basic_string< Ch, Tr, A > * pStr );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
Trim( const std::basic_string< Ch, Tr, A > & str );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
AddQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
AddQuotes( const std::basic_string< Ch, Tr, A > & str, Ch quote );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
AddQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote, Ch escape );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
AddQuotes( const std::basic_string< Ch, Tr, A > & str, Ch quote, Ch escape );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
RemoveQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
RemoveQuotes( const std::basic_string< Ch, Tr, A > & str, Ch quote );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
RemoveQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote, Ch escape );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
RemoveQuotes( const std::basic_string< Ch, Tr, A > & str,
              Ch quote, Ch escape );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
Replace( std::basic_string< Ch, Tr, A > * pStr,
         const std::basic_string< Ch, Tr, A > & oldSubstring,
         const std::basic_string< Ch, Tr, A > & newSubstring );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
Replace( const std::basic_string< Ch, Tr, A > & str,
         const std::basic_string< Ch, Tr, A > & oldSubstring,
         const std::basic_string< Ch, Tr, A > & newSubstring );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
Replace( std::basic_string< Ch, Tr, A > * pStr,
         const Ch * oldSubstring, const Ch * newSubstring );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
Replace( const std::basic_string< Ch, Tr, A > & str,
         const Ch * oldSubstring, const Ch * newSubstring );

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > > 
Split( const std::basic_string< Ch, Tr, A > & str, Ch delimiter );

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > > 
Split( const std::basic_string< Ch, Tr, A > & str, Ch delimiter,
       Ch quote, bool retainQuotes = false );

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > > 
Split( const std::basic_string< Ch, Tr, A > & str, Ch delimiter,
       Ch quote, Ch escape,
       bool retainQuotes = false, bool retainEscapes = false );

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > > 
Split( const std::basic_string< Ch, Tr, A > & str,
       const std::basic_string< Ch, Tr, A > & delimiter );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > 
IntToBasicString( long i, int width = 0, Ch comma = 0,
                  bool zeroFill = false, bool showSign = false );

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > 
RealToBasicString( double r, int width = 0, int decimals = 0,
                   Ch point = '.', Ch comma = 0,
                   bool zeroFill = false, bool showSign = false );

std::string 
IntToString( long i, int width = 0, char comma = 0,
             bool zeroFill = false, bool showSign = false );

std::wstring 
IntToWString( long i, int width = 0, char comma = 0,
              bool zeroFill = false, bool showSign = false );

std::string 
RealToString( double r, int width = 0, int decimals = 0,
              char point = '.', char comma = 0,
              bool zeroFill = false, bool showSign = false );

std::wstring 
RealToWString( double r, int width = 0, int decimals = 0,
              char point = '.', char comma = 0,
              bool zeroFill = false, bool showSign = false );

std::string 
OrdinalToString( long i, int width = 0, char comma = 0 );

std::wstring 
OrdinalToWString( long i, int width = 0, char comma = 0 );

int
StringToInt( std::string str, int def = 0, int pos = 0, int len = -1 );

long
StringToLong( std::string str, long def = 0L, int pos = 0, int len = -1 );

double
StringToReal( std::string str, double def = 0., int pos = 0, int len = -1 );

enum LineBreakAction
{
    LBA_Prohibited,
    LBA_Required,
    LBA_Allowed,
    LBA_AllowedWithPenalty
};

void DetermineLineBreakOpportunities( const std::wstring & text,
                                    std::vector< LineBreakAction > * pActions );

#ifdef DEBUG
bool TestStringUtil( );
bool TestLineBreakOpportunities( const std::string & testFileText );
#endif


//#############################################################################


template < typename Ch >
Ch * 
ToUpper( Ch * str )
{
    for ( Ch * p = str; *p; ++p )
        *p = static_cast< Ch >( ToUpper( *p ) );
    return str;
}

//-----------------------------------------------------------------------------

template < typename Ch >
Ch * 
ToLower( Ch * str )
{
    for ( Ch * p = str; *p; ++p )
        *p = static_cast< Ch >( ToLower( *p ) );
    return str;
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
ToUpper( std::basic_string< Ch, Tr, A > * pStr )
{
    for ( typename std::basic_string< Ch, Tr, A >::iterator  p = pStr->begin();
          p != pStr->end(); ++p )
        *p = static_cast< Ch >( ToUpper( *p ) );
    return pStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
ToUpper( const std::basic_string< Ch, Tr, A > & str )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( ToUpper( &newStr ) );
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
ToLower( std::basic_string< Ch, Tr, A > * pStr )
{
    for ( typename std::basic_string< Ch, Tr, A >::iterator  p = pStr->begin();
          p != pStr->end(); ++p )
        *p = static_cast< Ch >( ToLower( *p ) );
    return pStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
ToLower( const std::basic_string< Ch, Tr, A > & str )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( ToLower( &newStr ) );
}

//=============================================================================

template < typename Ch >
int 
CompareNoCase( const Ch * lhs, const Ch * rhs )
{
    for ( ; *lhs && *rhs; ++lhs, ++rhs )
        if ( ToUpper( *lhs ) != ToUpper( *rhs ) )
            return ( (ToUpper( *lhs ) < ToUpper( *rhs )) ? -1 : 1 );
    if ( *lhs )
        return 1;
    if ( *rhs )
        return -1;
    return 0;
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
int 
CompareNoCase( const std::basic_string< Ch, Tr, A > & lhs,
               const std::basic_string< Ch, Tr, A > & rhs )
{
    typename std::basic_string< Ch, Tr, A >::const_iterator  pL = lhs.begin();
    typename std::basic_string< Ch, Tr, A >::const_iterator  pR = rhs.begin();
    for ( ; pL != lhs.end() && pR != rhs.end(); ++pL, ++pR )
        if ( ToUpper( *pL ) != ToUpper( *pR ) )
            return ( (ToUpper( *pL ) < ToUpper( *pR )) ? -1 : 1 );
    if ( pL != lhs.end() )
        return 1;
    if ( pR != rhs.end() )
        return -1;
    return 0;
}

//=============================================================================

template < typename Ch >
Ch * 
TrimLeading( Ch * str )
{
    Ch * p = str;
    while ( IsSpace( *p ) )
        ++p;
    if ( p != str )
    {
        Ch * q = str;
        while ( *p != 0 )
            *q++ = *p++;
        *q = 0;
    }
    return str;
}

//-----------------------------------------------------------------------------

template < typename Ch >
Ch * 
TrimTrailing( Ch * str )
{
    Ch * p = str;
    while ( *p != 0 )
        ++p;
    --p;
    while ( (p >= str) && IsSpace( *p ) )
        --p;
    *(p + 1) = 0;
    return str;
}

//-----------------------------------------------------------------------------

template < typename Ch >
Ch * 
Trim( Ch * str )
{
    return TrimLeading( TrimTrailing( str ) );
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
TrimLeading( std::basic_string< Ch, Tr, A > * pStr )
{
    return &(*pStr = TrimLeading( *pStr ));
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
TrimLeading( const std::basic_string< Ch, Tr, A > & str )
{
    typename std::basic_string< Ch, Tr, A >::size_type len = str.length();
    typename std::basic_string< Ch, Tr, A >::size_type i = 0;
    while ( (i < len) && IsSpace( str[i] ) )
        ++i;
    return str.substr( i, std::basic_string< Ch, Tr, A >::npos );
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
TrimTrailing( std::basic_string< Ch, Tr, A > * pStr )
{
    return &(*pStr = TrimTrailing( *pStr ));
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
TrimTrailing( const std::basic_string< Ch, Tr, A > & str )
{
    typename std::basic_string< Ch, Tr, A >::size_type i = str.length();
    while ( (i > 0) && IsSpace( str[ i - 1 ] ) )
        --i;
    return str.substr( 0, i );
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
Trim( std::basic_string< Ch, Tr, A > * pStr )
{
    return TrimLeading( TrimTrailing( pStr ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
Trim( const std::basic_string< Ch, Tr, A > & str )
{
    return TrimLeading( TrimTrailing( str ) );
}

//=============================================================================

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > *
AddQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote )
{
    std::basic_string< Ch, Tr, A > oldQuote( 1, quote );
    std::basic_string< Ch, Tr, A > newQuote( 2, quote );
    Replace( pStr, oldQuote, newQuote );
    *pStr = quote + *pStr + quote;
    return pStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
AddQuotes( const std::basic_string< Ch, Tr, A > & str, Ch quote )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( AddQuotes( &newStr, quote ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > *
AddQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote, Ch escape )
{
    std::basic_string< Ch, Tr, A > oldEscape( 1, escape );
    std::basic_string< Ch, Tr, A > newEscape( 2, escape );
    Replace( pStr, oldEscape, newEscape );
    std::basic_string< Ch, Tr, A > oldQuote( 1, quote );
    std::basic_string< Ch, Tr, A > newQuote( 2, quote );
    newQuote[0] = escape;
    Replace( pStr, oldQuote, newQuote );
    *pStr = quote + *pStr + quote;
    return pStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
AddQuotes( const std::basic_string< Ch, Tr, A > & str, Ch quote, Ch escape )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( AddQuotes( &newStr, quote, escape ) );
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > *
RemoveQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote )
{
    bool inQuotes = false;
    typename std::basic_string< Ch, Tr, A >::iterator p = pStr->begin();
    while ( p != pStr->end() )
    {
        if ( *p == quote )
        {
            p = pStr->erase( p );
            if ( ! inQuotes )
                inQuotes = true;
            else if ( (p != pStr->end()) && (*p == quote) )
                ++p;    //consecutive quotes = literal quote
            else
                inQuotes = false;
        }
        else
            ++p;
    }
    return pStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
RemoveQuotes( const std::basic_string< Ch, Tr, A > & str, Ch quote )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( RemoveQuotes( &newStr, quote ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > *
RemoveQuotes( std::basic_string< Ch, Tr, A > * pStr, Ch quote, Ch escape )
{
    bool inQuotes = false;
    typename std::basic_string< Ch, Tr, A >::iterator p = pStr->begin();
    while ( p != pStr->end() )
    {
        if ( *p == quote )
        {
            p = pStr->erase( p );
            inQuotes = ! inQuotes;
        }
        else if ( *p == escape )
        {
            p = pStr->erase( p );
            if ( p != pStr->end() )
                ++p;
        }
        else
            ++p;
    }
    return pStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
RemoveQuotes( const std::basic_string< Ch, Tr, A > & str, Ch quote, Ch escape )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( RemoveQuotes( &newStr, quote, escape ) );
}

//=============================================================================

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > *
Replace( std::basic_string< Ch, Tr, A > * pStr,
         const std::basic_string< Ch, Tr, A > & oldSubstring,
         const std::basic_string< Ch, Tr, A > & newSubstring )
{
    typename std::basic_string< Ch, Tr, A >::size_type  oldSubstLen
            = oldSubstring.length();
    typename std::basic_string< Ch, Tr, A >::size_type  newSubstLen
            = newSubstring.length();
    typename std::basic_string< Ch, Tr, A >::size_type  pos = 0;
    while ( pos < pStr->length() )
    {
        pos = pStr->find( oldSubstring, pos );
        if ( pos == std::basic_string< Ch, Tr, A >::npos )
            break;
        pStr->replace( pos, oldSubstLen, newSubstring );
        pos += newSubstLen;
    }
    return pStr;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
Replace( const std::basic_string< Ch, Tr, A > & str, 
         const std::basic_string< Ch, Tr, A > & oldSubstring,
         const std::basic_string< Ch, Tr, A > & newSubstring )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( Replace( &newStr, oldSubstring, newSubstring ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A > * 
Replace( std::basic_string< Ch, Tr, A > * pStr,
         const Ch * oldSubstring, const Ch * newSubstring )
{
    return Replace( pStr, std::basic_string< Ch, Tr, A >( oldSubstring ),
                    std::basic_string< Ch, Tr, A >( newSubstring ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
Replace( const std::basic_string< Ch, Tr, A > & str,
         const Ch * oldSubstring, const Ch * newSubstring )
{
    std::basic_string< Ch, Tr, A > newStr( str );
    return *( Replace( &newStr, oldSubstring, newSubstring ) );
}

//=============================================================================

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > > 
Split( const std::basic_string< Ch, Tr, A > & str, Ch delimiter )
{
    std::vector< std::basic_string< Ch, Tr, A > > parts;
    std::basic_string< Ch, Tr, A > curPart;
    typename std::basic_string< Ch, Tr, A >::const_iterator  p = str.begin();
    while ( p != str.end() )
    {
        if ( *p == delimiter )
        {
            parts.push_back( curPart );
            curPart.erase( );
            ++p;
        }
        else
            curPart += *p++;
    }
    parts.push_back( curPart );
    return parts;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > >
Split( const std::basic_string< Ch, Tr, A > & str, Ch delimiter, Ch quote, bool retainQuotes )
{
    std::vector< std::basic_string< Ch, Tr, A > > parts;
    std::basic_string< Ch, Tr, A > curPart;
    bool inQuotes = false;
    typename std::basic_string< Ch, Tr, A >::const_iterator  p = str.begin();
    while ( p != str.end() )
    {
        if ( *p == quote )
        {
            if ( inQuotes && (p + 1 != str.end()) && (*(p + 1) == quote ) )
            {
                if ( retainQuotes )
                    curPart += quote;
                ++p;
                curPart += *p++;
            }
            else
            {
                inQuotes = ! inQuotes;
                if ( retainQuotes )
                    curPart += quote;
                ++p;
            }
        }
        else if ( (*p == delimiter) && (! inQuotes) )
        {
            parts.push_back( curPart );
            curPart.erase( );
            ++p;
        }
        else
            curPart += *p++;
    }
    parts.push_back( curPart );
    return parts;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > >
Split( const std::basic_string< Ch, Tr, A > & str,
       Ch delimiter, Ch quote, Ch escape,
       bool retainQuotes, bool retainEscapes )
{
    if ( escape == quote )
        return Split( str, delimiter, quote, retainQuotes );
    std::vector< std::basic_string< Ch, Tr, A > > parts;
    std::basic_string< Ch, Tr, A > curPart;
    bool inQuotes = false;
    typename std::basic_string< Ch, Tr, A >::const_iterator  p = str.begin();
    while ( p != str.end() )
    {
        if ( *p == quote )
        {
            inQuotes = ! inQuotes;
            if ( retainQuotes )
                curPart += quote;
            ++p;
        }
        else if ( *p == escape )
        {
            if ( retainEscapes )
                curPart += escape;
            ++p;
            if ( p != str.end() )
                curPart += *p++;
        }
        else if ( (*p == delimiter) && (! inQuotes) )
        {
            parts.push_back( curPart );
            curPart.erase( );
            ++p;
        }
        else
            curPart += *p++;
    }
    parts.push_back( curPart );
    return parts;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Ch, typename Tr, typename A >
std::vector< std::basic_string< Ch, Tr, A > > 
Split( const std::basic_string< Ch, Tr, A > & str,
       const std::basic_string< Ch, Tr, A > & delimiter )
{
    std::vector< std::basic_string< Ch, Tr, A > > parts;
    typedef typename std::basic_string< Ch, Tr, A >::size_type  size_type;
    size_type startPos = 0;
    while ( true )
    {
        size_type endPos = str.find( delimiter, startPos );
        if ( endPos == std::basic_string< Ch, Tr, A >::npos )
        {
            parts.push_back( str.substr( startPos ) );
            break;
        }
        else
            parts.push_back( str.substr( startPos, endPos - startPos ) );
        startPos = endPos + delimiter.length();
    }
    return parts;
}

//=============================================================================

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
IntToBasicString( long i, int width, Ch comma, bool zeroFill, bool showSign )
{
    Assert( width <= 29 ); //Enough for 64-bit signed value
    Ch buff[ 30 ];
    buff[ 29 ] = 0;
    bool negative = false;
    if ( i < 0 )
    {
        negative = true;
        i = -i;
    }
    if ( negative || showSign )
        --width;
    Ch * front = buff + 28;
    int characters = 0;
    int digits = 0;
    while ( (i != 0) || (digits == 0) )
    {
        *front-- = static_cast< Ch >( (i % 10) + '0' );
        i /= 10;
        ++characters;
        ++digits;
        if ( comma && (digits % 3 == 0) && (i != 0) )
        {
            *front-- = comma;
            ++characters;
        }
    }
    while ( characters < width )
    {
        *front-- = static_cast< Ch >( (zeroFill ? '0' : ' ') );
        ++characters;
    }
    if ( negative )
        *front-- = static_cast< Ch >( '-' );
    else if ( showSign )
        *front-- = static_cast< Ch >( '+' );
    return std::basic_string< Ch, Tr, A >( ++front );
}

//-----------------------------------------------------------------------------

template < typename Ch, typename Tr, typename A >
std::basic_string< Ch, Tr, A >
RealToBasicString( double r, int width, int decimals, Ch point, Ch comma,
                   bool zeroFill, bool showSign )
{
    //NB: This approach will only accommodate a limited range.
    bool negative = false;
    if ( r < 0. )
    {
        negative = true;
        r = -r;
    }
    if ( negative || showSign )
        --width;
    double whole;
    double fraction = modf( r, &whole );
    int wholeInt = static_cast<int>( whole );
    if ( decimals > 0 )
        width -= decimals + 1;
    std::basic_string< Ch, Tr, A > realString;
    if ( negative )
        realString += static_cast< Ch >( '-' );
    else if ( showSign )
        realString += static_cast< Ch >( '+' );
    realString += IntToBasicString< Ch, Tr, A >( wholeInt, width, comma,
                                                 zeroFill, false );
    if ( decimals > 0 )
    {
        realString += point;
        for ( int i = 0; i < decimals; ++i )
            fraction *= 10.;
        fraction += 0.5;
        int fractionInt = static_cast<int>( floor( fraction ) );
        realString += IntToBasicString< Ch, Tr, A >( fractionInt, decimals,
                                                     false, true, false );
    }
    return realString;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //STRINGUTIL_HPP

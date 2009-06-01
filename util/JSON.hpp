#ifndef JSON_HPP
#define JSON_HPP
/*
  JSON.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for serializing and deserializing basic C++ type objects in
  JSON (JavaScript Object Notation).
*/


#include "StringUtil.hpp"
#include "StdInt.hpp"
#include "Array.hpp"
#include "VMap.hpp"
#include <map>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef std::map< std::string, std::string >  JSONObject;

std::string ToJSON( bool b );
std::string ToJSON( int8_t i );
std::string ToJSON( uint8_t i );
std::string ToJSON( int16_t i );
std::string ToJSON( uint16_t i );
std::string ToJSON( int32_t i );
std::string ToJSON( uint32_t i );
std::string ToJSON( int64_t i );
std::string ToJSON( uint64_t i );
std::string ToJSON( float r );
std::string ToJSON( double r );
std::string ToJSON( long double r );
std::string ToJSON( const char * s );
std::string ToJSON( const std::string & s );
std::string ToJSON( const std::wstring & s );
template < typename T, size_t N >
std::string ToJSON( const array< T, N > & a );
template < typename T >
std::string ToJSON( const std::vector< T > & v );
std::string ToJSON( const JSONObject & o );

void FromJSON( const std::string & json, bool * pB );
void FromJSON( const std::string & json, char * pC );
void FromJSON( const std::string & json, wchar_t * pC );
void FromJSON( const std::string & json, int8_t * pI );
void FromJSON( const std::string & json, uint8_t * pI );
void FromJSON( const std::string & json, int16_t * pI );
void FromJSON( const std::string & json, uint16_t * pI );
void FromJSON( const std::string & json, int32_t * pI );
void FromJSON( const std::string & json, uint32_t * pI );
void FromJSON( const std::string & json, int64_t * pI );
void FromJSON( const std::string & json, uint64_t * pI );
void FromJSON( const std::string & json, float * pR );
void FromJSON( const std::string & json, double * pR );
void FromJSON( const std::string & json, long double * pR );
void FromJSON( const std::string & json, std::string * pS );
void FromJSON( const std::string & json, std::wstring * pS );
template < typename T, size_t N >
void FromJSON( const std::string & json, array< T, N > * pA );
template < typename T >
void FromJSON( const std::string & json, std::vector< T > * pV );
void FromJSON( const std::string & json, JSONObject * pO );

//helper:
void SplitJSONArray( const std::string & json,
                     std::vector< std::string > * pStringVec );

#ifdef DEBUG
bool TestJSON( );
#endif


//#############################################################################


template < typename T, size_t N >
std::string
ToJSON( const array< T, N > & a )
{
    std::string json = "[ ";
    for ( int i = 0; i < N; ++i )
    {
        if ( i > 0 )
            json += ", ";
        json += ToJSON( a[i] );
    }
    json += " ]";
    return json;
}

//.............................................................................

template < typename T >
std::string
ToJSON( const std::vector< T > & v )
{
    std::string json = "[ ";
    for ( size_t i = 0; i < v.size(); ++i )
    {
        if ( i > 0 )
            json += ", ";
        json += ToJSON( v[i] );
    }
    json += " ]";
    return json;
}

//=============================================================================

template < typename T, size_t N >
void 
FromJSON( const std::string & json, array< T, N > * pA )
{
    std::vector< std::string > stringVec;
    SplitJSONArray( json, &stringVec );
    for ( int i = 0; i < N; ++i )
    {
        T t;
        FromJSON( stringVec[i], &t );
        (*pA)[i] = t;
    }
}

//.............................................................................

template < typename T >
void 
FromJSON( const std::string & json, std::vector< T > * pV )
{
    pV->clear( );
    std::vector< std::string > stringVec;
    SplitJSONArray( json, &stringVec );
    for ( std::vector< std::string >::const_iterator pS = stringVec.begin();
          pS != stringVec.end(); ++pS )
    {
        T t;
        FromJSON( *pS, &t );
        pV->push_back( t );
    }
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JSON_HPP

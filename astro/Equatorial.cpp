/*
  Equatorial.cpp
  Copyright (C) 2007 David M. Anderson

  Equatorial class: represents equatorial coordinates of a point.
*/


#include "Equatorial.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


string 
ToJSON( const Equatorial & equ )
{
    JSONObject jsonObj;
    jsonObj[ "right_ascension" ] = ToJSON( equ.RightAscension() );
    jsonObj[ "declination" ] = ToJSON( equ.Declination() );
    jsonObj[ "distance" ] = ToJSON( equ.Distance() );
    return ToJSON( jsonObj );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, Equatorial * pEqu )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    Angle ra, dec;
    double distance;
    FromJSON( jsonObj[ "right_ascension" ], &ra );
    FromJSON( jsonObj[ "declination" ], &dec );
    FromJSON( jsonObj[ "distance" ], &distance );
    pEqu->Set( ra, dec, distance );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


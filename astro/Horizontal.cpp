/*
  Horizontal.cpp
  Copyright (C) 2007 David M. Anderson

  Horizontal class: represents horizontal coordinates of a point.
*/


#include "Horizontal.hpp"
#include "JSON.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


string 
ToJSON( const Horizontal & hor )
{
    JSONObject jsonObj;
    jsonObj[ "azimuth" ] = ToJSON( hor.Azimuth() );
    jsonObj[ "altitude" ] = ToJSON( hor.Altitude() );
    jsonObj[ "distance" ] = ToJSON( hor.Distance() );
    return ToJSON( jsonObj );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, Horizontal * pHor )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    Angle az, alt;
    double distance;
    FromJSON( jsonObj[ "azimuth" ], &az );
    FromJSON( jsonObj[ "altitude" ], &alt );
    FromJSON( jsonObj[ "distance" ], &distance );
    pHor->Set( az, alt, distance );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

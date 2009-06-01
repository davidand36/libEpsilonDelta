/*
  GeodeticDatum.cpp
  Copyright (C) 2007 David M. Anderson

  GeodeticDatum class, representing a specific regional geodetic datum or
  global reference system.
*/


#include "GeodeticDatum.hpp"
#include "JSON.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


GeodeticDatum::GeodeticDatum( const GeodeticEllipsoid & ellipsoid )
    :   m_ellipsoid( ellipsoid )
{
}

//=============================================================================

const GeodeticEllipsoid & 
GeodeticDatum::Ellipsoid( ) const
{
    return m_ellipsoid;
}

//=============================================================================

ostream & 
operator<<( ostream & out, const GeodeticDatum & datum )
{
    return out << "( " << datum.Ellipsoid() << " )";
}

//=============================================================================

string 
ToJSON( const GeodeticDatum & datum )
{
    JSONObject jsonObj;
    jsonObj[ "ellipsoid" ] = ToJSON( datum.Ellipsoid() );
    return ToJSON( jsonObj );
}


//*****************************************************************************


const GeodeticDatum g_WGS84Datum( g_WGS84Ellipsoid );
const GeodeticDatum g_IAU1976Datum( g_IAU1976Ellipsoid );
const GeodeticDatum g_NorthAmerican1983Datum( g_GRS1980Ellipsoid );
const GeodeticDatum g_NorthAmerican1927Datum( g_Clarke1866Ellipsoid );
const GeodeticDatum g_SouthAmerican1969Datum( g_SouthAmerican1969Ellipsoid );
const GeodeticDatum g_European1950Datum( g_International1924Ellipsoid );
const GeodeticDatum g_TokyoDatum( g_Bessel1841Ellipsoid );
const GeodeticDatum g_ArcCapeDatum( g_Clarke1880ModEllipsoid );
const GeodeticDatum g_AdindanDatum( g_Clarke1880ModEllipsoid );
const GeodeticDatum g_Indian1938Datum( g_Everest1830Ellipsoid );
const GeodeticDatum g_Australian1966Datum( g_Australian1965Ellipsoid );
const GeodeticDatum g_NewZealand1949Datum( g_International1924Ellipsoid );
const GeodeticDatum g_Pulkovo1942Datum( g_Krassovski1942Ellipsoid );
const GeodeticDatum g_CantonAstro1966Datum( g_International1924Ellipsoid );
const GeodeticDatum g_CapeCanaveralDatum( g_Clarke1866Ellipsoid );
const GeodeticDatum g_LuzonPhilippines1911Datum( g_Clarke1866Ellipsoid );
const GeodeticDatum g_PalmerAstro1969Datum( g_International1924Ellipsoid );


//*****************************************************************************

}                                                      //namespace EpsilonDelta


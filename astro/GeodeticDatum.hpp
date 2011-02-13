#ifndef GEODETICDATUM_HPP
#define GEODETICDATUM_HPP
/*
  GeodeticDatum.hpp
  Copyright (C) 2007 David M. Anderson

  GeodeticDatum class, representing a specific regional geodetic datum or
  global reference system.
  NOTES:
  1. The Global Positioning System (GPS) uses the World Geodetic System 1984
     (WGS84) datum.
*/


#include "GeodeticEllipsoid.hpp"
#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class GeodeticDatum
{
public:
    GeodeticDatum( const GeodeticEllipsoid & ellipsoid );

    const GeodeticEllipsoid & Ellipsoid( ) const;

private:
    //Undefined to avoid compiler error:
    GeodeticDatum & operator=( const GeodeticDatum & rhs );

    const GeodeticEllipsoid & m_ellipsoid;
};

//.............................................................................

std::ostream & operator<<( std::ostream & out, const GeodeticDatum & datum );
std::string ToJSON( const GeodeticDatum & datum );
//!!!void FromJSON( const std::string & json, GeodeticDatum * pDatum );


//*****************************************************************************


extern const GeodeticDatum g_WGS84Datum;
extern const GeodeticDatum g_IAU1976Datum;
extern const GeodeticDatum g_NorthAmerican1983Datum;
extern const GeodeticDatum g_NorthAmerican1927Datum;
extern const GeodeticDatum g_SouthAmerican1969Datum;
extern const GeodeticDatum g_European1950Datum;
extern const GeodeticDatum g_TokyoDatum;
extern const GeodeticDatum g_ArcCapeDatum;
extern const GeodeticDatum g_AdindanDatum;
extern const GeodeticDatum g_Indian1938Datum;
extern const GeodeticDatum g_Australian1966Datum;
extern const GeodeticDatum g_NewZealand1949Datum;
extern const GeodeticDatum g_Pulkovo1942Datum;
extern const GeodeticDatum g_CantonAstro1966Datum;
extern const GeodeticDatum g_CapeCanaveralDatum;
extern const GeodeticDatum g_LuzonPhilippines1911Datum;
extern const GeodeticDatum g_PalmerAstro1969Datum;


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GEODETICDATUM_HPP

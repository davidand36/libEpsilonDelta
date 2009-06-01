#ifndef GEODETICELLIPSOID_HPP
#define GEODETICELLIPSOID_HPP
/*
  GeodeticEllipsoid.hpp
  Copyright (C) 2007 David M. Anderson

  GeodeticEllipsoid class, characterizing an ellipsoid (spheroid) used to
  approximate the geoid.
  NOTES:
  1. Equatorial radius (a.k.a. semi-major axis) is in meters.
*/


#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class GeodeticEllipsoid
{
public:
    GeodeticEllipsoid( );
    GeodeticEllipsoid( double equatorialRadius, double flattening );
    double EquatorialRadius( ) const;
    double Flattening( ) const;

private:
    double m_equatorialRadius;
    double m_flattening;
};

//.............................................................................

std::ostream & operator<<( std::ostream & out, const GeodeticEllipsoid & geod );
std::string ToJSON( const GeodeticEllipsoid & geod );
void FromJSON( const std::string & json, GeodeticEllipsoid * pGeod );


//=============================================================================


extern const GeodeticEllipsoid g_IERS1989Ellipsoid;
extern const GeodeticEllipsoid g_WGS84Ellipsoid;
extern const GeodeticEllipsoid g_MERIT1983Ellipsoid;
extern const GeodeticEllipsoid g_GRS1980Ellipsoid;
extern const GeodeticEllipsoid g_IAU1976Ellipsoid;
extern const GeodeticEllipsoid g_SouthAmerican1969Ellipsoid;
extern const GeodeticEllipsoid g_WGS72Ellipsoid;
extern const GeodeticEllipsoid g_GRS1967Ellipsoid;
extern const GeodeticEllipsoid g_WGS66Ellipsoid;
extern const GeodeticEllipsoid g_Australian1965Ellipsoid;
extern const GeodeticEllipsoid g_IAU1964Ellipsoid;
extern const GeodeticEllipsoid g_WGS60Ellipsoid;
extern const GeodeticEllipsoid g_Krassovski1942Ellipsoid;
extern const GeodeticEllipsoid g_International1924Ellipsoid;
extern const GeodeticEllipsoid g_Clarke1880ModEllipsoid;
extern const GeodeticEllipsoid g_Clarke1866Ellipsoid;
extern const GeodeticEllipsoid g_Bessel1841Ellipsoid;
extern const GeodeticEllipsoid g_Everest1830Ellipsoid;
extern const GeodeticEllipsoid g_Airy1830Ellipsoid;


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GEODETICELLIPSOID_HPP

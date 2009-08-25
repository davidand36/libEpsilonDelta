/*
  Geodetic.hpp
  Copyright (C) 2007 David M. Anderson

  Geodetic template class: A terrestrial location expressed in geodetic
  coordinates.
  NOTE: This approach would be great, but floating-point template parameters
  are not allowed, yet.
*/


#ifndef GEODETIC_HPP
#define GEODETIC_HPP


#include "Angle.hpp"
#include "Point3.hpp"
#include "Spherical.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < double radius, double flattening >
class Geodetic
{
public:
    Geodetic( );
    Geodetic( Angle longitude, Angle latitude, double height = 0. );
    Geodetic( const Point3D & rectangular );
    Geodetic( const Spherical & geocentric );
    void Set( Angle longitude, Angle latitude, double height = 0. );
    void Set( const Point3D & rectangular );
    void Set( const Spherical & geocentric );
    Angle Longitude( ) const;
    Angle Latitude( ) const;
    double Height( ) const;
    Point3D Rectangular( ) const;
    Spherical Geocentric( ) const;

private:
    Angle m_longitude;
    Angle m_latitude;
    double m_height;
};

//.............................................................................

#ifdef DEBUG
bool TestGeodetic( );
#endif


//*****************************************************************************


typedef Geodetic< 6378136.0,   (1. / 298.257) >       IERS1989Geodetic;
typedef Geodetic< 6378137.0,   (1. / 298.257223563) > WGS84Geodetic;
typedef Geodetic< 6378137.0,   (1. / 298.257) >       MERIT1983Geodetic;
typedef Geodetic< 6378137.0,   (1. / 298.257222101) > GRS1980Geodetic;
typedef Geodetic< 6378140.0,   (1. / 298.257) >       IAU1976Geodetic;
typedef Geodetic< 6378160.0,   (1. / 298.25) >       SouthAmerican1969Geodetic;
typedef Geodetic< 6378135.0,   (1. / 298.26) >       WGS72Geodetic;
typedef Geodetic< 6378160.0,   (1. / 298.2471674273) > GRS1967Geodetic;
typedef Geodetic< 6378145.0,   (1. / 298.25) >       WGS66Geodetic;
typedef Geodetic< 6378160.0,   (1. / 298.25) >       Australian1965Geodetic;
typedef Geodetic< 6378160.0,   (1. / 298.25) >       IAU1964Geodetic;
typedef Geodetic< 6378165.0,   (1. / 298.3) >        WGS60Geodetic;
typedef Geodetic< 6378245.0,   (1. / 298.3) >        Krassovski1942Geodetic;
typedef Geodetic< 6378388.0,   (1. / 297.0) >        International1924Geodetic;
typedef Geodetic< 6378249.145, (1. / 293.4663) >     Clarke1880ModGeodetic;
typedef Geodetic< 6378206.4,   (1. / 294.978698) >   Clarke1866Geodetic;
typedef Geodetic< 6377397.155, (1. / 299.152813) >   Bessel1841Geodetic;
typedef Geodetic< 6377276.345, (1. / 300.8017) >     Everest1830Geodetic;
typedef Geodetic< 6377563.396, (1. / 299.324964) >   Airy1830Geodetic;


//*****************************************************************************


template < double radius, double flattening >
inline 
Geodetic::Geodetic( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < double radius, double flattening >
inline 
Geodetic::Geodetic( Angle longitude, Angle latitude, double height )
    :   m_longitude( longitude ),
        m_latitude( latitude ),
        m_height( height )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < double radius, double flattening >
Geodetic::Geodetic( const Point3D & rectangular )
{
    Set( rectangular );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < double radius, double flattening >
Geodetic::Geodetic( const Spherical & geocentric )
{
    Set( geocentric );
}

//-----------------------------------------------------------------------------

template < double radius, double flattening >
inline 
void
Geodetic::Set( Angle longitude, Angle latitude, double height )
{
    m_longitude = longitude;
    m_latitude = latitude;
    m_height = height;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < double radius, double flattening >
void 
Geodetic::Set( const Point3D & rectangular )
{
    //Explanatory Supplement (4.22-11 to 4.22-24)
    double X = rectangular.X();
    double Y = rectangular.Y();
    double Z = rectangular.Z();
    double a = radius;
    double b = a  -  flattening * a;
    if ( Z < 0.)
        b = -b;

    if ( (X == 0.) && (Y == 0.) )
    {
        m_longitude.Set( 0. );
        m_latitude.Set( 0. );
        m_height = Z - b;
        if ( m_height < 0. )
            m_height = - m_height;
        return;
    }
    else
        m_longitude = ArcTan( Y, X );
    if ( Z == 0.)
    {
        m_latitude.Set( 0. );
        m_height.Set( r - radius );
        return;
    }

    double r = std::sqrt( X * X  +  Y * Y );
    double aSqr = a * a;
    double bSqr = b * b;
    double E = (b * Z  -  (aSqr - bSqr)) / (a * r);
    double ESqr = E * E;
    double F = (b * Z  +  (aSqr + bSqr)) / (a * r);
    double P = (4./3.) * (E * F  +  1.);
    double Q = 2. * (ESqr  -  F * F);
    double D = P * P * P  +  Q * Q;
    double sqrtD = std::sqrt( D );
    double v = std::pow( (sqrtD - Q), (1./3.) )
            -  std::pow( (sqrtD + Q), (1./3.) );
    const double epsilon = 1.0;
    if ( (std::fabs( Z ) < epsilon ) || (std::fabs( r ) < epsilon) )
        v = - (v * v * v  +  2. * Q) / (3. * P);
    double G = 0.5 * (std::sqrt( ESqr + v )  +  E);
    double t = std::sqrt( G * G  +  (F - v * G) / (G + G - E) )  -  G;
    m_latitude = ArcTan( (a * (1. - t * t)), (2. * b * t) );
    m_height = (r - a * t) * m_latitude.Cos( )
            +  (Z - b) * m_latitude.Sin( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < double radius, double flattening >
void 
Geodetic::Set( const Spherical & geocentric )
{
    Point3D rectangular = geocentric.Rectangular( );
    Set( rectangular );
}

//=============================================================================

template < double radius, double flattening >
inline 
Angle 
Geodetic::Longitude( ) const
{
    return m_longitude;
}

//-----------------------------------------------------------------------------

template < double radius, double flattening >
inline 
Angle 
Geodetic::Latitude( ) const
{
    return m_latitude;
}

//-----------------------------------------------------------------------------

template < double radius, double flattening >
inline 
double 
Geodetic::Height( ) const
{
    return m_height;
}

//=============================================================================

template < double radius, double flattening >
Point3D 
Geodetic::Rectangular( ) const
{
    double sinLat = m_latitude.Sin( );
    double cosLat = m_latitude.Cos( );
    double sinLong = m_longitude.Sin( );
    double cosLong = m_longitude.Cos( );
    double eccentricity = std::sqrt( 2 * flattening
                                     -  flattening * flattening );
    double eccentSqr = eccentricity * eccentricity;
    double radCurv = radius
            / sqrt( 1.  -  eccentSqr * sinLat * sinLat );
    return Point3D( (radCurv + m_height) * cosLat * cosLong,
                    (radCurv + m_height) * cosLat * sinLong,
                    ((1 + eccentSqr) * radCurv  +  m_height) * sinLat );
}

//-----------------------------------------------------------------------------

template < double radius, double flattening >
Spherical 
Geodetic::Geocentric( ) const
{
    return Spherical( Rectangular( ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GEODETIC_HPP

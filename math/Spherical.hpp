#ifndef SPHERICAL_HPP
#define SPHERICAL_HPP
/*
  Spherical.hpp
  Copyright (C) 2007 David M. Anderson

  Spherical class: representing (3-dimensional) spherical coordinates.
  NOTES:
  1. Separation() returns the angular separation, ignoring radial distance.
  2. Normalize() normalizes both angles to be between -pi and pi.
     NormalizeSemiPositive() normalizes the longitude to be between 0 and 2*pi,
     and the latitude to be between -pi and pi.
*/


#include "Angle.hpp"
#include "Point3.hpp"
#include "Assert.hpp"
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Spherical
{
public:
    Spherical( );
    Spherical( Angle longitude, Angle latitude, double distance = 1. );
    Spherical( const Point3D & rectangular );
    void Set( );
    void Set( Angle longitude, Angle latitude, double distance = 1. );
    void Set( const Point3D & rectangular );
    void Normalize( );
    void NormalizeSemiPositive( );
    Angle Longitude( ) const;
    Angle Latitude( ) const;
    double Distance( ) const;
    Point3D Rectangular( ) const;
#ifdef DEBUG
    static bool Test( );
#endif

private:
    Angle m_longitude;
    Angle m_latitude;
    double m_distance;
};

//.............................................................................

Angle Separation( const Spherical & sph0, const Spherical & sph1 );
std::ostream & operator<<( std::ostream & out, const Spherical & spherical );
std::string ToJSON( const Spherical & spherical );
void FromJSON( const std::string & json, Spherical * pSpherical );


//*****************************************************************************


inline 
void 
Spherical::Set( )
{
    m_longitude.Set( 0. );
    m_latitude.Set( 0. );
    m_distance = 0.;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Spherical::Set( Angle longitude, Angle latitude, double distance )
{
    m_longitude = longitude;
    m_latitude = latitude;
    m_distance = distance;
}

//=============================================================================

inline
Spherical::Spherical( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Spherical::Spherical( Angle longitude, Angle latitude, double distance )
{
    Set( longitude, latitude, distance );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Spherical::Spherical( const Point3D & rectangular )
{
    Set( rectangular );
}

//=============================================================================

inline 
void 
Spherical::Normalize( )
{
    m_longitude.Normalize( );
    m_latitude.Normalize( );
}

//-----------------------------------------------------------------------------

inline 
void 
Spherical::NormalizeSemiPositive( )
{
    m_longitude.NormalizePositive( );
    m_latitude.Normalize( );
}

//=============================================================================

inline 
Angle 
Spherical::Longitude( ) const
{
    return m_longitude;
}

//-----------------------------------------------------------------------------

inline 
Angle 
Spherical::Latitude( ) const
{
    return m_latitude;
}

//-----------------------------------------------------------------------------

inline 
double 
Spherical::Distance( ) const
{
    return m_distance;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SPHERICAL_HPP

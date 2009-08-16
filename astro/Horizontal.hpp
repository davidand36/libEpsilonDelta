#ifndef HORIZONTAL_HPP
#define HORIZONTAL_HPP
/*
  Horizontal.hpp
  Copyright (C) 2007 David M. Anderson

  Horizontal class: represents horizontal coordinates of a point.
  NOTES:
  1. Azimuth, often denoted by the letter A, is generally expressed in
     positive degrees (or degrees, minutes, and seconds; cf. the AngleDMS
     class). Although some authors, including Jean Meeus, measure it westward
     from the south, I accept the convention adopted by the authors of the
     Astronomical Almanac and others of measuring it eastward from the north.
     Altitude, often denoted by the letter a (but sometimes h, as in Meeus's
     "Astronomical Algorithms"), is generally expressed in degrees (or
     degrees, minutes, and seconds). It is positive above the horizon;
     negative, below.
  2. Zenith distance, often denoted by the letter z, is pi/2 - altitude, so
     it ranges from 0 to pi, inclusive.
*/


#include "Spherical.hpp"
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Horizontal
{
public:
    Horizontal( );
    Horizontal( Angle azimuth, Angle altitude, double distance = 1. );
    Horizontal( const Point3D & rectangular );
    void Set( Angle azimuth, Angle altitude, double distance = 1. );
    void Set( const Point3D & rectangular );
    void Normalize( );
    Angle Azimuth( ) const;
    Angle Altitude( ) const;
    Angle ZenithDistance( ) const;
    double Distance( ) const;
    Point3D Rectangular( ) const;

private:
    Spherical m_spherical;

    friend Angle Separation( const Horizontal & hor0, const Horizontal & hor1 );
    friend std::ostream & operator<<( std::ostream & out,
                                      const Horizontal & hor );
};

//.............................................................................

Angle Separation( const Horizontal & hor0, const Horizontal & hor1 );
std::ostream & operator<<( std::ostream & out, const Horizontal & hor );
std::string ToJSON( const Horizontal & hor );
void FromJSON( const std::string & json, Horizontal * pHor );


//*****************************************************************************


inline
void 
Horizontal::Normalize( )
{
    m_spherical.NormalizeSemiPositive( );
}

//=============================================================================

inline 
void 
Horizontal::Set( Angle azimuth, Angle altitude, double distance )
{
    m_spherical.Set( azimuth, altitude, distance );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Horizontal::Set( const Point3D & rectangular )
{
    m_spherical.Set( rectangular );
    Normalize( );
}

//-----------------------------------------------------------------------------

inline 
Horizontal::Horizontal( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Horizontal::Horizontal( Angle azimuth, Angle altitude, double distance )
    :    m_spherical( azimuth, altitude, distance )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Horizontal::Horizontal( const Point3D & rectangular )
    :    m_spherical( rectangular )
{
    Normalize( );
}

//=============================================================================

inline 
Angle 
Horizontal::Azimuth( ) const
{
    return m_spherical.Longitude();
}

//-----------------------------------------------------------------------------

inline 
Angle 
Horizontal::Altitude( ) const
{
    return m_spherical.Latitude();
}

//-----------------------------------------------------------------------------

inline 
Angle 
Horizontal::ZenithDistance( ) const
{
    return Angle( M_PI / 2  -  Altitude() );
}

//-----------------------------------------------------------------------------

inline 
double 
Horizontal::Distance( ) const 
{
    return m_spherical.Distance();
}

//=============================================================================

inline 
Point3D 
Horizontal::Rectangular( ) const
{
    return m_spherical.Rectangular();
}

//=============================================================================

inline 
Angle 
Separation( const Horizontal & hor0, const Horizontal & hor1 )
{
    return Separation( hor0.m_spherical, hor1.m_spherical );
}

//=============================================================================

inline
std::ostream & 
operator<<( std::ostream & out, const Horizontal & hor )
{
    return out << hor.m_spherical;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HORIZONTAL_HPP

#ifndef EQUATORIAL_HPP
#define EQUATORIAL_HPP
/*
  Equatorial.hpp
  Copyright (C) 2007 David M. Anderson

  Equatorial class: represents equatorial coordinates of a point.
  NOTES:
  1. Right ascension, often denoted by the letter alpha, is generally
     expressed in hours, minutes, and seconds. The AngleHMS class can be used
     to make the conversion. It ranges from 0h0m0.s to 23h59m59.9+s,
     eastward from the vernal equinox.
     Declination, often denoted by the letter delta, is generally expressed in 
     degrees (or degrees, minutes, and seconds; cf. the AngleDMS class). It is
     positive north of the celestial equator; negative, south.
*/


#include "Spherical.hpp"
#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Equatorial
{
public:
    Equatorial( );
    Equatorial( Angle rightAscension, Angle declination,
                double distance = 1. );
    Equatorial( const Point3D & rectangular );
    void Set( Angle rightAscension, Angle declination, double distance = 1. );
    void Set( const Point3D & rectangular );
    void Normalize( );
    Angle RightAscension( ) const;
    Angle Declination( ) const;
    double Distance( ) const;
    Point3D Rectangular( ) const;

private:
    Spherical m_spherical;

    friend Angle Separation( const Equatorial & equ0, const Equatorial & equ1 );
    friend std::ostream & operator<<( std::ostream & out,
                                      const Equatorial & equ );
};

//.............................................................................

Angle Separation( const Equatorial & equ0, const Equatorial & equ1 );
std::ostream & operator<<( std::ostream & out, const Equatorial & equ );
std::string ToJSON( const Equatorial & equ );
void FromJSON( const std::string & json, Equatorial * pEqu );


//*****************************************************************************


inline
void 
Equatorial::Normalize( )
{
    m_spherical.NormalizeSemiPositive( );
}

//=============================================================================

inline 
void 
Equatorial::Set( Angle rightAscension, Angle declination, double distance )
{
    m_spherical.Set( rightAscension, declination, distance );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Equatorial::Set( const Point3D & rectangular )
{
    m_spherical.Set( rectangular );
    Normalize( );
}

//-----------------------------------------------------------------------------

inline 
Equatorial::Equatorial( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Equatorial::Equatorial( Angle rightAscension, Angle declination,
                        double distance )
    :    m_spherical( rightAscension, declination, distance )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Equatorial::Equatorial( const Point3D & rectangular )
    :    m_spherical( rectangular )
{
    Normalize( );
}

//=============================================================================

inline 
Angle 
Equatorial::RightAscension( ) const
{
    return m_spherical.Longitude();
}

//-----------------------------------------------------------------------------

inline 
Angle 
Equatorial::Declination( ) const
{
    return m_spherical.Latitude();
}

//-----------------------------------------------------------------------------

inline 
double 
Equatorial::Distance( ) const 
{
    return m_spherical.Distance();
}

//=============================================================================

inline 
Point3D 
Equatorial::Rectangular( ) const
{
    return m_spherical.Rectangular();
}

//=============================================================================

inline 
Angle 
Separation( const Equatorial & equ0, const Equatorial & equ1 )
{
    return Separation( equ0.m_spherical, equ1.m_spherical );
}

//=============================================================================

inline
std::ostream & 
operator<<( std::ostream & out, const Equatorial & equ )
{
    return out << equ.m_spherical;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EQUATORIAL_HPP

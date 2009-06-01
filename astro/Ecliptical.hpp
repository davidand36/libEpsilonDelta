#ifndef ECLIPTICAL_HPP
#define ECLIPTICAL_HPP
/*
  Ecliptical.hpp
  Copyright (C) 2007 David M. Anderson

  Ecliptical class: represents ecliptical coordinates of a point.
  NOTES:
  1. Ecliptic longitude, often denoted by the letter lambda, is generally
     expressed in positive degrees (or degrees, minutes, and seconds; cf. the
     AngleDMS class), measured eastward from the vernal equinox.
     Ecliptic latitude, often denoted by the letter beta, is generally
     expressed in degrees (or degrees, minutes, and seconds). It is positive
     north of the ecliptic; negative, south.
*/


#include "Spherical.hpp"
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Ecliptical
{
public:
    Ecliptical( );
    Ecliptical( Angle longitude, Angle latitude, double distance = 1. );
    Ecliptical( const Vector3D & rectangular );
    void Set( Angle longitude, Angle latitude, double distance = 1. );
    void Set( const Vector3D & rectangular );
    void Normalize( );
    Angle Longitude( ) const;
    Angle Latitude( ) const;
    double Distance( ) const;
    Vector3D Rectangular( ) const;

private:
    Spherical m_spherical;

friend
    Angle Separation( const Ecliptical & ecl0, const Ecliptical & ecl1 );
    friend std::ostream & operator<<( std::ostream & out,
                                      const Ecliptical & ecl );
    friend std::string ToJSON( const Ecliptical & ecl );
    friend void FromJSON( const std::string & json, Ecliptical * pEcl );
};

//.............................................................................

Angle Separation( const Ecliptical & ecl0, const Ecliptical & ecl1 );
std::ostream & operator<<( std::ostream & out, const Ecliptical & ecl );
std::string ToJSON( const Ecliptical & ecl );
void FromJSON( const std::string & json, Ecliptical * pEcl );


//*****************************************************************************


inline
void 
Ecliptical::Normalize( )
{
    m_spherical.NormalizeSemiPositive( );
}

//=============================================================================

inline 
void 
Ecliptical::Set( Angle longitude, Angle latitude, double distance )
{
    m_spherical.Set( longitude, latitude, distance );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Ecliptical::Set( const Vector3D & rectangular )
{
    m_spherical.Set( rectangular );
    Normalize( );
}

//-----------------------------------------------------------------------------

inline 
Ecliptical::Ecliptical( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Ecliptical::Ecliptical( Angle longitude, Angle latitude, double distance )
    :    m_spherical( longitude, latitude, distance )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Ecliptical::Ecliptical( const Vector3D & rectangular )
    :    m_spherical( rectangular )
{
    Normalize( );
}

//=============================================================================

inline 
Angle 
Ecliptical::Longitude( ) const
{
    return m_spherical.Longitude();
}

//-----------------------------------------------------------------------------

inline 
Angle 
Ecliptical::Latitude( ) const
{
    return m_spherical.Latitude();
}

//-----------------------------------------------------------------------------

inline 
double 
Ecliptical::Distance( ) const 
{
    return m_spherical.Distance();
}

//=============================================================================

inline 
Vector3D 
Ecliptical::Rectangular( ) const
{
    return m_spherical.Rectangular();
}

//=============================================================================

inline 
Angle 
Separation( const Ecliptical & ecl0, const Ecliptical & ecl1 )
{
    return Separation( ecl0.m_spherical, ecl1.m_spherical );
}

//=============================================================================

inline
std::ostream & 
operator<<( std::ostream & out, const Ecliptical & ecl )
{
    return out << ecl.m_spherical;
}

//=============================================================================

inline
std::string 
ToJSON( const Ecliptical & ecl )
{
    return ToJSON( ecl.m_spherical );
}

//-----------------------------------------------------------------------------

inline
void 
FromJSON( const std::string & json, Ecliptical * pEcl )
{
    FromJSON( json, &(pEcl->m_spherical) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ECLIPTICAL_HPP

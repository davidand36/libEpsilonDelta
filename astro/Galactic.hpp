#ifndef GALACTIC_HPP
#define GALACTIC_HPP
/*
  Galactic.hpp
  Copyright (C) 2007 David M. Anderson

  Galactic class: represents galactic coordinates of a point.
  NOTES:
  1. The galactic coordinate system has been defined by the IAU such that the
     galactic north pole is at RA=12h49m, Dec=27.4deg, and the origin of
     galactic longitude is at the point in Sagittarius 33deg west along the
     galactic equator from its ascending node with the celestial equator, all
     for the epoch B1950.0.
  2. Ecliptic longitude, often denoted by the letter l, is generally
     expressed in positive degrees (or degrees, minutes, and seconds; cf. the
     AngleDMS class), measured eastward from the galactic center.
     Ecliptic latitude, often denoted by the letter b, is generally
     expressed in degrees (or degrees, minutes, and seconds). It is positive
     north of the galactic plane; negative, south.
*/


#include "Spherical.hpp"
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Galactic
{
public:
    Galactic( );
    Galactic( Angle longitude, Angle latitude, double distance = 1. );
    Galactic( const Point3D & rectangular );
    void Set( Angle longitude, Angle latitude, double distance = 1. );
    void Set( const Point3D & rectangular );
    void Normalize( );
    Angle Longitude( ) const;
    Angle Latitude( ) const;
    double Distance( ) const;
    Point3D Rectangular( ) const;

private:
    Spherical m_spherical;

friend
    Angle Separation( const Galactic & gal0, const Galactic & gal1 );
    friend std::ostream & operator<<( std::ostream & out,
                                      const Galactic & gal );
    friend std::string ToJSON( const Galactic & gal );
    friend void FromJSON( const std::string & json, Galactic * pGal );
};

//.............................................................................

Angle Separation( const Galactic & gal0, const Galactic & gal1 );
std::ostream & operator<<( std::ostream & out, const Galactic & gal );
std::string ToJSON( const Galactic & gal );
void FromJSON( const std::string & json, Galactic * pGal );


//*****************************************************************************


inline
void 
Galactic::Normalize( )
{
    m_spherical.NormalizeSemiPositive( );
}

//=============================================================================

inline 
void 
Galactic::Set( Angle longitude, Angle latitude, double distance )
{
    m_spherical.Set( longitude, latitude, distance );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Galactic::Set( const Point3D & rectangular )
{
    m_spherical.Set( rectangular );
    Normalize( );
}

//-----------------------------------------------------------------------------

inline 
Galactic::Galactic( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Galactic::Galactic( Angle longitude, Angle latitude, double distance )
    :    m_spherical( longitude, latitude, distance )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Galactic::Galactic( const Point3D & rectangular )
    :    m_spherical( rectangular )
{
    Normalize( );
}

//=============================================================================

inline 
Angle 
Galactic::Longitude( ) const
{
    return m_spherical.Longitude();
}

//-----------------------------------------------------------------------------

inline 
Angle 
Galactic::Latitude( ) const
{
    return m_spherical.Latitude();
}

//-----------------------------------------------------------------------------

inline 
double 
Galactic::Distance( ) const 
{
    return m_spherical.Distance();
}

//=============================================================================

inline 
Point3D 
Galactic::Rectangular( ) const
{
    return m_spherical.Rectangular();
}

//=============================================================================

inline 
Angle 
Separation( const Galactic & gal0, const Galactic & gal1 )
{
    return Separation( gal0.m_spherical, gal1.m_spherical );
}

//=============================================================================

inline
std::ostream & 
operator<<( std::ostream & out, const Galactic & gal )
{
    return out << gal.m_spherical;
}

//=============================================================================

inline
std::string 
ToJSON( const Galactic & gal )
{
    return ToJSON( gal.m_spherical );
}

//-----------------------------------------------------------------------------

inline
void 
FromJSON( const std::string & json, Galactic * pGal )
{
    FromJSON( json, &(pGal->m_spherical) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GALACTIC_HPP

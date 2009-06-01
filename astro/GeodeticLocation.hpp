#ifndef GEODETICLOCATION_HPP
#define GEODETICLOCATION_HPP
/*
  GeodeticLocation.hpp
  Copyright (C) 2007 David M. Anderson

  GeodeticLocation class: A terrestrial location expressed in geodetic
  coordinates.
*/


#include "GeodeticDatum.hpp"
#include "Angle.hpp"
#include "Vector3.hpp"
#include <string>
#include <iostream>
namespace EpsilonDelta
{
class Spherical;
}


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class GeodeticLocation
{
public:
    GeodeticLocation( );
    GeodeticLocation( Angle longitude, Angle latitude, double height = 0.,
                      const GeodeticDatum * pDatum = &g_WGS84Datum );
    GeodeticLocation( const Vector3D & rectangular,
                      const GeodeticDatum * pDatum = &g_WGS84Datum );
    GeodeticLocation( const Spherical & geocentric,
                      const GeodeticDatum * pDatum = &g_WGS84Datum );
    void Set( Angle longitude, Angle latitude, double height = 0.,
              const GeodeticDatum * pDatum = &g_WGS84Datum );
    void Set( const Vector3D & rectangular,
              const GeodeticDatum * pDatum = &g_WGS84Datum );
    void Set( const Spherical & geocentric,
              const GeodeticDatum * pDatum = &g_WGS84Datum );
    Angle Longitude( ) const;
    Angle Latitude( ) const;
    double Height( ) const;
    Vector3D Rectangular( ) const;
    Spherical Geocentric( ) const;
    const GeodeticDatum * Datum( ) const;

private:
    Angle m_longitude;
    Angle m_latitude;
    double m_height;
    const GeodeticDatum * m_pDatum;
};

//.............................................................................


std::ostream & operator<<( std::ostream & out, const GeodeticLocation & loc );
std::string ToJSON( const GeodeticLocation & loc );
void FromJSON( const std::string & json, GeodeticLocation * pLoc );
#ifdef DEBUG
bool TestGeodeticLocation( );
#endif


//*****************************************************************************


inline 
GeodeticLocation::GeodeticLocation( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
GeodeticLocation::GeodeticLocation( Angle longitude, Angle latitude,
                                    double height,
                                    const GeodeticDatum * pDatum )
    :   m_longitude( longitude ),
        m_latitude( latitude ),
        m_height( height ),
        m_pDatum( pDatum )
{
}

//-----------------------------------------------------------------------------

inline 
void 
GeodeticLocation::Set( Angle longitude, Angle latitude, double height,
                       const GeodeticDatum * pDatum )
{
    m_longitude = longitude;
    m_latitude = latitude;
    m_height = height;
    m_pDatum = pDatum;
}

//=============================================================================

inline 
Angle 
GeodeticLocation::Longitude( ) const
{
    return m_longitude;
}

//-----------------------------------------------------------------------------

inline 
Angle 
GeodeticLocation::Latitude( ) const
{
    return m_latitude;
}

//-----------------------------------------------------------------------------

inline 
double 
GeodeticLocation::Height( ) const
{
    return m_height;
}

//-----------------------------------------------------------------------------

inline 
const GeodeticDatum * 
GeodeticLocation::Datum( ) const
{
    return m_pDatum;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GEODETICLOCATION_HPP

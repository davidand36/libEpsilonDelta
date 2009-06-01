/*
  GeodeticEllipsoid.cpp
  Copyright (C) 2007 David M. Anderson

  GeodeticEllipsoid class, characterizing an ellipsoid (spheroid) used to
  approximate the geoid.
*/


#include "GeodeticEllipsoid.hpp"
#include "AstroConst.hpp"
#include "JSON.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


GeodeticEllipsoid::GeodeticEllipsoid( )
    :   m_equatorialRadius( AstroConst::EarthRadius() ),
        m_flattening( AstroConst::EarthFlattening() )
{
}

//.............................................................................

GeodeticEllipsoid::GeodeticEllipsoid( double equatorialRadius,
                                      double flattening )
    :   m_equatorialRadius( equatorialRadius ),
        m_flattening( flattening )
{
}

//=============================================================================

double 
GeodeticEllipsoid::EquatorialRadius( ) const
{
    return m_equatorialRadius;
}

//-----------------------------------------------------------------------------

double 
GeodeticEllipsoid::Flattening( ) const
{
    return m_flattening;
}

//=============================================================================

ostream & 
operator<<( ostream & out, const GeodeticEllipsoid & geod )
{
    return out << "( " << geod.EquatorialRadius() << ", "
               << geod.Flattening() << " )";
}

//=============================================================================

string 
ToJSON( const GeodeticEllipsoid & geod )
{
    JSONObject jsonObj;
    jsonObj[ "radius" ] = ToJSON( geod.EquatorialRadius() );
    jsonObj[ "flattening" ] = ToJSON( geod.Flattening() );
    return ToJSON( jsonObj );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, GeodeticEllipsoid * pGeod )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    double radius, flattening;
    FromJSON( jsonObj[ "radius" ], &radius );
    FromJSON( jsonObj[ "flattening" ], &flattening );
    *pGeod = GeodeticEllipsoid( radius, flattening );
}


//*****************************************************************************


const GeodeticEllipsoid g_IERS1989Ellipsoid( 6378136.0, (1./298.257) );
const GeodeticEllipsoid g_WGS84Ellipsoid( 6378137.0, (1./298.257223563) );
const GeodeticEllipsoid g_MERIT1983Ellipsoid( 6378137.0, (1./298.257) );
const GeodeticEllipsoid g_GRS1980Ellipsoid( 6378137.0, (1./298.257222101) );
const GeodeticEllipsoid g_IAU1976Ellipsoid( 6378140.0, (1./298.257) );
const GeodeticEllipsoid g_SouthAmerican1969Ellipsoid( 6378160.0, (1./298.25) );
const GeodeticEllipsoid g_WGS72Ellipsoid( 6378135.0, (1./298.26) );
const GeodeticEllipsoid g_GRS1967Ellipsoid( 6378160.0, (1./298.2471674273) );
const GeodeticEllipsoid g_WGS66Ellipsoid( 6378145.0, (1./298.25) );
const GeodeticEllipsoid g_Australian1965Ellipsoid( 6378160.0, (1./298.25) );
const GeodeticEllipsoid g_IAU1964Ellipsoid( 6378160.0, (1./298.25) );
const GeodeticEllipsoid g_WGS60Ellipsoid( 6378165.0, (1./298.3) );
const GeodeticEllipsoid g_Krassovski1942Ellipsoid( 6378245.0, (1./298.3) );
const GeodeticEllipsoid g_International1924Ellipsoid( 6378388.0, (1./297.0) );
const GeodeticEllipsoid g_Clarke1880ModEllipsoid( 6378249.145, (1./293.4663) );
const GeodeticEllipsoid g_Clarke1866Ellipsoid( 6378206.4, (1./294.978698) );
const GeodeticEllipsoid g_Bessel1841Ellipsoid( 6377397.155, (1./299.152813) );
const GeodeticEllipsoid g_Everest1830Ellipsoid( 6377276.345, (1./300.8017) );
const GeodeticEllipsoid g_Airy1830Ellipsoid( 6377563.396, (1./299.324964) );


//*****************************************************************************

}                                                      //namespace EpsilonDelta

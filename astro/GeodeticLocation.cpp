/*
  GeodeticLocation.cpp
  Copyright (C) 2007 David M. Anderson

  GeodeticLocation class: A terrestrial location expressed in geodetic
  coordinates.
*/


#include "GeodeticLocation.hpp"
#include "Spherical.hpp"
#include "JSON.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "AngleDMS.hpp"
#include <sstream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


GeodeticLocation::GeodeticLocation( const Point3D & rectangular,
                                    const GeodeticDatum * pDatum )
{
    Set( rectangular, pDatum );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

GeodeticLocation::GeodeticLocation( const Spherical & geocentric,
                                    const GeodeticDatum * pDatum )
{
    Set( geocentric, pDatum );
}

//-----------------------------------------------------------------------------

void 
GeodeticLocation::Set( const Point3D & rectangular,
                       const GeodeticDatum * pDatum )
{
    m_pDatum = pDatum;
    double radius = m_pDatum->Ellipsoid().EquatorialRadius();
    double flattening = m_pDatum->Ellipsoid().Flattening();
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

    double r = std::sqrt( X * X  +  Y * Y );

    if ( Z == 0.)
    {
        m_latitude.Set( 0. );
        m_height = r - radius;
        return;
    }

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

void 
GeodeticLocation::Set( const Spherical & geocentric,
                       const GeodeticDatum * pDatum )
{
    Point3D rectangular = geocentric.Rectangular( );
    Set( rectangular, pDatum );
}

//=============================================================================

Point3D 
GeodeticLocation::Rectangular( ) const
{
    double radius = m_pDatum->Ellipsoid().EquatorialRadius();
    double flattening = m_pDatum->Ellipsoid().Flattening();
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
                    ((1 - eccentSqr) * radCurv  +  m_height) * sinLat );
}

//-----------------------------------------------------------------------------

Spherical 
GeodeticLocation::Geocentric( ) const
{
    return Spherical( Rectangular( ) );
}

//=============================================================================

ostream & 
operator<<( ostream & out, const GeodeticLocation & loc )
{
    return out << "( " << loc.Longitude() << ", " << loc.Latitude()
               << ", " << loc.Height() << " )";
}

//=============================================================================

string 
ToJSON( const GeodeticLocation & loc )
{
    JSONObject jsonObj;
    jsonObj[ "longitude" ] = ToJSON( loc.Longitude() );
    jsonObj[ "latitude" ] = ToJSON( loc.Latitude() );
    jsonObj[ "height" ] = ToJSON( loc.Height() );
    //Not serializing Datum.
    return ToJSON( jsonObj );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, GeodeticLocation * pLoc )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    Angle lng, lat;
    double height;
    FromJSON( jsonObj[ "longitude" ], &lng );
    FromJSON( jsonObj[ "latitude" ], &lat );
    FromJSON( jsonObj[ "height" ], &height );
    //Not serializing Datum.
    pLoc->Set( lng, lat, height );
}

//=============================================================================

#ifdef DEBUG

bool 
TestGeodeticLocation( )
{
    bool ok = true;
    cout << "Testing GeodeticLocation" << endl;

    Angle lng( 0 );
    int d = 45;
    int m = 5;
    double s = 46.36;
    Angle lat( AngleDMS( d, m, s ) );
    double h = 0.;
    cout << "GeodeticLocation(" << lng.Radians() << ", AngleDMS(" << d
         << ", " << m << ", " << s << "), " << h << ")" << endl;
    GeodeticLocation geo45( lng, lat, h, &g_IAU1976Datum );
    TESTCHECK( geo45.Longitude().Radians(), lng.Radians(), &ok );
    TESTCHECKF( geo45.Latitude().Radians(), lat.Radians(), &ok );
    TESTCHECK( geo45.Height(), h, &ok );
    Spherical geocentric = geo45.Geocentric();
    TESTCHECK( geocentric.Longitude().Radians(), lng.Radians(), &ok );
    TESTCHECKF( geocentric.Latitude().Radians(),
               Angle( AngleDMS( 44, 54, 13.64 ) ).Radians(), &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << geo45;
    TESTCHECK( ost.str(), string( "( 0°, 45.0962°, 0 )" ), &ok );
    TESTCHECK( ToJSON( geo45 ),
               string( "{\n"
                       "\"height\": +0.00000000000000e+000,\n"
                       "\"latitude\": +7.87077364063339e-001,\n"
                       "\"longitude\": +0.00000000000000e+000\n"
                       "}"),
               &ok );
    GeodeticLocation geo2;
    cout << "FromJSON( ToJSON( geo45 ), &geo2 )" << endl;
    FromJSON(  ToJSON( geo45 ), &geo2 );
    TESTCHECK( geo2.Longitude().Radians(), 0., &ok );
    TESTCHECK( geo2.Latitude().Radians(), 0.787077364063339, &ok );
    TESTCHECK( geo2.Height(), 0., &ok );

    if ( ok )
        cout << "GeodeticLocation PASSED." << endl << endl;
    else
        cout << "GeodeticLocation FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

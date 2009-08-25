/*
  Spherical.cpp
  Copyright (C) 2007 David M. Anderson

  Spherical class: representing (3-dimensional) spherical coordinates.
*/


#include "Spherical.hpp"
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "AngleDMS.hpp"
#include "AngleHMS.hpp"
#include <sstream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
Spherical::Set( const Point3D & rectangular )
{
    if ( (rectangular.X() == 0.) && (rectangular.Y() == 0.) )
        m_longitude.Set( 0. );
    else
        m_longitude = ArcTan( rectangular.Y(), rectangular.X() );
    double x2y2 = rectangular.X() * rectangular.X()
        +  rectangular.Y() * rectangular.Y();
    double r = sqrt( x2y2 );
    if ( (r == 0.) && (rectangular.Z() == 0.) )
        m_latitude.Set( 0. );
    else
        m_latitude = ArcTan( rectangular.Z(), r );
    m_distance = sqrt( x2y2  +  rectangular.Z() * rectangular.Z() );
}

//=============================================================================

Point3D 
Spherical::Rectangular( ) const
{
    return Point3D( m_longitude.Cos() * m_latitude.Cos() * m_distance,
                    m_longitude.Sin() * m_latitude.Cos() * m_distance,
                    m_latitude.Sin() * m_distance );
}

//=============================================================================

Angle 
Separation( const Spherical & sph0, const Spherical & sph1 )
{
    Angle deltaLong = sph0.Longitude() - sph1.Longitude();
    Angle deltaLat = sph0.Latitude() - sph1.Latitude();
    return ArcHav( deltaLat.Hav()
        +  deltaLong.Hav() * sph0.Latitude().Cos() * sph1.Latitude().Cos() );
}

//=============================================================================

ostream & 
operator<<( ostream & out, const Spherical & spherical )
{
    return out << "( " << spherical.Longitude() << ", " << spherical.Latitude()
               << ", " << spherical.Distance() << " )";
}

//=============================================================================

string 
ToJSON( const Spherical & spherical )
{
    JSONObject jsonObj;
    jsonObj[ "longitude" ] = ToJSON( spherical.Longitude() );
    jsonObj[ "latitude" ] = ToJSON( spherical.Latitude() );
    jsonObj[ "distance" ] = ToJSON( spherical.Distance() );
    return ToJSON( jsonObj );
}

//.............................................................................

void 
FromJSON( const std::string & json, Spherical * pSpherical )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    Angle longitude, latitude;
    double distance;
    FromJSON( jsonObj[ "longitude" ], &longitude );
    FromJSON( jsonObj[ "latitude" ], &latitude );
    FromJSON( jsonObj[ "distance" ], &distance );
    pSpherical->Set( longitude, latitude, distance );
}

//=============================================================================

#ifdef DEBUG

bool 
Spherical::Test( )
{
    bool ok = true;
    cout << "Testing Spherical" << endl;

    cout << "Spherical( ) [default constructor]" << endl;
    Spherical spherical0;
    cout << "Set( )" << endl;
    spherical0.Set( );
    TESTCHECK( spherical0.Longitude().Radians(), 0., &ok );
    TESTCHECK( spherical0.Latitude().Radians(), 0., &ok );
    TESTCHECK( spherical0.Distance(), 0., &ok );
    Point3D rect0 = spherical0.Rectangular();
    TESTCHECK( rect0.X(), 0., &ok );
    TESTCHECK( rect0.Y(), 0., &ok );
    TESTCHECK( rect0.Z(), 0., &ok );
    double lng = - M_PI / 2;
    Angle longitude( lng );
    double lat = M_PI / 4;
    Angle latitude( lat );
    double dist = sqrt( 2. );
    cout << "Spherical( Angle(" << lng << "), Angle(" << lng << "), " << dist << " ) [longitude, latitude, distance constructor]" << endl;
    Spherical spherical1( longitude, latitude, dist );
    TESTCHECK( spherical1.Longitude().Radians(), lng, &ok );
    TESTCHECK( spherical1.Latitude().Radians(), lat, &ok );
    TESTCHECK( spherical1.Distance(), dist, &ok );
    Point3D rect = spherical1.Rectangular();
    TESTCHECKF( rect.X(), 0., &ok );
    TESTCHECKF( rect.Y(), -1., &ok );
    TESTCHECKF( rect.Z(), 1., &ok );
    lng = 3 * M_PI / 4;
    lat = 0.;
    dist = 4.;
    longitude.Set( lng );
    latitude.Set( lat );
    cout << "Set( Angle(" << lng << "), Angle(" << lng << "),  " << dist << " )" << endl;
    spherical1.Set( longitude, latitude, dist );
    TESTCHECK( spherical1.Longitude().Radians(), lng, &ok );
    TESTCHECK( spherical1.Latitude().Radians(), lat, &ok );
    TESTCHECK( spherical1.Distance(), dist, &ok );
    rect = spherical1.Rectangular();
    TESTCHECKF( rect.X(), - sqrt( 8. ), &ok );
    TESTCHECKF( rect.Y(), sqrt( 8. ), &ok );
    TESTCHECKF( rect.Z(), 0., &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << spherical1;
    TESTCHECK( ost.str(), string( "( 135°, 0°, 4 )" ), &ok );
    double x = 3.;
    double y = 4.;
    double z = 0.;
    rect.Set( x, y, z );
    cout << "Spherical( Point3D(" << x << ", " << y <<  ", " << z << ") ) [rectangular constructor]" << endl;
    Spherical spherical2( rect );
    TESTCHECKF( spherical2.Longitude().Radians(), 0.927295218, &ok );
    TESTCHECKF( spherical2.Latitude().Radians(), 0., &ok );
    TESTCHECKF( spherical2.Distance(), 5., &ok );
    rect = spherical2.Rectangular();
    TESTCHECKF( rect.X(), x, &ok );
    TESTCHECKF( rect.Y(), y, &ok );
    TESTCHECKF( rect.Z(), z, &ok );
    x = 12.;
    y = 0.;
    z = -5.;
    rect.Set( x, y, z );
    cout << "Set( Point3D(" << x << ", " << y <<  ", " << z << ") )" << endl;
    spherical2.Set( rect );
    TESTCHECKF( spherical2.Longitude().Radians(), 0., &ok );
    TESTCHECKF( spherical2.Latitude().Radians(), -0.39479112, &ok );
    TESTCHECKF( spherical2.Distance(), 13., &ok );
    rect = spherical2.Rectangular();
    TESTCHECKF( rect.X(), x, &ok );
    TESTCHECKF( rect.Y(), y, &ok );
    TESTCHECKF( rect.Z(), z, &ok );
    int h = 14;
    int m = 15;
    double s = 39.7;
    cout << "ra = AngleHMS( " << h << ", " << m << ", " << s << " )" << endl;
    const Angle raArcturus( AngleHMS( h, m, s ) );
    int d = 19;
    m = 10;
    s = 57.;
    cout << "dec = AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    const Angle decArcturus( AngleDMS( d, m, s ) );
    cout << "Arcturus: ( ra, dec, 1. )" << endl;
    const Spherical Arcturus( raArcturus, decArcturus );
    h = 13;
    m = 25;
    s = 11.6;
    cout << "ra = AngleHMS( " << h << ", " << m << ", " << s << " )" << endl;
    const Angle raSpica( AngleHMS( h, m, s ) );
    d = -11;
    m = 9;
    s = 41.;
    cout << "dec = AngleDMS( " << d << ", " << m << ", " << s << " )" << endl;
    const Angle decSpica( AngleDMS( d, m, s ) );
    cout << "Spica: ( ra, dec, 1. )" << endl;
    const Spherical Spica( raSpica, decSpica );
    cout << "Separation( Arcturus, Spica )" << endl;
    Angle sep = Separation( Arcturus, Spica );
    TESTCHECKF( sep.Degrees(), 32.7930103, &ok );
    lng = -1.5;
    lat = 0.75;
    dist = 40.;
    longitude.Set( lng );
    latitude.Set( lat );
    cout << "Set( Angle(" << lng << "), Angle(" << lat << "), "
         << dist << " )" << endl;
    spherical1.Set( longitude, latitude, dist );
    TESTCHECK( ToJSON( spherical1 ),
               string( "{\n"
                       "\"distance\": +4.00000000000000e+001,\n"
                       "\"latitude\": +7.50000000000000e-001,\n"
                       "\"longitude\": -1.50000000000000e+000\n"
                       "}"),
               &ok );
    cout << "FromJSON( ToJSON( spherical1 ), &spherical2 )" << endl;
    FromJSON(  ToJSON( spherical1 ), &spherical2 );
    TESTCHECK( spherical2.Longitude().Radians(), lng, &ok );
    TESTCHECK( spherical2.Latitude().Radians(), lat, &ok );
    TESTCHECK( spherical2.Distance(), dist, &ok );

    if ( ok )
        cout << "Spherical PASSED." << endl << endl;
    else
        cout << "Spherical FAILED." << endl << endl;
    return ok;
}

#endif


//=============================================================================

}                                                      //namespace EpsilonDelta

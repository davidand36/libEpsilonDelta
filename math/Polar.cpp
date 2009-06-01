/*
  Polar.cpp
  Copyright (C) 2007 David M. Anderson

  Polar class: representing (2-dimensional) polar coordinates.
*/


#include "Polar.hpp"
#include "JSON.hpp"
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
Polar::Set( Vector2D rectangular )
{
    if ( (rectangular.X() == 0.) && (rectangular.Y() == 0.) )
    {
        m_bearing.Set( 0. );
        m_distance = 0.;
    }
    else
    {
        m_bearing = ArcTan( rectangular.Y(), rectangular.X() );
        m_distance = sqrt( rectangular.X() * rectangular.X()
                           +  rectangular.Y() * rectangular.Y() );
    }
}

//=============================================================================

Vector2D 
Polar::Rectangular( ) const
{
    return Vector2D( m_bearing.Cos() * m_distance, m_bearing.Sin() * m_distance );
}

//=============================================================================

ostream & 
operator<<( ostream & out, const Polar & polar )
{
    return out << "( " << polar.Bearing() << ", " << polar.Distance() << " )";
}

//=============================================================================

string 
ToJSON( const Polar & polar )
{
    JSONObject jsonObj;
    jsonObj[ "bearing" ] = ToJSON( polar.Bearing() );
    jsonObj[ "distance" ] = ToJSON( polar.Distance() );
    return ToJSON( jsonObj );
}

//.............................................................................

void 
FromJSON( const std::string & json, Polar * pPolar )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    Angle bearing;
    double distance;
    FromJSON( jsonObj[ "bearing" ], &bearing );
    FromJSON( jsonObj[ "distance" ], &distance );
    pPolar->Set( bearing, distance );
}

//=============================================================================

#ifdef DEBUG

bool 
Polar::Test( )
{
    bool ok = true;
    cout << "Testing Polar" << endl;

    cout << "Polar( ) [default constructor]" << endl;
    Polar polar0;
    cout << "Set( )" << endl;
    polar0.Set( );
    TESTCHECK( polar0.Bearing().Radians(), 0., &ok );
    TESTCHECK( polar0.Distance(), 0., &ok );
    Vector2D rect0 = polar0.Rectangular();
    TESTCHECK( rect0.X(), 0., &ok );
    TESTCHECK( rect0.Y(), 0., &ok );
    double rads = - M_PI / 2;
    Angle bearing( rads );
    double dist = 2.;
    cout << "Polar( Angle(" << rads << "), " << dist << " ) [bearing, distance constructor]" << endl;
    Polar polar1( bearing, dist );
    TESTCHECK( polar1.Bearing().Radians(), rads, &ok );
    TESTCHECK( polar1.Distance(), dist, &ok );
    Vector2D rect = polar1.Rectangular();
    TESTCHECKF( rect.X(), 0., &ok );
    TESTCHECKF( rect.Y(), -2., &ok );
    rads = 3 * M_PI / 4;
    dist = 4.;
    bearing.Set( rads );
    cout << "Set( Angle(" << rads << "), " << dist << " )" << endl;
    polar1.Set( bearing, dist );
    TESTCHECK( polar1.Bearing().Radians(), rads, &ok );
    TESTCHECK( polar1.Distance(), dist, &ok );
    rect = polar1.Rectangular();
    TESTCHECKF( rect.X(), - sqrt( 8. ), &ok );
    TESTCHECKF( rect.Y(), sqrt( 8. ), &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << polar1;
    TESTCHECK( ost.str(), string( "( 135°, 4 )" ), &ok );
    double x = 3.;
    double y = 4.;
    rect.Set( x, y );
    cout << "Polar( Vector2D(" << x << ", " << y << ") ) [rectangular constructor]" << endl;
    Polar polar2( rect );
    TESTCHECKF( polar2.Bearing().Radians(), 0.927295218, &ok );
    TESTCHECKF( polar2.Distance(), 5., &ok );
    rect = polar2.Rectangular();
    TESTCHECKF( rect.X(), x, &ok );
    TESTCHECKF( rect.Y(), y, &ok );
    x = 12.;
    y = 5.;
    rect.Set( x, y );
    cout << "Set( Vector2D(" << x << ", " << y << ") )" << endl;
    polar2.Set( rect );
    TESTCHECKF( polar2.Bearing().Radians(), 0.39479112, &ok );
    TESTCHECKF( polar2.Distance(), 13., &ok );
    rect = polar2.Rectangular();
    TESTCHECKF( rect.X(), x, &ok );
    TESTCHECKF( rect.Y(), y, &ok );
    rads = 3.;
    dist = 4.;
    bearing.Set( rads );
    cout << "Set( Angle(" << rads << "), " << dist << " )" << endl;
    polar1.Set( bearing, dist );
    TESTCHECK( ToJSON( polar1 ),
               string( "{\n"
                       "\"bearing\": +3.00000000000000e+000,\n"
                       "\"distance\": +4.00000000000000e+000\n"
                       "}"),
               &ok );
    cout << "FromJSON( ToJSON( polar1 ), &polar2 )" << endl;
    FromJSON( ToJSON( polar1 ), &polar2 );
    TESTCHECK( polar2.Bearing().Radians(), rads, &ok );
    TESTCHECK( polar2.Distance(), dist, &ok );

    if ( ok )
        cout << "Polar PASSED." << endl << endl;
    else
        cout << "Polar FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

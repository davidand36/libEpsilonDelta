/*
  AngleDMS.cpp
  Copyright (C) 2007 David M. Anderson

  AngleDMS class: represents an angle in degrees, minutes, and seconds.
*/


#include "AngleDMS.hpp"
#include "Angle.hpp"
#include "JSON.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
#include <iomanip>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


AngleDMS::AngleDMS( int degrees, int minutes, double seconds )

{
    Set( degrees, minutes, seconds );
}

//-----------------------------------------------------------------------------

AngleDMS::AngleDMS( double degrees )
{
    Set( degrees );
}

//-----------------------------------------------------------------------------

AngleDMS::AngleDMS( const Angle & angle )
{
    Set( angle );
}

//=============================================================================

void 
AngleDMS::Set( int degrees, int minutes, double seconds )
{
    m_positive = ( (degrees > 0)
                   || ((degrees == 0) && (minutes > 0))
                   || ((degrees == 0) && (minutes == 0) && (seconds >= 0.)) );
    m_degrees = abs( degrees );
    m_minutes = abs( minutes );
    m_seconds = abs( seconds );
    Assert( minutes < 60 );
    Assert( seconds < 60. );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void
AngleDMS::Set( double degrees )
{
    m_positive = (degrees >= 0.);
    if ( ! m_positive )
        degrees = -degrees;
    double w;
    double f = modf( degrees, &w );
    m_degrees = static_cast<int>( w );
    f = modf( 60. * f, &w );
    m_minutes = static_cast<int>( w );
    m_seconds = 60. * f;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void
AngleDMS::Set( const Angle & angle )
{
    Set( angle.Degrees( ) );
}

//=============================================================================

ostream & 
operator<<( ostream & out, const AngleDMS & angle )
{
    out.setf( ios_base::fixed, ios_base::floatfield );
    return out << (angle.Positive() ? "" : "-") << angle.Degrees() << "°"
               << setw( 2 ) << setfill( '0' ) << angle.Minutes() << "\'"
               << setw( 6 ) << setprecision( 3 ) << setfill( '0' )
               << angle.Seconds() << "\"";
    out.setf( (ios_base::fmtflags)0, ios_base::floatfield );
}

//=============================================================================

std::string 
ToJSON( const AngleDMS & angle )
{
    JSONObject jsonObj;
    jsonObj[ "positive" ] = ToJSON( angle.Positive() );
    jsonObj[ "degrees" ] = ToJSON( angle.Degrees() );
    jsonObj[ "minutes" ] = ToJSON( angle.Minutes() );
    jsonObj[ "seconds" ] = ToJSON( angle.Seconds() );
    return ToJSON( jsonObj );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, AngleDMS * pAngle )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    bool pos;
    int d, m;
    double s;
    FromJSON( jsonObj[ "positive" ], &pos );
    FromJSON( jsonObj[ "degrees" ], &d );
    FromJSON( jsonObj[ "minutes" ], &m );
    FromJSON( jsonObj[ "seconds" ], &s );
    if ( ! pos )
        d = -d;
    pAngle->Set( d, m, s );
}

//=============================================================================

#ifdef DEBUG

bool 
AngleDMS::Test( )
{
    bool ok = true;
    cout << "Testing AngleDMS" << endl;

    cout << "AngleDMS( ) [default constructor]" << endl;
    AngleDMS angleDMS0;
    cout << "Set( )" << endl;
    angleDMS0.Set( );
    TESTCHECK( angleDMS0.Positive(), true, &ok );
    TESTCHECK( angleDMS0.Degrees(), 0, &ok );
    TESTCHECK( angleDMS0.Minutes(), 0, &ok );
    TESTCHECK( angleDMS0.Seconds(), 0., &ok );
    TESTCHECK( angleDMS0.TotalDegrees(), 0., &ok );
    int d = -60;
    int m = 49;
    double s = 48.;
    cout << "AngleDMS( " << d << ", " << m << ", " << s << " ) [d,m,s constructor]" << endl;
    AngleDMS angleDMS1( d, m, s );
    TESTCHECK( angleDMS1.Positive(), false, &ok );
    TESTCHECK( angleDMS1.Degrees(), -d, &ok );
    TESTCHECK( angleDMS1.Minutes(), m, &ok );
    TESTCHECK( angleDMS1.Seconds(), s, &ok );
    TESTCHECKF( angleDMS1.TotalDegrees(), -60.83, &ok );
    double dd = -29.30;
    cout << "AngleDMS( " << dd << " ) [degrees constructor]" << endl;
    AngleDMS angleDMS2( dd );
    TESTCHECK( angleDMS2.Positive(), false, &ok );
    TESTCHECK( angleDMS2.Degrees(), 29, &ok );
    TESTCHECK( angleDMS2.Minutes(), 18, &ok );
    TESTCHECKF( angleDMS2.Seconds(), 0., &ok );
    TESTCHECKF( angleDMS2.TotalDegrees(), dd, &ok );
    double r = 4.;
    Angle angle4( r );
    cout << "AngleDMS( Angle( " << r << " ) ) [Angle constructor]" << endl;
    AngleDMS angleDMS3( angle4 );
    TESTCHECK( angleDMS3.Positive(), true, &ok );
    TESTCHECK( angleDMS3.Degrees(), 229, &ok );
    TESTCHECK( angleDMS3.Minutes(), 10, &ok );
    TESTCHECKF( angleDMS3.Seconds(), 59.2249884, &ok );
    cout << "Angle( angleDMS3 )" << endl;
    Angle angle( angleDMS3 );
    TESTCHECKF( angle.Radians(), r, &ok );
    TESTCHECKF( angleDMS3.TotalDegrees(), angle.Degrees(), &ok );

    ostringstream ost;
    ost.precision( 2 );
    cout << "operator<<" << endl;
    ost << angleDMS2;
    TESTCHECK( ost.str(), string( "-29°18\'00.000\"" ), &ok );

    TESTCHECK( ToJSON( angleDMS1 ),
               string( "{\n"
                       "\"degrees\": 60,\n"
                       "\"minutes\": 49,\n"
                       "\"positive\": false,\n"
                       "\"seconds\": +4.80000000000000e+001\n"
                       "}" ),
               &ok );
    cout << "FromJSON( ToJSON( angleDMS1 ), &angleDMS3 )" << endl;
    FromJSON( ToJSON( angleDMS1 ), &angleDMS3 );
    TESTCHECK( angleDMS3.Positive(), false, &ok );
    TESTCHECK( angleDMS3.Degrees(), 60, &ok );
    TESTCHECK( angleDMS3.Minutes(), 49, &ok );
    TESTCHECK( angleDMS3.Seconds(), 48., &ok );

    if ( ok )
        cout << "AngleDMS PASSED." << endl << endl;
    else
        cout << "AngleDMS FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

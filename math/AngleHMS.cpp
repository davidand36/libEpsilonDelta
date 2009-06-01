/*
  AngleHMS.cpp
  Copyright (C) 2007 David M. Anderson

  AngleHMS class: represents an angle in hours, minutes, and seconds.
*/


#include "AngleHMS.hpp"
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


AngleHMS::AngleHMS( double hours )
{
    Set( hours );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

AngleHMS::AngleHMS( const Angle & angle )
{
    Set( angle );
}

//=============================================================================

void
AngleHMS::Set( double hours )
{
    while ( hours < 0. )
        hours += 24.;
    while ( hours >= 24. )
        hours -= 24.;
    double w;
    double f = modf( hours, &w );
    m_hours = static_cast<int>( w );
    f = modf( 60. * f, &w );
    m_minutes = static_cast<int>( w );
    m_seconds = 60. * f;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
AngleHMS::Set( const Angle & angle )
{
    Set( angle.Degrees( ) / 15. );
}

//=============================================================================

ostream & 
operator<<( ostream & out, const AngleHMS & angle )
{
    return out << setw( 2 ) << setfill( '0' ) << angle.Hours() << "h"
               << setw( 2 ) << setfill( '0' ) << angle.Minutes() << "m"
               << setw( 2 ) << setfill( '0' ) << angle.Seconds() << "s";
}

//=============================================================================

std::string 
ToJSON( const AngleHMS & angle )
{
    JSONObject jsonObj;
    jsonObj[ "hours" ] = ToJSON( angle.Hours() );
    jsonObj[ "minutes" ] = ToJSON( angle.Minutes() );
    jsonObj[ "seconds" ] = ToJSON( angle.Seconds() );
    return ToJSON( jsonObj );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, AngleHMS * pAngle )
{
    JSONObject jsonObj;
    FromJSON( json, &jsonObj );
    int h, m;
    double s;
    FromJSON( jsonObj[ "hours" ], &h );
    FromJSON( jsonObj[ "minutes" ], &m );
    FromJSON( jsonObj[ "seconds" ], &s );
    pAngle->Set( h, m, s );
}

//=============================================================================

#ifdef DEBUG

bool 
AngleHMS::Test( )
{
    bool ok = true;
    cout << "Testing AngleHMS" << endl;

    cout << "AngleHMS( ) [default constructor]" << endl;
    AngleHMS angleHMS0;
    cout << "Set( )" << endl;
    angleHMS0.Set( );
    TESTCHECK( angleHMS0.Hours(), 0, &ok );
    TESTCHECK( angleHMS0.Minutes(), 0, &ok );
    TESTCHECK( angleHMS0.Seconds(), 0., &ok );
    TESTCHECK( angleHMS0.TotalHours(), 0., &ok );
    int h = 14;
    int m = 39;
    double s = 36.;
    cout << "AngleHMS( " << h << ", " << m << ", " << s << " ) [h,m,s constructor]" << endl;
    AngleHMS angleHMS1( h, m, s );
    TESTCHECK( angleHMS1.Hours(), h, &ok );
    TESTCHECK( angleHMS1.Minutes(), m, &ok );
    TESTCHECK( angleHMS1.Seconds(), s, &ok );
    TESTCHECKF( angleHMS1.TotalHours(), 14.66, &ok );
    double hh = 55.40166666667;
    cout << "AngleHMS( " << hh << " ) [hours constructor]" << endl;
    AngleHMS angleHMS2( hh );
    TESTCHECK( angleHMS2.Hours(), 7, &ok );
    TESTCHECK( angleHMS2.Minutes(), 24, &ok );
    TESTCHECKF( angleHMS2.Seconds(), 6., &ok );
    TESTCHECKF( angleHMS2.TotalHours(), 7.40166667, &ok );
    double r = -2.;
    Angle angle2( r );
    cout << "AngleHMS( Angle( " << r << " ) ) [Angle constructor]" << endl;
    AngleHMS angleHMS3( angle2 );
    TESTCHECK( angleHMS3.Hours(), 16, &ok );
    TESTCHECK( angleHMS3.Minutes(), 21, &ok );
    TESTCHECKF( angleHMS3.Seconds(), 38.02583372, &ok );
    TESTCHECKF( angleHMS3.TotalHours(), 16.36056273, &ok );
    cout << "Angle( angleHMS3 )" << endl;
    Angle angle( angleHMS3 );
    TESTCHECKF( angle.Radians(), (r + 2 * M_PI), &ok );

    ostringstream ost;
    cout << "operator<<" << endl;
    ost << angleHMS2;
    TESTCHECK( ost.str(), string( "07h24m06s" ), &ok );

    TESTCHECK( ToJSON( angleHMS1 ),
               string( "{\n"
                       "\"hours\": 14,\n"
                       "\"minutes\": 39,\n"
                       "\"seconds\": +3.60000000000000e+001\n"
                       "}" ),
               &ok );
    cout << "FromJSON( ToJSON( angleHMS1 ), &angleHMS3 )" << endl;
    FromJSON( ToJSON( angleHMS1 ), &angleHMS3 );
    TESTCHECK( angleHMS3.Hours(), 14, &ok );
    TESTCHECK( angleHMS3.Minutes(), 39, &ok );
    TESTCHECK( angleHMS3.Seconds(), 36., &ok );

    if ( ok )
        cout << "AngleHMS PASSED." << endl << endl;
    else
        cout << "AngleHMS FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//=============================================================================

}                                                      //namespace EpsilonDelta

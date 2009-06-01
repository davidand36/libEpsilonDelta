/*
  DateJD.cpp
  Copyright (C) 2007 David M. Anderson

  DateJD class, representing dates with a Julian Day equivalent,
  i.e. a base class for all dates.
  NOTE:
  Should always have a non-inline virtual function, in this case, Set.
*/


#include "DateJD.hpp"
#include "GregorianCalendar.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <string>
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void
DateJD::Set( bool today )
{
    m_julianDay = (today ? Today( ) : INVALID );
}

//-----------------------------------------------------------------------------

int
DateJD::Today( )
{
    int day, month, year;
    GregorianCalendar::Today( &day, &month, &year );
    return ( GregorianCalendar::DMYToJulianDay( day, month, year ) );
}

//=============================================================================

#if DEBUG

bool
DateJD::Test( )
{
    bool ok = true;
    cout << "Testing DateJD" << endl;

    cout << "DateJD() [default constructor]" << endl;
    DateJD jdDate;
    TESTCHECK( jdDate.JulianDay( ), INVALID, &ok );
    TESTCHECK( jdDate.Valid( ), false, &ok );
    int incr = 10;
    cout << "Increment(" << incr << ") :" << endl;
    jdDate.Increment( incr );
    TESTCHECK( jdDate.JulianDay( ), INVALID, &ok );
    TESTCHECK( jdDate.Valid( ), false, &ok );
    int jd = 2000000;
    cout << "Set(" << jd << ") :" << endl;
    jdDate.Set( jd );
    TESTCHECK( jdDate.JulianDay( ), jd, &ok );
    TESTCHECK( jdDate.Valid( ), true, &ok );
    cout << "Increment(" << incr << ") :" << endl;
    jdDate.Increment( incr );
    TESTCHECK( jdDate.JulianDay( ), jd + 10, &ok );
    TESTCHECK( jdDate.Valid( ), true, &ok );
    incr = -20;
    cout << "Increment(" << incr << ") :" << endl;
    jdDate.Increment( incr );
    TESTCHECK( jdDate.JulianDay( ), jd - 10, &ok );
    TESTCHECK( jdDate.Valid( ), true, &ok );

    cout << "DateJD( true ) [today constructor]" << endl;
    DateJD jdToday( true );
    cout << "jdToday.JulianDay( )=" << jdToday.JulianDay( );
    if ( (jdToday.JulianDay( ) > 2451545) && (jdToday.JulianDay( ) < 2500000) )
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( jdToday.JulianDay( ), Today( ), &ok );
    TESTCHECK( jdToday.Valid( ), true, &ok );
    TESTCHECK( (jdDate == jdToday), false, &ok );
    TESTCHECK( (jdDate < jdToday), true, &ok );

    jd = 2451545;
    cout << "DateJD(" << jd << ") [Julian Day constructor]" << endl;
    DateJD jdJD( jd );
    TESTCHECK( jdJD.JulianDay( ), jd, &ok );
    TESTCHECK( jdJD.Valid( ), true, &ok );
    incr = 10;
    cout << "Increment(" << incr << ") :" << endl;
    jdJD.Increment( incr );
    TESTCHECK( jdJD.JulianDay( ), jd + 10, &ok );
    TESTCHECK( jdJD.Valid( ), true, &ok );
    incr = -20;
    cout << "Increment(" << incr << ") :" << endl;
    jdJD.Increment( incr );
    TESTCHECK( jdJD.JulianDay( ), jd - 10, &ok );
    TESTCHECK( jdJD.Valid( ), true, &ok );
    TESTCHECK( (jdDate == jdJD), false, &ok );
    TESTCHECK( (jdDate < jdJD), true, &ok );
    TESTCHECK( (jdJD == jdToday), false, &ok );
    TESTCHECK( (jdJD < jdToday), true, &ok );

    if ( ok )
        cout << "DateJD PASSED." << endl << endl;
    else
        cout << "DateJD FAILED." << endl << endl;
    return ok;
}

#endif

//*****************************************************************************

}                                                      //namespace EpsilonDelta


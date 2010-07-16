/*
  BahaiDate.cpp
  Copyright (C) 2007 David M. Anderson

  BahaiDate class, representing a date in the Bahai calendar.
*/


#include "BahaiDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "GregorianDate.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool
TestBahaiDate( )
{
    bool ok = true;
    cout << "Testing BahaiDate" << endl;

    cout << "BahaiDate() [default constructor]" << endl;
    BahaiDate bahDate;
    TESTCHECK( bahDate.Valid( ), true, &ok );
    TESTCHECK( bahDate.Day( ), 1, &ok );
    TESTCHECK( bahDate.Month( ), 1, &ok );
    TESTCHECK( bahDate.Year( ), 1L, &ok );
    TESTCHECK( bahDate.JulianDay( ), 2394647L, &ok );
    TESTCHECK( bahDate.DayOfWeek( ), 5, &ok );
    TESTCHECK( bahDate.ToString( ), string( "Istijlāl, 1 Bahā` 1" ), &ok );

    int d = 9;
    int m = 13;
    long y = 102;
    cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
    bahDate.Set( d, m, y );
    TESTCHECK( bahDate.Valid( ), true, &ok );
    TESTCHECK( bahDate.Day( ), d, &ok );
    TESTCHECK( bahDate.Month( ), m, &ok );
    TESTCHECK( bahDate.Year( ), y, &ok );
    TESTCHECK( bahDate.JulianDay( ), 2431772L, &ok );
    TESTCHECK( bahDate.DayOfWeek( ), 2, &ok );
    TESTCHECK( bahDate.ToString( ), string( "Kamāl, 9 Qudrat 102" ), &ok );
    int incr = 40;
    cout << "Increment(" << incr << ") :" << endl;
    bahDate.Increment( incr );
    TESTCHECK( bahDate.JulianDay( ), 2431812L, &ok );
    TESTCHECK( bahDate.ToString( ), string( "Jalāl, 11 Masā'il 102" ), &ok );
    incr = 20;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    bahDate.Increment( 0, incr, 0 );
    TESTCHECK( bahDate.JulianDay( ), 2432177L, &ok );
    TESTCHECK( bahDate.ToString( ), string( "Jamāl, 11 Masā'il 103" ), &ok );

    cout << "BahaiDate( true ) [today constructor]" << endl;
    BahaiDate bahToday( true );
    TESTCHECK( bahToday.Valid( ), true, &ok );
    cout << "bahToday.JulianDay( )=" << bahToday.JulianDay( );
    if ( (bahToday.JulianDay( ) > 2451545)
         && (bahToday.JulianDay( ) < 2500000) ) //test good until 289 B.E.
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (bahDate == bahToday), false, &ok );
    TESTCHECK( (bahDate < bahToday), true, &ok );
    cout << "bahToday.ToString()=" << bahToday.ToString( ) << endl;

    long jd = 2431772;
    cout << "BahaiDate( " << jd << " ) [Julian Day constructor]" << endl;
    BahaiDate bahJD( jd );
    TESTCHECK( bahJD.Valid( ), true, &ok );
    TESTCHECK( (bahJD == bahJD), true, &ok );
    TESTCHECK( (bahJD < bahToday), true, &ok );
    TESTCHECK( (bahJD == bahDate), false, &ok );
    TESTCHECK( bahJD.ToString( ), string( "Kamāl, 9 Qudrat 102" ), &ok );

    d = 10;
    m = 13;
    y = 102;
    cout << "BahaiDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    BahaiDate bahDMY( d, m, y );
    TESTCHECK( bahDMY.Valid( ), true, &ok );
    TESTCHECK( (bahJD < bahDMY), true, &ok );
    TESTCHECK( (bahDMY < bahToday), true, &ok );
    TESTCHECK( bahDMY.JulianDay( ), 2431773L, &ok );
    TESTCHECK( bahDMY.ToString( ), string( "Fidāl, 10 Qudrat 102" ), &ok );

    jd = 2394646;
    cout << "BahaiDate( " << jd << " ) [Julian Day constructor]" << endl;
    BahaiDate bah0( jd );
    TESTCHECK( bah0.Valid( ), true, &ok );
    TESTCHECK( (bah0 == bah0), true, &ok );
    TESTCHECK( (bah0 < bahDate), true, &ok );
    TESTCHECK( (bah0 == bahDate), false, &ok );
    TESTCHECK( bah0.ToString( ), string( "`Idāl, 19 `Alā' 0" ), &ok );

    d = 1; m = 1; y = 0;
    cout << "BahaiDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    BahaiDate bah110( d, m, y );
    TESTCHECK( bah110.Valid( ), true, &ok );
    TESTCHECK( (bah110 < bahJD), true, &ok );
    TESTCHECK( (bah110 < bahToday), true, &ok );
    TESTCHECK( bah110.JulianDay( ), 2394281L, &ok );
    TESTCHECK( bah110.ToString( ), string( "Fidāl, 1 Bahā` 0" ), &ok );

    cout << "BahaiDate( bah0 ) [copy constructor]" << endl;
    BahaiDate bahEq = bah0;
    TESTCHECK( bahEq.Valid( ), true, &ok );
    TESTCHECK( (bahEq == bah0), true, &ok );
    TESTCHECK( bahEq.ToString( ), string( "`Idāl, 19 `Alā' 0" ), &ok );
    cout << "= bah110 [assignment]" << endl;
    bahEq = bah110;
    TESTCHECK( bahEq.Valid( ), true, &ok );
    TESTCHECK( (bahEq == bah0), false, &ok );
    TESTCHECK( (bahEq == bah110), true, &ok );
    TESTCHECK( bah110.ToString( ), string( "Fidāl, 1 Bahā` 0" ), &ok );

    cout << "BahaiDate( GregorianDate( 21, 3, 1844 ) ) [DateDMY constructor]" << endl;
    BahaiDate bahGreg( GregorianDate( 21, 3, 1844 ) );
    TESTCHECK( bahGreg.Valid( ), true, &ok );
    TESTCHECK( bahGreg.ToString( ), string( "Istijlāl, 1 Bahā` 1" ), &ok );
    cout << "= GregorianDate( 12, 11, 1945 )" << endl;
    bahGreg = GregorianDate( 12, 11, 1945 );
    TESTCHECK( bahGreg.Valid( ), true, &ok );
    TESTCHECK( bahGreg.ToString( ), string( "Kamāl, 9 Qudrat 102" ), &ok );
    cout << "GregorianDate( BahaiDate( 16, 10, 118 ) ) [conversion]" << endl;
    GregorianDate gregBah( BahaiDate( 16, 10, 118 ) );
    TESTCHECK( gregBah.Valid( ), true, &ok );
    TESTCHECK( gregBah.ToString( ), string( "Saturday, 23 September 1961" ), &ok );
    cout << "= BahaiDate( 7, 17, 111 )" << endl;
    gregBah = BahaiDate( 7, 17, 111 );
    TESTCHECK( gregBah.Valid( ), true, &ok );
    TESTCHECK( gregBah.ToString( ), string( "Tuesday, 25 January 1955" ), &ok );

    if ( ok )
        cout << "BahaiDate PASSED." << endl << endl;
    else
        cout << "BahaiDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

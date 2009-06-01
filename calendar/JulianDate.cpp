/*
  JulianDate.cpp
  Copyright (C) 2007 David M. Anderson

  JulianDate class, representing dates in the Julian calendar.
*/


#include "JulianDate.hpp"
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
TestJulianDate( )
{
    bool ok = true;
    cout << "Testing JulianDate" << endl;

    cout << "JulianDate() [default constructor]" << endl;
    JulianDate julDate;
    TESTCHECK( julDate.Valid( ), true, &ok );
    TESTCHECK( julDate.Day( ), 1, &ok );
    TESTCHECK( julDate.Month( ), 1, &ok );
    TESTCHECK( julDate.Year( ), 1, &ok );
    TESTCHECK( julDate.JulianDay( ), 1721424, &ok );
    TESTCHECK( julDate.DayOfWeek( ), 6, &ok );
    TESTCHECK( julDate.ToString( ), string( "Saturday, 1 January 1" ), &ok );
    TESTCHECK( julDate.ToString( "%2W %02d %3M %4y" ),
               string( "Sa 01 Jan    1" ), &ok );

    int jd = 2026872;
    cout << "Set(" << jd << ") :" << endl;
    julDate.Set( jd );
    TESTCHECK( julDate.Valid( ), true, &ok );
    TESTCHECK( julDate.Day( ), 10, &ok );
    TESTCHECK( julDate.Month( ), 4, &ok );
    TESTCHECK( julDate.Year( ), 837, &ok );
    TESTCHECK( julDate.JulianDay( ), jd, &ok );
    TESTCHECK( julDate.DayOfWeek( ), 2, &ok );
    TESTCHECK( julDate.ToString( ), string( "Tuesday, 10 April 837" ), &ok );
    TESTCHECK( julDate.ToString( "%2W %02d %3M %4y" ),
               string( "Tu 10 Apr  837" ), &ok );
    int incr = 30;
    cout << "Increment(" << incr << ") :" << endl;
    julDate.Increment( incr );
    TESTCHECK( julDate.JulianDay( ), jd + 30, &ok );
    TESTCHECK( julDate.ToString( ), string( "Thursday, 10 May 837" ), &ok );
    incr = 12;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    julDate.Increment( 0, incr, 0 );
    TESTCHECK( julDate.ToString( ), string( "Friday, 10 May 838" ), &ok );

    cout << "JulianDate( true ) [today constructor]" << endl;
    JulianDate julToday( true );
    TESTCHECK( julToday.Valid( ), true, &ok );
    cout << "julToday.JulianDay( )=" << julToday.JulianDay( );
    if ( (julToday.JulianDay( ) > 2451545) && (julToday.JulianDay( ) < 2500000) )
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (julDate == julToday), false, &ok );
    TESTCHECK( (julDate < julToday), true, &ok );
    cout << "julToday.ToString()=" << julToday.ToString( ) << endl;

    jd = 1356001;
    cout << "JulianDate( " << jd << " ) [Julian Day constructor]" << endl;
    JulianDate julJD( jd );
    TESTCHECK( julJD.Valid( ), true, &ok );
    TESTCHECK( (julJD == julJD), true, &ok );
    TESTCHECK( (julJD < julDate), true, &ok );
    TESTCHECK( (julJD == julDate), false, &ok );
    TESTCHECK( julJD.ToString( ), string( "Thursday, 12 July -1000" ), &ok );

    int d = 29, m = 2, y = -1000;
    cout << "JulianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    JulianDate julDMY( d, m, y );
    TESTCHECK( julDMY.Valid( ), true, &ok );
    TESTCHECK( (julDMY < julJD), true, &ok );
    TESTCHECK( (julDMY < julToday), true, &ok );
    TESTCHECK( julDMY.JulianDay( ), 1355867, &ok );
    TESTCHECK( julDMY.ToString( "%2W %02d %3M %4y" ),
               string( "We 29 Feb -1000" ), &ok );

    jd = 1721423;
    cout << "JulianDate( " << jd << " ) [Julian Day constructor]" << endl;
    JulianDate jul0( jd );
    TESTCHECK( jul0.Valid( ), true, &ok );
    TESTCHECK( (jul0 == jul0), true, &ok );
    TESTCHECK( (jul0 < julDate), true, &ok );
    TESTCHECK( (jul0 == julDate), false, &ok );
    TESTCHECK( jul0.ToString( ), string( "Friday, 31 December 0" ), &ok );

    d = 1; m = 1; y = 0;
    cout << "JulianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    JulianDate jul110( d, m, y );
    TESTCHECK( jul110.Valid( ), true, &ok );
    TESTCHECK( (jul110 < julJD), false, &ok );
    TESTCHECK( (jul110 < julToday), true, &ok );
    TESTCHECK( jul110.JulianDay( ), 1721058, &ok );
    TESTCHECK( jul110.ToString( "%2W %02d %3M %4y" ),
               string( "Th 01 Jan    0" ), &ok );

    cout << "JulianDate( jul0 ) [copy constructor]" << endl;
    JulianDate julEq = jul0;
    TESTCHECK( julEq.Valid( ), true, &ok );
    TESTCHECK( (julEq == jul0), true, &ok );
    TESTCHECK( julEq.ToString( ), string( "Friday, 31 December 0" ), &ok );
    cout << "= jul110 [assignment]" << endl;
    julEq = jul110;
    TESTCHECK( julEq.Valid( ), true, &ok );
    TESTCHECK( (julEq == jul0), false, &ok );
    TESTCHECK( (julEq == jul110), true, &ok );
    TESTCHECK( jul110.ToString( "%2W %02d %3M %4y" ),
               string( "Th 01 Jan    0" ), &ok );

    cout << "JulianDate( GregorianDate( 1, 1, 1 ) ) [DateDMY constructor]" << endl;
    JulianDate julGreg( GregorianDate( 1, 1, 1 ) );
    TESTCHECK( julGreg.Valid( ), true, &ok );
    TESTCHECK( julGreg.ToString( ), string( "Monday, 3 January 1" ), &ok );
    cout << "= GregorianDate( 12, 11, 1945 )" << endl;
    julGreg = GregorianDate( 12, 11, 1945 );
    TESTCHECK( julGreg.Valid( ), true, &ok );
    TESTCHECK( julGreg.ToString( ), string( "Monday, 30 October 1945" ), &ok );
    
    cout << "GregorianDate( JulianDate( 3, 1, 1 ) [DateDMY constructor]" << endl;
    GregorianDate gregJul( JulianDate( 3, 1, 1 ) );
    TESTCHECK( gregJul.Valid( ), true, &ok );
    TESTCHECK( gregJul.ToString( ), string( "Monday, 1 January 1" ), &ok );

    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
            = {
                { 1507232,  30,  7, -586 },
                { 1660038,   8, 12, -168 },
                { 1746894,  26,  9,   70 },
                { 1770642,   3, 10,  135 },
                { 1892732,   7,  1,  470 },
                { 1931580,  18,  5,  576 },
                { 1974852,   7, 11,  694 },
                { 2091165,  19,  4, 1013 },
                { 2121510,  18,  5, 1096 },
                { 2155780,  16,  3, 1190 },
                { 2174030,   3,  3, 1240 },
                { 2191585,  26,  3, 1288 },
                { 2195262,  20,  4, 1298 },
                { 2229275,   4,  6, 1391 },
                { 2245581,  25,  1, 1436 },
                { 2266101,  31,  3, 1492 },
                { 2288543,   9,  9, 1553 },
                { 2290902,  24,  2, 1560 },
                { 2323141,  31,  5, 1648 },
                { 2334849,  20,  6, 1680 },
                { 2348021,  13,  7, 1716 },
                { 2366979,   8,  6, 1768 },
                { 2385649,  21,  7, 1819 },
                { 2392826,  15,  3, 1839 },
                { 2416224,   6,  4, 1903 },
                { 2425849,  12,  8, 1929 },
                { 2430267,  16,  9, 1941 },
                { 2430834,   6,  4, 1943 },
                { 2431005,  24,  9, 1943 },
                { 2448699,   4,  3, 1992 },
                { 2450139,  12,  2, 1996 },
                { 2465738,  28, 10, 2038 },
                { 2486077,   5,  7, 2094 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        julDate.Set( d, m, y );
        TESTCHECK( julDate.Valid( ), true, &ok );
        TESTCHECK( julDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        julDate.Set( jd );
        TESTCHECK( julDate.Valid( ), true, &ok );
        TESTCHECK( julDate.Day( ), d, &ok );
        TESTCHECK( julDate.Month( ), m, &ok );
        TESTCHECK( julDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "JulianDate PASSED." << endl << endl;
    else
        cout << "JulianDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

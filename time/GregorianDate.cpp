/*
  GregorianDate.cpp
  Copyright (C) 2007 David M. Anderson

  GregorianDate class, representing a date in the Gregorian calendar.
*/


#include "GregorianDate.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Date.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


// Because std routines use the Gregorian calendar to begin with, 
//  we specialize this to avoid converting from Gregorian date to Julian Day 
//  and back again.

template<>
void 
DateDMY< GregorianCalendar >::Set( bool today )
{
    if ( today )
    {
        int day, month;
        long year;
        GregorianCalendar::Today( &day, &month, &year );
        Set( day, month, year );
    }
    else
        Set( 1, 1, 1 );
}

//=============================================================================

#ifdef DEBUG

bool 
TestGregorianDate( )
{
    bool ok = true;
    cout << "Testing GregorianDate" << endl;

    cout << "GregorianDate() [default constructor]" << endl;
    GregorianDate gregDate;
    TESTCHECK( gregDate.Valid( ), true, &ok );
    TESTCHECK( gregDate.Day( ), 1, &ok );
    TESTCHECK( gregDate.Month( ), 1, &ok );
    TESTCHECK( gregDate.Year( ), 1L, &ok );
    TESTCHECK( gregDate.JulianDay( ), 1721426L, &ok );
    TESTCHECK( gregDate.DayOfWeek( ), 1, &ok );
    TESTCHECK( gregDate.ToString( ), string( "Monday, 1 January 1" ), &ok );
    TESTCHECK( gregDate.ToString( "%2W %02d %3M %4y" ),
               string( "Mo 01 Jan    1" ), &ok );

    int d = 1;
    int m = 1;
    long y = 1600;
    cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
    gregDate.Set( d, m, y );
    TESTCHECK( gregDate.Valid( ), true, &ok );
    TESTCHECK( gregDate.Day( ), d, &ok );
    TESTCHECK( gregDate.Month( ), m, &ok );
    TESTCHECK( gregDate.Year( ), y, &ok );
    TESTCHECK( gregDate.JulianDay( ), 2305448L, &ok );
    TESTCHECK( gregDate.DayOfWeek( ), 6, &ok );
    TESTCHECK( gregDate.ToString( ), string( "Saturday, 1 January 1600" ), &ok );
    TESTCHECK( gregDate.ToString( "%2W %02d %3M %4y" ),
               string( "Sa 01 Jan 1600" ), &ok );
    int incr = 40;
    cout << "Increment(" << incr << ") :" << endl;
    gregDate.Increment( incr );
    TESTCHECK( gregDate.JulianDay( ), 2305488L, &ok );
    TESTCHECK( gregDate.ToString( ), string( "Thursday, 10 February 1600" ), &ok );
    incr = 12;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    gregDate.Increment( 0, incr, 0 );
    TESTCHECK( gregDate.JulianDay( ), 2305854L, &ok );
    TESTCHECK( gregDate.ToString( ), string( "Saturday, 10 February 1601" ), &ok );
    cout << "Increment( Monday, 1 )" << endl;
    gregDate.Increment( Monday, 1 );
    TESTCHECK( gregDate.ToString( ), string( "Monday, 19 February 1601" ), &ok );
    cout << "Increment( Friday, -1 )" << endl;
    gregDate.Increment( Friday, -1 );
    TESTCHECK( gregDate.ToString( ), string( "Friday, 16 February 1601" ), &ok );
    cout << "Increment( Friday, 0 )" << endl;
    gregDate.Increment( Friday, 0 );
    TESTCHECK( gregDate.ToString( ), string( "Friday, 16 February 1601" ), &ok );

    cout << "GregorianDate( true ) [today constructor]" << endl;
    GregorianDate gregToday( true );
    TESTCHECK( gregToday.Valid( ), true, &ok );
    cout << "gregToday.JulianDay( )=" << gregToday.JulianDay( );
    if ( (gregToday.JulianDay( ) > 2451545)
         && (gregToday.JulianDay( ) < 2500000) ) //test good until 2132 A.D.
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (gregDate == gregToday), false, &ok );
    TESTCHECK( (gregDate < gregToday), true, &ok );
    cout << "gregToday.ToString()=" << gregToday.ToString( ) << endl;

    long jd = 2451545;
    cout << "GregorianDate( " << jd << " ) [Julian Day constructor]" << endl;
    GregorianDate gregJD( jd );
    TESTCHECK( gregJD.Valid( ), true, &ok );
    TESTCHECK( (gregJD == gregJD), true, &ok );
    TESTCHECK( (gregJD < gregToday), true, &ok );
    TESTCHECK( (gregJD == gregDate), false, &ok );
    TESTCHECK( gregJD.ToString( ), string( "Saturday, 1 January 2000" ), &ok );

    d = 25;
    m = 1;
    y = 1955;
    cout << "GregorianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    GregorianDate gregDMY( d, m, y );
    TESTCHECK( gregDMY.Valid( ), true, &ok );
    TESTCHECK( (gregDMY < gregJD), true, &ok );
    TESTCHECK( (gregDMY < gregToday), true, &ok );
    TESTCHECK( gregDMY.JulianDay( ), 2435133L, &ok );
    TESTCHECK( gregDMY.ToString( "%2W %02d %3M %2y" ),
               string( "Tu 25 Jan 55" ), &ok );

    jd = 1721425;
    cout << "GregorianDate( " << jd << " ) [Julian Day constructor]" << endl;
    GregorianDate greg0( jd );
    TESTCHECK( greg0.Valid( ), true, &ok );
    TESTCHECK( (greg0 == greg0), true, &ok );
    TESTCHECK( (greg0 < gregDate), true, &ok );
    TESTCHECK( (greg0 == gregDate), false, &ok );
    TESTCHECK( greg0.ToString( ), string( "Sunday, 31 December 0" ), &ok );

    d = 1; m = 1; y = 0;
    cout << "GregorianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    GregorianDate greg110( d, m, y );
    TESTCHECK( greg110.Valid( ), true, &ok );
    TESTCHECK( (greg110 < gregJD), true, &ok );
    TESTCHECK( (greg110 < gregToday), true, &ok );
    TESTCHECK( greg110.JulianDay( ), 1721060L, &ok );
    TESTCHECK( greg110.ToString( "%2W %02d %3M %4y" ),
               string( "Sa 01 Jan    0" ), &ok );

    cout << "GregorianDate( greg0 ) [copy constructor]" << endl;
    GregorianDate gregEq = greg0;
    TESTCHECK( gregEq.Valid( ), true, &ok );
    TESTCHECK( (gregEq == greg0), true, &ok );
    TESTCHECK( gregEq.ToString( ), string( "Sunday, 31 December 0" ), &ok );
    cout << "= greg110 [assignment]" << endl;
    gregEq = greg110;
    TESTCHECK( gregEq.Valid( ), true, &ok );
    TESTCHECK( (gregEq == greg0), false, &ok );
    TESTCHECK( (gregEq == greg110), true, &ok );
    TESTCHECK( greg110.ToString( ), string( "Saturday, 1 January 0" ), &ok );

    cout << "Set( false )" << endl;
    gregToday.Set( false );
    TESTCHECK( gregToday.Valid( ), true, &ok );
    TESTCHECK( gregToday.Day( ), 1, &ok );
    TESTCHECK( gregToday.Month( ), 1, &ok );
    TESTCHECK( gregToday.Year( ), 1L, &ok );
    TESTCHECK( gregToday.JulianDay( ), 1721426L, &ok );
    TESTCHECK( gregToday.DayOfWeek( ), 1, &ok );
    TESTCHECK( gregToday.ToString( ), string( "Monday, 1 January 1" ), &ok );
    TESTCHECK( gregToday.ToString( "%2W %02d %3M %4y" ),
               string( "Mo 01 Jan    1" ), &ok );

    cout << "Set( true ) [today]" << endl;
    gregToday.Set( true );
    TESTCHECK( gregToday.Valid( ), true, &ok );
    cout << "gregToday.JulianDay( )=" << gregToday.JulianDay( );
    if ( (gregToday.JulianDay( ) > 2451545)
         && (gregToday.JulianDay( ) < 2500000) ) //test good until 2132 A.D.
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (gregDate == gregToday), false, &ok );
    TESTCHECK( (gregDate < gregToday), true, &ok );
    cout << "gregToday.ToString()=" << gregToday.ToString( ) << endl;

    struct
    {
        long julianDay;
        int day;
        int month;
        long year;
    } 
    testDates[]
            = {
                { 1507232,  24,  7, -586 },
                { 1660038,   5, 12, -168 },
                { 1746894,  24,  9,   70 },
                { 1770642,   2, 10,  135 },
                { 1892732,   8,  1,  470 },
                { 1931580,  20,  5,  576 },
                { 1974852,  10, 11,  694 },
                { 2091165,  25,  4, 1013 },
                { 2121510,  24,  5, 1096 },
                { 2155780,  23,  3, 1190 },
                { 2174030,  10,  3, 1240 },
                { 2191585,   2,  4, 1288 },
                { 2195262,  27,  4, 1298 },
                { 2229275,  12,  6, 1391 },
                { 2245581,   3,  2, 1436 },
                { 2266101,   9,  4, 1492 },
                { 2288543,  19,  9, 1553 },
                { 2290902,   5,  3, 1560 },
                { 2323141,  10,  6, 1648 },
                { 2334849,  30,  6, 1680 },
                { 2348021,  24,  7, 1716 },
                { 2366979,  19,  6, 1768 },
                { 2385649,   2,  8, 1819 },
                { 2392826,  27,  3, 1839 },
                { 2416224,  19,  4, 1903 },
                { 2425849,  25,  8, 1929 },
                { 2430267,  29,  9, 1941 },
                { 2430834,  19,  4, 1943 },
                { 2431005,   7, 10, 1943 },
                { 2448699,  17,  3, 1992 },
                { 2450139,  25,  2, 1996 },
                { 2465738,  10, 11, 2038 },
                { 2486077,  18,  7, 2094 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        gregDate.Set( d, m, y );
        TESTCHECK( gregDate.Valid( ), true, &ok );
        TESTCHECK( gregDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        gregDate.Set( jd );
        TESTCHECK( gregDate.Valid( ), true, &ok );
        TESTCHECK( gregDate.Day( ), d, &ok );
        TESTCHECK( gregDate.Month( ), m, &ok );
        TESTCHECK( gregDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "GregorianDate PASSED." << endl << endl;
    else
        cout << "GregorianDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

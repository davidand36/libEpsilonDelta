/*
  PersianDate.cpp
  Copyright (C) 2007 David M. Anderson

  PersianDate class, representing a date in the Persian calendar.
*/


#include "PersianDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "GregorianDate.hpp"
#include "Array.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool
TestPersianDate( )
{
    bool ok = true;
    cout << "Testing PersianDate" << endl;

    cout << "Astronomical:" << endl;
    
    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    astronomicalTestDates[]
            = {
                { 1507232,   1,  5,-1207 },
                { 1660038,  14,  9, -789 },
                { 1746894,   2,  7, -551 },
                { 1770642,   9,  7, -486 },
                { 1892732,  19, 10, -152 },
                { 1931580,  31,  2,  -45 },
                { 1974852,  19,  8,   73 },
                { 2091165,   5,  2,  392 },
                { 2121510,   4,  3,  475 },
                { 2155780,   3,  1,  569 },
                { 2174030,  20, 12,  618 },
                { 2191585,  14,  1,  667 },
                { 2195262,   8,  2,  677 },
                { 2229275,  22,  3,  770 },
                { 2245581,  13, 11,  814 },
                { 2266101,  21,  1,  871 },
                { 2288543,  28,  6,  932 },
                { 2290902,  14, 12,  938 },
                { 2323141,  21,  3, 1027 },
                { 2334849,  10,  4, 1059 },
                { 2348021,   2,  5, 1095 },
                { 2366979,  30,  3, 1147 },
                { 2385649,  10,  5, 1198 },
                { 2392826,   7,  1, 1218 },
                { 2416224,  29,  1, 1282 },
                { 2425849,   3,  6, 1308 },
                { 2430267,   7,  7, 1320 },
                { 2430834,  29,  1, 1322 },
                { 2431005,  14,  7, 1322 },
                { 2448699,  27, 12, 1370 },
                { 2450139,   6, 12, 1374 },
                { 2465738,  19,  8, 1417 },
                { 2486077,  28,  4, 1473 }
            };

    for ( int i = 0; i < ARRAY_LENGTH( astronomicalTestDates ); ++i )
    {
        int jd = astronomicalTestDates[i].julianDay;
        int d = astronomicalTestDates[i].day;
        int m = astronomicalTestDates[i].month;
        int y = astronomicalTestDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        PersianDate persDate( d, m, y );
        TESTCHECK( persDate.Valid( ), true, &ok );
        TESTCHECK( persDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        persDate.Set( jd );
        TESTCHECK( persDate.Valid( ), true, &ok );
        TESTCHECK( persDate.Day( ), d, &ok );
        TESTCHECK( persDate.Month( ), m, &ok );
        TESTCHECK( persDate.Year( ), y, &ok );
    }

    cout << "Arithmetic:" << endl;

    PersianCalendar::SetMethod( PersianCalendar::Arithmetic );
    
    cout << "PersianDate() [default constructor]" << endl;
    PersianDate persDate;
    TESTCHECK( persDate.Valid( ), true, &ok );
    TESTCHECK( persDate.Day( ), 1, &ok );
    TESTCHECK( persDate.Month( ), 1, &ok );
    TESTCHECK( persDate.Year( ), 1, &ok );
    TESTCHECK( persDate.JulianDay( ), 1948321, &ok );
    TESTCHECK( persDate.DayOfWeek( ), 6, &ok );
    TESTCHECK( persDate.ToString( ), string( "Jom'eh, 1 Farvardin 1" ), &ok );

    int d = 21;
    int m = 8;
    int y = 1324;
    cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
    persDate.Set( d, m, y );
    TESTCHECK( persDate.Valid( ), true, &ok );
    TESTCHECK( persDate.Day( ), d, &ok );
    TESTCHECK( persDate.Month( ), m, &ok );
    TESTCHECK( persDate.Year( ), y, &ok );
    TESTCHECK( persDate.JulianDay( ), 2431772, &ok );
    TESTCHECK( persDate.DayOfWeek( ), 2, &ok );
    TESTCHECK( persDate.ToString( ), string( "Do-shanbeh, 21 Aban 1324" ), &ok );
    int incr = 40;
    cout << "Increment(" << incr << ") :" << endl;
    persDate.Increment( incr );
    TESTCHECK( persDate.JulianDay( ), 2431812, &ok );
    TESTCHECK( persDate.ToString( ), string( "Shanbeh, 1 Dey 1324" ), &ok );
    incr = 12;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    persDate.Increment( 0, incr, 0 );
    TESTCHECK( persDate.JulianDay( ), 2432177, &ok );
    TESTCHECK( persDate.ToString( ), string( "Yek-shanbeh, 1 Dey 1325" ), &ok );

    cout << "PersianDate( true ) [today constructor]" << endl;
    PersianDate persToday( true );
    TESTCHECK( persToday.Valid( ), true, &ok );
    cout << "persToday.JulianDay( )=" << persToday.JulianDay( );
    if ( (persToday.JulianDay( ) > 2451545) && (persToday.JulianDay( ) < 2500000) )
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (persDate == persToday), false, &ok );
    TESTCHECK( (persDate < persToday), true, &ok );
    cout << "persToday.ToString()=" << persToday.ToString( ) << endl;

    int jd = 2431772;
    cout << "PersianDate( " << jd << " ) [Julian Day constructor]" << endl;
    PersianDate persJD( jd );
    TESTCHECK( persJD.Valid( ), true, &ok );
    TESTCHECK( (persJD == persJD), true, &ok );
    TESTCHECK( (persJD < persToday), true, &ok );
    TESTCHECK( (persJD == persDate), false, &ok );
    TESTCHECK( persJD.ToString( ), string( "Do-shanbeh, 21 Aban 1324" ), &ok );

    d = 22;
    m = 8;
    y = 1324;
    cout << "PersianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    PersianDate persDMY( d, m, y );
    TESTCHECK( persDMY.Valid( ), true, &ok );
    TESTCHECK( (persJD < persDMY), true, &ok );
    TESTCHECK( (persDMY < persToday), true, &ok );
    TESTCHECK( persDMY.JulianDay( ), 2431773, &ok );
    TESTCHECK( persDMY.ToString( ), string( "Se-shanbeh, 22 Aban 1324" ), &ok );

    jd = 1948320;
    cout << "PersianDate( " << jd << " ) [Julian Day constructor]" << endl;
    PersianDate pers0( jd );
    TESTCHECK( pers0.Valid( ), true, &ok );
    TESTCHECK( (pers0 == pers0), true, &ok );
    TESTCHECK( (pers0 < persDate), true, &ok );
    TESTCHECK( (pers0 == persDate), false, &ok );
    TESTCHECK( pers0.ToString( ), string( "Panj-shanbeh, 30 Esfand 0" ), &ok );

    d = 1; m = 1; y = 0;
    cout << "PersianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    PersianDate pers110( d, m, y );
    TESTCHECK( pers110.Valid( ), true, &ok );
    TESTCHECK( (pers110 < persJD), true, &ok );
    TESTCHECK( (pers110 < persToday), true, &ok );
    TESTCHECK( pers110.JulianDay( ), 1947955, &ok );
    TESTCHECK( pers110.ToString( ), string( "Chahar-shanbeh, 1 Farvardin 0" ), &ok );

    cout << "PersianDate( pers0 ) [copy constructor]" << endl;
    PersianDate persEq = pers0;
    TESTCHECK( persEq.Valid( ), true, &ok );
    TESTCHECK( (persEq == pers0), true, &ok );
    TESTCHECK( persEq.ToString( ), string( "Panj-shanbeh, 30 Esfand 0" ), &ok );
    cout << "= pers110 [assignment]" << endl;
    persEq = pers110;
    TESTCHECK( persEq.Valid( ), true, &ok );
    TESTCHECK( (persEq == pers0), false, &ok );
    TESTCHECK( (persEq == pers110), true, &ok );
    TESTCHECK( pers110.ToString( ), string( "Chahar-shanbeh, 1 Farvardin 0" ), &ok );

    cout << "PersianDate( GregorianDate( 22, 3, 622 ) ) [DateDMY constructor]" << endl;
    PersianDate persGreg( GregorianDate( 22, 3, 622 ) );
    TESTCHECK( persGreg.Valid( ), true, &ok );
    TESTCHECK( persGreg.ToString( ), string( "Jom'eh, 1 Farvardin 1" ), &ok );
    cout << "= GregorianDate( 12, 11, 1945 )" << endl;
    persGreg = GregorianDate( 12, 11, 1945 );
    TESTCHECK( persGreg.Valid( ), true, &ok );
    TESTCHECK( persGreg.ToString( ), string( "Do-shanbeh, 21 Aban 1324" ), &ok );
    cout << "GregorianDate( PersianDate( 1, 7, 1340 ) ) [conversion]" << endl;
    GregorianDate gregPers( PersianDate( 1, 7, 1340 ) );
    TESTCHECK( gregPers.Valid( ), true, &ok );
    TESTCHECK( gregPers.ToString( ), string( "Saturday, 23 September 1961" ), &ok );
    cout << "= PersianDate( 13, 1, 1381 )" << endl;
    gregPers = PersianDate( 13, 1, 1381 );
    TESTCHECK( gregPers.Valid( ), true, &ok );
    TESTCHECK( gregPers.ToString( ), string( "Tuesday, 2 April 2002" ), &ok );

    cout << "= PersianDate( 21, 8, 1324 )" << endl;
    gregPers = PersianDate( 21, 8, 1324 );
    TESTCHECK( gregPers.Valid( ), true, &ok );
    TESTCHECK( gregPers.ToString( ), string( "Monday, 12 November 1945" ), &ok );

    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    arithmeticTestDates[]
            = {
                { 1507232,   1,  5,-1207 },
                { 1660038,  14,  9, -789 },
                { 1746894,   2,  7, -551 },
                { 1770642,   9,  7, -486 },
                { 1892732,  18, 10, -152 },
                { 1931580,  30,  2,  -45 },
                { 1974852,  19,  8,   73 },
                { 2091165,   5,  2,  392 },
                { 2121510,   3,  3,  475 },
                { 2155780,   3,  1,  569 },
                { 2174030,  20, 12,  618 },
                { 2191585,  14,  1,  667 },
                { 2195262,   8,  2,  677 },
                { 2229275,  22,  3,  770 },
                { 2245581,  13, 11,  814 },
                { 2266101,  21,  1,  871 },
                { 2288543,  28,  6,  932 },
                { 2290902,  14, 12,  938 },
                { 2323141,  21,  3, 1027 },
                { 2334849,  10,  4, 1059 },
                { 2348021,   2,  5, 1095 },
                { 2366979,  30,  3, 1147 },
                { 2385649,  10,  5, 1198 },
                { 2392826,   7,  1, 1218 },
                { 2416224,  29,  1, 1282 },
                { 2425849,   3,  6, 1308 },
                { 2430267,   7,  7, 1320 },
                { 2430834,  29,  1, 1322 },
                { 2431005,  14,  7, 1322 },
                { 2448699,  27, 12, 1370 },
                { 2450139,   6, 12, 1374 },
                { 2465738,  19,  8, 1417 },
                { 2486077,  28,  4, 1473 }
            };

    for ( int i = 0; i < ARRAY_LENGTH( arithmeticTestDates ); ++i )
    {
        jd = arithmeticTestDates[i].julianDay;
        d = arithmeticTestDates[i].day;
        m = arithmeticTestDates[i].month;
        y = arithmeticTestDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        persDate.Set( d, m, y );
        TESTCHECK( persDate.Valid( ), true, &ok );
        TESTCHECK( persDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        persDate.Set( jd );
        TESTCHECK( persDate.Valid( ), true, &ok );
        TESTCHECK( persDate.Day( ), d, &ok );
        TESTCHECK( persDate.Month( ), m, &ok );
        TESTCHECK( persDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "PersianDate PASSED." << endl << endl;
    else
        cout << "PersianDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  EthiopianDate.cpp
  Copyright (C) 2007 David M. Anderson

  EthiopianDate class, representing dates in the Ethiopian calendar.
*/


#include "EthiopianDate.hpp"
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
TestEthiopianDate( )
{
    bool ok = true;
    cout << "Testing EthiopianDate" << endl;

    cout << "EthiopianDate() [default constructor]" << endl;
    EthiopianDate ethDate;
    TESTCHECK( ethDate.Valid( ), true, &ok );
    TESTCHECK( ethDate.Day( ), 1, &ok );
    TESTCHECK( ethDate.Month( ), 1, &ok );
    TESTCHECK( ethDate.Year( ), 1, &ok );
    TESTCHECK( ethDate.JulianDay( ), 1724221, &ok );
    TESTCHECK( ethDate.ToString( ), string( "Rob, 1 Maskaram 1" ), &ok );

    int d = 4;
    int m = 3;
    int y = 1939;
    cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
    ethDate.Set( d, m, y );
    TESTCHECK( ethDate.Valid( ), true, &ok );
    TESTCHECK( ethDate.Day( ), d, &ok );
    TESTCHECK( ethDate.Month( ), m, &ok );
    TESTCHECK( ethDate.Year( ), y, &ok );
    TESTCHECK( ethDate.JulianDay( ), 2432138, &ok );
    TESTCHECK( ethDate.ToString( ), string( "Rob, 4 Hedar 1939" ), &ok );
    int incr = 40;
    cout << "Increment(" << incr << ") :" << endl;
    ethDate.Increment( incr );
    TESTCHECK( ethDate.JulianDay( ), 2432178, &ok );
    TESTCHECK( ethDate.ToString( ), string( "Sanyo, 14 Takhsas 1939" ), &ok );
    incr = 13;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    ethDate.Increment( 0, incr, 0 );
    TESTCHECK( ethDate.JulianDay( ), 2432544, &ok );
    TESTCHECK( ethDate.ToString( ), string( "Rob, 14 Takhsas 1940" ), &ok );

    cout << "EthiopianDate( true ) [today constructor]" << endl;
    EthiopianDate ethToday( true );
    TESTCHECK( ethToday.Valid( ), true, &ok );
    cout << "ethToday.JulianDay( )=" << ethToday.JulianDay( );
    if ( (ethToday.JulianDay( ) > 2451545) && (ethToday.JulianDay( ) < 2500000) )
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (ethDate == ethToday), false, &ok );
    TESTCHECK( (ethDate < ethToday), true, &ok );
    cout << "ethToday.ToString()=" << ethToday.ToString( ) << endl;

    int jd = 2432138;
    cout << "EthiopianDate( " << jd << " ) [Julian Day constructor]" << endl;
    EthiopianDate ethJD( jd );
    TESTCHECK( ethJD.Valid( ), true, &ok );
    TESTCHECK( (ethJD == ethJD), true, &ok );
    TESTCHECK( (ethJD < ethToday), true, &ok );
    TESTCHECK( (ethJD == ethDate), false, &ok );
    TESTCHECK( ethJD.ToString( ), string( "Rob, 4 Hedar 1939" ), &ok );

    d = 5;
    m = 3;
    y = 1939;
    cout << "EthiopianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    EthiopianDate ethDMY( d, m, y );
    TESTCHECK( ethDMY.Valid( ), true, &ok );
    TESTCHECK( (ethJD < ethDMY), true, &ok );
    TESTCHECK( (ethDMY < ethToday), true, &ok );
    TESTCHECK( ethDMY.JulianDay( ), 2432139, &ok );
    TESTCHECK( ethDMY.ToString( ), string( "Hamus, 5 Hedar 1939" ), &ok );

    jd = 1724220;
    cout << "EthiopianDate( " << jd << " ) [Julian Day constructor]" << endl;
    EthiopianDate eth0( jd );
    TESTCHECK( eth0.Valid( ), true, &ok );
    TESTCHECK( (eth0 == eth0), true, &ok );
    TESTCHECK( (eth0 < ethDate), true, &ok );
    TESTCHECK( (eth0 == ethDate), false, &ok );
    TESTCHECK( eth0.ToString( ), string( "Maksanyo, 5 Paguemen 0" ), &ok );

    d = 1; m = 1; y = 0;
    cout << "EthiopianDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    EthiopianDate eth110( d, m, y );
    TESTCHECK( eth110.Valid( ), true, &ok );
    TESTCHECK( (eth110 < ethJD), true, &ok );
    TESTCHECK( (eth110 < ethToday), true, &ok );
    TESTCHECK( eth110.JulianDay( ), 1723856, &ok );
    TESTCHECK( eth110.ToString( ), string( "Maksanyo, 1 Maskaram 0" ), &ok );

    cout << "EthiopianDate( eth0 ) [copy constructor]" << endl;
    EthiopianDate ethEq = eth0;
    TESTCHECK( ethEq.Valid( ), true, &ok );
    TESTCHECK( (ethEq == eth0), true, &ok );
    TESTCHECK( ethEq.ToString( ), string( "Maksanyo, 5 Paguemen 0" ), &ok );
    cout << "= eth110 [assignment]" << endl;
    ethEq = eth110;
    TESTCHECK( ethEq.Valid( ), true, &ok );
    TESTCHECK( (ethEq == eth0), false, &ok );
    TESTCHECK( (ethEq == eth110), true, &ok );
    TESTCHECK( eth110.ToString( ), string( "Maksanyo, 1 Maskaram 0" ), &ok );

    cout << "EthiopianDate( GregorianDate( 27, 8, 8 ) ) [DateDMY constructor]" << endl;
    EthiopianDate ethGreg( GregorianDate( 27, 8, 8 ) );
    TESTCHECK( ethGreg.Valid( ), true, &ok );
    TESTCHECK( ethGreg.ToString( ), string( "Rob, 1 Maskaram 1" ), &ok );
    cout << "= GregorianDate( 12, 11, 1945 )" << endl;
    ethGreg = GregorianDate( 12, 11, 1945 );
    TESTCHECK( ethGreg.Valid( ), true, &ok );
    TESTCHECK( ethGreg.ToString( ), string( "Sanyo, 3 Hedar 1938" ), &ok );
    
    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
            = {
                { 1507232,   6, 12, -594 },
                { 1660038,  12,  4, -175 },
                { 1746894,  29,  1,   63 },
                { 1770642,   5,  2,  128 },
                { 1892732,  12,  5,  462 },
                { 1931580,  23,  9,  568 },
                { 1974852,  11,  3,  687 },
                { 2091165,  24,  8, 1005 },
                { 2121510,  23,  9, 1088 },
                { 2155780,  20,  7, 1182 },
                { 2174030,   7,  7, 1232 },
                { 2191585,  30,  7, 1280 },
                { 2195262,  25,  8, 1290 },
                { 2229275,  10, 10, 1383 },
                { 2245581,  29,  5, 1428 },
                { 2266101,   5,  8, 1484 },
                { 2288543,  12,  1, 1546 },
                { 2290902,  29,  6, 1552 },
                { 2323141,   6, 10, 1640 },
                { 2334849,  26, 10, 1672 },
                { 2348021,  19, 11, 1708 },
                { 2366979,  14, 10, 1760 },
                { 2385649,  27, 11, 1811 },
                { 2392826,  19,  7, 1831 },
                { 2416224,  11,  8, 1895 },
                { 2425849,  19, 12, 1921 },
                { 2430267,  19,  1, 1934 },
                { 2430834,  11,  8, 1935 },
                { 2431005,  26,  1, 1936 },
                { 2448699,   8,  7, 1984 },
                { 2450139,  17,  6, 1988 },
                { 2465738,   1,  3, 2031 },
                { 2486077,  11, 11, 2086 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        ethDate.Set( d, m, y );
        TESTCHECK( ethDate.Valid( ), true, &ok );
        TESTCHECK( ethDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        ethDate.Set( jd );
        TESTCHECK( ethDate.Valid( ), true, &ok );
        TESTCHECK( ethDate.Day( ), d, &ok );
        TESTCHECK( ethDate.Month( ), m, &ok );
        TESTCHECK( ethDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "EthiopianDate PASSED." << endl << endl;
    else
        cout << "EthiopianDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

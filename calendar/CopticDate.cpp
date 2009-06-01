/*
  CopticDate.cpp
  Copyright (C) 2007 David M. Anderson

  CopticDate class, representing dates in the Coptic calendar.
*/


#include "CopticDate.hpp"
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
TestCopticDate( )
{
    bool ok = true;
    cout << "Testing CopticDate" << endl;

    cout << "CopticDate() [default constructor]" << endl;
    CopticDate coptDate;
    TESTCHECK( coptDate.Valid( ), true, &ok );
    TESTCHECK( coptDate.Day( ), 1, &ok );
    TESTCHECK( coptDate.Month( ), 1, &ok );
    TESTCHECK( coptDate.Year( ), 1, &ok );
    TESTCHECK( coptDate.JulianDay( ), 1825030, &ok );
    TESTCHECK( coptDate.ToString( ), string( "Psoou, 1 Thoout 1" ), &ok );

    int d = 3;
    int m = 3;
    int y = 1662;
    cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
    coptDate.Set( d, m, y );
    TESTCHECK( coptDate.Valid( ), true, &ok );
    TESTCHECK( coptDate.Day( ), d, &ok );
    TESTCHECK( coptDate.Month( ), m, &ok );
    TESTCHECK( coptDate.Year( ), y, &ok );
    TESTCHECK( coptDate.JulianDay( ), 2431772, &ok );
    TESTCHECK( coptDate.ToString( ), string( "Pesnau, 3 Athor 1662" ), &ok );
    int incr = 40;
    cout << "Increment(" << incr << ") :" << endl;
    coptDate.Increment( incr );
    TESTCHECK( coptDate.JulianDay( ), 2431812, &ok );
    TESTCHECK( coptDate.ToString( ), string( "Psabbaton, 13 Koiak 1662" ), &ok );
    incr = 13;
    cout << "Increment( 0, " << incr << ", 0) :" << endl;
    coptDate.Increment( 0, incr, 0 );
    TESTCHECK( coptDate.JulianDay( ), 2432177, &ok );
    TESTCHECK( coptDate.ToString( ), string( "Tkyriake, 13 Koiak 1663" ), &ok );

    cout << "CopticDate( true ) [today constructor]" << endl;
    CopticDate coptToday( true );
    TESTCHECK( coptToday.Valid( ), true, &ok );
    cout << "coptToday.JulianDay( )=" << coptToday.JulianDay( );
    if ( (coptToday.JulianDay( ) > 2451545) && (coptToday.JulianDay( ) < 2500000) )
        cout << "\tOK" << endl;
    else
    {
        cout << "\tFAILED" << endl;
        ok = false;
    }
    TESTCHECK( (coptDate == coptToday), false, &ok );
    TESTCHECK( (coptDate < coptToday), true, &ok );
    cout << "coptToday.ToString()=" << coptToday.ToString( ) << endl;

    int jd = 2431772;
    cout << "CopticDate( " << jd << " ) [Julian Day constructor]" << endl;
    CopticDate coptJD( jd );
    TESTCHECK( coptJD.Valid( ), true, &ok );
    TESTCHECK( (coptJD == coptJD), true, &ok );
    TESTCHECK( (coptJD < coptToday), true, &ok );
    TESTCHECK( (coptJD == coptDate), false, &ok );
    TESTCHECK( coptJD.ToString( ), string( "Pesnau, 3 Athor 1662" ), &ok );

    d = 4;
    m = 3;
    y = 1662;
    cout << "CopticDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    CopticDate coptDMY( d, m, y );
    TESTCHECK( coptDMY.Valid( ), true, &ok );
    TESTCHECK( (coptJD < coptDMY), true, &ok );
    TESTCHECK( (coptDMY < coptToday), true, &ok );
    TESTCHECK( coptDMY.JulianDay( ), 2431773, &ok );
    TESTCHECK( coptDMY.ToString( ), string( "Pshoment, 4 Athor 1662" ), &ok );

    jd = 1825029;
    cout << "CopticDate( " << jd << " ) [Julian Day constructor]" << endl;
    CopticDate copt0( jd );
    TESTCHECK( copt0.Valid( ), true, &ok );
    TESTCHECK( (copt0 == copt0), true, &ok );
    TESTCHECK( (copt0 < coptDate), true, &ok );
    TESTCHECK( (copt0 == coptDate), false, &ok );
    TESTCHECK( copt0.ToString( ), string( "Ptiou, 5 Epagomene 0" ), &ok );

    d = 1; m = 1; y = 0;
    cout << "CopticDate( " << d << ", " << m << ", " << y
         << " ) [D,M,Y constructor]" << endl;
    CopticDate copt110( d, m, y );
    TESTCHECK( copt110.Valid( ), true, &ok );
    TESTCHECK( (copt110 < coptJD), true, &ok );
    TESTCHECK( (copt110 < coptToday), true, &ok );
    TESTCHECK( copt110.JulianDay( ), 1824665, &ok );
    TESTCHECK( copt110.ToString( ), string( "Ptiou, 1 Thoout 0" ), &ok );

    cout << "CopticDate( copt0 ) [copy constructor]" << endl;
    CopticDate coptEq( copt0 );
    TESTCHECK( coptEq.Valid( ), true, &ok );
    TESTCHECK( (coptEq == copt0), true, &ok );
    TESTCHECK( coptEq.ToString( ), string( "Ptiou, 5 Epagomene 0" ), &ok );
    cout << "= copt110 [assignment]" << endl;
    coptEq = copt110;
    TESTCHECK( coptEq.Valid( ), true, &ok );
    TESTCHECK( (coptEq == copt0), false, &ok );
    TESTCHECK( (coptEq == copt110), true, &ok );
    TESTCHECK( copt110.ToString( ), string( "Ptiou, 1 Thoout 0" ), &ok );

    cout << "CopticDate( GregorianDate( 29, 8, 284 ) ) [DateDMY constructor]" << endl;
    CopticDate coptGreg( GregorianDate( 29, 8, 284 ) );
    TESTCHECK( coptGreg.Valid( ), true, &ok );
    TESTCHECK( coptGreg.ToString( ), string( "Psoou, 1 Thoout 1" ), &ok );
    cout << "= GregorianDate( 12, 11, 1945 )" << endl;
    coptGreg = GregorianDate( 12, 11, 1945 );
    TESTCHECK( coptGreg.Valid( ), true, &ok );
    TESTCHECK( coptGreg.ToString( ), string( "Pesnau, 3 Athor 1662" ), &ok );
    
    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
            = {
                { 1507232,   6, 12, -870 },
                { 1660038,  12,  4, -451 },
                { 1746894,  29,  1, -213 },
                { 1770642,   5,  2, -148 },
                { 1892732,  12,  5,  186 },
                { 1931580,  23,  9,  292 },
                { 1974852,  11,  3,  411 },
                { 2091165,  24,  8,  729 },
                { 2121510,  23,  9,  812 },
                { 2155780,  20,  7,  906 },
                { 2174030,   7,  7,  956 },
                { 2191585,  30,  7, 1004 },
                { 2195262,  25,  8, 1014 },
                { 2229275,  10, 10, 1107 },
                { 2245581,  29,  5, 1152 },
                { 2266101,   5,  8, 1208 },
                { 2288543,  12,  1, 1270 },
                { 2290902,  29,  6, 1276 },
                { 2323141,   6, 10, 1364 },
                { 2334849,  26, 10, 1396 },
                { 2348021,  19, 11, 1432 },
                { 2366979,  14, 10, 1484 },
                { 2385649,  27, 11, 1535 },
                { 2392826,  19,  7, 1555 },
                { 2416224,  11,  8, 1619 },
                { 2425849,  19, 12, 1645 },
                { 2430267,  19,  1, 1658 },
                { 2430834,  11,  8, 1659 },
                { 2431005,  26,  1, 1660 },
                { 2448699,   8,  7, 1708 },
                { 2450139,  17,  6, 1712 },
                { 2465738,   1,  3, 1755 },
                { 2486077,  11, 11, 1810 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        coptDate.Set( d, m, y );
        TESTCHECK( coptDate.Valid( ), true, &ok );
        TESTCHECK( coptDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        coptDate.Set( jd );
        TESTCHECK( coptDate.Valid( ), true, &ok );
        TESTCHECK( coptDate.Day( ), d, &ok );
        TESTCHECK( coptDate.Month( ), m, &ok );
        TESTCHECK( coptDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "CopticDate PASSED." << endl << endl;
    else
        cout << "CopticDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

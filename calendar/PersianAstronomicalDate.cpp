/*
  PersianAstronomicalDate.cpp
  Copyright (C) 2007 David M. Anderson

  PersianAstronomicalDate class, representing a date in the PersianAstronomical calendar.
*/


#include "PersianAstronomicalDate.hpp"
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
TestPersianAstronomicalDate( )
{
    bool ok = true;
    cout << "Testing PersianAstronomicalDate" << endl;

    int jd;
    int d, m, y;
    PersianAstronomicalDate persDate;

    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
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

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
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
        cout << "PersianAstronomicalDate PASSED." << endl << endl;
    else
        cout << "PersianAstronomicalDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

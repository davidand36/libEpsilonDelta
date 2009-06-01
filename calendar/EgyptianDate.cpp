/*
  EgyptianDate.cpp
  Copyright (C) 2007 David M. Anderson

  EgyptianDate class, representing dates in the Egyptian calendar.
*/


#include "EgyptianDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool
TestEgyptianDate( )
{
    bool ok = true;
    cout << "Testing EgyptianDate" << endl;

    int jd;
    int d, m, y;
    EgyptianDate egyptDate;

    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
            = {
                { 1507232,  15,  7,  161 },
                { 1660038,   6,  3,  580 },
                { 1746894,  22,  2,  818 },
                { 1770642,  15,  3,  883 },
                { 1892732,  15,  9, 1217 },
                { 1931580,  18,  2, 1324 },
                { 1974852,  10,  9, 1442 },
                { 2091165,   8,  5, 1761 },
                { 2121510,  28,  6, 1844 },
                { 2155780,  18,  5, 1938 },
                { 2174030,  18,  5, 1988 },
                { 2191585,  23,  6, 2036 },
                { 2195262,  20,  7, 2046 },
                { 2229275,  28,  9, 2139 },
                { 2245581,  29,  5, 2184 },
                { 2266101,  19,  8, 2240 },
                { 2288543,  11,  2, 2302 },
                { 2290902,  30,  7, 2308 },
                { 2323141,  29, 11, 2396 },
                { 2334849,  27, 12, 2428 },
                { 2348021,  24,  1, 2465 },
                { 2366979,   2,  1, 2517 },
                { 2385649,  27,  2, 2568 },
                { 2392826,  29, 10, 2587 },
                { 2416224,   7, 12, 2651 },
                { 2425849,  17,  4, 2678 },
                { 2430267,  25,  5, 2690 },
                { 2430834,  17, 12, 2691 },
                { 2431005,   3,  6, 2692 },
                { 2448699,  27, 11, 2740 },
                { 2450139,   7, 11, 2744 },
                { 2465738,   1,  8, 2787 },
                { 2486077,  20,  4, 2843 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        egyptDate.Set( d, m, y );
        TESTCHECK( egyptDate.Valid( ), true, &ok );
        TESTCHECK( egyptDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        egyptDate.Set( jd );
        TESTCHECK( egyptDate.Valid( ), true, &ok );
        TESTCHECK( egyptDate.Day( ), d, &ok );
        TESTCHECK( egyptDate.Month( ), m, &ok );
        TESTCHECK( egyptDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "EgyptianDate PASSED." << endl << endl;
    else
        cout << "EgyptianDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

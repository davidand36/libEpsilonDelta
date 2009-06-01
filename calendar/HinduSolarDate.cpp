/*
  HinduSolarDate.cpp
  Copyright (C) 2007 David M. Anderson

  HinduSolarDate class, representing a date in the Hindu solar calendar.
*/


#include "HinduSolarDate.hpp"
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
TestHinduSolarDate( )
{
    bool ok = true;
    cout << "Testing HinduSolarDate" << endl;

    int jd;
    int d, m, y;
    HinduSolarDate hinduSolarDate;

    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
            = {
                { 1507232,  19,  5, -664 },
                { 1660038,  26,  9, -246 },
                { 1746894,   9,  7,   -8 },
                { 1770642,  16,  7,   57 },
                { 1892732,  21, 10,  391 },
                { 1931580,  31,  2,  498 },
                { 1974852,  16,  8,  616 },
                { 2091165,  28,  1,  935 },
                { 2121510,  26,  2, 1018 },
                { 2155780,  23, 12, 1111 },
                { 2174030,  10, 12, 1161 },
                { 2191585,   2,  1, 1210 },
                { 2195262,  27,  1, 1220 },
                { 2229275,   8,  3, 1313 },
                { 2245581,  30, 10, 1357 },
                { 2266101,   5,  1, 1414 },
                { 2288543,  10,  6, 1475 },
                { 2290902,  29, 11, 1481 },
                { 2323141,   3,  3, 1570 },
                { 2334849,  22,  3, 1602 },
                { 2348021,  13,  4, 1638 },
                { 2366979,  10,  3, 1690 },
                { 2385649,  20,  4, 1741 },
                { 2392826,  16, 12, 1760 },
                { 2416224,   7,  1, 1825 },
                { 2425849,  10,  5, 1851 },
                { 2430267,  14,  6, 1863 },
                { 2430834,   7,  1, 1865 },
                { 2431005,  21,  6, 1865 },
                { 2448699,   4, 12, 1913 },
                { 2450139,  13, 11, 1917 },
                { 2465738,  24,  7, 1960 },
                { 2486077,   2,  4, 2016 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        hinduSolarDate.Set( d, m, y );
        TESTCHECK( hinduSolarDate.Valid( ), true, &ok );
        TESTCHECK( hinduSolarDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        hinduSolarDate.Set( jd );
        TESTCHECK( hinduSolarDate.Valid( ), true, &ok );
        TESTCHECK( hinduSolarDate.Day( ), d, &ok );
        TESTCHECK( hinduSolarDate.Month( ), m, &ok );
        TESTCHECK( hinduSolarDate.Year( ), y, &ok );
    }


    if ( ok )
        cout << "HinduSolarDate PASSED." << endl << endl;
    else
        cout << "HinduSolarDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

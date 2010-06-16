/*
  ArmenianDate.cpp
  Copyright (C) 2007 David M. Anderson

  ArmenianDate class, representing dates in the Armenian calendar.
*/


#include "ArmenianDate.hpp"
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
TestArmenianDate( )
{
    bool ok = true;
    cout << "Testing ArmenianDate" << endl;


    long jd;
    int d, m;
    long y;
    ArmenianDate armDate;

    struct
    {
        long julianDay;
        int day;
        int month;
        long year;
    } 
    testDates[]
            = {
                { 1507232,  10,  4,-1138 },
                { 1660038,   6, 12, -720 },
                { 1746894,  22, 11, -482 },
                { 1770642,  15, 12, -417 },
                { 1892732,  10,  6,  -82 },
                { 1931580,  18, 11,   24 },
                { 1974852,   5,  6,  143 },
                { 2091165,   3,  2,  462 },
                { 2121510,  23,  3,  545 },
                { 2155780,  13,  2,  639 },
                { 2174030,  13,  2,  689 },
                { 2191585,  18,  3,  737 },
                { 2195262,  15,  4,  747 },
                { 2229275,  23,  6,  840 },
                { 2245581,  24,  2,  885 },
                { 2266101,  14,  5,  941 },
                { 2288543,  11, 11, 1002 },
                { 2290902,  25,  4, 1009 },
                { 2323141,  24,  8, 1097 },
                { 2334849,  22,  9, 1129 },
                { 2348021,  24, 10, 1165 },
                { 2366979,   2, 10, 1217 },
                { 2385649,  27, 11, 1268 },
                { 2392826,  24,  7, 1288 },
                { 2416224,   2,  9, 1352 },
                { 2425849,  12,  1, 1379 },
                { 2430267,  20,  2, 1391 },
                { 2430834,  12,  9, 1392 },
                { 2431005,  28,  2, 1393 },
                { 2448699,  22,  8, 1441 },
                { 2450139,   2,  8, 1445 },
                { 2465738,  26,  4, 1488 },
                { 2486077,  15,  1, 1544 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        armDate.Set( d, m, y );
        TESTCHECK( armDate.Valid( ), true, &ok );
        TESTCHECK( armDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        armDate.Set( jd );
        TESTCHECK( armDate.Valid( ), true, &ok );
        TESTCHECK( armDate.Day( ), d, &ok );
        TESTCHECK( armDate.Month( ), m, &ok );
        TESTCHECK( armDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "ArmenianDate PASSED." << endl << endl;
    else
        cout << "ArmenianDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

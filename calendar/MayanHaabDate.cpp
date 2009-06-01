/*
  MayanHaabDate.cpp
  Copyright (C) 2007 David M. Anderson

  MayanHaabDate class, representing dates in the Mayan haab calendar.
*/


#include "MayanHaabDate.hpp"
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
TestMayanHaabDate( )
{
    bool ok = true;
    cout << "Testing MayanHaabDate" << endl;

    int jd;
    MayanHaabDate mhDate;

    struct
    {
        int julianDay;
        int day;
        int month;
    } 
    testDates[]
            = {
                { 1507232,  12, 11 },
                { 1660038,   3,  5 },
                { 1746894,   9,  4 },
                { 1770642,  12,  5 },
                { 1892732,  12, 14 },
                { 1931580,   5,  4 },
                { 1974852,   7, 14 },
                { 2091165,   5,  8 },
                { 2121510,  15, 10 },
                { 2155780,  15,  8 },
                { 2174030,  15,  8 },
                { 2191585,  10, 10 },
                { 2195262,  17, 11 },
                { 2229275,   5, 15 },
                { 2245581,   6,  9 },
                { 2266101,   6, 13 },
                { 2288543,  18,  3 },
                { 2290902,   7, 12 },
                { 2323141,   6, 18 },
                { 2334849,   9,  1 },
                { 2348021,   1,  3 },
                { 2366979,  19,  1 },
                { 2385649,  14,  4 },
                { 2392826,  16, 16 },
                { 2416224,  14, 18 },
                { 2425849,   4,  7 },
                { 2430267,   2,  9 },
                { 2430834,   4, 19 },
                { 2431005,  10,  9 },
                { 2448699,   4, 18 },
                { 2450139,   4, 17 },
                { 2465738,   8, 12 },
                { 2486077,   7,  7 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        cout << "Set( " << jd << " ) :" << endl;
        mhDate.Set( jd );
        TESTCHECK( mhDate.Valid( ), true, &ok );
        TESTCHECK( mhDate.Day(), testDates[i].day, &ok );
        TESTCHECK( mhDate.Month(), testDates[i].month, &ok );
    }

    if ( ok )
        cout << "MayanHaabDate PASSED." << endl << endl;
    else
        cout << "MayanHaabDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

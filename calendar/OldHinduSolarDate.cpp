/*
  OldHinduSolarDate.cpp
  Copyright (C) 2007 David M. Anderson

  OldHinduSolarDate class, representing a date in the old Hindu solar calendar.
*/


#include "OldHinduSolarDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


// Specialized because the year in the Hindu calendar represents the number of
// years elapsed since the Kali Yuga epoch, i.e. years begin with 0.

template<>
void 
DateDMY< OldHinduSolarCalendar >::Set( bool today )
{
    if ( today )
    {
        DateJD::Set( true );
        Assert( DateJD::Valid() );
        OldHinduSolarCalendar::JulianDayToDMY( JulianDay(),
                                               &m_day, &m_month, &m_year );
    }
    else
    {
        DateJD::Set( false );
        Set( 1, 1, 0 );
    }
}


//=============================================================================


#ifdef DEBUG

bool
TestOldHinduSolarDate( )
{
    bool ok = true;
    cout << "Testing OldHinduSolarDate" << endl;

    int jd;
    int d, m, y;
    OldHinduSolarDate ohsDate;

    struct
    {
        int julianDay;
        int day;
        int month;
        int year;
    } 
    testDates[]
            = {
                { 1507232,  19,  5, 2515 },
                { 1660038,  26,  9, 2933 },
                { 1746894,  11,  7, 3171 },
                { 1770642,  17,  7, 3236 },
                { 1892732,  19, 10, 3570 },
                { 1931580,  28,  2, 3677 },
                { 1974852,  17,  8, 3795 },
                { 2091165,  26,  1, 4114 },
                { 2121510,  24,  2, 4197 },
                { 2155780,  20, 12, 4290 },
                { 2174030,   7, 12, 4340 },
                { 2191585,  30, 12, 4388 },
                { 2195262,  24,  1, 4399 },
                { 2229275,   7,  3, 4492 },
                { 2245581,  28, 10, 4536 },
                { 2266101,   3,  1, 4593 },
                { 2288543,  12,  6, 4654 },
                { 2290902,  27, 11, 4660 },
                { 2323141,   1,  3, 4749 },
                { 2334849,  21,  3, 4781 },
                { 2348021,  13,  4, 4817 },
                { 2366979,   8,  3, 4869 },
                { 2385649,  20,  4, 4920 },
                { 2392826,  13, 12, 4939 },
                { 2416224,   4,  1, 5004 },
                { 2425849,  11,  5, 5030 },
                { 2430267,  15,  6, 5042 },
                { 2430834,   4,  1, 5044 },
                { 2431005,  23,  6, 5044 },
                { 2448699,   2, 12, 5092 },
                { 2450139,  11, 11, 5096 },
                { 2465738,  26,  7, 5139 },
                { 2486077,   2,  4, 5195 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m << ", " << y << " ) :" << endl;
        ohsDate.Set( d, m, y );
        TESTCHECK( ohsDate.Valid( ), true, &ok );
        TESTCHECK( ohsDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        ohsDate.Set( jd );
        TESTCHECK( ohsDate.Valid( ), true, &ok );
        TESTCHECK( ohsDate.Day( ), d, &ok );
        TESTCHECK( ohsDate.Month( ), m, &ok );
        TESTCHECK( ohsDate.Year( ), y, &ok );
    }


    if ( ok )
        cout << "OldHinduSolarDate PASSED." << endl << endl;
    else
        cout << "OldHinduSolarDate FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

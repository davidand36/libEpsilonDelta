/*
  HinduSolarDate.cpp
  Copyright (C) 2007 David M. Anderson

  HinduSolarDate class, representing a date in the Hindu solar calendar.
*/


#include "HinduSolarDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "Array.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


// Specialized because the year in the Hindu calendar represents the number of
// years elapsed since the Kali Yuga epoch, i.e. years begin with 0.

template<>
void 
DateDMY< HinduSolarCalendar >::Set( bool today )
{
    if ( today )
    {
        DateJD::Set( true );
        Assert( DateJD::Valid() );
        HinduSolarCalendar::JulianDayToDMY( JulianDay(),
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
TestHinduSolarDate( )
{
    bool ok = true;
    cout << "Testing HinduSolarDate" << endl;

    long jd;
    int d, m;
    long y;
    HinduSolarDate hinduSolarDate;

    cout << "Modern:" << endl;
    
    struct
    {
        long julianDay;
        int day;
        int month;
        long year;
    } 
    modernTestDates[]
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

    for ( int i = 0; i < ARRAY_LENGTH( modernTestDates ); ++i )
    {
        jd = modernTestDates[i].julianDay;
        d = modernTestDates[i].day;
        m = modernTestDates[i].month;
        y = modernTestDates[i].year;
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

    cout << "Old:" << endl;
    HinduSolarCalendar::SetVersion( HinduSolarCalendar::Old );
    
    struct
    {
        long julianDay;
        int day;
        int month;
        long year;
    } 
    oldTestDates[]
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

    for ( int i = 0; i < ARRAY_LENGTH( oldTestDates ); ++i )
    {
        jd = oldTestDates[i].julianDay;
        d = oldTestDates[i].day;
        m = oldTestDates[i].month;
        y = oldTestDates[i].year;
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

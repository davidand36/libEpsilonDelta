/*
  PersianArithmeticCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class PersianArithmeticCalendar, which defines the Persian calendar,
  using an arithmetic intercalation scheme.
*/


#include "PersianArithmeticCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#include <cmath>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


static const int s_persianEpoch = 1948321;


//=============================================================================


void
PersianArithmeticCalendar::JulianDayToDMY( int julianDay,
                                 int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 73.*/
    const int jd475 = 2121446; /*DMYToJulianDay( 1, 1, 475 )*/
    int d0 = julianDay - jd475;
    int n = DivF( d0, 1029983 );
    int d1 = ModF( d0, 1029983 );
    int y;
    if ( d1 == 1029982 )
        y = 2820;
    else
    {
        int a, b;
        DivModF( d1, 366, &a, &b );
        y = 1 + a + DivF( (2134 * a + 2816 * b + 2815), 1028522 );
    }
    int year = y + 2820 * n + 474;
    int dy = julianDay - DMYToJulianDay( 1, 1, year ) + 1;
    int month = (dy <= 186) ? DivC( dy, 31 ) : DivC( (dy - 6), 30 );
    int day = julianDay - DMYToJulianDay( 1, month, year ) + 1;
    *pDay = day;
    *pMonth = month;
    *pYear = year;
}

//-----------------------------------------------------------------------------

int
PersianArithmeticCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 72.*/
    int y = year - 474;
    int yr = ModF( y, 2820 ) + 474;
    int md = (month <= 7) ? (31 * (month - 1)) : ((30 * (month - 1)) + 6);
    return (s_persianEpoch - 1 + 365 * (yr - 1) + 1029983 * DivF( y, 2820 )
            + DivF( (682 * yr - 110), 2816 ) + md + day);
}

//=============================================================================

int
PersianArithmeticCalendar::DaysInMonth( int month, int year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    static const int daysInMonth[ 12 ]
         = { 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29 };
    if ( (month != 12) || ! IsLeapYear( year ) )
        return daysInMonth[ month - 1 ];
    return 30;
}

//-----------------------------------------------------------------------------

const string &
PersianArithmeticCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= 12) );
    return g_persianMonthNames[ month - 1 ];
}

//=============================================================================

bool
PersianArithmeticCalendar::IsLeapYear( int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 71.*/
    int y = year - 474;
    int yy = ModF( y, 2820 ) + 474;
    return ( ModF( ((yy + 38) * 682), 2816) < 682 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  HebrewCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class HebrewCalendar, which defines the Hebrew calendar.
*/


#include "HebrewCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#include <cmath>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace
{

const int s_hebrewEpoch = 347998;

int DaysElapsed( int year );

}


//=============================================================================


void
HebrewCalendar::JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 94.*/
    int approx = static_cast<int>( DivRF(
        static_cast<double>( julianDay - s_hebrewEpoch ),
        (35975351. / 98496.) ) );
    int year = approx - 1;
    //Year begins with month 7 (Tishri)
    for ( int y = approx; DMYToJulianDay( 1, 7, y ) <= julianDay; ++y )
        ++year;
    int start;
    if ( julianDay < DMYToJulianDay( 1, 1, year ) )
        start = 7;
    else
        start = 1;
    int month = start;
    for ( int m = start;
          DMYToJulianDay( DaysInMonth( m, year ), m, year ) < julianDay; ++m )
        ++month;
    int day = julianDay - DMYToJulianDay( 1, month, year ) + 1;
    *pDay = day;
    *pMonth = month;
    *pYear = year;
}

//-----------------------------------------------------------------------------

int
HebrewCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 91-93.*/
    int elapsedM1 = DaysElapsed( year - 1 );
    int elapsed = DaysElapsed( year );
    int elapsedP1 = DaysElapsed( year + 1 );
    int newYearDelay = 0;
    if ( elapsedP1 - elapsed == 356 )
        newYearDelay = 2;
    else if ( elapsed - elapsedM1 == 382 )
        newYearDelay = 1;
    int jd = s_hebrewEpoch + elapsed + newYearDelay + day - 1;
    if ( month < 7 )  //Year begins with month 7 (Tishri).
    {
        int monthsInYear = MonthsInYear( year );
        for ( int m = 7; m <= monthsInYear; ++m )
            jd += DaysInMonth( m, year );
        for ( int m = 1; m < month; ++m )
            jd += DaysInMonth( m, year );
    }
    else
    {
        for ( int m = 7; m < month; ++m )
            jd += DaysInMonth( m, year );
    }
    return jd;
}

//=============================================================================

namespace
{

int
DaysElapsed( int year )
{
    int monthsElapsed = DivF( (235 * year - 234), 19 );
    int partsElapsed = 204 + 793 * ModF( monthsElapsed, 1080 );
    //In Hebrew time, there are 1080 parts per hour. (1 part = 3-1/3 sec.)
    int hoursElapsed = 11 + 12 * monthsElapsed
        + 793 * DivF( monthsElapsed, 1080 ) + DivF( partsElapsed, 1080 );
    int day = 29 * monthsElapsed + DivF( hoursElapsed, 24 );
    if ( ModF( 3 * (day + 1), 7 ) < 3 )
        return day + 1;
    return day;
}

} //namespace

//=============================================================================

int
HebrewCalendar::MonthsInYear( int year )
{
    return ( IsLeapYear( year ) ? 13 : 12 );
}

//-----------------------------------------------------------------------------

int
HebrewCalendar::DaysInMonth( int month, int year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    static const int daysInMonth[ 13 ]
        = { 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 29 };
    if ( month == Heshvan )
    {
        int daysInYear = DMYToJulianDay( 1, 7, year + 1 )
            - DMYToJulianDay( 1, 7, year );
        if ( ModF( daysInYear, 10 ) == 5 )
            return 30;
        return 29;
    }
    else if ( month == Kislev )
    {
        int daysInYear = DMYToJulianDay( 1, 7, year + 1 )
            - DMYToJulianDay( 1, 7, year );
        if ( ModF( daysInYear, 10 ) == 3 )
            return 29;
        return 30;
    }
    else if ( month == Adar )
    {
        if ( IsLeapYear( year ) )
            return 30;
        return 29;
    }
    else
        return daysInMonth[ month - 1 ];
}

//-----------------------------------------------------------------------------

const string &
HebrewCalendar::MonthName( int month, int year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    if ( IsLeapYear( year ) )
        return g_hebrewMonthNamesLY[ month - 1 ];
    else
        return g_hebrewMonthNamesCY[ month - 1 ];
}

//-----------------------------------------------------------------------------

bool
HebrewCalendar::IsLeapYear( int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 86.*/
    return ( ModF( (7 * year + 1), 19 ) < 7 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

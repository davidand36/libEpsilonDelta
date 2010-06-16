/*
  JulianCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class JulianCalendar, which defines the Julian calendar.
*/


#include "JulianCalendar.hpp"
#include "Assert.hpp"
#include "TimeLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void
JulianCalendar::JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear )
{
    /*Adapted from Jean Meeus, "Astronomical Algorithms", p. 63.*/
    long b = julianDay + 1524;
    long c = (long)( (b - 122.1) / 365.25 );
    long d = (long)( c * 365.25 );
    long e = (long)( (b - d) / 30.6001 );
    int day = (int)(b - d - (long)( e * 30.6001 ));
    int month;
    if ( e < 14 )
        month = e - 1;
    else
        month = e - 13;
    long year;
    if ( month > 2 )
        year = c - 4716;
    else
        year = c - 4715;
    *pDay = day;
    *pMonth = month;
    *pYear = year;
}

//-----------------------------------------------------------------------------

long
JulianCalendar::DMYToJulianDay( int day, int month, long year )
{
    /*Adapted from Jean Meeus, "Astronomical Algorithms", p. 63.*/
    if ( month <= 2 )
    {
        month += 12;
        --year;
    }
    return ( (long)( 365.25 * (year + 4716) )
             + (long)( 30.6001 * (month + 1) )
             + day - 1524 );
}

//=============================================================================

int
JulianCalendar::DaysInMonth( int month, long year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    static const int daysInMonth[ 12 ]
        = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if ( (month != 2) || ! IsLeapYear( year ) )
        return daysInMonth[ month - 1 ];
    return 29;
}

//-----------------------------------------------------------------------------

const string &
JulianCalendar::MonthName( int month, long /*year*/ )
{
    return MonthName( month );
}

//-----------------------------------------------------------------------------

const string &
JulianCalendar::MonthName( int month )
{
    Assert( (month > 0) && (month <= 12) );
    return g_westernMonthNames[ month - 1 ];
}

//=============================================================================

bool
JulianCalendar::IsLeapYear( long year )
{
    return ( year % 4 == 0 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

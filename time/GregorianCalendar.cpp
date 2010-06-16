/*
  GregorianCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class GregorianCalendar, which defines the Gregorian calendar.
*/


#include "GregorianCalendar.hpp"
#include "TimeLibText.hpp"
#include "Assert.hpp"
#include <ctime>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void
GregorianCalendar::JulianDayToDMY( long julianDay,
                                   int * pDay, int * pMonth, long * pYear )
{
    /*Adapted from Jean Meeus, "Astronomical Algorithms", p. 63.*/
    long alpha = (long)( (julianDay - 1867216.25) / 36524.25 );
    long a = julianDay + 1 + alpha - alpha / 4;
    long b = a + 1524;
    long c = (long)( (b - 122.1) / 365.25 );
    long d = (long)( c * 365.25 );
    long e = (long)( (b - d) / 30.6001 );
    int day = (int)(b - d - (long)( e * 30.6001 ));
    int month;
    if ( e < 14 )
        month = (int)(e - 1);
    else
        month = (int)(e - 13);
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
GregorianCalendar::DMYToJulianDay( int day, int month, long year )
{
    /*Adapted from Jean Meeus, "Astronomical Algorithms", p. 63.*/
    if ( month <= 2 )
    {
        month += 12;
        --year;
    }
    long a = year / 100;
    long b = 2 - a + a / 4;
    return ( (long)( 365.25 * (year + 4716) )
             + (long)( 30.6001 * (month + 1) )
             + day + b - 1524 );
}

//=============================================================================

int
GregorianCalendar::DaysInMonth( int month, long year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    static const int daysInMonth[ 12 ]
        = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    if ( (month == 2) && IsLeapYear( year ) )
        return 29;
    return daysInMonth[ month - 1 ];
}

//-----------------------------------------------------------------------------

const string &
GregorianCalendar::MonthName( int month, long /*year*/ )
{
    return MonthName( month );
}

//-----------------------------------------------------------------------------

const string &
GregorianCalendar::MonthName( int month )
{
    Assert( (month > 0) && (month <= 12) );
    return g_westernMonthNames[ month - 1 ];
}

//=============================================================================

bool
GregorianCalendar::IsLeapYear( long year )
{
    if ( (year & 3) == 0 )    //fast implementation of year % 4
        if ( (year % 100) == 0 )
            if ( (year % 400) == 0 )
                return true;
            else
                return false;
        else
            return true;
    else
        return false;
}

//=============================================================================

void
GregorianCalendar::Today( int * pDay, int * pMonth, long * pYear )
{
    std::time_t t = std::time( 0 );
    Assert( t != time_t( -1 ) );
    std::tm * pNow = std::localtime( &t );
    *pDay = pNow->tm_mday;
    *pMonth = pNow->tm_mon + 1;
    *pYear = pNow->tm_year + 1900;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

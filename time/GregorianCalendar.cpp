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
GregorianCalendar::JulianDayToDMY( int julianDay,
                                   int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Jean Meeus, "Astronomical Algorithms", p. 63.*/
    int alpha = static_cast<int>( (julianDay - 1867216.25) / 36524.25 );
    int a = julianDay + 1 + alpha - alpha / 4;
    int b = a + 1524;
    int c = static_cast<int>( (b - 122.1) / 365.25 );
    int d = static_cast<int>( c * 365.25 );
    int e = static_cast<int>( (b - d) / 30.6001 );
    int day = b - d - static_cast<int>( e * 30.6001 );
    int month;
    if ( e < 14 )
        month = e - 1;
    else
        month = e - 13;
    int year;
    if ( month > 2 )
        year = c - 4716;
    else
        year = c - 4715;
    *pDay = day;
    *pMonth = month;
    *pYear = year;
}

//-----------------------------------------------------------------------------

int
GregorianCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Jean Meeus, "Astronomical Algorithms", p. 63.*/
    if ( month <= 2 )
    {
        month += 12;
        --year;
    }
    int a = year / 100;
    int b = 2 - a + a / 4;
    return ( static_cast<int>( 365.25 * (year + 4716) )
             + static_cast<int>( 30.6001 * (month + 1) )
             + day + b - 1524 );
}

//=============================================================================

int
GregorianCalendar::DaysInMonth( int month, int year )
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
GregorianCalendar::MonthName( int month, int /*year*/ )
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
GregorianCalendar::IsLeapYear( int year )
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
GregorianCalendar::Today( int * pDay, int * pMonth, int * pYear )
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

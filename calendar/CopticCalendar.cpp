/*
  CopticCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class CopticCalendar, which defines the Coptic calendar.
*/


#include "CopticCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


static const int s_copticEpoch = 1825030;


//=============================================================================


void
CopticCalendar::JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 58.*/
    int year = DivF( (4 * (julianDay - s_copticEpoch) + 1463), 1461 );
    int month = DivF( (julianDay - DMYToJulianDay( 1, 1, year )), 30 ) + 1;
    int day = julianDay + 1 - DMYToJulianDay( 1, month, year );
    *pDay = day;
    *pMonth = month;
    *pYear = year;
}

//-----------------------------------------------------------------------------

int
CopticCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 58.*/
    return (s_copticEpoch  +  365 * (year - 1)  +  DivF( year, 4 )
            +  30 * (month - 1)  +  day  -  1);
}

//=============================================================================

int
CopticCalendar::DaysInMonth( int month, int year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    if ( month < 13 )
        return 30;
    return (IsLeapYear( year ) ? 6 : 5);
}

//-----------------------------------------------------------------------------

const string &
CopticCalendar::MonthName( int month, int /*year*/ )
{
    return MonthName( month );
}

//-----------------------------------------------------------------------------

const string &
CopticCalendar::MonthName( int month )
{
    Assert( (month > 0) && (month <= 13) );
    return g_copticMonthNames[ month - 1 ];
}

//=============================================================================

bool
CopticCalendar::IsLeapYear( int year )
{
    return ( ModF( year, 4 ) == 3 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

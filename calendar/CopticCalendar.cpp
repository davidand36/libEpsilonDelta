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


static const long s_copticEpoch = 1825030;


//=============================================================================


void
CopticCalendar::JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 58.*/
    long year = DivF( (4 * (julianDay - s_copticEpoch) + 1463), 1461L );
    int month = (int)DivF( (julianDay - DMYToJulianDay( 1, 1, year )), 30L ) + 1;
    int day = (int)(julianDay + 1 - DMYToJulianDay( 1, month, year ));
    *pDay = day;
    *pMonth = month;
    *pYear = year;
}

//-----------------------------------------------------------------------------

long
CopticCalendar::DMYToJulianDay( int day, int month, long year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 58.*/
    return (s_copticEpoch  +  365 * (year - 1)  +  DivF( year, 4L )
            +  30 * (month - 1)  +  day  -  1);
}

//=============================================================================

int
CopticCalendar::DaysInMonth( int month, long year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    if ( month < 13 )
        return 30;
    return (IsLeapYear( year ) ? 6 : 5);
}

//-----------------------------------------------------------------------------

const string &
CopticCalendar::MonthName( int month, long /*year*/ )
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
CopticCalendar::IsLeapYear( long year )
{
    return ( ModF( year, 4L ) == 3 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

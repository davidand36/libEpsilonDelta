/*
  EthiopianCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class EthiopianCalendar, which defines the Ethiopian calendar.
*/


#include "EthiopianCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#include "CopticCalendar.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


static const long s_copticEpoch = 1825030;
static const long s_ethiopianEpoch = 1724221;


//=============================================================================


void
EthiopianCalendar::JulianDayToDMY( long julianDay,
                                   int * pDay, int * pMonth, long * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 59.*/
    CopticCalendar::JulianDayToDMY(
        (julianDay + s_copticEpoch - s_ethiopianEpoch), pDay, pMonth, pYear );
}

//-----------------------------------------------------------------------------

long
EthiopianCalendar::DMYToJulianDay( int day, int month, long year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 59.*/
    return (CopticCalendar::DMYToJulianDay( day, month, year )
            + s_ethiopianEpoch - s_copticEpoch);
}

//=============================================================================

int
EthiopianCalendar::DaysInMonth( int month, long year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    if ( month < 13 )
        return 30;
    return (IsLeapYear( year ) ? 6 : 5);
}

//-----------------------------------------------------------------------------

const string &
EthiopianCalendar::MonthName( int month, long /*year*/ )
{
    Assert( (month > 0) && (month <= 13) );
    return g_ethiopianMonthNames[ month - 1 ];
}

//=============================================================================

bool
EthiopianCalendar::IsLeapYear( long year )
{
    return ( ModF( year, 4L ) == 3 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

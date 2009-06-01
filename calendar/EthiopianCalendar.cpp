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


static const int s_copticEpoch = 1825030;
static const int s_ethiopianEpoch = 1724221;


//=============================================================================


void
EthiopianCalendar::JulianDayToDMY( int julianDay,
                                   int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 59.*/
    CopticCalendar::JulianDayToDMY(
        (julianDay + s_copticEpoch - s_ethiopianEpoch), pDay, pMonth, pYear );
}

//-----------------------------------------------------------------------------

int
EthiopianCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations", p. 59.*/
    return (CopticCalendar::DMYToJulianDay( day, month, year )
            + s_ethiopianEpoch - s_copticEpoch);
}

//=============================================================================

int
EthiopianCalendar::DaysInMonth( int month, int year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    if ( month < 13 )
        return 30;
    return (IsLeapYear( year ) ? 6 : 5);
}

//-----------------------------------------------------------------------------

const string &
EthiopianCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= 13) );
    return g_ethiopianMonthNames[ month - 1 ];
}

//=============================================================================

bool
EthiopianCalendar::IsLeapYear( int year )
{
    return ( ModF( year, 4 ) == 3 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

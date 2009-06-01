/*
  EgyptianCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class EgyptianCalendar, which defines the Egyptian calendar.
*/


#include "EgyptianCalendar.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace
{
const int s_egyptianEpoch = 1448638;
}

//=============================================================================


void
EgyptianCalendar::JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 25.*/
    int days = julianDay - s_egyptianEpoch;
    int year;
    int rem;
    DivModP( days, 365, &year, &rem );
    int month;
    int day;
    DivModP( rem, 30, &month, &day );
    *pDay = day + 1;
    *pMonth = month + 1;
    *pYear = year + 1;
}

//-----------------------------------------------------------------------------

int
EgyptianCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 25.*/
    return (s_egyptianEpoch  +  365 * (year - 1)
            +  30 * (month - 1)  +  day  -  1);
}

//=============================================================================

const string &
EgyptianCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= 13) );
    return g_egyptianMonthNames[ month - 1 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

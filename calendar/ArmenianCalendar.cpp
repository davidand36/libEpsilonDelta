/*
  ArmenianCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class ArmenianCalendar, which defines the Armenian calendar.
*/


#include "ArmenianCalendar.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


static const int s_armenianEpoch = 1922868;


//=============================================================================


void
ArmenianCalendar::JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 25.*/
    int days = julianDay - s_armenianEpoch;
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
ArmenianCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 25.*/
    return (s_armenianEpoch  +  365 * (year - 1)
            +  30 * (month - 1)  +  day  -  1);
}

//=============================================================================

const string &
ArmenianCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= 13) );
    return g_armenianMonthNames[ month - 1 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

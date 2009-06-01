/*
  MayanHaabCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  MayanHaabCalendar class, representing the Mayan haab calendar.
*/


#include "MayanHaabCalendar.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


static const int s_mayanHaabEpoch = 583935;


//=============================================================================


void 
MayanHaabCalendar::JulianDayToDMY( int julianDay, int * pDay, int * pMonth,
                                   int * pYear )
{
    int days = julianDay - s_mayanHaabEpoch;
    int year;
    int rem;
    DivModP( days, 365, &year, &rem );
    int month;
    int day;
    DivModP( rem, 20, &month, &day );
    if ( pDay )
        *pDay = day;
    if ( pMonth )
        *pMonth = month + 1;
    if ( pYear )
        *pYear = year;
}

//-----------------------------------------------------------------------------

int 
MayanHaabCalendar::DMYToJulianDay( int day, int month, int year )
{
    return  s_mayanHaabEpoch  +  year * 365  +  (month - 1) * 20  +  day;
}

//=============================================================================

const std::string & 
MayanHaabCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month >= 1) && (month <= 19) );
    return g_mayanHaabMonthNames[ month - 1 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

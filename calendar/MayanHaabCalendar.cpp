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


static const long s_mayanHaabEpoch = 583935;


//=============================================================================


void 
MayanHaabCalendar::JulianDayToDMY( long julianDay, int * pDay, int * pMonth,
                                   long * pYear )
{
    long days = julianDay - s_mayanHaabEpoch;
    long year;
    long rem;
    DivModP( days, 365L, &year, &rem );
    int month;
    int day;
    DivModP( (int)rem, 20, &month, &day );
    if ( pDay )
        *pDay = day;
    if ( pMonth )
        *pMonth = (int)(month + 1);
    if ( pYear )
        *pYear = (int)year;
}

//-----------------------------------------------------------------------------

long 
MayanHaabCalendar::DMYToJulianDay( int day, int month, long year )
{
    return  s_mayanHaabEpoch  +  year * 365  +  (month - 1) * 20  +  day;
}

//=============================================================================

const std::string & 
MayanHaabCalendar::MonthName( int month, long /*year*/ )
{
    Assert( (month >= 1) && (month <= 19) );
    return g_mayanHaabMonthNames[ month - 1 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

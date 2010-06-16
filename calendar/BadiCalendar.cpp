/*
  BadiCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class BadiCalendar, which defines the Badi calendar.
*/


#include "BadiCalendar.hpp"
#include "BahaiCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
BadiCalendar::JulianDayToDMYVK( long julianDay,
                                int * pDay, int * pMonth, 
                                int * pYear, int * pVahid, long * pKulliShay )
{
    long y;
    BahaiCalendar::JulianDayToDMY( julianDay, pDay, pMonth, &y );
    YearToYVK( y, pYear, pVahid, pKulliShay );
}

//-----------------------------------------------------------------------------

long 
BadiCalendar::DMYVKToJulianDay( int day, int month,
                                int year, int vahid, long kulliShay )
{
    return BahaiCalendar::DMYToJulianDay( day, month,
                                         YVKToYear( year, vahid, kulliShay ) );
}

//=============================================================================

int 
BadiCalendar::DaysInMonth( int month, int year, int vahid, long kulliShay )
{
    return BahaiCalendar::DaysInMonth( month,
                                       YVKToYear( year, vahid, kulliShay ) );
}

//-----------------------------------------------------------------------------

const std::string & 
BadiCalendar::DayName( int day )
{
    Assert( (day > 0) && (day <= 19) );
    return g_bahaiDayNames[ day - 1 ];
}

//-----------------------------------------------------------------------------

const std::string & 
BadiCalendar::MonthName( int month )
{
    Assert( (month > 0) && (month <= 20) );
    return g_bahaiMonthNames[ month - 1 ];
}

//-----------------------------------------------------------------------------

const std::string & 
BadiCalendar::YearName( int year )
{
    Assert( (year > 0) && (year <= 19) );
    return g_bahaiYearNames[ year - 1 ];
}

//=============================================================================

bool 
BadiCalendar::IsLeapYear( int year, int vahid, long kulliShay )
{
    return BahaiCalendar::IsLeapYear( YVKToYear( year, vahid, kulliShay ) );
}

//=============================================================================

void 
BadiCalendar::YearToYVK( long seqYear,
                         int * pYear, int * pVahid, long * pKulliShay )
{
    long y, v, k;
    DivModF( seqYear, (19L * 19L), &k, &y );
    DivModF( y - 1, 19L, &v, &y );
    *pYear = (int)(y + 1);
    *pVahid = (int)(v + 1);
    *pKulliShay = k + 1;
}

//-----------------------------------------------------------------------------

long 
BadiCalendar::YVKToYear( int year, int vahid, long kulliShay )
{
    return (year  +  19 * ((vahid - 1)  +  19 * (kulliShay - 1)));
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

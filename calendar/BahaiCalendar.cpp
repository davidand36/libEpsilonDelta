/*
  BahaiCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class BahaiCalendar, which defines the Baha'i calendar.

  Note that, at least in the West, this calendar is explicitly tied to
  the Gregorian calendar.
*/


#include "BahaiCalendar.hpp"
#include "GregorianCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
BahaiCalendar::JulianDayToDMY( int julianDay,
                               int * pDay, int * pMonth, int * pYear )
{
    int gd, gm, gy;
    GregorianCalendar::JulianDayToDMY( julianDay, &gd, &gm, &gy );
    int year;
    if ( (gm < 3) || ((gm == 3) && (gd < 21)) )
        year = gy - 1844;
    else
        year = gy - 1844 + 1;
    int days = julianDay - DMYToJulianDay( 1, 1, year );
    Assert( days >= 0 );
    int daysInAyyamiHa = DaysInMonth( AyyamiHa, year );
    if ( days >= (19 * 18 + daysInAyyamiHa) )
        days += (19 - daysInAyyamiHa);
    int month;
    int day;
    DivModF( days, 19, &month, &day );
    *pDay = day + 1;
    *pMonth = month + 1;
    *pYear = year;
}

//-----------------------------------------------------------------------------

int 
BahaiCalendar::DMYToJulianDay( int day, int month, int year )
{
    int jd = GregorianCalendar::DMYToJulianDay( 20, 3, (year - 1 + 1844) );
    jd += 19 * (month - 1);
    if ( month == Ala)
        jd -= (19 - DaysInMonth( AyyamiHa, year ));
    jd += day;
    return jd;
}

//=============================================================================

int 
BahaiCalendar::DaysInMonth( int month, int year )
{
    if ( month == AyyamiHa )
        return ( IsLeapYear( year )  ?  5  :  4 );
    else
        return 19;
}

//-----------------------------------------------------------------------------

const std::string & 
BahaiCalendar::MonthName( int month, int /*year*/ )
{
    return MonthName( month );
}

//-----------------------------------------------------------------------------

const std::string & 
BahaiCalendar::MonthName( int month )
{
    Assert( (month > 0) && (month <= 20) );
    return g_bahaiMonthNames[ month - 1 ];
}

//=============================================================================

bool 
BahaiCalendar::IsLeapYear( int year )
{
    return GregorianCalendar::IsLeapYear( year + 1844 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

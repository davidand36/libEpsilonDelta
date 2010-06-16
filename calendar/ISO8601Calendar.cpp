/*
  ISO8601Calendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class ISO8601Calendar, representing the ISO 8601 calendar.
*/


#include "ISO8601Calendar.hpp"
#include "GregorianDate.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
ISO8601Calendar::JulianDayToDWY( long julianDay,
                                 int * pDay, int * pWeek, long * pYear )
{
    GregorianDate date3( julianDay - 3 );
    long year = date3.Year();
    if ( DWYToJulianDay( 1, 1, year + 1 ) <= julianDay )
        ++year;
    int days = (int)(julianDay - DWYToJulianDay( 1, 1, year ));
    int week = (days / 7) + 1;
    int day = (days % 7) + 1;
    if ( pDay )
        *pDay = day;
    if ( pWeek )
        *pWeek = week;
    if ( pYear )
        *pYear = year;
}

//-----------------------------------------------------------------------------

long 
ISO8601Calendar::DWYToJulianDay( int day, int week, long year )
{
    GregorianDate greg( 28, GregorianCalendar::December, year - 1 );
    greg.Increment( WesternWeek::Sunday, (week - 1) );
    return  greg.JulianDay() + day;
}

//=============================================================================

int 
ISO8601Calendar::WeeksInYear( long year )
{
    return (int)((DWYToJulianDay( 1, 1, year + 1 )
                  - DWYToJulianDay( 1, 1, year )) / 7);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

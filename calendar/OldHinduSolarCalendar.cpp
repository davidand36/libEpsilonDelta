/*
  OldHinduSolarCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class OldHinduSolarCalendar, representing the old Hindu solar calendar.
*/


#include "OldHinduSolarCalendar.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace 
{

const double s_kaliYugaEpoch = 588466.; //JD of Kali Yuga (Iron Age) epoch
const double s_siderealYear = 1577917500. / 4320000.;
const double s_solarMonth = s_siderealYear / 12.;

}

//=============================================================================


void 
OldHinduSolarCalendar::JulianDayToDMY( int julianDay, 
                                       int * pDay, int * pMonth, int * pYear )
{
    double rise = julianDay - s_kaliYugaEpoch  +  1. / 4.;
    double year, rem;
    DivModRP( rise, s_siderealYear, &year, &rem );
    double month, day;
    DivModRP( rem, s_solarMonth, &month, &day );
    month = ModRP( std::floor( month ), 12. );
    if ( pDay )
        *pDay = static_cast< int >( std::floor( day ) ) + 1;
    if ( pMonth )
        *pMonth = static_cast< int >( month ) + 1;
    if ( pYear )
        *pYear = static_cast< int >( std::floor( year ) );
}

//-----------------------------------------------------------------------------

int 
OldHinduSolarCalendar::DMYToJulianDay( int day, int month, int year )
{
    double jd = s_kaliYugaEpoch  +  year * s_siderealYear
            +  (month - 1) * s_solarMonth  +  day  -  1. / 4.;
    return static_cast< int >( std::floor( jd ) );
}

//=============================================================================

int 
OldHinduSolarCalendar::DaysInMonth( int month, int year )
{
    int month1 = month + 1;
    int year1 = year;
    if ( month1 > 12 )
    {
        month1 = 1;
        ++year1;
    }
    return  DMYToJulianDay( 1, month1, year1 )
            - DMYToJulianDay( 1, month, year );
}

//-----------------------------------------------------------------------------

const std::string & 
OldHinduSolarCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month >= 1) && (month <= 12) );
    return g_hinduSolarMonthNames[ month - 1 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

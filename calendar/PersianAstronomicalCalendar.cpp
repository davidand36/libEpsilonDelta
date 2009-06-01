/*
  PersianAstronomicalCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class PersianAstronomicalCalendar, which defines the Persian calendar,
  as determined by the true astronomical equinox.
*/


#include "PersianAstronomicalCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#include "TimeIncrement.hpp"
#include "AstroPhenomena.hpp"
#include "EquationOfTime.hpp"
#include "TimeStandards.hpp"
#include "Angle.hpp"
#include <cmath>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace
{                                                                   //namespace

const int s_persianEpoch = 1948321;
#if 1
//Longitude of Iranian Standard Time (UT+3.5h) used, following note 244
// in errata to Reingold & Dershowitz
const double s_localZone = Angle( 52.5, Angle::Degree ).Cycles();
#else
//Longitude of Tehran used, following Reingold & Dershowitz.
const double s_localZone = Angle( 51.42, Angle::Degree ).Cycles();
#endif
const double s_tropicalYear = 365.2421896698;

double JDItoJD( int jdi );
int JDtoJDI( double jd );
Angle SolarLongitude( int jdi );
int PriorSpringEquinox( int jdi );

}                                                                   //namespace

//=============================================================================


void
PersianAstronomicalCalendar::JulianDayToDMY( int julianDay,
                                 int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Edward M. Reingold and Nachum Dershowitz,
      "Calendrical Calculations, the Millennium Edition", p. 214.*/
    int newYear = PriorSpringEquinox( julianDay ) + 1;
    int year = static_cast<int>( std::floor( ((newYear - s_persianEpoch)
                                             / s_tropicalYear) + 0.5 ) )  +  1;
    int dayOfYear = julianDay  -  DMYToJulianDay( 1, 1, year )  +  1;
    int month;
    if ( dayOfYear <= 186 )
        month = static_cast<int>( std::ceil( dayOfYear / 31. ) );
    else
        month = static_cast<int>( std::ceil( (dayOfYear - 6) / 30. ) );
    int day = julianDay  -  DMYToJulianDay( 1, month, year )  +  1;
    if ( pDay )
        *pDay = day;
    if ( pMonth )
        *pMonth = month;
    if ( pYear )
        *pYear = year;
}

//-----------------------------------------------------------------------------

int
PersianAstronomicalCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Edward M. Reingold and Nachum Dershowitz,
      "Calendrical Calculations, the Millennium Edition", p. 214.*/
    int d = s_persianEpoch  +  180
            +  static_cast<int>( std::floor( s_tropicalYear * (year - 1) ) );
    int newYear = PriorSpringEquinox( d ) + 1;
    int jd;
    if ( month <= 7 )
        jd = newYear  +  31 * (month - 1)  +  day  -  1;
    else
        jd = newYear  +  30 * (month - 1)  +  day  +  5;
    return jd;
}

//=============================================================================

int
PersianAstronomicalCalendar::DaysInMonth( int month, int year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    static const int daysInMonth[ 12 ]
         = { 31, 31, 31, 31, 31, 31, 30, 30, 30, 30, 30, 29 };
    if ( (month != 12) || ! IsLeapYear( year ) )
        return daysInMonth[ month - 1 ];
    return 30;
}

//-----------------------------------------------------------------------------

const string &
PersianAstronomicalCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= 12) );
    return g_persianMonthNames[ month - 1 ];
}

//=============================================================================

bool
PersianAstronomicalCalendar::IsLeapYear( int year )
{
    int daysInYear = DMYToJulianDay( 1, 1, year + 1 )
            -  DMYToJulianDay( 1, 1, year );
    return daysInYear > 365;
}

//=============================================================================

namespace
{                                                                 /*namespace*/

//-----------------------------------------------------------------------------

double
JDItoJD( int jdi )
{
    double tdb_ut = TDB_UT( jdi ).Days();
    double jd = jdi - s_localZone + tdb_ut;
    double eot = EquationOfTime( jd ).Days();
    return  jd - eot;
}

//-----------------------------------------------------------------------------

int 
JDtoJDI( double jd )
{
    double ut_tdb = - TDB_UT( jd ).Days();
    double eot = EquationOfTime( jd ).Days();
    return  static_cast<int>( std::floor( jd + s_localZone + ut_tdb + eot ) );
}

//=============================================================================

Angle
SolarLongitude( int jdi )
{
    double jd = JDItoJD( jdi );
    return EpsilonDelta::SolarLongitude( jd );
}

//-----------------------------------------------------------------------------

int 
PriorSpringEquinox( int jdi )
{
    const double spring2000 = 2451623.8159722;
    double jd = JDItoJD( jdi );
    double offset = ModRP( (jd - spring2000), s_tropicalYear );
    if ( offset < 10. )
    {
        //Initial estimate might bring us to the next equinox..
        if ( SolarLongitude( jdi ).Radians() < 0. )
            offset = s_tropicalYear;
    }
    else if ( offset > 355. )
    {
        //Initial estimate might be a year too early.
        if ( SolarLongitude( jdi ).Radians() >= 0. )
            offset = 0.;
    }
    jdi = JDtoJDI( jd - offset );    //first estimate
    Angle solarLong = SolarLongitude( jdi );
    if ( solarLong.Radians() < 0. )
        while ( SolarLongitude( jdi + 1 ).Radians() < 0. )
            ++jdi;
    else
        while ( solarLong.Radians() > 0. )
            solarLong = SolarLongitude( --jdi );
    return jdi;
}

//-----------------------------------------------------------------------------

}                                                                 /*namespace*/


//*****************************************************************************

}                                                      //namespace EpsilonDelta

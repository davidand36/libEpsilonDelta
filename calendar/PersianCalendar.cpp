/*
  PersianCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class PersianCalendar, which defines the Persian calendar.
*/


#include "PersianCalendar.hpp"
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

PersianCalendar::EMethod PersianCalendar::ms_method
    = PersianCalendar::Astronomical;

//=============================================================================

void
PersianCalendar::JulianDayToDMY( int julianDay,
                                 int * pDay, int * pMonth, int * pYear )
{
    switch ( ms_method )
    {
    case Astronomical:
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
        break;
    }
    case Arithmetic:
    {
        /*Adapted from Nachum Dershowitz and Edward M. Reingold,
          "Calendrical Calculations" (1st ed.), p. 73.*/
        const int jd475 = 2121446; /*DMYToJulianDay( 1, 1, 475 )*/
        int d0 = julianDay - jd475;
        int n = DivF( d0, 1029983 );
        int d1 = ModF( d0, 1029983 );
        int y;
        if ( d1 == 1029982 )
            y = 2820;
        else
        {
            int a, b;
            DivModF( d1, 366, &a, &b );
            y = 1 + a + DivF( (2134 * a + 2816 * b + 2815), 1028522 );
        }
        int year = y + 2820 * n + 474;
        int dy = julianDay - DMYToJulianDay( 1, 1, year ) + 1;
        int month = (dy <= 186) ? DivC( dy, 31 ) : DivC( (dy - 6), 30 );
        int day = julianDay - DMYToJulianDay( 1, month, year ) + 1;
        *pDay = day;
        *pMonth = month;
        *pYear = year;
        break;
    }
    default:
        Assert( 0 && "Unexpected method for Persian calendar" );
        break;
    }
}

//-----------------------------------------------------------------------------

int
PersianCalendar::DMYToJulianDay( int day, int month, int year )
{
    switch ( ms_method )
    {
    case Astronomical:
    {
        /*Adapted from Edward M. Reingold and Nachum Dershowitz,
          "Calendrical Calculations, the Millennium Edition", p. 214.*/
        int d = s_persianEpoch  +  180
                + static_cast<int>( std::floor( s_tropicalYear * (year - 1) ) );
        int newYear = PriorSpringEquinox( d ) + 1;
        int jd;
        if ( month <= 7 )
            jd = newYear  +  31 * (month - 1)  +  day  -  1;
        else
            jd = newYear  +  30 * (month - 1)  +  day  +  5;
        return jd;
    }
    case Arithmetic:
    {
        /*Adapted from Nachum Dershowitz and Edward M. Reingold,
          "Calendrical Calculations" (1st ed.), p. 72.*/
        int y = year - 474;
        int yr = ModF( y, 2820 ) + 474;
        int md = (month <= 7) ? (31 * (month - 1)) : ((30 * (month - 1)) + 6);
        return (s_persianEpoch - 1 + 365 * (yr - 1) + 1029983 * DivF( y, 2820 )
                + DivF( (682 * yr - 110), 2816 ) + md + day);
    }
    default:
        Assert( 0 && "Unexpected method for Persian calendar" );
        return 0;
    }
}

//=============================================================================

int
PersianCalendar::DaysInMonth( int month, int year )
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
PersianCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= 12) );
    return g_persianMonthNames[ month - 1 ];
}

//=============================================================================

bool
PersianCalendar::IsLeapYear( int year )
{
    switch ( ms_method )
    {
    case Astronomical:
    {
        int daysInYear = DMYToJulianDay( 1, 1, year + 1 )
                -  DMYToJulianDay( 1, 1, year );
        return daysInYear > 365;
    }
    case Arithmetic:
    {
        /*Adapted from Nachum Dershowitz and Edward M. Reingold,
          "Calendrical Calculations" (1st ed.), p. 71.*/
        int y = year - 474;
        int yy = ModF( y, 2820 ) + 474;
        return ( ModF( ((yy + 38) * 682), 2816) < 682 );
    }
    default:
        Assert( 0 && "Unexpected method for Persian calendar" );
        break;
    }
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

/*
  IslamicCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class IslamicCalendar, which defines the Islamic calendar.
*/



#include "IslamicCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#include "AngleDMS.hpp"
#include "AstroPhenomena.hpp"
#include "RiseSet.hpp"
#include "MoonPhases.hpp"
#include "SiderealTime.hpp"
#include "AstroCoordTransformations.hpp"
#include "Horizontal.hpp"
#include <cmath>
#include <algorithm>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


GeodeticLocation Mecca( AngleDMS( 39, 49, 24. ), AngleDMS( 21, 25, 24 ),
                        1000 );


//*****************************************************************************


namespace
{                                                                   //namespace

const int s_islamicEpoch = 1948440;
const double s_synodicMonth = 29.5305888531;
const shared_ptr< IslamicCalendar::LocalMonthFunc > s_spUmmAlQuraMonthFunc(
    new IslamicCalendar::LocalMonthFunc( IslamicCalendar::UmmAlQuraVisibility,
                                         Mecca ) );
}                                                                   //namespace

//*****************************************************************************


shared_ptr< IslamicCalendar::System > IslamicCalendar::ms_pSystem( 
    new IslamicCalendar::AstronomicalSystem( s_spUmmAlQuraMonthFunc ) );


//=============================================================================


void
IslamicCalendar::JulianDayToDMY( int julianDay,
                                 int * pDay, int * pMonth, int * pYear )
{
    Assert( ms_pSystem );
    ms_pSystem->JulianDayToDMY( julianDay, pDay, pMonth, pYear );
}

//-----------------------------------------------------------------------------

int
IslamicCalendar::DMYToJulianDay( int day, int month, int year )
{
    Assert( ms_pSystem );
    return ms_pSystem->DMYToJulianDay( day, month, year );
}

//=============================================================================

int
IslamicCalendar::DaysInMonth( int month, int year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    Assert( ms_pSystem );
    return ms_pSystem->DaysInMonth( month, year );
}

//-----------------------------------------------------------------------------

const string &
IslamicCalendar::MonthName( int month, int /*year*/ )
{
    return MonthName( month );
}

//-----------------------------------------------------------------------------

const string &
IslamicCalendar::MonthName( int month )
{
    Assert( (month > 0) && (month <= 12) );
    return g_islamicMonthNames[ month - 1 ];
}

//=============================================================================

bool
IslamicCalendar::IsLeapYear( int year )
{
    Assert( ms_pSystem );
    return ms_pSystem->IsLeapYear( year );
}


//*****************************************************************************


void 
IslamicCalendar::ArithmeticSystem::JulianDayToDMY( int julianDay,
                                                   int * pDay, int * pMonth,
                                                   int * pYear )
{
    int day = 0;
    int month = 0;
    int year = 0;

    switch ( m_leapSequence )
    {
    case Nizari:
    default:
    {
        /*Adapted from Edward M. Reingold and Nachum Dershowitz,
          "Calendrical Calculations, Millennium Edition", p. 89.*/
        year = DivF( (30 * (julianDay - s_islamicEpoch) + 10646), 10631 );
        int priorDays = julianDay - DMYToJulianDay( 1, 1, year );
        month = DivF( (11 * priorDays + 330), 325 );
        day = julianDay - DMYToJulianDay( 1, month, year ) + 1;
        break;
    }

    case Labban:
    {
        /*Adapted from Edward M. Reingold and Nachum Dershowitz,
          "Calendrical Calculations, Millennium Edition", p. 89, footnote.*/
        year = DivF( (30 * (julianDay - s_islamicEpoch) + 10645), 10631 );
        int priorDays = julianDay - DMYToJulianDay( 1, 1, year );
        month = DivF( (11 * priorDays + 330), 325 );
        day = julianDay - DMYToJulianDay( 1, month, year ) + 1;
        break;
    }
    }

    *pDay = day;
    *pMonth = month;
    *pYear = year;
}

//-----------------------------------------------------------------------------

int 
IslamicCalendar::ArithmeticSystem::DMYToJulianDay( int day, int month,
                                                   int year )
{
    switch ( m_leapSequence )
    {
    case Nizari:
    default:
    {
        /*Adapted from Edward M. Reingold and Nachum Dershowitz,
          "Calendrical Calculations, Millennium Edition", p. 89.*/
        return (s_islamicEpoch - 1
                + (year - 1) * 354  +  DivF( (11 * year + 3), 30 )
                + (month - 1) * 29  +  DivF( month, 2 )
                + day);
    }

    case Labban:
    {
        /*Adapted from Edward M. Reingold and Nachum Dershowitz,
          "Calendrical Calculations, Millennium Edition", p. 89, footnote.*/
        return (s_islamicEpoch - 1
                + (year - 1) * 354  +  DivF( (11 * year + 4), 30 )
                + (month - 1) * 29  +  DivF( month, 2 )
                + day);
    }
    }
}

//=============================================================================

int 
IslamicCalendar::ArithmeticSystem::DaysInMonth( int month, int year )
{
    static const int daysInMonth[ 12 ]
            = { 30, 29, 30, 29, 30, 29, 30, 29, 30, 29, 30, 29 };
    if ( (month == 12) && IsLeapYear( year ) )
        return 30;
    return daysInMonth[ month - 1 ];
}

//-----------------------------------------------------------------------------

bool 
IslamicCalendar::ArithmeticSystem::IsLeapYear( int year )
{
    switch ( m_leapSequence )
    {
    case Nizari:
    default:
    {
        /*Adapted from Edward M. Reingold and Nachum Dershowitz,
          "Calendrical Calculations, Millennium Edition", p. 89.*/
        return ( ModF( (11 * year + 14), 30 ) < 11 );
    }

    case Labban:
    {
        /*Adapted from Edward M. Reingold and Nachum Dershowitz,
          "Calendrical Calculations, Millennium Edition", p. 89, footnote.*/
        return ( ModF( (11 * year + 15), 30 ) < 11 );
    }
    }
}


//*****************************************************************************


void 
IslamicCalendar::AstronomicalSystem::JulianDayToDMY( int julianDay,
                                                     int * pDay, int * pMonth,
                                                     int * pYear )
{
    /*Adapted from Edward M. Reingold and Nachum Dershowitz,
      "Calendrical Calculations, Millennium Edition", p 206.*/
    int monthStart = MonthStart( julianDay );
    int elapsedMonths = static_cast< int >(
        std::floor( ((monthStart - s_islamicEpoch) / s_synodicMonth) + 0.5 ) );
    int month;
    int year;
    DivModF( elapsedMonths, 12, &year, &month );
    *pDay = julianDay - monthStart + 1;
    *pMonth = month + 1;
    *pYear = year + 1;
}

//-----------------------------------------------------------------------------

int 
IslamicCalendar::AstronomicalSystem::DMYToJulianDay( int day, int month,
                                                     int year )
{
    /*Adapted from Edward M. Reingold and Nachum Dershowitz,
      "Calendrical Calculations, Millennium Edition", p 206.*/
    int midMonth = s_islamicEpoch
            + static_cast<int>( std::floor( ((year - 1) * 12  +  month  -  0.5)
                                            * s_synodicMonth ) );
    int monthStart = MonthStart( midMonth );
    return monthStart + day - 1;
}

//-----------------------------------------------------------------------------

int 
IslamicCalendar::AstronomicalSystem::MonthStart( int julianDay )
{
    /*Adapted from Edward M. Reingold and Nachum Dershowitz,
      "Calendrical Calculations, Millennium Edition", p 205.*/
    Angle phase = LunarPhase( julianDay + 1 );
    phase.NormalizePositive( );
    int jd = julianDay -
            static_cast<int>( std::floor( phase.Cycles() * s_synodicMonth ) );
    if ( (julianDay - jd <= 3) && (! (*m_pMonthFunc)( julianDay )) )
        jd -= 30;
    else
        jd -= 2;
    while ( (! (*m_pMonthFunc)( jd )) && (jd <= julianDay) )
        ++jd;
    return jd;
}

//=============================================================================

int 
IslamicCalendar::AstronomicalSystem::DaysInMonth( int month, int year )
{
    if ( month < MonthsInYear( year ) )
        return DMYToJulianDay( 1, month + 1, year )
                - DMYToJulianDay( 1, month, year );
    else
        return DMYToJulianDay( 1, 1, year + 1 )
                - DMYToJulianDay( 1, month, year );
}

//-----------------------------------------------------------------------------

bool 
IslamicCalendar::AstronomicalSystem::IsLeapYear( int year )
{
    return ( (DMYToJulianDay( 1, 1, year + 1 ) - DMYToJulianDay( 1, 1, year ))
            > 354 );
}


//*****************************************************************************


bool 
IslamicCalendar::ISNA_Hijri::operator()( int julianDay )
{
    /*The rule is simply that New Moon (conjunction) occur before 12:00 GMT.
      See http://www.fiqhcouncil.org/articles/Lunar_Calendar.html*/
    double newMoon = MoonPhases::FindNext( julianDay - 15., MoonPhases::New );
    return (newMoon < julianDay - 1. );
}


//*****************************************************************************


bool 
IslamicCalendar::LocalMonthFunc::operator()( int julianDay )
{
    double jd = julianDay  - 1. - m_location.Longitude().Cycles();
    return m_visibilityFunc( jd, m_location );
}


//*****************************************************************************


bool 
IslamicCalendar::UmmAlQuraVisibility( double julianDay,
                                      const GeodeticLocation & location )
{
    /* Using rules reported by the Islamic Crescents' Observation Project
       on http://www.icoproject.org/sau.html.
       See also http://www.phys.uu.nl/~vgent/islam/ummalqura.htm.*/
    int refJD = static_cast< int >( julianDay + 1.
                                    + location.Longitude().Cycles() );
    RiseSet::Result sunsetRslt
            = RiseSet::FindNext( julianDay, SolarSystem::Sun,
                                 RiseSet::Set, location );
    if ( sunsetRslt.m_status != RiseSet::OK )
        return true;
    double sunset = sunsetRslt.m_julianDay;
    if ( refJD < 2451286 ) //before 1420 A.H.
    {
        //New Moon is before or within 12 hours of sunset
        double newMoon
                = MoonPhases::FindNext( julianDay - 15., MoonPhases::New );
        return ( newMoon < sunset + 0.5 );
    }
    else if ( refJD < 2452349 ) //before 1423 A.H.
    {
        //Moonset is after sunset
        RiseSet::Result moonsetRslt
                = RiseSet::FindNext( julianDay, SolarSystem::Moon,
                                     RiseSet::Set, location );
        if ( moonsetRslt.m_status != RiseSet::OK )
            return true;
        double moonset = moonsetRslt.m_julianDay;
        return ( (moonset > sunset) && (moonset < sunset + 0.25) );
    }
    else    //since 1423 A.H.
    {
        //New Moon is before sunset and moonset is after sunset
        double newMoon
                = MoonPhases::FindNext( julianDay - 15., MoonPhases::New );
        if ( sunset < newMoon )
            return false;
        else if ( newMoon + 1. < sunset )
        {
            return true;
        }
        RiseSet::Result moonsetRslt
                = RiseSet::FindNext( julianDay, SolarSystem::Moon,
                                     RiseSet::Set, location );
        if ( moonsetRslt.m_status != RiseSet::OK )
            return true;
        double moonset = moonsetRslt.m_julianDay;
        return ( moonset > sunset );
    }
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

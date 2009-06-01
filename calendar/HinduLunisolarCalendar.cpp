/*
  HinduLunisolarCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class HinduLunisolarCalendar, representing the old Hindu lunisolar
  calendar.
*/


#include "HinduLunisolarCalendar.hpp"
#include "HinduSolarCalendar.hpp"
#include "OldHinduLunisolarCalendar.hpp"
#include "HinduLunisolarDate.hpp"
#include "HinduAstro.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"
#include "DateJD.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace 
{

const int s_kaliYugaEpoch = 588466; //JD of Kali Yuga (Iron Age) epoch
const int s_vikramaEra = 3044;      //Kali Yuga era year of Vikrama era year 0

}

//=============================================================================


void 
HinduLunisolarCalendar::JulianDayToDLMLY( int julianDay, 
                                         int * pDay, bool * pDayLeap,
                                         int * pMonth, bool * pMonthLeap,
                                         int * pYear )
{
    int kaliYugaDay = julianDay - s_kaliYugaEpoch;
    double rise = HinduAstro::Sunrise( kaliYugaDay );
    int day = HinduAstro::LunarDay( rise );
    double prevRise = HinduAstro::Sunrise( kaliYugaDay - 1 );
    int prevDay = HinduAstro::LunarDay( prevRise );
    bool dayLeap = (day == prevDay);
    double priorNewMoon = HinduAstro::PriorNewMoon( rise );
    double nextNewMoon = HinduAstro::PriorNewMoon( priorNewMoon + 35 );
    int solarMonth = HinduAstro::Zodiac( priorNewMoon );
    int nextSolarMonth = HinduAstro::Zodiac( nextNewMoon );
    bool monthLeap = (solarMonth == nextSolarMonth);
    int month = ModP( solarMonth, 12 ) + 1;
    int kyYear;
    if ( month <= 2 )
        kyYear = HinduSolarCalendar::KaliYugaYear( nextNewMoon + 180. );
    else
        kyYear = HinduSolarCalendar::KaliYugaYear( nextNewMoon );
    int year = kyYear - s_vikramaEra;
    if ( pDay )
        *pDay = day;
    if ( pDayLeap )
        *pDayLeap = dayLeap;
    if ( pMonth )
        *pMonth = month;
    if ( pMonthLeap )
        *pMonthLeap = monthLeap;
    if ( pYear )
        *pYear = year;
}

//-----------------------------------------------------------------------------

int 
HinduLunisolarCalendar::DLMLYToJulianDay( int day, bool dayLeap,
                                          int month, bool monthLeap,
                                          int year )
{
    static const double siderealYear = HinduAstro::SiderealYear();
    HinduLunisolarDate hlDate0( day, dayLeap, month, monthLeap, year );
    double kyt = siderealYear * (s_vikramaEra  +  year  +  (month - 1) / 12.);
    double offset = (siderealYear / 360.)
            * (ModRP( (HinduAstro::SolarLongitude( kyt )
                       -  (month - 1) * 30  +  180.),  360. )
               -  180.);
    kyt = std::floor( kyt - offset );
    int k = HinduAstro::LunarDay( kyt + (1. / 4.) );
    int midJD = static_cast< int >( kyt )  -  15  +  s_kaliYugaEpoch;
    HinduLunisolarDate hlDate1( midJD );
    kyt += day;
    if ( (k > 3) && (k < 27) )
        kyt -= k;
    else if ( (hlDate1.Month() < month)
              || (hlDate1.IsMonthLeap() && (! monthLeap)) )
        kyt -= (ModP( (k + 15), 30 ) - 15);
    else
        kyt -= (ModP( (k + 15), 30 ) + 15);
    kyt += 14  -  ModP( (HinduAstro::LunarDay( kyt + (1. / 4.) ) - day + 15),
                        30 );
    int jd = static_cast< int >( kyt ) + s_kaliYugaEpoch;
    hlDate1.Set( jd );
    while ( hlDate1 < hlDate0 )
        hlDate1.Set( ++jd );
    return jd;
}

//=============================================================================

bool 
HinduLunisolarCalendar::Valid( int day, bool dayLeap,
                               int month, bool monthLeap,
                               int year )
{
    if ( (month < 1) || (month > 12) )
        return false;
    if ( day < 1 )
        return false;
    if ( monthLeap && (! IsMonthLeap( month, year )) )
        return false;
    if ( day > LastDayOfMonth( month, monthLeap, year ) )
        return false;
    if ( day == LostDay( month, monthLeap, year ) )
        return false;
    if ( dayLeap && (day != LeapDay( month, monthLeap, year )) )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

int 
HinduLunisolarCalendar::MakeValid( int * pDay, bool * pDayLeap, int * pMonth,
                                      bool * pMonthLeap, int * pYear,
                                      DateFixup::EMethod fixupMethod )
{
    Assert( pDay && pDayLeap && pMonth && pMonthLeap && pYear );
    if ( fixupMethod == DateFixup::Carry )
    {
        int julianDay = DLMLYToJulianDay( *pDay, *pDayLeap, *pMonth,
                                          *pMonthLeap, *pYear );
        JulianDayToDLMLY( julianDay, pDay, pDayLeap, pMonth, pMonthLeap,
                          pYear );
        return julianDay;
    }
    else    //DateFixup::Clamp
    {
        if ( *pMonth < 1 )
            *pMonth = 1;
        else if ( *pMonth > 12 )
            *pMonth = 12;
        if ( *pMonthLeap && (! IsMonthLeap( *pMonth, *pYear )) )
            *pMonthLeap = false;
        if ( *pDay < 1 )
            *pDay = 1;
        else
        {
            int lastDay = LastDayOfMonth( *pMonth, *pMonthLeap, *pYear );
            int lostDay = LostDay( *pMonth, *pMonthLeap, *pYear );
            if ( *pDay > lastDay )
                *pDay = lastDay;
            else if ( *pDay == lostDay )
                *pDay = lostDay + 1;
            Assert( *pDay <= lastDay );
            Assert( *pDay != lostDay );
        }
        if ( *pDayLeap && (*pDay != LeapDay( *pMonth, *pMonthLeap, *pYear )) )
            *pDayLeap = false;
        return DateJD::INVALID;
    }
}

//=============================================================================

int 
HinduLunisolarCalendar::LastDayOfMonth( int month, bool monthLeap,
                                           int year )
{
    int jd0 = DLMLYToJulianDay( 1, false, month, monthLeap, year );
    int year1 = year;
    int month1 = month + 1;
    if ( monthLeap )
        --month1;
    if ( month1 > 12 )
    {
        month1 = 1;
        --year1;
    }
    int jd1 = DLMLYToJulianDay( 1, false, month1, false, year1 );
    if ( jd1 > jd0 + 40 )
        jd1 = DLMLYToJulianDay( 1, false, month1, true, year1 );
    Assert( jd1 < jd0 + 40 );
    int lastDay;
    JulianDayToDLMLY( (jd1 - 1), &lastDay, 0, 0, 0, 0 );
    return lastDay;
}

//-----------------------------------------------------------------------------

int 
HinduLunisolarCalendar::LostDay( int month, bool monthLeap, int year )
{
    int jd1 = DLMLYToJulianDay( 1, false, month, monthLeap, year );
    int kyDay = jd1 - s_kaliYugaEpoch;
    int lastDayOfMonth = LastDayOfMonth( month, monthLeap, year );
    int prevDay = 1;
    for ( int d = 1; d <= lastDayOfMonth; ++d )
    {
        double rise = HinduAstro::Sunrise( kyDay );
        int day = HinduAstro::LunarDay( rise );
        if ( day > prevDay + 1 )
            return prevDay + 1;
        prevDay = day;
        ++kyDay;
    }
    return LDNone;
}

//-----------------------------------------------------------------------------

int 
HinduLunisolarCalendar::LeapDay( int month, bool monthLeap, int year )
{
    int jd1 = DLMLYToJulianDay( 1, false, month, monthLeap, year );
    int kyDay = jd1 - s_kaliYugaEpoch;
    int lastDayOfMonth = LastDayOfMonth( month, monthLeap, year );
    int prevDay = -1;
    for ( int d = 1; d <= lastDayOfMonth + 1; ++d )
    {
        double rise = HinduAstro::Sunrise( kyDay );
        int day = HinduAstro::LunarDay( rise );
        if ( day == prevDay )
            return day;
        prevDay = day;
        ++kyDay;
    }
    return LDNone;
}

//=============================================================================

bool 
HinduLunisolarCalendar::IsMonthLeap( int month, int year )
{
    return true; //!!!
}

//=============================================================================

const std::string & 
HinduLunisolarCalendar::MonthName( int month )
{
    Assert( (month >= 1) && (month <= 12) );
    return g_hinduLunarMonthNames[ month - 1 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

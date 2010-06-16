/*
  HinduLunisolarCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class HinduLunisolarCalendar, representing the old Hindu lunisolar
  calendar.
*/


#include "HinduLunisolarCalendar.hpp"
#include "HinduSolarCalendar.hpp"
#include "HinduLunisolarDate.hpp"
#include "HinduAstro.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"
#include "JDDate.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace 
{

const long s_kaliYugaEpoch = 588466; //JD of Kali Yuga (Iron Age) epoch
const long s_vikramaEra = 3044;      //Kali Yuga era year of Vikrama era year 0
const double s_siderealYear = 1577917500. / 4320000.;
const double s_solarMonth = s_siderealYear / 12.;
const double s_lunarMonth = 1577917500. / 53433336.;
const double s_lunarDay = s_lunarMonth / 30.;
const double s_solarLunarMonthDiff = s_solarMonth - s_lunarMonth;

}

//=============================================================================

HinduLunisolarCalendar::EVersion HinduLunisolarCalendar::ms_version
    = HinduLunisolarCalendar::Modern;

//=============================================================================


void 
HinduLunisolarCalendar::JulianDayToDLMLY( long julianDay, 
                                         int * pDay, bool * pDayLeap,
                                         int * pMonth, bool * pMonthLeap,
                                         long * pYear )
{
    switch ( ms_version )
    {
    case Modern:
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
        long kyYear;
        if ( month <= 2 )
            kyYear = HinduSolarCalendar::KaliYugaYear( nextNewMoon + 180. );
        else
            kyYear = HinduSolarCalendar::KaliYugaYear( nextNewMoon );
        long year = kyYear - s_vikramaEra;
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
        break;
    }
    case Old:
    {
        double rise = julianDay - s_kaliYugaEpoch  +  1. / 4.;
        double day = ModRP( std::floor( rise / s_lunarDay ), 30. );
        double newMoon = rise - ModRP( rise, s_lunarMonth );
        double monthDiff = ModRP( newMoon, s_solarMonth );
        bool monthLeap = ((monthDiff > 0.)
                          && (monthDiff <= s_solarLunarMonthDiff));
        double month = ModRP( std::ceil( newMoon / s_solarMonth ), 12. );
        double year = (newMoon + s_solarMonth) / s_siderealYear;
        if ( pDay )
            *pDay = (int)( day ) + 1;
        if ( pDayLeap )
            *pDayLeap = false;
        if ( pMonth )
            *pMonth = (int)( month ) + 1;
        if ( pMonthLeap )
            *pMonthLeap = monthLeap;
        if ( pYear )
            *pYear = (long)( std::ceil( year ) ) - 1;
        break;
    }
    default:
        Assert( 0 && "Unexpected version of HinduLunisolarCalendar" );
        break;
    }
}

//-----------------------------------------------------------------------------

long 
HinduLunisolarCalendar::DLMLYToJulianDay( int day, bool dayLeap,
                                          int month, bool monthLeap,
                                          long year )
{
    switch ( ms_version )
    {
    case Modern:
    {
        static const double siderealYear = HinduAstro::SiderealYear();
        HinduLunisolarDate hlDate0( day, dayLeap, month, monthLeap, year );
        double kyt = siderealYear
                * (s_vikramaEra  +  year  +  (month - 1) / 12.);
        double offset = (siderealYear / 360.)
                * (ModRP( (HinduAstro::SolarLongitude( kyt )
                           -  (month - 1) * 30  +  180.),  360. )
                   -  180.);
        kyt = std::floor( kyt - offset );
        int k = HinduAstro::LunarDay( kyt + (1. / 4.) );
        long midJD = (long)( kyt )  -  15  +  s_kaliYugaEpoch;
        HinduLunisolarDate hlDate1( midJD );
        kyt += day;
        if ( (k > 3) && (k < 27) )
            kyt -= k;
        else if ( (hlDate1.Month() < month)
                  || (hlDate1.IsMonthLeap() && (! monthLeap)) )
            kyt -= (ModP( (k + 15), 30 ) - 15);
        else
            kyt -= (ModP( (k + 15), 30 ) + 15);
        kyt += 14 - ModP( (HinduAstro::LunarDay( kyt + (1. / 4.) ) - day + 15),
                          30 );
        long jd = (long)( kyt ) + s_kaliYugaEpoch;
        hlDate1.Set( jd );
        while ( hlDate1 < hlDate0 )
            hlDate1.Set( ++jd );
        return jd;
    }
    case Old:
    {
        double mina = (12. * year - 1) * s_solarMonth;
        double newYear = (std::floor( mina / s_lunarMonth ) + 1) * s_lunarMonth;
        if ( monthLeap
            || (std::ceil( (newYear - mina) / s_solarLunarMonthDiff ) > month) )
            --month;
        double jd = s_kaliYugaEpoch  +  newYear  +  month * s_lunarMonth
                +  (day - 1) * s_lunarDay  +  3. / 4.;
        return (long)( std::floor( jd ) );
    }
    default:
        Assert( 0 && "Unexpected version of HinduLunisolarCalendar" );
        return 0;
    }
}

//=============================================================================

bool 
HinduLunisolarCalendar::Valid( int day, bool dayLeap,
                               int month, bool monthLeap,
                               long year )
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

long 
HinduLunisolarCalendar::MakeValid( int * pDay, bool * pDayLeap, int * pMonth,
                                      bool * pMonthLeap, long * pYear,
                                      DateFixup::EMethod fixupMethod )
{
    Assert( pDay && pDayLeap && pMonth && pMonthLeap && pYear );
    if ( fixupMethod == DateFixup::Carry )
    {
        long julianDay = DLMLYToJulianDay( *pDay, *pDayLeap, *pMonth,
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
        return JDDate::INVALID;
    }
}

//=============================================================================

int 
HinduLunisolarCalendar::LastDayOfMonth( int month, bool monthLeap,
                                           long year )
{
    long jd0 = DLMLYToJulianDay( 1, false, month, monthLeap, year );
    long year1 = year;
    int month1 = month + 1;
    if ( monthLeap )
        --month1;
    if ( month1 > 12 )
    {
        month1 = 1;
        --year1;
    }
    long jd1 = DLMLYToJulianDay( 1, false, month1, false, year1 );
    if ( jd1 > jd0 + 40 )
        jd1 = DLMLYToJulianDay( 1, false, month1, true, year1 );
    Assert( jd1 < jd0 + 40 );
    int lastDay;
    JulianDayToDLMLY( (jd1 - 1), &lastDay, 0, 0, 0, 0 );
    return lastDay;
}

//-----------------------------------------------------------------------------

int 
HinduLunisolarCalendar::LostDay( int month, bool monthLeap, long year )
{
    switch ( ms_version )
    {
    case Modern:
    {
        long jd1 = DLMLYToJulianDay( 1, false, month, monthLeap, year );
        long kyDay = jd1 - s_kaliYugaEpoch;
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
    case Old:
    {
        static const double solarLunarDayDiff = 1. - s_lunarDay;
        long jd1 = DLMLYToJulianDay( 1, false, month, monthLeap, year );
        double rise = jd1 - s_kaliYugaEpoch +  1. / 4.;
        double dayGap = ModRP( rise, s_lunarDay );
        return (int)( std::ceil( (1. - dayGap)
                                              / solarLunarDayDiff ) );
    }
    default:
        Assert( 0 && "Unexpected version of HinduLunisolarCalendar" );
        return LDNone;
    }
}

//-----------------------------------------------------------------------------

int 
HinduLunisolarCalendar::LeapDay( int month, bool monthLeap, long year )
{
    switch ( ms_version )
    {
    case Modern:
    {
        long jd1 = DLMLYToJulianDay( 1, false, month, monthLeap, year );
        long kyDay = jd1 - s_kaliYugaEpoch;
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
    case Old:
    {
        return LDNone;
    }
    default:
        Assert( 0 && "Unexpected version of HinduLunisolarCalendar" );
        return LDNone;
    }
}

//=============================================================================

bool 
HinduLunisolarCalendar::IsMonthLeap( int month, long year )
{
    switch ( ms_version )
    {
    case Modern:
    {
        long jd = DLMLYToJulianDay( 15, false, month, true, year );
        int mnth = month;
        bool monthLeap = true;
        JulianDayToDLMLY( jd, 0, 0, &mnth, &monthLeap, 0 );
        return ((mnth == month) && monthLeap);
    }
    case Old:
    {
        double mina = (12. * year - 1) * s_solarMonth;
        double newYear = (std::floor( mina / s_lunarMonth ) + 1) * s_lunarMonth;
        int leapMonth = (int)( std::ceil( (newYear - mina)
                                                    / s_solarLunarMonthDiff ) );
        return (month == leapMonth);
    }
    default:
        Assert( 0 && "Unexpected version of HinduLunisolarCalendar" );
        return LDNone;
    }
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

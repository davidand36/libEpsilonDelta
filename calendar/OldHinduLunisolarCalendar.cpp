/*
  OldHinduLunisolarCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class OldHinduLunisolarCalendar, representing the old Hindu lunisolar
  calendar.
*/


#include "OldHinduLunisolarCalendar.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"
#include "DateJD.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace 
{

const double s_kaliYugaEpoch = 588466.; //JD of Kali Yuga (Iron Age) epoch
const double s_siderealYear = 1577917500. / 4320000.;
const double s_solarMonth = s_siderealYear / 12.;
const double s_lunarMonth = 1577917500. / 53433336.;
const double s_lunarDay = s_lunarMonth / 30.;
const double s_solarLunarMonthDiff = s_solarMonth - s_lunarMonth;

}

//=============================================================================


void 
OldHinduLunisolarCalendar::JulianDayToDMLY( int julianDay, 
                                            int * pDay,
                                            int * pMonth, bool * pMonthLeap,
                                            int * pYear )
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
        *pDay = static_cast< int >( day ) + 1;
    if ( pMonth )
        *pMonth = static_cast< int >( month ) + 1;
    if ( pMonthLeap )
        *pMonthLeap = monthLeap;
    if ( pYear )
        *pYear = static_cast< int >( std::ceil( year ) ) - 1;
}

//-----------------------------------------------------------------------------

int 
OldHinduLunisolarCalendar::DMLYToJulianDay( int day, int month, bool monthLeap,
                                            int year )
{
    double mina = (12. * year - 1) * s_solarMonth;
    double newYear = (std::floor( mina / s_lunarMonth ) + 1) * s_lunarMonth;
    if ( monthLeap
         || (std::ceil( (newYear - mina) / s_solarLunarMonthDiff ) > month) )
        --month;
    double jd = s_kaliYugaEpoch  +  newYear  +  month * s_lunarMonth
            +  (day - 1) * s_lunarDay  +  3. / 4.;
    return static_cast< int >( std::floor( jd ) );
}

//=============================================================================

bool 
OldHinduLunisolarCalendar::Valid( int day, int month, bool monthLeap,
                                  int year )
{
    if ( (month < 1) || (month > 12) )
        return false;
    if ( day < 1 )
        return false;
    if ( monthLeap && (month != LeapMonth( year )) )
        return false;
    if ( day > LastDayOfMonth( month, monthLeap, year ) )
        return false;
    if ( day == LostDay( month, monthLeap, year ) )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

int 
OldHinduLunisolarCalendar::MakeValid( int * pDay, int * pMonth,
                                      bool * pMonthLeap, int * pYear,
                                      DateFixup::EMethod fixupMethod )
{
    Assert( pDay && pMonth && pMonthLeap && pYear );
    if ( fixupMethod == DateFixup::Carry )
    {
        int julianDay = DMLYToJulianDay( *pDay, *pMonth, *pMonthLeap, *pYear );
        JulianDayToDMLY( julianDay, pDay, pMonth, pMonthLeap, pYear );
        return julianDay;
    }
    else    //DateFixup::Clamp
    {
        if ( *pMonth < 1 )
            *pMonth = 1;
        else if ( *pMonth > 12 )
            *pMonth = 12;
        if ( *pMonthLeap && (*pMonth != LeapMonth( *pYear )) )
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
        return DateJD::INVALID;
    }
}

//=============================================================================

int 
OldHinduLunisolarCalendar::LastDayOfMonth( int month, bool monthLeap,
                                           int year )
{
    int jd0 = DMLYToJulianDay( 1, month, monthLeap, year );
    int year1 = year;
    int month1 = month + 1;
    if ( monthLeap )
        --month1;
    if ( month1 > 12 )
    {
        month1 = 1;
        --year1;
    }
    int jd1 = DMLYToJulianDay( 1, month1, false, year1 );
    if ( jd1 > jd0 + 40 )
        jd1 = DMLYToJulianDay( 1, month1, true, year1 );
    Assert( jd1 < jd0 + 40 );
    int lastDay;
    JulianDayToDMLY( (jd1 - 1), &lastDay, 0, 0, 0 );
    return lastDay;
}

//-----------------------------------------------------------------------------

int 
OldHinduLunisolarCalendar::LostDay( int month, bool monthLeap, int year )
{
    static const double solarLunarDayDiff = 1. - s_lunarDay;
    int jd1 = DMLYToJulianDay( 1, month, monthLeap, year );
    double rise = jd1 - s_kaliYugaEpoch +  1. / 4.;
    double dayGap = ModRP( rise, s_lunarDay );
    return static_cast< int >( std::ceil( (1. - dayGap)
                                          / solarLunarDayDiff ) );
}

//-----------------------------------------------------------------------------

int 
OldHinduLunisolarCalendar::LeapMonth( int year )
{
    double mina = (12. * year - 1) * s_solarMonth;
    double newYear = (std::floor( mina / s_lunarMonth ) + 1) * s_lunarMonth;
    return static_cast< int >( std::ceil( (newYear - mina)
                                          / s_solarLunarMonthDiff ) );
}

//=============================================================================

const std::string & 
OldHinduLunisolarCalendar::MonthName( int month )
{
    Assert( (month >= 1) && (month <= 12) );
    return g_hinduLunarMonthNames[ month - 1 ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

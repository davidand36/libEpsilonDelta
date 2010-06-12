/*
  HinduSolarCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class HinduSolarCalendar, representing the modern Hindu solar calendar.
  Algorithms are based on Edward M. Reingold and Nachum Dershowitz,
  "Calendrical Calculations, the Millennium Edition".
*/


#include "HinduSolarCalendar.hpp"
#include "HinduSolarDate.hpp"
#include "HinduAstro.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace 
{

const int s_kaliYugaEpoch = 588466; //Julian day of Kali Yuga (Iron Age) epoch
const int s_sakaEra = 3179;         //Kali Yuga era year of Saka era year 0
const double s_siderealYear = 1577917500. / 4320000.;
const double s_solarMonth = s_siderealYear / 12.;

}

//=============================================================================

HinduSolarCalendar::EVersion HinduSolarCalendar::ms_version
    = HinduSolarCalendar::Modern;

//=============================================================================


void 
HinduSolarCalendar::JulianDayToDMY( int julianDay, 
                                    int * pDay, int * pMonth, int * pYear )
{
    switch ( ms_version )
    {
    case Modern:
    {
        int kaliYugaDay = julianDay - s_kaliYugaEpoch;
        //The Orissa rule is based on sunrise of the following day.
        //See Reingold & Dershowitz, p. 279, for this and other rules.
        double rise = HinduAstro::Sunrise( kaliYugaDay + 1 );
        int month = HinduAstro::Zodiac( rise );
        int year = KaliYugaYear( rise ) - s_sakaEra;
        double solarLong = HinduAstro::SolarLongitude( rise );
        int monthBegin = kaliYugaDay  -  3
                -  ModP( static_cast< int >( std::floor( solarLong ) ), 30 );
        while ( HinduAstro::Zodiac( HinduAstro::Sunrise( monthBegin + 1 ) )
                != month )
            ++monthBegin;
        int day = kaliYugaDay - monthBegin + 1;
        if ( pDay )
            *pDay = day;
        if ( pMonth )
            *pMonth = month;
        if ( pYear )
            *pYear = year;
        break;
    }
    case Old:
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
        break;
    }
    default:
        Assert( 0 && "Unexpected version of HinduSolarCalendar" );
        break;
    }
}

//-----------------------------------------------------------------------------

int 
HinduSolarCalendar::DMYToJulianDay( int day, int month, int year )
{
    switch ( ms_version )
    {
    case Modern:
    {
        HinduSolarDate hsDate0( day, month, year );
        int jd = static_cast< int >( std::floor( HinduAstro::SiderealYear() 
                                    * (year + s_sakaEra + (month - 1) / 12.) ) )
                +  s_kaliYugaEpoch  +  day  -  1;
        static const double rate = 360. / HinduAstro::SiderealYear();
        double phi = (month - 1) * 30  +  (day - 1) * rate;
        double delta = ModRP( (HinduAstro::SolarLongitude( jd + (1./ 4.) )
                               - phi + 180.),  360. )  -  180.;
        jd = jd  -  static_cast< int >( std::ceil( delta / rate ) );
        HinduSolarDate hsDate1( jd );
        while ( hsDate1 < hsDate0 )
            hsDate1.Set( ++jd );
        return jd;
    }
    case Old:
    {
        double jd = s_kaliYugaEpoch  +  year * s_siderealYear
                +  (month - 1) * s_solarMonth  +  day  -  1. / 4.;
        return static_cast< int >( std::floor( jd ) );
    }
    default:
        Assert( 0 && "Unexpected version of HinduSolarCalendar" );
        return 0;
    }
}

//=============================================================================

int 
HinduSolarCalendar::DaysInMonth( int month, int year )
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

//=============================================================================

const std::string & 
HinduSolarCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month >= 1) && (month <= 12) );
    return g_hinduSolarMonthNames[ month - 1 ];
}

//=============================================================================

int 
HinduSolarCalendar::KaliYugaYear( double kaliYugaTime )
{
    double kyYr = kaliYugaTime / HinduAstro::SiderealYear();
    double correction = HinduAstro::SolarLongitude( kaliYugaTime ) / 360.;
    int year = static_cast< int >( std::floor( kyYr - correction + 0.5 ) );
    return year;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

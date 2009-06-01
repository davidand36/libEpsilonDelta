/*
  HinduSolarCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class HinduSolarCalendar, representing the modern Hindu solar calendar.
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

}

//=============================================================================


void 
HinduSolarCalendar::JulianDayToDMY( int julianDay, 
                                    int * pDay, int * pMonth, int * pYear )
{
    int kaliYugaDay = julianDay - s_kaliYugaEpoch;
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
}

//-----------------------------------------------------------------------------

int 
HinduSolarCalendar::DMYToJulianDay( int day, int month, int year )
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

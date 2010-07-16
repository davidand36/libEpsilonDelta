/*
  HinduSolarCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class HinduSolarCalendar, representing the Hindu solar calendar.
  Algorithms are based on Edward M. Reingold and Nachum Dershowitz,
  "Calendrical Calculations, the Millennium Edition".
*/


#include "HinduSolarCalendar.hpp"
#include "HinduSolarDate.hpp"
#include "HinduAstro.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace 
{

const long s_kaliYugaEpoch = 588466; //Julian day of Kali Yuga (Iron Age) epoch
const long s_sakaEra = 3179;         //Kali Yuga era year of Saka era year 0
const double s_siderealYear = 1577917500. / 4320000.;
const double s_solarMonth = s_siderealYear / 12.;

}

//=============================================================================

HinduSolarCalendar::EVersion HinduSolarCalendar::ms_version
    = HinduSolarCalendar::Modern;

//=============================================================================


void 
HinduSolarCalendar::JulianDayToDMY( long julianDay, 
                                    int * pDay, int * pMonth, long * pYear )
{
    switch ( ms_version )
    {
    case Modern:
    {
        long kaliYugaDay = julianDay - s_kaliYugaEpoch;
        //The Orissa rule is based on sunrise of the following day.
        //See Reingold & Dershowitz, p. 279, for this and other rules.
        double rise = HinduAstro::Sunrise( kaliYugaDay + 1 );
        int month = HinduAstro::Zodiac( rise );
        long year = KaliYugaYear( rise ) - s_sakaEra;
        double solarLong = HinduAstro::SolarLongitude( rise );
        long monthBegin = (long)(kaliYugaDay  -  3
                                 -  ModP( (int)( floor( solarLong ) ), 30 ));
        while ( HinduAstro::Zodiac( HinduAstro::Sunrise( monthBegin + 1 ) )
                != month )
            ++monthBegin;
        int day = (int)(kaliYugaDay - monthBegin + 1);
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
        month = ModRP( floor( month ), 12. );
        if ( pDay )
            *pDay = (int)( floor( day ) ) + 1;
        if ( pMonth )
            *pMonth = (int)( month ) + 1;
        if ( pYear )
            *pYear = (long)( floor( year ) );
        break;
    }
    default:
        Assert( 0 && "Unexpected version of HinduSolarCalendar" );
        break;
    }
}

//-----------------------------------------------------------------------------

long 
HinduSolarCalendar::DMYToJulianDay( int day, int month, long year )
{
    switch ( ms_version )
    {
    case Modern:
    {
        HinduSolarDate hsDate0( day, month, year );
        long jd = (long)( floor( HinduAstro::SiderealYear() 
                                    * (year + s_sakaEra + (month - 1) / 12.) ) )
                +  s_kaliYugaEpoch  +  day  -  1;
        static const double rate = 360. / HinduAstro::SiderealYear();
        double phi = (month - 1) * 30  +  (day - 1) * rate;
        double delta = ModRP( (HinduAstro::SolarLongitude( jd + (1./ 4.) )
                               - phi + 180.),  360. )  -  180.;
        jd = jd  -  (long)( ceil( delta / rate ) );
        HinduSolarDate hsDate1( jd );
        while ( hsDate1 < hsDate0 )
            hsDate1.Set( ++jd );
        return jd;
    }
    case Old:
    {
        double jd = s_kaliYugaEpoch  +  year * s_siderealYear
                +  (month - 1) * s_solarMonth  +  day  -  1. / 4.;
        return (long)( floor( jd ) );
    }
    default:
        Assert( 0 && "Unexpected version of HinduSolarCalendar" );
        return 0;
    }
}

//=============================================================================

int 
HinduSolarCalendar::DaysInMonth( int month, long year )
{
    int month1 = month + 1;
    long year1 = year;
    if ( month1 > 12 )
    {
        month1 = 1;
        ++year1;
    }
    return  DMYToJulianDay( 1, month1, year1 )
            - DMYToJulianDay( 1, month, year );
}

//=============================================================================

const string & 
HinduSolarCalendar::MonthName( int month, long /*year*/ )
{
    Assert( (month >= 1) && (month <= 12) );
    return g_hinduSolarMonthNames[ month - 1 ];
}

//=============================================================================

long 
HinduSolarCalendar::KaliYugaYear( double kaliYugaTime )
{
    double kyYr = kaliYugaTime / HinduAstro::SiderealYear();
    double correction = HinduAstro::SolarLongitude( kaliYugaTime ) / 360.;
    long year = (long)( floor( kyYr - correction + 0.5 ) );
    return year;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

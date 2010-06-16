/*
  ChineseCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class ChineseCalendar, which defines the Chinese calendar.
*/


#include "ChineseCalendar.hpp"
#include "Assert.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#include "TimeIncrement.hpp"
#include "AstroPhenomena.hpp"
#include "TimeStandards.hpp"
#include "Angle.hpp"
#include "DateJD.hpp"
#include <string>
#include <vector>
#ifdef DEBUG
#include "Date.hpp"
#include "TestCheck.hpp"
#include <iostream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace
{                                                                   //namespace

const long s_chineseEpoch = 758326;   //15 Feb -2636 Gregorian
const long s_chineseEpochWS = 758269; //20 Dec -2637 Gregorian
//About 1 Jan 1929, switched from using local Beijing time to standard zone.
const long s_switchToStdZone = 2425613;
const double s_localZone = TimeIncrement( 7, 45, 40. ).Days() + 0.5;
const double s_stdZone = TimeIncrement( 8, 0, 0. ).Days() + 0.5;
const double s_tropicalYear = 365.2421896698;
const double s_synodicMonth = 29.5305888531;

double JDItoJD( long jdi );
long JDtoJDI( double jd );
Angle SolarLongitude( long jdi );
Angle LunarPhase( long jdi );
long NextWinterSolstice( long jdi );
long PriorNewMoon( long jdi );
long NextNewMoon( long jdi );
int MajorSolarTerm( long jdi );

}                                                                   //namespace


//=============================================================================


void 
ChineseCalendar::JulianDayToDMYL( long julianDay,
                                  int * pDay, int * pMonth, long * pYear,
                                  int * pLeapMonth )
{
    long lastWS = NextWinterSolstice( julianDay + 1 );
    long lastNM = PriorNewMoon( lastWS );
    lastNM = PriorNewMoon( lastNM - 1 );
    long firstWS = NextWinterSolstice( lastWS - 370 );
    long firstNM = PriorNewMoon( firstWS );
    long currNM = PriorNewMoon( julianDay );
    int day = (int)(julianDay - currNM + 1);
    int month = (int)(((currNM - firstNM) / s_synodicMonth) + 0.5 );
    long year
           = (long)(((firstWS - s_chineseEpochWS) / s_tropicalYear) + 0.5 ) + 1;
    int leapMonth = LMUnknown;
    int monthsInYear = (int)(((lastNM - firstNM) / s_synodicMonth) + 0.5 ) + 1;
    bool prevSolYear = false;
    bool nextSolYear = false;
    if ( monthsInYear == 12 )
    {
        --month;
        if ( month <= 0 )
        {
            month += 12;
            --year;
            //Need to check previous solar year for leap month.
            lastWS = firstWS;
            lastNM = PriorNewMoon( firstNM );
            lastNM = PriorNewMoon( lastNM - 1 );
            firstWS = NextWinterSolstice( lastWS - 370 );
            firstNM = PriorNewMoon( firstWS );
            monthsInYear
                    = (int)(((lastNM - firstNM) / s_synodicMonth) + 0.5 ) + 1;
            if ( monthsInYear == 12 )
                leapMonth = LMNone;
            else    //A leap month will be found in the next section.
            {
                ++month;
                prevSolYear = true;
            }
        }
        else
        {
            //Need to check following solar year for leap month.
            firstWS = lastWS;
            firstNM = PriorNewMoon( firstWS );
            lastWS = NextWinterSolstice( firstWS + 1 );
            lastNM = PriorNewMoon( lastWS );
            lastNM = PriorNewMoon( lastNM - 1 );
            monthsInYear
                    = (int)(((lastNM - firstNM) / s_synodicMonth) + 0.5 ) + 1;
            if ( monthsInYear == 12 )
                leapMonth = LMNone;
            else    //A leap month may be found in the next section.
                nextSolYear = true;
        }
    }
    if ( leapMonth == LMUnknown )
    {
        Assert( monthsInYear == 13 );
        //Search for leap month
        // (= first lunar month w/ no change in major solar term).
        long nm = NextNewMoon( firstNM + 1 );
        //Month begins w/solar term at end of previous day.
        int startMajor = MajorSolarTerm( nm - 1 );
        for ( int m = 1; m < 13; ++m )
        {
            if ( nextSolYear && (m >= 3) )
            {
                leapMonth = LMNone; //Leap month belongs to following year.
                break;
            }
            nm = NextNewMoon( nm + 1 );
            int endMajor = MajorSolarTerm( nm - 1 );
            if ( endMajor == startMajor )
            {
                if ( m < 3 )
                {
                    if ( prevSolYear )
                        leapMonth = LMNone; //Leap month belongs to prev year.
                    else
                    {
                        if ( ! nextSolYear )
                            month -= 2;
                        if ( month <= 0 )
                        {
                            month += 13;
                            --year;
                            leapMonth = m + 11;
                        }
                        else if ( nextSolYear )
                            leapMonth = m + 11;
                        else
                            leapMonth = LMNone;
                    }
                }
                else
                {
                    if ( ! prevSolYear )
                        --month;
                    if ( month <= 0 )
                    {
                        month += 12;
                        --year;
                        leapMonth = LMNone; //Consecutive years can't be leap.
                    }
                    else
                        leapMonth = m - 1;
                }
                break;
            }
            startMajor = endMajor;
        }
    }
    Assert( leapMonth != LMUnknown );
    if ( pDay )
        *pDay = day;
    if ( pMonth )
        *pMonth = month;
    if ( pYear )
        *pYear = year;
    if ( pLeapMonth )
        *pLeapMonth = leapMonth;
}

//-----------------------------------------------------------------------------

void 
ChineseCalendar::JulianDayToDMLY( long julianDay,
                                  int * pDay, int * pMonth, bool * pLeap,
                                  long * pYear )
{
    int leapMonth;
    JulianDayToDMYL( julianDay, pDay, pMonth, pYear, &leapMonth );
    *pLeap = ( leapMonth == *pMonth );
    if ( leapMonth <= *pMonth )
        --*pMonth;
}

//-----------------------------------------------------------------------------

long 
ChineseCalendar::DMYToJulianDay( int day, int month, long year )
{
    long firstWS
            = (long)( s_chineseEpochWS  +  (year - 1) * s_tropicalYear - 10 );
    firstWS = NextWinterSolstice( firstWS );
    long firstNM = PriorNewMoon( firstWS );
    long lastWS = NextWinterSolstice( firstWS + 1 );
    long lastNM = PriorNewMoon( lastWS );
    lastNM = PriorNewMoon( lastNM - 1 );
    int monthsInYear = (int)(((lastNM - firstNM) / s_synodicMonth) + 0.5 ) + 1;
    long newYear;
    if ( monthsInYear == 12 )
    {
        long m12 = NextNewMoon( firstNM + 1 );
        newYear = NextNewMoon( m12 + 1 );
    }
    else
    {
        Assert( monthsInYear == 13 );
        long nm = NextNewMoon( firstNM + 1 );
        int startMajor = MajorSolarTerm( nm - 1 );
        int m = 11;
        for ( int i = 0; i < 4; ++i )   //Limit iterations to be safe.
        {
            long nextNM = NextNewMoon( nm + 1 );
            int endMajor = MajorSolarTerm( nextNM - 1 );
            if ( endMajor != startMajor )
                if ( ++m > 12 )
                    break;
            nm = nextNM;
            startMajor = endMajor;
        }
        Assert( m == 13 );
        newYear = nm;
    }
    long jd = newYear  +  (month - 1) * 29;
    jd = NextNewMoon( jd ) + day - 1;
    return jd;
}

//-----------------------------------------------------------------------------

long 
ChineseCalendar::DMLYToJulianDay( int day, int month, bool leap, long year )
{
    int leapMonth = LeapMonth( year );
    if ( leap || (month >= leapMonth) )
        ++month;
    return DMYToJulianDay( day, month, year );
}

//=============================================================================

bool 
ChineseCalendar::Valid( int day, int month, long year, int leapMonth )
{
    if ( (day < 1) || (day > 30) || (month < 1) || (month > 13) )
        return false;
    int leap = LeapMonth( year );
    if ( (leapMonth != LMUnknown) && (leapMonth != leap) )
        return false;
    if ( (leap == LMNone) && (month > 12) )
        return false;
    int daysInMonth = DaysInMonth( month, year );
    if ( day > daysInMonth )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

long 
ChineseCalendar::MakeValid( int * pDay, int * pMonth, long * pYear,
                            int * pLeapMonth, DateFixup::EMethod fixupMethod )
{
    Assert( pDay && pMonth && pYear && pLeapMonth );
    if ( fixupMethod == DateFixup::Carry )
    {
        long julianDay = DMYToJulianDay( *pDay, *pMonth, *pYear );
        JulianDayToDMYL( julianDay, pDay, pMonth, pYear, pLeapMonth );
        return julianDay;
    }
    else    //DateFixup::Clamp
    {
        *pLeapMonth = LeapMonth( *pYear );
        if ( *pMonth < 1 )
            *pMonth = 1;
        else if ( *pLeapMonth == LMNone )
        {
            if ( *pMonth > 12 )
                *pMonth = 12;
        }
        else if ( *pMonth > 13 )
            *pMonth = 13;
        if ( *pDay < 1 )
            *pDay = 1;
        else
        {
            int daysInMonth = DaysInMonth( *pMonth, *pYear );
            if ( *pDay > daysInMonth )
                *pDay = daysInMonth;
        }
        return DateJD::INVALID;
    }
}

//=============================================================================

int 
ChineseCalendar::LeapMonth( long year )
{
    long firstWS = (long)( s_chineseEpochWS
                                      +  (year - 1) * s_tropicalYear - 10 );
    firstWS = NextWinterSolstice( firstWS );
    long firstNM = PriorNewMoon( firstWS );
    long lastWS = NextWinterSolstice( firstWS + 1 );
    long lastNM = PriorNewMoon( lastWS );
    lastNM = PriorNewMoon( lastNM - 1 );
    int monthsInYear = (int)(((lastNM - firstNM) / s_synodicMonth) + 0.5 ) + 1;
    bool firstSolarYear = true;
    if ( monthsInYear == 12 )
    {
        firstWS = lastWS;
        firstNM = PriorNewMoon( firstWS );
        lastWS = NextWinterSolstice( firstWS + 1 );
        lastNM = PriorNewMoon( lastWS );
        lastNM = PriorNewMoon( lastNM - 1 );
        monthsInYear = (int)(((lastNM - firstNM) / s_synodicMonth) + 0.5 ) + 1;
        if ( monthsInYear == 12 )
            return LMNone;
        firstSolarYear = false;
    }
    Assert( monthsInYear == 13 );
    long nm = NextNewMoon( firstNM + 1 );
    int startMajor = MajorSolarTerm( nm - 1 );
    for ( int m = 1; m < 13; ++m )
    {
        long nextNM = NextNewMoon( nm + 1 );
        int endMajor = MajorSolarTerm( nextNM - 1 );
        if ( endMajor == startMajor )
        {
            if ( firstSolarYear )
            {
                if ( m < 3 )
                    return LMNone;  //Leap month was in previous year.
                else
                    return (m - 1);
            }
            else
            {
                if ( m < 3 )
                    return (m + 11);
                else
                    return LMNone;  //Leap month is in following year.
            }
        }
        nm = nextNM;
        startMajor = endMajor;
    }
    Assert( 0 && "No leap month in year of 13 moons!" );
#ifndef DEBUG   //Just to avoid compiler warnings.
    return LMUnknown;
#endif
}

//-----------------------------------------------------------------------------

int 
ChineseCalendar::DaysInMonth( int month, long year )
{
    return  (DMYToJulianDay( 1, (month + 1), year )
             - DMYToJulianDay( 1, month, year ));
}

//=============================================================================

void 
ChineseCalendar::SolarTerms( long julianDay,
                             int * pMajorTerm, int * pMinorTerm )
{
    //Add 1 to date, because the solar term is based on the solar longitude
    // reached by the end of the day.
    Angle solarLong = SolarLongitude( julianDay + 1 );
    if ( pMajorTerm )
    {
        //Add 30 degrees, because the first major solar term (Yushui) begins
        // with solar longitude 330 deg.
        Angle adjSolarLong = solarLong + Angle( M_PI / 6. );
        adjSolarLong.NormalizePositive( );
        double quot = DivRP( adjSolarLong.Radians(), (M_PI / 6.) );
        *pMajorTerm = (int)( quot ) + 1;
    }
    if ( pMinorTerm )
    {
        //Add 45 degrees, because the first minor solar term (Lichun) begins
        // with solar longitude 315 deg.
        Angle adjSolarLong = solarLong + Angle( M_PI / 4. );
        adjSolarLong.NormalizePositive( );
        double quot = DivRP( adjSolarLong.Radians(), (M_PI / 6.) );
        *pMinorTerm = (int)( quot ) + 1;
    }
}

//-----------------------------------------------------------------------------

long 
ChineseCalendar::JDofNextSolarTerm( long julianDay, int term, bool major )
{
    Angle solarLongOfTerm = Angle( (term - 1) * (M_PI / 6.)
                                 -  (major  ?  (M_PI / 6.)  :  (M_PI / 4.)) );
    double jd = JDItoJD( julianDay );
    const double spring2000 = 2451623.8159722;
    double term2000 = spring2000  +  solarLongOfTerm.Cycles() * s_tropicalYear;
    double offset = ModRP( (term2000 - jd), s_tropicalYear );
    if ( offset < 10. )
    {
        //Initial estimate might bring us to the previous term.
        if ( (SolarLongitude( julianDay ) - solarLongOfTerm).Radians() > 0. )
            offset = s_tropicalYear;
    }
    else if ( offset > 355. )
    {
        //Initial estimate might be a year too advanced.
        if ( (SolarLongitude( julianDay ) - solarLongOfTerm).Radians() <= 0. )
            offset = 0.;
    }
    long jdi = JDtoJDI( jd + offset );    //first estimate
    Angle diff = SolarLongitude( jdi ) - solarLongOfTerm;
    if ( diff.Radians() < 0. )
        while ( (SolarLongitude( jdi + 1 ) - solarLongOfTerm).Radians() < 0. )
            ++jdi;
    else
        while ( diff.Radians() > 0. )
            diff = SolarLongitude( --jdi ) - solarLongOfTerm;
    return jdi;
}

//=============================================================================

long 
ChineseCalendar::SexagesimalToLinear( int cyclical, long cycle )
{
    return  60 * (cycle - 1)  +  cyclical;
}

//-----------------------------------------------------------------------------

void 
ChineseCalendar::LinearToSexagesimal( long linear,
                                      int * pCyclical, long * pCycle )
{
    long cycle;
    long cyclical;
    DivModP( (linear - 1), 60L, &cycle, &cyclical );
    if ( pCyclical )
        *pCyclical = (int)(cyclical + 1);
    if ( pCycle )
        *pCycle = cycle + 1;
}

//-----------------------------------------------------------------------------

void 
ChineseCalendar::StemAndBranch( int cyclical, int * pCelestialStem,
                                int * pTerrestrialBranch )
{
    --cyclical;
    if ( pCelestialStem )
        *pCelestialStem = ModP( cyclical, 10 ) + 1;
    if ( pTerrestrialBranch )
        *pTerrestrialBranch = ModP( cyclical, 12 ) + 1;
}

//-----------------------------------------------------------------------------

int 
ChineseCalendar::StemToElement( int celestialStem )
{
    return ((celestialStem - 1) / 2);
}

//-----------------------------------------------------------------------------

int 
ChineseCalendar::DayCyclical( long julianDay )
{
    int cyclical;
    LinearToSexagesimal( (julianDay + 40), &cyclical, 0 );
    return cyclical;
}

//-----------------------------------------------------------------------------

int 
ChineseCalendar::MonthCyclical( int month, long year )
{
    int cyclical;
    LinearToSexagesimal( (12 * year + month + 44), &cyclical, 0 );
    return cyclical;
}

//-----------------------------------------------------------------------------

void 
ChineseCalendar::DayStemAndBranch( long julianDay,
                                   int * pCelestialStem,
                                   int * pTerrestrialBranch )
{
    int cyclical = DayCyclical( julianDay );
    StemAndBranch( cyclical, pCelestialStem, pTerrestrialBranch );
}

//-----------------------------------------------------------------------------

void 
ChineseCalendar::MonthStemAndBranch( int month, long year,
                                     int * pCelestialStem,
                                     int * pTerrestrialBranch )
{
    int cyclical = MonthCyclical( month, year );
    StemAndBranch( cyclical, pCelestialStem, pTerrestrialBranch );
}

//-----------------------------------------------------------------------------

void 
ChineseCalendar::YearStemAndBranch( long year,
                                    int * pCelestialStem,
                                    int * pTerrestrialBranch )
{
    int cyclical;
    LinearToSexagesimal( year, &cyclical, 0 );
    StemAndBranch( cyclical, pCelestialStem, pTerrestrialBranch );
}

//=============================================================================

const std::string & 
ChineseCalendar::MajorSolarTermName( int term, bool english )
{
    if ( english )
        return g_chineseMajorSolarTermEnglishNames[ term - 1 ];
    else
        return g_chineseMajorSolarTermNames[ term - 1 ];
}

//-----------------------------------------------------------------------------

const std::string & 
ChineseCalendar::MinorSolarTermName( int term, bool english )
{
    if ( english )
        return g_chineseMinorSolarTermEnglishNames[ term - 1 ];
    else
        return g_chineseMinorSolarTermNames[ term - 1 ];
}

//-----------------------------------------------------------------------------

const std::string & 
ChineseCalendar::CelestialStemName( int stem, bool english )
{
    if ( english )
        return g_chineseCelestialStemEnglishNames[ stem - 1 ];
    else
        return g_chineseCelestialStemNames[ stem - 1 ];
}

//-----------------------------------------------------------------------------

const std::string & 
ChineseCalendar::TerrestrialBranchName( int branch, bool english )
{
    if ( english )
        return g_chineseTerrestrialBranchEnglishNames[ branch - 1 ];
    else
        return g_chineseTerrestrialBranchNames[ branch - 1 ];
}

//-----------------------------------------------------------------------------

const std::string & 
ChineseCalendar::ElementName( int element, bool english )
{
    if ( english )
        return g_chineseElementEnglishNames[ element ];
    else
        return g_chineseElementNames[ element ];
}

//=============================================================================

namespace
{                                                                 /*namespace*/

//-----------------------------------------------------------------------------

double
JDItoJD( long jdi )
{
    double tdb_ut = TDB_UT( jdi ).Days();
    if ( jdi < s_switchToStdZone )
        return  jdi - s_localZone + tdb_ut;
    else
        return  jdi - s_stdZone + tdb_ut;
}

//-----------------------------------------------------------------------------

long 
JDtoJDI( double jd )
{
    double ut_tdb = - TDB_UT( jd ).Days();
    if ( jd < s_switchToStdZone )
        return  (long)( std::floor( jd + s_localZone + ut_tdb ) );
    else
        return  (long)( std::floor( jd + s_stdZone + ut_tdb ) );
}

//=============================================================================

Angle
SolarLongitude( long jdi )
{
    double jd = JDItoJD( jdi );
    return EpsilonDelta::SolarLongitude( jd );
}

//-----------------------------------------------------------------------------

Angle 
LunarPhase( long jdi )
{
    double jd = JDItoJD( jdi );
    return EpsilonDelta::LunarPhase( jd );
}

//-----------------------------------------------------------------------------

long 
NextWinterSolstice( long jdi )
{
    return ChineseCalendar::JDofNextSolarTerm( jdi, 11, true );
}

//-----------------------------------------------------------------------------

long 
PriorNewMoon( long jdi )
{
    Angle lunarPhase = LunarPhase( jdi + 1 );
    lunarPhase.NormalizePositive( );
    double offset = lunarPhase.Cycles() * s_synodicMonth;
    long nm = JDtoJDI( JDItoJD( jdi ) - offset );    //first estimate
    lunarPhase = LunarPhase( nm );
    if ( lunarPhase.Radians() < 0 )
        while ( LunarPhase( nm + 1 ).Radians() < 0. )
            ++nm;
    else
        while ( lunarPhase.Radians() > 0. )
            lunarPhase = LunarPhase( --nm );
    return nm;
}

//-----------------------------------------------------------------------------

long 
NextNewMoon( long jdi )
{
    Angle lunarPhase = - LunarPhase( jdi );
    lunarPhase.NormalizePositive( );
    double offset = lunarPhase.Cycles() * s_synodicMonth;
    long nm = JDtoJDI( JDItoJD( jdi ) + offset );    //first estimate
    lunarPhase = LunarPhase( nm );
    if ( lunarPhase.Radians() < 0 )
        while ( LunarPhase( nm + 1 ).Radians() < 0. )
            ++nm;
    else
        while ( lunarPhase.Radians() > 0. )
            lunarPhase = LunarPhase( --nm );
    return nm;
}

//-----------------------------------------------------------------------------

int
MajorSolarTerm( long jdi )
{
    int majorTerm;
    ChineseCalendar::SolarTerms( jdi, &majorTerm, 0 );
    return majorTerm;
}

//-----------------------------------------------------------------------------

}                                                                 /*namespace*/

//=============================================================================

#ifdef DEBUG

bool 
ChineseCalendar::Test( )
{
    bool ok = true;
    cout << "Testing ChineseCalendar" << endl;

    int d = 22;  int m = December;  long y = 1989;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    long jd = Date( d, m, y ).JulianDay();
    TESTCHECK( NextWinterSolstice( jd ),
               Date( 22, December, 1989 ).JulianDay(), &ok );
    y = 1990;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( NextWinterSolstice( jd ),
               Date( 22, December, 1990 ).JulianDay(), &ok );
    d = 22;  y = 1989;
    jd = Date( d, m, y ).JulianDay();
    cout << "New moons beginning Date( " << d << ", " << m << ", " << y << " )" << endl;
    long nm = NextNewMoon( jd );
    TESTCHECK( nm, Date( 28, December, 1989 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 28, December, 1989 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 27, January, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 27, January, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 25, February, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 25, February, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 27, March, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 27, March, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 25, April, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 25, April, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 24, May, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 24, May, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 23, June, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 23, June, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 22, July, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 22, July, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 20, August, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 20, August, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 19, September, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 19, September, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 18, October, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 18, October, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 17, November, 1990 ).JulianDay(), &ok );
    nm = NextNewMoon( nm + 1 );
    TESTCHECK( PriorNewMoon( nm - 1 ), Date( 17, November, 1990 ).JulianDay(),
               &ok );
    TESTCHECK( nm, Date( 17, December, 1990 ).JulianDay(), &ok );

    d = 20;  m = January;  y = 1990;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 11, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 12, &ok );
    d = 19;  m = February;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 12, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 1, &ok );
    d = 21;  m = March;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 1, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 2, &ok );
    d = 20;  m = April;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 2, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 3, &ok );
    d = 21;  m = May;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 3, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 4, &ok );
    d = 21;  m = June;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 4, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 5, &ok );
    d = 23;  m = July;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 5, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 6, &ok );
    d = 23;  m = August;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 6, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 7, &ok );
    d = 23;  m = September;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 7, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 8, &ok );
    d = 24;  m = October;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 8, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 9, &ok );
    d = 22;  m = November;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 9, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 10, &ok );
    d = 22;  m = December;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    TESTCHECK( MajorSolarTerm( jd - 1 ), 10, &ok );
    TESTCHECK( MajorSolarTerm( jd ), 11, &ok );

    d = 20; m = December; y = -2637;
    cout << "Date( " << d << ", " << m << ", " << y << " )" << endl;
    jd = Date( d, m, y ).JulianDay();
    cout << " Next WS: " << NextWinterSolstice( jd ) << endl;

    if ( ok )
        cout << "ChineseCalendar PASSED." << endl << endl;
    else
        cout << "ChineseCalendar FAILED." << endl << endl;
    return ok;
    
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

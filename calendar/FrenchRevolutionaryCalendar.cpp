/*
  FrenchRevolutionaryCalendar.cpp
  Copyright (C) 2007 David M. Anderson

  Class FrenchRevolutionaryCalendar, which defines the FrenchRevolutionary calendar.
*/


#include "FrenchRevolutionaryCalendar.hpp"
#include "DivMod.hpp"
#include "CalendarLibText.hpp"
#include "TimeIncrement.hpp"
#include "AstroPhenomena.hpp"
#include "TimeStandards.hpp"
#include "Angle.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace
{
const long s_frenchRevolutionaryEpoch = 2375840;
//Local time in Paris + noon-to-midnight offset
const double s_localZone = TimeIncrement( 0, 9, 21. ).Days() + 0.5;
const double s_tropicalYear = 365.2421896698;

double JDItoJD( long jdi );
long JDtoJDI( double jd );
Angle SolarLongitude( long jdi );
long PriorAutumnalEquinox( long jdi );
}


//=============================================================================


void
FrenchRevolutionaryCalendar::JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 236-7.*/
    long newYear = PriorAutumnalEquinox( julianDay );
    long year = (long)( floor(((newYear - s_frenchRevolutionaryEpoch)
                               / s_tropicalYear) + 0.5 ) );
    int month;
    int day;
    DivModP( (int)(julianDay - newYear), 30, &month, &day );
    if ( pDay )
        *pDay = day + 1;
    if ( pMonth )
        *pMonth = month + 1;
    if ( pYear )
        *pYear = year + 1;
}

//-----------------------------------------------------------------------------

long
FrenchRevolutionaryCalendar::DMYToJulianDay( int day, int month, long year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 236.*/
    long jdi = (long)( (year - 1) * s_tropicalYear )
            +  s_frenchRevolutionaryEpoch  +  180;
    long newYear = PriorAutumnalEquinox( jdi );
    return  newYear  +  (month - 1) * 30  +  day  -  1;
}

//=============================================================================

bool 
FrenchRevolutionaryCalendar::IsLeapYear( long year )
{
    long daysInYear = DMYToJulianDay( 1, 1, (year + 1) )
            - DMYToJulianDay( 1, 1, year );
    return ( daysInYear > 365 );
}

//=============================================================================

int
FrenchRevolutionaryCalendar::DaysInMonth( int month, long year )
{
    Assert( (month > 0) && (month <= MonthsInYear( year )) );
    if ( month < 13 )
        return 30;
    else if ( IsLeapYear( year ) )
        return 6;
    else
        return 5;
        
}

//-----------------------------------------------------------------------------

const string &
FrenchRevolutionaryCalendar::MonthName( int month, long /*year*/ )
{
    Assert( (month > 0) && (month <= 13) );
    return g_frenchRevolutionaryMonthNames[ month - 1 ];
}

//-----------------------------------------------------------------------------

const string & 
FrenchRevolutionaryCalendar::DayName( int day, int month, long /*year*/ )
{
    if ( month < 13 )
        return  g_frenchRevolutionaryDecadeNames[ (day - 1) % 10 ];
    else
        return  g_frenchRevolutionarySansculottidesNames[ day - 1 ];
}

//=============================================================================

namespace
{                                                                 /*namespace*/

//-----------------------------------------------------------------------------

double
JDItoJD( long jdi )
{
    double tdb_ut = TDB_UT( jdi ).Days();
    return  jdi - s_localZone + tdb_ut;
}

//-----------------------------------------------------------------------------

long 
JDtoJDI( double jd )
                         {
    double ut_tdb = - TDB_UT( jd ).Days();
    return  (long)( floor( jd + s_localZone + ut_tdb ) );
}

//=============================================================================

Angle
SolarLongitude( long jdi )
{
    double jd = JDItoJD( jdi );
    return EpsilonDelta::SolarLongitude( jd );
}

//-----------------------------------------------------------------------------

long 
PriorAutumnalEquinox( long jdi )
{
    double jd = JDItoJD( jdi );
    const Angle autumnLong( M_PI );
    const double autumn2000 = 2451810.22708333;
    double offset = ModRP( (jd - autumn2000), s_tropicalYear );
    if ( offset < 10. )
    {
        //Initial estimate might be a year off.
        if ( (SolarLongitude( jdi ) - autumnLong).Radians() < 0. )
            offset = s_tropicalYear;
    }
    else if ( offset > 355. )
    {
        //Initial estimate might be a year off.
        if ( (SolarLongitude( jdi ) - autumnLong).Radians() > 0. )
            offset = 0.;
    }
    jdi = JDtoJDI( jd - offset );
    Angle diff = SolarLongitude( jdi ) - autumnLong;
    if ( diff.Radians() < 0. )
        while ( (SolarLongitude( jdi + 1 ) - autumnLong).Radians() < 0. )
            ++jdi;
    else
        while ( diff.Radians() > 0. )
            diff = (SolarLongitude( --jdi ) - autumnLong).Radians();
    return jdi;
}

//-----------------------------------------------------------------------------

}                                                                 /*namespace*/


//*****************************************************************************

}                                                      //namespace EpsilonDelta

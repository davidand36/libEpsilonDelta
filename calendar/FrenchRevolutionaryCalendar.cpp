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
const int s_frenchRevolutionaryEpoch = 2375840;
//Local time in Paris + noon-to-midnight offset
const double s_localZone = TimeIncrement( 0, 9, 21. ).Days() + 0.5;
const double s_tropicalYear = 365.2421896698;

double JDItoJD( int jdi );
int JDtoJDI( double jd );
Angle SolarLongitude( int jdi );
int PriorAutumnalEquinox( int jdi );
}


//=============================================================================


void
FrenchRevolutionaryCalendar::JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 236-7.*/
    int newYear = PriorAutumnalEquinox( julianDay );
    int year = static_cast< int >( std::floor(
                                       ((newYear - s_frenchRevolutionaryEpoch)
                                        / s_tropicalYear) + 0.5 ) );
    int month;
    int day;
    DivModP( (julianDay - newYear), 30, &month, &day );
    if ( pDay )
        *pDay = day + 1;
    if ( pMonth )
        *pMonth = month + 1;
    if ( pYear )
        *pYear = year + 1;
}

//-----------------------------------------------------------------------------

int
FrenchRevolutionaryCalendar::DMYToJulianDay( int day, int month, int year )
{
    /*Adapted from Nachum Dershowitz and Edward M. Reingold,
      "Calendrical Calculations - Millennium Ed.", p. 236.*/
    int jdi = static_cast< int >( (year - 1) * s_tropicalYear )
            +  s_frenchRevolutionaryEpoch  +  180;
    int newYear = PriorAutumnalEquinox( jdi );
    return  newYear  +  (month - 1) * 30  +  day  -  1;
}

//=============================================================================

bool 
FrenchRevolutionaryCalendar::IsLeapYear( int year )
{
    int daysInYear = DMYToJulianDay( 1, 1, (year + 1) )
            - DMYToJulianDay( 1, 1, year );
    return ( daysInYear > 365 );
}

//=============================================================================

int
FrenchRevolutionaryCalendar::DaysInMonth( int month, int year )
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
FrenchRevolutionaryCalendar::MonthName( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= 13) );
    return g_frenchRevolutionaryMonthNames[ month - 1 ];
}

//-----------------------------------------------------------------------------

const std::string & 
FrenchRevolutionaryCalendar::DayName( int day, int month, int /*year*/ )
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
JDItoJD( int jdi )
{
    double tdb_ut = TDB_UT( jdi ).Days();
    return  jdi - s_localZone + tdb_ut;
}

//-----------------------------------------------------------------------------

int 
JDtoJDI( double jd )
                         {
    double ut_tdb = - TDB_UT( jd ).Days();
    return  static_cast<int>( std::floor( jd + s_localZone + ut_tdb ) );
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
PriorAutumnalEquinox( int jdi )
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

/*
  OldHinduLunisolarDate.hpp
  Copyright (C) 2007 David M. Anderson

  OldHinduLunisolarDate class, representing a date in the old Hindu lunisolar
  calendar.
*/


#include "OldHinduLunisolarDate.hpp"
#include "OldHinduLunisolarCalendar.hpp"
#include "HinduWeek.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include <string>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


OldHinduLunisolarDate::OldHinduLunisolarDate( bool today )
    :   DateJD( false )
{
    Set( today );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OldHinduLunisolarDate::OldHinduLunisolarDate( int julianDay )
    :   DateJD( false )
{
    Set( julianDay );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

OldHinduLunisolarDate::OldHinduLunisolarDate( const DateJD & date )
    :   DateJD( false )
{
    Set( date.JulianDay() );
}

//=============================================================================

OldHinduLunisolarDate & 
OldHinduLunisolarDate::operator=( const DateJD & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

void 
OldHinduLunisolarDate::Set( bool today )
{
    if ( today )
    {
        DateJD::Set( true );
        Assert( DateJD::Valid() );
        Calendar::JulianDayToDMLY( DateJD::JulianDay(),
                                   &m_day, &m_month, &m_monthLeap, &m_year );
    }
    else
    {
        DateJD::Set( false );
        Set( 1, 1, false, 0 );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
OldHinduLunisolarDate::Set( int julianDay )
{
    DateJD::Set( julianDay );
    Calendar::JulianDayToDMLY( julianDay,
                               &m_day, &m_month, &m_monthLeap, &m_year );
}

//=============================================================================

bool 
OldHinduLunisolarDate::Valid( ) const
{
    return Calendar::Valid( m_day, m_month, m_monthLeap, m_year );
}

//-----------------------------------------------------------------------------

void 
OldHinduLunisolarDate::MakeValid( DateFixup::EMethod fixupMethod )
{
    int julianDay = Calendar::MakeValid( &m_day, &m_month, &m_monthLeap,
                                         &m_year, fixupMethod );
    DateJD::Set( julianDay );
}

//=============================================================================

int 
OldHinduLunisolarDate::JulianDay( ) const
{
    if ( ! DateJD::Valid() )
        m_julianDay = Calendar::DMLYToJulianDay( m_day, m_month, m_monthLeap,
                                                 m_year );
    return DateJD::JulianDay();
}

//=============================================================================

int
OldHinduLunisolarDate::DayOfWeek( ) const
{
    return ModF( (JulianDay( ) + Week::DayOfWeekOfJD0()), Week::DaysInWeek() );
}

//-----------------------------------------------------------------------------

int
OldHinduLunisolarDate::DaysUntilWeekday( int weekday, int n ) const
{
    int daysUntilNext = - ModC( (JulianDay( ) + Week::DayOfWeekOfJD0()
                                 - weekday), Week::DaysInWeek() );
    return ( daysUntilNext + n * Week::DaysInWeek() );
}

//=============================================================================

void 
OldHinduLunisolarDate::Increment( int days )
{
    m_day += days;
    MakeValid( DateFixup::Carry );
}

//=============================================================================

std::string 
OldHinduLunisolarDate::m_defaultFormat = ""; //!!!

//-----------------------------------------------------------------------------

std::string 
OldHinduLunisolarDate::ToString( const std::string & format ) const
{
    return format; //!!!
}

//=============================================================================

#ifdef DEBUG

bool 
TestOldHinduLunisolarDate( )
{
    bool ok = true;
    cout << "Testing OldHinduLunisolarDate" << endl;

    int jd;
    int d, m, y;
    bool l;
    OldHinduLunisolarDate ohlDate;

    struct
    {
        int julianDay;
        int day;
        int month;
        bool leap;
        int year;
    } 
    testDates[]
            = {
                { 1507232,  11,  6, false, 2515 },
                { 1660038,  26,  9, false, 2933 },
                { 1746894,   3,  8, false, 3171 },
                { 1770642,   9,  8, false, 3236 },
                { 1892732,  19, 11,  true, 3570 },
                { 1931580,   5,  3, false, 3677 },
                { 1974852,  15,  9, false, 3795 },
                { 2091165,   7,  2, false, 4114 },
                { 2121510,  24,  2, false, 4197 },
                { 2155780,   9,  1, false, 4291 },
                { 2174030,   9, 12, false, 4340 },
                { 2191585,  23,  1, false, 4389 },
                { 2195262,   8,  2, false, 4399 },
                { 2229275,   2,  4, false, 4492 },
                { 2245581,   7, 11, false, 4536 },
                { 2266101,   3,  1, false, 4593 },
                { 2288543,   2,  7, false, 4654 },
                { 2290902,  29, 11, false, 4660 },
                { 2323141,  20,  3, false, 4749 },
                { 2334849,   4,  4, false, 4781 },
                { 2348021,   6,  5, false, 4817 },
                { 2366979,   5,  4, false, 4869 },
                { 2385649,  12,  5, false, 4920 },
                { 2392826,  13,  1,  true, 4940 },
                { 2416224,  23,  1, false, 5004 },
                { 2425849,  21,  5, false, 5030 },
                { 2430267,   9,  7, false, 5042 },
                { 2430834,  15,  1, false, 5044 },
                { 2431005,   9,  7, false, 5044 },
                { 2448699,  14, 12, false, 5092 },
                { 2450139,   7, 12, false, 5096 },
                { 2465738,  14,  8, false, 5139 },
                { 2486077,   6,  4, false, 5195 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        m = testDates[i].month;
        l = testDates[i].leap;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << m <<  ", " << l << ", " << y
             << " ) :" << endl;
        ohlDate.Set( d, m, l, y );
        TESTCHECK( ohlDate.Valid( ), true, &ok );
        TESTCHECK( ohlDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        ohlDate.Set( jd );
        TESTCHECK( ohlDate.Valid( ), true, &ok );
        TESTCHECK( ohlDate.Day( ), d, &ok );
        TESTCHECK( ohlDate.Month( ), m, &ok );
        TESTCHECK( ohlDate.IsMonthLeap( ), l, &ok );
        TESTCHECK( ohlDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "OldHinduLunisolarDate PASSED." << endl << endl;
    else
        cout << "OldHinduLunisolarDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

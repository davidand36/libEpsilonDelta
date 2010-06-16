/*
  HinduLunisolarDate.hpp
  Copyright (C) 2007 David M. Anderson

  HinduLunisolarDate class, representing a date in the old Hindu lunisolar
  calendar.
*/


#include "HinduLunisolarDate.hpp"
#include "HinduLunisolarCalendar.hpp"
#include "HinduWeek.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#include <string>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Array.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


HinduLunisolarDate::HinduLunisolarDate( bool today )
    :   DateJD( false )
{
    Set( today );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

HinduLunisolarDate::HinduLunisolarDate( long julianDay )
    :   DateJD( false )
{
    Set( julianDay );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

HinduLunisolarDate::HinduLunisolarDate( const DateJD & date )
    :   DateJD( false )
{
    Set( date.JulianDay() );
}

//=============================================================================

HinduLunisolarDate & 
HinduLunisolarDate::operator=( const DateJD & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

void 
HinduLunisolarDate::Set( bool today )
{
    if ( today )
    {
        DateJD::Set( true );
        Assert( DateJD::Valid() );
        Calendar::JulianDayToDLMLY( DateJD::JulianDay(),
                                    &m_day, &m_dayLeap,
                                    &m_month, &m_monthLeap, &m_year );
    }
    else
    {
        DateJD::Set( false );
        Set( 1, false, 1, false, 0 );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
HinduLunisolarDate::Set( long julianDay )
{
    DateJD::Set( julianDay );
    Calendar::JulianDayToDLMLY( julianDay,
                                &m_day, &m_dayLeap,
                                &m_month, &m_monthLeap, &m_year );
}

//=============================================================================

bool 
HinduLunisolarDate::Valid( ) const
{
    return Calendar::Valid( m_day, m_dayLeap, m_month, m_monthLeap, m_year );
}

//-----------------------------------------------------------------------------

void 
HinduLunisolarDate::MakeValid( DateFixup::EMethod fixupMethod )
{
    long julianDay = Calendar::MakeValid( &m_day, &m_dayLeap,
                                         &m_month, &m_monthLeap,
                                         &m_year, fixupMethod );
    DateJD::Set( julianDay );
}

//=============================================================================

long 
HinduLunisolarDate::JulianDay( ) const
{
    if ( ! DateJD::Valid() )
        m_julianDay = Calendar::DLMLYToJulianDay( m_day, m_dayLeap,
                                                  m_month, m_monthLeap,
                                                  m_year );
    return DateJD::JulianDay();
}

//=============================================================================

int
HinduLunisolarDate::DayOfWeek( ) const
{
    return (int)ModF( (JulianDay( ) + Week::DayOfWeekOfJD0()),
                      (long)Week::DaysInWeek() );
}

//-----------------------------------------------------------------------------

int
HinduLunisolarDate::DaysUntilWeekday( int weekday, int n ) const
{
    int daysUntilNext
            = - (int)ModC( (JulianDay( ) + Week::DayOfWeekOfJD0() - weekday),
                           (long)Week::DaysInWeek() );
    return ( daysUntilNext + n * Week::DaysInWeek() );
}

//=============================================================================

void 
HinduLunisolarDate::Increment( int days )
{
    m_day += days;
    MakeValid( DateFixup::Carry );
}

//=============================================================================

std::string 
HinduLunisolarDate::m_defaultFormat = ""; //!!!

//-----------------------------------------------------------------------------

std::string 
HinduLunisolarDate::ToString( const std::string & format ) const
{
    return format; //!!!
}

//=============================================================================

#ifdef DEBUG

bool 
TestHinduLunisolarDate( )
{
    bool ok = true;
    cout << "Testing HinduLunisolarDate" << endl;

    long jd;
    int d, m;
    long y;
    bool ld, lm;
    HinduLunisolarDate hlDate;

    cout << "Modern:" << endl;
    
    struct
    {
        long julianDay;
        int day;
        bool dayLeap;
        int month;
        bool monthLeap;
        long year;
    } 
    modernTestDates[]
            = {
                { 1507232,  11, false,  6, false, -529 },
                { 1660038,  27, false,  9, false, -111 },
                { 1746894,   3, false,  8, false,  127 },
                { 1770642,   9, false,  8, false,  192 },
                { 1892732,  19, false, 11, false,  526 },
                { 1931580,   5, false,  3, false,  633 },
                { 1974852,  15, false,  9, false,  751 },
                { 2091165,   6, false,  2, false, 1070 },
                { 2121510,  23, false,  3,  true, 1153 },
                { 2155780,   8, false,  1, false, 1247 },
                { 2174030,   8, false,  1, false, 1297 },
                { 2191585,  22, false,  1, false, 1345 },
                { 2195262,   8, false,  2, false, 1355 },
                { 2229275,   1, false,  4, false, 1448 },
                { 2245581,   7, false, 11, false, 1492 },
                { 2266101,   3, false,  2,  true, 1549 },
                { 2288543,   2, false,  7, false, 1610 },
                { 2290902,  28,  true, 11, false, 1616 },
                { 2323141,  20, false,  3, false, 1705 },
                { 2334849,   4, false,  4, false, 1737 },
                { 2348021,   6, false,  5, false, 1773 },
                { 2366979,   5, false,  4, false, 1825 },
                { 2385649,  11, false,  5, false, 1876 },
                { 2392826,  13, false,  1, false, 1896 },
                { 2416224,  22, false,  1, false, 1960 },
                { 2425849,  20, false,  5, false, 1986 },
                { 2430267,   9, false,  7, false, 1998 },
                { 2430834,  14, false,  1, false, 2000 },
                { 2431005,   8, false,  7, false, 2000 },
                { 2448699,  14, false, 12, false, 2048 },
                { 2450139,   7, false, 12, false, 2052 },
                { 2465738,  14, false,  8, false, 2095 },
                { 2486077,   6, false,  4, false, 2151 }
            };

    for ( int i = 0; i < ARRAY_LENGTH( modernTestDates ); ++i )
    {
        jd = modernTestDates[i].julianDay;
        d = modernTestDates[i].day;
        ld = modernTestDates[i].dayLeap;
        m = modernTestDates[i].month;
        lm = modernTestDates[i].monthLeap;
        y = modernTestDates[i].year;
        cout << "Set( " << d << ", " << ld << ", " << m <<  ", " << lm
             << ", " << y << " ) :" << endl;
        hlDate.Set( d, ld, m, lm, y );
        TESTCHECK( hlDate.Valid( ), true, &ok );
        TESTCHECK( hlDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        hlDate.Set( jd );
        TESTCHECK( hlDate.Valid( ), true, &ok );
        TESTCHECK( hlDate.Day( ), d, &ok );
        TESTCHECK( hlDate.IsDayLeap( ), ld, &ok );
        TESTCHECK( hlDate.Month( ), m, &ok );
        TESTCHECK( hlDate.IsMonthLeap( ), lm, &ok );
        TESTCHECK( hlDate.Year( ), y, &ok );
    }

    cout << "Old:" << endl;
    HinduLunisolarCalendar::SetVersion( HinduLunisolarCalendar::Old );
    
    struct
    {
        long julianDay;
        int day;
        int month;
        bool leap;
        long year;
    } 
    oldTestDates[]
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

    for ( int i = 0; i < ARRAY_LENGTH( oldTestDates ); ++i )
    {
        jd = oldTestDates[i].julianDay;
        d = oldTestDates[i].day;
        m = oldTestDates[i].month;
        lm = oldTestDates[i].leap;
        y = oldTestDates[i].year;
        cout << "Set( " << d << ", false, " << m <<  ", " << lm << ", " << y
             << " ) :" << endl;
        hlDate.Set( d, false, m, lm, y );
        TESTCHECK( hlDate.Valid( ), true, &ok );
        TESTCHECK( hlDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        hlDate.Set( jd );
        TESTCHECK( hlDate.Valid( ), true, &ok );
        TESTCHECK( hlDate.Day( ), d, &ok );
        TESTCHECK( hlDate.IsDayLeap( ), false, &ok );
        TESTCHECK( hlDate.Month( ), m, &ok );
        TESTCHECK( hlDate.IsMonthLeap( ), lm, &ok );
        TESTCHECK( hlDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "HinduLunisolarDate PASSED." << endl << endl;
    else
        cout << "HinduLunisolarDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

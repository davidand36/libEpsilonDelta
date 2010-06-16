/*
  ISO8601Date.cpp
  Copyright (C) 2007 David M. Anderson

  ISO8601Date class, representing a date in the ISO 8601 calendar.
*/


#include "ISO8601Date.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif

using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


ISO8601Date::ISO8601Date( bool today )
    :   DateJD( false )
{
    Set( today );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ISO8601Date::ISO8601Date( long julianDay )
    :   DateJD( false )
{
    Set( julianDay );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

ISO8601Date::ISO8601Date( const DateJD & date )
    :   DateJD( false )
{
    Set( date.JulianDay() );
}

//=============================================================================

ISO8601Date & 
ISO8601Date::operator=( const DateJD & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

void 
ISO8601Date::Set( bool today )
{
    if ( today )
    {
        DateJD::Set( true );
        Assert( DateJD::Valid() );
        Calendar::JulianDayToDWY( DateJD::JulianDay(),
                                  &m_day, &m_week, &m_year );
    }
    else
    {
        DateJD::Set( false );
        Set( 1, 1, 1 );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
ISO8601Date::Set( long julianDay )
{
    DateJD::Set( julianDay );
    Calendar::JulianDayToDWY( julianDay,
                              &m_day, &m_week, &m_year );
}

//=============================================================================

bool 
ISO8601Date::Valid( ) const
{
    if ( (m_day < 1) || (m_day > Calendar::DaysInWeek()) )
        return false;
    if ( (m_week  < 1) || (m_week > Calendar::WeeksInYear( m_year )) )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

void 
ISO8601Date::MakeValid( DateFixup::EMethod fixupMethod )
{
    int daysInWeek = Calendar::DaysInWeek( );
    if ( fixupMethod & DateFixup::ClampWeek )
    {
        if ( m_week < 1 )
            m_week = 1;
        else
        {
            int weeksInYear = Calendar::WeeksInYear( m_year );
            if ( m_week > weeksInYear )
                m_week = weeksInYear;
        }
    }
    else
    {
        while ( m_week < 1 )
        {
            --m_year;
            m_week += Calendar::WeeksInYear( m_year );
        }
        int weeksInYear = Calendar::WeeksInYear( m_year );
        while ( m_week > weeksInYear )
        {
            m_week -= weeksInYear;
            ++m_year;
            weeksInYear = Calendar::WeeksInYear( m_year );
        }
    }
    if ( fixupMethod & DateFixup::ClampDay )
    {
        if ( m_day < 1 )
            m_day = 1;
        if ( m_day > daysInWeek )
            m_day = daysInWeek;
    }
    else
    {
        while ( m_day < 1 )
        {

            --m_week;
            if ( m_week < 1 )
            {
                --m_year;
                m_week = Calendar::WeeksInYear( m_year );
            }
            Assert( (m_week >= 1)
                    && (m_week <= Calendar::WeeksInYear( m_year )) );
            m_day += daysInWeek;
        }
        while ( m_day > daysInWeek )
        {
            m_day -= daysInWeek;
            ++m_week;
            if ( m_week > Calendar::WeeksInYear( m_year ) )
            {
                m_week = 1;
                ++m_year;
            }
            Assert( (m_week >= 1)
                    && (m_week <= Calendar::WeeksInYear( m_year )) );
        }
    }
    Assert( Valid() );
}    

//=============================================================================

long 
ISO8601Date::JulianDay( ) const
{
    if ( ! DateJD::Valid() )
        m_julianDay = Calendar::DWYToJulianDay( m_day, m_week, m_year );
    return DateJD::JulianDay();
}

//=============================================================================

void 
ISO8601Date::Increment( int days )
{
    DateJD::Increment( days );
    m_day += days;
    MakeValid( DateFixup::Carry );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
ISO8601Date::Increment( int days, int weeks, long years,
                        DateFixup::EMethod fixupMethod )
{
    DateJD::Set( DateJD::INVALID );
    m_day += days;
    m_week += weeks;
    m_year += years;
    MakeValid( fixupMethod );
}

//=============================================================================

std::string 
ISO8601Date::m_defaultFormat = ""; //!!!

//-----------------------------------------------------------------------------

std::string 
ISO8601Date::ToString( const std::string & format ) const
{
    return format; //!!!
}

//=============================================================================

#ifdef DEBUG

bool 
TestISO8601Date( )
{
    bool ok = true;
    cout << "Testing ISO8601Date" << endl;

    long jd;
    int d, w;
    long y;
    ISO8601Date isoDate;

    struct
    {
        long julianDay;
        int day;
        int week;
        long year;
    } 
    testDates[]
            = {
                { 1507232,   7, 29, -586 },
                { 1660038,   3, 49, -168 },
                { 1746894,   3, 39,   70 },
                { 1770642,   7, 39,  135 },
                { 1892732,   3,  2,  470 },
                { 1931580,   1, 21,  576 },
                { 1974852,   6, 45,  694 },
                { 2091165,   7, 16, 1013 },
                { 2121510,   7, 21, 1096 },
                { 2155780,   5, 12, 1190 },
                { 2174030,   6, 10, 1240 },
                { 2191585,   5, 14, 1288 },
                { 2195262,   7, 17, 1298 },
                { 2229275,   7, 23, 1391 },
                { 2245581,   3,  5, 1436 },
                { 2266101,   6, 14, 1492 },
                { 2288543,   6, 38, 1553 },
                { 2290902,   6,  9, 1560 },
                { 2323141,   3, 24, 1648 },
                { 2334849,   7, 26, 1680 },
                { 2348021,   5, 30, 1716 },
                { 2366979,   7, 24, 1768 },
                { 2385649,   1, 31, 1819 },
                { 2392826,   3, 13, 1839 },
                { 2416224,   7, 16, 1903 },
                { 2425849,   7, 34, 1929 },
                { 2430267,   1, 40, 1941 },
                { 2430834,   1, 16, 1943 },
                { 2431005,   4, 40, 1943 },
                { 2448699,   2, 12, 1992 },
                { 2450139,   7,  8, 1996 },
                { 2465738,   3, 45, 2038 },
                { 2486077,   7, 28, 2094 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        jd = testDates[i].julianDay;
        d = testDates[i].day;
        w = testDates[i].week;
        y = testDates[i].year;
        cout << "Set( " << d << ", " << w << ", " << y << " ) :" << endl;
        isoDate.Set( d, w, y );
        TESTCHECK( isoDate.Valid( ), true, &ok );
        TESTCHECK( isoDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        isoDate.Set( jd );
        TESTCHECK( isoDate.Valid( ), true, &ok );
        TESTCHECK( isoDate.Day( ), d, &ok );
        TESTCHECK( isoDate.Week( ), w, &ok );
        TESTCHECK( isoDate.Year( ), y, &ok );
    }

    if ( ok )
        cout << "ISO8601Date PASSED." << endl << endl;
    else
        cout << "ISO8601Date FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

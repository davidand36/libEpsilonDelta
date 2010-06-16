/*
  BadiDate.cpp
  Copyright (C) 2007 David M. Anderson

  BadiDate class, representing a date in the Badi calendar.
*/


#include "BadiDate.hpp"
#include "BadiCalendar.hpp"
#include "BahaiDate.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


BadiDate::BadiDate( bool today )
{
    Set( today );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BadiDate::BadiDate( long julianDay )
    :   JDDate( false )
{
    JDDate::Set( julianDay );
    BadiCalendar::JulianDayToDMYVK( JulianDay(), &m_day, &m_month,
                                    &m_year, &m_vahid, &m_kulliShay );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BadiDate::BadiDate( int day, int month, int year, int vahid, long kulliShay )
    :   JDDate( false ),
        m_day( day ),
        m_month( month ),
        m_year( year ),
        m_vahid( vahid ),
        m_kulliShay( kulliShay )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BadiDate::BadiDate( const JDDate & date )
    :   JDDate( false )
{
    JDDate::Set( date.JulianDay() );
    BadiCalendar::JulianDayToDMYVK( JulianDay(), &m_day, &m_month,
                                    &m_year, &m_vahid, &m_kulliShay );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BadiDate::BadiDate( const BadiDate & date )
    :   JDDate( false )
{
    Set( date.m_day, date.m_month, date.m_year, date.m_vahid, date.m_kulliShay );
    if ( date.JDDate::Valid() )
        JDDate::Set( date.JulianDay() );
}

//-----------------------------------------------------------------------------

BadiDate::~BadiDate( )
{
}

//=============================================================================

void 
BadiDate::Set( bool today )
{
    if ( today )
    {
        JDDate::Set( true );
        Assert( JDDate::Valid() );
        BadiCalendar::JulianDayToDMYVK( JulianDay(), &m_day, &m_month,
                                        &m_year, &m_vahid, &m_kulliShay );
    }
    else
    {
        JDDate::Set( false );
        Set( 1, 1, 1, 1, 1 );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
BadiDate::Set( long julianDay )
{
    JDDate::Set( julianDay );
    BadiCalendar::JulianDayToDMYVK( JulianDay(), &m_day, &m_month,
                                    &m_year, &m_vahid, &m_kulliShay );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
BadiDate::Set( int day, int month, int year, int vahid, long kulliShay )
{
    JDDate::Set( JDDate::INVALID );
    m_day = day;
    m_month = month;
    m_year = year;
    m_vahid = vahid;
    m_kulliShay = kulliShay;
}

//=============================================================================

BadiDate & 
BadiDate::operator=( const JDDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.JulianDay() );
    return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

BadiDate & 
BadiDate::operator=( const BadiDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.m_day, rhs.m_month, rhs.m_year, rhs.m_vahid, rhs.m_kulliShay );
    if ( rhs.JDDate::Valid() )
        JDDate::Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

bool 
BadiDate::Valid( ) const
{
    if ( (m_vahid < 1) || (m_vahid > BadiCalendar::VahidsInKulliShay()) )
        return false;
    if ( (m_year < 1) || (m_year > BadiCalendar::YearsInVahid()) )
        return false;
    if ( (m_month < 1) || (m_month > BadiCalendar::MonthsInYear()) )
        return false;
    if ( (m_day < 1)
         || (m_day > BadiCalendar::DaysInMonth( m_month, m_year,
                                                m_vahid, m_kulliShay )) )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

void 
BadiDate::MakeValid( DateFixup::EMethod fixupMethod )
{
    if ( fixupMethod == DateFixup::Clamp )
    {
        if ( m_vahid < 1 )
            m_vahid = 1;
        else if ( m_vahid > BadiCalendar::VahidsInKulliShay() )
            m_vahid = BadiCalendar::VahidsInKulliShay();
        if ( m_year < 1 )
            m_year = 1;
        else if ( m_year > BadiCalendar::YearsInVahid() )
            m_year = BadiCalendar::YearsInVahid();
    }
    long yr = BadiCalendar::YVKToYear( m_year, m_vahid, m_kulliShay );
    BahaiDate bahaiDate( m_day, m_month, yr );
    bahaiDate.MakeValid( fixupMethod );
    m_day = bahaiDate.Day();
    m_month = bahaiDate.Month();
    BadiCalendar::YearToYVK( bahaiDate.Year(),
                             &m_year, &m_vahid, &m_kulliShay );
    Assert( Valid() );
}

//=============================================================================

long 
BadiDate::JulianDay( ) const
{
    if ( ! JDDate::Valid() )
        m_julianDay = BadiCalendar::DMYVKToJulianDay( m_day, m_month, m_year,
                                                      m_vahid, m_kulliShay );
    return JDDate::JulianDay();
}

//=============================================================================

int 
BadiDate::DayOfWeek( ) const
{
    return (int)ModF( (JulianDay( ) + BahaiWeek::DayOfWeekOfJD0()),
                      (long)BahaiWeek::DaysInWeek() );
}

//-----------------------------------------------------------------------------

int 
BadiDate::DaysUntilWeekday( int weekday, int n ) const
{
    long daysUntilNext
            = - ModC( (JulianDay( ) + BahaiWeek::DayOfWeekOfJD0() - weekday),
                      (long)BahaiWeek::DaysInWeek() );
    return (int)( daysUntilNext  +  n * BahaiWeek::DaysInWeek() );
}

//=============================================================================

void 
BadiDate::Increment( int days )
{
    JDDate::Increment( days );
    m_day += days;
    MakeValid( DateFixup::Carry );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
BadiDate::Increment( int days, int months,
                     int years, int vahids, long kulliShays,
                     DateFixup::EMethod fixupMethod )
{
    JDDate::Set( JDDate::INVALID );
    m_day += days;
    m_month += months;
    m_year += years;
    m_vahid += vahids;
    m_kulliShay += kulliShays;
    MakeValid( fixupMethod );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
BadiDate::Increment( int weekday, int n )
{
    int days = DaysUntilWeekday( weekday, n );
    Increment( days );
}

//=============================================================================

std::string 
BadiDate::ToString( const std::string & format ) const
{
    std::string dateString;
    for ( std::string::const_iterator p = format.begin(); p != format.end(); ++p )
    {
        if ( *p == '%' )
        {
            int width = 0;
            bool zeroFill = false;
            ++p;
            if ( isdigit( *p ) )
            {
                if ( *p == '0' )
                {
                    zeroFill = true;
                    ++p;
                }
                while ( isdigit( *p ) )
                {
                    width = width * 10 + (*p - '0');
                    ++p;
                }
            }
            switch ( *p )
            {
            case 'd':
            {
                dateString += IntToString( m_day, width, 0, zeroFill );
                break;
            }
            case 'D':
            {
                const std::string & dayName
                    = BadiCalendar::DayName( m_day );
                if ( width == 0 )
                    dateString += dayName;
                else
                    dateString += dayName.substr( 0, width );
                break;
            }
            case 'm':
            {
                dateString += IntToString( m_month, width, 0, zeroFill );
                break;
            }
            case 'M':
            {
                const std::string & monthName
                    = BadiCalendar::MonthName( m_month );
                if ( width == 0 )
                    dateString += monthName;
                else
                    dateString += monthName.substr( 0, width );
                break;
            }
            case 'y':
            {
                dateString += IntToString( m_year, width, 0, zeroFill );
                break;
            }
            case 'Y':
            {
                const std::string & yearName
                    = BadiCalendar::YearName( m_year );
                if ( width == 0 )
                    dateString += yearName;
                else
                    dateString += yearName.substr( 0, width );
                break;
            }
            case 'v':
            {
                dateString += IntToString( m_vahid, width, 0, zeroFill );
                break;
            }
            case 'V':
            {
                dateString += OrdinalToString( m_vahid, width );
                break;
            }
            case 'k':
            {
                dateString += IntToString( m_kulliShay, width, 0, zeroFill );
                break;
            }
            case 'K':
            {
                dateString += OrdinalToString( m_kulliShay, width );
                break;
            }
            case 'w':
            {
                dateString += IntToString( DayOfWeek(), width, 0, zeroFill );
                break;
            }
            case 'W':
            {
                const std::string & weekdayName
                    = BahaiWeek::WeekDayName( DayOfWeek() );
                if ( weekdayName.length() == 0 )
                    dateString += IntToString( DayOfWeek(), width, 0,
                                               zeroFill );
                else if ( width == 0 )
                    dateString += weekdayName;
                else
                    dateString += weekdayName.substr( 0, width );
                break;
            }
            default:
                
                break;
            } //switch
        }
        else
        {
            dateString += *p;
        }
    }
    return dateString;
}

//-----------------------------------------------------------------------------

std::string 
BadiDate::m_defaultFormat = "%W, the day of %D, of the month of %M, of the year %Y, of the %V Vahid, of the %K Kull-i-Shay";

//-----------------------------------------------------------------------------

void 
BadiDate::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

const std::string & 
BadiDate::DefaultFormat( )
{
    return m_defaultFormat;
}

//=============================================================================

bool operator==( const BadiDate & lhs, const BadiDate & rhs )
{
    return ( (lhs.Day() == rhs.Day()) && (lhs.Month() == rhs.Month())
             && (lhs.Year() == rhs.Year()) && (lhs.Vahid() == rhs.Vahid())
             && (lhs.KulliShay() == rhs.KulliShay()) );
}

//-----------------------------------------------------------------------------

bool operator<( const BadiDate & lhs, const BadiDate & rhs )
{
    if ( lhs.KulliShay() < rhs.KulliShay() )
        return true;
    else if ( lhs.KulliShay() > rhs.KulliShay() )
        return false;
    else if ( lhs.Vahid() < rhs.Vahid() )
        return true;
    else if ( lhs.Vahid() > rhs.Vahid() )
        return false;
    else if ( lhs.Year() < rhs.Year() )
        return true;
    else if ( lhs.Year() > rhs.Year() )
        return false;
    else if ( lhs.Month() < rhs.Month() )
        return true;
    else if ( lhs.Month() > rhs.Month() )
        return false;
    else
        return ( lhs.Day() < rhs.Day() );
}

//=============================================================================

#ifdef DEBUG

bool 
TestBadiDate( )
{
    bool ok = true;
    cout << "Testing BadiDate" << endl;

    struct
    {
        long julianDay;
        int day;
        int month;
        int year;
        int vahid;
        long kulliShay;
    } 
    testDates[]
            = {
                { 1507232,  12,  7,  3,  6, -6 },
                { 1660038,  13, 14,  3,  9, -5 },
                { 1746894,  17, 10, 13,  2, -4 },
                { 1770642,   6, 11,  2,  6, -4 },
                { 1892732,   9, 16, 13,  4, -3 },
                { 1931580,   4,  4,  6, 10, -3 },
                { 1974852,   7, 13, 10, 16, -3 },
                { 2091165,  17,  2,  6, 14, -2 },
                { 2121510,   8,  4, 13, 18, -2 },
                { 2155780,   3,  1, 12,  4, -1 },
                { 2174030,   9, 20,  4,  7, -1 },
                { 2191585,  13,  1, 15,  9, -1 },
                { 2195262,  19,  2,  6, 10, -1 },
                { 2229275,   8,  5,  4, 15, -1 },
                { 2245581,  16, 17, 10, 17, -1 },
                { 2266101,   1,  2, 10,  1,  0 },
                { 2288543,  12, 10, 14,  4,  0 },
                { 2290902,   4, 20,  1,  5,  0 },
                { 2323141,   6,  5, 14,  9,  0 },
                { 2334849,   7,  6,  8, 11,  0 },
                { 2348021,  12,  7,  6, 13,  0 },
                { 2366979,  15,  5,  1, 16,  0 },
                { 2385649,   2,  8, 14, 18,  0 },
                { 2392826,   7,  1, 15, 19,  0 },
                { 2416224,  11,  2,  3,  4,  1 },
                { 2425849,   6,  9, 10,  5,  1 },
                { 2430267,   3, 11,  3,  6,  1 },
                { 2430834,  11,  2,  5,  6,  1 },
                { 2431005,  11, 11,  5,  6,  1 },
                { 2448699,  16, 20, 15,  8,  1 },
                { 2450139,  19, 18, 19,  8,  1 },
                { 2465738,   7, 13,  5, 11,  1 },
                { 2486077,   6,  7,  4, 14,  1 }
            };

    for ( int i = 0; i < sizeof(testDates)/sizeof(testDates[0]); ++i )
    {
        long jd = testDates[i].julianDay;
        int d = testDates[i].day;
        int m = testDates[i].month;
        int y = testDates[i].year;
        int v = testDates[i].vahid;
        long k = testDates[i].kulliShay;
        cout << "BadiDate( " << d << ", " << m << ", " << y << ", " << v
             << ", " << k << " ) :" << endl;
        BadiDate badiDate( d, m, y, v, k );
        TESTCHECK( badiDate.Valid( ), true, &ok );
        TESTCHECK( badiDate.JulianDay( ), jd, &ok );
        cout << "Set( " << jd << " ) :" << endl;
        badiDate.Set( jd );
        TESTCHECK( badiDate.Valid( ), true, &ok );
        TESTCHECK( badiDate.Day( ), d, &ok );
        TESTCHECK( badiDate.Month( ), m, &ok );
        TESTCHECK( badiDate.Year( ), y, &ok );
        TESTCHECK( badiDate.Vahid( ), v, &ok );
        TESTCHECK( badiDate.KulliShay( ), k, &ok );
    }


    if ( ok )
        cout << "BadiDate PASSED." << endl << endl;
    else
        cout << "BadiDate FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

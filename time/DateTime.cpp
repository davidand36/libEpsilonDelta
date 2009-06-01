/*
  DateTime.cpp
  Copyright (C) 2007 David M. Anderson

  DateTime class, consisting of a (Gregorian) date and a time of day.
*/


#include "DateTime.hpp"
#include <ctime>
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Assert.hpp"
#include <cstdio>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

DateTime::DateTime( bool now )
    :    m_date( false ),
         m_time( false )
{
    Set( now );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DateTime::DateTime( std::time_t stdTime )
{
    Set( stdTime );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DateTime::DateTime( const Date & date, const Time & time )
    :    m_date( date ),
         m_time( time )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DateTime::DateTime( int day, int month, int year,
                    int hour, int minute, double second )
    :    m_date( day, month, year ),
         m_time( hour, minute, second )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

DateTime::DateTime( double julianDay )
{
    Set( julianDay );
}

//-----------------------------------------------------------------------------

DateTime::~DateTime( )
{
}

//=============================================================================

void 
DateTime::Set( bool now )
{
    if ( now )
    {
        std::time_t t = std::time( 0 );
        Assert( t != std::time_t( -1 ) );
        Set( t );
    }
    else
    {
        m_date.Set( false );
        m_time.Set( false );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
DateTime::Set( std::time_t stdTime )
{
    std::tm * pNow = std::localtime( &stdTime );
    Set( pNow->tm_mday, (pNow->tm_mon + 1), (pNow->tm_year + 1900),
         pNow->tm_hour, pNow->tm_min, pNow->tm_sec );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
DateTime::Set( const Date & date, const Time & time )
{
    m_date = date;
    m_time = time;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
DateTime::Set( int day, int month, int year,
               int hour, int minute, double second )
{
    m_date.Set( day, month, year );
    m_time.Set( hour, minute, second );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
DateTime::Set( double julianDay )
{
    double whole;
    double fraction = modf( julianDay, &whole );
    fraction += 0.5;
    if ( fraction >= 1. )
    {
        fraction -= 1.;
        whole += 1.;
    }
    m_date.Set( static_cast<int>( whole ) );
    m_time.Set( fraction );
}

//=============================================================================

bool 
DateTime::Valid( ) const
{
    return ( m_date.Valid( ) && m_time.Valid( ) );
}

//-----------------------------------------------------------------------------

void 
DateTime::MakeValid( )
{
    int days = m_time.MakeValid( );
    m_date.Increment( days );
}

//=============================================================================

Date
DateTime::GetDate( ) const
{
    return m_date;
}

//-----------------------------------------------------------------------------

Time 
DateTime::GetTime( ) const
{
    return m_time;
}

//-----------------------------------------------------------------------------

int 
DateTime::Day( ) const
{
    return m_date.Day( );
}

//-----------------------------------------------------------------------------

int 
DateTime::Month( ) const
{
    return m_date.Month( );
}

//-----------------------------------------------------------------------------

int 
DateTime::Year( ) const
{
    return m_date.Year( );
}

//-----------------------------------------------------------------------------

int 
DateTime::Hour( ) const
{
    return m_time.Hour( );
}

//-----------------------------------------------------------------------------

int 
DateTime::Minute( ) const
{
    return m_time.Minute( );
}

//-----------------------------------------------------------------------------

double 
DateTime::Second( ) const
{
    return m_time.Second( );
}

//-----------------------------------------------------------------------------

double 
DateTime::JulianDay( ) const
{
    return ( m_date.JulianDay( ) + m_time.Days( ) - 0.5 );
}

//-----------------------------------------------------------------------------

int 
DateTime::DayOfWeek( ) const
{
    return m_date.DayOfWeek( );
}

//=============================================================================

void
DateTime::Increment( int days, int months, int years,
                     int hours, int minutes, double seconds )
{
    days += m_time.Increment( hours, minutes, seconds );
    m_date.Increment( days, months, years );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
DateTime::Increment( const TimeIncrement & increment )
{
    int days = m_time.Increment( increment );
    m_date.Increment( days );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
DateTime::Increment( double days )
{
    TimeIncrement increment( days );
    int d = m_time.Increment( increment );
    m_date.Increment( d );
}

//=============================================================================

bool 
operator==( const DateTime & lhs, const DateTime & rhs )
{
    Assert( lhs.Valid( ) && rhs.Valid( ) );
    return ( (lhs.GetDate() == rhs.GetDate())
             && (lhs.GetTime() == rhs.GetTime()) );
}

//-----------------------------------------------------------------------------

bool 
operator<( const DateTime & lhs, const DateTime & rhs )
{
    Assert( lhs.Valid( ) && rhs.Valid( ) );
    return ( (lhs.GetDate() < rhs.GetDate())
             || ((lhs.GetDate() == rhs.GetDate())
                 && (lhs.GetTime() < rhs.GetTime())) );
}

//=============================================================================

#ifdef DEBUG

bool 
DateTime::Test( )
{
    bool ok = true;
    cout << "Testing DateTime" << endl;

    cout << "DateTime() [default constructor]" << endl;
    DateTime dateTime;
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 1, &ok );
    TESTCHECK( dateTime.Month( ), 1, &ok );
    TESTCHECK( dateTime.Year( ), 1, &ok );
    TESTCHECK( dateTime.Hour( ), 0, &ok );
    TESTCHECK( dateTime.Minute( ), 0, &ok );
    TESTCHECK( dateTime.Second( ), 0., &ok );
    TESTCHECK( dateTime.JulianDay( ), 1721425.5, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), 1, &ok );

    cout << "Set( true )" << endl;
    dateTime.Set( true );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    cout << "dateTime.Date().ToString()=" << dateTime.GetDate().ToString( )
         << "\tOK?" << endl;
    cout << "dateTime.Time().ToString()=" << dateTime.GetTime().ToString( )
         << "\tOK?" << endl;

    cout << "Set( false ) :" << endl;
    dateTime.Set( false );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 1, &ok );
    TESTCHECK( dateTime.Month( ), 1, &ok );
    TESTCHECK( dateTime.Year( ), 1, &ok );
    TESTCHECK( dateTime.Hour( ), 0, &ok );
    TESTCHECK( dateTime.Minute( ), 0, &ok );
    TESTCHECK( dateTime.Second( ), 0., &ok );
    TESTCHECK( dateTime.JulianDay( ), 1721425.5, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), Monday, &ok );

    int d = 9;
    int m = 2;
    int y = 1601;
    int h = 44;
    int mm = 13;
    double s = 16.;
    cout << "Set( Date( " << d << ", " << m << ", " << y << " ), "
         << "Time( " << h << ", " << mm << ", " << s << " ) ) :" << endl;
    dateTime.Set( Date( d, m, y ), Time( h, mm, s ) );
    TESTCHECK( dateTime.Valid( ), false, &ok );
    cout << "MakeValid( ) :" << endl;
    dateTime.MakeValid( );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 10, &ok );
    TESTCHECK( dateTime.Month( ), 2, &ok );
    TESTCHECK( dateTime.Year( ), 1601, &ok );
    TESTCHECK( dateTime.Hour( ), 20, &ok );
    TESTCHECK( dateTime.Minute( ), 13, &ok );
    TESTCHECK( dateTime.Second( ), 16., &ok );
    TESTCHECKF( dateTime.JulianDay( ), 2305854.3425463, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), Saturday, &ok );

    d = 25; m = 1; y = 1955;
    h = 19; mm = 42; s = 0.;
    cout << "Set( " << d << ", " << m << ", " << y << ", "
         << h << ", " << mm << ", " << s << " ) :" << endl;
    dateTime.Set( d, m, y, h, mm, s );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 25, &ok );
    TESTCHECK( dateTime.Month( ), 1, &ok );
    TESTCHECK( dateTime.Year( ), 1955, &ok );
    TESTCHECK( dateTime.Hour( ), 19, &ok );
    TESTCHECK( dateTime.Minute( ), 42, &ok );
    TESTCHECK( dateTime.Second( ), 0., &ok );
    TESTCHECKF( dateTime.JulianDay( ), 2435133.32083333, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), Tuesday, &ok );

    double jd = 2451545.4875;
    cout << "Set( " << jd << " ) :" << endl;
    dateTime.Set( jd );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 1, &ok );
    TESTCHECK( dateTime.Month( ), 1, &ok );
    TESTCHECK( dateTime.Year( ), 2000, &ok );
    TESTCHECK( dateTime.Hour( ), 23, &ok );
//    TESTCHECK( dateTime.Minute( ), 42, &ok );
//    TESTCHECKF( dateTime.Second( ), 0., &ok );
    TESTCHECK( dateTime.Minute( ), 41, &ok );
    TESTCHECKF( dateTime.Second( ), 60., &ok );
    TESTCHECK( dateTime.JulianDay( ), 2451545.4875, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), Saturday, &ok );

    d = 1; m = 1; y = 2000;
    h = 23; mm = 42; s = 0.;
    cout << "Set( " << d << ", " << m << ", " << y << ", "
         << h << ", " << mm << ", " << s << " ) :" << endl;
    dateTime.Set( d, m, y, h, mm, s );

    d = 40; m = 12; y = 0;
    h = -6; mm = 0; s = 0.;
    cout << "Increment( " << d << ", " << m << ", " << y << ", "
         << h << ", " << mm << ", " << s << " ) :" << endl;
    dateTime.Increment( d, m, y, h, mm, s );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 10, &ok );
    TESTCHECK( dateTime.Month( ), 2, &ok );
    TESTCHECK( dateTime.Year( ), 2001, &ok );
    TESTCHECK( dateTime.Hour( ), 17, &ok );
    TESTCHECK( dateTime.Minute( ), 42, &ok );
    TESTCHECK( dateTime.Second( ), 0., &ok );
    TESTCHECK( dateTime.JulianDay( ), 2451951.2375, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), Saturday, &ok );

    h = 48; mm=-18; s = 0.;
    cout << "Increment( - TimeIncrement( "
         << h << ", " << mm << ", " << s << " ) ) :" << endl;
    dateTime.Increment( - TimeIncrement( h, mm, s ) );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 8, &ok );
    TESTCHECK( dateTime.Month( ), 2, &ok );
    TESTCHECK( dateTime.Year( ), 2001, &ok );
    TESTCHECK( dateTime.Hour( ), 18, &ok );
    TESTCHECK( dateTime.Minute( ), 0, &ok );
    TESTCHECK( dateTime.Second( ), 0., &ok );
    TESTCHECK( dateTime.JulianDay( ), 2451949.25, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), Thursday, &ok );

    double dd = -404.484375;
    cout << "Increment( " << dd << " ) :" << endl;
    dateTime.Increment( dd );
    TESTCHECK( dateTime.Valid( ), true, &ok );
    TESTCHECK( dateTime.Day( ), 1, &ok );
    TESTCHECK( dateTime.Month( ), 1, &ok );
    TESTCHECK( dateTime.Year( ), 2000, &ok );
    TESTCHECK( dateTime.Hour( ), 6, &ok );
    TESTCHECK( dateTime.Minute( ), 22, &ok );
    TESTCHECK( dateTime.Second( ), 30., &ok );
    TESTCHECK( dateTime.JulianDay( ), 2451544.765625, &ok );
    TESTCHECK( dateTime.DayOfWeek( ), Saturday, &ok );
    TESTCHECK( (dateTime == dateTime), true, &ok );

    cout << "DateTime( true ) [now constructor]" << endl;
    DateTime dateTimeNow( true );
    TESTCHECK( dateTimeNow.Valid( ), true, &ok );
    cout << "dateTimeNow.Date().ToString()=" << dateTimeNow.GetDate().ToString( )
         << "\tOK?" << endl;
    cout << "dateTimeNow.Time().ToString()=" << dateTimeNow.GetTime().ToString( )
         << "\tOK?" << endl;
    TESTCHECK( (dateTimeNow == dateTimeNow), true, &ok );
    TESTCHECK( (dateTime == dateTimeNow), false, &ok );
    TESTCHECK( (dateTime < dateTimeNow), true, &ok );

    d = 25; m = 1; y = 1955;
    h = 19; mm = 42; s = 0.;
    cout << "DateTime( Date( " << d << ", " << m << ", " << y << " ), "
         "Time( " << h << ", " << mm << ", " << s
         << " ) ) [Date, Time constructor]" << endl;
    DateTime dateTimeDT( Date( d, m, y ), Time( h, mm, s ) );
    TESTCHECK( dateTimeDT.Valid( ), true, &ok );
    TESTCHECK( dateTimeDT.Day( ), 25, &ok );
    TESTCHECK( dateTimeDT.Month( ), 1, &ok );
    TESTCHECK( dateTimeDT.Year( ), 1955, &ok );
    TESTCHECK( dateTimeDT.Hour( ), 19, &ok );
    TESTCHECK( dateTimeDT.Minute( ), 42, &ok );
    TESTCHECK( dateTimeDT.Second( ), 0., &ok );
    TESTCHECKF( dateTimeDT.JulianDay( ), 2435133.32083333, &ok );
    TESTCHECK( dateTimeDT.DayOfWeek( ), Tuesday, &ok );
    TESTCHECK( (dateTimeDT == dateTimeDT), true, &ok );
    TESTCHECK( (dateTimeDT == dateTime), false, &ok );
    TESTCHECK( (dateTime < dateTimeDT), false, &ok );
    TESTCHECK( (dateTimeDT < dateTime), true, &ok );
    TESTCHECK( (dateTimeDT < dateTimeNow), true, &ok );

    d = 1; m = 1; y = 1600;
    h = 23; mm = 42; s = 0.;
    cout << "DateTime( " << d << ", " << m << ", " << y << ", "
         << h << ", " << mm << ", " << s << " ) [DMYHMS constructor]"
         << endl;
    DateTime dateTimeDMYHMS( Date( d, m, y ), Time( h, mm, s ) );
    TESTCHECK( dateTimeDMYHMS.Valid( ), true, &ok );
    TESTCHECK( dateTimeDMYHMS.Day( ), 1, &ok );
    TESTCHECK( dateTimeDMYHMS.Month( ), 1, &ok );
    TESTCHECK( dateTimeDMYHMS.Year( ), 1600, &ok );
    TESTCHECK( dateTimeDMYHMS.Hour( ), 23, &ok );
    TESTCHECK( dateTimeDMYHMS.Minute( ), 42, &ok );
    TESTCHECK( dateTimeDMYHMS.Second( ), 0., &ok );
    TESTCHECKF( dateTimeDMYHMS.JulianDay( ), 2305448.4875, &ok );
    TESTCHECK( dateTimeDMYHMS.DayOfWeek( ), Saturday, &ok );
    TESTCHECK( (dateTimeDMYHMS == dateTimeDMYHMS), true, &ok );
    TESTCHECK( (dateTimeDMYHMS == dateTime), false, &ok );
    TESTCHECK( (dateTime < dateTimeDMYHMS), false, &ok );
    TESTCHECK( (dateTimeDMYHMS < dateTime), true, &ok );
    TESTCHECK( (dateTimeDMYHMS < dateTimeDT), true, &ok );
    TESTCHECK( (dateTimeDMYHMS < dateTimeNow), true, &ok );

    jd = 2305448.4875;
    cout << "DateTime( " << jd << " ) [Julian Day constructor]" << endl;
    DateTime dateTimeJD( jd );
    TESTCHECK( dateTimeJD.Valid( ), true, &ok );
    TESTCHECK( dateTimeJD.Day( ), 1, &ok );
    TESTCHECK( dateTimeJD.Month( ), 1, &ok );
    TESTCHECK( dateTimeJD.Year( ), 1600, &ok );
    TESTCHECK( dateTimeJD.Hour( ), 23, &ok );
//    TESTCHECK( dateTimeJD.Minute( ), 42, &ok );
//    TESTCHECKF( dateTimeJD.Second( ), 0., &ok );
    TESTCHECK( dateTimeJD.Minute( ), 41, &ok );
    TESTCHECKF( dateTimeJD.Second( ), 60., &ok );
    TESTCHECKF( dateTimeJD.JulianDay( ), 2305448.4875, &ok );
    TESTCHECK( dateTimeJD.DayOfWeek( ), Saturday, &ok );
    TESTCHECK( (dateTimeJD == dateTimeJD), true, &ok );
//    TESTCHECK( (dateTimeJD == dateTimeDMYHMS), true, &ok );

    if ( ok )
        cout << "DateTime PASSED." << endl << endl;
    else
        cout << "DateTime FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta

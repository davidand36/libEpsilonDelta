/*
  Time.cpp
  Copyright (C) 2007 David M. Anderson

  Time class, representing time of day.
*/


#include "Time.hpp"
#include "TimeLibText.hpp"
#include "StringUtil.hpp"
#include "Assert.hpp"
#include <ctime>
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <cstdio>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

string Time::m_defaultFormat = "%02H:%02m";

//=============================================================================

Time::Time( bool now )
    :    m_hour( 0 ),
         m_minute( 0 ),
         m_second( 0. )
{
    if ( now )
    {
        int h = 0, m = 0;
        double s = 0.;
        if ( TimeNow( &h, &m, &s ) )
            Set( h, m, s );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Time::Time( int hour, int minute, double second )
    :    m_hour( hour ),
         m_minute( minute ),
         m_second( second )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Time::Time( double days )
{
    Set( days );
}

//=============================================================================

void 
Time::Set( bool now )
{
    if ( now )
    {
        int h = 0, m = 0;
        double s = 0.;
#ifdef DEBUG
        bool timeNowRslt =
#endif
                TimeNow( &h, &m, &s );
        Assert( timeNowRslt );
        Set( h, m, s );
    }
    else
        Set( 0, 0, 0 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Time::Set( double days )
{
    days *= 24.;
    double whole;
    double fraction = modf( days, &whole );
    m_hour = static_cast<int>( whole );
    fraction *= 60.;
    fraction = modf( fraction, &whole );
    m_minute = static_cast<int>( whole );
    m_second = fraction * 60.;
    if ( m_second > 59.9999999999 )
    {    //"Fix" this particular presumed floating-point inaccuracy
        m_second = 0.;
        ++m_minute;
        while ( m_minute >= 60 )
        {
            m_minute -= 60;
            ++m_hour;
        }
    }
}

//=============================================================================

bool
Time::Valid( ) const
{
    if ( (m_hour < 0) || (m_hour >= 24) )
        return false;
    if ( (m_minute < 0) || (m_minute >= 60) )
        return false;
    if ( (m_second < 0.) || (m_second >= 60.) )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

int
Time::MakeValid( )
{
    while ( m_second < 0. )
    {
        m_second += 60.;
        --m_minute;
    }
    while ( m_second >= 60. )
    {
        m_second -= 60.;
        ++m_minute;
    }
    if ( m_second > 59.9999999999 )
    {    //"Fix" this particular presumed floating-point inaccuracy
        m_second = 0.;
        ++m_minute;
    }
    while ( m_minute < 0 )
    {
        m_minute += 60;
        --m_hour;
    }
    while ( m_minute >= 60 )
    {
        m_minute -= 60;
        ++m_hour;
    }
    int days = 0;
    while ( m_hour < 0 )
    {
        m_hour += 24;
        --days;
    }
    while ( m_hour >= 24 )
    {
        m_hour -= 24;
        ++days;
    }
    return days;
}

//=============================================================================

double
Time::Days( ) const
{
    return ( (m_hour + (m_minute + m_second / 60.) / 60.) / 24. );
}

//=============================================================================

int
Time::Increment( int hours, int minutes, double seconds )
{
    m_second += seconds;
    m_minute += minutes;
    m_hour += hours;
    return MakeValid( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int
Time::Increment( const TimeIncrement & increment )
{
    return Increment( increment.Hours(), increment.Minutes(),
                      increment.Seconds() );
}

//=============================================================================

bool 
operator==( const Time & lhs, const Time & rhs )
{
    return ( (lhs.m_hour == rhs.m_hour) && (lhs.m_minute == rhs.m_minute)
             && (lhs.m_second == rhs.m_second) );
}

//-----------------------------------------------------------------------------

bool 
operator<( const Time & lhs, const Time & rhs )
{
    return ( (lhs.m_hour < rhs.m_hour)
             || ((lhs.m_hour == rhs.m_hour) && (lhs.m_minute < rhs.m_minute))
             || ((lhs.m_hour == rhs.m_hour) && (lhs.m_minute == rhs.m_minute)
                 && (lhs.m_second < rhs.m_second)) );
}

//=============================================================================

string
Time::ToString( const string & format ) const
{
    string timeString;
    for ( string::const_iterator p = format.begin(); p != format.end(); ++p )
    {
        if ( *p == '%' )
        {
            int width = 0;
            int decimals = 0;
            bool zeroFill = false;
            ++p;
            if ( isdigit( *p ) || (*p == '.') )
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
                if ( *p == '.' )
                {
                    ++p;
                    while ( isdigit( *p ) )
                    {
                        decimals = decimals * 10 + (*p - '0');
                        ++p;
                    }
                }
            }
            switch ( *p )
            {
            case 'H':
                timeString += IntToString( Hour(), width, 0, zeroFill );
                break;
            case 'h':
            {
                int hour = Hour() % 12;
                if ( hour == 0 )
                    hour = 12;
                timeString += IntToString( hour, width, 0, zeroFill );
                break;
            }
            case 'm':
            case 'M':
                timeString += IntToString( Minute(), width, 0, zeroFill );
                break;
            case 's':
            case 'S':
                timeString += RealToString( Second(), width, decimals, '.', 0,
                                            zeroFill );
                break;
            case 'a':
                if ( Hour() < 12 )
                    timeString += g_ampmAbbreviations[ 0 ];
                else
                    timeString += g_ampmAbbreviations[ 1 ];
                break;
            case 'A':
                if ( Hour() < 12 )
                    timeString += g_AMPMAbbreviations[ 0 ];
                else
                    timeString += g_AMPMAbbreviations[ 1 ];
                break;
            case '%':
                timeString += '%';
                break;
            default:

                break;
            } //switch
        }
        else
        {
            timeString += *p;
        }
    }
    return timeString;
}

//=============================================================================

bool
Time::TimeNow( int * pHour, int * pMinute, double * pSecond )
{
    std::time_t t = std::time( 0 );
    if ( t == time_t( -1 ) )
        return false;
    std::tm * pNow = std::localtime( &t );
    *pHour = pNow->tm_hour;
    *pMinute = pNow->tm_min;
    *pSecond = pNow->tm_sec;
    return true;
}

//=============================================================================

Time 
operator+( const Time & lhs, const TimeIncrement & rhs )
{
    Time sum = lhs;
    sum.Increment( rhs );
    return sum;
}

//-----------------------------------------------------------------------------

Time 
operator-( const Time & lhs, const TimeIncrement & rhs )
{
    Time sum = lhs;
    sum.Increment( - rhs );
    return sum;
}

//=============================================================================

#ifdef DEBUG

bool
Time::Test( )
{
    bool ok = true;
    cout << "Testing Time" << endl;

    cout << "Time() [default constructor]" << endl;
    Time time;
    TESTCHECK( time.Valid( ), true, &ok );
    TESTCHECK( time.Hour( ), 0, &ok );
    TESTCHECK( time.Minute( ), 0, &ok );
    TESTCHECK( time.Second( ), 0., &ok );
    TESTCHECK( time.Days( ), 0., &ok );
    TESTCHECK( time.ToString( ), string( "00:00" ), &ok );
    TESTCHECK( time.ToString( "%h:%m:%.2s %a" ),
               string( "12:0:0.00 a.m." ), &ok );

    cout << "Set( true ) :" << endl;
    time.Set( true );
    TESTCHECK( time.Valid( ), true, &ok );
    cout << "time.ToString()=" << time.ToString( ) << "\tOK?" << endl;
    cout << "time.ToString(\"%02h:%02m:%06.3s %a\")="
         << time.ToString( "%02h:%02m:%06.3s %a" ) << "\tOK?" << endl;

    cout << "Set( false ) :" << endl;
    time.Set( false );
    TESTCHECK( time.Valid( ), true, &ok );
    TESTCHECK( time.Hour( ), 0, &ok );
    TESTCHECK( time.Minute( ), 0, &ok );
    TESTCHECK( time.Second( ), 0., &ok );
    TESTCHECK( time.Days( ), 0., &ok );
    TESTCHECK( time.ToString( ), string( "00:00" ), &ok );
    TESTCHECK( time.ToString( "%h:%m:%.2s %a" ),
               string( "12:0:0.00 a.m." ), &ok );

    int h = 20; int m = 13; double s = 16.;
    cout << "Set(" << h << ", " << m << ", " << s << ") :" << endl;
    time.Set( h, m, s );
    TESTCHECK( time.Valid( ), true, &ok );
    TESTCHECK( time.Hour( ), 20, &ok );
    TESTCHECK( time.Minute( ), 13, &ok );
    TESTCHECK( time.Second( ), 16., &ok );
    TESTCHECKF( time.Days( ), 0.8425463, &ok );
    TESTCHECK( time.ToString( ), string( "20:13" ), &ok );
    TESTCHECK( time.ToString( "%h:%m:%.2s %A" ),
               string( "8:13:16.00 PM" ), &ok );

    h = 35; m = 20; s = 16.;
    cout << "days = Increment(" << h << ", " << m << ", " << s << ") :" << endl;
    int days = time.Increment( h, m, s );
    TESTCHECK( time.Valid( ), true, &ok );
    TESTCHECK( time.Hour( ), 7, &ok );
    TESTCHECK( time.Minute( ), 33, &ok );
    TESTCHECK( time.Second( ), 32., &ok );
    TESTCHECKF( time.Days( ), 0.3149537, &ok );
    TESTCHECK( days, 2, &ok );
    TESTCHECK( time.ToString( ), string( "07:33" ), &ok );
    TESTCHECK( time.ToString( "%h:%m:%.2s %A" ),
               string( "7:33:32.00 AM" ), &ok );

    h = 35; m = 20; s = 16.;
    cout << "days = Increment( - TimeIncrement(" << h << ", " << m << ", " << s << ") ) :" << endl;
    days = time.Increment( - TimeIncrement( h, m, s ) );
    TESTCHECK( time.Valid( ), true, &ok );
    TESTCHECK( time.Hour( ), 20, &ok );
    TESTCHECK( time.Minute( ), 13, &ok );
    TESTCHECK( time.Second( ), 16., &ok );
    TESTCHECKF( time.Days( ), 0.8425463, &ok );
    TESTCHECK( days, -2, &ok );
    TESTCHECK( time.ToString( ), string( "20:13" ), &ok );
    TESTCHECK( time.ToString( "%h:%m:%.2s %a" ),
               string( "8:13:16.00 p.m." ), &ok );

    cout << "Time(true) [now constructor]" << endl;
    Time timeNow( true );
    TESTCHECK( timeNow.Valid( ), true, &ok );
    cout << "timeNow.ToString()=" << timeNow.ToString( ) << "\tOK?" << endl;
    cout << "timeNow.ToString(\"%02h:%02m:%06.3s %a\")="
         << timeNow.ToString( "%02h:%02m:%06.3s %a" ) << "\tOK?" << endl;

    h = 11; m = 42; s = 0.;
    cout << "Time( " << h << ", " << m << ", " << s << " ) [h,m,s constructor]"
         << endl;
    Time timeHMS( h, m, s );
    TESTCHECK( timeHMS.Valid( ), true, &ok );
    TESTCHECK( timeHMS.Hour( ), 11, &ok );
    TESTCHECK( timeHMS.Minute( ), 42, &ok );
    TESTCHECK( timeHMS.Second( ), 0., &ok );
    TESTCHECK( timeHMS.Days( ), 0.4875, &ok );
    TESTCHECK( timeHMS.ToString( ), string( "11:42" ), &ok );
    TESTCHECK( timeHMS.ToString( "%h:%m:%.2s %a" ),
               string( "11:42:0.00 a.m." ), &ok );
    TESTCHECK( (timeHMS == time), false, &ok );
    TESTCHECK( (timeHMS == timeHMS), true, &ok );
    TESTCHECK( (timeHMS < time), true, &ok );
    TESTCHECK( (time < timeHMS), false, &ok );

    double d = 0.4875;
    cout << "Time( " << d << " ) [days constructor]" << endl;
    Time timeD( d );
    TESTCHECK( timeD.Valid( ), true, &ok );
    TESTCHECK( timeD.Hour( ), 11, &ok );
    TESTCHECK( timeD.Minute( ), 42, &ok );
    TESTCHECKF( timeD.Second( ), 0., &ok );
    TESTCHECKF( timeD.Days( ), 0.4875, &ok );
    TESTCHECK( timeD.ToString( ), string( "11:42" ), &ok );
    TESTCHECK( timeD.ToString( "%h:%m:%.2s %a" ),
               string( "11:42:0.00 a.m." ), &ok );
    TESTCHECK( (timeD == time), false, &ok );
    TESTCHECK( (timeD == timeHMS), true, &ok );
    TESTCHECK( (timeD < time), true, &ok );
    TESTCHECK( (time < timeD), false, &ok );

    if ( ok )
        cout << "Time PASSED." << endl << endl;
    else
        cout << "Time FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

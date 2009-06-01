#ifndef TIME_HPP
#define TIME_HPP
/*
  Time.hpp
  Copyright (C) 2007 David M. Anderson

  Time class, representing time of day.
  NOTES:
  1. Valid(), MakeValid(), and routines that use them, such as Increment(),
     assume Seconds()<60. Thus they don't properly handle the occasional
     "leap seconds" added at the middle or end of some years to adjust for
     the Earth's varying rotational rate, when times such as 24:59:60.999
     occur in civil time.
  2. Time format strings for ToString() are similar to those for printf().
     %H prints the hour in 24-hour form; %h, the hour in 12-hour form;
     %m, the minute; %s, the second; %a, "a.m." or "p.m."; %A, "AM" or "PM".
     Between the '%' and the field selector, a number can
     be used to specify the minimum width for numeric fields. For seconds,
     a period followed by a number specifies the number of decimal places
     printed. A zero preceding the width for a numeric field causes leading
     zeros to be printed.
     Examples:
         "%02H:%02m"  =>  "20:27"
         "%02h:%02m:%02s %a"  =>  "08:27:40 p.m."
         "%2h:%02m:%02.1s %a"  =>  " 8:27:40.0 p.m."
  3. Note that operator+() and operator-() do not commute, since their
     arguments have different types. More important, in some cases, they
     discard overflows and underflows, unlike Increment().
*/


#include "TimeIncrement.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Time
{
public:
    explicit Time( bool now = false );
    Time( int hour, int minute, double second = 0. );
    Time( double days );
    void Set( bool now );
    void Set( int hour, int minute, double second = 0. );
    void Set( double days );
    bool Valid( ) const;
    int MakeValid( );
    int Hour( ) const;
    int Minute( ) const;
    double Second( ) const;
    double Days( ) const;
    int Increment( int hours, int minutes = 0, double seconds = 0. );
    int Increment( const TimeIncrement & increment );
    std::string ToString( const std::string & format
                          = m_defaultFormat ) const;

    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );
#ifdef DEBUG
    static bool Test( );
#endif

    friend bool operator==( const Time & lhs, const Time & rhs );
    friend bool operator<( const Time & lhs, const Time & rhs );

private:
    int m_hour;
    int m_minute;
    double m_second;

    static std::string m_defaultFormat;

    static bool TimeNow( int * pHour, int * pMinute, double * pSecond );
};

//.............................................................................

Time operator+( const Time & lhs, const TimeIncrement & rhs );
Time operator-( const Time & lhs, const TimeIncrement & rhs );


//*****************************************************************************


inline
void
Time::Set( int hour, int minute, double second )
{
    m_hour = hour;
    m_minute = minute;
    m_second = second;
}

//=============================================================================

inline
int
Time::Hour( ) const
{
    return m_hour;
}

//-----------------------------------------------------------------------------

inline
int
Time::Minute( ) const
{
    return m_minute;
}

//-----------------------------------------------------------------------------

inline
double
Time::Second( ) const
{
    return m_second;
}

//=============================================================================

inline
void
Time::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

inline
const std::string &
Time::DefaultFormat( )
{
    return m_defaultFormat;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //TIME_HPP

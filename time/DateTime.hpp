#ifndef DATETIME_HPP
#define DATETIME_HPP
/*
  DateTime.hpp
  Copyright (C) 2007 David M. Anderson

  DateTime class, consisting of a (Gregorian) date and a time of day.
  NOTES:
  1. See the notes in Date.hpp and Time.hpp.
  2. The conversions from and to JulianDay are only really valid if the
     date and time are UTC, where the fractional part is 0 at noon.
*/


#include "Date.hpp"
#include "Time.hpp"
#include "TimeIncrement.hpp"
#include <ctime>

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class DateTime
{
public:
    explicit DateTime( bool now = false );
    explicit DateTime( std::time_t stdTime );
    DateTime( const Date & date, const Time & time );
    DateTime( int day, int month, int year,
              int hour, int minute, double second = 0. );
    explicit DateTime( double julianDay );
    virtual ~DateTime( );
    void Set( bool now );
    void Set( std::time_t stdTime );
    void Set( const Date & date, const Time & time );
    void Set( int day, int month, int year,
              int hour, int minute, double second = 0. );
    void Set( double julianDay );
    bool Valid( ) const;
    void MakeValid( );
    Date GetDate( ) const;
    Time GetTime( ) const;
    int Day( ) const;
    int Month( ) const;
    int Year( ) const;
    int Hour( ) const;
    int Minute( ) const;
    double Second( ) const;
    double JulianDay( ) const;
    int DayOfWeek( ) const;
    void Increment( int days, int months, int years,
                    int hours = 0, int minutes = 0, double seconds = 0. );
    void Increment( const TimeIncrement & increment );
    void Increment( double days );
#ifdef DEBUG
    static bool Test( );
#endif

private:
    Date m_date;
    Time m_time;
};

//.............................................................................

bool operator==( const DateTime & lhs, const DateTime & rhs );
bool operator<( const DateTime & lhs, const DateTime & rhs );


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DATETIME_HPP

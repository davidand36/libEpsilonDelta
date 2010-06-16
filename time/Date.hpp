#ifndef DATE_HPP
#define DATE_HPP
/*
  Date.hpp
  Copyright (C) 2007 David M. Anderson

  Shortcut definitions, recognizing the predominance of the Gregorian
  calendar.
  NOTES:
  1. So that the reader doesn't have to refer to the various included headers,
     the class Date has the following public interface:
    Date( bool today = false );
    Date( int day, int month, long year );
    Date( long julianDay );
    Date( const DateJD & date );
    Date( const Date & date );
    ~Date( );
    DateDMY & operator=( const DateJD & rhs );
    DateDMY & operator=( const Date & rhs );
    bool Valid( ) const;
    void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );
    int Day( ) const;
    int Month( ) const;
    long Year( ) const;
    long JulianDay( ) const;
    int DayOfWeek( ) const;
    void Set( bool today );
    void Set( int day, int month, long year );
    void Set( long julianDay );
    void Increment( int days );
    void Increment( int days, int months, long years,
                    DateFixup::EMethod fixupMethod = DateFixup::Carry );
    void Increment( int weekday, int n );
    virtual int DaysUntilWeekday( int weekday, int n ) const;
    std::string ToString( const std::string & format
                                  = DefaultFormat() ) const;

    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    bool operator==( const Date & lhs, const Date & rhs );
    bool operator<( const Date & lhs, const Date & rhs );
  2. Day and month numbering starts at 1. Weekdays start at 0 (= Sunday).
*/


#include "GregorianCalendar.hpp"
#include "GregorianDate.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef GregorianDate  Date;


enum EWeekDay
{
    Sunday = 0,
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday
};

enum EMonth
{
    January = 1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
};


//*****************************************************************************

inline 
int 
MonthsInYear( )
{
    return 12;
}

//-----------------------------------------------------------------------------

inline 
const std::string &
MonthName( int month )
{
    return GregorianCalendar::MonthName( month );
}

//-----------------------------------------------------------------------------

inline
int
DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
const std::string & 
WeekDayName( int weekday )
{
    return WesternWeek::WeekDayName( weekday );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DATE_HPP

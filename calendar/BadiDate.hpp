#ifndef BADIDATE_HPP
#define BADIDATE_HPP
/*
  BadiDate.hpp
  Copyright (C) 2007 David M. Anderson

  BadiDate class, representing a date in the Badi calendar.
  NOTES:
  1. Day, month, year, and vahid numbering start at 1.
  2. In principle, dates before the epoch of the Baha'i Era are undefined,
     but negative values for the Kull-i-Shay are accepted.
  4. See DateFixupMethod.hh for details about the fixup methods.
  5. Date format strings for ToString() are similar to those for printf().
     %d prints the day; %D, the day name;
     %m, the month number; %M, the month name;
     %y, the year; %Y, the year name;
     %v, the Vahid; %V, the Vahid as an ordinal;
     %k, the Kull-i-Shay; %K, the Kull-i-Shay as an ordinal;
     %w, the weekday number; and %W, the weekday name.
     Between the '%' and the field selector, a number can be used to specify
     the width. The width is taken taken as the minimum for numeric fields,
     and the maximum for string fields. An exception is made for year:
     if the width is 2 or 3, only the last 2 or 3 digits are printed.
     A zero preceding the width for a numeric field causes leading zeros
     to be printed.
     Examples (Gregorian dates):
         "%d %M %y"  =>  "25 January 1955"
         "%D %M %y" => "25th January 1955"
         "%y-%02m-%02d" => "1955-01-25"
         "%2d/%2m/%2y" => "25/ 1/55"
         "%02d/%02m/%02y" => "25/01/55"
         "%02m/%02d/%02y" => "01/25/55"
         "%02d%3M%y" => "25Jan1955"
     The first of these is the default unless SetDefaultFormat() is called.
     The third is recommended by ISO 8601.
*/


#include "DateJD.hpp"
#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class BadiDate
    :   public DateJD
{
public:
    explicit BadiDate( bool today = false );
    explicit BadiDate( int julianDay );
    BadiDate( int day, int month, int year, int vahid, int kulliShay );
    explicit BadiDate( const DateJD & date );
    BadiDate( const BadiDate & date );
    virtual ~BadiDate( );

    BadiDate & operator=( const DateJD & rhs );
    BadiDate & operator=( const BadiDate & rhs );

    virtual void Set( bool today );
    virtual void Set( int julianDay );
    void Set( int day, int month, int year, int vahid, int kulliShay );

    virtual bool Valid( ) const;
    void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );

    virtual int JulianDay( ) const;
    int Day( ) const;
    int Month( ) const;
    int Year( ) const;
    int Vahid( ) const;
    int KulliShay( ) const;
    int DayOfWeek( ) const;
    int DaysUntilWeekday( int weekday, int n ) const;

    virtual void Increment( int days );
    void Increment( int days, int months,
                    int years, int vahids, int kulliShays,
                    DateFixup::EMethod fixupMethod = DateFixup::Carry );
    void Increment( int weekday, int n );

    std::string ToString( const std::string & format
                          = DefaultFormat() ) const;
    
    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef class BadiCalendar Calendar;
    typedef class BahaiWeek Week;


private:
    int m_day;
    int m_month;
    int m_year;
    int m_vahid;
    int m_kulliShay;

    static std::string m_defaultFormat;
};

//.............................................................................

bool operator==( const BadiDate & lhs, const BadiDate & rhs );
bool operator<( const BadiDate & lhs, const BadiDate & rhs );

#ifdef DEBUG
bool TestBadiDate( );
#endif


//*****************************************************************************


inline 
int 
BadiDate::Day( ) const
{
    return m_day;
}

//-----------------------------------------------------------------------------

inline 
int 
BadiDate::Month( ) const
{
    return m_month;
}

//-----------------------------------------------------------------------------

inline 
int 
BadiDate::Year( ) const
{
    return m_year;
}

//-----------------------------------------------------------------------------

inline 
int 
BadiDate::Vahid( ) const
{
    return m_vahid;
}

//-----------------------------------------------------------------------------

inline 
int 
BadiDate::KulliShay( ) const
{
    return m_kulliShay;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BADIDATE_HPP

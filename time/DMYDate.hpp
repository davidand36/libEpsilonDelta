#ifndef DMYDATE_HPP
#define DMYDATE_HPP
/*
  DMYDate.hpp
  Copyright (C) 2007 David M. Anderson

  DMYDate class template, for dates represented by (day, month, year) triplets.
  NOTES:
  1. The template parameter Cal, representing the calendrical system, 
     should provide the following interface:
     class Cal
     {
     public:
         static void JulianDayToDMY( long julianDay,
                                     int * pDay, int * pMonth, long * pYear );
         static long DMYToJulianDay( int day, int month, long year );
         static int MonthsInYear( long year );
         static int DaysInMonth( int month, long year );
         static const std::string & MonthName( int month, long year );
     };
  2. Day and month numbering start at 1.
  3. Years can be 0 or negative. So, e.g., -2 = 3 B.C.
  4. See DateFixupMethod.hh for details about the fixup methods.
  5. Date format strings for ToString() are similar to those for printf().
     %d prints the day; %D, the day as an ordinal;
     %m, the month number; %M, the month name;
     and %y, the year.
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


#include "JDDate.hpp"
#include "DateFixupMethod.hpp"
#include "Assert.hpp"
#include "StringUtil.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename Cal>
class DMYDate
    :   public JDDate
{
public:
    explicit DMYDate( bool today = false );
    explicit DMYDate( long julianDay );
    DMYDate( int day, int month, long year );
    explicit DMYDate( const JDDate & date );
    DMYDate( const DMYDate & date );
    virtual ~DMYDate( );

    virtual DMYDate & operator=( const JDDate & rhs );
    virtual DMYDate & operator=( const DMYDate & rhs );

    virtual void Set( bool today );
    virtual void Set( long julianDay );
    virtual void Set( int day, int month, long year );

    virtual bool Valid( ) const;
    virtual void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );

    virtual long JulianDay( ) const;
    int Day( ) const;
    int Month( ) const;
    long Year( ) const;

    virtual void Increment( int days );
    virtual void Increment( int days, int months, long years,
                            DateFixup::EMethod fixupMethod = DateFixup::Carry );

    virtual std::string ToString( const std::string & format
                                  = DefaultFormat() ) const;
    
    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef Cal Calendar;

protected:
    int m_day;
    int m_month;
    long m_year;

    static std::string m_defaultFormat;
};

//.............................................................................

template <typename Cal>
bool operator==( const DMYDate<Cal> & lhs, const DMYDate<Cal> & rhs );
template <typename Cal>
bool operator<( const DMYDate<Cal> & lhs, const DMYDate<Cal> & rhs );


//*****************************************************************************


template <typename Cal>
DMYDate<Cal>::DMYDate( bool today )
{
    Set( today );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
DMYDate<Cal>::DMYDate( long julianDay )
    :   JDDate( false )
{
    JDDate::Set( julianDay );
    Cal::JulianDayToDMY( JulianDay(), &m_day, &m_month, &m_year );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
inline
DMYDate<Cal>::DMYDate( int day, int month, long year )
    :   JDDate( false ),
        m_day( day ),
        m_month( month ),
        m_year( year )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
DMYDate<Cal>::DMYDate( const JDDate & date )
    :   JDDate( false )
{
    JDDate::Set( date.JulianDay() );
    Cal::JulianDayToDMY( JulianDay(), &m_day, &m_month, &m_year );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
inline 
DMYDate<Cal>::DMYDate( const DMYDate & date )
    :    JDDate( false )
{
    Set( date.m_day, date.m_month, date.m_year );
    if ( date.JDDate::Valid() )
        JDDate::Set( date.JulianDay() );
}

//-----------------------------------------------------------------------------

template <typename Cal>
inline
DMYDate<Cal>::~DMYDate( )
{
}

//=============================================================================

template <typename Cal>
DMYDate<Cal> & 
DMYDate<Cal>::operator=( const JDDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.JulianDay() );
    return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
inline 
DMYDate<Cal> & 
DMYDate<Cal>::operator=( const DMYDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.m_day, rhs.m_month, rhs.m_year );
    if ( rhs.JDDate::Valid() )
        JDDate::Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

template <typename Cal>
void 
DMYDate<Cal>::Set( bool today )
{
    if ( today )
    {
        JDDate::Set( true );
        Assert( JDDate::Valid() );
        Cal::JulianDayToDMY( JulianDay(), &m_day, &m_month, &m_year );
    }
    else
    {
        JDDate::Set( false );
        Set( 1, 1, 1 );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
void 
DMYDate<Cal>::Set( long julianDay )
{
    JDDate::Set( julianDay );
    Cal::JulianDayToDMY( julianDay, &m_day, &m_month, &m_year );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
inline 
void 
DMYDate<Cal>::Set( int day, int month, long year )
{
    JDDate::Set( JDDate::INVALID );
    m_day = day;
    m_month = month;
    m_year = year;
}

//=============================================================================

template <typename Cal>
inline 
bool 
DMYDate<Cal>::Valid( ) const
{
    if ( (m_month < 1) || (m_month > Cal::MonthsInYear( m_year )) )
        return false;
    if ( (m_day < 1) || (m_day > Cal::DaysInMonth( m_month, m_year )) )
        return false;
    return true;
}

//-----------------------------------------------------------------------------

template <typename Cal>
void
DMYDate<Cal>::MakeValid( DateFixup::EMethod fixupMethod )
{
    if ( fixupMethod & DateFixup::ClampMonth )
    {
        if ( m_month < 1 )
            m_month = 1;
        else 
        {
            int monthsInYear = Cal::MonthsInYear( m_year );
            if ( m_month > monthsInYear )
                m_month = monthsInYear;
        }
    }
    else
    {
        while ( m_month < 1 )
        {
            --m_year;
            m_month += Cal::MonthsInYear( m_year );
        }
        int monthsInYear = Cal::MonthsInYear( m_year );
        while ( m_month > monthsInYear )
        {
            m_month -= monthsInYear;
            ++m_year;
            monthsInYear = Cal::MonthsInYear( m_year );
        }
    }
    if ( fixupMethod & DateFixup::ClampDay )
    {
        if ( m_day < 1 )
            m_day = 1;
        int daysInMonth = Cal::DaysInMonth( m_month, m_year );
        if ( m_day > daysInMonth )
            m_day = daysInMonth;
    }
    else
    {
        while ( m_day < 1 )
        {

            --m_month;
            if ( m_month < 1 )
            {
                --m_year;
                m_month = Cal::MonthsInYear( m_year );
            }
            Assert( (m_month >= 1)
                    && (m_month <= Cal::MonthsInYear( m_year )) );
            m_day += Cal::DaysInMonth( m_month, m_year );
        }
        int daysInMonth = Cal::DaysInMonth( m_month, m_year );
        while ( m_day > daysInMonth )
        {
            m_day -= daysInMonth;
            ++m_month;
            if ( m_month > Cal::MonthsInYear( m_year ) )
            {
                m_month = 1;
                ++m_year;
            }
            Assert( (m_month >= 1)
                    && (m_month <= Cal::MonthsInYear( m_year )) );
            daysInMonth = Cal::DaysInMonth( m_month, m_year );
        }
    }
    Assert( Valid() );
}

//=============================================================================

template <typename Cal>
long 
DMYDate<Cal>::JulianDay( ) const
{
    if ( ! JDDate::Valid() )
        m_julianDay = Cal::DMYToJulianDay( m_day, m_month, m_year );
    return JDDate::JulianDay();
}

//-----------------------------------------------------------------------------

template <typename Cal>
inline 
int 
DMYDate<Cal>::Day( ) const
{
    return m_day;
}

//-----------------------------------------------------------------------------

template <typename Cal>
inline 
int 
DMYDate<Cal>::Month( ) const
{
    return m_month;
}

//-----------------------------------------------------------------------------

template <typename Cal>
inline 
long 
DMYDate<Cal>::Year( ) const
{
    return m_year;
}

//=============================================================================

template <typename Cal>
void 
DMYDate<Cal>::Increment( int days )
{
    JDDate::Increment( days );
    m_day += days;
    MakeValid( DateFixup::Carry );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal>
void 
DMYDate<Cal>::Increment( int days, int months, long years,
                         DateFixup::EMethod fixupMethod )
{
    JDDate::Set( JDDate::INVALID );
    m_day += days;
    m_month += months;
    m_year += years;
    MakeValid( fixupMethod );
}

//=============================================================================

template <typename Cal>
std::string 
DMYDate<Cal>::ToString( const std::string & format ) const
{
    std::string dateString;
    for ( std::string::const_iterator p = format.begin();
          p != format.end(); ++p )
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
                dateString += OrdinalToString( m_day, width );
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
                    = Calendar().MonthName( m_month, m_year );
                if ( monthName.length() == 0 )
                    dateString += IntToString( m_month, width, 0, zeroFill );
                else if ( width == 0 )
                    dateString += monthName;
                else
                    dateString += monthName.substr( 0, width );
                break;
            }
            case 'y':
            case 'Y':
            {
                long y = m_year;
                if ( width == 2 )
                    y %= 100;
                else if ( width == 3 )
                    y %= 1000;
                dateString += IntToString( y, width, 0, zeroFill );
                break;
            }
            case '%':
                dateString += '%';
                break;
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

template <typename Cal>
std::string 
DMYDate<Cal>::m_defaultFormat = "%d %M %y";

//-----------------------------------------------------------------------------

template <typename Cal>
inline 
void 
DMYDate<Cal>::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

template <typename Cal>
inline 
const std::string &
DMYDate<Cal>::DefaultFormat( )
{
    return m_defaultFormat;
}

//=============================================================================

template <typename Cal>
inline
bool operator==( const DMYDate<Cal> & lhs, const DMYDate<Cal> & rhs )
{
    return ( (lhs.Day() == rhs.Day()) && (lhs.Month() == rhs.Month())
             && (lhs.Year() == rhs.Year()) );
}

//-----------------------------------------------------------------------------

template <typename Cal>
inline
bool operator<( const DMYDate<Cal> & lhs, const DMYDate<Cal> & rhs )
{
    if ( lhs.Year() < rhs.Year() )
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


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DMYDATE_HPP

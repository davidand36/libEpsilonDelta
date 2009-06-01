#ifndef DATEDMYW_HPP
#define DATEDMYW_HPP
/*
  DateDMYW.hpp
  Copyright (C) 2007 David M. Anderson

  DateDMYW class template, for dates represented by (day, month, year) triplets
  with a week-style cycle.
  NOTES:
  1. See DateDMY.hh for details about the DateDMY parent class template and 
     the Cal template parameter.
  2. The Week template parameter, representing the week cycle, should provide 
     the following interface:
     class Week
     {
     public:
         static int DaysInWeek( );
         static int DayOfWeekOfJD0( );
         static const std::string & WeekDayName( int weekday );
     };
  3. Weekdays will be numbered from 0 to DaysInWeek - 1.
  4. ToString() is the same as in DateDMY, with the addition of
     %w for the weekday number and %W for the weekday name.
     Examples (Gregorian dates):
         "%W, %d %M %y"  =>  "Monday, 1 January 2001"
         "%2d/%m/%2y (%w)" => " 1/1/ 1 (1)"
         "(%02w)%02d/%02m/%02y" => "(01)01/01/01"
         "%02m/%02d/%02y" => "01/01/01"
         "%3W %02d%3M%y" => "Mon 01Jan2001"
     The first of these is the default unless SetDefaultFormat() is called.
  5. Increment( weekday, n ) advances to the (n+1)st weekday on or after the
     date. ("weekday" here means, e.g., 0 for Sunday, etc., depending on
     the Week parameter.). n=-1 gives the last weekday before the date.
*/


#include "DateDMY.hpp"
#include "DivMod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename Cal, typename WeekType>
class DateDMYW
    :   public DateDMY<Cal>
{
public:
    explicit DateDMYW( bool today = false );
    explicit DateDMYW( int julianDay );
    DateDMYW( int day, int month, int year );
    DateDMYW( const DateJD & date );
    DateDMYW( const DateDMY<Cal> & date );
    virtual ~DateDMYW( );

    virtual DateDMYW & operator=( const DateJD & rhs );
    virtual DateDMYW & operator=( const DateDMY<Cal> & rhs );

    virtual int DayOfWeek( ) const;
    virtual int DaysUntilWeekday( int weekday, int n ) const;

    virtual void Increment( int days );
    virtual void Increment( int days, int months, int years,
                            DateFixup::EMethod fixupMethod = DateFixup::Carry );
    virtual void Increment( int weekday, int n );

    virtual std::string ToString( const std::string & format
                                  = DefaultFormat() ) const;

    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef WeekType Week;

private:
    static std::string m_defaultFormat;
};


//*****************************************************************************


template <typename Cal, typename WeekType>
DateDMYW<Cal, WeekType>::DateDMYW( bool today )
    :   DateDMY<Cal>( today )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal, typename WeekType>
DateDMYW<Cal, WeekType>::DateDMYW( int julianDay )
    :   DateDMY<Cal>( julianDay )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal, typename WeekType>
inline 
DateDMYW<Cal, WeekType>::DateDMYW( int day, int month, int year )
    :   DateDMY<Cal>( day, month, year )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal, typename WeekType>
DateDMYW<Cal, WeekType>::DateDMYW( const DateJD & date )
    :   DateDMY<Cal>( date )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal, typename WeekType>
inline 
DateDMYW<Cal, WeekType>::DateDMYW( const DateDMY<Cal> & date )
    :   DateDMY<Cal>( date )
{
}

//-----------------------------------------------------------------------------

template <typename Cal, typename WeekType>
DateDMYW<Cal, WeekType>::~DateDMYW( )
{
}

//=============================================================================

template <typename Cal, typename WeekType>
DateDMYW<Cal, WeekType> & 
DateDMYW<Cal, WeekType>::operator=( const DateJD & rhs )
{
    DateDMY<Cal>::operator=( rhs );
    return *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal, typename WeekType>
DateDMYW<Cal, WeekType> & 
DateDMYW<Cal, WeekType>::operator=( const DateDMY<Cal> & rhs )
{
    DateDMY<Cal>::operator=( rhs );
    return *this;
}

//=============================================================================

template <typename Cal, typename WeekType>
int
DateDMYW<Cal, WeekType>::DayOfWeek( ) const
{
    return ModF( (this->JulianDay( ) + Week::DayOfWeekOfJD0()),
                 Week::DaysInWeek() );
}

//-----------------------------------------------------------------------------

template <typename Cal, typename WeekType>
int
DateDMYW<Cal, WeekType>::DaysUntilWeekday( int weekday, int n ) const
{
    int daysUntilNext = - ModC( (this->JulianDay( ) + Week::DayOfWeekOfJD0()
                                 - weekday), Week::DaysInWeek() );
    return ( daysUntilNext + n * Week::DaysInWeek() );
}

//=============================================================================

template <typename Cal, typename WeekType>
void 
DateDMYW<Cal, WeekType>::Increment( int days )
{
    DateDMY<Cal>::Increment( days );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal, typename WeekType>
void 
DateDMYW<Cal, WeekType>::Increment( int days, int months, int years,
                                    DateFixup::EMethod fixupMethod )
{
    DateDMY<Cal>::Increment( days, months, years, fixupMethod );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename Cal, typename WeekType>
void 
DateDMYW<Cal, WeekType>::Increment( int weekday, int n )
{
    int days = DaysUntilWeekday( weekday, n );
    Increment( days );
}

//=============================================================================

template <typename Cal, typename WeekType>
std::string
DateDMYW<Cal, WeekType>::ToString( const std::string & format ) const
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
            case 'D':
            {
                dateString += IntToString( this->m_day, width, 0, zeroFill );
                break;
            }
            case 'm':
            {
                dateString += IntToString( this->m_month, width, 0, zeroFill );
                break;
            }
            case 'M':
            {
                const std::string & monthName
                    = Cal::MonthName( this->m_month, this->m_year );
                if ( monthName.length() == 0 )
                    dateString += IntToString( this->m_month, width, 0,
                                               zeroFill );
                else if ( width == 0 )
                    dateString += monthName;
                else
                    dateString += monthName.substr( 0, width );
                break;
            }
            case 'y':
            case 'Y':
            {
                int y = this->m_year;
                if ( width == 2 )
                    y %= 100;
                else if ( width == 3 )
                    y %= 1000;
                dateString += IntToString( y, width, 0, zeroFill );
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
                    = Week::WeekDayName( DayOfWeek() );
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

template <typename Cal, typename WeekType>
std::string 
DateDMYW<Cal, WeekType>::m_defaultFormat = "%W, %d %M %y";

//-----------------------------------------------------------------------------

template <typename Cal, typename WeekType>
inline 
void
DateDMYW<Cal, WeekType>::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

template <typename Cal, typename WeekType>
inline 
const std::string & 
DateDMYW<Cal, WeekType>::DefaultFormat( )
{
    return m_defaultFormat;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DATEDMYW_HPP

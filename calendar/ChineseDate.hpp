#ifndef CHINESEDATE_HPP
#define CHINESEDATE_HPP
/*
  ChineseDate.hpp
  Copyright (C) 2007 David M. Anderson

  ChineseDate class, representing a date in the Chinese calendar.
*/


#include "ChineseCalendar.hpp"
#include "DateJD.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ChineseDate
    :   public DateJD
{
public:
    explicit ChineseDate( bool today = false );
    explicit ChineseDate( int julianDay );
    ChineseDate( int day, int month, int year,
                 int leapMonth = ChineseCalendar::LMUnknown );
    ChineseDate( int day, int month, bool leap, int year );
    ChineseDate( int day, int month, int yearCyclical, int yearCycle,
                 int leapMonth );
    ChineseDate( int day, int month, bool leap,
                 int yearCyclical, int yearCycle );
    explicit ChineseDate( const DateJD & date );
    ChineseDate( const ChineseDate & date );
    virtual ~ChineseDate( );

    ChineseDate & operator=( const DateJD & rhs );
    ChineseDate & operator=( const ChineseDate & rhs );

    virtual void Set( bool today );
    virtual void Set( int julianDay );
    void Set( int day, int month, int year,
                      int leapMonth = ChineseCalendar::LMUnknown );
    void Set( int day, int month, bool leap, int year );
    void Set( int day, int month, int yearCyclical, int yearCycle,
              int leapMonth );
    void Set( int day, int month, bool leap, int yearCyclical, int yearCycle );

    virtual bool Valid( ) const;
    void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );

    virtual int JulianDay( ) const;
    int Day( ) const;
    int TrueMonth( ) const;
    int MonthNumber( ) const;
    bool IsMonthLeap( ) const;
    int Year( ) const;

    int DayCyclical( ) const;
    int MonthCyclical( ) const;
    int YearCyclical( ) const;
    int YearCycle( ) const;
    int DayCelestialStem( ) const;
    int DayTerrestrialBranch( ) const;
    int MonthCelestialStem( ) const;
    int MonthTerrestrialBranch( ) const;
    int YearCelestialStem( ) const;
    int YearTerrestrialBranch( ) const;
    int MajorSolarTerm( ) const;
    int MinorSolarTerm( ) const;

    virtual void Increment( int days );
    void Increment( int days, int months, int years, 
                    DateFixup::EMethod fixupMethod = DateFixup::Carry );

    std::string ToString( const std::string & format
                                  = m_defaultFormat ) const;
    
    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef ChineseCalendar Calendar;

protected:
    int m_day;
    int m_month;
    int m_year;
    mutable int m_leapMonth;

    static std::string m_defaultFormat;
};

//.............................................................................

bool operator==( const ChineseDate & lhs, const ChineseDate & rhs );
bool operator<( const ChineseDate & lhs, const ChineseDate & rhs );

#ifdef DEBUG
bool TestChineseDate( );
#endif


//*****************************************************************************


inline 
void 
ChineseDate::Set( int day, int month, int year, int leapMonth )
{
    DateJD::Set( DateJD::INVALID );
    m_day = day;
    m_month = month;
    m_year = year;
    m_leapMonth = leapMonth;
}

//=============================================================================

inline 
ChineseDate::ChineseDate( int day, int month, int year, int leapMonth )
    :   DateJD( false ),
        m_day( day ),
        m_month( month ),
        m_year( year ),
        m_leapMonth( leapMonth )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
ChineseDate::ChineseDate( const ChineseDate & date )
    :    DateJD( false )
{
    Set( date.m_day, date.m_month, date.m_year,
         date.m_leapMonth );
    if ( date.DateJD::Valid() )
        DateJD::Set( date.JulianDay() );
}

//-----------------------------------------------------------------------------

inline 
ChineseDate::~ChineseDate( )
{
}

//=============================================================================

inline 
ChineseDate & 
ChineseDate::operator=( const ChineseDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.m_day, rhs.m_month, rhs.m_year,
         rhs.m_leapMonth );
    if ( rhs.DateJD::Valid() )
        DateJD::Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

inline 
int 
ChineseDate::Day( ) const
{
    return m_day;
}

//-----------------------------------------------------------------------------

inline 
int 
ChineseDate::TrueMonth( ) const
{
    return m_month;
}

//-----------------------------------------------------------------------------

inline 
int 
ChineseDate::Year( ) const
{
    return m_year;
}

//=============================================================================

inline     
void 
ChineseDate::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

inline 
const std::string & 
ChineseDate::DefaultFormat( )
{
    return m_defaultFormat;
}

//=============================================================================

inline 
bool 
operator==( const ChineseDate & lhs, const ChineseDate & rhs )
{
    return ( (lhs.Day() == rhs.Day()) && (lhs.TrueMonth() == rhs.TrueMonth())
             && (lhs.Year() == rhs.Year()) );
}

//-----------------------------------------------------------------------------

inline 
bool 
operator<( const ChineseDate & lhs, const ChineseDate & rhs )
{
    if ( lhs.Year() < rhs.Year() )
        return true;
    else if ( lhs.Year() > rhs.Year() )
        return false;
    else if ( lhs.TrueMonth() < rhs.TrueMonth() )
        return true;
    else if ( lhs.TrueMonth() > rhs.TrueMonth() )
        return false;
    else
        return ( lhs.Day() < rhs.Day() );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CHINESEDATE_HPP

#ifndef OLDHINDULUNISOLARDATE_HPP
#define OLDHINDULUNISOLARDATE_HPP
/*
  OldHinduLunisolarDate.hpp
  Copyright (C) 2007 David M. Anderson

  OldHinduLunisolarDate class, representing a date in the old Hindu lunisolar
  calendar.
*/


#include "DateJD.hpp"
#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class OldHinduLunisolarDate
    :   public DateJD
{
public:
    explicit OldHinduLunisolarDate( bool today = false );
    explicit OldHinduLunisolarDate( int julianDay );
    OldHinduLunisolarDate( int day, int month, bool monthLeap, int year );
    explicit OldHinduLunisolarDate( const DateJD & date );
    OldHinduLunisolarDate( const OldHinduLunisolarDate & date );
    virtual ~OldHinduLunisolarDate( );

    OldHinduLunisolarDate & operator=( const DateJD & rhs );
    OldHinduLunisolarDate & operator=( const OldHinduLunisolarDate & rhs );

    virtual void Set( bool today );
    virtual void Set( int julianDay );
    void Set( int day, int month, bool monthLeap, int year );

    virtual bool Valid( ) const;
    void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );

    virtual int JulianDay( ) const;
    int Day( ) const;
    int Month( ) const;
    bool IsMonthLeap( ) const;
    int Year( ) const;

    virtual int DayOfWeek( ) const;
    virtual int DaysUntilWeekday( int weekday, int n ) const;

    virtual void Increment( int days );

    std::string ToString( const std::string & format
                                  = m_defaultFormat ) const;
    
    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef class OldHinduLunisolarCalendar Calendar;
    typedef class HinduWeek Week;


protected:
    int m_day;
    int m_month;
    bool m_monthLeap;
    int m_year;

    static std::string m_defaultFormat;
};

//.............................................................................

bool operator==( const OldHinduLunisolarDate & lhs,
                 const OldHinduLunisolarDate & rhs );
bool operator<( const OldHinduLunisolarDate & lhs,
                const OldHinduLunisolarDate & rhs );

#ifdef DEBUG
bool TestOldHinduLunisolarDate( );
#endif


//*****************************************************************************


inline 
void 
OldHinduLunisolarDate::Set( int day, int month, bool monthLeap, int year )
{
    DateJD::Set( DateJD::INVALID );
    m_day = day;
    m_month = month;
    m_monthLeap = monthLeap;
    m_year = year;
}

//=============================================================================

inline 
OldHinduLunisolarDate::OldHinduLunisolarDate( int day, int month,
                                              bool monthLeap, int year )
    :   DateJD( false ),
        m_day( day ),
        m_month( month ),
        m_monthLeap( monthLeap ),
        m_year( year )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
OldHinduLunisolarDate::OldHinduLunisolarDate(
    const OldHinduLunisolarDate & date )
    :    DateJD( false )
{
    Set( date.m_day, date.m_month, m_monthLeap, date.m_year );
    if ( date.DateJD::Valid() )
        DateJD::Set( date.JulianDay() );
}

//-----------------------------------------------------------------------------

inline 
OldHinduLunisolarDate::~OldHinduLunisolarDate( )
{
}

//=============================================================================

inline 
OldHinduLunisolarDate & 
OldHinduLunisolarDate::operator=( const OldHinduLunisolarDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.m_day, rhs.m_month, rhs.m_monthLeap, rhs.m_year );
    if ( rhs.DateJD::Valid() )
        DateJD::Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

inline 
int 
OldHinduLunisolarDate::Day( ) const
{
    return m_day;
}

//-----------------------------------------------------------------------------

inline 
int 
OldHinduLunisolarDate::Month( ) const
{
    return m_month;
}

//-----------------------------------------------------------------------------

inline 
bool 
OldHinduLunisolarDate::IsMonthLeap( ) const
{
    return m_monthLeap;
}

//-----------------------------------------------------------------------------

inline 
int 
OldHinduLunisolarDate::Year( ) const
{
    return m_year;
}

//=============================================================================

inline     
void 
OldHinduLunisolarDate::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

inline 
const std::string & 
OldHinduLunisolarDate::DefaultFormat( )
{
    return m_defaultFormat;
}

//=============================================================================

inline 
bool 
operator==( const OldHinduLunisolarDate & lhs, const OldHinduLunisolarDate & rhs )
{
    return ( (lhs.Day() == rhs.Day()) && (lhs.Month() == rhs.Month())
             && (lhs.IsMonthLeap() == rhs.IsMonthLeap())
             && (lhs.Year() == rhs.Year()) );
}

//-----------------------------------------------------------------------------

inline 
bool 
operator<( const OldHinduLunisolarDate & lhs, const OldHinduLunisolarDate & rhs )
{
    if ( lhs.Year() < rhs.Year() )
        return true;
    else if ( lhs.Year() > rhs.Year() )
        return false;
    else if ( lhs.Month() < rhs.Month() )
        return true;
    else if ( lhs.Month() > rhs.Month() )
        return false;
    else if ( lhs.IsMonthLeap() && (! rhs.IsMonthLeap()) )
        return true;
    else if ( (! lhs.IsMonthLeap()) && rhs.IsMonthLeap() )
        return false;
    else
        return ( lhs.Day() < rhs.Day() );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //OLDHINDULUNISOLARDATE_HPP

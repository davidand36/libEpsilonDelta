#ifndef HINDULUNISOLARDATE_HPP
#define HINDULUNISOLARDATE_HPP
/*
  HinduLunisolarDate.hpp
  Copyright (C) 2007 David M. Anderson

  HinduLunisolarDate class, representing a date in the old Hindu lunisolar
  calendar.
*/


#include "DateJD.hpp"
#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//=============================================================================


class HinduLunisolarDate
    :   public DateJD
{
public:
    explicit HinduLunisolarDate( bool today = false );
    explicit HinduLunisolarDate( long julianDay );
    HinduLunisolarDate( int day, bool dayLeap,
                        int month, bool monthLeap, long year );
    explicit HinduLunisolarDate( const DateJD & date );
    HinduLunisolarDate( const HinduLunisolarDate & date );
    virtual ~HinduLunisolarDate( );

    HinduLunisolarDate & operator=( const DateJD & rhs );
    HinduLunisolarDate & operator=( const HinduLunisolarDate & rhs );

    virtual void Set( bool today );
    virtual void Set( long julianDay );
    void Set( int day, bool dayLeap, int month, bool monthLeap, long year );

    virtual bool Valid( ) const;
    void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );

    virtual long JulianDay( ) const;
    int Day( ) const;
    bool IsDayLeap( ) const;
    int Month( ) const;
    bool IsMonthLeap( ) const;
    long Year( ) const;

    virtual int DayOfWeek( ) const;
    virtual int DaysUntilWeekday( int weekday, int n ) const;

    virtual void Increment( int days );

    std::string ToString( const std::string & format
                                  = m_defaultFormat ) const;
    
    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef class HinduLunisolarCalendar Calendar;
    typedef class HinduWeek Week;


protected:
    int m_day;
    bool m_dayLeap;
    int m_month;
    bool m_monthLeap;
    long m_year;

    static std::string m_defaultFormat;
};

//.............................................................................

bool operator==( const HinduLunisolarDate & lhs,
                 const HinduLunisolarDate & rhs );
bool operator<( const HinduLunisolarDate & lhs,
                const HinduLunisolarDate & rhs );

#ifdef DEBUG
bool TestHinduLunisolarDate( );
#endif


//*****************************************************************************


inline 
void 
HinduLunisolarDate::Set( int day, bool dayLeap,
                         int month, bool monthLeap, long year )
{
    DateJD::Set( DateJD::INVALID );
    m_day = day;
    m_dayLeap = dayLeap;
    m_month = month;
    m_monthLeap = monthLeap;
    m_year = year;
}

//=============================================================================

inline 
HinduLunisolarDate::HinduLunisolarDate( int day, bool dayLeap,
                                        int month, bool monthLeap, long year )
    :   DateJD( false ),
        m_day( day ),
        m_dayLeap( dayLeap ),
        m_month( month ),
        m_monthLeap( monthLeap ),
        m_year( year )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
HinduLunisolarDate::HinduLunisolarDate(
    const HinduLunisolarDate & date )
    :    DateJD( false )
{
    Set( date.m_day, date.m_dayLeap, date.m_month, m_monthLeap, date.m_year );
    if ( date.DateJD::Valid() )
        DateJD::Set( date.JulianDay() );
}

//-----------------------------------------------------------------------------

inline 
HinduLunisolarDate::~HinduLunisolarDate( )
{
}

//=============================================================================

inline 
HinduLunisolarDate & 
HinduLunisolarDate::operator=( const HinduLunisolarDate & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.m_day, rhs.m_dayLeap, rhs.m_month, rhs.m_monthLeap, rhs.m_year );
    if ( rhs.DateJD::Valid() )
        DateJD::Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

inline 
int 
HinduLunisolarDate::Day( ) const
{
    return m_day;
}

//-----------------------------------------------------------------------------

inline 
bool 
HinduLunisolarDate::IsDayLeap( ) const
{
    return m_dayLeap;
}

//-----------------------------------------------------------------------------

inline 
int 
HinduLunisolarDate::Month( ) const
{
    return m_month;
}

//-----------------------------------------------------------------------------

inline 
bool 
HinduLunisolarDate::IsMonthLeap( ) const
{
    return m_monthLeap;
}

//-----------------------------------------------------------------------------

inline 
long 
HinduLunisolarDate::Year( ) const
{
    return m_year;
}

//=============================================================================

inline     
void 
HinduLunisolarDate::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

inline 
const std::string & 
HinduLunisolarDate::DefaultFormat( )
{
    return m_defaultFormat;
}

//=============================================================================

inline 
bool 
operator==( const HinduLunisolarDate & lhs, const HinduLunisolarDate & rhs )
{
    return ( (lhs.Day() == rhs.Day()) && (lhs.IsDayLeap() == rhs.IsDayLeap())
             && (lhs.Month() == rhs.Month())
             && (lhs.IsMonthLeap() == rhs.IsMonthLeap())
             && (lhs.Year() == rhs.Year()) );
}

//-----------------------------------------------------------------------------

inline 
bool 
operator<( const HinduLunisolarDate & lhs, const HinduLunisolarDate & rhs )
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
    else if ( lhs.Day() < rhs.Day() )
        return true;
    else if ( lhs.Day() > rhs.Day() )
        return false;
    else if ( (! lhs.IsDayLeap()) && rhs.IsDayLeap() )
        return true;
    return false;
 }


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HINDULUNISOLARDATE_HPP

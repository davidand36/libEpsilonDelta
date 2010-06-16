#ifndef ISO8601DATE_HPP
#define ISO8601DATE_HPP
/*
  ISO8601Date.hpp
  Copyright (C) 2007 David M. Anderson

  ISO8601Date class, representing a date in the ISO 8601 calendar.
*/


#include "ISO8601Calendar.hpp"
#include "DateJD.hpp"
#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ISO8601Date
    :   public DateJD
{
public:
    explicit ISO8601Date( bool today = false );
    explicit ISO8601Date( long julianDay );
    ISO8601Date( int day, int week, long year );
    explicit ISO8601Date( const DateJD & date );
    ISO8601Date( const ISO8601Date & date );
    virtual ~ISO8601Date( );

    ISO8601Date & operator=( const DateJD & rhs );
    ISO8601Date & operator=( const ISO8601Date & rhs );

    virtual void Set( bool today );
    virtual void Set( long julianDay );
    void Set( int day, int week, long year );

    virtual bool Valid( ) const;
    void MakeValid( DateFixup::EMethod fixupMethod = DateFixup::Clamp );

    virtual long JulianDay( ) const;
    int Day( ) const;
    int Week( ) const;
    long Year( ) const;

    virtual void Increment( int days );
    void Increment( int days, int weeks, long years,
                    DateFixup::EMethod fixupMethod = DateFixup::Carry );

    std::string ToString( const std::string & format
                                  = m_defaultFormat ) const;
    
    static void SetDefaultFormat( const std::string & format );
    static const std::string & DefaultFormat( );

    typedef ISO8601Calendar Calendar;


private:
    int m_day;
    int m_week;
    long m_year;

    static std::string m_defaultFormat;
};

//.............................................................................

bool operator==( const ISO8601Date & lhs, const ISO8601Date & rhs );
bool operator<( const ISO8601Date & lhs, const ISO8601Date & rhs );

#ifdef DEBUG
bool TestISO8601Date( );
#endif


//*****************************************************************************


inline 
void 
ISO8601Date::Set( int day, int week, long year )
{
    DateJD::Set( DateJD::INVALID );
    m_day = day;
    m_week = week;
    m_year = year;
}

//=============================================================================

inline 
ISO8601Date::ISO8601Date( int day, int week, long year )
    :   DateJD( false ),
        m_day( day ),
        m_week( week ),
        m_year( year )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
ISO8601Date::ISO8601Date( const ISO8601Date & date )
    :    DateJD( false )
{
    Set( date.m_day, date.m_week, date.m_year );
    if ( date.DateJD::Valid() )
        DateJD::Set( date.JulianDay() );
}

//-----------------------------------------------------------------------------

inline 
ISO8601Date::~ISO8601Date( )
{
}

//=============================================================================

inline 
ISO8601Date & 
ISO8601Date::operator=( const ISO8601Date & rhs )
{
    if ( &rhs == this )
        return *this;
    Set( rhs.m_day, rhs.m_week, rhs.m_year );
    if ( rhs.DateJD::Valid() )
        DateJD::Set( rhs.JulianDay() );
    return *this;
}

//=============================================================================

inline 
int 
ISO8601Date::Day( ) const
{
    return m_day;
}

//-----------------------------------------------------------------------------

inline 
int 
ISO8601Date::Week( ) const
{
    return m_week;
}

//-----------------------------------------------------------------------------

inline 
long 
ISO8601Date::Year( ) const
{
    return m_year;
}

//=============================================================================

inline     
void 
ISO8601Date::SetDefaultFormat( const std::string & format )
{
    m_defaultFormat = format;
}

//-----------------------------------------------------------------------------

inline 
const std::string & 
ISO8601Date::DefaultFormat( )
{
    return m_defaultFormat;
}

//=============================================================================

inline 
bool 
operator==( const ISO8601Date & lhs, const ISO8601Date & rhs )
{
    return ( (lhs.Day() == rhs.Day()) && (lhs.Week() == rhs.Week())
             && (lhs.Year() == rhs.Year()) );
}

//-----------------------------------------------------------------------------

inline 
bool 
operator<( const ISO8601Date & lhs, const ISO8601Date & rhs )
{
    if ( lhs.Year() < rhs.Year() )
        return true;
    else if ( lhs.Year() > rhs.Year() )
        return false;
    else if ( lhs.Week() < rhs.Week() )
        return true;
    else if ( lhs.Week() > rhs.Week() )
        return false;
    else
        return ( lhs.Day() < rhs.Day() );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ISO8601DATE_HPP

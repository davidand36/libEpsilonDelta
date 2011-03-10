#ifndef ANGLEHMS_HPP
#define ANGLEHMS_HPP
/*
  AngleHMS.hpp
  Copyright (C) 2007 David M. Anderson

  AngleHMS class: represents an angle in hours, minutes, and seconds.
  NOTES:
  1. Hour angles range from 00:00:00. to 23:59:59.9+.
*/


#include "Assert.hpp"
#include <cmath>
#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Angle;


class AngleHMS
{
public:
    AngleHMS( );
    AngleHMS( int hours, int minutes = 0, double seconds = 0. );
    AngleHMS( double hours );
    AngleHMS( const Angle & angle );
    void Set( int hours = 0, int minutes = 0, double seconds = 0. );
    void Set( double hours );
    void Set( const Angle & angle );
    int Hours( ) const;
    int Minutes( ) const;
    double Seconds( ) const;
    double TotalHours( ) const;
#ifdef DEBUG
    static bool Test( );
#endif

private:
    int m_hours;
    int m_minutes;
    double m_seconds;

    void Init( double hours );
};

//.............................................................................

std::ostream & operator<<( std::ostream & out, const AngleHMS & angle );
std::string ToJSON( const AngleHMS & angle );
void FromJSON( const std::string & json, AngleHMS * pAngle );


//*****************************************************************************


inline 
AngleHMS::AngleHMS( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
AngleHMS::AngleHMS( int hours, int minutes, double seconds )
    :   m_hours( hours ),
        m_minutes( minutes ),
        m_seconds( seconds )
{
    Assert( hours >= 0 );
    Assert( hours < 24 );
    Assert( minutes >= 0 );
    Assert( minutes < 60 );
    Assert( seconds >= 0 );
    Assert( seconds < 60. );
}

//=============================================================================

inline 
void 
AngleHMS::Set( int hours, int minutes, double seconds )
{
    Assert( hours >= 0 );
    Assert( hours < 24 );
    Assert( minutes >= 0 );
    Assert( minutes < 60 );
    Assert( seconds >= 0 );
    Assert( seconds < 60. );
    m_hours = hours;
    m_minutes = minutes;
    m_seconds = seconds;
}

//=============================================================================

inline 
int 
AngleHMS::Hours( ) const
{
    return m_hours;
}

//-----------------------------------------------------------------------------

inline 
int 
AngleHMS::Minutes( ) const
{
    return m_minutes;
}

//-----------------------------------------------------------------------------

inline 
double 
AngleHMS::Seconds( ) const
{
    return m_seconds;
}

//-----------------------------------------------------------------------------

inline 
double 
AngleHMS::TotalHours( ) const
{
    return (m_hours  +  m_minutes / 60.  +  m_seconds / 3600.);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ANGLEHMS_HPP

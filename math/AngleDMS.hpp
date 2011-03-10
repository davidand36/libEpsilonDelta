#ifndef ANGLEDMS_HPP
#define ANGLEDMS_HPP
/*
  AngleDMS.hpp
  Copyright (C) 2007 David M. Anderson

  AngleDMS class: represents an angle in degrees, minutes, and seconds.
  NOTES:
  1. For negative angles, only the degrees field is negative.
     So, for example, -60.83 degrees is treated as -60d 49m 48.s.
*/


#include "Assert.hpp"
#include <cmath>
#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Angle;


class AngleDMS
{
public:
    AngleDMS( );
    AngleDMS( int degrees, int minutes = 0, double seconds = 0. );
    AngleDMS( double degrees );
    AngleDMS( const Angle & angle );
    void Set( int degrees = 0, int minutes = 0, double seconds = 0. );
    void Set( double degrees );
    void Set( const Angle & angle );
    bool Positive( ) const;
    int Degrees( ) const;
    int Minutes( ) const;
    double Seconds( ) const;
    double TotalDegrees( ) const;
#ifdef DEBUG
    static bool Test( );
#endif

private:
    bool m_positive;
    int m_degrees;
    int m_minutes;
    double m_seconds;
};

//.............................................................................

std::ostream & operator<<( std::ostream & out, const AngleDMS & angle );
std::string ToJSON( const AngleDMS & angle );
void FromJSON( const std::string & json, AngleDMS * pAngle );


//*****************************************************************************


inline 
AngleDMS::AngleDMS( )
{
}

//=============================================================================

inline 
bool 
AngleDMS::Positive( ) const
{
    return m_positive;
}

//-----------------------------------------------------------------------------

inline 
int 
AngleDMS::Degrees( ) const
{
    return m_degrees;
}

//-----------------------------------------------------------------------------

inline 
int 
AngleDMS::Minutes( ) const
{
    return m_minutes;
}

//-----------------------------------------------------------------------------

inline 
double 
AngleDMS::Seconds( ) const
{
    return m_seconds;
}

//-----------------------------------------------------------------------------

inline 
double 
AngleDMS::TotalDegrees( ) const
{
    if ( m_positive )
        return (m_degrees  +  m_minutes / 60.  +  m_seconds / 3600.);
    else
        return (- (m_degrees  +  m_minutes / 60.  +  m_seconds / 3600.));
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ANGLEDMS_HPP

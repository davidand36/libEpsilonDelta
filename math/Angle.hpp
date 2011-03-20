#ifndef ANGLE_HPP
#define ANGLE_HPP
/*
  Angle.hpp
  Copyright (C) 2007 David M. Anderson

  Angle class: represents an angle in radians.
  NOTES:
  1. Normalized angles are between -pi and pi (inclusive). 
     Angles returned from functions (other than constructors) are normalized.
     Abs() returns an angle between 0 and pi (inclusive).
     NormalizePositive() will adjust the angle to be between 0 and 2*pi. It is
     not used internally, but is for the convenience of applications that
     require angles in this range.
  2. By default angles are normalized after arithmetic operations, but this
     can be changed on a global basis by calling NormalizeOperators( false ).
     That might be proper, for instance, if the Angle is actually an angular
     velocity (rads/sec), where values greater than pi are meaningful.
  3. Because of the default argument, ArcTan() can take either one argument or
     two. If one, it returns an angle between -pi/2 and pi/2 (like atan()).
     If two, it returns an angle between -pi and pi (like atan2()).
  4. Hav() and ArcHav() refer to the haversine:
     hav(x) = (1-cos(x))/2 = sin^2(x/2).
  5. MakeSinCosTables(Angle,int,...) computes sin(i*angle) and cos(i*angle)
     for i=0,...,maxMultiple, inclusive.
     MakeSinCosTables(int,...) does the same for a given number of divisions
     of a full circle.
     Either of the table arguments may be null.
*/


#include <cmath>
#include <vector>
#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class AngleDMS;
class AngleHMS;

//*****************************************************************************


class Angle
{
public:
    enum Unit { Radian, Degree, ArcSecond, Grad, Cycle };

    Angle( );
    Angle( double value, Unit unit = Radian );
    Angle( const AngleDMS & dms );
    Angle( const AngleHMS & hms );
    void Set( double value = 0., Unit unit = Radian );
    void Set( const AngleDMS & dms );
    void Set( const AngleHMS & hms );
    void Normalize( );
    void NormalizePositive( );
    Angle Abs( ) const;
    double Radians( ) const;
    double Degrees( ) const;
    double Grads( ) const;
    double Cycles( ) const;
    double Sin( ) const;
    double Cos( ) const;
    double Tan( ) const;
    double Hav( ) const;
    bool operator==( Angle rhs ) const;
    bool operator<( Angle rhs ) const;
    bool operator<=( Angle rhs ) const;
    bool operator>( Angle rhs ) const;
    bool operator>=( Angle rhs ) const;
    Angle operator-( ) const;
    Angle & operator+=( Angle rhs );
    Angle & operator-=( Angle rhs );
    Angle & operator*=( double rhs );
    Angle & operator/=( double rhs );

    static void NormalizeOperators( bool normalize = true );
    static bool OperatorsNormalized( );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    double m_radians;

    static bool ms_operatorsNormalized;
};

//.............................................................................

Angle ArcSin( double s );
Angle ArcCos( double c );
Angle ArcTan( double s, double c = 1. );
Angle ArcHav( double h );
Angle operator+( Angle lhs, Angle rhs );
Angle operator-( Angle lhs, Angle rhs );
Angle operator*( double lhs, Angle rhs );
Angle operator*( Angle lhs, double rhs );
Angle operator/( Angle lhs, double rhs );
double Sin( Angle angle );
double Cos( Angle angle );
double Tan( Angle angle );
double Hav( Angle angle );
void MakeSinCosTables( Angle angle, int maxMultiple,
                       std::vector<double> * pSinTable,
                       std::vector<double> * pCosTable );
void MakeSinCosTables( int divisions,
                       std::vector<double> * pSinTable,
                       std::vector<double> * pCosTable );
std::ostream & operator<<( std::ostream & out, Angle angle );
std::string ToJSON( Angle angle );
void FromJSON( const std::string & json, Angle * pAngle );


//*****************************************************************************


inline 
void 
Angle::Set( double value, Unit unit )
{
    switch ( unit )
    {
    case Radian:
        m_radians = value;
        break;
    case Degree:
        m_radians = value * M_PI / 180.;
        break;
    case ArcSecond:
        m_radians = value * M_PI / (180. * 3600.);
        break;
    case Grad:
        m_radians = value * M_PI / 200.;
        break;
    case Cycle:
        m_radians = value * 2 * M_PI;
        break;
    }
}

//=============================================================================

inline
Angle::Angle( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Angle::Angle( double value, Unit unit )
{
    Set( value, unit );
}

//=============================================================================

inline 
void 
Angle::Normalize( )
{
    if ( (-M_PI < m_radians) && (m_radians <= M_PI) )
        return;
    m_radians = std::fmod( m_radians, 2. * M_PI );
    while ( m_radians <= - M_PI )
        m_radians += 2. * M_PI;
    while ( m_radians > M_PI )
        m_radians -= 2. * M_PI;
}

//-----------------------------------------------------------------------------

inline 
void 
Angle::NormalizePositive( )
{
    if ( (0. <= m_radians) && (m_radians < 2. * M_PI) )
        return;
    m_radians = std::fmod( m_radians, 2. * M_PI );
    while ( m_radians < 0. )
        m_radians += 2. * M_PI;
    while ( m_radians >= 2. * M_PI )
        m_radians -= 2. * M_PI;
}

//=============================================================================

inline 
Angle 
Angle::Abs( ) const
{
    Angle a = *this;
    a.Normalize( );
    a.m_radians = std::fabs( a.m_radians );
    return a;
}

//=============================================================================

inline 
double 
Angle::Radians( ) const
{
    return m_radians;
}

//-----------------------------------------------------------------------------

inline 
double 
Angle::Degrees( ) const
{
    return (m_radians * 180. / M_PI);
}

//-----------------------------------------------------------------------------

inline 
double 
Angle::Grads( ) const
{
    return (m_radians * 200. / M_PI);
}

//-----------------------------------------------------------------------------

inline 
double 
Angle::Cycles( ) const
{
    return (m_radians / (2 * M_PI));
}

//=============================================================================

inline 
double 
Angle::Sin( ) const
{
    return std::sin( m_radians );
}

//-----------------------------------------------------------------------------

inline 
double 
Angle::Cos( ) const
{
    return std::cos( m_radians );
}

//-----------------------------------------------------------------------------

inline 
double 
Angle::Hav( ) const
{
    double s = std::sin( m_radians / 2. );
    return (s * s);
}

//=============================================================================

inline 
bool 
Angle::operator==( Angle rhs ) const
{
    return (m_radians == rhs.m_radians);
}

//-----------------------------------------------------------------------------

inline 
bool 
Angle::operator<( Angle rhs ) const
{
    return (m_radians < rhs.m_radians);
}

//-----------------------------------------------------------------------------

inline 
bool 
Angle::operator<=( Angle rhs ) const
{
    return (m_radians <= rhs.m_radians);
}

//-----------------------------------------------------------------------------

inline 
bool 
Angle::operator>( Angle rhs ) const
{
    return (m_radians > rhs.m_radians);
}

//-----------------------------------------------------------------------------

inline 
bool 
Angle::operator>=( Angle rhs ) const
{
    return (m_radians >= rhs.m_radians);
}

//=============================================================================

inline 
Angle 
Angle::operator-( ) const
{
    return Angle( - m_radians );
}

//-----------------------------------------------------------------------------

inline
Angle & 
Angle::operator+=( Angle rhs )
{
    m_radians += rhs.m_radians;
    if ( ms_operatorsNormalized )
        Normalize( );
    return *this;
}

//-----------------------------------------------------------------------------

inline
Angle & 
Angle::operator-=( Angle rhs )
{
    m_radians -= rhs.m_radians;
    if ( ms_operatorsNormalized )
        Normalize( );
    return *this;
}

//-----------------------------------------------------------------------------

inline 
Angle & 
Angle::operator*=( double rhs )
{
    m_radians *= rhs;
    if ( ms_operatorsNormalized )
        Normalize( );
    return *this;
}

//-----------------------------------------------------------------------------

inline 
Angle & 
Angle::operator/=( double rhs )
{
    m_radians /= rhs;
    if ( ms_operatorsNormalized )
        Normalize( );
    return *this;
}

//-----------------------------------------------------------------------------

inline 
Angle 
operator+( Angle lhs, Angle rhs )
{
    Angle a = lhs;
    return a += rhs;
}

//-----------------------------------------------------------------------------

inline 
Angle 
operator-( Angle lhs, Angle rhs )
{
    Angle a = lhs;
    return a -= rhs;
}

//-----------------------------------------------------------------------------

inline 
Angle 
operator*( double lhs, Angle rhs )
{
    Angle a = rhs;
    return a *= lhs;
}

//-----------------------------------------------------------------------------

inline 
Angle 
operator*( Angle lhs, double rhs )
{
    Angle a = lhs;
    return a *= rhs;
}

//-----------------------------------------------------------------------------

inline 
Angle 
operator/( Angle lhs, double rhs )
{
    Angle a = lhs;
    return a /= rhs;
}

//=============================================================================

inline 
double 
Sin( Angle angle )
{
    return angle.Sin( );
}

//-----------------------------------------------------------------------------

inline 
double 
Cos( Angle angle )
{
    return angle.Cos( );
}

//-----------------------------------------------------------------------------

inline 
double 
Tan( Angle angle )
{
    return angle.Tan( );
}

//-----------------------------------------------------------------------------

inline 
double 
Hav( Angle angle )
{
    return angle.Hav( );
}

//=============================================================================

inline 
void 
Angle::NormalizeOperators( bool normalize )
{
    ms_operatorsNormalized = normalize;
}

//-----------------------------------------------------------------------------

inline 
bool 
Angle::OperatorsNormalized( )
{
    return ms_operatorsNormalized;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ANGLE_HPP

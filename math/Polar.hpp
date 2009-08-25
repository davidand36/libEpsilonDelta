#ifndef POLAR_HPP
#define POLAR_HPP
/*
  Polar.hpp
  Copyright (C) 2007 David M. Anderson

  Polar class: representing (2-dimensional) polar coordinates.
*/


#include "Angle.hpp"
#include "Point2.hpp"
#include "Assert.hpp"
#include <string>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Polar
{
public:
    Polar( );
    Polar( Angle bearing, double distance = 1. );
    Polar( Point2D rectangular );
    void Set( );
    void Set( Angle bearing, double distance = 1. );
    void Set( Point2D rectangular );
    Angle Bearing( ) const;
    double Distance( ) const;
    Point2D Rectangular( ) const;
#ifdef DEBUG
    static bool Test( );
#endif

private:
    Angle m_bearing;
    double m_distance;
};

//.............................................................................

std::ostream & operator<<( std::ostream & out, const Polar & polar );
std::string ToJSON( const Polar & polar );
void FromJSON( const std::string & json, Polar * pPolar );


//*****************************************************************************


inline 
void 
Polar::Set( )
{
    m_bearing.Set( );
    m_distance = 0.;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Polar::Set( Angle bearing, double distance )
{
    m_bearing = bearing;
    m_bearing.Normalize( );
    Assert( distance >= 0. );
    m_distance = distance;
}

//=============================================================================

inline 
Polar::Polar( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Polar::Polar( Angle bearing, double distance )
{
    Set( bearing, distance );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Polar::Polar( Point2D rectangular )
{
    Set( rectangular );
}

//=============================================================================

inline 
Angle 
Polar::Bearing( ) const
{
    return m_bearing;
}

//-----------------------------------------------------------------------------

inline 
double 
Polar::Distance( ) const
{
    return m_distance;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //POLAR_HPP

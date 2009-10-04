#ifndef EULERANGLES_HPP
#define EULERANGLES_HPP
/*
  EulerAngles.hpp
  Copyright (C) 2007 David M. Anderson

  EulerAngles class: a triplet of angles.
  Includes EOrder: an enumeration of axis orders.
  NOTES:
  1. Euler angles represent a sequence of rotations about the (fixed)
     coordinate axes. The axis order is specified by an EOrder value.
     The interpretation here is that the corresponding rotation matrices are
     multiplied from left to right and applied to a column vector, so the
     rotation indicated by the last axis letter (and angle2) is applied to
     the vector first. If, e.g., the order is kZXY, then the corresponding
     matrix is
       Rot( 2, angle0 ) * Rot( 0, angle1 ) * Rot( 1, angle2 ),
     where Rot( i, angle ) is a rotation matrix about the ith axis.
  2. If, in an object's coordinate space, the Y-axis is up, the X-axis is
     right, and the (negative) Z-axis is forward, then common sequence of
     applying yaw (heading), pitch, and roll corresponds to kZXY, and
     EulerAngles( roll, pitch, yaw ).
  3. operator[] provides unchecked access.
     At() asserts that index is in the proper range.
*/


#include "Angle.hpp"
#include "Array.hpp"
#include "Assert.hpp"
#include "JSON.hpp"
#include <string>
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class EulerAngles
{
public:
    EulerAngles( );
    EulerAngles( const Angle & angle0, const Angle & angle1,
                 const Angle & angle2 );
    EulerAngles( const Angle * pAngles );
    void Set( const Angle & angle0, const Angle & angle1,
              const Angle & angle2 );
    void Set( const Angle * pAngles );
    const Angle & operator[]( int index ) const;
    Angle & operator[]( int index );
    const Angle & At( int index ) const;
    Angle & At( int index );

    enum EOrder { XYZ, YZX, ZXY, XZY, ZYX, YXZ };

#ifdef DEBUG
    static bool Test( );
#endif

private:
    array< Angle, 3 >   m_angles;

    friend std::string ToJSON( const EulerAngles & euler );
    friend void FromJSON( const std::string & json, EulerAngles * pEuler );
};

//.............................................................................

std::ostream & operator<<( std::ostream & out, const EulerAngles & euler );
std::string ToJSON( const EulerAngles & euler );
void FromJSON( const std::string & json, EulerAngles * pEuler );


//*****************************************************************************


inline 
void 
EulerAngles::Set( const Angle & angle0, const Angle & angle1,
                  const Angle & angle2 )
{
    m_angles[0] = angle0;
    m_angles[1] = angle1;
    m_angles[2] = angle2;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
EulerAngles::Set( const Angle * pAngles )
{
    memcpy( m_angles.c_array(), pAngles, sizeof( m_angles ) );
}

//=============================================================================

inline 
EulerAngles::EulerAngles( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
EulerAngles::EulerAngles( const Angle & angle0, const Angle & angle1,
                          const Angle & angle2 )
{
    Set( angle0, angle1, angle2 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
EulerAngles::EulerAngles( const Angle * pAngles )
{
    Set( pAngles );
}

//=============================================================================

inline 
const Angle & 
EulerAngles::operator[]( int index ) const
{
    return  m_angles[ index ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Angle & 
EulerAngles::operator[]( int index )
{
    return  m_angles[ index ];
}

//-----------------------------------------------------------------------------

inline 
const Angle & 
EulerAngles::At( int index ) const
{
    Assert( (index >= 0) && (index < 3) );
    return  m_angles[ index ];
}

//-----------------------------------------------------------------------------

inline 
Angle & 
EulerAngles::At( int index )
{
    Assert( (index >= 0) && (index < 3) );
    return  m_angles[ index ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EULERANGLES_HPP

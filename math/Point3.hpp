#ifndef POINT3_HPP
#define POINT3_HPP
/*
  Point3.hpp
  Copyright (C) 2007 David M. Anderson

  Point3 template class: 3-dimensional point (Cartesian coordinates).
  NOTES:
  1. It is assumed that T is a simple enough type that passing by value,
     rather than by reference, makes sense. It is also assumed that T values
     can be assigned simply via memcpy().
  2. operator[] provides unchecked access.
     At() asserts that index is in the proper range.
  3. The Array() methods provide direct access to the internal representation,
     primarily for efficient interaction with other libraries.
  4. The distinction between points and vectors, while useful for physics
     and related applications, cannot always be conveniently maintained, so
     explicit conversions between the two classes are provided here.
     ToVector() can be regarded as a shorthand for (*this - Zero), and
     Set( Vector vec ) can be regarded as (Zero + vec).
  5. The difference between two points (operator-) yields a vector. However,
     operator-=, as well as Translate() can be used to change the coordinates
     of a point to correspond to a new origin (rhs), also a point.
  6. The Round() functions are only useful for Point3F, Point3D, and Point3LD.
*/


#include "Vector3.hpp"
#include "FixedPoint.hpp"
#include "JSON.hpp"
#include "Assert.hpp"
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class Point3;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Point3<T> & pt );
template <typename T>
std::string ToJSON( const Point3<T> & pt );
template <typename T>
void FromJSON( const std::string & json, Point3<T> * pPt );

//*****************************************************************************


template <typename T>
class Point3
{
public:
    Point3( );
    Point3( T x, T y, T z );
    Point3( const T * pCoords );
    template < typename U >
    explicit Point3( const Point3<U> & pt );
    explicit Point3( const Vector3<T> & vec );
    void Set( T x = T(), T y = T(), T z = T() );
    void Set( const T * pCoords );
    template < typename U >
    void Set( const Point3<U> & pt );
    void Set( const Vector3<T> & vec );
    T X( ) const;
    T Y( ) const;
    T Z( ) const;
    void SetX( T x );
    void SetY( T y );
    void SetZ( T z );
    const T & operator[]( int index ) const;
    T & operator[]( int index );
    const T & At( int index ) const;
    T & At( int index );
    const T * Array( ) const;
    T * Array( );
    Vector3<T> ToVector( ) const;
    bool operator==( const Point3 & rhs ) const;
    bool operator!=( const Point3 & rhs ) const;
    Point3 & operator+=( const Vector3<T> & rhs );
    Point3 & operator-=( const Vector3<T> & rhs );
    Point3 & operator-=( const Point3 & rhs );
    Point3 & operator*=( const Matrix3<T> & m );

    static const Point3 Zero;

private:
    std::tr1::array< T, 3 >   m_coords;

    friend std::string ToJSON<>( const Point3 & pt );
    friend void FromJSON<>( const std::string & json, Point3 * pPt );
};

//.............................................................................

template <typename T>
Point3<T> operator+( const Point3<T> & lhs, const Vector3<T> & rhs );
template <typename T>
Point3<T> operator-( const Point3<T> & lhs, const Vector3<T> & rhs );
template <typename T>
Vector3<T> operator-( const Point3<T> & lhs, const Point3<T> & rhs );
template <typename T>
Point3<T> Translate( const Point3<T> & pt, const Point3<T> & newOrigin );
template <typename T>
Point3<T> operator*( const Matrix3<T> & m, const Point3<T> & pt );
template <typename T>
Point3<T> operator*( const Point3<T> & pt, const Matrix3<T> & m );
template <typename T>
Point3<T> Round( const Point3<T> & pt );

#ifdef DEBUG
bool TestPoint3( );
#endif


//=============================================================================


typedef Point3<int>         Point3I;
typedef Point3<float>       Point3F;
typedef Point3<double>      Point3D;
typedef Point3<long double> Point3LD;
typedef Point3<Fixed16_8>   Point3Fx16_8;
typedef Point3<Fixed16_12>  Point3Fx16_12;
typedef Point3<Fixed32_6>   Point3Fx32_6;
typedef Point3<Fixed32_8>   Point3Fx32_8;
typedef Point3<Fixed32_12>  Point3Fx32_12;
typedef Point3<Fixed32_16>  Point3Fx32_16;


//*****************************************************************************


template <typename T>
inline 
Point3<T>::Point3( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Point3<T>::Point3( T x, T y, T z )
{
    Set( x, y, z );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Point3<T>::Point3( const T * pCoords )
{
    Set( pCoords );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
template < typename U >
inline
Point3<T>::Point3( const Point3<U> & pt )
{
    Set( pt );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline
Point3<T>::Point3( const Vector3<T> & vec )
{
    Set( vec );
}

//=============================================================================

template <typename T>
inline 
void
Point3<T>::Set( T x, T y, T z )
{
    m_coords[0] = x;
    m_coords[1] = y;
    m_coords[2] = z;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Point3<T>::Set( const T * pCoords )
{
    memcpy( m_coords.data(), pCoords, sizeof( m_coords ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
template < typename U >
inline
void 
Point3<T>::Set( const Point3<U> & pt )
{
    Set( (T)pt[0], (T)pt[1], (T)pt[2] );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline
void 
Point3<T>::Set( const Vector3<T> & vec )
{
    Set( vec.X(), vec.Y(), vec.Z() );
}

//=============================================================================

template <typename T>
inline 
T
Point3<T>::X( ) const
{
    return m_coords[0];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Point3<T>::Y( ) const
{
    return m_coords[1];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Point3<T>::Z( ) const
{
    return m_coords[2];
}

//=============================================================================

template <typename T>
inline 
void
Point3<T>::SetX( T x )
{
    m_coords[0] = x;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Point3<T>::SetY( T y )
{
    m_coords[1] = y;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Point3<T>::SetZ( T z )
{
    m_coords[2] = z;
}

//=============================================================================

template <typename T>
inline 
const T &
Point3<T>::operator[]( int index ) const
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T &
Point3<T>::operator[]( int index )
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
const T &
Point3<T>::At( int index ) const
{
    Assert( (0 <= index) && (index < 3) );
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
T &
Point3<T>::At( int index )
{
    Assert( (0 <= index) && (index < 3) );
    return m_coords[index];
}

//=============================================================================

template <typename T>
inline 
const T * 
Point3<T>::Array( ) const
{
    return m_coords.data();
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T * 
Point3<T>::Array( )
{
    return m_coords.data();
}

//=============================================================================

template <typename T>
inline 
Vector3<T> 
Point3<T>::ToVector( ) const
{
    return Vector3<T>( m_coords[0], m_coords[1], m_coords[2] );
}

//=============================================================================

template <typename T>
inline 
bool 
Point3<T>::operator==( const Point3 & rhs ) const
{
    return ( (m_coords[0] == rhs.m_coords[0])
             && (m_coords[1] == rhs.m_coords[1])
             && (m_coords[2] == rhs.m_coords[2]) );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
bool 
Point3<T>::operator!=( const Point3 & rhs ) const
{
    return ! (*this == rhs);
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point3<T> & 
Point3<T>::operator+=( const Vector3<T> & rhs )
{
    m_coords[0] += rhs[0];
    m_coords[1] += rhs[1];
    m_coords[2] += rhs[2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point3<T> & 
Point3<T>::operator-=( const Vector3<T> & rhs )
{
    m_coords[0] -= rhs[0];
    m_coords[1] -= rhs[1];
    m_coords[2] -= rhs[2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point3<T> & 
Point3<T>::operator-=( const Point3<T> & rhs )
{
    m_coords[0] -= rhs[0];
    m_coords[1] -= rhs[1];
    m_coords[2] -= rhs[2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Point3<T> & 
Point3<T>::operator*=( const Matrix3<T> & m )
{
    T pt0 = m_coords[0];
    T pt1 = m_coords[1];
    m_coords[0] = m(0,0) * pt0  +  m(0,1) * pt1  +  m(0,2) * m_coords[2];
    m_coords[1] = m(1,0) * pt0  +  m(1,1) * pt1  +  m(1,2) * m_coords[2];
    m_coords[2] = m(2,0) * pt0  +  m(2,1) * pt1  +  m(2,2) * m_coords[2];
    return *this;
}

//=============================================================================

template <typename T>
inline 
Point3<T> 
operator+( const Point3<T> & lhs, const Vector3<T> & rhs )
{
    Point3<T> v = lhs;
    return v += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point3<T> 
operator-( const Point3<T> & lhs, const Vector3<T> & rhs )
{
    Point3<T> v = lhs;
    return v -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T> 
operator-( const Point3<T> & lhs, const Point3<T> & rhs )
{
    return Vector3<T>( lhs[0] - rhs[0], lhs[1] - rhs[1], lhs[2] - rhs[2] );
}

//-----------------------------------------------------------------------------

template <typename T>
Point3<T> 
Translate( const Point3<T> & pt, const Point3<T> & newOrigin )
{
    Point3<T> newPt = pt;
    return newPt -= newOrigin;
}

//-----------------------------------------------------------------------------

template <typename T>
Point3<T> 
operator*( const Matrix3<T> & m, const Point3<T> & pt )
{
    return Point3<T>( (m(0,0) * pt[0]  +  m(0,1) * pt[1]  +  m(0,2) * pt[2]),
                      (m(1,0) * pt[0]  +  m(1,1) * pt[1]  +  m(1,2) * pt[2]),
                      (m(2,0) * pt[0]  +  m(2,1) * pt[1]  +  m(2,2) * pt[2]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Point3<T> 
operator*( const Point3<T> & pt, const Matrix3<T> & m )
{
    return Point3<T>( (m(0,0) * pt[0]  +  m(1,0) * pt[1]  +  m(2,0) * pt[2]),
                      (m(0,1) * pt[0]  +  m(1,1) * pt[1]  +  m(2,1) * pt[2]),
                      (m(0,2) * pt[0]  +  m(1,2) * pt[1]  +  m(2,2) * pt[2]) );
}

//=============================================================================

template <typename T>
const Point3<T> Point3<T>::Zero( static_cast<T>( 0 ), static_cast<T>( 0 ),
                                 static_cast<T>( 0 ) );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Point3<T> & pt )
{
    return out << "[ " << pt[0] << ", " << pt[1] << ", " << pt[2] << " ]";
}

//=============================================================================

template <typename T>
Point3<T> Round( const Point3<T> & pt )
{
    return Point3<T>( std::floor( pt[0] + (T)0.5 ),
                      std::floor( pt[1] + (T)0.5 ),
                      std::floor( pt[2] + (T)0.5 ) );
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Point3<T> & pt )
{
    return ToJSON( pt.m_coords );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Point3<T> * pPt )
{
    FromJSON( json, &(pPt->m_coords) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //POINT3_HPP

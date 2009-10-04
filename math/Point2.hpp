#ifndef POINT2_HPP
#define POINT2_HPP
/*
  Point2.hpp
  Copyright (C) 2007 David M. Anderson

  Point2 template class: 2-dimensional point (Cartesian coordinates).
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
*/


#include "Vector2.hpp"
#include "Assert.hpp"
#include "JSON.hpp"
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class Point2;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Point2<T> & pt );
template <typename T>
std::string ToJSON( const Point2<T> & pt );
template <typename T>
void FromJSON( const std::string & json, Point2<T> * pPt );

//*****************************************************************************


template <typename T>
class Point2
{
public:
    Point2( );
    Point2( T x, T y );
    Point2( const T * pCoords );
    template < typename U >
    explicit Point2( const Point2<U> & pt );
    explicit Point2( const Vector2<T> & vec );
    void Set( T x = T(), T y = T() );
    void Set( const T * pCoords );
    template < typename U >
    void Set( const Point2<U> & pt );
    void Set( const Vector2<T> & vec );
    T X( ) const;
    T Y( ) const;
    void SetX( T x );
    void SetY( T y );
    const T & operator[]( int index ) const;
    T & operator[]( int index );
    const T & At( int index ) const;
    T & At( int index );
    const T * Array( ) const;
    T * Array( );
    Vector2<T> ToVector( ) const;
    bool operator==( const Point2 & rhs ) const;
    bool operator!=( const Point2 & rhs ) const;
    Point2 & operator+=( const Vector2<T> & rhs );
    Point2 & operator-=( const Vector2<T> & rhs );
    Point2 & operator-=( const Point2 & rhs );
    Point2 & operator*=( const Matrix2<T> & m );

    static const Point2 Zero;

private:
    array< T, 2 >   m_coords;

    friend std::string ToJSON<>( const Point2 & pt );
    friend void FromJSON<>( const std::string & json, Point2 * pPt );
};

//.............................................................................

template <typename T>
Point2<T> operator+( const Point2<T> & lhs, const Vector2<T> & rhs );
template <typename T>
Point2<T> operator-( const Point2<T> & lhs, const Vector2<T> & rhs );
template <typename T>
Vector2<T> operator-( const Point2<T> & lhs, const Point2<T> & rhs );
template <typename T>
Point2<T> Translate( const Point2<T> & pt, const Point2<T> & newOrigin );
template <typename T>
Point2<T> operator*( const Matrix2<T> & m, const Point2<T> & pt );
template <typename T>
Point2<T> operator*( const Point2<T> & pt, const Matrix2<T> & m );

#ifdef DEBUG
bool TestPoint2( );
#endif


//=============================================================================


typedef Point2<int>    Point2I;
typedef Point2<float>  Point2F;
typedef Point2<double> Point2D;


//*****************************************************************************


template <typename T>
inline 
Point2<T>::Point2( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Point2<T>::Point2( T x, T y )
{
    Set( x, y );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Point2<T>::Point2( const T * pCoords )
{
    Set( pCoords );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
template < typename U >
inline
Point2<T>::Point2( const Point2<U> & pt )
{
    Set( (T)pt.X(), (T)pt.Y() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline
Point2<T>::Point2( const Vector2<T> & vec )
{
    Set( vec );
}

//=============================================================================

template <typename T>
inline 
void
Point2<T>::Set( T x, T y )
{
    m_coords[0] = x;
    m_coords[1] = y;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Point2<T>::Set( const T * pCoords )
{
    memcpy( m_coords.c_array(), pCoords, sizeof( m_coords ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
template < typename U >
inline
void 
Point2<T>::Set( const Point2<U> & pt )
{
    Set( (T)pt.X(), (T)pt.Y() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline
void 
Point2<T>::Set( const Vector2<T> & vec )
{
    Set( vec.X(), vec.Y() );
}

//=============================================================================

template <typename T>
inline 
T
Point2<T>::X( ) const
{
    return m_coords[0];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Point2<T>::Y( ) const
{
    return m_coords[1];
}

//=============================================================================

template <typename T>
inline 
void
Point2<T>::SetX( T x )
{
    m_coords[0] = x;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Point2<T>::SetY( T y )
{
    m_coords[1] = y;
}

//=============================================================================

template <typename T>
inline 
const T &
Point2<T>::operator[]( int index ) const
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T &
Point2<T>::operator[]( int index )
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
const T &
Point2<T>::At( int index ) const
{
    Assert( (0 <= index) && (index < 2) );
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
T &
Point2<T>::At( int index )
{
    Assert( (0 <= index) && (index < 2) );
    return m_coords[index];
}

//=============================================================================

template <typename T>
inline 
const T * 
Point2<T>::Array( ) const
{
    return m_coords.data();
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T * 
Point2<T>::Array( )
{
    return m_coords.c_array();
}

//=============================================================================

template <typename T>
inline 
Vector2<T> 
Point2<T>::ToVector( ) const
{
    return Vector2<T>( m_coords[0], m_coords[1] );
}

//=============================================================================

template <typename T>
inline 
bool 
Point2<T>::operator==( const Point2 & rhs ) const
{
    return ( (m_coords[0] == rhs.m_coords[0])
             && (m_coords[1] == rhs.m_coords[1]) );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
bool 
Point2<T>::operator!=( const Point2 & rhs ) const
{
    return ! (*this == rhs);
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point2<T> & 
Point2<T>::operator+=( const Vector2<T> & rhs )
{
    m_coords[0] += rhs[0];
    m_coords[1] += rhs[1];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point2<T> & 
Point2<T>::operator-=( const Vector2<T> & rhs )
{
    m_coords[0] -= rhs[0];
    m_coords[1] -= rhs[1];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point2<T> & 
Point2<T>::operator-=( const Point2 & rhs )
{
    m_coords[0] -= rhs[0];
    m_coords[1] -= rhs[1];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Point2<T> & 
Point2<T>::operator*=( const Matrix2<T> & m )
{
    T pt0 = m_coords[0];
    m_coords[0] = m(0,0) * pt0  +  m(0,1) * m_coords[1];
    m_coords[1] = m(1,0) * pt0  +  m(1,1) * m_coords[1];
    return *this;
}

//=============================================================================

template <typename T>
inline 
Point2<T> 
operator+( const Point2<T> & lhs, const Vector2<T> & rhs )
{
    Point2<T> v = lhs;
    return v += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Point2<T> 
operator-( const Point2<T> & lhs, const Vector2<T> & rhs )
{
    Point2<T> v = lhs;
    return v -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Vector2<T> 
operator-( const Point2<T> & lhs, const Point2<T> & rhs )
{
    return Vector2<T>( lhs[0] - rhs[0], lhs[1] - rhs[1] );
}

//-----------------------------------------------------------------------------

template <typename T>
Point2<T> 
Translate( const Point2<T> & pt, const Point2<T> & newOrigin )
{
    Point2<T> newPt = pt;
    return newPt -= newOrigin;
}

//-----------------------------------------------------------------------------

template <typename T>
Point2<T> 
operator*( const Matrix2<T> & m, const Point2<T> & pt )
{
    return Point2<T>( (m(0,0) * pt[0]  +  m(0,1) * pt[1]),
                       (m(1,0) * pt[0]  +  m(1,1) * pt[1]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Point2<T> 
operator*( const Point2<T> & pt, const Matrix2<T> & m )
{
    return Vector2<T>( (m(0,0) * pt[0]  +  m(1,0) * pt[1]),
                       (m(0,1) * pt[0]  +  m(1,1) * pt[1]) );
}

//=============================================================================

template <typename T>
const Point2<T> Point2<T>::Zero( static_cast<T>( 0 ), static_cast<T>( 0 ) );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Point2<T> & pt )
{
    return out << "[ " << pt[0] << ", " << pt[1] << " ]";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Point2<T> & pt )
{
    return ToJSON( pt.m_coords );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Point2<T> * pPt )
{
    FromJSON( json, &(pPt->m_coords) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //POINT2_HPP

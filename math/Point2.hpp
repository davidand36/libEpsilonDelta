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
*/


#include "Vector2.hpp"
#include "Assert.hpp"
#include "JSON.hpp"
#include <iostream>


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
    void Set( T x = T(), T y = T() );
    void Set( const T * pCoords );
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
    bool operator==( const Point2 & rhs ) const;
    bool operator!=( const Point2 & rhs ) const;
    Point2 & operator+=( const Vector2<T> & rhs );
    Point2 & operator-=( const Vector2<T> & rhs );

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

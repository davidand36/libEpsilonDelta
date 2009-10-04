#ifndef VECTOR2_HPP
#define VECTOR2_HPP
/*
  Vector2.hpp
  Copyright (C) 2007 David M. Anderson

  Vector2 template class: 2-dimensional vector (Cartesian coordinates).
  NOTES:
  1. It is assumed that T is a simple enough type that passing by value,
     rather than by reference, makes sense. It is also assumed that T values
     can be assigned simply via memcpy().
  2. operator[] provides unchecked access.
     At() asserts that index is in the proper range.
  3. The Array() methods provide direct access to the internal representation,
     primarily for efficient interaction with other libraries.
  4. v *= M, where M is a Matrix2, performs left multiplication, i.e.,
     v = M*v, since this is the more common operation. (Vectors are most
     commonly viewed as column vectors.) operator* provides both left and
     right options.
  5. Length() uses sqrt() and will produce less-than-useful results for (e.g.)
     integer types.
  6. Normalize() scales the length to 1., and is also not appropriate for
     integer types. If you already have the length, you can pass it to
     Normalize() to save some time. If the length is 0, Normalize() will
     throw a NullVectorException.
  7. Perp(vec) returns a vector perpendicular (normal) to vec. There are, of
     course, two choices. I prefer the one which produces a counterclockwise
     pi/2 rotation, rotating kUnitX to kUnitY. (Eberly chooses the other.)
     PerpDot(v1, v2) returns Perp(v1) * v2, which is the determinant of the
     matrix with columns (or rows) [v1 v2]. See F.S. Hill, Jr. "The Pleasures
     of 'Perp Dot' Products" in Graphics Gems IV, for details.
*/


#include "Assert.hpp"
#include "NullVectorException.hpp"
#include "Matrix2.hpp"
#include "JSON.hpp"
#include <cmath>
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

template <typename T>
class Vector2;
template <typename T>
class Matrix2;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Vector2<T> & vec );
template <typename T>
std::string ToJSON( const Vector2<T> & vec );
template <typename T>
void FromJSON( const std::string & json, Vector2<T> * pVec );

//*****************************************************************************


template <typename T>
class Vector2
{
public:
    Vector2( );
    Vector2( T x, T y );
    Vector2( const T * pCoords );
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
    bool operator==( const Vector2 & rhs ) const;
    bool operator!=( const Vector2 & rhs ) const;
    Vector2 operator-( ) const;
    Vector2 & operator+=( const Vector2 & rhs );
    Vector2 & operator-=( const Vector2 & rhs );
    Vector2 & operator*=( T rhs );
    Vector2 & operator*=( const Matrix2<T> & m );
    T Length( ) const;
    T LengthSquared( ) const;
    void Normalize( T length = T() );

    static const Vector2 Zero;
    static const Vector2 UnitX;
    static const Vector2 UnitY;

private:
    std::tr1::array< T, 2 >   m_coords;

    friend std::string ToJSON<>( const Vector2 & vec );
    friend void FromJSON<>( const std::string & json, Vector2 * pVec );
};

//.............................................................................

template <typename T>
Vector2<T> operator+( const Vector2<T> & lhs, const Vector2<T> & rhs );
template <typename T>
Vector2<T> operator-( const Vector2<T> & lhs, const Vector2<T> & rhs );
template <typename T>   //scalar product
Vector2<T> operator*( T lhs, const Vector2<T> & rhs );
template <typename T>   //scalar product
Vector2<T> operator*( const Vector2<T> & lhs, T rhs );
template <typename T>   //inner (dot) product
T operator*( const Vector2<T> & lhs, const Vector2<T> & rhs );
template <typename T>
Vector2<T> operator*( const Matrix2<T> & m, const Vector2<T> & v );
template <typename T>
Vector2<T> operator*( const Vector2<T> & v, const Matrix2<T> & m );
template <typename T>
Vector2<T> Perp( const Vector2<T> & v );
template <typename T>
T PerpDot( const Vector2<T> & v1, const Vector2<T> & v2 );

#ifdef DEBUG
bool TestVector2( );
#endif

//=============================================================================


typedef Vector2<int>    Vector2I;
typedef Vector2<float>  Vector2F;
typedef Vector2<double> Vector2D;


//*****************************************************************************


template <typename T>
inline 
Vector2<T>::Vector2( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Vector2<T>::Vector2( T x, T y )
{
    Set( x, y );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Vector2<T>::Vector2( const T * pCoords )
{
    Set( pCoords );
}

//=============================================================================

template <typename T>
inline 
void
Vector2<T>::Set( T x, T y )
{
    m_coords[0] = x;
    m_coords[1] = y;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Vector2<T>::Set( const T * pCoords )
{
    memcpy( m_coords.data(), pCoords, sizeof( m_coords ) );
}

//=============================================================================

template <typename T>
inline 
T
Vector2<T>::X( ) const
{
    return m_coords[0];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector2<T>::Y( ) const
{
    return m_coords[1];
}

//=============================================================================

template <typename T>
inline 
void
Vector2<T>::SetX( T x )
{
    m_coords[0] = x;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Vector2<T>::SetY( T y )
{
    m_coords[1] = y;
}

//=============================================================================

template <typename T>
inline 
const T &
Vector2<T>::operator[]( int index ) const
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T &
Vector2<T>::operator[]( int index )
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
const T &
Vector2<T>::At( int index ) const
{
    Assert( (0 <= index) && (index < 2) );
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
T &
Vector2<T>::At( int index )
{
    Assert( (0 <= index) && (index < 2) );
    return m_coords[index];
}

//=============================================================================

template <typename T>
inline 
const T * 
Vector2<T>::Array( ) const
{
    return m_coords.data();
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T * 
Vector2<T>::Array( )
{
    return m_coords.data();
}

//=============================================================================

template <typename T>
inline 
bool 
Vector2<T>::operator==( const Vector2 & rhs ) const
{
    return ( (m_coords[0] == rhs.m_coords[0])
             && (m_coords[1] == rhs.m_coords[1]) );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
bool 
Vector2<T>::operator!=( const Vector2 & rhs ) const
{
    return ! (*this == rhs);
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector2<T>
Vector2<T>::operator-( ) const
{
    return Vector2( - m_coords[0], - m_coords[1] );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector2<T> & 
Vector2<T>::operator+=( const Vector2 & rhs )
{
    m_coords[0] += rhs.m_coords[0];
    m_coords[1] += rhs.m_coords[1];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector2<T> & 
Vector2<T>::operator-=( const Vector2 & rhs )
{
    m_coords[0] -= rhs.m_coords[0];
    m_coords[1] -= rhs.m_coords[1];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector2<T> & 
Vector2<T>::operator*=( T rhs )
{
    m_coords[0] *= rhs;
    m_coords[1] *= rhs;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Vector2<T> & 
Vector2<T>::operator*=( const Matrix2<T> & m )
{
    T v0 = m_coords[0];
    m_coords[0] = m(0,0) * v0  +  m(0,1) * m_coords[1];
    m_coords[1] = m(1,0) * v0  +  m(1,1) * m_coords[1];
    return *this;
}

//=============================================================================

template <typename T>
T
Vector2<T>::Length( ) const
{
    return static_cast<T>( std::sqrt( LengthSquared( ) ) );
} 

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector2<T>::LengthSquared( ) const
{
    return (m_coords[0] * m_coords[0] +  m_coords[1] * m_coords[1]);
} 

//-----------------------------------------------------------------------------

template <typename T>
void 
Vector2<T>::Normalize( T length )
{
    T len = ( (length == T()) ? Length() : length );
    if ( len == T() )
        throw NullVectorException();
    T invLen = T( 1. ) / len;
    m_coords[0] *= invLen;
    m_coords[1] *= invLen;
}

//=============================================================================

template <typename T>
inline 
Vector2<T> 
operator+( const Vector2<T> & lhs, const Vector2<T> & rhs )
{
    Vector2<T> v = lhs;
    return v += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector2<T> 
operator-( const Vector2<T> & lhs, const Vector2<T> & rhs )
{
    Vector2<T> v = lhs;
    return v -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector2<T> 
operator*( T lhs, const Vector2<T> & rhs )
{
    Vector2<T> v = rhs;
    return v *= lhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector2<T> 
operator*( const Vector2<T> & lhs, T rhs )
{
    Vector2<T> v = lhs;
    return v *= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T 
operator*( const Vector2<T> & lhs, const Vector2<T> & rhs )
{
    return (lhs[0] * rhs[0]  +  lhs[1] * rhs[1]);
}

//-----------------------------------------------------------------------------

template <typename T>
Vector2<T> 
operator*( const Matrix2<T> & m, const Vector2<T> & v )
{
    return Vector2<T>( (m(0,0) * v[0]  +  m(0,1) * v[1]),
                       (m(1,0) * v[0]  +  m(1,1) * v[1]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector2<T> 
operator*( const Vector2<T> & v, const Matrix2<T> & m )
{
    return Vector2<T>( (m(0,0) * v[0]  +  m(1,0) * v[1]),
                       (m(0,1) * v[0]  +  m(1,1) * v[1]) );
}

//=============================================================================

template <typename T>
inline
Vector2<T> 
Perp( const Vector2<T> & v )
{
    return Vector2<T>( -v.Y(), v.X() );
}

//-----------------------------------------------------------------------------

template <typename T>
T 
PerpDot( const Vector2<T> & v1, const Vector2<T> & v2 )
{
    return  Perp( v1 ) * v2;
}

//=============================================================================

template <typename T>
const Vector2<T> Vector2<T>::Zero( static_cast<T>( 0 ),
                                   static_cast<T>( 0 ) );
template <typename T>
const Vector2<T> Vector2<T>::UnitX( static_cast<T>( 1 ),
                                    static_cast<T>( 0 ) );
template <typename T>
const Vector2<T> Vector2<T>::UnitY( static_cast<T>( 0 ),
                                    static_cast<T>( 1 ) );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Vector2<T> & vec )
{
    return out << "[ " << vec[0] << ", " << vec[1] << " ]";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Vector2<T> & vec )
{
    return ToJSON( vec.m_coords );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Vector2<T> * pVec )
{
    FromJSON( json, &(pVec->m_coords) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //VECTOR2_HPP

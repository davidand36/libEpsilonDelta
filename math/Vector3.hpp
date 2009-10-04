#ifndef VECTOR3_HPP
#define VECTOR3_HPP
/*
  Vector3.hpp
  Copyright (C) 2007 David M. Anderson

  Vector3 template class: 3-dimensional vector (Cartesian coordinates).
  NOTES:
  1. It is assumed that T is a simple enough type that passing by value,
     rather than by reference, makes sense. It is also assumed that T values
     can be assigned simply via memcpy().
  2. operator[] provides unchecked access.
     At() asserts that index is in the proper range.
  3. The Array() methods provide direct access to the internal representation,
     primarily for efficient interaction with other libraries.
  4. v *= M, where M is a Matrix2, performs left multiplication, i.e.,
     v = M*v, since this is the more common operation. operator* provides
     both left and right options.
  5. Length() uses sqrt() and will produce less-than-useful results for (e.g.)
     integer types.
  6. Normalize() scales the length to 1., and is also not appropriate for
     integer types. If you already have the length, you can pass it to
     Normalize() to save some time. If the length is 0, Normalize() will
     throw a NullVectorException.
*/


#include "Assert.hpp"
#include "NullVectorException.hpp"
#include "Matrix3.hpp"
#include "JSON.hpp"
#include <cmath>
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

template <typename T>
class Vector3;
template <typename T>
class Matrix3;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Vector3<T> & vec );
template <typename T>
std::string ToJSON( const Vector3<T> & vec );
template <typename T>
void FromJSON( const std::string & json, Vector3<T> * pVec );

//*****************************************************************************


template <typename T>
class Vector3
{
public:
    Vector3( );
    Vector3( T x, T y, T z );
    Vector3( const T * pCoords );
    void Set( T x = T(), T y = T(), T z = T() );
    void Set( const T * pCoords );
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
    bool operator==( const Vector3 & rhs ) const;
    bool operator!=( const Vector3 & rhs ) const;
    Vector3 operator-( ) const;
    Vector3 & operator+=( const Vector3 & rhs );
    Vector3 & operator-=( const Vector3 & rhs );
    Vector3 & operator*=( T rhs );
    Vector3 & operator*=( const Matrix3<T> & m );
    T Length( ) const;
    T LengthSquared( ) const;
    void Normalize( T length = T() );

    static const Vector3 Zero;
    static const Vector3 UnitX;
    static const Vector3 UnitY;
    static const Vector3 UnitZ;

private:
    std::tr1::array< T, 3 >   m_coords;

    friend std::string ToJSON<>( const Vector3 & vec );
    friend void FromJSON<>( const std::string & json, Vector3 * pVec );
};

//.............................................................................

template <typename T>
Vector3<T> operator+( const Vector3<T> & lhs, const Vector3<T> & rhs );
template <typename T>
Vector3<T> operator-( const Vector3<T> & lhs, const Vector3<T> & rhs );
template <typename T>   //scalar product
Vector3<T> operator*( T lhs, const Vector3<T> & rhs );
template <typename T>   //scalar product
Vector3<T> operator*( const Vector3<T> & lhs, T rhs );
template <typename T>   //inner (dot) product
T operator*( const Vector3<T> & lhs, const Vector3<T> & rhs );
template <typename T>
Vector3<T> Cross( const Vector3<T> & lhs, const Vector3<T> & rhs );
template <typename T>
Vector3<T> operator*( const Matrix3<T> & m, const Vector3<T> & v );
template <typename T>
Vector3<T> operator*( const Vector3<T> & v, const Matrix3<T> & m );

#ifdef DEBUG
bool TestVector3( );
#endif

//=============================================================================


typedef Vector3<int>  Vector3I;
typedef Vector3<float>  Vector3F;
typedef Vector3<double> Vector3D;


//*****************************************************************************


template <typename T>
inline 
Vector3<T>::Vector3( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Vector3<T>::Vector3( T x, T y, T z )
{
    Set( x, y, z );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Vector3<T>::Vector3( const T * pCoords )
{
    Set( pCoords );
}

//=============================================================================

template <typename T>
inline 
void
Vector3<T>::Set( T x, T y, T z )
{
    m_coords[0] = x;
    m_coords[1] = y;
    m_coords[2] = z;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Vector3<T>::Set( const T * pCoords )
{
    memcpy( m_coords.data(), pCoords, sizeof( m_coords ) );
}

//=============================================================================

template <typename T>
inline 
T
Vector3<T>::X( ) const
{
    return m_coords[0];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector3<T>::Y( ) const
{
    return m_coords[1];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector3<T>::Z( ) const
{
    return m_coords[2];
}

//=============================================================================

template <typename T>
inline 
void
Vector3<T>::SetX( T x )
{
    m_coords[0] = x;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Vector3<T>::SetY( T y )
{
    m_coords[1] = y;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Vector3<T>::SetZ( T z )
{
    m_coords[2] = z;
}

//=============================================================================

template <typename T>
inline 
const T &
Vector3<T>::operator[]( int index ) const
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T &
Vector3<T>::operator[]( int index )
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
const T &
Vector3<T>::At( int index ) const
{
    Assert( (0 <= index) && (index < 3) );
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
T &
Vector3<T>::At( int index )
{
    Assert( (0 <= index) && (index < 3) );
    return m_coords[index];
}

//=============================================================================

template <typename T>
inline 
const T * 
Vector3<T>::Array( ) const
{
    return m_coords.data();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
T * 
Vector3<T>::Array( )
{
    return m_coords.data();
}

//=============================================================================

template <typename T>
inline 
bool 
Vector3<T>::operator==( const Vector3 & rhs ) const
{
    return ( (m_coords[0] == rhs.m_coords[0])
             && (m_coords[1] == rhs.m_coords[1])
             && (m_coords[2] == rhs.m_coords[2]) );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
bool 
Vector3<T>::operator!=( const Vector3 & rhs ) const
{
    return ! (*this == rhs);
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T>
Vector3<T>::operator-( ) const
{
    return Vector3( - m_coords[0], - m_coords[1], - m_coords[2] );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T> & 
Vector3<T>::operator+=( const Vector3 & rhs )
{
    m_coords[0] += rhs.m_coords[0];
    m_coords[1] += rhs.m_coords[1];
    m_coords[2] += rhs.m_coords[2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T> & 
Vector3<T>::operator-=( const Vector3 & rhs )
{
    m_coords[0] -= rhs.m_coords[0];
    m_coords[1] -= rhs.m_coords[1];
    m_coords[2] -= rhs.m_coords[2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T> & 
Vector3<T>::operator*=( T rhs )
{
    m_coords[0] *= rhs;
    m_coords[1] *= rhs;
    m_coords[2] *= rhs;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T> & 
Vector3<T>::operator*=( const Matrix3<T> & m )
{
    T v0 = m_coords[0];
    T v1 = m_coords[1];
    m_coords[0] = m(0,0) * v0  +  m(0,1) * v1  +  m(0,2) * m_coords[2];
    m_coords[1] = m(1,0) * v0  +  m(1,1) * v1  +  m(1,2) * m_coords[2];
    m_coords[2] = m(2,0) * v0  +  m(2,1) * v1  +  m(2,2) * m_coords[2];
    return *this;
}

//=============================================================================

template <typename T>
T
Vector3<T>::Length( ) const
{
    return static_cast<T>( std::sqrt( LengthSquared( ) ) );
} 

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector3<T>::LengthSquared( ) const
{
    return (m_coords[0] * m_coords[0] +  m_coords[1] * m_coords[1]
            +  m_coords[2] * m_coords[2]);
} 

//-----------------------------------------------------------------------------

template <typename T>
void 
Vector3<T>::Normalize( T length )
{
    T len = ( (length == T()) ? Length() : length );
    if ( len == T() )
        throw NullVectorException();
    T invLen = T( 1. ) / len;
    m_coords[0] *= invLen;
    m_coords[1] *= invLen;
    m_coords[2] *= invLen;
}

//=============================================================================

template <typename T>
inline 
Vector3<T> 
operator+( const Vector3<T> & lhs, const Vector3<T> & rhs )
{
    Vector3<T> v = lhs;
    return v += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T> 
operator-( const Vector3<T> & lhs, const Vector3<T> & rhs )
{
    Vector3<T> v = lhs;
    return v -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T> 
operator*( T lhs, const Vector3<T> & rhs )
{
    Vector3<T> v = rhs;
    return v *= lhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T> 
operator*( const Vector3<T> & lhs, T rhs )
{
    Vector3<T> v = lhs;
    return v *= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T 
operator*( const Vector3<T> & lhs, const Vector3<T> & rhs )
{
    return (lhs[0] * rhs[0]  +  lhs[1] * rhs[1]  +  lhs[2] * rhs[2]);
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T> 
Cross( const Vector3<T> & lhs, const Vector3<T> & rhs )
{
    return Vector3<T>( (lhs[1] * rhs[2]  -  lhs[2] * rhs[1]),
                       (lhs[2] * rhs[0]  -  lhs[0] * rhs[2]),
                       (lhs[0] * rhs[1]  -  lhs[1] * rhs[0]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T> 
operator*( const Matrix3<T> & m, const Vector3<T> & v )
{
    return Vector3<T>( (m(0,0) * v[0]  +  m(0,1) * v[1]  +  m(0,2) * v[2]),
                       (m(1,0) * v[0]  +  m(1,1) * v[1]  +  m(1,2) * v[2]),
                       (m(2,0) * v[0]  +  m(2,1) * v[1]  +  m(2,2) * v[2]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T> 
operator*( const Vector3<T> & v, const Matrix3<T> & m )
{
    return Vector3<T>( (m(0,0) * v[0]  +  m(1,0) * v[1]  +  m(2,0) * v[2]),
                       (m(0,1) * v[0]  +  m(1,1) * v[1]  +  m(2,1) * v[2]),
                       (m(0,2) * v[0]  +  m(1,2) * v[1]  +  m(2,2) * v[2]) );
}

//=============================================================================

template <typename T>
const Vector3<T> Vector3<T>::Zero( static_cast<T>( 0 ),
                                   static_cast<T>( 0 ),
                                   static_cast<T>( 0 ) );
template <typename T>
const Vector3<T> Vector3<T>::UnitX( static_cast<T>( 1 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 0 ) );
template <typename T>
const Vector3<T> Vector3<T>::UnitY( static_cast<T>( 0 ),
                                    static_cast<T>( 1 ),
                                    static_cast<T>( 0 ) );
template <typename T>
const Vector3<T> Vector3<T>::UnitZ( static_cast<T>( 0 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 1 ) );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Vector3<T> & vec )
{
    return out << "[ " << vec[0] << ", " << vec[1] << ", " << vec[2] << " ]";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Vector3<T> & vec )
{
    return ToJSON( vec.m_coords );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Vector3<T> * pVec )
{
    FromJSON( json, &(pVec->m_coords) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //VECTOR3_HPP

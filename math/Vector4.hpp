#ifndef VECTOR4_HPP
#define VECTOR4_HPP
/*
  Vector4.hpp
  Copyright (C) 2007 David M. Anderson

  Vector4 template class: 4-dimensional vector (Cartesian coordinates).
  NOTES:
  1. It is assumed that T is a simple enough type that passing by value,
     rather than by reference, makes sense. It is also assumed that T values
     can be assigned simply via memcpy().
  2. operator[] provides unchecked access.
     At() asserts that index is in the proper range.
  3. The Array() methods provide direct access to the internal representation,
     primarily for efficient interaction with other libraries.
  4. Length() uses sqrt() and will produce less-than-useful results for (e.g.)
     integer types.
  5. Normalize() scales the length to 1., and is also not appropriate for
     integer types. If you already have the length, you can pass it to
     Normalize() to save some time. If the length is 0, Normalize() will
     throw a NullVectorException.
  6. Homogenize() and Project() divide all coordinates by the last (W).
     If W=0, Project() simply drops the last coordinate, but Homogenize()
     fails, asserting and returning a zero vector.
  7. The Vector3 forms of the constructor and Set() set W=0.
     The Point3 forms set W=1.
*/


#include "Assert.hpp"
#include "NullVectorException.hpp"
#include "Vector3.hpp"
#include "Point3.hpp"
#include "JSON.hpp"
#include <cmath>
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

template <typename T>
class Vector4;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Vector4<T> & vec );
template <typename T>
std::string ToJSON( const Vector4<T> & vec );
template <typename T>
void FromJSON( const std::string & json, Vector4<T> * pVec );

//*****************************************************************************


template <typename T>
class Vector4
{
public:
    Vector4( );
    Vector4( T x, T y, T z, T w );
    Vector4( const T * pCoords );
    Vector4( const Vector3<T> & vec3 );
    Vector4( const Point3<T> & pt3 );
    void Set( T x = T(), T y = T(), T z = T(), T w = T() );
    void Set( const T * pCoords );
    void Set( const Vector3<T> & vec3 );
    void Set( const Point3<T> & pt3 );
    T X( ) const;
    T Y( ) const;
    T Z( ) const;
    T W( ) const;
    void SetX( T x );
    void SetY( T y );
    void SetZ( T z );
    void SetW( T w );
    const T & operator[]( int index ) const;
    T & operator[]( int index );
    const T & At( int index ) const;
    T & At( int index );
    const T * Array( ) const;
    T * Array( );
    bool operator==( const Vector4 & rhs ) const;
    bool operator!=( const Vector4 & rhs ) const;
    Vector4 operator-( ) const;
    Vector4 & operator+=( const Vector4 & rhs );
    Vector4 & operator-=( const Vector4 & rhs );
    Vector4 & operator*=( T rhs );
    T Length( ) const;
    T LengthSquared( ) const;
    void Normalize( T length = T() );
    void Homogenize( );
    Vector3<T> Project3( ) const;

    static const Vector4 Zero;
    static const Vector4 UnitX;
    static const Vector4 UnitY;
    static const Vector4 UnitZ;
    static const Vector4 UnitW;

private:
    std::tr1::array< T, 4 >   m_coords;

    friend std::string ToJSON<>( const Vector4 & vec );
    friend void FromJSON<>( const std::string & json, Vector4 * pVec );
};

//.............................................................................

template <typename T>
Vector4<T> operator+( const Vector4<T> & lhs, const Vector4<T> & rhs );
template <typename T>
Vector4<T> operator-( const Vector4<T> & lhs, const Vector4<T> & rhs );
template <typename T>   //scalar product
Vector4<T> operator*( T lhs, const Vector4<T> & rhs );
template <typename T>   //scalar product
Vector4<T> operator*( const Vector4<T> & lhs, T rhs );
template <typename T>   //inner (dot) product
T operator*( const Vector4<T> & lhs, const Vector4<T> & rhs );

#ifdef DEBUG
bool TestVector4( );
#endif

//=============================================================================


typedef Vector4<float>  Vector4F;
typedef Vector4<double> Vector4D;


//*****************************************************************************


template <typename T>
inline 
Vector4<T>::Vector4( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Vector4<T>::Vector4( T x, T y, T z, T w )
{
    Set( x, y, z, w );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Vector4<T>::Vector4( const T * pCoords )
{
    Set( pCoords );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Vector4<T>::Vector4( const Vector3<T> & vec3 )
{
    Set( vec3 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Vector4<T>::Vector4( const Point3<T> & pt3 )
{
    Set( pt3 );
}

//=============================================================================

template <typename T>
inline 
void
Vector4<T>::Set( T x, T y, T z, T w )
{
    m_coords[0] = x;
    m_coords[1] = y;
    m_coords[2] = z;
    m_coords[3] = w;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Vector4<T>::Set( const T * pCoords )
{
    memcpy( m_coords.data(), pCoords, sizeof( m_coords ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
void 
Vector4<T>::Set( const Vector3<T> & vec3 )
{
    Set( vec3[0], vec3[1], vec3[2], 0 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
void 
Vector4<T>::Set( const Point3<T> & pt3 )
{
    Set( pt3[0], pt3[1], pt3[2], 1 );
}

//=============================================================================

template <typename T>
inline 
T
Vector4<T>::X( ) const
{
    return m_coords[0];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector4<T>::Y( ) const
{
    return m_coords[1];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector4<T>::Z( ) const
{
    return m_coords[2];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector4<T>::W( ) const
{
    return m_coords[3];
}

//=============================================================================

template <typename T>
inline 
void
Vector4<T>::SetX( T x )
{
    m_coords[0] = x;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Vector4<T>::SetY( T y )
{
    m_coords[1] = y;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Vector4<T>::SetZ( T z )
{
    m_coords[2] = z;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Vector4<T>::SetW( T w )
{
    m_coords[3] = w;
}

//=============================================================================

template <typename T>
inline 
const T &
Vector4<T>::operator[]( int index ) const
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T &
Vector4<T>::operator[]( int index )
{
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
const T &
Vector4<T>::At( int index ) const
{
    Assert( (0 <= index) && (index < 4) );
    return m_coords[index];
}

//-----------------------------------------------------------------------------

template <typename T>
T &
Vector4<T>::At( int index )
{
    Assert( (0 <= index) && (index < 4) );
    return m_coords[index];
}

//=============================================================================

template <typename T>
inline 
const T * 
Vector4<T>::Array( ) const
{
    return m_coords.data();
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T * 
Vector4<T>::Array( )
{
    return m_coords.data();
}

//=============================================================================

template <typename T>
inline 
bool 
Vector4<T>::operator==( const Vector4 & rhs ) const
{
    return ( (m_coords[0] == rhs.m_coords[0])
             && (m_coords[1] == rhs.m_coords[1])
             && (m_coords[2] == rhs.m_coords[2])
             && (m_coords[3] == rhs.m_coords[3]) );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
bool 
Vector4<T>::operator!=( const Vector4 & rhs ) const
{
    return ! (*this == rhs);
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector4<T>
Vector4<T>::operator-( ) const
{
    return Vector4( - m_coords[0], - m_coords[1], - m_coords[2],
                    - m_coords[3] );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector4<T> & 
Vector4<T>::operator+=( const Vector4 & rhs )
{
    m_coords[0] += rhs.m_coords[0];
    m_coords[1] += rhs.m_coords[1];
    m_coords[2] += rhs.m_coords[2];
    m_coords[3] += rhs.m_coords[3];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector4<T> & 
Vector4<T>::operator-=( const Vector4 & rhs )
{
    m_coords[0] -= rhs.m_coords[0];
    m_coords[1] -= rhs.m_coords[1];
    m_coords[2] -= rhs.m_coords[2];
    m_coords[3] -= rhs.m_coords[3];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector4<T> & 
Vector4<T>::operator*=( T rhs )
{
    m_coords[0] *= rhs;
    m_coords[1] *= rhs;
    m_coords[2] *= rhs;
    m_coords[3] *= rhs;
    return *this;
}

//=============================================================================

template <typename T>
T
Vector4<T>::Length( ) const
{
    return static_cast<T>( std::sqrt( LengthSquared( ) ) );
} 

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Vector4<T>::LengthSquared( ) const
{
    return (m_coords[0] * m_coords[0] +  m_coords[1] * m_coords[1]
            +  m_coords[2] * m_coords[2] +  m_coords[3] * m_coords[3]);
} 

//-----------------------------------------------------------------------------

template <typename T>
void 
Vector4<T>::Normalize( T length )
{
    T len = ( (length == T()) ? Length() : length );
    if ( len == T() )
        throw NullVectorException();
    T invLen = T( 1. ) / len;
    m_coords[0] *= invLen;
    m_coords[1] *= invLen;
    m_coords[2] *= invLen;
    m_coords[3] *= invLen;
}

//-----------------------------------------------------------------------------

template <typename T>
void 
Vector4<T>::Homogenize( )
{
    Assert( m_coords[3] != T() );
    if ( m_coords[3] == T() )
    {
        Set( );
        return;
    }
    T invW = static_cast<T>(1.) / m_coords[3];
    m_coords[0] *= invW;
    m_coords[1] *= invW;
    m_coords[2] *= invW;
    m_coords[3] = static_cast<T>(1);
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T>
Vector4<T>::Project3( ) const
{
    if ( m_coords[3] == T() )
        return Vector3<T>( m_coords[0], m_coords[1], m_coords[2] );
    T invW = static_cast<T>(1.) / m_coords[3];
    return Vector3<T>( m_coords[0] * invW,
                       m_coords[1] * invW,
                       m_coords[2] * invW );
}

//=============================================================================

template <typename T>
inline 
Vector4<T> 
operator+( const Vector4<T> & lhs, const Vector4<T> & rhs )
{
    Vector4<T> v = lhs;
    return v += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector4<T> 
operator-( const Vector4<T> & lhs, const Vector4<T> & rhs )
{
    Vector4<T> v = lhs;
    return v -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector4<T> 
operator*( T lhs, const Vector4<T> & rhs )
{
    Vector4<T> v = rhs;
    return v *= lhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector4<T> 
operator*( const Vector4<T> & lhs, T rhs )
{
    Vector4<T> v = lhs;
    return v *= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T 
operator*( const Vector4<T> & lhs, const Vector4<T> & rhs )
{
    return (lhs[0] * rhs[0]  +  lhs[1] * rhs[1]  +  lhs[2] * rhs[2]
            +  lhs[3] * rhs[3]);
}

//=============================================================================

template <typename T>
const Vector4<T> Vector4<T>::Zero( static_cast<T>( 0 ),
                                   static_cast<T>( 0 ),
                                   static_cast<T>( 0 ),
                                   static_cast<T>( 0 ) );
template <typename T>
const Vector4<T> Vector4<T>::UnitX( static_cast<T>( 1 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 0 ) );
template <typename T>
const Vector4<T> Vector4<T>::UnitY( static_cast<T>( 0 ),
                                    static_cast<T>( 1 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 0 ) );
template <typename T>
const Vector4<T> Vector4<T>::UnitZ( static_cast<T>( 0 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 1 ),
                                    static_cast<T>( 0 ) );
template <typename T>
const Vector4<T> Vector4<T>::UnitW( static_cast<T>( 0 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 0 ),
                                    static_cast<T>( 1 ) );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Vector4<T> & vec )
{
    return out << "[ " << vec[0] << ", " << vec[1] << ", " << vec[2]
               << ", " << vec[3] << " ]";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Vector4<T> & vec )
{
    return ToJSON( vec.m_coords );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Vector4<T> * pVec )
{
    FromJSON( json, &(pVec->m_coords) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //VECTOR4_HPP

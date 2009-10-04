#ifndef VECTOR_HPP
#define VECTOR_HPP
/*
  Vector.hpp
  Copyright (C) 2009 David M. Anderson

  Vector template class: N-dimensional vector (Cartesian coordinates).
*/


#include "NullVectorException.hpp"
#include <tr1/array>
#include <cmath>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename T, int N >
class Vector
{
public:
    const T & operator[]( int index ) const;
    T & operator[]( int index );
    const T & At( int index ) const;
    T & At( int index );
    const T * Array( ) const;
    T * Array( );
    bool operator==( const Vector & rhs ) const;
    bool operator!=( const Vector & rhs ) const;
    Vector operator-( ) const;
    Vector & operator+=( const Vector & rhs );
    Vector & operator-=( const Vector & rhs );
    Vector & operator*=( T rhs );
    Vector & Negate( );
    T Length( ) const;
    T LengthSquared( ) const;
    void Normalize( T length = T() );

    static const Vector Zero;

private:
    std::tr1::array<T,N> m_coords;
};

//.............................................................................

template <typename T, int N>
Vector<T,N> operator+( const Vector<T,N> & lhs, const Vector<T,N> & rhs );
template <typename T, int N>
Vector<T,N> operator-( const Vector<T,N> & lhs, const Vector<T,N> & rhs );
template <typename T, int N>   //scalar product
Vector<T,N> operator*( T lhs, const Vector<T,N> & rhs );
template <typename T, int N>   //scalar product
Vector<T,N> operator*( const Vector<T,N> & lhs, T rhs );
template <typename T, int N>   //inner (dot) product
T operator*( const Vector<T,N> & lhs, const Vector<T,N> & rhs );

#ifdef DEBUG
bool TestVector( );
#endif


//*****************************************************************************


template < typename T, int N >
inline
const T & 
Vector<T,N>::operator[]( int index ) const
{
    return m_coords[ index ];
}

//.............................................................................

template < typename T, int N >
inline
T & 
Vector<T,N>::operator[]( int index )
{
    return m_coords[ index ];
}

//-----------------------------------------------------------------------------

template < typename T, int N >
const T & 
Vector<T,N>::At( int index ) const
{
    return m_coords.at( index );
}

//.............................................................................

template < typename T, int N >
T & 
Vector<T,N>::At( int index )
{
    return m_coords.at( index );
}

//=============================================================================

template < typename T, int N >
inline
const T * 
Vector<T,N>::Array( ) const
{
    return m_coords.data();
}

//-----------------------------------------------------------------------------

template < typename T, int N >
inline
T * 
Vector<T,N>::Array( )
{
    return m_coords.data();
}

//=============================================================================

template < typename T, int N >
bool 
Vector<T,N>::operator==( const Vector & rhs ) const
{
    for ( int i = 0; i < N; ++i )
        if ( m_coords[i] != rhs.m_coords[i] )
            return false;
    return true;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
bool 
Vector<T,N>::operator!=( const Vector & rhs ) const
{
    return ! (*this == rhs);
}

//=============================================================================

template < typename T, int N >
Vector<T,N> 
Vector<T,N>::operator-( ) const
{
    Vector<T,N> neg = *this;
    neg.Negate( );
    return neg;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
Vector<T,N> & 
Vector<T,N>::operator+=( const Vector & rhs )
{
    for ( int i = 0; i < N; ++i )
        m_coords[i] += rhs.m_coords[i];
    return *this;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
Vector<T,N> & 
Vector<T,N>::operator-=( const Vector & rhs )
{
    for ( int i = 0; i < N; ++i )
        m_coords[i] -= rhs.m_coords[i];
    return *this;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
Vector<T,N> & 
Vector<T,N>::operator*=( T rhs )
{
    for ( int i = 0; i < N; ++i )
        m_coords[i] *= rhs;
    return *this;
}

//=============================================================================

template < typename T, int N >
Vector<T,N> & 
Vector<T,N>::Negate( )
{
    for ( int i = 0; i < N; ++i )
        m_coords[i] = -m_coords[i];
    return *this;
}

//=============================================================================

template < typename T, int N >
T 
Vector<T,N>::Length( ) const
{
    return static_cast<T>( std::sqrt( LengthSquared( ) ) );
}

//-----------------------------------------------------------------------------

template < typename T, int N >
T 
Vector<T,N>::LengthSquared( ) const
{
    T lenSq = T();
    for ( int i = 0; i < N; ++i )
        lenSq += m_coords[i] * m_coords[i];
    return lenSq;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
void 
Vector<T,N>::Normalize( T length )
{
    T len = ( (length == T()) ? Length() : length );
    if ( len == T() )
        throw NullVectorException();
    T invLen = T( 1. ) / len;
    *this *= invLen;
}

//=============================================================================

template < typename T, int N >
Vector<T,N> 
operator+( const Vector<T,N> & lhs, const Vector<T,N> & rhs )
{
    Vector<T,N> v = lhs;
    return v += rhs;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
Vector<T,N> 
operator-( const Vector<T,N> & lhs, const Vector<T,N> & rhs )
{
    Vector<T,N> v = lhs;
    return v -= rhs;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
Vector<T,N> 
operator*( T lhs, const Vector<T,N> & rhs )
{
    Vector<T,N> v = rhs;
    return v *= lhs;
}

//.............................................................................

template < typename T, int N >
Vector<T,N> 
operator*( const Vector<T,N> & lhs, T rhs )
{
    Vector<T,N> v = lhs;
    return v *= rhs;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
T 
operator*( const Vector<T,N> & lhs, const Vector<T,N> & rhs )
{
    T dotProd = T();
    for ( int i = 0; i < N; ++i )
        dotProd += lhs[i] * rhs[i];
    return dotProd;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //VECTOR_HPP

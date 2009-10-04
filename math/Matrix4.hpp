#ifndef MATRIX4_HPP
#define MATRIX4_HPP
/*
  Matrix4.hpp
  Copyright (C) 2007 David M. Anderson

  Matrix4 template class: a 4x4 matrix.
  NOTES:
  1. It is assumed that T is a simple enough type that passing by value,
     rather than by reference, makes sense. It is also assumed that T values
     can be assigned simply via memcpy().
  2. Internally, elements are stored column by column (column-major), for
     efficient interaction with outside libraries (such as OpenGL). This
     should mainly affect the implementation, but not the interface,
     which maintains the more common row-major orientation, except as noted.
  3. Various forms of constructors and Set() are offered:
     (i) The default constructor does nothing, for efficiency in allocating 
        arrays, etc. The default Set() creates a zero matrix.
     (ii) The (T d) form produces a diagonal (scale) matrix, with all
        diagonal elements equal to d, and all off-diagonal elements 0.
     (iii) The (T e00, T e01, etc.) form takes a complete enumeration
        of the elements. By default, it expects elements listed row by row
        (row major). This can be overridden by setting the columnMajor
        argument to true;
     (iv) The (const T * pElements) form takes a pointer to an array listing
        all of the elements. Here, though, the default is to expect a
        column-major ordering. The expectation is that the array will 
        typically be provided by a routine from an outside library.
     (v) The (const Vector4<T> & v0, etc.) form takes vectors representing
        either the columns (the default) or rows of the matrix.
  4. operator() is effectively the indexing operator, serving the role of
     [][]. Note that the arguments are (row, column), regardless of the
     internal representation.
  5. operator() provides unchecked access.
     Element() asserts that the indices are in the proper range.
  6. The Array() methods provide direct access to the internal representation,
     primarily for efficient interaction with other libraries.
  7. If you already have the determinant, you may pass it to Inverse(), to
     save that routine some time. If the determinant is 0, Inverse() will
     throw a SingularMatrixException.
*/


#include "Array.hpp"
#include "Assert.hpp"
#include "SingularMatrixException.hpp"
#include "JSON.hpp"
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class Matrix4;
template <typename T>
class Vector4;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Matrix4<T> & mat );
template <typename T>
std::string ToJSON( const Matrix4<T> & mat );
template <typename T>
void FromJSON( const std::string & json, Matrix4<T> * pMat );


//*****************************************************************************


template <typename T>
class Matrix4
{
public:
    Matrix4( );
    explicit Matrix4( T d );
    Matrix4( T e00, T e01, T e02, T e03,
             T e10, T e11, T e12, T e13,
             T e20, T e21, T e22, T e23,
             T e30, T e31, T e32, T e33,
             bool columnMajor = false );
    Matrix4( const T * pElements, bool columnMajor = true );
    Matrix4( const Vector4<T> & v0, const Vector4<T> & v1,
             const Vector4<T> & v2, const Vector4<T> & v3,
             bool columns = true );
    void Set( T d = T() );
    void Set( T e00, T e01, T e02, T e03,
              T e10, T e11, T e12, T e13,
              T e20, T e21, T e22, T e23,
              T e30, T e31, T e32, T e33,
              bool columnMajor = false );
    void Set( const T * pElements, bool columnMajor = true );
    void Set( const Vector4<T> & v0, const Vector4<T> & v1,
              const Vector4<T> & v2, const Vector4<T> & v3,
              bool columns = true );
    const T & operator()( int row, int column ) const;
    T & operator()( int row, int column );
    const T & Element( int row, int column ) const;
    T & Element( int row, int column );
    Vector4<T> Row( int row ) const;
    Vector4<T> Column( int column ) const;
    const T * Array( ) const;
    T * Array( );
    Matrix4 Transpose( ) const;
    T Trace( ) const;
    T Determinant( ) const;
    Matrix4 Adjoint( ) const;
    Matrix4 Inverse( T determinant = T() ) const;
    Matrix4 Inverse( T determinant, const Matrix4 & adjoint ) const;
    bool operator==( const Matrix4 & rhs ) const;
    Matrix4 & operator+=( const Matrix4 & rhs );
    Matrix4 & operator-=( const Matrix4 & rhs );
    Matrix4 & operator*=( T rhs );
    Matrix4 & operator*=( const Matrix4 & rhs );
    Matrix4 operator-( ) const;
    
    static const Matrix4 Zero;
    static const Matrix4 Identity;

protected:
    array< array< T, 4 >, 4 >   m_elements; //[column][row]

    friend std::string ToJSON<>( const Matrix4 & mat );
    friend void FromJSON<>( const std::string & json, Matrix4 * pMat );
};

//.............................................................................

template <typename T>
Matrix4<T> operator+( const Matrix4<T> & lhs, const Matrix4<T> & rhs );
template <typename T>
Matrix4<T> operator-( const Matrix4<T> & lhs, const Matrix4<T> & rhs );
template <typename T>
Matrix4<T> operator*( T lhs, const Matrix4<T> & rhs );
template <typename T>
Matrix4<T> operator*( const Matrix4<T> & lhs, T rhs );
template <typename T>
Matrix4<T> operator*( const Matrix4<T> & lhs, const Matrix4<T> & rhs );

#ifdef DEBUG
bool TestMatrix4( );
#endif

//=============================================================================

typedef Matrix4<float>  Matrix4F;
typedef Matrix4<double> Matrix4D;


//*****************************************************************************


template <typename T>
Matrix4<T>::Matrix4( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix4<T>::Matrix4( T d )
{
    Set( d );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix4<T>::Matrix4( T e00, T e01, T e02, T e03,
                     T e10, T e11, T e12, T e13,
                     T e20, T e21, T e22, T e23,
                     T e30, T e31, T e32, T e33,
                     bool columnMajor )
{
    Set( e00, e01, e02, e03, e10, e11, e12, e13, e20, e21, e22, e23,
         e30, e31, e32, e33, columnMajor );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix4<T>::Matrix4( const T * pElements, bool columnMajor )
{
    Set( pElements, columnMajor );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix4<T>::Matrix4( const Vector4<T> & v0, const Vector4<T> & v1,
                     const Vector4<T> & v2, const Vector4<T> & v3,
                     bool columns )
{
    Set( v0, v1, v2, v3, columns );
}

//=============================================================================

template <typename T>
void 
Matrix4<T>::Set( T d )
{
    m_elements[0][0] = m_elements[1][1] = m_elements[2][2] = m_elements[3][3]
            = d;
    m_elements[0][1] = m_elements[0][2] = m_elements[0][3]
            = m_elements[1][0] = m_elements[1][2] = m_elements[1][3]
            = m_elements[2][0] = m_elements[2][1] = m_elements[2][3]
            = m_elements[3][0] = m_elements[3][1] = m_elements[3][2] = T();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix4<T>::Set( T e00, T e01, T e02, T e03,
                 T e10, T e11, T e12, T e13,
                 T e20, T e21, T e22, T e23,
                 T e30, T e31, T e32, T e33,
                 bool columnMajor )
{
    if ( columnMajor )
    {
        m_elements[0][0] = e00;
        m_elements[0][1] = e01;
        m_elements[0][2] = e02;
        m_elements[0][3] = e03;
        m_elements[1][0] = e10;
        m_elements[1][1] = e11;
        m_elements[1][2] = e12;
        m_elements[1][3] = e13;
        m_elements[2][0] = e20;
        m_elements[2][1] = e21;
        m_elements[2][2] = e22;
        m_elements[2][3] = e23;
        m_elements[3][0] = e30;
        m_elements[3][1] = e31;
        m_elements[3][2] = e32;
        m_elements[3][3] = e33;
    }
    else
    {
        m_elements[0][0] = e00;
        m_elements[0][1] = e10;
        m_elements[0][2] = e20;
        m_elements[0][3] = e30;
        m_elements[1][0] = e01;
        m_elements[1][1] = e11;
        m_elements[1][2] = e21;
        m_elements[1][3] = e31;
        m_elements[2][0] = e02;
        m_elements[2][1] = e12;
        m_elements[2][2] = e22;
        m_elements[2][3] = e32;
        m_elements[3][0] = e03;
        m_elements[3][1] = e13;
        m_elements[3][2] = e23;
        m_elements[3][3] = e33;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix4<T>::Set( const T * pElements, bool columnMajor )
{
    if ( columnMajor )
        memcpy( m_elements[0].c_array(), pElements, sizeof( m_elements ) );
    else
    {
        m_elements[0][0] = *pElements++;
        m_elements[1][0] = *pElements++;
        m_elements[2][0] = *pElements++;
        m_elements[3][0] = *pElements++;
        m_elements[0][1] = *pElements++;
        m_elements[1][1] = *pElements++;
        m_elements[2][1] = *pElements++;
        m_elements[3][1] = *pElements++;
        m_elements[0][2] = *pElements++;
        m_elements[1][2] = *pElements++;
        m_elements[2][2] = *pElements++;
        m_elements[3][2] = *pElements++;
        m_elements[0][3] = *pElements++;
        m_elements[1][3] = *pElements++;
        m_elements[2][3] = *pElements++;
        m_elements[3][3] = *pElements;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix4<T>::Set( const Vector4<T> & v0, const Vector4<T> & v1,
                 const Vector4<T> & v2, const Vector4<T> & v3,
                 bool columns )
{
    if ( columns )
    {
        memcpy( m_elements[0].c_array(), v0.Array(), 4 * sizeof(T) );
        memcpy( m_elements[1].c_array(), v1.Array(), 4 * sizeof(T) );
        memcpy( m_elements[2].c_array(), v2.Array(), 4 * sizeof(T) );
        memcpy( m_elements[3].c_array(), v3.Array(), 4 * sizeof(T) );
    }
    else
    {
        Set( v0[0], v0[1], v0[2], v0[3], v1[0], v1[1], v1[2], v1[3],
             v2[0], v2[1], v2[2], v2[3], v3[0], v3[1], v3[2], v3[3] );
    }
}

//=============================================================================

template <typename T>
const T & 
Matrix4<T>::operator()( int row, int column ) const
{
    return m_elements[column][row];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
T & 
Matrix4<T>::operator()( int row, int column )
{
    return m_elements[column][row];
}

//=============================================================================

template <typename T>
const T & 
Matrix4<T>::Element( int row, int column ) const
{
    Assert( (0 <= row) && (row < 4) );
    Assert( (0 <= column) && (column < 4) );
    return m_elements[column][row];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
T & 
Matrix4<T>::Element( int row, int column )
{
    Assert( (0 <= row) && (row < 4) );
    Assert( (0 <= column) && (column < 4) );
    return m_elements[column][row];
}

//=============================================================================

template <typename T>
Vector4<T>
Matrix4<T>::Row( int row ) const
{
    return Vector4<T>( m_elements[0][row], m_elements[1][row],
                       m_elements[2][row], m_elements[3][row] );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector4<T>
Matrix4<T>::Column( int column ) const
{
    return Vector4<T>( m_elements[column].data() );
}

//=============================================================================

template <typename T>
const T * 
Matrix4<T>::Array( ) const
{
    return reinterpret_cast<const T *>( m_elements.data() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
T * 
Matrix4<T>::Array( )
{
    return reinterpret_cast<T *>( m_elements.c_array() );
}

//=============================================================================

template <typename T>
Matrix4<T> 
Matrix4<T>::Transpose( ) const
{
    return Matrix4( m_elements[0][0], m_elements[1][0],
                    m_elements[2][0], m_elements[3][0],
                    m_elements[0][1], m_elements[1][1],
                    m_elements[2][1], m_elements[3][1],
                    m_elements[0][2], m_elements[1][2],
                    m_elements[2][2], m_elements[3][2],
                    m_elements[0][3], m_elements[1][3],
                    m_elements[2][3], m_elements[3][3],
                    true );
}

//=============================================================================

template <typename T>
T 
Matrix4<T>::Trace( ) const
{
    return (m_elements[0][0] + m_elements[1][1] + m_elements[2][2]
            + m_elements[3][3]);
}

//-----------------------------------------------------------------------------

template <typename T>
T 
Matrix4<T>::Determinant( ) const
{
    return  ( ((m_elements[0][0] * m_elements[1][1]
                -  m_elements[0][1] * m_elements[1][0])
               * (m_elements[2][2] * m_elements[3][3]
                  -  m_elements[2][3] * m_elements[3][2]))
              - ((m_elements[0][0] * m_elements[1][2]
                -  m_elements[0][2] * m_elements[1][0])
               * (m_elements[2][1] * m_elements[3][3]
                  -  m_elements[2][3] * m_elements[3][1]))
              + ((m_elements[0][0] * m_elements[1][3]
                -  m_elements[0][3] * m_elements[1][0])
               * (m_elements[2][1] * m_elements[3][2]
                  -  m_elements[2][2] * m_elements[3][1]))
              + ((m_elements[0][1] * m_elements[1][2]
                -  m_elements[0][2] * m_elements[1][1])
               * (m_elements[2][0] * m_elements[3][3]
                  -  m_elements[2][3] * m_elements[3][0]))
              - ((m_elements[0][1] * m_elements[1][3]
                -  m_elements[0][3] * m_elements[1][1])
               * (m_elements[2][0] * m_elements[3][2]
                  -  m_elements[2][2] * m_elements[3][0]))
              + ((m_elements[0][2] * m_elements[1][3]
                -  m_elements[0][3] * m_elements[1][2])
               * (m_elements[2][0] * m_elements[3][1]
                  -  m_elements[2][1] * m_elements[3][0])) );
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T>
Matrix4<T>::Adjoint( ) const
{
    Matrix4 adjoint;
    static const int coIndices[4][3]
            = { { 1, 2, 3 }, { 0, 2, 3 }, { 0, 1, 3 }, { 0, 1, 2 } };
    for ( int i = 0; i < 4; ++i )
    {
        int i0 = coIndices[i][0];
        int i1 = coIndices[i][1];
        int i2 = coIndices[i][2];
        for ( int j = 0; j < 4; ++j )
        {
            int j0 = coIndices[j][0];
            int j1 = coIndices[j][1];
            int j2 = coIndices[j][2];
            T sign = static_cast<T>( (((i + j) & 1) == 0)  ?  1.  :  -1. );
            adjoint( i, j ) = sign
                    * (m_elements[i0][j0]
                       * (m_elements[i1][j1] * m_elements[i2][j2]
                          -  m_elements[i1][j2] * m_elements[i2][j1])
                       -  m_elements[i0][j1]
                       * (m_elements[i1][j0] * m_elements[i2][j2]
                          -  m_elements[i1][j2] * m_elements[i2][j0])
                       +  m_elements[i0][j2]
                       * (m_elements[i1][j0] * m_elements[i2][j1]
                          -  m_elements[i1][j1] * m_elements[i2][j0]));
        }
    }
    return adjoint;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> 
Matrix4<T>::Inverse( T determinant ) const
{
    //LU decomposition would be better.
    T det = ( (determinant == T()) ? Determinant( ) : determinant );
    return  Inverse( det, Adjoint( ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix4<T>
Matrix4<T>::Inverse( T determinant, const Matrix4 & adjoint ) const
{
    if ( determinant == T() )
        throw SingularMatrixException();
    return  adjoint * static_cast<T>( 1. / determinant );
}

//=============================================================================

template <typename T>
bool 
Matrix4<T>::operator==( const Matrix4 & rhs ) const
{
    return ( (m_elements[0][0] == rhs.m_elements[0][0])
             && (m_elements[0][1] == rhs.m_elements[0][1])
             && (m_elements[0][2] == rhs.m_elements[0][2])
             && (m_elements[0][3] == rhs.m_elements[0][3])
             && (m_elements[1][0] == rhs.m_elements[1][0])
             && (m_elements[1][1] == rhs.m_elements[1][1])
             && (m_elements[1][2] == rhs.m_elements[1][2])
             && (m_elements[1][3] == rhs.m_elements[1][3])
             && (m_elements[2][0] == rhs.m_elements[2][0])
             && (m_elements[2][1] == rhs.m_elements[2][1])
             && (m_elements[2][2] == rhs.m_elements[2][2])
             && (m_elements[2][3] == rhs.m_elements[2][3])
             && (m_elements[3][0] == rhs.m_elements[3][0])
             && (m_elements[3][1] == rhs.m_elements[3][1])
             && (m_elements[3][2] == rhs.m_elements[3][2])
             && (m_elements[3][3] == rhs.m_elements[3][3]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> & 
Matrix4<T>::operator+=( const Matrix4 & rhs )
{
    m_elements[0][0] += rhs.m_elements[0][0];
    m_elements[0][1] += rhs.m_elements[0][1];
    m_elements[0][2] += rhs.m_elements[0][2];
    m_elements[0][3] += rhs.m_elements[0][3];
    m_elements[1][0] += rhs.m_elements[1][0];
    m_elements[1][1] += rhs.m_elements[1][1];
    m_elements[1][2] += rhs.m_elements[1][2];
    m_elements[1][3] += rhs.m_elements[1][3];
    m_elements[2][0] += rhs.m_elements[2][0];
    m_elements[2][1] += rhs.m_elements[2][1];
    m_elements[2][2] += rhs.m_elements[2][2];
    m_elements[2][3] += rhs.m_elements[2][3];
    m_elements[3][0] += rhs.m_elements[3][0];
    m_elements[3][1] += rhs.m_elements[3][1];
    m_elements[3][2] += rhs.m_elements[3][2];
    m_elements[3][3] += rhs.m_elements[3][3];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> & 
Matrix4<T>::operator-=( const Matrix4 & rhs )
{
    m_elements[0][0] -= rhs.m_elements[0][0];
    m_elements[0][1] -= rhs.m_elements[0][1];
    m_elements[0][2] -= rhs.m_elements[0][2];
    m_elements[0][3] -= rhs.m_elements[0][3];
    m_elements[1][0] -= rhs.m_elements[1][0];
    m_elements[1][1] -= rhs.m_elements[1][1];
    m_elements[1][2] -= rhs.m_elements[1][2];
    m_elements[1][3] -= rhs.m_elements[1][3];
    m_elements[2][0] -= rhs.m_elements[2][0];
    m_elements[2][1] -= rhs.m_elements[2][1];
    m_elements[2][2] -= rhs.m_elements[2][2];
    m_elements[2][3] -= rhs.m_elements[2][3];
    m_elements[3][0] -= rhs.m_elements[3][0];
    m_elements[3][1] -= rhs.m_elements[3][1];
    m_elements[3][2] -= rhs.m_elements[3][2];
    m_elements[3][3] -= rhs.m_elements[3][3];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> & 
Matrix4<T>::operator*=( T rhs )
{
    m_elements[0][0] *= rhs;
    m_elements[0][1] *= rhs;
    m_elements[0][2] *= rhs;
    m_elements[0][3] *= rhs;
    m_elements[1][0] *= rhs;
    m_elements[1][1] *= rhs;
    m_elements[1][2] *= rhs;
    m_elements[1][3] *= rhs;
    m_elements[2][0] *= rhs;
    m_elements[2][1] *= rhs;
    m_elements[2][2] *= rhs;
    m_elements[2][3] *= rhs;
    m_elements[3][0] *= rhs;
    m_elements[3][1] *= rhs;
    m_elements[3][2] *= rhs;
    m_elements[3][3] *= rhs;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> & 
Matrix4<T>::operator*=( const Matrix4 & rhs )
{
    T m00 = m_elements[0][0];
    T m10 = m_elements[1][0];
    T m20 = m_elements[2][0];
    m_elements[0][0] = m00 * rhs.m_elements[0][0]
        +  m10 * rhs.m_elements[0][1]
        +  m20 * rhs.m_elements[0][2]
        +  m_elements[3][0] * rhs.m_elements[0][3];
    m_elements[1][0] = m00 * rhs.m_elements[1][0]
        +  m10 * rhs.m_elements[1][1]
        +  m20 * rhs.m_elements[1][2]
        +  m_elements[3][0] * rhs.m_elements[1][3];
    m_elements[2][0] = m00 * rhs.m_elements[2][0]
        +  m10 * rhs.m_elements[2][1]
        +  m20 * rhs.m_elements[2][2]
        +  m_elements[3][0] * rhs.m_elements[2][3];
    m_elements[3][0] = m00 * rhs.m_elements[3][0]
        +  m10 * rhs.m_elements[3][1]
        +  m20 * rhs.m_elements[3][2]
        +  m_elements[3][0] * rhs.m_elements[3][3];
    T m01 = m_elements[0][1];
    T m11 = m_elements[1][1];
    T m21 = m_elements[2][1];
    m_elements[0][1] = m01 * rhs.m_elements[0][0]
        +  m11 * rhs.m_elements[0][1]
        +  m21 * rhs.m_elements[0][2]
        +  m_elements[3][1] * rhs.m_elements[0][3];
    m_elements[1][1] = m01 * rhs.m_elements[1][0]
        +  m11 * rhs.m_elements[1][1]
        +  m21 * rhs.m_elements[1][2]
        +  m_elements[3][1] * rhs.m_elements[1][3];
    m_elements[2][1] = m01 * rhs.m_elements[2][0]
        +  m11 * rhs.m_elements[2][1]
        +  m21 * rhs.m_elements[2][2]
        +  m_elements[3][1] * rhs.m_elements[2][3];
    m_elements[3][1] = m01 * rhs.m_elements[3][0]
        +  m11 * rhs.m_elements[3][1]
        +  m21 * rhs.m_elements[3][2]
        +  m_elements[3][1] * rhs.m_elements[3][3];
    T m02 = m_elements[0][2];
    T m12 = m_elements[1][2];
    T m22 = m_elements[2][2];
    m_elements[0][2] = m02 * rhs.m_elements[0][0]
        +  m12 * rhs.m_elements[0][1]
        +  m22 * rhs.m_elements[0][2]
        +  m_elements[3][2] * rhs.m_elements[0][3];
    m_elements[1][2] = m02 * rhs.m_elements[1][0]
        +  m12 * rhs.m_elements[1][1]
        +  m22 * rhs.m_elements[1][2]
        +  m_elements[3][2] * rhs.m_elements[1][3];
    m_elements[2][2] = m02 * rhs.m_elements[2][0]
        +  m12 * rhs.m_elements[2][1]
        +  m22 * rhs.m_elements[2][2]
        +  m_elements[3][2] * rhs.m_elements[2][3];
    m_elements[3][2] = m02 * rhs.m_elements[3][0]
        +  m12 * rhs.m_elements[3][1]
        +  m22 * rhs.m_elements[3][2]
        +  m_elements[3][2] * rhs.m_elements[3][3];
    T m03 = m_elements[0][3];
    T m13 = m_elements[1][3];
    T m23 = m_elements[2][3];
    m_elements[0][3] = m03 * rhs.m_elements[0][0]
        +  m13 * rhs.m_elements[0][1]
        +  m23 * rhs.m_elements[0][2]
        +  m_elements[3][3] * rhs.m_elements[0][3];
    m_elements[1][3] = m03 * rhs.m_elements[1][0]
        +  m13 * rhs.m_elements[1][1]
        +  m23 * rhs.m_elements[1][2]
        +  m_elements[3][3] * rhs.m_elements[1][3];
    m_elements[2][3] = m03 * rhs.m_elements[2][0]
        +  m13 * rhs.m_elements[2][1]
        +  m23 * rhs.m_elements[2][2]
        +  m_elements[3][3] * rhs.m_elements[2][3];
    m_elements[3][3] = m03 * rhs.m_elements[3][0]
        +  m13 * rhs.m_elements[3][1]
        +  m23 * rhs.m_elements[3][2]
        +  m_elements[3][3] * rhs.m_elements[3][3];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> 
Matrix4<T>::operator-( ) const
{
    return Matrix4( - m_elements[0][0], - m_elements[0][1],
                    - m_elements[0][2], - m_elements[0][3],
                    - m_elements[1][0], - m_elements[1][1],
                    - m_elements[1][2], - m_elements[1][3],
                    - m_elements[2][0], - m_elements[2][1],
                    - m_elements[2][2], - m_elements[2][3],
                    - m_elements[3][0], - m_elements[3][1],
                    - m_elements[3][2], - m_elements[3][3],
                    true );
}

//=============================================================================

template <typename T>
Matrix4<T> 
operator+( const Matrix4<T> & lhs, const Matrix4<T> & rhs )
{
    Matrix4<T> m = lhs;
    return m += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> 
operator-( const Matrix4<T> & lhs, const Matrix4<T> & rhs )
{
    Matrix4<T> m = lhs;
    return m -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> 
operator*( T lhs, const Matrix4<T> & rhs )
{
    Matrix4<T> m = rhs;
    return m *= lhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> 
operator*( const Matrix4<T> & lhs, T rhs )
{
    Matrix4<T> m = lhs;
    return m *= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix4<T> 
operator*( const Matrix4<T> & lhs, const Matrix4<T> & rhs )
{
    Matrix4<T> m = lhs;
    return m *= rhs;
}

//=============================================================================

template <typename T>
const Matrix4<T> Matrix4<T>::Zero( 0. );
template <typename T>
const Matrix4<T> Matrix4<T>::Identity( 1. );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Matrix4<T> & mat )
{
    return out << "[ [ " << mat( 0, 0 ) << ", " << mat( 0, 1 )
               << ", " << mat( 0, 2 ) << ", " << mat( 0, 3 ) << " ], "
               << "[ " << mat( 1, 0 ) << ", " << mat( 1, 1 )
               << ", " << mat( 1, 2 ) << ", " << mat( 1, 3 ) << " ], "
               << "[ " << mat( 2, 0 ) << ", " << mat( 2, 1 )
               << ", " << mat( 2, 2 ) << ", " << mat( 2, 3 ) << " ], "
               << "[ " << mat( 3, 0 ) << ", " << mat( 3, 1 )
               << ", " << mat( 3, 2 ) << ", " << mat( 3, 3 ) << " ] ]";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Matrix4<T> & mat )
{
    return ToJSON( mat.m_elements );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Matrix4<T> * pMat )
{
    FromJSON( json, &(pMat->m_elements) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MATRIX4_HPP

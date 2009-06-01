#ifndef MATRIX2_HPP
#define MATRIX2_HPP
/*
  Matrix2.hpp
  Copyright (C) 2007 David M. Anderson

  Matrix2 template class: a 2x2 matrix.
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
     (v) The (const Vector2<T> & v0, etc.) form takes vectors representing
        either the columns (the default) or rows of the matrix.
     (vi) The (const Angle & angle) form produces a coordinate transformation
        matrix corresponding to rotation by the specified angle.
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
#include "Vector2.hpp"
#include "Angle.hpp"
#include "JSON.hpp"
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

template <typename T>
class Matrix2;
template <typename T>
class Vector2;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Matrix2<T> & mat );
template <typename T>
std::string ToJSON( const Matrix2<T> & mat );
template <typename T>
void FromJSON( const std::string & json, Matrix2<T> * pMat );

//=============================================================================


template <typename T>
class Matrix2
{
public:
    Matrix2( );
    explicit Matrix2( T d );
    Matrix2( T e00, T e01, T e10, T e11, bool columnMajor = false );
    Matrix2( const T * pElements, bool columnMajor = true );
    Matrix2( const Vector2<T> & v0, const Vector2<T> & v1,
             bool columns = true );
    explicit Matrix2( const Angle & angle );
    void Set( T d = T() );
    void Set( T e00, T e01, T e10, T e11, bool columnMajor = false );
    void Set( const T * pElements, bool columnMajor = true );
    void Set( const Vector2<T> & v0, const Vector2<T> & v1,
             bool columns = true );
    void Set( const Angle & angle );
    const T & operator()( int row, int column ) const;
    T & operator()( int row, int column );
    const T & Element( int row, int column ) const;
    T & Element( int row, int column );
    Vector2<T> Row( int row ) const;
    Vector2<T> Column( int column ) const;
    const T * Array( ) const;
    T * Array( );
    Matrix2 Transpose( ) const;
    T Trace( ) const;
    T Determinant( ) const;
    Matrix2 Adjoint( ) const;
    Matrix2 Inverse( T determinant = T() ) const;
    Matrix2 Inverse( T determinant, const Matrix2 & adjoint ) const;
    bool operator==( const Matrix2 & rhs ) const;
    Matrix2 & operator+=( const Matrix2 & rhs );
    Matrix2 & operator-=( const Matrix2 & rhs );
    Matrix2 & operator*=( T rhs );
    Matrix2 & operator*=( const Matrix2 & rhs );
    Matrix2 operator-( ) const;

    static const Matrix2 Zero;
    static const Matrix2 Identity;

protected:
    array< array< T, 2 >, 2 >   m_elements; //[column][row]

    friend std::string ToJSON<>( const Matrix2 & mat );
    friend void FromJSON<>( const std::string & json, Matrix2 * pMat );
};

//.............................................................................

template <typename T>
Matrix2<T> operator+( const Matrix2<T> & lhs, const Matrix2<T> & rhs );
template <typename T>
Matrix2<T> operator-( const Matrix2<T> & lhs, const Matrix2<T> & rhs );
template <typename T>
Matrix2<T> operator*( T lhs, const Matrix2<T> & rhs );
template <typename T>
Matrix2<T> operator*( const Matrix2<T> & lhs, T rhs );
template <typename T>
Matrix2<T> operator*( const Matrix2<T> & lhs, const Matrix2<T> & rhs );

#ifdef DEBUG
bool TestMatrix2( );
#endif

//=============================================================================


typedef Matrix2<float>  Matrix2F;
typedef Matrix2<double> Matrix2D;


//*****************************************************************************


template <typename T>
inline 
Matrix2<T>::Matrix2( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Matrix2<T>::Matrix2( T d )
{
    Set( d );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix2<T>::Matrix2( T e00, T e01, T e10, T e11, bool columnMajor )
{
    Set( e00, e01, e10, e11, columnMajor );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix2<T>::Matrix2( const T * pElements, bool columnMajor )
{
    Set( pElements, columnMajor );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix2<T>::Matrix2( const Vector2<T> & v0, const Vector2<T> & v1,
                     bool columns )
{
    Set( v0, v1, columns );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix2<T>::Matrix2( const Angle & angle )
{
    Set( angle );
}

//=============================================================================

template <typename T>
inline 
void 
Matrix2<T>::Set( T d )
{
    m_elements[0][0] = m_elements[1][1] = d;
    m_elements[0][1] = m_elements[1][0] = T();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix2<T>::Set( T e00, T e01, T e10, T e11, bool columnMajor )
{
    if ( columnMajor )
    {
        m_elements[0][0] = e00;
        m_elements[0][1] = e01;
        m_elements[1][0] = e10;
        m_elements[1][1] = e11;
    }
    else
    {
        m_elements[0][0] = e00;
        m_elements[0][1] = e10;
        m_elements[1][0] = e01;
        m_elements[1][1] = e11;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix2<T>::Set( const T * pElements, bool columnMajor )
{
    if ( columnMajor )
        memcpy( m_elements[0].c_array(), pElements, sizeof( m_elements ) );
    else
    {
        m_elements[0][0] = *pElements++;
        m_elements[1][0] = *pElements++;
        m_elements[0][1] = *pElements++;
        m_elements[1][1] = *pElements;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix2<T>::Set( const Vector2<T> & v0, const Vector2<T> & v1,
                 bool columns )
{
    if ( columns )
    {
        memcpy( m_elements[0].c_array(), v0.Array(), 2 * sizeof(T) );
        memcpy( m_elements[1].c_array(), v1.Array(), 2 * sizeof(T) );
    }
    else
    {
        Set( v0[0], v0[1], v1[0], v1[1] );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix2<T>::Set( const Angle & angle )
{
    T c = static_cast<T>( angle.Cos( ) );
    T s = static_cast<T>( angle.Sin( ) );
    m_elements[0][0] = m_elements[1][1] = c;
    m_elements[1][0] = -s;
    m_elements[0][1] = s;
}

//=============================================================================

template <typename T>
inline 
const T & 
Matrix2<T>::operator()( int row, int column ) const
{
    return m_elements[column][row];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
T & 
Matrix2<T>::operator()( int row, int column )
{
    return m_elements[column][row];
}

//=============================================================================

template <typename T>
inline 
const T & 
Matrix2<T>::Element( int row, int column ) const
{
    Assert( (0 <= row) && (row < 2) );
    Assert( (0 <= column) && (column < 2) );
    return m_elements[column][row];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
T & 
Matrix2<T>::Element( int row, int column )
{
    Assert( (0 <= row) && (row < 2) );
    Assert( (0 <= column) && (column < 2) );
    return m_elements[column][row];
}

//=============================================================================

template <typename T>
Vector2<T>
Matrix2<T>::Row( int row ) const
{
    return Vector2<T>( m_elements[0][row], m_elements[1][row] );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector2<T>
Matrix2<T>::Column( int column ) const
{
    return Vector2<T>( m_elements[column].data() );
}

//=============================================================================

template <typename T>
inline 
const T * 
Matrix2<T>::Array( ) const
{
    return m_elements[0].c_array();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
T * 
Matrix2<T>::Array( )
{
    return m_elements[0].c_array();
}

//=============================================================================

template <typename T>
Matrix2<T> 
Matrix2<T>::Transpose( ) const
{
    return Matrix2( m_elements[0][0], m_elements[1][0],
                    m_elements[0][1], m_elements[1][1],
                    true );
}

//=============================================================================

template <typename T>
T
Matrix2<T>::Trace( ) const
{
    return  (m_elements[0][0] + m_elements[1][1]);
}

//-----------------------------------------------------------------------------

template <typename T>
T 
Matrix2<T>::Determinant( ) const
{
    return (m_elements[0][0] * m_elements[1][1]
            -  m_elements[0][1] * m_elements[1][0]);
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> 
Matrix2<T>::Adjoint( ) const
{
    return  Matrix2( m_elements[1][1],  -m_elements[0][1],
                     - m_elements[1][0],  m_elements[0][0], true );
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> 
Matrix2<T>::Inverse( T determinant ) const
{
    T det = ( (determinant == T()) ? Determinant( ) : determinant );
    return  Inverse( det, Adjoint( ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix2<T>
Matrix2<T>::Inverse( T determinant, const Matrix2 & adjoint ) const
{
    if ( determinant == T() )
        throw SingularMatrixException();
    return  adjoint * static_cast<T>( 1. / determinant );
}

//=============================================================================

template <typename T>
bool 
Matrix2<T>::operator==( const Matrix2 & rhs ) const
{
    return ( (m_elements[0][0] == rhs.m_elements[0][0])
             && (m_elements[0][1] == rhs.m_elements[0][1])
             && (m_elements[1][0] == rhs.m_elements[1][0])
             && (m_elements[1][1] == rhs.m_elements[1][1]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> & 
Matrix2<T>::operator+=( const Matrix2 & rhs )
{
    m_elements[0][0] += rhs.m_elements[0][0];
    m_elements[0][1] += rhs.m_elements[0][1];
    m_elements[1][0] += rhs.m_elements[1][0];
    m_elements[1][1] += rhs.m_elements[1][1];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> & 
Matrix2<T>::operator-=( const Matrix2 & rhs )
{
    m_elements[0][0] -= rhs.m_elements[0][0];
    m_elements[0][1] -= rhs.m_elements[0][1];
    m_elements[1][0] -= rhs.m_elements[1][0];
    m_elements[1][1] -= rhs.m_elements[1][1];
    return *this;
}


//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> & 
Matrix2<T>::operator*=( T rhs )
{
    m_elements[0][0] *= rhs;
    m_elements[0][1] *= rhs;
    m_elements[1][0] *= rhs;
    m_elements[1][1] *= rhs;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> & 
Matrix2<T>::operator*=( const Matrix2 & rhs )
{
    T m00 = m_elements[0][0];
    m_elements[0][0] = m00 * rhs.m_elements[0][0]
        +  m_elements[1][0] * rhs.m_elements[0][1];
    m_elements[1][0] = m00 * rhs.m_elements[1][0]
        +  m_elements[1][0] * rhs.m_elements[1][1];
    T m01 = m_elements[0][1];
    m_elements[0][1] = m01 * rhs.m_elements[0][0]
        +  m_elements[1][1] * rhs.m_elements[0][1];
    m_elements[1][1] = m01 * rhs.m_elements[1][0]
        +  m_elements[1][1] * rhs.m_elements[1][1];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> 
Matrix2<T>::operator-( ) const
{
    return Matrix2( - m_elements[0][0], - m_elements[0][1],
                    - m_elements[1][0], - m_elements[1][1],
                    true );
}

//=============================================================================

template <typename T>
Matrix2<T> 
operator+( const Matrix2<T> & lhs, const Matrix2<T> & rhs )
{
    Matrix2<T> m = lhs;
    return m += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> 
operator-( const Matrix2<T> & lhs, const Matrix2<T> & rhs )
{
    Matrix2<T> m = lhs;
    return m -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> 
operator*( T lhs, const Matrix2<T> & rhs )
{
    Matrix2<T> m = rhs;
    return m *= lhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> 
operator*( const Matrix2<T> & lhs, T rhs )
{
    Matrix2<T> m = lhs;
    return m *= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix2<T> 
operator*( const Matrix2<T> & lhs, const Matrix2<T> & rhs )
{
    Matrix2<T> m = lhs;
    return m *= rhs;
}

//=============================================================================

template <typename T>
const Matrix2<T> Matrix2<T>::Zero( static_cast<T>( 0 ) );
template <typename T>
const Matrix2<T> Matrix2<T>::Identity( static_cast<T>( 1 ) );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Matrix2<T> & mat )
{
    return out << "[ [ " << mat( 0, 0 ) << ", " << mat( 0, 1 ) << " ], "
               << "[ " << mat( 1, 0 ) << ", " << mat( 1, 1 ) << " ] ]";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Matrix2<T> & mat )
{
    return ToJSON( mat.m_elements );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Matrix2<T> * pMat )
{
    FromJSON( json, &(pMat->m_elements) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MATRIX2_HPP

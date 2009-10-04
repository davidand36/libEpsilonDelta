#ifndef MATRIX3_HPP
#define MATRIX3_HPP
/*
  Matrix3.hpp
  Copyright (C) 2007 David M. Anderson

  Matrix3 template class: a 3x3 matrix.
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
     (v) The (const Vector3<T> & v0, etc.) form takes vectors representing
        either the columns (the default) or rows of the matrix.
     (vi) The (const AxisAngle<T> & axisAngle) form produces
        a coordinate transformation matrix corresponding to rotation by the
        specified angle around the specified axis. The axis vector must
        be unit-length, i.e. normalized.
     (vii) The (int axis, const Angle & angle) form is the special case of
        the last for the three coordinate axes.
     (viii) The ( const EulerAngles & eulerAngles, EulerAngles::EOrder order )
        form produces the result of rotations around each of the three
        coordinate axes in the specified order.
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


#include "Assert.hpp"
#include "SingularMatrixException.hpp"
#include "EulerAngles.hpp"
#include "JSON.hpp"
#include <tr1/array>
#include <iostream>
#include <cstring>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class Matrix3;
template <typename T>
class Vector3;
template <typename T>
class AxisAngle;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Matrix3<T> & mat );
template <typename T>
std::string ToJSON( const Matrix3<T> & mat );
template <typename T>
void FromJSON( const std::string & json, Matrix3<T> * pMat );


//*****************************************************************************


template <typename T>
class Matrix3
{
public:
    Matrix3( );
    explicit Matrix3( T d );
    Matrix3( T e00, T e01, T e02, T e10, T e11, T e12, T e20, T e21, T e22,
             bool columnMajor = false );
    Matrix3( const T * pElements, bool columnMajor = true );
    Matrix3( const Vector3<T> & v0, const Vector3<T> & v1,
             const Vector3<T> & v2, bool columns = true );
    Matrix3( const AxisAngle<T> & axisAngle );
    Matrix3( int axis, const Angle & angle );
    Matrix3( const EulerAngles & eulerAngles, EulerAngles::EOrder order );
    void Set( T d = T() );
    void Set( T e00, T e01, T e02, T e10, T e11, T e12, T e20, T e21, T e22,
              bool columnMajor = false );
    void Set( const T * pElements, bool columnMajor = true );
    void Set( const Vector3<T> & v0, const Vector3<T> & v1,
              const Vector3<T> & v2, bool columns = true );
    void Set( const AxisAngle<T> & axisAngle );
    void Set( int axis, const Angle & angle );
    void Set( const EulerAngles & eulerAngles, EulerAngles::EOrder order );
    const T & operator()( int row, int column ) const;
    T & operator()( int row, int column );
    const T & Element( int row, int column ) const;
    T & Element( int row, int column );
    Vector3<T> Row( int row ) const;
    Vector3<T> Column( int column ) const;
    const T * Array( ) const;
    T * Array( );
    Matrix3 Transpose( ) const;
    T Trace( ) const;
    T Determinant( ) const;
    Matrix3 Adjoint( ) const;
    Matrix3 Inverse( T determinant = T() ) const;
    Matrix3 Inverse( T determinant, const Matrix3 & adjoint ) const;
    bool operator==( const Matrix3 & rhs ) const;
    Matrix3 & operator+=( const Matrix3 & rhs );
    Matrix3 & operator-=( const Matrix3 & rhs );
    Matrix3 & operator*=( T rhs );
    Matrix3 & operator*=( const Matrix3 & rhs );
    Matrix3 operator-( ) const;
    
    static const Matrix3 Zero;
    static const Matrix3 Identity;

protected:
    std::tr1::array< std::tr1::array< T, 3 >, 3 >   m_elements; //[column][row]

    friend std::string ToJSON<>( const Matrix3 & mat );
    friend void FromJSON<>( const std::string & json, Matrix3 * pMat );
};

//.............................................................................

template <typename T>
Matrix3<T> operator+( const Matrix3<T> & lhs, const Matrix3<T> & rhs );
template <typename T>
Matrix3<T> operator-( const Matrix3<T> & lhs, const Matrix3<T> & rhs );
template <typename T>
Matrix3<T> operator*( T lhs, const Matrix3<T> & rhs );
template <typename T>
Matrix3<T> operator*( const Matrix3<T> & lhs, T rhs );
template <typename T>
Matrix3<T> operator*( const Matrix3<T> & lhs, const Matrix3<T> & rhs );

#ifdef DEBUG
bool TestMatrix3( );
#endif

//=============================================================================

typedef Matrix3<float>  Matrix3F;
typedef Matrix3<double> Matrix3D;


//*****************************************************************************


template <typename T>
Matrix3<T>::Matrix3( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>::Matrix3( T d )
{
    Set( d );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>::Matrix3( T e00, T e01, T e02, T e10, T e11, T e12,
                     T e20, T e21, T e22,
                     bool columnMajor )
{
    Set( e00, e01, e02, e10, e11, e12, e20, e21, e22, columnMajor );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>::Matrix3( const T * pElements, bool columnMajor )
{
    Set( pElements, columnMajor );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>::Matrix3( const Vector3<T> & v0, const Vector3<T> & v1,
                     const Vector3<T> & v2, bool columns )
{
    Set( v0, v1, v2, columns );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>::Matrix3( const AxisAngle<T> & axisAngle )
{
    Set( axisAngle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>::Matrix3( int axis, const Angle & angle )
{
    Set( axis, angle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>::Matrix3( const EulerAngles & eulerAngles,
                     EulerAngles::EOrder order )
{
    Set( eulerAngles, order );
}

//=============================================================================

template <typename T>
void 
Matrix3<T>::Set( T d )
{
    m_elements[0][0] = m_elements[1][1] = m_elements[2][2] = d;
    m_elements[0][1] = m_elements[0][2] = m_elements[1][0] = m_elements[1][2]
        = m_elements[2][0] = m_elements[2][1] = T();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix3<T>::Set( T e00, T e01, T e02, T e10, T e11, T e12,
                 T e20, T e21, T e22,
                 bool columnMajor )
{
    if ( columnMajor )
    {
        m_elements[0][0] = e00;
        m_elements[0][1] = e01;
        m_elements[0][2] = e02;
        m_elements[1][0] = e10;
        m_elements[1][1] = e11;
        m_elements[1][2] = e12;
        m_elements[2][0] = e20;
        m_elements[2][1] = e21;
        m_elements[2][2] = e22;
    }
    else
    {
        m_elements[0][0] = e00;
        m_elements[0][1] = e10;
        m_elements[0][2] = e20;
        m_elements[1][0] = e01;
        m_elements[1][1] = e11;
        m_elements[1][2] = e21;
        m_elements[2][0] = e02;
        m_elements[2][1] = e12;
        m_elements[2][2] = e22;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix3<T>::Set( const T * pElements, bool columnMajor )
{
    if ( columnMajor )
        memcpy( m_elements[0].data(), pElements, sizeof( m_elements ) );
    else
    {
        m_elements[0][0] = *pElements++;
        m_elements[1][0] = *pElements++;
        m_elements[2][0] = *pElements++;
        m_elements[0][1] = *pElements++;
        m_elements[1][1] = *pElements++;
        m_elements[2][1] = *pElements++;
        m_elements[0][2] = *pElements++;
        m_elements[1][2] = *pElements++;
        m_elements[2][2] = *pElements;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix3<T>::Set( const Vector3<T> & v0, const Vector3<T> & v1,
                 const Vector3<T> & v2, bool columns )
{
    if ( columns )
    {
        memcpy( m_elements[0].data(), v0.Array(), 3 * sizeof(T) );
        memcpy( m_elements[1].data(), v1.Array(), 3 * sizeof(T) );
        memcpy( m_elements[2].data(), v2.Array(), 3 * sizeof(T) );
    }
    else
    {
        Set( v0[0], v0[1], v0[2], v1[0], v1[1], v1[2], v2[0], v2[1], v2[2] );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix3<T>::Set( const AxisAngle<T> & axisAngle )
{
    Angle angle = axisAngle.GetAngle( );
    const Vector3<T> & axis = axisAngle.Axis( );
    const T cc = static_cast<T>( 1. - angle.Cos( ) );
    const T s = static_cast<T>( angle.Sin( ) );
    const T a00 = axis[0] * axis[0];
    const T a01 = axis[0] * axis[1];
    const T a02 = axis[0] * axis[2];
    const T a11 = axis[1] * axis[1];
    const T a12 = axis[1] * axis[2];
    const T a22 = axis[2] * axis[2];
    m_elements[0][0] = static_cast<T>( 1. - cc * (a11 + a22) );
    m_elements[1][0] = static_cast<T>( -s * axis[2] + cc * a01  );
    m_elements[2][0] = static_cast<T>(  s * axis[1] + cc * a02  );
    m_elements[0][1] = static_cast<T>(  s * axis[2] + cc * a01 );
    m_elements[1][1] = static_cast<T>( 1. - cc * (a00 + a22) );
    m_elements[2][1] = static_cast<T>( -s * axis[0] + cc * a12 );
    m_elements[0][2] = static_cast<T>( -s * axis[1] + cc * a02 );
    m_elements[1][2] = static_cast<T>(  s * axis[0] + cc * a12 );
    m_elements[2][2] = static_cast<T>( 1. - cc * (a00 + a11) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix3<T>::Set( int axis, const Angle & angle )
{
    Assert( (0 <= axis) && (axis < 3) );
    static const int indices[3][3]
            = { { 0, 1, 2 }, { 1, 2, 0 }, { 2, 0, 1 } };
    const int i0 = indices[ axis ][ 0 ];
    const int i1 = indices[ axis ][ 1 ];
    const int i2 = indices[ axis ][ 2 ];
    const T cos = static_cast< T >( angle.Cos( ) );
    const T sin = static_cast< T >( angle.Sin( ) );
    m_elements[ i0 ][ i0 ] = 1.;
    m_elements[ i0 ][ i1 ] = m_elements[ i0 ][ i2 ] = m_elements[ i1 ][ i0 ]
            = m_elements[ i2 ][ i0 ] = 0.;
    m_elements[ i1 ][ i1 ] = m_elements[ i2 ][ i2 ] = cos;
    m_elements[ i2 ][ i1 ] = - sin;
    m_elements[ i1 ][ i2 ] = sin;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Matrix3<T>::Set( const EulerAngles & eulerAngles, EulerAngles::EOrder order )
{
    Assert( order <= EulerAngles::YXZ );
    static const int indices[6][3]
            = { { 0, 1, 2 }, { 1, 2, 0 }, { 2, 0, 1 },
                { 0, 2, 1 }, { 2, 1, 0 }, { 1, 0, 2 } };
    int i0 = indices[ order ][ 0 ];
    int i1 = indices[ order ][ 1 ];
    int i2 = indices[ order ][ 2 ];
    T c0 = static_cast<T>( eulerAngles[0].Cos( ) );
    T s0 = static_cast<T>( eulerAngles[0].Sin( ) );
    T c1 = static_cast<T>( eulerAngles[1].Cos( ) );
    T s1 = static_cast<T>( eulerAngles[1].Sin( ) );
    T c2 = static_cast<T>( eulerAngles[2].Cos( ) );
    T s2 = static_cast<T>( eulerAngles[2].Sin( ) );
    T c0c2 = c0 * c2;
    T c0s2 = c0 * s2;
    T s0c2 = s0 * c2;
    T s0s2 = s0 * s2;
    if ( order <= EulerAngles::ZXY )    //even permutation
    {
        m_elements[ i0 ][ i0 ] = c1 * c2;
        m_elements[ i1 ][ i0 ] = - c1 * s2;
        m_elements[ i2 ][ i0 ] = s1;
        m_elements[ i0 ][ i1 ] = c0s2  +  s1 * s0c2;
        m_elements[ i1 ][ i1 ] = c0c2  -  s1 * s0s2;
        m_elements[ i2 ][ i1 ] = - s0 * c1;
        m_elements[ i0 ][ i2 ] = s0s2  -  s1 * c0c2;
        m_elements[ i1 ][ i2 ] = s0c2  +  s1 * c0s2;
        m_elements[ i2 ][ i2 ] = c0 * c1;
    }
    else                    //odd permutation
    {
        m_elements[ i0 ][ i0 ] = c1 * c2;
        m_elements[ i1 ][ i0 ] = c1 * s2;
        m_elements[ i2 ][ i0 ] = - s1;
        m_elements[ i0 ][ i1 ] = - c0s2  +  s1 * s0c2;
        m_elements[ i1 ][ i1 ] = c0c2  +  s1 * s0s2;
        m_elements[ i2 ][ i1 ] = s0 * c1;
        m_elements[ i0 ][ i2 ] = s0s2  +  s1 * c0c2;
        m_elements[ i1 ][ i2 ] = - s0c2  +  s1 * c0s2;
        m_elements[ i2 ][ i2 ] = c0 * c1;
    }
}

//=============================================================================

template <typename T>
const T & 
Matrix3<T>::operator()( int row, int column ) const
{
    return m_elements[column][row];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
T & 
Matrix3<T>::operator()( int row, int column )
{
    return m_elements[column][row];
}

//=============================================================================

template <typename T>
const T & 
Matrix3<T>::Element( int row, int column ) const
{
    Assert( (0 <= row) && (row < 3) );
    Assert( (0 <= column) && (column < 3) );
    return m_elements[column][row];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
T & 
Matrix3<T>::Element( int row, int column )
{
    Assert( (0 <= row) && (row < 3) );
    Assert( (0 <= column) && (column < 3) );
    return m_elements[column][row];
}

//=============================================================================

template <typename T>
Vector3<T>
Matrix3<T>::Row( int row ) const
{
    return Vector3<T>( m_elements[0][row], m_elements[1][row],
                       m_elements[2][row] );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T>
Matrix3<T>::Column( int column ) const
{
    return Vector3<T>( m_elements[column].data() );
}

//=============================================================================

template <typename T>
const T * 
Matrix3<T>::Array( ) const
{
    return reinterpret_cast<T *>( m_elements.data() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
T * 
Matrix3<T>::Array( )
{
    return reinterpret_cast<T *>( m_elements.data() );
}

//=============================================================================

template <typename T>
Matrix3<T> 
Matrix3<T>::Transpose( ) const
{
    return Matrix3( m_elements[0][0], m_elements[1][0], m_elements[2][0],
                    m_elements[0][1], m_elements[1][1], m_elements[2][1],
                    m_elements[0][2], m_elements[1][2], m_elements[2][2],
                    true );
}

//=============================================================================

template <typename T>
T 
Matrix3<T>::Trace( ) const
{
    return (m_elements[0][0] + m_elements[1][1] + m_elements[2][2]);
}

//-----------------------------------------------------------------------------

template <typename T>
T 
Matrix3<T>::Determinant( ) const
{
    return (m_elements[0][0] * (m_elements[1][1] * m_elements[2][2]
                                -  m_elements[1][2] * m_elements[2][1])
            -  m_elements[0][1] * (m_elements[1][0] * m_elements[2][2]
                                   -  m_elements[1][2] * m_elements[2][0])
            +  m_elements[0][2] * (m_elements[1][0] * m_elements[2][1]
                                   -  m_elements[1][1] * m_elements[2][0]));
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T>
Matrix3<T>::Adjoint( ) const
{
    return Matrix3( (m_elements[1][1] * m_elements[2][2]
                     -  m_elements[1][2] * m_elements[2][1]),
                    (m_elements[0][2] * m_elements[2][1]
                     -  m_elements[0][1] * m_elements[2][2]),
                    (m_elements[0][1] * m_elements[1][2]
                     -  m_elements[1][1] * m_elements[0][2]),
                    (m_elements[1][2] * m_elements[2][0]
                     -  m_elements[1][0] * m_elements[2][2]),
                    (m_elements[0][0] * m_elements[2][2]
                     -  m_elements[0][2] * m_elements[2][0]),
                    (m_elements[0][2] * m_elements[1][0]
                     -  m_elements[0][0] * m_elements[1][2]),
                    (m_elements[1][0] * m_elements[2][1]
                     -  m_elements[2][0] * m_elements[1][1]),
                    (m_elements[0][1] * m_elements[2][0]
                     -  m_elements[0][0] * m_elements[2][1]),
                    (m_elements[0][0] * m_elements[1][1]
                     -  m_elements[0][1] * m_elements[1][0]),
                    true );
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> 
Matrix3<T>::Inverse( T determinant ) const
{
    T det = ( (determinant == T()) ? Determinant( ) : determinant );
    return  Inverse( det, Adjoint( ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Matrix3<T>
Matrix3<T>::Inverse( T determinant, const Matrix3 & adjoint ) const
{
    if ( determinant == T() )
        throw SingularMatrixException();
    return  adjoint * static_cast<T>( 1. / determinant );
}

//=============================================================================

template <typename T>
bool 
Matrix3<T>::operator==( const Matrix3 & rhs ) const
{
    return ( (m_elements[0][0] == rhs.m_elements[0][0])
             && (m_elements[0][1] == rhs.m_elements[0][1])
             && (m_elements[0][2] == rhs.m_elements[0][2])
             && (m_elements[1][0] == rhs.m_elements[1][0])
             && (m_elements[1][1] == rhs.m_elements[1][1])
             && (m_elements[1][2] == rhs.m_elements[1][2])
             && (m_elements[2][0] == rhs.m_elements[2][0])
             && (m_elements[2][1] == rhs.m_elements[2][1])
             && (m_elements[2][2] == rhs.m_elements[2][2]) );
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> & 
Matrix3<T>::operator+=( const Matrix3 & rhs )
{
    m_elements[0][0] += rhs.m_elements[0][0];
    m_elements[0][1] += rhs.m_elements[0][1];
    m_elements[0][2] += rhs.m_elements[0][2];
    m_elements[1][0] += rhs.m_elements[1][0];
    m_elements[1][1] += rhs.m_elements[1][1];
    m_elements[1][2] += rhs.m_elements[1][2];
    m_elements[2][0] += rhs.m_elements[2][0];
    m_elements[2][1] += rhs.m_elements[2][1];
    m_elements[2][2] += rhs.m_elements[2][2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> & 
Matrix3<T>::operator-=( const Matrix3 & rhs )
{
    m_elements[0][0] -= rhs.m_elements[0][0];
    m_elements[0][1] -= rhs.m_elements[0][1];
    m_elements[0][2] -= rhs.m_elements[0][2];
    m_elements[1][0] -= rhs.m_elements[1][0];
    m_elements[1][1] -= rhs.m_elements[1][1];
    m_elements[1][2] -= rhs.m_elements[1][2];
    m_elements[2][0] -= rhs.m_elements[2][0];
    m_elements[2][1] -= rhs.m_elements[2][1];
    m_elements[2][2] -= rhs.m_elements[2][2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> & 
Matrix3<T>::operator*=( T rhs )
{
    m_elements[0][0] *= rhs;
    m_elements[0][1] *= rhs;
    m_elements[0][2] *= rhs;
    m_elements[1][0] *= rhs;
    m_elements[1][1] *= rhs;
    m_elements[1][2] *= rhs;
    m_elements[2][0] *= rhs;
    m_elements[2][1] *= rhs;
    m_elements[2][2] *= rhs;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> & 
Matrix3<T>::operator*=( const Matrix3 & rhs )
{
    T m00 = m_elements[0][0];
    T m10 = m_elements[1][0];
    m_elements[0][0] = m00 * rhs.m_elements[0][0]
        +  m10 * rhs.m_elements[0][1]
        +  m_elements[2][0] * rhs.m_elements[0][2];
    m_elements[1][0] = m00 * rhs.m_elements[1][0]
        +  m10 * rhs.m_elements[1][1]
        +  m_elements[2][0] * rhs.m_elements[1][2];
    m_elements[2][0] = m00 * rhs.m_elements[2][0]
        +  m10 * rhs.m_elements[2][1]
        +  m_elements[2][0] * rhs.m_elements[2][2];
    T m01 = m_elements[0][1];
    T m11 = m_elements[1][1];
    m_elements[0][1] = m01 * rhs.m_elements[0][0]
        +  m11 * rhs.m_elements[0][1]
        +  m_elements[2][1] * rhs.m_elements[0][2];
    m_elements[1][1] = m01 * rhs.m_elements[1][0]
        +  m11 * rhs.m_elements[1][1]
        +  m_elements[2][1] * rhs.m_elements[1][2];
    m_elements[2][1] = m01 * rhs.m_elements[2][0]
        +  m11 * rhs.m_elements[2][1]
        +  m_elements[2][1] * rhs.m_elements[2][2];
    T m02 = m_elements[0][2];
    T m12 = m_elements[1][2];
    m_elements[0][2] = m02 * rhs.m_elements[0][0]
        +  m12 * rhs.m_elements[0][1]
        +  m_elements[2][2] * rhs.m_elements[0][2];
    m_elements[1][2] = m02 * rhs.m_elements[1][0]
        +  m12 * rhs.m_elements[1][1]
        +  m_elements[2][2] * rhs.m_elements[1][2];
    m_elements[2][2] = m02 * rhs.m_elements[2][0]
        +  m12 * rhs.m_elements[2][1]
        +  m_elements[2][2] * rhs.m_elements[2][2];
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> 
Matrix3<T>::operator-( ) const
{
    return Matrix3( - m_elements[0][0], - m_elements[0][1], - m_elements[0][2],
                    - m_elements[1][0], - m_elements[1][1], - m_elements[1][2],
                    - m_elements[2][0], - m_elements[2][1], - m_elements[2][2],
                    true );
}

//=============================================================================

template <typename T>
Matrix3<T> 
operator+( const Matrix3<T> & lhs, const Matrix3<T> & rhs )
{
    Matrix3<T> m = lhs;
    return m += rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> 
operator-( const Matrix3<T> & lhs, const Matrix3<T> & rhs )
{
    Matrix3<T> m = lhs;
    return m -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> 
operator*( T lhs, const Matrix3<T> & rhs )
{
    Matrix3<T> m = rhs;
    return m *= lhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> 
operator*( const Matrix3<T> & lhs, T rhs )
{
    Matrix3<T> m = lhs;
    return m *= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Matrix3<T> 
operator*( const Matrix3<T> & lhs, const Matrix3<T> & rhs )
{
    Matrix3<T> m = lhs;
    return m *= rhs;
}

//=============================================================================

template <typename T>
const Matrix3<T> Matrix3<T>::Zero( 0. );
template <typename T>
const Matrix3<T> Matrix3<T>::Identity( 1. );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Matrix3<T> & mat )
{
    return out << "[ [ " << mat( 0, 0 ) << ", " << mat( 0, 1 )
               << ", " << mat( 0, 2 ) << " ], "
               << "[ " << mat( 1, 0 ) << ", " << mat( 1, 1 )
               << ", " << mat( 1, 2 ) << " ], "
               << "[ " << mat( 2, 0 ) << ", " << mat( 2, 1 )
               << ", " << mat( 2, 2 ) << " ] ]";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Matrix3<T> & mat )
{
    return ToJSON( mat.m_elements );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Matrix3<T> * pMat )
{
    FromJSON( json, &(pMat->m_elements) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MATRIX3_HPP

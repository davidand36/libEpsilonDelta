#ifndef QUATERNION_HPP
#define QUATERNION_HPP
/*
  Quaternion.hpp
  Copyright (C) 2007 David M. Anderson

  Quaternion template class: the four-dimensional equivalent of complex
  numbers.
  NOTES:
  1. Quaternion is naturally implemented using Vector4, but note that the
     coordinates are somewhat different: w represents the "real" part, and is
     given index 0, unlike our Vector4, where w is typically the homogeneous
     scaling factor, and is given index 3. Of course the mathematics,
     especially multiplication, is different, although a vector-style "dot"
     product is sometimes also useful.
*/


#include "Vector4.hpp"
#include "NullVectorException.hpp"
#include "Algorithms.hpp"
#include "Array.hpp"
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class Quaternion;
template <typename T>
class AxisAngle;
template <typename T>
class RotationMatrix3;
template <typename T>
std::ostream & operator<<( std::ostream & out, const Quaternion<T> & quat );
template <typename T>
std::string ToJSON( const Quaternion<T> & quat );
template <typename T>
void FromJSON( const std::string & json, Quaternion<T> * pQuat );


//*****************************************************************************


template <typename T>
class Quaternion
{
public:
    Quaternion( );
    Quaternion( T w, T x, T y, T z );
    Quaternion( const T * pCoords );
    Quaternion( const Vector3<T> & vec3, T w = T() );
    Quaternion( const AxisAngle<T> & axisAngle );
    Quaternion( int axis, const Angle & angle );
    Quaternion( const EulerAngles & eulerAngles, EulerAngles::EOrder order );
    Quaternion( const RotationMatrix3<T> & matrix );
    void Set( T w, T x, T y, T z );
    void Set( const T * pCoords );
    void Set( const Vector3<T> & vec3, T w = T() );
    void Set( const AxisAngle<T> & axisAngle );
    void Set( int axis, const Angle & angle );
    void Set( const EulerAngles & eulerAngles, EulerAngles::EOrder order );
    void Set( const RotationMatrix3<T> & matrix );
    T W( ) const;
    T X( ) const;
    T Y( ) const;
    T Z( ) const;
    void SetW( T w );
    void SetX( T x );
    void SetY( T y );
    void SetZ( T z );
    const T & operator[]( int index ) const;
    T & operator[]( int index );
    const T & At( int index ) const;
    T & At( int index );
    const T * Array( ) const;
    T * Array( );
    T Real( ) const;
    Vector3<T> Imaginary( ) const;
    Quaternion Conjugate( ) const;
    T Length( ) const;
    T Norm( ) const;
    void Normalize( T length = T() );
    Quaternion Inverse( ) const;
    Quaternion Exp( ) const;
    Quaternion Log( ) const;
    AxisAngle<T> GetAxisAngle( ) const;
    EulerAngles GetEulerAngles( EulerAngles::EOrder order );
    RotationMatrix3<T> Matrix( ) const;
    Vector3<T> Rotate( const Vector3<T> & vector );
    bool operator==( const Quaternion & rhs ) const;
    bool operator!=( const Quaternion & rhs ) const;
    Quaternion operator-( ) const;
    Quaternion & operator+=( const Quaternion & rhs );
    Quaternion & operator-=( const Quaternion & rhs );
    Quaternion & operator*=( T rhs );

    static const Quaternion One;

private:
    Vector4<T> m_vec;

    friend std::string ToJSON<>( const Quaternion & quat );
    friend void FromJSON<>( const std::string & json, Quaternion * pQuat );
};

template <typename T>
Quaternion<T> operator+( const Quaternion<T> & lhs,
                         const Quaternion<T> & rhs );
template <typename T>
Quaternion<T> operator-( const Quaternion<T> & lhs,
                         const Quaternion<T> & rhs );
template <typename T>   //scalar product
Quaternion<T> operator*( T lhs, const Quaternion<T> & rhs );
template <typename T>   //scalar product
Quaternion<T> operator*( const Quaternion<T> & lhs, T rhs );
template <typename T>   //quaternion product
Quaternion<T> operator*( const Quaternion<T> & lhs,
                         const Quaternion<T> & rhs );
template <typename T>   //vector-style componentwise dot product
T Dot( const Quaternion<T> & lhs, const Quaternion<T> & rhs );

#ifdef DEBUG
bool TestQuaternion( );
#endif

//=============================================================================


typedef Quaternion<float>  QuaternionF;
typedef Quaternion<double> QuaternionD;


//*****************************************************************************


template <typename T>
inline 
Quaternion<T>::Quaternion( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
Quaternion<T>::Quaternion( T w, T x, T y, T z )
    :   m_vec( w, x, y, z )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Quaternion<T>::Quaternion( const T * pCoords )
    :   m_vec( pCoords )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Quaternion<T>::Quaternion( const Vector3<T> & vec3, T w )
{
    Set( vec3, w );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Quaternion<T>::Quaternion( const AxisAngle<T> & axisAngle )
{
    Set( axisAngle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Quaternion<T>::Quaternion( int axis, const Angle & angle )
{
    Set( axis, angle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Quaternion<T>::Quaternion( const EulerAngles & eulerAngles,
                           EulerAngles::EOrder order )
{
    Set( eulerAngles, order );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Quaternion<T>::Quaternion( const RotationMatrix3<T> & matrix )
{
    Set( matrix );
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Quaternion<T>::Set( T w, T x, T y, T z )
{
    m_vec.Set( w, x, y, z );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Quaternion<T>::Set( const T * pCoords )
{
    m_vec.Set( pCoords );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Quaternion<T>::Set( const Vector3<T> & vec3, T w )
{
    m_vec.Set( w, vec3[0], vec3[1], vec3[2] );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Quaternion<T>::Set( const AxisAngle<T> & axisAngle )
{
    Angle halfAngle = axisAngle.GetAngle() / 2.;
    T sin = static_cast< T >( halfAngle.Sin( ) );
    T cos = static_cast< T >( halfAngle.Cos( ) );
    Set( sin * axisAngle.Axis(), cos );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Quaternion<T>::Set( int axis, const Angle & angle )
{
    Angle halfAngle = angle / 2.;
    m_vec[0] = static_cast< T >( halfAngle.Cos( ) );
    m_vec[1] = m_vec[2] = m_vec[3] = 0.;
    m_vec[ axis + 1 ] = static_cast< T >( halfAngle.Sin( ) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
Quaternion<T>::Set( const EulerAngles & eulerAngles,
                    EulerAngles::EOrder order )
{
    Assert( order <= EulerAngles::YXZ );
    static const int indices[6][3]
            = { { 0, 1, 2 }, { 1, 2, 0 }, { 2, 0, 1 },
                { 0, 2, 1 }, { 2, 1, 0 }, { 1, 0, 2 } };
    int i0 = indices[ order ][ 0 ] + 1;
    int i1 = indices[ order ][ 1 ] + 1;
    int i2 = indices[ order ][ 2 ] + 1;
    Angle a0 = eulerAngles[0] * 0.5;
    Angle a1 = eulerAngles[1] * 0.5;
    Angle a2 = eulerAngles[2] * 0.5;
    T c0 = static_cast<T>( Cos( a0 ) );
    T s0 = static_cast<T>( Sin( a0 ) );
    T c1 = static_cast<T>( Cos( a1 ) );
    T s1 = static_cast<T>( Sin( a1 ) );
    T c2 = static_cast<T>( Cos( a2 ) );
    T s2 = static_cast<T>( Sin( a2 ) );
    T c0c1 = c0 * c1;
    T c0s1 = c0 * s1;
    T s0c1 = s0 * c1;
    T s0s1 = s0 * s1;
    if ( order <= EulerAngles::ZXY )    //even permutation
    {
        m_vec[0] = c0c1 * c2  -  s0s1 * s2;
        m_vec[i0] = c0s1 * s2  +  s0c1* c2;
        m_vec[i1] = - s0c1 * s2  +  c0s1 * c2;
        m_vec[i2] = s0s1 * c2  +  c0c1 * s2;
    }
    else                    //odd permutation
    {
        m_vec[0] = c0c1 * c2  +  s0s1 * s2;
        m_vec[i0] = - c0s1 * s2  +  s0c1 * c2;
        m_vec[i1] = s0c1 * s2  +  c0s1 * c2;
        m_vec[i2] = - s0s1 * c2  +  c0c1 * s2;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
Quaternion<T>::Set( const RotationMatrix3<T> & matrix )
{
    T m00 = matrix(0,0);
    T m11 = matrix(1,1);
    T m22 = matrix(2,2);
    T u = m00 + m11 + m22;
    array< T, 4 > diag = { { u, m00, m11, m22 } };
    int iMax = MaxIndex( diag.begin(), diag.end() );
    if ( iMax == 0 )
    {
        m_vec[0] = static_cast< T >( 0.5 * sqrt( 1 + m00 + m11 + m22 ) );
        T inv4W = static_cast< T >( 1. / (4. * m_vec[0]) );
        m_vec[1] = (matrix(2,1) - matrix(1,2)) * inv4W;
        m_vec[2] = (matrix(0,2) - matrix(2,0)) * inv4W;
        m_vec[3] = (matrix(1,0) - matrix(0,1)) * inv4W;
    }
    else
    {
        static const int indices[3][3]
                = { { 0, 1, 2 }, { 1, 2, 0 }, { 2, 0, 1 } };
        int i0 = indices[ iMax - 1 ][ 0 ];
        int i1 = indices[ iMax - 1 ][ 1 ];
        int i2 = indices[ iMax - 1 ][ 2 ];
        T x = static_cast< T >( 0.5 * sqrt( 1 + matrix( i0, i0 )
                                            - matrix( i1, i1 )
                                            - matrix( i2, i2 ) ) );
        T inv4X = static_cast< T >( 1. / (4. * x) );
        m_vec[0] = (matrix( i2, i1 ) - matrix( i1, i2 )) * inv4X;
        m_vec[ i0 + 1 ] = x;
        m_vec[ i1 + 1 ] = (matrix( i0, i1 ) + matrix( i1, i0 )) * inv4X;
        m_vec[ i2 + 1 ] = (matrix( i0, i2 ) + matrix( i2, i0 )) * inv4X;
    }
}

//=============================================================================

template <typename T>
inline 
T
Quaternion<T>::W( ) const
{
    return  m_vec[0];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Quaternion<T>::X( ) const
{
    return  m_vec[1];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Quaternion<T>::Y( ) const
{
    return  m_vec[2];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
T
Quaternion<T>::Z( ) const
{
    return  m_vec[3];
}

//=============================================================================

template <typename T>
inline 
void
Quaternion<T>::SetW( T w )
{
    m_vec[0] = w;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Quaternion<T>::SetX( T x )
{
    m_vec[1] = x;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Quaternion<T>::SetY( T y )
{
    m_vec[2] = y;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
void
Quaternion<T>::SetZ( T z )
{
    m_vec[3] = z;
}

//=============================================================================

template <typename T>
inline 
const T &
Quaternion<T>::operator[]( int index ) const
{
    return m_vec[ index ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
T &
Quaternion<T>::operator[]( int index )
{
    return m_vec[ index ];
}

//-----------------------------------------------------------------------------

template <typename T>
const T &
Quaternion<T>::At( int index ) const
{
    return  m_vec.At( index );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
T &
Quaternion<T>::At( int index )
{
    return  m_vec.At( index );
}

//=============================================================================

template <typename T>
inline 
const T *
Quaternion<T>::Array( ) const
{
    return  m_vec.Array( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
inline 
T *
Quaternion<T>::Array( )
{
    return  m_vec.Array( );
}

//=============================================================================

template <typename T>
inline 
T
Quaternion<T>::Real( ) const
{
    return  m_vec[0];
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Vector3<T>
Quaternion<T>::Imaginary( ) const
{
    return  Vector3<T>( m_vec[1], m_vec[2], m_vec[3] );
}

//=============================================================================

template <typename T>
inline 
Quaternion<T>
Quaternion<T>::Conjugate( ) const
{
    return Quaternion( m_vec[0], - m_vec[1], - m_vec[2], - m_vec[3] );
}

//=============================================================================

template <typename T>
T
Quaternion<T>::Length( ) const
{
    return  m_vec.Length();
}

//-----------------------------------------------------------------------------

template <typename T>
T
Quaternion<T>::Norm( ) const
{
    return  m_vec.LengthSquared();
}

//-----------------------------------------------------------------------------

template <typename T>
void
Quaternion<T>::Normalize( T length )
{
    m_vec.Normalize( length );
}

//=============================================================================

template <typename T>
Quaternion<T>
Quaternion<T>::Inverse( ) const
{
    return  Conjugate() * static_cast<T>(1. / Norm());
}

//=============================================================================

template <typename T>
Quaternion<T>
Quaternion<T>::Exp( ) const
{
    T r = Real();
    Vector3<T> im = Imaginary();
    Angle a = im.Length();
    if ( a.Radians() != 0. )
        im.Normalize( static_cast< T >( a.Radians() ) );
    T sin = static_cast< T >( a.Sin( ) );
    T cos = static_cast< T >( a.Cos( ) );
    Quaternion q( sin * im, cos );
    return  q *= exp( r );
}

//-----------------------------------------------------------------------------

template <typename T>
Quaternion<T>
Quaternion<T>::Log( ) const
{
    Quaternion q = *this;
    T len = q.Length();
    if ( len == 0 )
        throw NullVectorException();
    q.Normalize( len );
    T w = q.Real();
    Vector3<T> im = q.Imaginary();
    T s = im.Length();
    if ( s != 0 )
        im.Normalize( s );
    Angle angle = ArcTan( s, w );
    T a = static_cast< T >( angle.Radians() );
    return  Quaternion( a * im, std::log( len ) );
}

//=============================================================================

template <typename T>
AxisAngle<T>
Quaternion<T>::GetAxisAngle( ) const
{
    T w = Real();
    if ( fabs( w ) >= 1. )
        return  AxisAngle<T>( Vector3<T>::UnitX, 0. );
    Angle angle = 2 * ArcCos( w );
    Vector3<T> axis = Imaginary();
    axis.Normalize( sqrt( 1 - w * w ) );
    return  AxisAngle<T>( axis, angle );
}

//-----------------------------------------------------------------------------

template <typename T>
EulerAngles 
Quaternion<T>::GetEulerAngles( EulerAngles::EOrder order )
{
    Assert( order <= EulerAngles::YXZ );
    static const int indices[6][3]
            = { { 0, 1, 2 }, { 1, 2, 0 }, { 2, 0, 1 },
                { 0, 2, 1 }, { 2, 1, 0 }, { 1, 0, 2 } };
    int i0 = indices[ order ][ 0 ] + 1;
    int i1 = indices[ order ][ 1 ] + 1;
    int i2 = indices[ order ][ 2 ] + 1;
    T w = m_vec[0];
    T x = m_vec[ i0 ];
    T y = m_vec[ i1 ];
    T z = m_vec[ i2 ];
    Angle angles[3];
    if ( order <= EulerAngles::ZXY )    //even permutation
    {
        T m00 = static_cast<T>( 1. - 2. * (y * y  +  z * z) );
        T m01 = static_cast<T>( 2. * (x * y  -  w * z) );
        T c1 = static_cast<T>( std::sqrt( m00 * m00  +  m01 * m01 ) );
        angles[1] = ArcTan( 2. * (x * z  +  w * y), c1 );
        if ( c1 > 16. * std::numeric_limits<T>::epsilon() )
        {
            angles[0] = ArcTan( -2. * (y * z  -  w  * x),
                                1. - 2. * (x * x +  y * y) );
            angles[2] = ArcTan( -2. * (x * y  -  w * z), m00 );
        }
        else
        {
            if ( angles[1].Radians() > 0. )
            {
                //"gimble lock", only (angles[0] + angles[2]) determined
                angles[0] = ArcTan( 2. * (x * y  +  w * z),
                                    1. - 2. * (x * x  +  z * z) );
            }
            else
            {
                //"gimble lock", only (angles[0] - angles[2]) determined
                angles[0] = - ArcTan( 2. * (x * y  +  w * z),
                                      1. - 2. * (x * x  +  z * z) );
            }
            angles[2] = 0.;
        }
    }
    else                    //odd permutation
    {
        T m00 = static_cast<T>( 1. - 2. * (y * y  +  z * z) );
        T m01 = static_cast<T>( 2. * (x * y  +  w * z) );
        T c1 = static_cast<T>( std::sqrt( m00 * m00  +  m01 * m01 ) );
        angles[1] = ArcTan( -2. * (x * z  -  w * y), c1 );
        if ( c1 > 16. * std::numeric_limits<T>::epsilon() )
        {
            angles[0] = ArcTan( 2. * (y * z  +  w  * x),
                                1. - 2. * (x * x +  y * y) );
            angles[2] = ArcTan( 2. * (x * y  +  w * z), m00 );
        }
        else
        {
            if ( angles[1].Radians() > 0. )
            {
                //"gimble lock", only (angles[0] - angles[2]) determined
                angles[0] = ArcTan( 2. * (x * y  -  w * z),
                                    1. - 2. * (x * x  +  z * z) );
            }
            else
            {
                //"gimble lock", only (angles[0] + angles[2]) determined 
                angles[0] = ArcTan( -2. * (x * y  -  w * z),
                                    1. - 2. * (x * x  +  z * z) );
            }
            angles[2] = 0.;
        }
    }
    return  EulerAngles( angles );
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix3<T>
Quaternion<T>::Matrix( ) const
{
    T xsq2 = static_cast< T >( 2. * m_vec[1] * m_vec[1] );
    T ysq2 = static_cast< T >( 2. * m_vec[2] * m_vec[2] );
    T zsq2 = static_cast< T >( 2. * m_vec[3] * m_vec[3] );
    T wx2 = static_cast< T >( 2. * m_vec[0] * m_vec[1] );
    T wy2 = static_cast< T >( 2. * m_vec[0] * m_vec[2] );
    T wz2 = static_cast< T >( 2. * m_vec[0] * m_vec[3] );
    T xy2 = static_cast< T >( 2. * m_vec[1] * m_vec[2] );
    T xz2 = static_cast< T >( 2. * m_vec[1] * m_vec[3] );
    T yz2 = static_cast< T >( 2. * m_vec[2] * m_vec[3] );
    T m00 = static_cast< T >( 1. - (ysq2 + zsq2) );
    T m11 = static_cast< T >( 1. - (xsq2 + zsq2) );
    T m22 = static_cast< T >( 1. - (xsq2 + ysq2) );
    Matrix3<T> mat3( m00,  xy2 - wz2,  xz2 + wy2,
                     xy2 + wz2,  m11,  yz2 - wx2,
                     xz2 - wy2,  yz2 + wx2,  m22 );
    return  RotationMatrix3<T>( mat3 );
}

//=============================================================================

template <typename T>
Vector3<T> 
Quaternion<T>::Rotate( const Vector3<T> & vector )
{
    //The direct way would be
    // return  (*this * Quaternion( vector ) * Conjugate()).Imaginary();
    // but that is a bit less efficient than converting to a matrix.
    return  Matrix() * vector;
}

//=============================================================================

template <typename T>
bool
Quaternion<T>::operator==( const Quaternion & rhs ) const
{
    return  m_vec == rhs.m_vec;
}

//-----------------------------------------------------------------------------

template <typename T>
bool
Quaternion<T>::operator!=( const Quaternion & rhs ) const
{
    return  m_vec != rhs.m_vec;
}

//-----------------------------------------------------------------------------

template <typename T>
inline 
Quaternion<T>
Quaternion<T>::operator-( ) const
{
    return  Quaternion<T>( -m_vec[0], -m_vec[1], -m_vec[2], -m_vec[3] );
}

//-----------------------------------------------------------------------------

template <typename T>
Quaternion<T> &
Quaternion<T>::operator+=( const Quaternion & rhs )
{
    m_vec += rhs.m_vec;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Quaternion<T> &
Quaternion<T>::operator-=( const Quaternion & rhs )
{
    m_vec -= rhs.m_vec;
    return *this;
}

//-----------------------------------------------------------------------------

template <typename T>
Quaternion<T> &
Quaternion<T>::operator*=( T rhs )
{
    m_vec *= rhs;
    return *this;
}

//=============================================================================

template <typename T>
Quaternion<T>
operator+( const Quaternion<T> & lhs, const Quaternion<T> & rhs )
{
    Quaternion<T> q = lhs;
    return  q += rhs;
}
    
//-----------------------------------------------------------------------------

template <typename T>
Quaternion<T>
operator-( const Quaternion<T> & lhs, const Quaternion<T> & rhs )
{
    Quaternion<T> q = lhs;
    return  q -= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Quaternion<T>
operator*( T lhs, const Quaternion<T> & rhs )
{
    Quaternion<T> q = rhs;
    return  q *= lhs;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
Quaternion<T>
operator*( const Quaternion<T> & lhs, T rhs )
{
    Quaternion<T> q = lhs;
    return  q *= rhs;
}

//-----------------------------------------------------------------------------

template <typename T>
Quaternion<T>
operator*( const Quaternion<T> & lhs, const Quaternion<T> & rhs )
{
    return  Quaternion<T>(  lhs.W() * rhs.W()  -  lhs.X() * rhs.X()
                                -  lhs.Y() * rhs.Y()  -  lhs.Z() * rhs.Z(),
                            lhs.Y() * rhs.Z()  -  lhs.Z() * rhs.Y()
                                +  lhs.X() * rhs.W()  +  lhs.W() * rhs.X(),
                            lhs.Z() * rhs.X()  -  lhs.X() * rhs.Z()
                                +  lhs.Y() * rhs.W()  +  lhs.W() * rhs.Y(),
                            lhs.X() * rhs.Y()  -  lhs.Y() * rhs.X()
                                +  lhs.Z() * rhs.W()  +  lhs.W() * rhs.Z() );
}

//=============================================================================

template <typename T>
T 
Dot( const Quaternion<T> & lhs, const Quaternion<T> & rhs )
{
    return  lhs[0] * rhs[0]  +  lhs[1] * rhs[1]  +  lhs[2] * rhs[2]
            +  lhs[3] * rhs[3];
}

//=============================================================================

template <typename T>
const Quaternion<T> Quaternion<T>::One( Vector3<T>::Zero,
                                         static_cast<T>( 1 ) );

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const Quaternion<T> & quat )
{
    return out << "( " << quat.W() << ", " << quat.X()
               << ", " << quat.Y() << ", " << quat.Z() << " )";
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const Quaternion<T> & quat )
{
    return ToJSON( quat.m_vec );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, Quaternion<T> * pQuat )
{
    FromJSON( json, &(pQuat->m_vec) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //QUATERNION_HPP

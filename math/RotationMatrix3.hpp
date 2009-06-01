#ifndef ROTATIONMATRIX3_HPP
#define ROTATIONMATRIX3_HPP
/*
  RotationMatrix3.hpp
  Copyright (C) 2007 David M. Anderson

  RotationMatrix3 template class: a 3x3 matrix representing a rotation about
  an axis.
  NOTES:
  1. Fixup() performs a Gram-Schmidt orthogonalization, which will restore
     orthogonality that may be lost through accumulated round-off errors.
*/


#include "Matrix3.hpp"
#include <cmath>
#include <limits>
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class RotationMatrix3;
template <typename T>
class Quaternion;
template <typename T>
std::ostream & operator<<( std::ostream & out, const RotationMatrix3<T> & mat );
template <typename T>
std::string ToJSON( const RotationMatrix3<T> & mat );
template <typename T>
void FromJSON( const std::string & json, RotationMatrix3<T> * pMat );


//#############################################################################


template <typename T>
class RotationMatrix3
{
public:
    RotationMatrix3( );
    RotationMatrix3( const AxisAngle<T> & axisAngle );
    RotationMatrix3( int axis, const Angle & angle );
    RotationMatrix3( const EulerAngles & eulerAngles,
                     EulerAngles::EOrder order );
    void Set( const AxisAngle<T> & axisAngle );
    void Set( int axis, const Angle & angle );
    void Set( const EulerAngles & eulerAngles, EulerAngles::EOrder order );
    operator const Matrix3<T> &( ) const;
    const T & operator()( int row, int column ) const;
    const T & Element( int row, int column ) const;
    Vector3<T> Row( int row ) const;
    Vector3<T> Column( int column ) const;
    const T * Array( ) const;
    AxisAngle<T> GetAxisAngle( ) const;
    EulerAngles GetEulerAngles( EulerAngles::EOrder order );
    bool operator==( const RotationMatrix3 & rhs ) const;
    RotationMatrix3 & operator*=( const RotationMatrix3 & rhs );
    T Trace( ) const;
    T Determinant( ) const;
    RotationMatrix3 Transpose( ) const;
    RotationMatrix3 Inverse( ) const;
    void Fixup( );

private:
    RotationMatrix3( const Matrix3<T> & matrix );

    Matrix3<T> m_matrix;

    friend class Quaternion<T>;
    friend std::string ToJSON<>( const RotationMatrix3 & mat );
    friend void FromJSON<>( const std::string & json, RotationMatrix3 * pMat );
};

//.............................................................................

template <typename T>
RotationMatrix3<T> operator*( const RotationMatrix3<T> & lhs,
                              const RotationMatrix3<T> & rhs );
template <typename T>
Vector3<T> operator*( const RotationMatrix3<T> & m, const Vector3<T> & v );
template <typename T>
Vector3<T> operator*( const Vector3<T> & v, const RotationMatrix3<T> & m );

#ifdef DEBUG
bool TestRotationMatrix3( );
#endif


//=============================================================================


typedef RotationMatrix3<float>   RotationMatrix3F;
typedef RotationMatrix3<double>  RotationMatrix3D;


//#############################################################################


template <typename T>
RotationMatrix3<T>::RotationMatrix3( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
RotationMatrix3<T>::RotationMatrix3( const AxisAngle<T> & axisAngle )
    :   m_matrix( axisAngle )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
RotationMatrix3<T>::RotationMatrix3( int axis, const Angle & angle )
    :   m_matrix( axis, angle )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
RotationMatrix3<T>::RotationMatrix3( const EulerAngles & eulerAngles,
                                     EulerAngles::EOrder order )
    :   m_matrix( eulerAngles, order )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
RotationMatrix3<T>::RotationMatrix3( const Matrix3<T> & matrix )
    :   m_matrix( matrix )
{
}

//=============================================================================

template <typename T>
void
RotationMatrix3<T>::Set( const AxisAngle<T> & axisAngle )
{
    m_matrix.Set( axisAngle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void
RotationMatrix3<T>::Set( int axis, const Angle & angle )
{
    m_matrix.Set( axis, angle );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
void 
RotationMatrix3<T>::Set( const EulerAngles & eulerAngles,
                         EulerAngles::EOrder order )
{
    m_matrix.Set( eulerAngles, order );
}

//=============================================================================

template <typename T>
RotationMatrix3<T>::operator const Matrix3<T> &( ) const
{
    return  m_matrix;
}

//=============================================================================

template <typename T>
const T & 
RotationMatrix3<T>::operator()( int row, int column ) const
{
    return  m_matrix( row, column );
}

//-----------------------------------------------------------------------------

template <typename T>
const T & 
RotationMatrix3<T>::Element( int row, int column ) const
{
    return  m_matrix.Element( row, column );
}

//=============================================================================

template <typename T>
Vector3<T> 
RotationMatrix3<T>::Row( int row ) const
{
    return  m_matrix.Row( row );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T> 
RotationMatrix3<T>::Column( int column ) const
{
    return  m_matrix.Column( column );
}

//=============================================================================

template <typename T>
const T * 
RotationMatrix3<T>::Array( ) const
{
    return  m_matrix.Array( );
}

//=============================================================================

template <typename T>
AxisAngle<T> 
RotationMatrix3<T>::GetAxisAngle( ) const
{
    Angle angle = ArcCos( (Trace() - 1.) / 2. );
    Vector3<T> axis;
    if ( angle.Radians() <= 0. )
        axis = Vector3<T>::UnitX;   //arbitrary: there is no rotation.
    else if ( angle.Radians() < M_PI )
    {
        axis.Set( m_matrix( 2, 1 ) - m_matrix( 1, 2 ),
                  m_matrix( 0, 2 ) - m_matrix( 2, 0 ),
                  m_matrix( 1, 0 ) - m_matrix( 0, 1 ) );
        axis.Normalize( );
    }
    else
    {
        int i0 = 0;
        int i1 = 1;
        int i2 = 2;
        T maxD = m_matrix( 0, 0 );
        if ( m_matrix( 1, 1 ) > maxD )
        {
            i0 = 1;
            i1 = 0;
            maxD = m_matrix( 1, 1 );
        }
        if ( m_matrix( 2, 2 ) > maxD )
        {
            i0 = 2;
            i1 = 0;
            i2 = 1;
        }
        T axisComps[3];
        axisComps[i0] = static_cast<T>( 0.5 ) * std::sqrt( m_matrix( i0, i0 )
                                         - m_matrix( i1, i1 )
                                         - m_matrix( i2, i2 )
                                         + static_cast<T>( 1. ) );
        axisComps[i1] = m_matrix( i1, i0 )
                / (static_cast<T>( 2. ) * axisComps[i0]);
        axisComps[i2] = m_matrix( i1, i0 )
                / (static_cast<T>( 2. ) * axisComps[i0]);
        axis.Set( axisComps[0], axisComps[1], axisComps[2] );
    }
    return  AxisAngle<T>( axis, angle );
}

//-----------------------------------------------------------------------------

template <typename T>
EulerAngles 
RotationMatrix3<T>::GetEulerAngles( EulerAngles::EOrder order )
{
    Assert( order <= EulerAngles::YXZ );
    static const int indices[6][3]
            = { { 0, 1, 2 }, { 1, 2, 0 }, { 2, 0, 1 },
                { 0, 2, 1 }, { 2, 1, 0 }, { 1, 0, 2 } };
    const int i0 = indices[ order ][ 0 ];
    const int i1 = indices[ order ][ 1 ];
    const int i2 = indices[ order ][ 2 ];
    T c1 = std::sqrt( m_matrix( i0, i0 ) * m_matrix( i0, i0 )
                      +  m_matrix( i0, i1 ) * m_matrix( i0, i1 ) );
    Angle angles[3];
    if ( order <= EulerAngles::ZXY )    //even permutation
    {
        angles[1] = ArcTan( m_matrix( i0, i2 ), c1 );
        if ( c1 > 16. * std::numeric_limits<T>::epsilon() )
        {
            angles[0] = ArcTan( - m_matrix( i1, i2 ), m_matrix( i2, i2 ) );
            angles[2] = ArcTan( - m_matrix( i0, i1 ), m_matrix( i0, i0 ) );
        }
        else
        {
            if ( angles[1].Radians() > 0. )
                //"gimble lock", only (angles[0] + angles[2]) determined
                angles[0] = ArcTan( m_matrix( i1, i0 ), m_matrix( i1, i1 ) );
            else
                //"gimble lock", only (angles[0] - angles[2]) determined
                angles[0] = - ArcTan( m_matrix( i1, i0 ), m_matrix( i1, i1 ) );
            angles[2] = 0.;
        }
    }
    else                    //odd permutation
    {
        angles[1] = ArcTan( - m_matrix( i0, i2 ), c1 );
        if ( c1 > 16. * std::numeric_limits<T>::epsilon() )
        {
            angles[0] = ArcTan( m_matrix( i1, i2 ), m_matrix( i2, i2 ) );
            angles[2] = ArcTan(  m_matrix( i0, i1 ), m_matrix( i0, i0 ) );
        }
        else
        {
            if ( angles[1].Radians() > 0. )
                //"gimble lock", only (angles[0] - angles[2]) determined
                angles[0] = ArcTan( m_matrix( i1, i0 ), m_matrix( i1, i1 ) );
            else
                //"gimble lock", only (angles[0] + angles[2]) determined
                angles[0] = ArcTan( - m_matrix( i1, i0 ), m_matrix( i1, i1 ) );
            angles[2] = 0.;
        }
    }
    return  EulerAngles( angles );
}

//=============================================================================

template <typename T>
bool 
RotationMatrix3<T>::operator==( const RotationMatrix3 & rhs ) const
{
    return  (m_matrix == rhs.m_matrix);
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix3<T> & 
RotationMatrix3<T>::operator*=( const RotationMatrix3 & rhs )
{
    m_matrix *= rhs;
    return *this;
}

//=============================================================================

template <typename T>
T 
RotationMatrix3<T>::Trace( ) const
{
    return  m_matrix.Trace( );
}

//-----------------------------------------------------------------------------

template <typename T>
T 
RotationMatrix3<T>::Determinant( ) const
{
    return  m_matrix.Determinant( );
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix3<T> 
RotationMatrix3<T>::Transpose( ) const
{
    return  m_matrix.Transpose( );
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix3<T> 
RotationMatrix3<T>::Inverse( ) const
{
    return  m_matrix.Transpose( );  //inverse of orthogonal matrix is transpose
}

//=============================================================================

template <typename T>
void
RotationMatrix3<T>::Fixup( )
{
    Vector3<T> v0 = Column( 0 );
    Vector3<T> v1 = Column( 1 );
    Vector3<T> v2 = Column( 2 );
    v0.Normalize( );
    v1 -= (v0 * v1) * v0;
    v1.Normalize( );
    v2 -= (v0 * v2) * v0  +  (v1 * v2) * v2;
    v2.Normalize( );
    m_matrix.Set( v0, v1, v2 );
}

//=============================================================================

template <typename T>
RotationMatrix3<T> 
operator*( const RotationMatrix3<T> & lhs, const RotationMatrix3<T> & rhs )
{
    RotationMatrix3<T> m = lhs;
    return  m *= rhs;
}

//=============================================================================

template <typename T>
Vector3<T> 
operator*( const RotationMatrix3<T> & m, const Vector3<T> & v )
{
    return  static_cast< const Matrix3<T> & >( m ) * v;
}

//-----------------------------------------------------------------------------

template <typename T>
Vector3<T> 
operator*( const Vector3<T> & v, const RotationMatrix3<T> & m )
{
    return v * static_cast< const Matrix3<T> & >( m );
}

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const RotationMatrix3<T> & mat )
{
    return out << (const Matrix3<T> &)mat;
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const RotationMatrix3<T> & mat )
{
    return ToJSON( mat.m_matrix );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, RotationMatrix3<T> * pMat )
{
    FromJSON( json, &(pMat->m_matrix) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ROTATIONMATRIX3_HPP

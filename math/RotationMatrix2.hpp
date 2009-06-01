#ifndef ROTATIONMATRIX2_HPP
#define ROTATIONMATRIX2_HPP
/*
  RotationMatrix2.hpp
  Copyright (C) 2007 David M. Anderson

  RotationMatrix2 template class: a 2x2 matrix representing a rotation about
  the origin.
  NOTES:
  1. Fixup() performs a Gram-Schmidt orthogonalization, which will restore
     orthogonality that may be lost through accumulated round-off errors.
*/


#include "Matrix2.hpp"
#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
class RotationMatrix2;
template <typename T>
std::ostream & operator<<( std::ostream & out, const RotationMatrix2<T> & mat );
template <typename T>
std::string ToJSON( const RotationMatrix2<T> & mat );
template <typename T>
void FromJSON( const std::string & json, RotationMatrix2<T> * pMat );


//#############################################################################


template <typename T>
class RotationMatrix2
{
public:
    RotationMatrix2( );
    RotationMatrix2( const Angle & angle );
    void Set( const Angle & angle = 0. );
    operator const Matrix2<T> &( ) const;
    const T & operator()( int row, int column ) const;
    const T & Element( int row, int column ) const;
    Vector2<T> Row( int row ) const;
    Vector2<T> Column( int column ) const;
    const T * Array( ) const;
    Angle GetAngle( ) const;
    bool operator==( const RotationMatrix2 & rhs ) const;
    RotationMatrix2 & operator*=( const RotationMatrix2 & rhs );
    RotationMatrix2 operator-( ) const;
    T Trace( ) const;
    T Determinant( ) const;
    RotationMatrix2 Transpose( ) const;
    RotationMatrix2 Inverse( ) const;
    void Fixup( );

private:
    RotationMatrix2( const Matrix2<T> & matrix );

    Matrix2<T> m_matrix;

    friend std::string ToJSON<>( const RotationMatrix2 & mat );
    friend void FromJSON<>( const std::string & json, RotationMatrix2 * pMat );
};

//.............................................................................

template <typename T>
RotationMatrix2<T> operator*( const RotationMatrix2<T> & lhs,
                              const RotationMatrix2<T> & rhs );
template <typename T>
Vector2<T> operator*( const RotationMatrix2<T> & m, const Vector2<T> & v );
template <typename T>
Vector2<T> operator*( const Vector2<T> & v, const RotationMatrix2<T> & m );

#ifdef DEBUG
bool TestRotationMatrix2( );
#endif


//=============================================================================


typedef RotationMatrix2<float>  RotationMatrix2F;
typedef RotationMatrix2<double> RotationMatrix2D;


//*****************************************************************************


template <typename T>
RotationMatrix2<T>::RotationMatrix2( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
RotationMatrix2<T>::RotationMatrix2( const Angle & angle )
    :   m_matrix( angle )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template <typename T>
RotationMatrix2<T>::RotationMatrix2( const Matrix2<T> & matrix )
    :   m_matrix( matrix )
{
}

//=============================================================================

template <typename T>
void
RotationMatrix2<T>::Set( const Angle & angle )
{
    m_matrix.Set( angle );
}

//=============================================================================

template <typename T>
RotationMatrix2<T>::operator const Matrix2<T> &( ) const
{
    return  m_matrix;
}

//=============================================================================

template <typename T>
const T & 
RotationMatrix2<T>::operator()( int row, int column ) const
{
    return  m_matrix( row, column );
}

//-----------------------------------------------------------------------------

template <typename T>
const T & 
RotationMatrix2<T>::Element( int row, int column ) const
{
    return  m_matrix.Element( row, column );
}

//=============================================================================

template <typename T>
Vector2<T> 
RotationMatrix2<T>::Row( int row ) const
{
    return  m_matrix.Row( row );
}

//-----------------------------------------------------------------------------

template <typename T>
Vector2<T> 
RotationMatrix2<T>::Column( int column ) const
{
    return  m_matrix.Column( column );
}

//=============================================================================

template <typename T>
const T * 
RotationMatrix2<T>::Array( ) const
{
    return  m_matrix.Array( );
}

//=============================================================================

template <typename T>
Angle 
RotationMatrix2<T>::GetAngle( ) const
{
    return  ArcTan( m_matrix( 1, 0 ), m_matrix( 0, 0 ) );
}

//=============================================================================

template <typename T>
bool 
RotationMatrix2<T>::operator==( const RotationMatrix2 & rhs ) const
{
    return  (m_matrix == rhs.m_matrix);
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix2<T> & 
RotationMatrix2<T>::operator*=( const RotationMatrix2 & rhs )
{
    m_matrix *= rhs;
    return  *this;
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix2<T> 
RotationMatrix2<T>::operator-( ) const
{
    return  -m_matrix;
}

//=============================================================================

template <typename T>
T 
RotationMatrix2<T>::Trace( ) const
{
    return  m_matrix.Trace( );
}

//-----------------------------------------------------------------------------

template <typename T>
T 
RotationMatrix2<T>::Determinant( ) const
{
    return  m_matrix.Determinant( );
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix2<T> 
RotationMatrix2<T>::Transpose( ) const
{
    return  m_matrix.Transpose( );
}

//-----------------------------------------------------------------------------

template <typename T>
RotationMatrix2<T> 
RotationMatrix2<T>::Inverse( ) const
{
    return  m_matrix.Transpose( );  //inverse of orthogonal matrix is transpose
}

//=============================================================================

template <typename T>
void
RotationMatrix2<T>::Fixup( )
{
    Vector2<T> v0 = Column( 0 );
    Vector2<T> v1 = Column( 1 );
    v0.Normalize( );
    v1 -= (v0 * v1) * v0;
    v1.Normalize( );
    m_matrix.Set( v0, v1 );
}

//=============================================================================

template <typename T>
RotationMatrix2<T> 
operator*( const RotationMatrix2<T> & lhs, const RotationMatrix2<T> & rhs )
{
    RotationMatrix2<T> m = lhs;
    return  m *= rhs;
}

//=============================================================================

template <typename T>
Vector2<T> 
operator*( const RotationMatrix2<T> & m, const Vector2<T> & v )
{
    return  static_cast< const Matrix2<T> & >( m ) * v;
}

//-----------------------------------------------------------------------------

template <typename T>
Vector2<T> 
operator*( const Vector2<T> & v, const RotationMatrix2<T> & m )
{
    return v * static_cast< const Matrix2<T> & >( m );
}

//=============================================================================

template <typename T>
std::ostream & 
operator<<( std::ostream & out, const RotationMatrix2<T> & mat )
{
    return out << (const Matrix2<T> &)mat;
}

//=============================================================================

template <typename T>
std::string 
ToJSON( const RotationMatrix2<T> & mat )
{
    return ToJSON( mat.m_matrix );
}

//-----------------------------------------------------------------------------

template <typename T>
void 
FromJSON( const std::string & json, RotationMatrix2<T> * pMat )
{
    FromJSON( json, &(pMat->m_matrix) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ROTATIONMATRIX2_HPP

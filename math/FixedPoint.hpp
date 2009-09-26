#ifndef FIXEDPOINT_HPP
#define FIXEDPOINT_HPP
/*
  FixedPoint.hpp
  Copyright (C) 2009 David M. Anderson

  FixedPoint template class: fixed-point representation of a (real) number.
  NOTES:
  1. The template parameter R is the "raw" underlying type (e.g., int32_t).
  2. The template parameter F is the number of fractional bits.
  3. Raw() returns the underlying representation. So, e.g., a fixed-point
     number representing 2 has a raw value of (2 << F).
     ToInt() returns the truncated (i.e. floor) integer part of the number.
     ToFloat() returns the (approximate) floating-point equivalent.
  4. For the operators * and /, separate implementations are used when there is
     a larger native type available (e.g. int32_t for FixedPoint< int32_t, F >.
*/


#include "StdInt.hpp"
#include "Shift.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename R, int F >
class FixedPoint
{
public:
    typedef R  RawType;

    FixedPoint( );
    FixedPoint( int i );
    FixedPoint( R i, bool raw );
    FixedPoint( const FixedPoint & f );
    template < typename R1, int F1 > FixedPoint( const FixedPoint<R1,F1> & f );
    FixedPoint( double f );

    R Raw( ) const;
    int ToInt( ) const;
    float ToFloat( ) const;

    bool operator==( const FixedPoint & rhs ) const;
    bool operator!=( const FixedPoint & rhs ) const;
    bool operator<( const FixedPoint & rhs ) const;
    bool operator<=( const FixedPoint & rhs ) const;
    bool operator>( const FixedPoint & rhs ) const;
    bool operator>=( const FixedPoint & rhs ) const;
    FixedPoint & operator+=( const FixedPoint & rhs );
    FixedPoint & operator-=( const FixedPoint & rhs );
    FixedPoint & operator*=( const FixedPoint & rhs );
    FixedPoint & operator/=( const FixedPoint & rhs );
    FixedPoint operator-( ) const;

    static int TotalBits( );
    static int FractionBits( );

private:
    R    m_raw;
};

//.............................................................................

template < typename R, int F >
FixedPoint<R,F> operator+( FixedPoint<R,F> f1, FixedPoint<R,F> f2 );
template < typename R, int F >
FixedPoint<R,F> operator+( FixedPoint<R,F> f1, int i2 );
template < typename R, int F >
FixedPoint<R,F> operator+( int i1, FixedPoint<R,F> f2 );
template < typename R, int F >
FixedPoint<R,F> operator-( FixedPoint<R,F> f1, FixedPoint<R,F> f2 );
template < typename R, int F >
FixedPoint<R,F> operator-( FixedPoint<R,F> f1, int i2 );
template < typename R, int F >
FixedPoint<R,F> operator-( int i1, FixedPoint<R,F> f2 );
template < typename R, int F >
FixedPoint<R,F> operator*( FixedPoint<R,F> f1, FixedPoint<R,F> f2 );
template < typename R, int F, typename L >
FixedPoint<R,F> operator*( FixedPoint<R,F> f1, FixedPoint<R,F> f2 );
template< int F >
FixedPoint<int16_t, F> operator*( FixedPoint<int16_t,F> f1,
                                  FixedPoint<int16_t,F> f2 );
template< int F >
FixedPoint<int32_t, F> operator*( FixedPoint<int32_t,F> f1,
                                  FixedPoint<int32_t,F> f2 );
template < typename R, int F >
FixedPoint<R,F> operator*( FixedPoint<R,F> f1, int i2 );
template < typename R, int F >
FixedPoint<R,F> operator*( int i1, FixedPoint<R,F> f2 );
template < typename R, int F >
FixedPoint<R,F> operator/( FixedPoint<R,F> f1, FixedPoint<R,F> f2 );
template < typename R, int F, typename L >
FixedPoint<R,F> operator/( FixedPoint<R,F> f1, FixedPoint<R,F> f2 );
template< int F >
FixedPoint<int16_t, F> operator/( FixedPoint<int16_t,F> f1,
                                  FixedPoint<int16_t,F> f2 );
template< int F >
FixedPoint<int32_t, F> operator/( FixedPoint<int32_t,F> f1,
                                  FixedPoint<int32_t,F> f2 );
template < typename R, int F >
FixedPoint<R,F> operator/( FixedPoint<R,F> f1, int i2 );
template < typename R, int F >
FixedPoint<R,F> operator/( int i1, FixedPoint<R,F> f2 );

#ifdef DEBUG
bool TestFixedPoint( );
#endif

//.............................................................................

typedef FixedPoint<int32_t, 12 > Fixed32_12;
typedef FixedPoint<int32_t, 8 > Fixed32_8;
typedef FixedPoint<int32_t, 6 > Fixed32_6;
typedef FixedPoint<int16_t, 12 > Fixed16_12;
typedef FixedPoint<int16_t, 8 > Fixed16_8;


//*****************************************************************************


template < typename R, int F >
inline
FixedPoint<R,F>::FixedPoint( )
    :   m_raw( 0 )
{
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F>::FixedPoint( int i )
    :   m_raw( (R)((R)i << F) )
{
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F>::FixedPoint( R i, bool raw )
    :   m_raw( raw  ?  (R)i  :  (R)((R)i << F) )
{
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F>::FixedPoint( const FixedPoint & f )
    :   m_raw( f.m_raw )
{
}

//.............................................................................

template < typename R, int F >
template < typename R1, int F1 > 
inline
FixedPoint<R,F>::FixedPoint( const FixedPoint<R1,F1> & f )
{
    if ( sizeof( R ) >= sizeof( R1 ) )
        m_raw = ShiftLeft< R, F - F1 >( (R)(f.Raw()) );
    else
        m_raw = (R)(ShiftLeft< R1, F - F1 >( f.Raw() ));
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F>::FixedPoint( double f )
    :   m_raw( (R)(f * (double)((R)1 << F)) )
{
}

//=============================================================================

template < typename R, int F >
inline
R
FixedPoint<R,F>::Raw( ) const
{
    return m_raw;
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
int
FixedPoint<R,F>::ToInt( ) const
{
    return (int)(m_raw >> F);
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
float 
FixedPoint<R,F>::ToFloat( ) const
{
    return ((float)m_raw) / ((float)((R)1 << F));
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
bool 
FixedPoint<R,F>::operator==( const FixedPoint & rhs ) const
{
    return (m_raw == rhs.m_raw);
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
bool 
FixedPoint<R,F>::operator!=( const FixedPoint & rhs ) const
{
    return (m_raw != rhs.m_raw);
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
bool 
FixedPoint<R,F>::operator<( const FixedPoint & rhs ) const
{
    return (m_raw < rhs.m_raw);
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
bool 
FixedPoint<R,F>::operator<=( const FixedPoint & rhs ) const
{
    return (m_raw <= rhs.m_raw);
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
bool 
FixedPoint<R,F>::operator>( const FixedPoint & rhs ) const
{
    return (m_raw > rhs.m_raw);
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
bool 
FixedPoint<R,F>::operator>=( const FixedPoint & rhs ) const
{
    return (m_raw >= rhs.m_raw);
}

//=============================================================================

template < typename R, int F >
inline
FixedPoint<R,F> & 
FixedPoint<R,F>::operator+=( const FixedPoint & rhs )
{
    m_raw += rhs.m_raw;
    return *this;
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
FixedPoint<R,F> & 
FixedPoint<R,F>::operator-=( const FixedPoint & rhs )
{
    m_raw -= rhs.m_raw;
    return *this;
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
FixedPoint<R,F> & 
FixedPoint<R,F>::operator*=( const FixedPoint & rhs )
{
    *this = *this * rhs;
    return *this;
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
FixedPoint<R,F> & 
FixedPoint<R,F>::operator/=( const FixedPoint & rhs )
{
    *this = *this / rhs;
    return *this;
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
FixedPoint<R,F> 
FixedPoint<R,F>::operator-( ) const
{
    return FixedPoint( -m_raw, true );
}

//=============================================================================

template < typename R, int F >
inline
int 
FixedPoint<R,F>::TotalBits( )
{
    return 8 * sizeof( R );
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
int 
FixedPoint<R,F>::FractionBits( )
{
    return F;
}

//=============================================================================

template < typename R, int F >
inline
FixedPoint<R,F> 
operator+( FixedPoint<R,F> f1, FixedPoint<R,F> f2 )
{
    f1 += f2;
    return f1;
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator+( FixedPoint<R,F> f1, int i2 )
{
    return FixedPoint<R,F>( f1.Raw() + (i2 << F), true );
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator+( int i1, FixedPoint<R,F> f2 )
{
    return FixedPoint<R,F>( (i1 << F) + f2.Raw(), true );
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
FixedPoint<R,F> 
operator-( FixedPoint<R,F> f1, FixedPoint<R,F> f2 )
{
    f1 -= f2;
    return f1;
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator-( FixedPoint<R,F> f1, int i2 )
{
    return FixedPoint<R,F>( f1.Raw() - (i2 << F), true );
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator-( int i1, FixedPoint<R,F> f2 )
{
    return FixedPoint<R,F>( (i1 << F) - f2.Raw(), true );
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
FixedPoint<R,F> 
operator*( FixedPoint<R,F> f1, FixedPoint<R,F> f2 )
{
    const int N = FixedPoint<R,F>::TotalBits();
    if ( F <= N/2 )
    {
        R r1 = f1.Raw();
        R r2 = f2.Raw();
        R i1 = r1 >> F;
        R p1 = r1 - (i1 << F);
        R i2 = r2 >> F;
        R p2 = r2 - (i2 << F);
        R r = (i1 * r2)  +  (p1 * i2)  +  ((p1 * p2) >> F);
        return FixedPoint<R,F>( r, true );
    }
    else
    {
        R r1 = f1.Raw();
        R r2 = f2.Raw();
        R u1 = r1 >> N/2;
        R l1 = r1 - (u1 << N/2);
        R u2 = r2 >> N/2;
        R l2 = r2 - (u2 << N/2);
        R r = ((u1 * u2) << (N - F))  +  ((u1 * l2) >> (F - N/2))
                +  ((l1 * u2) >> (F - N/2))  +  ((l1 * l2) >> F);
        return FixedPoint<R,F>( r, true );
    }
}

//.............................................................................

template < typename R, int F, typename L >
FixedPoint<R,F> 
operator*( FixedPoint<R,F> f1, FixedPoint<R,F> f2 )
{
    L prod = (L)f1.Raw() * f2.Raw();
    return FixedPoint<R,F>( (R)(prod >> F), true );
}

//.............................................................................

template< int F >
inline
FixedPoint<int16_t, F> 
operator*( FixedPoint<int16_t,F> f1, FixedPoint<int16_t,F> f2 )
{
    return operator*< int16_t, F, int32_t >( f1, f2 );
}

//.............................................................................

template< int F >
inline
FixedPoint<int32_t, F> 
operator*( FixedPoint<int32_t,F> f1, FixedPoint<int32_t,F> f2 )
{
    return operator*< int32_t, F, int64_t >( f1, f2 );
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator*( FixedPoint<R,F> f1, int i2 )
{
    return FixedPoint<R,F>( f1.Raw() * i2, true );
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator*( int i1, FixedPoint<R,F> f2 )
{
    return FixedPoint<R,F>( i1 * f2.Raw(), true );
}

//-----------------------------------------------------------------------------

template < typename R, int F >
inline
FixedPoint<R,F> 
operator/( FixedPoint<R,F> f1, FixedPoint<R,F> f2 )
{
    R r1 = f1.Raw();
    R r2 = f2.Raw();
    R r = (r1 << F/2) / (r2 >> F/2);
    return FixedPoint<R,F>( r, true );
}

//.............................................................................

template < typename R, int F, typename L >
FixedPoint<R,F> 
operator/( FixedPoint<R,F> f1, FixedPoint<R,F> f2 )
{
    const int N = FixedPoint<R,F>::TotalBits();
    R r1 = f1.Raw();
    R r2 = f2.Raw();
    R ci = r1 / r2;
    R cf = 0;
    L s = (L)(r1 - ci * r2) << N;
    L t = (L)r2 << (N - 1);
    for ( int i = 0; i < F; ++i )
    {
        cf <<= 1;
        s -= t;
        if ( s < 0 )
            s += t;
        else
            cf |= 1;
        s <<= 1;
    }
    R r = (ci << F) | cf;
    return FixedPoint<R,F>( r, true );
}

//.............................................................................

template< int F >
inline
FixedPoint<int16_t, F> 
operator/( FixedPoint<int16_t,F> f1, FixedPoint<int16_t,F> f2 )
{
    return operator/< int16_t, F, int32_t >( f1, f2 );
}

//.............................................................................

template< int F >
FixedPoint<int32_t, F> 
operator/( FixedPoint<int32_t,F> f1, FixedPoint<int32_t,F> f2 )
{
    return operator/< int32_t, F, int64_t >( f1, f2 );
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator/( FixedPoint<R,F> f1, int i2 )
{
    return FixedPoint<R,F>( f1.Raw() / i2, true );
}

//.............................................................................

template < typename R, int F >
inline
FixedPoint<R,F> 
operator/( int i1, FixedPoint<R,F> f2 )
{
    FixedPoint<R,F> f1( i1 );
    return f1 / f2;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FIXEDPOINT_HPP

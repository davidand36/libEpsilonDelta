#ifndef DIVMOD_HPP
#define DIVMOD_HPP
/*
  DivMod.hpp
  Copyright (C) 2007 David M. Anderson

  Utility functions for division and modulo with dependable signs.
  NOTES:
  1. Versions are provided for integer and floating point (real) types.
     The former have names like DivModF and DivF; the latter,
     DivModRF and DivRF.
  2. In the floating point functions, the quotient is a mathematical integer,
     but its type is still floating point.
  3. All versions guarantee that
     dividend = divisor * quotient  +  remainder
     (aside from floating-point rounding error),
     sign( quotient ) == sign( dividend / divisor )
     (considering 0 to have both signs), and
     abs( remainder ) < abs( divisor ).
  4. The ..P() functions guarantee that
     0 <= remainder < abs( divisor );
  5. The ..F() functions guarantee that
     quotient = floor( dividend / divisor )
     and
     sign( remainder ) == sign( divisor ).
  6. The ..C() functions guarantee that
     quotient = ceil( dividend / divisor )
     and
     sign( remainder ) == - sign( divisor ).
  7. The ..A() functions guarantee that
     abs( quotient ) <= abs( dividend / divisor )
     and
     sign( remainder ) == sign( dividend ).
*/


#include <cstdlib>
#include <cmath>
#include "Assert.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename T>
void DivModP( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivP( T dividend, T divisor );
template <typename T>
T ModP( T dividend, T divisor );
template <typename T>
void DivModRP( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivRP( T dividend, T divisor );
template <typename T>
T ModRP( T dividend, T divisor );

template <typename T>
void DivModF( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivF( T dividend, T divisor );
template <typename T>
T ModF( T dividend, T divisor );
template <typename T>
void DivModRF( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivRF( T dividend, T divisor );
template <typename T>
T ModRF( T dividend, T divisor );

template <typename T>
void DivModC( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivC( T dividend, T divisor );
template <typename T>
T ModC( T dividend, T divisor );
template <typename T>
void DivModRC( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivRC( T dividend, T divisor );
template <typename T>
T ModRC( T dividend, T divisor );

template <typename T>
void DivModA( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivA( T dividend, T divisor );
template <typename T>
T ModA( T dividend, T divisor );
template <typename T>
void DivModRA( T dividend, T divisor, T * pQuotient, T * pRemainder );
template <typename T>
T DivRA( T dividend, T divisor );
template <typename T>
T ModRA( T dividend, T divisor );

#ifdef DEBUG
bool TestDivMod( );
#endif


//*****************************************************************************
//*****************************************************************************

#if ( (-7 % 3 == 2) && (-7 % -3 == 2) )
#define NATIVE_DIVMODP
#elif ( (-7 % 3 == 2) && (-7 % -3 == -1) )
#define NATIVE_DIVMODF
#elif ( (-7 % 3 == -1) && (-7 % -3 == 2) )
#define NATIVE_DIVMODC
#elif ( (-7 % 3 == -1) && (-7 % -3 == -1) )
#define NATIVE_DIVMODA
#endif

//=============================================================================


template <typename T>
void
DivModP( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    Assert( divisor != 0 );
    T q, r;
    q = dividend / divisor;
    r = dividend % divisor;
#ifndef NATIVE_DIVMODP
    if ( r < 0 )
    {
        if ( divisor > 0 )
        {
            --q;
            r += divisor;
        }
        else
        {
            ++q;
            r -= divisor;
        }
    }
#endif
    Assert( divisor * q + r == dividend );
    Assert( (r >= 0) && (r < abs( divisor )) );
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
        *pRemainder = r;
}

//-----------------------------------------------------------------------------

template <typename T>
T
DivP( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODP
    return (dividend / divisor);
#else
    if ( (dividend >= 0) && (divisor >= 0) )
        return (dividend / divisor);
    T quotient;
    DivModP( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
T
ModP( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODP
    return (dividend % divisor);
#else
    if ( (dividend >= 0) && (divisor >= 0) )
        return (dividend % divisor);
    T remainder;
    DivModP( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
void 
DivModRP( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    T q = std::floor( dividend / divisor );
    if ( divisor < 0. )
        q += 1.;
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
    {
        *pRemainder = dividend - divisor * q;

        //Assert( divisor * q + *pRemainder == dividend );
        Assert( *pRemainder >= 0. );
        Assert( fabs( *pRemainder ) < fabs( divisor ) );
    }
}

//-----------------------------------------------------------------------------

template <typename T>
T 
DivRP( T dividend, T divisor )
{
    T quotient;
    DivModRP( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
}

//-----------------------------------------------------------------------------

template <typename T>
T 
ModRP( T dividend, T divisor )
{
    T remainder;
    DivModRP( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
}


//=============================================================================


template <typename T>
void
DivModF( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    Assert( divisor != 0 );
    T q, r;
    q = dividend / divisor;
    r = dividend % divisor;
#ifndef NATIVE_DIVMODF
    if ( ((r < 0) && (divisor > 0)) || ((divisor < 0) && (r > 0)) )
    {
        --q;
        r += divisor;
    }
#endif
    Assert( divisor * q + r == dividend );
    Assert( q <= (dividend/divisor) );
    Assert( abs( r ) < abs( divisor ) );
    Assert( ((divisor > 0) && (r >= 0)) || ((divisor < 0) && (r <= 0)) );
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
        *pRemainder = r;
}

//-----------------------------------------------------------------------------

template <typename T>
T
DivF( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODF
    return (dividend / divisor);
#else
    if ( (dividend >= 0) && (divisor >= 0) )
        return (dividend / divisor);
    T quotient;
    DivModF( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
T
ModF( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODF
    return (dividend % divisor);
#else
    if ( (dividend >= 0) && (divisor >= 0) )
        return (dividend % divisor);
    T remainder;
    DivModF( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
void 
DivModRF( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    Assert( divisor != 0. );
    T q = std::floor( dividend / divisor );
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
    {
        *pRemainder = dividend - divisor * q;

        //Assert( divisor * q + *pRemainder == dividend );
        Assert( fabs( *pRemainder ) < fabs( divisor ) );
        Assert( ((divisor > 0.) && (*pRemainder >= 0.))
                || ((divisor < 0.) && (*pRemainder <= 0.)) );
    }
}

//-----------------------------------------------------------------------------

template <typename T>
T 
DivRF( T dividend, T divisor )
{
    T quotient;
    DivModRF( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
}

//-----------------------------------------------------------------------------

template <typename T>
T 
ModRF( T dividend, T divisor )
{
    T remainder;
    DivModRF( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
}


//=============================================================================


template <typename T>
void
DivModC( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    Assert( divisor != 0 );
    T q, r;
    q = dividend / divisor;
    r = dividend % divisor;
#ifndef NATIVE_DIVMODC
    if ( ((r > 0) && (divisor > 0)) || ((r < 0) && (divisor < 0)) )
    {
        ++q;
        r -= divisor;
    }
#endif
    Assert( divisor * q + r == dividend );
    Assert( (dividend/divisor) <= q );
    Assert( abs( r ) < abs( divisor ) );
    Assert( ((divisor > 0) && (r <= 0)) || ((divisor < 0) && (r >= 0)) );
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
        *pRemainder = r;
}

//-----------------------------------------------------------------------------

template <typename T>
T
DivC( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODC
    return (dividend / divisor);
#else
    T quotient;
    DivModC( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
T
ModC( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODC
    return (dividend % divisor);
#else
    T remainder;
    DivModC( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
void 
DivModRC( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    Assert( divisor != 0. );
    T q = std::ceil( dividend / divisor );
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
    {
        *pRemainder = dividend - divisor * q;

        //Assert( divisor * q + *pRemainder == dividend );
        Assert( fabs( *pRemainder ) < fabs( divisor ) );
        Assert( ((divisor > 0.) && (*pRemainder <= 0.))
                || ((divisor < 0.) && (*pRemainder >= 0.)) );
    }
}

//-----------------------------------------------------------------------------

template <typename T>
T 
DivRC( T dividend, T divisor )
{
    T quotient;
    DivModRC( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
}

//-----------------------------------------------------------------------------

template <typename T>
T 
ModRC( T dividend, T divisor )
{
    T remainder;
    DivModRC( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
}

//=============================================================================


template <typename T>
void
DivModA( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    Assert( divisor != 0 );
    T q, r;
    q = dividend / divisor;
    r = dividend % divisor;
#ifndef NATIVE_DIVMODA 
   if ( (r < 0) && (dividend >= 0) )
    {
        if ( divisor > 0 )
        {
            --q;
            r += divisor;
        }
        else
        {
            ++q;
            r -= divisor;
        }
    }
    else if ( (dividend <= 0) && (r > 0) )
    {
        if ( divisor > 0 )
        {
            ++q;
            r -= divisor;
        }
        else
        {
            --q;
            r += divisor;
        }
    }
#endif
    Assert( divisor * q + r == dividend );
    Assert( abs( q ) <= abs( dividend/divisor ) );
    Assert( abs( r ) < abs( divisor ) );
    Assert( ((dividend >= 0) && (r >= 0)) || ((dividend < 0) && (r <= 0)) );
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
        *pRemainder = r;
}

//-----------------------------------------------------------------------------

template <typename T>
T
DivA( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODA
    return (dividend / divisor);
#else
    T quotient;
    DivModA( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
T
ModA( T dividend, T divisor )
{
#ifdef NATIVE_DIVMODA
    return (dividend % divisor);
#else
    T remainder;
    DivModA( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
#endif
}

//-----------------------------------------------------------------------------

template <typename T>
void 
DivModRA( T dividend, T divisor, T * pQuotient, T * pRemainder )
{
    Assert( divisor != 0. );
    T q;
    T r = divisor * std::modf( (dividend / divisor), &q );
    if ( pQuotient )
        *pQuotient = q;
    if ( pRemainder )
    {
        *pRemainder = r;

        //Assert( divisor * q + *pRemainder == dividend );
        Assert( fabs( q ) <= fabs( dividend / divisor ) );
        Assert( fabs( r ) < fabs( divisor ) );
        Assert( ((dividend >= 0.) && (r >= 0.))
                || ((dividend < 0.) && (r <= 0.)) );
    }
}

//-----------------------------------------------------------------------------

template <typename T>
T 
DivRA( T dividend, T divisor )
{
    T quotient;
    DivModRA( dividend, divisor, &quotient, static_cast<T*>(0) );
    return quotient;
}

//-----------------------------------------------------------------------------

template <typename T>
T 
ModRA( T dividend, T divisor )
{
    T remainder;
    DivModRA( dividend, divisor, static_cast<T*>(0), &remainder );
    return remainder;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DIVMOD_HPP

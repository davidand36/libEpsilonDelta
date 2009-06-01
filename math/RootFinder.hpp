/*
  RootFinder.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for finding zeros of functions.
  NOTES:
  1. These routines are expressed as templates, and generally expect a
     floating-point type T and a function or function object (functor)
     as specified. These routines only find real roots.
  2. The root-finding algorithms, such as Brent and NewtonRaphson, expect
     two initial values, x0 and x1, which bracket a suspected root, in the
     sense that func(x0) and func(x1) have opposite signs.
     The routine BracketRoot takes an initial guess at a bracket and expands
     outwards until bracketing is acheived or it gives up. It returns true
     if successful.
  3. Brent (the Van Wijngaarden-Decker-Brent algorithm) is an excellent
     general-purpose root finder. At each iteration, it chooses between
     bisection and interpolation and is claimed to converge as long as the
     bracket contains a root.
     Brent expects a function of the form  T func( T arg ). If it finds a
     root, it returns true and passes the root to *pRoot. Otherwise it
     returns false.
  4. NewtonRaphson expects a function of the form
     void func( T arg, T * pVal, T * pDeriv ),
     which computes both the value of the function and its derivative at arg.
     This is a "safe" variant which reverts to bisection whenever the
     Newton-Raphson extrapolation would take it out of bounds or converge
     too slowly.
  5. Linear, Quadratic, and Cubic solve polynomial equations of the form
     c0 + c1*x = 0,  c0 + c1*x + c2*x^2 = 0, c0 + c1*x + c2*x^2 + c3*x^3 = 0,
     respectively, for real roots, which are returned in a vector.
     PolynomialR<T>::RealRoots() calls these when appropriate.
  6. Most algorithms are from W.H. Press, et al., "Numerical Recipes in C".
*/


#ifndef ROOTFINDER_HPP
#define ROOTFINDER_HPP


#include "Assert.hpp"
#include <limits>
#include <vector>
#include <cmath>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace RootFinder
{                                                        //namespace RootFinder

template <typename T, typename F>
bool BracketRoot( F func, T * pX0, T * pX1 );
template <typename T, typename F>
bool Brent( F func, T * pRoot, T x0, T x1, T accuracy = 1e-6 );
template <typename T, typename F>
bool NewtonRaphson( F func, T * pRoot, T x0, T x1, T accuracy = 1e-6 );
template <typename T>
std::vector<T> Linear( T c0, T c1 );
template <typename T>
std::vector<T> Quadratic( T c0, T c1, T c2 );
template <typename T>
std::vector<T> Cubic( T c0, T c1, T c2, T c3 );


#ifdef DEBUG
bool Test( );
#endif

}                                                        //namespace RootFinder


//*****************************************************************************


template <typename T, typename F>
bool 
RootFinder::BracketRoot( F func, T * pX0, T * pX1 )
{
    const int kMaxTries = 50;
    const T kFactor = static_cast<T>( 1.6 );
    Assert( (pX0 != 0) && (pX1 != 0) );
    Assert( *pX0 != *pX1 );
    T y0 = func( *pX0 );
    T y1 = func( *pX1 );
    for ( int i = 0; i < kMaxTries; ++i )
    {
        if ( ((y0 <= 0) && (y1 >= 0)) || ((y0 >= 0) && (y1 <= 0)) )
            return true;
        if ( fabs( y0 ) < fabs( y1 ) )
            y0 = func( *pX0 += kFactor * (*pX0 - *pX1) );
        else
            y1 = func( *pX1 += kFactor * (*pX1 - *pX0) );
    }
    return false;
}

//=============================================================================

template <typename T, typename F>
bool 
RootFinder::Brent( F func, T * pRoot, T x0, T x1, T accuracy )
{                                                                     /*Brent*/
    const int kMaxIter = 100;
    const T epsilon = ((std::numeric_limits<T>::epsilon() < 1)
        ?  std::numeric_limits<T>::epsilon()  :  static_cast<T>( 3.e-8 ));
    T y0 = func( x0 );
    T y1 = func( x1 );
    Assert( ((y0 <= 0) && (y1 >= 0)) || ((y0 >= 0) && (y1 <= 0)) );
    T x2 = x1;
    T y2 = y1;
    for ( int iter = 0; iter < kMaxIter; ++iter )
    {
        T adj = 0;
        T xdiff = 0;
        if ( ((y1 <= 0) && (y2 <= 0)) || ((y1 >= 0) && (y2 >= 0))  )
        {
            x2 = x0;
            y2 = y0;
            xdiff = adj = x1 - x0;
        }
        if ( fabs( y2 ) < fabs( y1 ) )
        {   //make x1 our best guess so far
            x0 = x1;
            x1 = x2;
            x2 = x0;
            y0 = y1;
            y1 = y2;
            y2 = y0;
        }
        T tol = static_cast<T>( 2 ) * epsilon * fabs( x1 )
                +  static_cast<T>( 0.5 ) * accuracy;
        T xm = static_cast<T>( 0.5 ) * (x2 - x1);
        if ( (y1 == 0) || (fabs( xm ) <= tol) )
        {
            *pRoot = x1;
            return true;    //root found
        }
        if ( (fabs( xdiff ) >= tol) && (fabs( y0 ) > fabs( y1 )) )
        {   //attempt interpolation
            T p;
            T q;
            T s = y1 / y0;
            if ( x0 == x2 )
            {   //linear interpolation
                p = 2 * xm * s;
                q = 1 - s;
            }
            else
            {   //inverse quadratic interpolation
                q = y0 / y2;
                T r = y1 / y2;
                p = s * (2 * xm * q * (q - r)  -  (x1 - x0) * (r - 1));
                q = (q - 1) * (r - 1) * (s - 1);
            }
            if ( p > 0 )
                q = -q;
            else
                p = -p;
            T min1 = 3 * xm * q  -  fabs( tol * q );
            T min2 = fabs( xdiff * q );
            if ( 2 * p < std::min( min1, min2 ) )
            {
                xdiff = adj;
                adj = p / q;
            }
            else    //interpolation failed: use bisection
                xdiff = adj = xm;
        }
        else    //bounds decreasing too slowly: use bisection
            xdiff = adj = xm;
        x0 = x1;
        y0 = y1;
        if ( fabs( adj ) > tol )
            x1 += adj;
        else
            x1 += ((xm > 0)  ?  fabs( tol )  :  - fabs( tol ));
        y1 = func( x1 );
    }
    return false;   //root not found
}                                                                     /*Brent*/

//=============================================================================

template <typename T, typename F>
bool 
RootFinder::NewtonRaphson( F func, T * pRoot, T x0, T x1, T accuracy )
{                                                             /*NewtonRaphson*/
    const int kMaxIter = 100;
    T yL;
    T yH;
    T deriv;
    func( x0, &yL, &deriv );
    func( x1, &yH, &deriv );
    Assert( ((yL <= 0) && (yH >= 0)) || ((yL >= 0) && (yH <= 0)) );
    T xL;
    T xH;
    if ( yL == 0 )
    {
        *pRoot = x0;
        return true;    //root already found
    }
    if ( yH == 0 )
    {
        *pRoot = x1;
        return true;    //root already found
    }
    if ( yL < 0 )   //arrange that f(xL)<0
    {
        xL = x0;
        xH = x1;
    }
    else
    {
        xL = x1;
        xH = x0;
    }
    T root = static_cast<T>( 0.5 ) * (xL + xH);
    T dxOld = fabs( xH - xL );
    T dx = dxOld;
    T yR;
    func( root, &yR, &deriv );
    for ( int iter = 0; iter < kMaxIter; ++iter )
    {
        if ( ( ((root - xL) * deriv - yR) * ((root - xH) * deriv - yR) >= 0 )
             || ( fabs( 2 * yR ) > fabs( dxOld * deriv ) ) )
        {   //Newton out of range or convergence too slow: use bisection
            dxOld = dx;
            dx = static_cast<T>( .5 ) * (xH - xL);
            root = xL + dx;
            if ( root == xL )
            {
                *pRoot = root;
                return true;    //root found
            }
        }
        else
        {   //use Newton
            dxOld = dx;
            dx = - yR / deriv;
            T oldRoot = root;
            root += dx;
            if ( root == oldRoot )
            {
                *pRoot = root;
                return true;    //root found
            }
        }
        if ( fabs( dx ) < accuracy )
        {
            *pRoot = root;
            return true;        //root found
        }
        func( root, &yR, &deriv );
        if ( yR < 0 )
            xL = root;
        else
            xH = root;
    }
    return false;   //root not found
}                                                             /*NewtonRaphson*/

//=============================================================================

template <typename T>
std::vector<T> 
RootFinder::Linear( T c0, T c1 )
{
    std::vector<T> roots;
    if ( c1 != 0 )
        roots.push_back( - c0 / c1 );
    //else the polynomial is just a constant with zero or infinitely many roots
    return roots;
}

//-----------------------------------------------------------------------------

template <typename T>
std::vector<T>
RootFinder::Quadratic( T c0, T c1, T c2 )
{
    if ( c2 == 0 )
        return Linear( c0, c1 );
    std::vector<T> roots;
    T discriminant = c1 * c1  -  4 * c0 * c2;
    if ( discriminant < 0 )
        return roots;   //no real roots; two complex roots
    T q = (c1 > 0)  ?  (-0.5 * (c1 + sqrt( discriminant )))
                    :  (-0.5 * (c1 - sqrt( discriminant )));
    roots.push_back( q / c2 );
    if ( q != 0 )
        roots.push_back( c0 / q );
    return roots;
}

//-----------------------------------------------------------------------------

template <typename T>
std::vector<T> 
RootFinder::Cubic( T c0, T c1, T c2, T c3 )
{
    if ( c3 == 0 )
        return Quadratic( c0, c1, c2 );
    std::vector<T> roots;
    c0 /= c3;   //reduce to c3 = 1
    c1 /= c3;
    c2 /= c3;
    T q = (c2 * c2  -  3 * c1) / 9;
    T r = (2 * c2 * c2 * c2  -  9 * c1 * c2  +  27 * c0) / 54;
    T q3 = q * q * q;
    T r2 = r * r;
    if ( q3 >= r2 )
    {
        T theta3 = std::acos( r / sqrt( q3 ) ) / 3;
        T sqrtq_2 = -2 * sqrt( q );
        T c2_3 = c2 / 3;
        roots.push_back( sqrtq_2 * cos( theta3 ) - c2_3 );
        roots.push_back( sqrtq_2 * cos( theta3 + 2 * M_PI / 3 ) - c2_3 );
        roots.push_back( sqrtq_2 * cos( theta3 + 4 * M_PI / 3 ) - c2_3 );
    }
    else
    {
        T a = (r > 0 )  ?  - std::pow( (sqrt( r2 - q3 ) + r),
                                       static_cast<T>(1./3.) )
                        :    std::pow( (sqrt( r2 - q3 ) - r),
                                       static_cast<T>(1./3.) );
        T b = (a == 0 )  ?  0  :  q / a;
        roots.push_back( a + b - c2 / 3 );
        //only one real root; two complex roots
    }
    return roots;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ROOTFINDER_HPP

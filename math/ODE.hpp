#ifndef ODE_HPP
#define ODE_HPP
/*
  ODE.hpp
  Copyright (C) 2009 David M. Anderson

  Routines for solving ordinary differential equations.
  NOTES:
  1. These routines are expressed as templates, and generally expect a
     floating-point type T, the dimension N (the number of dependent scalar
     variables), and a function or function object (functor) type F.
  2. The derivFunc function is of the form
     void derivFunc( T x, const Vector<T,N> & y, Vector<T,N> * pDeriv ).
  3. RungaKutta4() is a fourth-order (i.e. per-step errors of order
     O( deltaX ^ 5 )) stepper using a fixed-size step. It is a good, fast
     choice for most ODEs.
*/


#include "Vector.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace ODE
{                                                               //namespace ODE

template <typename T, int N, typename F>
void RungeKutta4( T * pX, Vector<T,N> * pY, F derivFunc, T deltaX );

#ifdef DEBUG
bool Test( );
#endif

}                                                               //namespace ODE


//*****************************************************************************


template <typename T, int N, typename F>
void 
ODE::RungeKutta4( T * pX, Vector<T,N> * pY, F derivFunc, T deltaX )
{
    Vector<T,N> d1;
    derivFunc( *pX, *pY, &d1 );
    Vector<T,N> d2;
    derivFunc( (*pX + 0.5 * deltaX), (*pY  +  0.5 * deltaX * d1), &d2 );
    Vector<T,N> d3;
    derivFunc( (*pX + 0.5 * deltaX), (*pY  +  0.5 * deltaX * d2), &d3 );
    Vector<T,N> d4;
    derivFunc( (*pX + deltaX), (*pY  +  deltaX * d3), &d4 );
    *pX += deltaX;
    *pY += deltaX * (d1  +  2. * d2  +  2. * d3  +  d4) * (1./6.);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ODE_HPP

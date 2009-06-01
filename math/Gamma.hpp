#ifndef GAMMA_HPP
#define GAMMA_HPP
/*
  Gamma.hpp
  Copyright (C) 2007 David M. Anderson

  Gamma and related functions.
  Gamma: the integral over 0 <= t < infinity of t^(a-1) * exp(-t),
      a continuous function with the property that
      Gamma(n+1) = n * Gamma(n), and so Gamma(n) = (n-1)! for integral n.
  LogGamma: log of Gamma(a). Use of this helps avoid overflow.
  IncompleteGamma: integral over 0 <= t <= x of t^(a-1) * exp(-t), divided by
      Gamma(a).
  Beta: the integral over 0 <= t <= 1 of t^(a-1) * (1-t)^(b-1).
      Beta(a,b) = Gamma(a) * Gamma(b) / Gamma(a+b).
  IncompleteBeta: integral over 0 <= t <= x of t^(a-1) * (1-t)^(b-1),
      divided by Beta(a,b).
  Erf: 2 / sqrt(pi) * the integral over 0 <= t <= x of exp( -x^2 ).
      The error function. Erf(x) = +/- Gamma( 0.5, x^2 ).
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


double Gamma( double a );
double LogGamma( double a );
double IncompleteGamma( double a, double x );
double Beta( double a, double b );
double IncompleteBeta( double a, double b, double x );
double Erf( double x );

#ifdef DEBUG
bool TestGamma( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GAMMA_HPP

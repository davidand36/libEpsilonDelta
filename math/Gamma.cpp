/*
  Gamma.cpp
  Copyright (C) 2007 David M. Anderson

  Gamma and related functions.
*/


#include "Gamma.hpp"
#include "Polynomial.hpp"
#include "Assert.hpp"
#include "Exception.hpp"
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

double
Gamma( double a )
{
    return exp( LogGamma( a ) );
}

//=============================================================================

double 
LogGamma( double a )
{
    //Approximation due to Lanczos, as given in Press, et al.,
    // "Numerical Recipes in C++", 2nd ed., p. 218.
    Assert( a > 0. );
    static const double logSqrt2pi = log( 2. * M_PI ) / 2.;
    static const double c0 = 1.000000000190015;
    static const double cs[6]
            = { 76.18009172947146, -86.50532032941677,
                24.01409824083091, -1.231739572450155,
                0.1208650973866179e-2, -0.5395239384953e-5 };
    double b = a + 4.5;
    double s = c0;
    double y = a;
    for ( int i = 0; i < 6; ++i )
    {
        s += cs[i] / y;
        y += 1.;
    }
    return  log( b ) * (a - 0.5)  -  b  +  logSqrt2pi  +  log( s );
}

//=============================================================================

double 
IncompleteGamma( double a, double x )
{
    Assert( a > 0. );
    Assert( x >= 0. );
    const double epsilon = numeric_limits< double >::epsilon();
    if ( x < a + 1. )
    {
        //series approximation
        double term = 1. / a;
        double sum = term;
        double aa = a;
        for ( int i = 0; i < 1000; ++i )
        {
            aa += 1.;
            term *= x / aa;
            sum += term;
            if ( fabs( term )
                 < fabs( sum ) * epsilon )
                return sum * exp( -x  +  a * log( x )  -  LogGamma( a ) );
        }
        throw Exception( "IncompleteGamma series failed to converge." );
    }
    else //x >= a + 1.
    {
        //continued fraction approximation
        const double tiny = numeric_limits< double >::min() / epsilon;
        double b = x - a + 1.;
        double c = 1. / tiny  +  b;
        double d = b;
        if ( fabs( d ) < tiny )
            d = tiny;
        d = 1. / d;
        double f = d;
        for ( int i = 1; i < 1000; ++i )
        {
            double ai = -i * (i - a);
            b += 2.;
            c = ai / c  +  b;
            if ( fabs( c ) < tiny )
                c = tiny;
            d = ai * d  +  b;
            if ( fabs( d ) < tiny )
                d = tiny;
            d = 1. / d;
            double delta = c * d;
            f *= delta;
            if ( fabs( delta - 1. ) < epsilon )
                return 1.  -  f * exp( -x  +  a * log( x )  -  LogGamma( a ) );
        }
        throw Exception( "IncompleteGamma continued fraction"
                         " failed to converge." );
    }
}

//=============================================================================

double 
Beta( double a, double b )
{
    Assert( a > 0. );
    Assert( b > 0. );
    return exp( LogGamma( a ) + LogGamma( b ) - LogGamma( a + b ) );
}

//=============================================================================

double 
IncompleteBeta( double a, double b, double x )
{
    Assert( a > 0. );
    Assert( b > 0. );
    Assert( (x >= 0.) && (x <= 1.) );
    if ( x <= 0. )
        return 0.;
    if ( x >= 1. )
        return 1.;
    if ( x > (a + 1.) / (a + b + 2.) )
        return 1. - IncompleteBeta( b, a, 1. - x );

    const double epsilon = numeric_limits< double >::epsilon();
    const double tiny = numeric_limits< double >::min() / epsilon;
    //continued fraction approximation
    const double ap1 = a + 1.;
    const double am1 = a - 1.;
    const double apb = a + b;
    double c = 1.;
    double d = 1. - apb * x / ap1;
    if ( fabs( d ) < tiny )
        d = tiny;
    d = 1. / d;
    double f = d;
    for ( int i = 1; i < 5000; ++i )
    {
        double i2 = 2. * i;
        //even step
        double aa = i * (b - i) * x / ((a + i2) * (am1 + i2));
        c = aa / c  +  1.;
        if ( fabs( c ) < tiny )
            c = tiny;
        d = aa * d  +  1.;
        if ( fabs( d ) < tiny )
            d = tiny;
        d = 1. / d;
        f *= c * d;
        //odd step
        aa = - (a + i) * (apb + i) * x / ((a + i2) * (ap1 + i2));
        c = aa / c  +  1.;
        if ( fabs( c ) < tiny )
            c = tiny;
        d = aa * d  +  1.;
        if ( fabs( d ) < tiny )
            d = tiny;
        d = 1. / d;
        double delta = c * d;
        f *= delta;
        if ( fabs( delta - 1. ) < epsilon )
            return f / a * exp( a * log( x )  +  b * log( 1. - x )
                        + LogGamma( a + b ) - LogGamma( a ) - LogGamma( b ) );
    }
    throw Exception( "IncompleteBeta continued fraction failed to converge." );
}

//=============================================================================

double 
Erf( double x )
{
    //See Press, et al., "Numerical Recipes in C++", 2nd ed., p. 226.
    static const Polynomial< double >
            poly( 9, -1.26551223, 1.00002368, 0.37409196, 0.09678418,
                  -0.18628806, 0.27886807, -1.13520398, 1.48851587,
                  -0.82215223, 0.17087277 );
    double y = fabs( x );
    double z = 1. / (1. + 0.5 * y);
    double e = z * exp( -y * y  +  poly( z ) );
    if ( x >= 0. )
        return 1. - e;
    else
        return e - 1.;
}

//=============================================================================


#ifdef DEBUG

bool 
TestGamma( )
{
    bool ok = true;
    cout << "Testing Gamma" << endl;

    TESTCHECKF( Gamma( 1.461 ), 0.885604, &ok );
    TESTCHECKFE( Gamma( 2.5 ), 1.5 * 0.886228, &ok, 2.e-6 );
    TESTCHECKF( IncompleteGamma( 2.4, 4.2 ), 0.8774251114, &ok );
    TESTCHECKF( IncompleteGamma( 3.4, 4.3 ), 0.7353098873, &ok );
    TESTCHECKF( IncompleteGamma( 5.4, 4.5 ), 0.3954640487, &ok );
    TESTCHECKF( Beta( 0.1, 0.2 ), 14.599371492765, &ok );
    TESTCHECKF( Beta( 3.2, 2.3 ), 0.054029791748, &ok );
    TESTCHECKF( IncompleteBeta( 4., 10., 0.2 ), 0.2526756905, &ok );
    TESTCHECKF( IncompleteBeta( 4., 10., 0.8 ), 0.9999839396, &ok );
    TESTCHECKF( IncompleteBeta( 10., 4., 0.3 ), 0.0006519600, &ok );
    TESTCHECKF( IncompleteBeta( 10., 4., 0.9 ), 0.9658392791, &ok );
    TESTCHECKF( Erf( 0. ), 0., &ok );
    TESTCHECKF( Erf( 0.48 ), 0.5027496707, &ok );
    TESTCHECKF( Erf( 1.0 ), 0.8427007929, &ok );
    TESTCHECKF( Erf( -1.57 ), -0.9736026275, &ok );

    if ( ok )
        cout << "Gamma PASSED." << endl << endl;
    else
        cout << "Gamma FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

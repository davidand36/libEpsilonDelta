/*
  ProbabilityDistributions.cpp
  Copyright (C) 2007 David M. Anderson

  Density and distribution functions for common probability distributions.
*/


#include "ProbabilityDistributions.hpp"
#include "Factorial.hpp"
#include "Gamma.hpp"
#include "Angle.hpp"
#include "Assert.hpp"
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
Uniform_PDF( int x, int limit )
{
    if ( (x < 0) || (x >= limit) )
        return 0.;
    Assert( limit > 0 );
    return 1. / limit;
}

//-----------------------------------------------------------------------------

double 
Uniform_DF( int x, int limit )
{
    if ( x < 0 )
        return 0.;
    if ( x >= limit - 1 )
        return 1.;
    Assert( limit > 0 );
    return (x + 1.) / limit;
}

//=============================================================================

double 
Uniform_PDF( double x, double minimum, double maximum )
{
    if ( (x < minimum) || (x >= maximum) )
        return 0.;
    Assert( minimum < maximum );
    return 1. / (maximum - minimum);
}

//-----------------------------------------------------------------------------

double 
Uniform_DF( double x, double minimum, double maximum )
{
    if ( x <= minimum )
        return 0.;
    if ( x >= maximum )
        return 1.;
    Assert( minimum < maximum );
    return (x - minimum) / (maximum - minimum);
}

//=============================================================================

double 
Triangle_PDF( double x, double minimum, double mode, double maximum )
{
    Assert( minimum < maximum );
    Assert( (minimum <= mode) && (mode <= maximum) );
    double peak = 2. / (maximum - minimum);
    if ( x == mode )
        return peak;
    if ( (x <= minimum) || (x >= maximum) )
        return 0.;
    if ( x <= mode )
        return peak * (x - minimum) / (mode - minimum);
    else
        return peak * (maximum - x) / (maximum - mode);
}

//-----------------------------------------------------------------------------

double 
Triangle_DF( double x, double minimum, double mode, double maximum )
{
    Assert( minimum < maximum );
    Assert( (minimum <= mode) && (mode <= maximum) );
    if ( x <= minimum )
        return 0.;
    if ( x >= maximum )
        return 1.;
    double peak = 2. / (maximum - minimum);
    if ( x <= mode )
        return 0.5 * peak * (x - minimum) * (x - minimum) / (mode - minimum);
    else
        return 1.
              -  0.5 * peak * (maximum - x) * (maximum - x) / (maximum - mode);
}

//=============================================================================

double 
Binomial_PDF( int x, double probability, int trials )
{
    Assert( (probability >= 0.) && (probability <= 1.) );
    Assert( trials > 0 );
    if ( (x < 0) || (x > trials) )
        return 0.;
    return BinomialCoefficient( trials, x ) * pow( probability, x )
            * pow( (1. - probability), (trials - x) );
}

//-----------------------------------------------------------------------------

double 
Binomial_DF( int x, double probability, int trials )
{
    Assert( (probability >= 0.) && (probability <= 1.) );
    Assert( trials > 0 );
    if ( x < 0 )
        return 0.;
    if ( x >= trials )
        return 1.;
    if ( probability == 0. )
        return 1.;
    if ( probability == 1. )
        return ((x < trials)  ?  0.  :  1.);
    if ( trials <= 15 )
    {
        double prob = pow( (1. - probability), trials );
        double cumProb = prob;
        double odds = probability / (1. - probability);
        for ( int i = 1; i <= x; ++i )
        {
            prob *= odds * (trials - i + 1) / i;
            cumProb += prob;
        }
        return cumProb;
    }
    else
    {
        return 1. - IncompleteBeta( (x + 1), (trials - x), probability );
    }
}

//=============================================================================

double 
Geometric_PDF( int x, double probability )
{
    Assert( (probability >= 0.) && (probability <= 1.) );
    if ( x <= 0 )
        return 0.;
    if ( probability == 0. )
        return 0.;
    if ( probability == 1. )
        return ((x == 1)  ?  1.  :  0.);
    return probability * pow( (1. - probability), (x - 1.) );
}

//-----------------------------------------------------------------------------

double 
Geometric_DF( int x, double probability )
{
    Assert( (probability >= 0.) && (probability <= 1.) );
    if ( x <= 0 )
        return 0.;
    double failProb = 1. - probability;
    double prob = probability;
    double cumProb = probability;
    for ( int i = 2; i <= x; ++i )
    {
        prob *= failProb;
        cumProb += prob;
    }
    return cumProb;
}

//=============================================================================

double 
Hypergeometric_PDF( int x, int populationSize, int subsetSize, int sampleSize )
{
    Assert( subsetSize >= 0 );
    Assert( subsetSize <= populationSize );
    Assert( sampleSize >= 0 );
    Assert( sampleSize <= populationSize );
    int minX = max( 0, subsetSize + sampleSize - populationSize );
    int maxX = min( subsetSize, sampleSize );
    if ( x < minX )
        return 0.;
    if ( x > maxX )
        return 0.;
    return BinomialCoefficient( subsetSize, x )
            * BinomialCoefficient( (populationSize - subsetSize),
                                   (sampleSize - x) )
            / BinomialCoefficient( populationSize, sampleSize );
}

//-----------------------------------------------------------------------------

double 
Hypergeometric_DF( int x, int populationSize, int subsetSize, int sampleSize )

{
    Assert( subsetSize >= 0 );
    Assert( subsetSize <= populationSize );
    Assert( sampleSize >= 0 );
    Assert( sampleSize <= populationSize );
    int minX = max( 0, subsetSize + sampleSize - populationSize );
    int maxX = min( subsetSize, sampleSize );
    if ( x < minX )
        return 0.;
    if ( x >= maxX )
        return 1.;
    double prob = 0.;
    if ( minX == 0 )
        prob = exp( LogFactorial( populationSize - subsetSize )
                    + LogFactorial( populationSize - sampleSize )
                    - LogFactorial( populationSize )
                    - LogFactorial( populationSize - subsetSize
                                    - sampleSize ) );
    else
        prob = exp( LogFactorial( subsetSize )
                    + LogFactorial( sampleSize )
                    - LogFactorial( populationSize )
                    - LogFactorial( minX ) );
    double cumProb = prob;
    for ( int i = minX + 1; i <= x; ++i )
    {
        prob *= (subsetSize - i + 1.) * (sampleSize - i + 1.)
                / (i * (populationSize - subsetSize - sampleSize + i));
        cumProb += prob;
    }
    return cumProb;
}

//=============================================================================

double 
Poisson_PDF( int x, double mean )
{
    Assert( mean >= 0. );
    if ( x < 0 )
        return 0.;
    return exp( - mean  +  x * log( mean )  -  LogFactorial( x ) );
}

//-----------------------------------------------------------------------------

double 
Poisson_DF( int x, double mean )
{
    Assert( mean >= 0. );
    if ( x < 0 )
        return 0.;
    return 1. - IncompleteGamma( (x + 1.), mean );
}

//=============================================================================

double 
Exponential_PDF( double x, double lambda )
{
    Assert( lambda > 0. );
    if ( x < 0. )
        return 0.;
    return lambda * exp( - lambda * x );
}

//-----------------------------------------------------------------------------

double 
Exponential_DF( double x, double lambda )
{
    Assert( lambda > 0. );
    if ( x < 0. )
        return 0.;
    return 1. - exp( - lambda * x );
}

//=============================================================================

double 
Gamma_PDF( double x, int n, double lambda )
{
    Assert( n > 0 );
    Assert( lambda > 0. );
    if ( x <= 0. )
        return 0.;
    return exp( (n - 1.) * log( x )  -  x * lambda  +  n * log( lambda )
                -  LogGamma( n ) );
}

//-----------------------------------------------------------------------------

double 
Gamma_DF( double x, int n, double lambda )
{
    Assert( n > 0 );
    Assert( lambda > 0. );
    if ( x <= 0. )
        return 0.;
    return IncompleteGamma( n, x * lambda );
}

//=============================================================================

double 
Normal_PDF( double x, double mean, double standardDeviation )
{
    Assert( standardDeviation > 0. );
    static const double sqrt2pi = sqrt( 2 * M_PI );
    return exp( - (x - mean)*(x - mean)
                / (2 * standardDeviation * standardDeviation) )
            / (standardDeviation * sqrt2pi);
}

//-----------------------------------------------------------------------------

double 
Normal_DF( double x, double mean, double standardDeviation )
{
    Assert( standardDeviation > 0. );
    static const double sqrt2 = sqrt( 2. );
    return 0.5 * (1. + Erf( (x - mean) / (sqrt2 * standardDeviation) ));
}

//=============================================================================

//This is also used in Random.cpp, so don't make it private.
void ConvertLogNormalMoments( double mean, double stdDev,
                              double * meanOfLog, double * stdDevOfLog );

//-----------------------------------------------------------------------------

double 
LogNormal_PDF( double x, double mean, double standardDeviation,
               bool momentsOfLog )
{
    Assert( standardDeviation > 0. );
    if ( x <= 0. )
        return 0.;
    if ( momentsOfLog )
    {
        return Normal_PDF( log( x ), mean, standardDeviation );
    }
    else
    {
        Assert( mean > 0. );
        double meanOfLog, stdDevOfLog;
        ConvertLogNormalMoments( mean, standardDeviation,
                                 &meanOfLog, &stdDevOfLog );
        return Normal_PDF( log( x ), meanOfLog, stdDevOfLog );
    }
}

//-----------------------------------------------------------------------------

double 
LogNormal_DF( double x, double mean, double standardDeviation,
              bool momentsOfLog )
{
    Assert( standardDeviation > 0. );
    if ( x <= 0. )
        return 0.;

    if ( momentsOfLog )
    {
        return Normal_DF( log( x ), mean, standardDeviation );
    }
    else
    {
        Assert( mean > 0. );
        double meanOfLog, stdDevOfLog;
        ConvertLogNormalMoments( mean, standardDeviation,
                                 &meanOfLog, &stdDevOfLog );
        return Normal_DF( log( x ), meanOfLog, stdDevOfLog );
    }
}

//-----------------------------------------------------------------------------

void
ConvertLogNormalMoments( double mean, double stdDev,
                         double * meanOfLog, double * stdDevOfLog )
{
    double meanSqr = mean * mean;
    double variance = stdDev * stdDev;
    double v = log( 1.  +  variance / meanSqr );
    if ( meanOfLog )
        *meanOfLog = log( mean ) - 0.5 * v;
    if ( stdDevOfLog )
        *stdDevOfLog = sqrt( v );
}

//=============================================================================

double 
ChiSquare_PDF( double x, int degreesOfFreedom )
{
    Assert( degreesOfFreedom > 0 );
    if ( x <= 0. )
        return 0.;
    double dof2 = degreesOfFreedom / 2.;
    static const double logHalf = log( 0.5 );
    return exp( dof2 * logHalf  +  (dof2 - 1.) * log( x )  -  x / 2.
                -  LogGamma( dof2 ) );
}

//-----------------------------------------------------------------------------

double 
ChiSquare_DF( double x, int degreesOfFreedom )
{
    Assert( degreesOfFreedom > 0 );
    if ( x <= 0. )
        return 0.;
    return IncompleteGamma( (degreesOfFreedom / 2.), (x / 2.) );
}

//=============================================================================

double 
StudentsT_PDF( double x, int degreesOfFreedom )
{
    Assert( degreesOfFreedom > 0 );
    double d = degreesOfFreedom;
    static const double logSqrtPi = log( M_PI ) / 2.;
    return exp( LogGamma( (d + 1.) / 2. )  -  LogGamma( d / 2. )
                -  log( d ) / 2.  -  logSqrtPi
                -  ((d + 1.) / 2.) * log( 1. +  x * x / d ) );
}

//-----------------------------------------------------------------------------

double 
StudentsT_DF( double x, int degreesOfFreedom )
{
    Assert( degreesOfFreedom > 0 );
    double d = degreesOfFreedom;
    if ( x > 0. )
        return 1.  -  0.5 * IncompleteBeta( (d / 2.), 0.5, (d / (d + x * x)) );
    else if ( x < 0. )
        return 0.5 * IncompleteBeta( (d / 2.), 0.5, (d / (d + x * x)) );
    else
        return 0.5;
}

//=============================================================================

double 
F_PDF( double x, int dof1, int dof2 )
{
    Assert( (dof1 > 0) && (dof2 > 0) );
    if ( x < 0. )
        return 0.;
    double a = (dof1 * x) / (dof1 * x  +  dof2);
    return pow( a, dof1 / 2. ) * pow( (1. - a), dof2 / 2. )
            / (x * Beta( dof1 / 2., dof2 / 2. ));
}

//-----------------------------------------------------------------------------

double 
F_DF( double x, int dof1, int dof2 )
{
    Assert( (dof1 > 0) && (dof2 > 0) );
    if ( x <= 0. )
        return 0.;
    double a = (dof1 * x) / (dof1 * x  +  dof2);
    return IncompleteBeta( dof1 / 2., dof2 / 2., a );
}

//=============================================================================

double 
Cauchy_PDF( double x, double a )
{
    Assert( a > 0. );
    return a / (M_PI * (a * a  +  x * x));
}

//-----------------------------------------------------------------------------

double 
Cauchy_DF( double x, double a )
{
    Assert( a > 0. );
    return 0.5  +  ArcTan( x / a ).Radians() / M_PI;
}

//=============================================================================

double 
Beta_PDF( double x, double a, double b )
{
    Assert( a > 0. );
    Assert( b > 0. );
    if ( x <= 0. )
        return 0.;
    if ( x >= 1. )
        return 0.;
    return exp( (a - 1.) * log( x )  +  (b - 1.) * log( 1. - x )
                +  LogGamma( a + b )  -  LogGamma( a )  -  LogGamma( b ) );
}

//-----------------------------------------------------------------------------

double 
Beta_DF( double x, double a, double b )
{
    Assert( a > 0. );
    Assert( b > 0. );
    if ( x <= 0. )
        return 0.;
    if ( x >= 1. )
        return 1.;
    return IncompleteBeta( a, b, x );
}

//=============================================================================

double 
KolmogorovSmirnov_DF( double x )
{
    //Press, et al., "Numerical Recipes in C++", 2nd ed., p. 631.
    const double epsilon1 = 1.e-6;
    const double epsilon2 = 1.e-16;
    double x2 = -2. * x * x;
    double factor = 2.;
    double sum = 0.;
    double absPrevTerm = 0.;
    for ( int i = 1; i < 1000; ++i )
    {
        double term = factor * exp( x2 * i * i );
        sum += term;
        double absTerm = fabs( term );
        if ( (absTerm < epsilon1 * absPrevTerm)
             || (absTerm < epsilon2 * sum) )
            return 1. - sum;
        factor = - factor;
        absPrevTerm = absTerm;
    }
    //failed to converge
    return 1.;
}

//=============================================================================

#ifdef DEBUG

bool 
TestProbabilityDistributions( )
{
    bool ok = true;
    cout << "Testing Probability Distributions" << endl;

    TESTCHECK( Uniform_PDF( -1, 8 ), 0., &ok );
    TESTCHECK( Uniform_PDF( 0, 8 ), 0.125, &ok );
    TESTCHECK( Uniform_PDF( 6, 8 ), 0.125, &ok );
    TESTCHECK( Uniform_PDF( 7, 8 ), 0.125, &ok );
    TESTCHECK( Uniform_PDF( 8, 8 ), 0., &ok );
    TESTCHECK( Uniform_DF( -1, 8 ), 0., &ok );
    TESTCHECK( Uniform_DF( 0, 8 ), 0.125, &ok );
    TESTCHECK( Uniform_DF( 6, 8 ), 0.875, &ok );
    TESTCHECK( Uniform_DF( 7, 8 ), 1., &ok );
    TESTCHECK( Uniform_DF( 8, 8 ), 1., &ok );

    TESTCHECK( Uniform_PDF( 1., 1.5, 3.5 ), 0., &ok );
    TESTCHECK( Uniform_PDF( 2., 1.5, 3.5 ), 0.5, &ok );
    TESTCHECK( Uniform_PDF( 3., 1.5, 3.5 ), 0.5, &ok );
    TESTCHECK( Uniform_PDF( 4., 1.5, 3.5 ), 0., &ok );
    TESTCHECK( Uniform_DF( 1., 1.5, 3.5 ), 0., &ok );
    TESTCHECK( Uniform_DF( 2., 1.5, 3.5 ), 0.25, &ok );
    TESTCHECK( Uniform_DF( 3., 1.5, 3.5 ), 0.75, &ok );
    TESTCHECK( Uniform_DF( 4., 1.5, 3.5 ), 1., &ok );

    TESTCHECK( Triangle_PDF( 1., 2., 2., 3. ), 0., &ok );
    TESTCHECK( Triangle_PDF( 2., 2., 2., 3. ), 2., &ok );
    TESTCHECK( Triangle_PDF( 2.5, 2., 2., 3. ), 1., &ok );
    TESTCHECK( Triangle_PDF( 3., 2., 2., 3. ), 0., &ok );
    TESTCHECK( Triangle_PDF( 4., 2., 2., 3. ), 0., &ok );
    TESTCHECK( Triangle_DF( 1., 2., 2., 3. ), 0., &ok );
    TESTCHECK( Triangle_DF( 2., 2., 2., 3. ), 0., &ok );
    TESTCHECK( Triangle_DF( 2.5, 2., 2., 3. ), 0.75, &ok );
    TESTCHECK( Triangle_DF( 3., 2., 2., 3. ), 1., &ok );
    TESTCHECK( Triangle_DF( 4., 2., 2., 3. ), 1., &ok );
    TESTCHECK( Triangle_PDF( 1., 2., 3., 4. ), 0., &ok );
    TESTCHECK( Triangle_PDF( 2., 2., 3., 4. ), 0., &ok );
    TESTCHECK( Triangle_PDF( 2.5, 2., 3., 4. ), 0.5, &ok );
    TESTCHECK( Triangle_PDF( 3., 2., 3., 4. ), 1., &ok );
    TESTCHECK( Triangle_PDF( 3.5, 2., 3., 4. ), 0.5, &ok );
    TESTCHECK( Triangle_PDF( 4., 2., 3., 4. ), 0., &ok );
    TESTCHECK( Triangle_PDF( 5., 2., 3., 4. ), 0., &ok );
    TESTCHECK( Triangle_DF( 1., 2., 3., 4. ), 0., &ok );
    TESTCHECK( Triangle_DF( 2., 2., 3., 4. ), 0., &ok );
    TESTCHECK( Triangle_DF( 2.5, 2., 3., 4. ), 0.125, &ok );
    TESTCHECK( Triangle_DF( 3., 2., 3., 4. ), 0.5, &ok );
    TESTCHECK( Triangle_DF( 3.5, 2., 3., 4. ), 0.875, &ok );
    TESTCHECK( Triangle_DF( 4., 2., 3., 4. ), 1., &ok );
    TESTCHECK( Triangle_DF( 5., 2., 3., 4. ), 1., &ok );
    TESTCHECK( Triangle_PDF( 1., 2., 3., 3. ), 0., &ok );
    TESTCHECK( Triangle_PDF( 2., 2., 3., 3. ), 0., &ok );
    TESTCHECK( Triangle_PDF( 2.5, 2., 3., 3. ), 1., &ok );
    TESTCHECK( Triangle_PDF( 3., 2., 3., 3. ), 2., &ok );
    TESTCHECK( Triangle_PDF( 4., 2., 3., 3. ), 0., &ok );
    TESTCHECK( Triangle_DF( 1., 2., 3., 3. ), 0., &ok );
    TESTCHECK( Triangle_DF( 2., 2., 3., 3. ), 0., &ok );
    TESTCHECK( Triangle_DF( 2.5, 2., 3., 3. ), 0.25, &ok );
    TESTCHECK( Triangle_DF( 3., 2., 3., 3. ), 1., &ok );
    TESTCHECK( Triangle_DF( 4., 2., 3., 3. ), 1., &ok );

    TESTCHECK( Binomial_PDF( -3, 0.5, 10 ), 0., &ok );
    TESTCHECKFE( Binomial_PDF( 4, 0.25, 13 ), 0.2097, &ok, 0.0001 );
    TESTCHECKFE( Binomial_PDF( 9, 0.4, 19 ), 0.1464, &ok, 0.0002 );
    TESTCHECK( Binomial_PDF( 20, 0.4, 19 ), 0., &ok );
    TESTCHECK( Binomial_DF( -1, 0.35, 13 ), 0., &ok );
    TESTCHECKFE( Binomial_DF( 0, 0.05, 13 ), 0.5133, &ok, 0.0001 );
    TESTCHECKFE( Binomial_DF( 4, 0.35, 13 ), 0.5005, &ok, 0.0001 );
    TESTCHECKFE( Binomial_DF( 9, 0.35, 13 ), 0.9975, &ok, 0.0001 );
    TESTCHECKF( Binomial_DF( 13, 0.35, 13 ), 1., &ok );
    TESTCHECKFE( Binomial_DF( 0, 0.05, 19 ), 0.3774, &ok, 0.0001 );
    TESTCHECKFE( Binomial_DF( 4, 0.35, 19 ), 0.1500, &ok, 0.0001 );
    TESTCHECKFE( Binomial_DF( 8, 0.35, 19 ), 0.8145, &ok, 0.0001 );
    TESTCHECKF( Binomial_DF( 19, 0.35, 19 ), 1., &ok );
    TESTCHECK( Binomial_DF( 20, 0.35, 19 ), 1., &ok );

    //!!!Geometric

    TESTCHECKF( Hypergeometric_PDF( 4, 50, 5, 10 ), 0.003964583, &ok );
    TESTCHECKF( Hypergeometric_PDF( 5, 50, 5, 10 ), 0.0001189375, &ok );
    TESTCHECKF( Hypergeometric_DF( 3, 50, 5, 10 ), 0.99591648, &ok );

    TESTCHECK( Poisson_PDF( -1, 1. ), 0., &ok );
    TESTCHECKFE( Poisson_PDF( 0, 0.6 ), 0.5488, &ok, 0.0001 );
    TESTCHECKFE( Poisson_PDF( 4, 4.6 ), 0.1875, &ok, 0.0001 );
    TESTCHECK( Poisson_DF( -2, 2. ), 0., &ok );
    TESTCHECKFE( Poisson_DF( 1, 1.7 ), 0.4932, &ok, 0.0001 );
    TESTCHECKFE( Poisson_DF( 7, 7.7 ), 0.4956, &ok, 0.0001 );
    TESTCHECKFE( Poisson_DF( 3, 8.7 ), 0.0262, &ok, 0.0001 );
    TESTCHECKFE( Poisson_DF( 26, 15. ), 0.9967, &ok, 0.0001 );

    //!!!Exponential

    //!!!Gamma_PDF
    TESTCHECK( Gamma_DF( 0., 3, 2. ), 0., &ok );
    TESTCHECKF( Gamma_DF( 0.01005035, 1, 1. ), 0.01, &ok );
    TESTCHECKF( Gamma_DF( 0.17768025, 2, 2. ), 0.05, &ok );
    TESTCHECKF( Gamma_DF( 9.34182, 5, 0.5 ), 0.5, &ok );
    TESTCHECKF( Gamma_DF( 22.82085, 13, 1. ), 0.99, &ok );

    TESTCHECKF( Normal_PDF( 0. ), 0.398942280401433, &ok );
    TESTCHECKF( Normal_PDF( 1.5 ), 0.129517595665892, &ok );
    TESTCHECKF( Normal_PDF( -2.72 ), 0.009871153794751, &ok );
    TESTCHECKF( Normal_DF( -2.32635 ), 0.01, &ok );
    TESTCHECKF( Normal_DF( 0. ), 0.5, &ok );
    TESTCHECKF( Normal_DF( 1. ), 0.84134474606855, &ok );
    TESTCHECKF( Normal_DF( 1.28155 ), 0.9, &ok );
    TESTCHECKF( Normal_DF( 2. ), 0.9772498680518, &ok );
    TESTCHECKF( Normal_DF( 3. ), 0.99865010196835, &ok );

    //!!!LogNormal

    //!!!ChiSquare_PDF
    TESTCHECK( ChiSquare_PDF( -1., 5 ), 0., &ok );
    TESTCHECK( ChiSquare_DF( -1., 5 ), 0., &ok );
    TESTCHECKF( ChiSquare_DF( 0.000982069, 1 ), 0.025, &ok );
    TESTCHECKF( ChiSquare_DF( 4.86518, 10 ), 0.1, &ok );
    TESTCHECKFE( ChiSquare_DF( 22.3072, 15 ), 0.9, &ok, 0.000002 );
    TESTCHECKF( ChiSquare_DF( 41.9232, 26 ), 0.975, &ok );

    //!!!StudentsT_PDF
    TESTCHECKFE( StudentsT_DF( 0.325, 1 ), 0.6, &ok, 0.001 );
    TESTCHECKFE( StudentsT_DF( 1., 1 ), 0.75, &ok, 0.001 );
    TESTCHECKFE( StudentsT_DF( 31.821, 1 ), 0.99, &ok, 0.00001 );
    TESTCHECKF( StudentsT_DF( 636.619, 1 ), 0.9995, &ok );
    TESTCHECKFE( StudentsT_DF( 0.267, 5 ), 0.6, &ok, 0.001 );
    TESTCHECKFE( StudentsT_DF( 2.571, 5 ), 0.975, &ok, 0.0001 );
    TESTCHECK( StudentsT_DF( 0., 10 ), 0.5, &ok );
    TESTCHECKFE( StudentsT_DF( -1.725, 20 ), 0.05, &ok, 0.0006 );

    //!!!F_PDF
    TESTCHECK( F_PDF( -1., 3, 5 ), 0., &ok );
    TESTCHECK( F_DF( -1., 3, 5 ), 0., &ok );
    TESTCHECKFE( F_DF( 60.19, 10, 1 ), 0.9, &ok, 0.001 );
    TESTCHECKFE( F_DF( 2.17, 7, 15 ), 0.9, &ok, 0.002 );
    TESTCHECKFE( F_DF( 2.63, 15, 7 ), 0.9, &ok, 0.001 );
    TESTCHECKFE( F_DF( 10.04, 1, 10 ), 0.99, &ok, 0.001 );
    TESTCHECKFE( F_DF( 2.61, 60, 20 ), 0.99, &ok, 0.001 );
    TESTCHECKFE( F_DF( 2.20, 20, 60 ), 0.99, &ok, 0.001 );
    TESTCHECKFE( F_DF( 623500, 24, 1 ), 0.999, &ok, 0.0001 );
    TESTCHECKFE( F_DF( 14.19, 1, 23 ), 0.999, &ok, 0.0001 );
    TESTCHECKFE( F_DF( 4.14, 15, 24 ), 0.999, &ok, 0.0001 );
    TESTCHECKFE( F_DF( 5.10, 24, 15 ), 0.999, &ok, 0.0001 );

    //!!!Cauchy

    //!!!Beta_PDF
    TESTCHECK( Beta_DF( -1., 3, 4 ), 0., &ok );
    TESTCHECKFE( Beta_DF( 0.3415, 11, 6 ), 0.005, &ok, 0.0002 );
    TESTCHECKFE( Beta_DF( 0.1909, 6, 10 ), 0.05, &ok, 0.001 );
    TESTCHECKFE( Beta_DF( 0.4099, 3, 15 ), 0.99, &ok, 0.0001 );
    TESTCHECKFE( Beta_DF( 0.9085, 15, 5 ), 0.975, &ok, 0.0001 );
    TESTCHECKFE( Beta_DF( 0.3344, 4, 15 ), 0.9, &ok, 0.0001 );
    TESTCHECK( Beta_DF( 1., 11, 6 ), 1., &ok );
    TESTCHECK( Beta_DF( 1.5, 13, 16 ), 1., &ok );

    //!!!KolmogorovSmirnov

    if ( ok )
        cout << "Probability Distributions PASSED." << endl << endl;
    else
        cout << "Probability Distributions FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

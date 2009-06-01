#ifndef PROBABILITYDISTRIBUTIONS_HPP
#define PROBABILITYDISTRIBUTIONS_HPP
/*
  ProbabilityDistributions.hpp
  Copyright (C) 2007 David M. Anderson

  Density and distribution functions for common probability distributions.
  NOTES:
  1. The _PDF functions compute the probability density function.
  2. The _DF functions compute the (cumulative) distribution function,
     i.e. Prob( X <= x ).
  3. Distributions:
      Uniform: any number in the range as likely as any other.
      Triangle: pdf is triangular, with a peak at mode.
      Geometric: X = number of independent Bernoulli trials, each with
          given probability, needed for success.
      Binomial: X = number of successes in given number of Bernoulli trials,
          each with given probability.
      Hypergeometric: Given a total population size, the size of a
          subset, and a sample size (without replacement), X = number of
          samples from the subset.
      Poisson: X = number of occurences in interval t of a Poisson process:
          independent increments; probability of occurence in interval h is
          lambda * h + o(h); probability of more than one is o(h).
          The mean is lambda * t.
      Exponential: X = waiting time for first occurence of Poisson process.
      Gamma: X = waiting time for n occurences of Poisson process.
      Normal (a.k.a. Gaussian): Limiting distribution of sum (or average) of
          independent random variables with a finite variance.
          Symmetric around mean.
      LogNormal: X = exp(Y), where Y is normally distributed.
          If momentsOfLog is true, the given mean and standard
          deviation are of Y, otherwise they are of X itself. Typically,
          this is the limiting distribution of a product of independent
          random variables.
      Chi-square: X = sum from i = 1 to (degreesOfFreedom) of Y[i]^2
          where Y[i] is N(0,1).
      Student's t: X = Y1 / sqrt(Y2/r), where Y1 is N(0,1) and Y2 is
          Chi-square. Symmetric around 0.
      Snedecor's F: X = (Y1/d1) / (Y2/d2), where Y1 and Y2 are Chi-square
          with degrees of freedom d1 and d2.
      Cauchy: (a) X = a * tan(Y), where Y is uniform between -pi/2 and pi/2.
          (b) X = Y1 / Y2, where Y1 and Y2 are independent N(0,1) (a=1).
      Beta: (a) X = Y1 / (Y1 + Y2), where Y1 and Y2 have Gamma distributions
          with n = 1 and lambda parameters a and b.
          (b) X = 1 / (1 + (a/b)X), where X has an F distribution.
      Kolmogorov Smirnov: distribution of the statistic from the K-S test.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


double Uniform_PDF( int x, int limit );
double Uniform_DF( int x, int limit );
double Uniform_PDF( double x, double minimum, double maximum );
double Uniform_DF( double x, double minimum, double maximum );
double Triangle_PDF( double x, double minimum, double mode, double maximum );
double Triangle_DF( double x, double minimum, double mode, double maximum );
double Geometric_PDF( int x, double probability );
double Geometric_DF( int x, double probability );
double Binomial_PDF( int x, double probability, int trials );
double Binomial_DF( int x, double probability, int trials );
double Hypergeometric_PDF( int x, int populationSize, int subsetSize,
                           int sampleSize );
double Hypergeometric_DF( int x, int populationSize, int subsetSize,
                          int sampleSize );
double Poisson_PDF( int x, double mean );
double Poisson_DF( int x, double mean );
double Exponential_PDF( double x, double lambda = 1. );
double Exponential_DF( double x, double lambda = 1. );
double Gamma_PDF( double x, int n, double lambda = 1. );
double Gamma_DF( double x, int n, double lambda = 1. );
double Normal_PDF( double x, double mean = 0., double standardDeviation = 1. );
double Normal_DF( double x, double mean = 0., double standardDeviation = 1. );
double LogNormal_PDF( double x, double mean, double standardDeviation,
                      bool momentsOfLog = false );
double LogNormal_DF( double x, double mean, double standardDeviation,
                     bool momentsOfLog = false );
double ChiSquare_PDF( double x, int degreesOfFreedom );
double ChiSquare_DF( double x, int degreesOfFreedom );
double StudentsT_PDF( double x, int degreesOfFreedom );
double StudentsT_DF( double x, int degreesOfFreedom );
double F_PDF( double x, int dof1, int dof2 );
double F_DF( double x, int dof1, int dof2 );
double Cauchy_PDF( double x, double a = 1. );
double Cauchy_DF( double x, double a = 1. );
double Beta_PDF( double x, double a, double b );
double Beta_DF( double x, double a, double b );
double KolmogorovSmirnov_DF( double x );

#ifdef DEBUG
bool TestProbabilityDistributions( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PROBABILITYDISTRIBUTIONS_HPP

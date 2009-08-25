#ifndef RANDOM_HPP
#define RANDOM_HPP
/*
  Random.hpp
  Copyright (C) 2007 David M. Anderson

  RandomNumberGenerator class: generates pseudo-random numbers from various
      distributions.
  Random object: Global object of RandomNumberGenerator class.
  QuickRandomNumberGenerator class: generates uniformly-distributed
      pseudo-random numbers. Not as high quality, but very fast. (From Press,
      et al., "Numerical Recipes in C++", 2nd ed., p. 289.)
  QuickRandom object: Global object of QuickRandomNumberGenerator class.
  NOTES:
  1. The operator() methods return uniformly distributed random numbers.
  2. For details of most of the probability distributions,
     see ProbabilityDistributions.hpp.
  3. Bernoulli: boolean random variable, true with given probability.
     UniformOnSphere: Random point on unit sphere.
*/


#include "SmartPtr.hpp"
#include "Vector3.hpp"
#include "Array.hpp"
#include "StdInt.hpp"
#include "FixEndian.hpp"

#include <iostream>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class RandomNumberGeneratorImpl;


//=============================================================================


class RandomNumberGenerator
{
public:
    RandomNumberGenerator( );
    ~RandomNumberGenerator( );
    void Reseed( int seed );
    int Seed( ) const;

    int operator()( );
    int operator()( int limit );
    double operator()( double minimum, double maximum );
    double Triangle( double minimum, double mode, double maximum );
    bool Bernoulli( double probability );
    int Geometric( double probability );
    int Binomial( double probability, int trials );
    int Poisson( double mean );
    double Exponential( double lambda = 1. );
    double Gamma( int n, double lambda = 1. );
    double Normal( double mean = 0., double standardDeviation = 1. );
    double LogNormal( double mean, double standardDeviation,
                      bool momentsOfLog = false );
    Vector3D UniformOnSphere( );
    std::vector< double > UniformOnSphere( int dimension );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    double Uni01( );    //operator()(0.,1.)

    RandomNumberGeneratorImpl * m_pImpl;
    int     m_seed;
};


//=============================================================================


extern RandomNumberGenerator Random;


//*****************************************************************************


class QuickRandomNumberGenerator
{
public:
    QuickRandomNumberGenerator( );
    void Reseed( int seed );
    int Seed( ) const;

    int operator()( );
    int operator()( int limit );
    double operator()( double minimum, double maximum );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    uint32_t    m_seed;
};


//=============================================================================


extern QuickRandomNumberGenerator QuickRandom;


//#############################################################################


inline
void 
QuickRandomNumberGenerator::Reseed( int seed )
{
    m_seed = static_cast< uint32_t >( seed );
}

//-----------------------------------------------------------------------------

inline
int 
QuickRandomNumberGenerator::Seed( ) const
{
    return static_cast< int >( m_seed );
}

//=============================================================================

inline
int 
QuickRandomNumberGenerator::operator()( )
{
    m_seed = 1664525 * m_seed + 1013904223;
    return static_cast< int >( m_seed );
}

//-----------------------------------------------------------------------------

inline
int 
QuickRandomNumberGenerator::operator()( int limit )
{
    Assert( limit > 0 );
    m_seed = 1664525 * m_seed + 1013904223;
    uint32_t i = m_seed;
    SwapEndian( &i );   //low-order bits are the least random
    return (i % limit);
}

//-----------------------------------------------------------------------------

inline
double 
QuickRandomNumberGenerator::operator()( double minimum, double maximum )
{
    m_seed = 1664525 * m_seed + 1013904223;
    uint32_t i = (m_seed & 0x007FFFFF) | 0x3F800000;
    float f = *( reinterpret_cast< float * >( &i ) ) - 1.f;
    return (f * (maximum - minimum)  +  minimum);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //RANDOM_HPP

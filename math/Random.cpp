/*
  Random.cpp
  Copyright (C) 2007 David M. Anderson

  RandomNumberGenerator class: generates pseudo-random numbers from various
  distributions.
  Random object: Global object of RandomNumberGenerator class.
*/

#include "Random.hpp"
#include "Gamma.hpp"
#include <tr1/random>
#include <ctime>
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "ProbabilityDistributions.hpp"
#include "StatisticalTests.hpp"
#include <iostream>
#include <algorithm>
#include <numeric>
#include <boost/bind.hpp>
#include <tr1/array>
#include <string>
#endif
using namespace std;
using namespace std::tr1;
#ifdef DEBUG
using boost::bind;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


RandomNumberGenerator Random;
QuickRandomNumberGenerator QuickRandom;


//*****************************************************************************


class RandomNumberGeneratorImpl
{
public:
    RandomNumberGeneratorImpl( );
    void Reseed( int seed );

    int operator()( );
    int operator()( int limit );
    double operator()( double minimum, double maximum );
    double Triangle( double minimum, double mode, double maximum );
    bool Bernoulli( double probability );
    int Binomial( double probability, int trials );
    int Geometric( double probability );
    int Poisson( double mean );
    double Exponential( double lambda );
    double Gamma( double n, double lambda );
    double Normal( double mean, double standardDeviation );
    double LogNormal( double mean, double standardDeviation );
    Vector3D UniformOnSphere( );
    std::vector< double > UniformOnSphere( int dimension );

private:
    mt19937  m_rng;
    variate_generator< mt19937 &, uniform_real<> >  m_rng01;
};


//*****************************************************************************


RandomNumberGenerator::RandomNumberGenerator( )
    :   m_pImpl( new RandomNumberGeneratorImpl( ) )
{
    time_t t = time( 0 );
    m_seed = (int) t;
    m_pImpl->Reseed( m_seed );
}

//-----------------------------------------------------------------------------

RandomNumberGenerator::~RandomNumberGenerator( )
{
}

//=============================================================================

void 
RandomNumberGenerator::Reseed( int seed )
{
    m_seed = seed;
    m_pImpl->Reseed( seed );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGenerator::Seed( ) const
{
    return m_seed;
}

//=============================================================================

int 
RandomNumberGenerator::operator()( )
{
    return (*m_pImpl)( );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGenerator::operator()( int limit )
{
    Assert( limit > 0 );
    return (*m_pImpl)( limit );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGenerator::operator()( double minimum, double maximum )
{
    return (*m_pImpl)( minimum, maximum );
}

//=============================================================================

double 
RandomNumberGenerator::Triangle( double minimum, double mode, double maximum )
{
    Assert( minimum < maximum );
    Assert( (minimum <= mode) && (mode <= maximum) );
    return m_pImpl->Triangle( minimum, mode, maximum );
}

//-----------------------------------------------------------------------------

bool
RandomNumberGenerator::Bernoulli( double probability )
{
    Assert( (probability >= 0.) && (probability <= 1.) );
    return m_pImpl->Bernoulli( probability );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGenerator::Binomial( double probability, int trials )
{
    Assert( (probability >= 0.) && (probability <= 1.) );
    Assert( trials > 0 );
    return m_pImpl->Binomial( probability, trials );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGenerator::Geometric( double probability )
{
    Assert( (probability > 0.) && (probability <= 1.) );
    return m_pImpl->Geometric( probability );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGenerator::Poisson( double mean )
{
    Assert( mean >= 0. );
    return m_pImpl->Poisson( mean );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGenerator::Exponential( double lambda )
{
    Assert( lambda > 0. );
    return m_pImpl->Exponential( lambda );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGenerator::Gamma( int n, double lambda )
{
    Assert( n > 0 );
    Assert( lambda > 0. );
    return m_pImpl->Gamma( n, lambda );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGenerator::Normal( double mean, double standardDeviation )
{
    Assert( standardDeviation > 0. );
    return m_pImpl->Normal( mean, standardDeviation );
}

//-----------------------------------------------------------------------------

//Defined in ProbabilityDistributions.cpp:
void ConvertLogNormalMoments( double mean, double stdDev,
                              double * meanOfLog, double * stdDevOfLog );

//.............................................................................

double 
RandomNumberGenerator::LogNormal( double mean, double standardDeviation,
                                  bool momentsOfLog )
{
    Assert( standardDeviation > 0. );
    if ( momentsOfLog )
    {
        return m_pImpl->LogNormal( mean, standardDeviation );
    }
    else
    {
        Assert( mean > 0. );
        double meanOfLog, stdDevOfLog;
        ConvertLogNormalMoments( mean, standardDeviation,
                                 &meanOfLog, &stdDevOfLog );
        return m_pImpl->LogNormal( meanOfLog, stdDevOfLog );
    }
}

//-----------------------------------------------------------------------------

double
RandomNumberGenerator::ChiSquare( int degreesOfFreedom )
{
    Assert( degreesOfFreedom > 0 );
    return m_pImpl->Gamma( degreesOfFreedom * 0.5, 0.5 );
}

//-----------------------------------------------------------------------------

double
RandomNumberGenerator::StudentsT( int degreesOfFreedom )
{
    Assert( degreesOfFreedom > 0 );
    double x = Normal( 0., 1. );
    double y = ChiSquare( degreesOfFreedom );
    Assert( y > 0. );
    return x / sqrt( y / degreesOfFreedom );
}

//-----------------------------------------------------------------------------

double
RandomNumberGenerator::F( int dof1, int dof2 )
{
    Assert( (dof1 > 0) && (dof2 > 0) );
    double x = ChiSquare( dof1 );
    double y = ChiSquare( dof2 );
    Assert( y > 0. );
    return (x / dof1) / (y / dof2);
}

//-----------------------------------------------------------------------------

double
RandomNumberGenerator::Cauchy( double a )
{
    const double halfPi = 3.14159265358979 * 0.5; //Must be slightly < π/2
    double r = (*m_pImpl)( -halfPi, halfPi );
    return a * tan( r );
}

//-----------------------------------------------------------------------------

double
RandomNumberGenerator::Beta( double a, double b )
{
    Assert( (a > 0.) && (b > 0.) );
    double y1 = Gamma( a, 1. );
    double y2 = Gamma( b, 1. );
    Assert( y1 + y2 > 0. );
    return y1 / (y1 + y2);
}

//-----------------------------------------------------------------------------

Vector3D 
RandomNumberGenerator::UniformOnSphere( )
{
    return m_pImpl->UniformOnSphere( );
}

//.............................................................................

std::vector< double > 
RandomNumberGenerator::UniformOnSphere( int dimension )
{
    Assert( dimension > 0 );
    return m_pImpl->UniformOnSphere( dimension );
}


//*****************************************************************************


RandomNumberGeneratorImpl::RandomNumberGeneratorImpl( )
    :   m_rng01( m_rng, uniform_real<>( 0., 1. ) )
{
}

//=============================================================================

void 
RandomNumberGeneratorImpl::Reseed( int seed )
{
    m_rng.seed( (uint32_t) seed );
}

//=============================================================================

int 
RandomNumberGeneratorImpl::operator()( )
{
    return (int) m_rng( );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGeneratorImpl::operator()( int limit )
{
    uniform_int<> distribution( 0, limit - 1 );
    return distribution( m_rng );
}

//-----------------------------------------------------------------------------

double
RandomNumberGeneratorImpl::operator()( double minimum, double maximum )
{
    uniform_real<> distribution( minimum, maximum );
    return distribution( m_rng01 );
}

//=============================================================================

double 
RandomNumberGeneratorImpl::Triangle( double minimum, double mode,
                                     double maximum )
{
    double u = m_rng01( );
    double d1 = mode - minimum;
    double d2 = maximum - minimum;
    if ( u < (d1 / d2) )
        return minimum  +  sqrt( u * d1 * d2 );
    else
        return maximum  -  sqrt( (maximum - mode) * (u * d2 - d1) );
}

//-----------------------------------------------------------------------------

bool 
RandomNumberGeneratorImpl::Bernoulli( double probability )
{
    bernoulli_distribution distribution( probability );
    return distribution( m_rng );
}

//-----------------------------------------------------------------------------

int
RandomNumberGeneratorImpl::Binomial( double probability, int trials )
{
    binomial_distribution<> distribution( trials, probability );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGeneratorImpl::Geometric( double probability )
{
    //TR1/Boost uses the opposite probability from us.
    geometric_distribution<> distribution( 1. - probability );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

int
RandomNumberGeneratorImpl::Poisson( double mean )
{
    poisson_distribution<> distribution( mean );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGeneratorImpl::Exponential( double lambda )
{
    exponential_distribution<> distribution( lambda );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

double
RandomNumberGeneratorImpl::Gamma( double n, double lambda )
{
    gamma_distribution<> distribution( n );
    return distribution( m_rng01 ) / lambda;
}

//-----------------------------------------------------------------------------

double 
RandomNumberGeneratorImpl::Normal( double mean, double standardDeviation )
{
    normal_distribution<> distribution( mean, standardDeviation );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGeneratorImpl::LogNormal( double mean, double standardDeviation )
{
    double r = Normal( mean, standardDeviation );
    return exp( r );
}

//-----------------------------------------------------------------------------

Vector3D 
RandomNumberGeneratorImpl::UniformOnSphere( )
{
    vector< double > v = UniformOnSphere( 3 );
    return Vector3D( &v[0] );
}

//.............................................................................

vector< double > 
RandomNumberGeneratorImpl::UniformOnSphere( int dimension )
{
    vector< double > rslt( dimension );
    double sqSum = 0.;
    do
    {
        for ( int i = 0; i < dimension; ++i )
        {
            double r = Normal( 0., 1. );
            rslt[ i ] = r;
            sqSum += r * r;
        }
    } while ( sqSum == 0. );  //extremely unlikely, but...
    double sqrtSum = sqrt( sqSum );
    for ( int i = 0; i < dimension; ++i )
        rslt[ i ] /= sqrtSum;
    return rslt;
}


//*****************************************************************************


QuickRandomNumberGenerator::QuickRandomNumberGenerator( )
{
    time_t t = time( 0 );
    m_seed = (uint32_t) t;
}


//*****************************************************************************


#ifdef DEBUG

//-----------------------------------------------------------------------------

namespace
{

void GraphSampleAndExpected( vector< int > & sample,
                             vector< double > & expectedFreqs );
template < typename DistribFunc >
void GraphSampleAndDist( vector< double > & sample, DistribFunc hypothFunc );

}

//-----------------------------------------------------------------------------

bool 
RandomNumberGenerator::Test( )
{
    bool ok = true;
    cout << "Testing Random" << endl;

    vector< int > sampleFreqs( 256 );
    vector< double > expectedFreqs( 256 );
    for ( int i = 0; i < 256; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Uniform_PDF( i, 256 );
    }
    for ( int i = 0; i < 4096 * 256; ++i )
    {
        int r = Random( 256 );
        Assert( (r >= 0) && (r < 256) );
        ++sampleFreqs[ r ];
    }
    ChiSquareTestResult chiSqrRslt
            = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    double prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Random( 256 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    vector< double > sample( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random( 1.5, 3.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Uniform_DF, _1, 1.5, 3.5 ) );
    cout << "KolmogorovSmirnovTest of Random( 1.5, 3.5 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Uniform_DF, _1, 1.5, 3.5 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Triangle( -3.5, -1., 0.5 );
    prob = KolmogorovSmirnovTest( sample,
                                  bind( Triangle_DF, _1, -3.5, -1., 0.5 ) );
    cout << "KolmogorovSmirnovTest of Triangle( -3.5, -1., 0.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Triangle_DF, _1, -3.5, -1., 0.5 ) );
    sample.clear();

    sampleFreqs.resize( 2 );
    expectedFreqs.resize( 2 );
    sampleFreqs[ 0 ] = 0;
    sampleFreqs[ 1 ] = 0;
    expectedFreqs[ 0 ] = 0.25;
    expectedFreqs[ 1 ] = 0.75;
    for ( int i = 0; i < 1000000; ++i )
    {
        bool r = Random.Bernoulli( 0.75 );
        ++sampleFreqs[ r  ?  1  :  0 ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Bernoulli( 0.75 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    sampleFreqs.resize( 10 );
    expectedFreqs.resize( 10 );
    for ( int i = 0; i < 10; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Binomial_PDF( i, 0.2, 9 );
    }
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Binomial( 0.2, 9 );
        Assert( r >= 0 );
        Assert( r < 10 );
        ++sampleFreqs[ r ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.2, 9 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();
    
    sampleFreqs.resize( 20 );
    expectedFreqs.resize( 20 );
    for ( int i = 0; i < 20; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Binomial_PDF( i, 0.8, 19 );
    }
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Binomial( 0.8, 19 );
        Assert( r >= 0 );
        Assert( r < 20 );
        ++sampleFreqs[ r ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.8, 19 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();
    
    sampleFreqs.resize( 50 );
    expectedFreqs.resize( 50 );
    for ( int i = 0; i < 50; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Binomial_PDF( i, 0.002, 49 );
    }
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Binomial( 0.002, 49 );
        Assert( r >= 0 );
        Assert( r < 50 );
        ++sampleFreqs[ r ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.002, 49 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();
    
    sampleFreqs.resize( 500 );
    expectedFreqs.resize( 500 );
    for ( int i = 0; i < 500; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Binomial_PDF( i, 0.0002, 499 );
    }
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Binomial( 0.0002, 499 );
        Assert( r >= 0 );
        Assert( r < 500 );
        ++sampleFreqs[ r ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.0002, 499 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();
    
    sampleFreqs.resize( 500 );
    expectedFreqs.resize( 500 );
    for ( int i = 0; i < 500; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Binomial_PDF( i, 0.1, 499 );
    }
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Binomial( 0.1, 499 );
        Assert( r >= 0 );
        Assert( r < 500 );
        ++sampleFreqs[ r ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.1, 499 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();
    
    sampleFreqs.resize( 50 );
    expectedFreqs.resize( 50 );
    for ( int i = 0; i < 50; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Geometric_PDF( i, 0.1 );
    }
    expectedFreqs[ 49 ] = 1. - Geometric_DF( 48, 0.1 );
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Geometric( 0.1 );
        Assert( r > 0 );
        if ( r < 49 )
            ++sampleFreqs[ r ];
        else
            ++sampleFreqs[ 49 ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Geometric( 0.1 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    sampleFreqs.resize( 50 );
    expectedFreqs.resize( 50 );
    for ( int i = 0; i < 50; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Poisson_PDF( i, 10.5 );
    }
    expectedFreqs[ 49 ] = 1. - Poisson_DF( 48, 10.5 );
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Poisson( 10.5 );
        Assert( r >= 0 );
        if ( r < 49 )
            ++sampleFreqs[ r ];
        else
            ++sampleFreqs[ 49 ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Poisson( 10.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    sampleFreqs.resize( 50 );
    expectedFreqs.resize( 50 );
    for ( int i = 0; i < 50; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Poisson_PDF( i, 15.5 );
    }
    expectedFreqs[ 49 ] = 1. - Poisson_DF( 48, 15.5 );
    for ( int i = 0; i < 1000000; ++i )
    {
        int r = Random.Poisson( 15.5 );
        Assert( r >= 0 );
        if ( r < 49 )
            ++sampleFreqs[ r ];
        else
            ++sampleFreqs[ 49 ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of Poisson( 15.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Exponential( 0.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Exponential_DF, _1, 0.5 ) );
    cout << "KolmogorovSmirnovTest of Exponential( 0.5 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Exponential_DF, _1, 0.5 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Gamma( 11, 0.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Gamma_DF, _1, 11, 0.5 ) );
    cout << "KolmogorovSmirnovTest of Gamma( 11, 0.5 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Gamma_DF, _1, 11, 0.5 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Normal( -12.2, 4.4 );
    prob = KolmogorovSmirnovTest( sample, bind( Normal_DF, _1, -12.2, 4.4 ) );
    cout << "KolmogorovSmirnovTest of Normal( -12.2, 4.4 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Normal_DF, _1, -12.2, 4.4 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.LogNormal( 2.2, 4.4 );
    prob = KolmogorovSmirnovTest( sample,
                                 bind( LogNormal_DF, _1, 2.2, 4.4, false ) );
    cout << "KolmogorovSmirnovTest of LogNormal( 2.2, 4.4 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( LogNormal_DF, _1, 2.2, 4.4, false ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.LogNormal( -2.2, 1.4, true );
    prob = KolmogorovSmirnovTest( sample,
                                  bind( LogNormal_DF, _1, -2.2, 1.4, true ) );
    cout << "KolmogorovSmirnovTest of LogNormal( -2.2, 1.4, true ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( LogNormal_DF, _1, 2.2, 4.4, true ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.ChiSquare( 17 );
    prob = KolmogorovSmirnovTest( sample, bind( ChiSquare_DF, _1, 17 ) );
    cout << "KolmogorovSmirnovTest of ChiSquare( 17 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( ChiSquare_DF, _1, 17 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.StudentsT( 23 );
    prob = KolmogorovSmirnovTest( sample, bind( StudentsT_DF, _1, 23 ) );
    cout << "KolmogorovSmirnovTest of StudentT( 23 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( StudentsT_DF, _1, 23 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.F( 15, 9 );
    prob = KolmogorovSmirnovTest( sample, bind( F_DF, _1, 15, 9 ) );
    cout << "KolmogorovSmirnovTest of F( 15, 9 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( F_DF, _1, 15, 9 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Cauchy( 2.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Cauchy_DF, _1, 2.5 ) );
    cout << "KolmogorovSmirnovTest of Cauchy( 2.5 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Cauchy_DF, _1, 2.5 ) );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Beta( 15., 55. );
    prob = KolmogorovSmirnovTest( sample, bind( Beta_DF, _1, 15., 55. ) );
    cout << "KolmogorovSmirnovTest of Beta( 15., 55. ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Beta_DF, _1, 15., 55. ) );
    sample.clear();

    //Knuth's Serial test
    sampleFreqs.resize( 64 * 64 );
    expectedFreqs.resize( 64 * 64 );
    for ( int i = 0; i < 64 * 64; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = 1. / (64 * 64);
    }
    for ( int i = 0; i < 1000000; ++i )
    {
        int r0 = Random( 64 );
        int r1 = Random( 64 );
        ++sampleFreqs[ r0  +  64 * r1 ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of consecutive pairs of Random(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    //Knuth's Gap test
    sampleFreqs.resize( 18 );
    expectedFreqs.resize( 18 );
    double p = 0.25;
    for ( int i = 0; i < 18; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = p;
        p *= 0.75;
    }
    expectedFreqs[17] /= 0.25;
    for ( int i = 0; i < 1500000; ++i )
    {
        int j = 0;
        while ( Random( 4 ) > 0 )
            ++j;
        if ( j < 17 )
            ++sampleFreqs[j];
        else
            ++sampleFreqs[17];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of gaps in Random(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    sampleFreqs.resize( 18 );
    expectedFreqs.resize( 18 );
    p = 0.25;
    for ( int i = 0; i < 18; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = p;
        p *= 0.75;
    }
    expectedFreqs[17] /= 0.25;
    for ( int i = 0; i < 1500000; ++i )
    {
        int j = 0;
        while ( Random( 0., 1. ) > 0.25 )
            ++j;
        if ( j < 17 )
            ++sampleFreqs[j];
        else
            ++sampleFreqs[17];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of gaps in Random(0.,1.) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    if ( ok )
        cout << "Random PASSED." << endl << endl;
    else
        cout << "Random FAILED." << endl << endl;
    return ok;
}

//=============================================================================

bool 
QuickRandomNumberGenerator::Test( )
{
    bool ok = true;
    cout << "Testing QuickRandom" << endl;

    vector< int > sampleFreqs( 256 );
    vector< double > expectedFreqs( 256 );

    for ( int i = 0; i < 256; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = Uniform_PDF( i, 256 );
    }
    for ( int i = 0; i < 4096 * 256; ++i )
    {
        int r = QuickRandom( 256 );
        Assert( (r >= 0) && (r < 256) );
        ++sampleFreqs[ r ];
    }
    ChiSquareTestResult chiSqrRslt
            = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    double prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of QuickRandom( 256 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    vector< double > sample( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = QuickRandom( 1.5, 3.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Uniform_DF, _1, 1.5, 3.5 ) );
    cout << "KolmogorovSmirnovTest of QuickRandom( 1.5, 3.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndDist( sample, bind( Uniform_DF, _1, 1.5, 3.5 ) );
    sample.clear();

    //Knuth's Serial test
    sampleFreqs.resize( 64 * 64 );
    expectedFreqs.resize( 64 * 64 );
    for ( int i = 0; i < 64 * 64; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = 1. / (64 * 64);
    }
    for ( int i = 0; i < 1000000; ++i )
    {
        int r0 = QuickRandom( 64 );
        int r1 = QuickRandom( 64 );
        ++sampleFreqs[ r0  +  64 * r1 ];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of consecutive pairs of"
            " QuickRandom(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    //Knuth's Gap test
    sampleFreqs.resize( 18 );
    expectedFreqs.resize( 18 );
    double p = 0.25;
    for ( int i = 0; i < 18; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = p;
        p *= 0.75;
    }
    expectedFreqs[17] /= 0.25;
    for ( int i = 0; i < 1500000; ++i )
    {
        int j = 0;
        while ( QuickRandom( 4 ) > 0 )
            ++j;
        if ( j < 17 )
            ++sampleFreqs[j];
        else
            ++sampleFreqs[17];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of gaps in QuickRandom(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    sampleFreqs.resize( 18 );
    expectedFreqs.resize( 18 );
    p = 0.25;
    for ( int i = 0; i < 18; ++i )
    {
        sampleFreqs[i] = 0;
        expectedFreqs[i] = p;
        p *= 0.75;
    }
    expectedFreqs[17] /= 0.25;
    for ( int i = 0; i < 1500000; ++i )
    {
        int j = 0;
        while ( QuickRandom( 0., 1. ) > 0.25 )
            ++j;
        if ( j < 17 )
            ++sampleFreqs[j];
        else
            ++sampleFreqs[17];
    }
    chiSqrRslt = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    prob = chiSqrRslt.probability;
    cout << "ChiSquareGoodnessOfFitTest of gaps in QuickRandom(0.,1.) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    GraphSampleAndExpected( sampleFreqs, expectedFreqs );
    sampleFreqs.clear();
    expectedFreqs.clear();

    if ( ok )
        cout << "QuickRandom PASSED." << endl << endl;
    else
        cout << "QuickRandom FAILED." << endl << endl;
    return ok;
}

//=============================================================================

namespace
{                                                                   //namespace
//-----------------------------------------------------------------------------

const int height = 12;
const int width = 60;

void DrawGraph( array< double, width > sampleFreqs,
                array< double, width > hypothFreqs );

//-----------------------------------------------------------------------------

void
GraphSampleAndExpected( vector< int > & sample,
                        vector< double > & expectedFreqs )
{
    int sampleSize = accumulate( sample.begin(), sample.end(), 0 );
    if ( sampleSize <= 0 )
        return;
    int binSize = (int) ceil( ((double) sample.size()) / width );
    Assert( binSize > 0 );
    int numBins = sample.size() / binSize;
    Assert( numBins <= width );
    array< double, width > sampleFreqs;
    array< double, width > hypothFreqs;
    int i = 0;
    for ( ; i < numBins; ++i )
    {
        sampleFreqs[ i ] = 0.;
        hypothFreqs[ i ] = 0.;
        for ( int j = 0; j < binSize; ++j )
        {
            int k = i * binSize  +  j;
            sampleFreqs[ i ] += ((double) sample[ k ]) / sampleSize;
            hypothFreqs[ i ] += expectedFreqs[ k ];
        }
    }
    for ( ; i < width; ++i )
    {
        sampleFreqs[ i ] = 0.;
        hypothFreqs[ i ] = 0.;
    }
    DrawGraph( sampleFreqs, hypothFreqs );
    cout << endl;
}

//-----------------------------------------------------------------------------

template < typename DistribFunc >
void
GraphSampleAndDist( vector< double > & sample, DistribFunc hypothFunc )
{
    const int height = 12;
    double sampleMin = *(min_element( sample.begin(), sample.end() ));
    double sampleMax = *(max_element( sample.begin(), sample.end() ));
    double graphMin = sampleMin;
    double graphMax = sampleMax;
    double binSize = (graphMax - graphMin) / width;
    array< int, width > binCounts;
    for ( int i = 0; i < width; ++i )
        binCounts[ i ] = 0;
    for ( vector< double >::const_iterator p = sample.begin();
          p != sample.end(); ++p )
    {
        int bin = (int) ((*p - graphMin) / binSize);
        bin = max( 0, min( bin, (width - 1) ) );
        ++binCounts[ bin ];
    }
    array< double, width > sampleFreqs;
    for ( int i = 0; i < width; ++i )
        sampleFreqs[ i ] = ((double) binCounts[ i ]) / sample.size();
    array< double, width > hypothFreqs;
    for ( int i = 0; i < width; ++i )
    {
        double binMin = graphMin  +  i * binSize;
        hypothFreqs[ i ] = hypothFunc( binMin + binSize )
                - hypothFunc( binMin );
    }
    DrawGraph( sampleFreqs, hypothFreqs );
    cout << graphMin << " - " << graphMax << endl;
    cout << endl;
}

//-----------------------------------------------------------------------------

void
DrawGraph( array< double, width > sampleFreqs,
           array< double, width > hypothFreqs )
{
   double maxFreq
            = max( *(max_element( sampleFreqs.begin(), sampleFreqs.end() )),
                   *(max_element( hypothFreqs.begin(), hypothFreqs.end() )) );
    array< string, height > graph;
    for ( int i = 0; i < height; ++i )
        graph[ i ].assign( width, ' ' );
    for ( int i = 0; i < width; ++i )
    {
        //draw x-axis
        graph[ 0 ][ i ] = '_';
        //draw hypothetical pdf
        int y = (int) ((hypothFreqs[ i ] / maxFreq) * (height - 1));
        graph[ y ][ i ] = '-';
        //draw sample pdf
        y = (int) ((sampleFreqs[ i ] / maxFreq) * (height - 1));
        graph[ y ][ i ] = '*';
    }
    cout << endl;
    for ( int y = height - 1; y >= 0; --y )
        cout << graph[ y ] << endl;
 }

//-----------------------------------------------------------------------------
}                                                                   //namespace

//-----------------------------------------------------------------------------

#endif



//*****************************************************************************

}                                                      //namespace EpsilonDelta

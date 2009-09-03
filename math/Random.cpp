/*
  Random.cpp
  Copyright (C) 2007 David M. Anderson

  RandomNumberGenerator class: generates pseudo-random numbers from various
  distributions.
  Random object: Global object of RandomNumberGenerator class.
*/


#include "Random.hpp"
#include "Gamma.hpp"
#if defined( USE_BOOST )
#include <boost/random.hpp>
#endif
#include <ctime>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "ProbabilityDistributions.hpp"
#include "StatisticalTests.hpp"
#include <iostream>
#include <boost/bind.hpp>
#endif
using namespace std;
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
    int Geometric( double probability );
    double Exponential( double lambda );
    double Normal( double mean, double standardDeviation );
    double LogNormal( double mean, double standardDeviation );
    Vector3D UniformOnSphere( );
    std::vector< double > UniformOnSphere( int dimension );

private:
#if defined( USE_BOOST )
    boost::mt19937  m_rng;
    boost::uniform_01< boost::mt19937 > m_rng01;
#endif
};


//*****************************************************************************


RandomNumberGenerator::RandomNumberGenerator( )
    :   m_pImpl( new RandomNumberGeneratorImpl( ) )
{
    time_t t = time( 0 );
    m_seed = static_cast< int >( t );
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
RandomNumberGenerator::Geometric( double probability )
{
    Assert( (probability > 0.) && (probability <= 1.) );
    return m_pImpl->Geometric( probability );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGenerator::Binomial( double probability, int trials )
{
    //From Press, et al., "Numerical Recipes in C++", 2nd Ed., p. 299-300.
    Assert( (probability >= 0.) && (probability <= 1.) );
    Assert( trials > 0 );
    if ( trials < 100 )
    {
        //directly count successes
        int b = 0;
        for ( int i = 0; i < trials; ++i )
            if ( Uni01() < probability )
                ++b;
        return b;
    }

    bool countFailures = false;
    if ( probability > 0.5 )
    {
        probability = 1. - probability;
        countFailures = true;
    }
    double expected = probability * trials;
    if ( expected < 0.1 )
    {
        //Poisson approximation
        int b = 0;
        double g = exp( - expected );
        double t = 1.;
        while ( b < trials )
        {
            t *= Uni01();
            if ( t < g )
                break;
            ++b;
        }
        if ( countFailures )
            b = trials - b;
        return b;
    }
    //rejection method
    static double t = -1.;
    static double lgam;
    if ( t != trials )
    {
        t = trials;
        lgam = LogGamma( t + 1. );
    }
    static double p = -1.;
    static double pf;
    static double logp;
    static double logpf;
    if ( p != probability )
    {
        p = probability;
        pf = 1. - p;
        logp = log( p );
        logpf = log( pf );
    }
    double bb;
    double s = sqrt( 2. * expected * pf );
    double rej;
    do
    {
        double y;
        do
        {
            double v1, v2;
            do
            {
                v1 = Uni01();
                v2 = 2. * Uni01() - 1.;
            } while ( v1*v1 + v2*v2 > 1. );
            y = v2 / v1; //equiv of y = tan( Uni01() * pi )
            bb = floor( y * s  +  expected );
        } while ( (bb < 0.) || (bb > t) );
        rej = 1.2 * s * (1. + y*y)
                * exp( lgam - LogGamma( bb + 1. )
                       -  LogGamma( t - bb + 1. )
                       +  bb * logp  +  ( t - bb ) * logpf );
    } while ( Uni01() > rej );
    int b = static_cast< int >( bb );
    if ( countFailures )
        b = trials - b;
    return b;
}

//-----------------------------------------------------------------------------

int 
RandomNumberGenerator::Poisson( double mean )
{
    Assert( mean >= 0. );
    //From Press, et al., "Numerical Recipes in C++", 2nd Ed., p. 298-299.
    static double m = -1.;
    if ( mean < 12. )
    {
        //directly count successes
        static double e;
        if ( m != mean )
        {
            m = mean;
            e = exp( -m );
        }
        int n = -1;
        double t = 1.;
        do
        {
            ++n;
            t *= Uni01();
        } while ( t > e );  //equiv to sum of -log(Uni01()) < m
        return n;
    }

    //rejection method
    static double s;
    static double l;
    static double g;
    if ( m != mean )
    {
        m = mean;
        s = sqrt( 2. * m );
        l = log( m );
        g = m * l - LogGamma( m + 1. );
    }
    double nn;
    double rej;
    do
    {
        double y;
        do
        {
            double v1, v2;
            do
            {
                v1 = Uni01();
                v2 = 2. * Uni01() - 1.;
            } while ( v1*v1 + v2*v2 > 1. );
            y = v2 / v1; //equiv of y = tan( Uni01() * pi )
            nn = floor( y * s  +  m );
        } while ( nn < 0. );
        rej = 0.9 * (1. + y*y) * exp( nn * l  -  LogGamma( nn + 1. ) - g );
    } while ( Uni01() > rej );
    return static_cast< int >( nn );
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
    //Algorithm from Donald Knuth, "The Art of Computer Programming", Vol 2,
    // 2nd Ed., p. 129, and Press, et al., "Numerical Recipes in C++", 2nd Ed.,
    // p 296-7.
    if ( n < 6 )
    {
        //directly accumulate successes
        double g = 1.;
        for ( int i = 0; i < n; ++i )
            g *= Uni01();
        return - log( g ) / lambda;
    }

    //rejection method
    double g;
    const double s = sqrt( 2. * n  -  1. );
    const double n1 = n - 1.;
    double rej;
    do
    {
        double y;
        do
        {
            double v1, v2;
            do
            {
                v1 = Uni01();
                v2 = 2. * Uni01() - 1.;
            } while ( v1*v1 + v2*v2 > 1. );
            y = v2 / v1; //equiv of y = tan( Uni01() * pi )
            g = s * y  +  n1;
        } while ( g <= 0. );
        rej = (1. + y * y) * exp( n1 * log( g / n1 ) - s * y );
    } while ( Uni01() > rej );
    return g / lambda;
}

//-----------------------------------------------------------------------------

double 
RandomNumberGenerator::Normal( double mean, double standardDeviation )
{
    Assert( standardDeviation > 0. );
    return m_pImpl->Normal( mean, standardDeviation );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGenerator::LogNormal( double mean, double standardDeviation,
                                  bool momentsOfLog )
{
    Assert( standardDeviation > 0. );
    if ( momentsOfLog )
    {
        double variance = standardDeviation * standardDeviation;
        double m = exp( mean  +  variance / 2. );
        double v = (exp( variance ) - 1.) * exp( 2. * mean  +  variance );
        double s = sqrt( v );
        return m_pImpl->LogNormal( m, s );
    }
    Assert( mean > 0. );
    return m_pImpl->LogNormal( mean, standardDeviation );
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

//=============================================================================

double 
RandomNumberGenerator::Uni01( )
{
    return (*m_pImpl)( 0., 1. );
}


//*****************************************************************************


#if defined( USE_BOOST )

//-----------------------------------------------------------------------------

RandomNumberGeneratorImpl::RandomNumberGeneratorImpl( )
    :   m_rng01( m_rng )
{
}

//=============================================================================

void 
RandomNumberGeneratorImpl::Reseed( int seed )
{
    m_rng.seed( static_cast< uint32_t >( seed ) );
}

//=============================================================================

int 
RandomNumberGeneratorImpl::operator()( )
{
    return static_cast< int >( m_rng( ) );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGeneratorImpl::operator()( int limit )
{
    const int smallIntLimit = 1000;
    if ( limit < smallIntLimit )
    {
        boost::uniform_smallint<> distribution( 0, limit - 1 );
        return distribution( m_rng );
    }
    else
    {
        boost::uniform_int<> distribution( 0, limit - 1 );
        return distribution( m_rng );
    }
}

//-----------------------------------------------------------------------------

double
RandomNumberGeneratorImpl::operator()( double minimum, double maximum )
{
    boost::uniform_real<> distribution( minimum, maximum );
    return distribution( m_rng01 );
}

//=============================================================================

double 
RandomNumberGeneratorImpl::Triangle( double minimum, double mode,
                                     double maximum )
{
    boost::triangle_distribution<> distribution( minimum, mode, maximum );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

bool 
RandomNumberGeneratorImpl::Bernoulli( double probability )
{
    boost::bernoulli_distribution<> distribution( probability );
    return distribution( m_rng );
}

//-----------------------------------------------------------------------------

int 
RandomNumberGeneratorImpl::Geometric( double probability )
{
    //boost interprets probability as that of failure, not success.
    boost::geometric_distribution<> distribution( 1. - probability );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGeneratorImpl::Exponential( double lambda )
{
    boost::exponential_distribution<> distribution( lambda );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGeneratorImpl::Normal( double mean, double standardDeviation )
{
    boost::normal_distribution<> distribution( mean, standardDeviation );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

double 
RandomNumberGeneratorImpl::LogNormal( double mean, double standardDeviation )
{
    boost::lognormal_distribution<> distribution( mean, standardDeviation );
    return distribution( m_rng01 );
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
    boost::uniform_on_sphere<> distribution( dimension );
    return distribution( m_rng01 );
}

//-----------------------------------------------------------------------------

#endif //USE_BOOST


//*****************************************************************************


QuickRandomNumberGenerator::QuickRandomNumberGenerator( )
{
    time_t t = time( 0 );
    m_seed = static_cast< uint32_t >( t );
}


//*****************************************************************************


#ifdef DEBUG

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
    double prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Random( 256 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sampleFreqs.clear();
    expectedFreqs.clear();

    vector< double > sample( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random( 1.5, 3.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Uniform_DF, _1, 1.5, 3.5 ) );
    cout << "KolmogorovSmirnovTest of Random( 1.5, 3.5 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Triangle( -3.5, -1., 0.5 );
    prob = KolmogorovSmirnovTest( sample,
                                  bind( Triangle_DF, _1, -3.5, -1., 0.5 ) );
    cout << "KolmogorovSmirnovTest of Triangle( -3.5, -1., 0.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Bernoulli( 0.75 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Geometric( 0.1 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.2, 9 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.8, 19 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.002, 49 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.0002, 499 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Binomial( 0.1, 499 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Poisson( 10.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of Poisson( 15.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sampleFreqs.clear();
    expectedFreqs.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Exponential( 0.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Exponential_DF, _1, 0.5 ) );
    cout << "KolmogorovSmirnovTest of Exponential( 0.5 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Gamma( 11, 0.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Gamma_DF, _1, 11, 0.5 ) );
    cout << "KolmogorovSmirnovTest of Gamma( 11, 0.5 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.Normal( -12.2, 4.4 );
    prob = KolmogorovSmirnovTest( sample, bind( Normal_DF, _1, -12.2, 4.4 ) );
    cout << "KolmogorovSmirnovTest of Normal( -12.2, 4.4 ) = " << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.LogNormal( 2.2, 4.4 );
    prob = KolmogorovSmirnovTest( sample,
                                 bind( LogNormal_DF, _1, 2.2, 4.4, false ) );
    cout << "KolmogorovSmirnovTest of LogNormal( 2.2, 4.4 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sample.clear();

    sample.resize( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = Random.LogNormal( -2.2, 1.4, true );
    prob = KolmogorovSmirnovTest( sample,
                                  bind( LogNormal_DF, _1, -2.2, 1.4, true ) );
    cout << "KolmogorovSmirnovTest of LogNormal( -2.2, 1.4, true ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of consecutive pairs of Random(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of gaps in Random(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of gaps in Random(0.,1.) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    double prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of QuickRandom( 256 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sampleFreqs.clear();
    expectedFreqs.clear();

    vector< double > sample( 100000 );
    for ( int i = 0; i < 100000; ++i )
        sample[i] = QuickRandom( 1.5, 3.5 );
    prob = KolmogorovSmirnovTest( sample, bind( Uniform_DF, _1, 1.5, 3.5 ) );
    cout << "KolmogorovSmirnovTest of QuickRandom( 1.5, 3.5 ) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of consecutive pairs of"
            " QuickRandom(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of gaps in QuickRandom(64) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
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
    prob = ChiSquareGoodnessOfFitTest( sampleFreqs, expectedFreqs );
    cout << "ChiSquareGoodnessOfFitTest of gaps in QuickRandom(0.,1.) = "
         << prob << endl;
    TESTCHECK( (prob > 0.01), true, &ok );
    sampleFreqs.clear();
    expectedFreqs.clear();

    if ( ok )
        cout << "QuickRandom PASSED." << endl << endl;
    else
        cout << "QuickRandom FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

#endif



//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  StatisticalTests.cpp
  Copyright (C) 2007 David M. Anderson

  Basic statistical estimates and hypothesis tests.
*/


#include "StatisticalTests.hpp"
#include "Factorial.hpp"
#include <limits>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


double 
Mean( const vector< double > & sample )
{
    Assert( sample.size() > 0 );
    double sum = 0.;
    typedef vector< double >::const_iterator iter;
    for ( iter p = sample.begin(); p != sample.end(); ++p )
        sum += *p;
    return sum / sample.size();
}

//-----------------------------------------------------------------------------

double 
Variance( const vector< double > & sample, double mean )
{
    int sampleSize = sample.size();
    Assert( sampleSize > 1 );
    //Chan, T.F., et al., American Statistician, vol 37 (1983), p 242-47.
    double diffSum = 0.;
    double diffSqrSum = 0.;
    typedef vector< double >::const_iterator iter;
    for ( iter p = sample.begin(); p != sample.end(); ++p )
    {
        double diff = *p - mean;
        diffSum += diff;
        diffSqrSum += diff * diff;
    }
    return ( diffSqrSum  -  (diffSum * diffSum) / sampleSize )
            / (sampleSize - 1.);
}

//-----------------------------------------------------------------------------

double 
Median( vector< double > & sample )
{
    int sampleSize = sample.size();
    Assert( sampleSize > 0 );
    int n2 = (sampleSize - 1) / 2;
    nth_element( sample.begin(), sample.begin() + n2, sample.end() );
    double median = sample[n2];
    if ( (sampleSize & 1) == 0 ) //even
    {
        nth_element( sample.begin(), sample.begin() + n2 + 1, sample.end() );
        median += sample[ n2 + 1 ];
        median /= 2.;
    }
    return median;
}

//=============================================================================

TTestResult
MeanTest( int sampleSize, double sampleMean, double sampleVariance,
          double hypothMean, Tail tail )
{
    Assert( sampleSize > 1 );
    Assert( sampleVariance >= 0. );
    double prob = 0.;
    double t = 0.;
    int degreesOfFreedom = sampleSize - 1;
    if ( sampleVariance == 0. )
    {
        if ( (sampleMean == hypothMean)
             || ( (tail == LOWER_TAIL) && (sampleMean >= hypothMean) )
             || ( (tail == UPPER_TAIL) && (sampleMean <= hypothMean) ) )
            prob = 1.;
        else
            prob = 0.;
        t = (sampleMean == hypothMean)  ?  0.
                :  (sampleMean < hypothMean)
                ?  - numeric_limits< double >::infinity()
                :  numeric_limits< double >::infinity();
    }
    else
    {
        t = (sampleMean - hypothMean) * sqrt( (double) sampleSize )
                / sqrt( sampleVariance );
        prob = StudentsT_DF( t, degreesOfFreedom );
        if ( tail == UPPER_TAIL )
            prob = 1. - prob;
        else if ( tail == BOTH_TAILS )
            prob = 2. * min( prob, 1. - prob );
    }
    TTestResult tResult = { prob, t, degreesOfFreedom };
    return tResult;
}

//-----------------------------------------------------------------------------

ChiSquareTestResult 
VarianceTest( int sampleSize, double sampleVariance, double hypothVariance,
              Tail tail )
{
    Assert( sampleSize > 1 );
    Assert( sampleVariance >= 0. );
    Assert( hypothVariance >= 0. );
    double prob = 0.;
    double chiSquare = 0.;
    int degreesOfFreedom = sampleSize - 1;
    if ( hypothVariance == 0. )
    {
        prob = (sampleVariance == 0.)  ?  1.  :  0.;
        chiSquare = (sampleVariance == 0.)  ?  0.
                :  numeric_limits< double >::infinity();
    }
    else
    {
        chiSquare = sampleVariance * (sampleSize - 1.) / hypothVariance;
        prob = ChiSquare_DF( chiSquare, degreesOfFreedom );
    }
    if ( tail == UPPER_TAIL )
        prob = 1. - prob;
    else if ( tail == BOTH_TAILS )
        prob = 2. * min( prob, 1. - prob );
    ChiSquareTestResult chiSquareResult
            = { prob, chiSquare, degreesOfFreedom };
    return chiSquareResult;
}

//=============================================================================

ChiSquareTestResult
ChiSquareGoodnessOfFitTest( const vector< int > & sampleFreqs,
                            const vector< double > & hypothFreqs,
                            bool probabilities, int constraints )
{
    int numBins = sampleFreqs.size();
    Assert( static_cast< int >( hypothFreqs.size() ) == numBins );
    int degreesOfFreedom = numBins - constraints;
    int sampleSize = 0;
    if ( probabilities )
    {
        --degreesOfFreedom;
        for ( int i = 0; i < numBins; ++i )
            sampleSize += sampleFreqs[i];
    }
    double chiSquare = 0.;
    for ( int i = 0; i < numBins; ++i )
    {
        double hypFreq = hypothFreqs[i];
        if ( probabilities )
            hypFreq *= sampleSize;
        if ( (hypFreq == 0.) && (sampleFreqs[i] > 0) )
        {   //hypothesis is clearly false
            ChiSquareTestResult chiSquareResult = { 0., 0., 0 };
            return chiSquareResult;
        }
        if ( (hypFreq < 0.5) && (sampleFreqs[i] == 0) )
        {
            --degreesOfFreedom;
            continue;
        }
        double diff = sampleFreqs[i] - hypFreq;
        chiSquare += diff * diff / hypFreq;
    }
    double prob = 1. - ChiSquare_DF( chiSquare, degreesOfFreedom );
    ChiSquareTestResult chiSquareResult = { prob, chiSquare, degreesOfFreedom };
    return chiSquareResult;
}

//=============================================================================

TTestResult
MeansTest( int sampleSize1, double sampleMean1, double sampleVariance1,
           int sampleSize2, double sampleMean2, double sampleVariance2,
           bool equalVariances, Tail tail )
{
    Assert( sampleSize1 > 1. );
    Assert( sampleSize2 > 1. );
    Assert( sampleVariance1 >= 0. );
    Assert( sampleVariance2 >= 0. );
    double pooledVariance;
    int degreesOfFreedom;
    if ( equalVariances )
    {
        pooledVariance = ((sampleVariance1 * (sampleSize1 - 1.)
                           +  sampleVariance2 * (sampleSize2 - 1.))
                          / (sampleSize1 + sampleSize2 - 2.))
                *  ((1. / sampleSize1) + (1. / sampleSize2));
        degreesOfFreedom = sampleSize1 + sampleSize2 - 2;
    }
    else
    {
        double vn1 = sampleVariance1 / sampleSize1;
        double vn2 = sampleVariance2 / sampleSize2;
        pooledVariance = vn1 + vn2;
        double dof = ( ((vn1 + vn2) * (vn1 + vn2))
                / ((vn1 * vn1 / (sampleSize1 - 1.))
                   +  (vn2 * vn2 / (sampleSize2 - 1.))) );
        degreesOfFreedom = (int) dof;
    }
    double prob = 0.;
    double t = 0.;
    Assert( pooledVariance >= 0. );
    if ( pooledVariance == 0. )
    {
        if ( (sampleMean1 == sampleMean2)
             || ( (tail == LOWER_TAIL) && (sampleMean1 >= sampleMean2) )
             || ( (tail == UPPER_TAIL) && (sampleMean1 <= sampleMean2) ) )
            prob = 1.;
        else
            prob = 0.;
        t = (sampleMean1 == sampleMean2)  ?  0.
                :  (sampleMean1 < sampleMean2)
                ?  - numeric_limits< double >::infinity()
                :  numeric_limits< double >::infinity();
    }
    else
    {
        t = (sampleMean1 - sampleMean2) / sqrt( pooledVariance );
        prob = StudentsT_DF( t, degreesOfFreedom );
        if ( tail == UPPER_TAIL )
            prob = 1. - prob;
        else if ( tail == BOTH_TAILS )
            prob = 2. * min( prob, 1. - prob );
    }
    TTestResult tResult = { prob, t, degreesOfFreedom };
    return tResult;
}

//-----------------------------------------------------------------------------

FTestResult
VariancesTest( int sampleSize1, double sampleVariance1,
               int sampleSize2, double sampleVariance2,
               Tail tail )
{
    Assert( sampleSize1 > 1 );
    Assert( sampleSize2 > 1 );
    Assert( sampleVariance1 >= 0. );
    Assert( sampleVariance2 >= 0. );
    double prob = 0.;
    double f = 0.;
    int dof1 = (sampleSize1 - 1);
    int dof2 = (sampleSize2 - 1);
    if ( sampleVariance2 == 0. )
    {
        prob = (sampleVariance1 == 0.) ? 1. : 0.;
        f = (sampleVariance1 == 0.)  ?  0.
                :  numeric_limits< double >::infinity();
    }
    else
    {
        f = sampleVariance1 / sampleVariance2;
        prob = F_DF( f, dof1, dof2 );
        if ( tail == UPPER_TAIL )
            prob = 1. - prob;
        else if ( tail == BOTH_TAILS )
            prob = 2. * min( prob, 1. - prob );
    }
    FTestResult fResult = { prob, f, dof1, dof2 };
    return fResult;
}

//=============================================================================

double 
MediansTest( const vector< double > & sample1,
             const vector< double > & sample2, Tail tail )
{
    vector< double > grandSample = sample1;
    grandSample.insert( grandSample.end(), sample2.begin(), sample2.end() );
    double grandMedian = Median( grandSample );
    int numBelowMedian = 0;
    int numAboveMedian = 0;
    typedef vector< double >::const_iterator iter;
    for ( iter p = sample1.begin(); p != sample1.end(); ++p )
    {
        if ( *p < grandMedian )
            ++numBelowMedian;
        else if ( *p > grandMedian )
            ++numAboveMedian;
    }
    int grandSize = grandSample.size();
    int sampleSize1 = sample1.size();
    int x = 0;
    if ( tail == LOWER_TAIL )
        x = numAboveMedian;
    else if ( tail == UPPER_TAIL )
        x = numBelowMedian;
    else //BOTH_TAILS
        x = min( numBelowMedian, numAboveMedian );
    double prob = Hypergeometric_DF( x, grandSize, grandSize / 2,
                                     sampleSize1 );
    if ( tail == BOTH_TAILS )
        prob = 2. * min( prob, 1. - prob );
    return prob;
}

//=============================================================================

double 
KolmogorovSmirnovTest( vector< double > & sample1, vector< double > & sample2,
                       bool sorted )
{
    if ( ! sorted )
    {
        sort( sample1.begin(), sample1.end() );
        sort( sample2.begin(), sample2.end() );
    }
    double d = 0.;
    int sampleSize1 = sample1.size();
    int sampleSize2 = sample2.size();
    double recipSize1 = 1. / sampleSize1;
    double recipSize2 = 1. / sampleSize2;
    double DF1 = 0.;
    double DF2 = 0.;
    int i1 = 0;
    int i2 = 0;
    while ( (i1 < sampleSize1) && (i2 < sampleSize2) )
    {
        double s1 = sample1[ i1 ];
        double s2 = sample2[ i2 ];
        if ( s1 <= s2 )
        {
            ++i1;
            DF1 += recipSize1;
        }
        if ( s2 <= s1 )
        {
            ++i2;
            DF2 += recipSize2;
        }
        double diff = fabs( DF1 - DF2 );
        if ( diff > d )
            d = diff;
    }
    double sqrtSize = sqrt( static_cast< double >( sampleSize1 * sampleSize2 )
                            / (sampleSize1 + sampleSize2) );
    double x = d * (sqrtSize  +  0.12  +  0.11 / sqrtSize);
    return 1. - KolmogorovSmirnov_DF( x );
}

//=============================================================================

CorrelationTestResult
LinearCorrelationTest( const vector< array< double, 2 > > & samples, Tail tail )
{
    int sampleSize = samples.size();
    Assert( sampleSize > 3 );
    double mean0 = 0.;
    double mean1 = 0.;
    typedef vector< array< double, 2 > >::const_iterator iter;
    for ( iter p = samples.begin(); p != samples.end(); ++p )
    {
        mean0 += (*p)[0];
        mean1 += (*p)[1];
    }
    mean0 /= sampleSize;
    mean1 /= sampleSize;
    double sum0 = 0.;
    double sum1 = 0.;
    double sum01 = 0.;
    double sum00 = 0.;
    double sum11 = 0.;
    for ( iter p = samples.begin(); p != samples.end(); ++p )
    {
        double diff0 = (*p)[0] - mean0;
        double diff1 = (*p)[1] - mean1;
        sum0 += diff0;
        sum1 += diff1;
        sum01 += diff0 * diff1;
        sum00 += diff0 * diff0;
        sum11 += diff1 * diff1;
    }
    //Chan, T.F., et al., American Statistician, vol 37 (1983), p 242-47.
    double variance0 = ( sum00  -  (sum0 * sum0) / sampleSize )
            / (sampleSize - 1.);
    double variance1 = ( sum11  -  (sum1 * sum1) / sampleSize )
            / (sampleSize - 1.);
    double covariance = ( sum01 - (sum0 * sum1) / sampleSize )
            / (sampleSize - 1.);
    double denom = sqrt( variance0 * variance1 );
    double r = 0.;
    double t = 0.;
    double prob = 0.;
    if ( denom == 0. )
        r = (covariance < 0) ? -1. : 1.;
    else
        r = covariance / denom;
    if ( r <= -1. )
    {
        t = - numeric_limits< double >::infinity();
        prob = 0.;
    }
    else if ( r >= 1. )
    {
        t = numeric_limits< double >::infinity();
        prob = 0.;
    }
    else
    {
        t = r * sqrt( (sampleSize - 2.) / (1. - r * r) );
        prob = StudentsT_DF( t, sampleSize - 2 );
        if ( tail == UPPER_TAIL )
            prob = 1. - prob;
        else if ( tail == BOTH_TAILS )
            prob = 2. * min( prob, 1. - prob );
    }

    CorrelationTestResult result = { { prob, t, sampleSize - 2 }, r,
                                     mean0, mean1, variance0, variance1 };
    return result;
}

//=============================================================================

SimpleRegressionResult
SimpleLinearRegression( int sampleSize,
                        double meanX, double meanY,
                        double varianceX, double varianceY,
                        double pearsonsR )
{
    Assert( sampleSize > 2 );
    Assert( varianceX > 0. );
    Assert( varianceY >= 0. );
    double beta = pearsonsR * sqrt( varianceY / varianceX );
    double alpha = meanY - beta * meanX;
    double residualVariance = ((sampleSize - 1.) / (sampleSize - 2.))
            * varianceY * (1. - pearsonsR * pearsonsR);
    double varianceAlpha = residualVariance
            * ((1. / sampleSize)
               +  (meanX * meanX) / ((sampleSize - 1.) * varianceX));
    double varianceBeta = residualVariance / ((sampleSize - 1) * varianceX);
    SimpleRegressionResult result = { alpha, beta, varianceAlpha,
                                      varianceBeta, residualVariance };
    return result;
}
    
//=============================================================================

namespace
{                                                                   //namespace

class CompareArrays2
{
public:
    CompareArrays2( int index )
        :   m_index( index )
    {
    }

    bool operator()( const array< double, 2 > & arr1,
              const array< double, 2 > & arr2 )
    {
        return ( arr1[ m_index ] < arr2[ m_index ] );
    }

private:
    int m_index;
};

}                                                                   //namespace

//.............................................................................

CorrelationTestResult
SpearmansRankCorrelationTest( const vector< array< double, 2 > > & samples,
                              Tail tail )
{
    int sampleSize = samples.size();
    Assert( sampleSize > 3 );
    vector< array< double, 2 > > ranks = samples;
    for ( int i = 0; i < 2; ++i )
    {
        sort( ranks.begin(), ranks.end(), CompareArrays2( i ) );
        int j = 1;
        while ( j < sampleSize )
        {
            if ( ranks[ j - 1 ][ i ] != ranks[ j ][ i ] )
            {
                ranks[ j - 1 ][ i ] = j;
                ++j;
            }
            else
            {
                int k = j + 1;
                while ( (k <= sampleSize)
                        && (ranks[ k - 1 ][ i ] == ranks[ j - 1 ][ i ]) )
                    ++k;
                double aveRank = 0.5 * (j + k - 1);
                for ( int m = j; m <= k - 1; ++m )
                    ranks[ m - 1 ][ i ] = aveRank;
                j = k;
            }
        }
        if ( j == sampleSize )
            ranks[ j - 1 ][ i ] = j;
    }
    return LinearCorrelationTest( ranks, tail );
}

//-----------------------------------------------------------------------------

KendallsTauTestResult
KendallsTauTest( const std::vector< array< double, 2 > > & samples, Tail tail )
{
    int sampleSize = samples.size();
    Assert( sampleSize > 1 );
    int concordantMinusDiscordant = 0;
    int total0 = 0;
    int total1 = 0;
    for ( int i = 0; i < sampleSize; ++i )
        for ( int j = 0; j < sampleSize; ++j )
        {
            double diff0 = samples[i][0] - samples[j][0];
            double diff1 = samples[i][1] - samples[j][1];
            double rel01 = diff0 * diff1;
            if ( rel01 > 0. )   //concordant
            {
                ++concordantMinusDiscordant;
                ++total0;
                ++total1;
            }
            else if ( rel01 < 0. )  //discordant
            {
                --concordantMinusDiscordant;
                ++total0;
                ++total1;
            }
            else //a tie
            {
                if ( diff0 != 0. )
                    ++total0;
                if ( diff1 != 0. )
                    ++total1;
            }
        }
    double total01 = total0 * total1;
    Assert( total01 > 0. );
    double tau = concordantMinusDiscordant / sqrt( total01 );
    double var = (4. * sampleSize  +  10.)
            / (9. * sampleSize * (sampleSize - 1.));
    double prob = Normal_DF( tau, 0., var );
    if ( tail == UPPER_TAIL )
        prob = 1. - prob;
    else if ( tail == BOTH_TAILS )
        prob = 2. * min( prob, 1. - prob );
    KendallsTauTestResult result = { prob, tau };
    return result;
}

//=============================================================================

ContingencyTableResult
ChiSquareContingencyTableTest( const TwoDArray< int > & table,
                               bool yatesCorrection )
{
    return ChiSquareContingencyTableTest( table,
                                          table.NumRows(), table.NumColumns(),
                                          yatesCorrection );
}

//.............................................................................

ContingencyTableResult
ChiSquareContingencyTableTest( const vector< vector< int > > & table,
                               bool yatesCorrection )
{
    return ChiSquareContingencyTableTest( table,
                                          table.size(), table[0].size(),
                                          yatesCorrection );
}

//-----------------------------------------------------------------------------

double 
FishersExactTest( const array< array< int, 2 >, 2 > & table, int tails )
{
    //locate smallest cell value
    int i0 = 0;
    int j0 = 0;
    int minVal = table[0][0];
    for ( int i = 0; i < 2; ++i )
        for ( int j = 0; j < 2; ++j )
            if ( table[i][j] < minVal )
            {
                i0 = i;
                j0 = j;
                minVal = table[i][j];
            }
    int i1 = 1 - i0;
    int j1 = 1 - j0;
    int a = table[i0][j0];
    int b = table[i0][j1];
    int c = table[i1][j0];
    int d = table[i1][j1];
    double prob = Hypergeometric_DF( a, (a + b + c + d), (a + b), (a + c) );
    if ( tails == 2 )
    {
        double prA = Hypergeometric_PDF( a, (a + b + c + d), (a + b), (a + c) );
        int maxA = min( (a + b), (a + c) );
        for ( int i = maxA; i > a; --i )
        {
            double pr
                   = Hypergeometric_PDF( i, (a + b + c + d), (a + b), (a + c) );
            if ( pr <= prA )
                prob += pr;
            else
                break;
        }
    }
    return prob;
}

//=============================================================================

#ifdef DEBUG

bool 
TestStatisticalTests( )
{
    bool ok = true;
    cout << "Testing Statistical Tests" << endl;

    double estsSampleArr[]
            = { 6., 9., 10., 12., 13., 14., 14., 15., 16., 16.,
                16., 17., 17., 18., 18., 19., 20., 21., 22., 24. };
    vector< double > estsSample( estsSampleArr,
                            estsSampleArr + ARRAY_LENGTH( estsSampleArr ) );
    TESTCHECKF( Mean( estsSample ), 15.85, &ok );
    TESTCHECKF( Variance( estsSample, 15.85 ), 19.923684, &ok );
    TESTCHECK( Median( estsSample ), 16., &ok );
    cout << "MeanTest( 5, 767., 81225., 1000., LOWER_TAIL )" << endl;
    TTestResult tResult = MeanTest( 5, 767., 81225., 1000., LOWER_TAIL );
    TESTCHECKFE( tResult.probability, 0.071, &ok, 0.001 );
    TESTCHECKFE( tResult.t, -1.83, &ok, 0.01 );
    TESTCHECK( tResult.degreesOfFreedom, 4, &ok );
    cout << "MeanTest( 15, 442.2, 1936., 420., BOTH_TAILS )" << endl;
    tResult = MeanTest( 15, 442.2, 1936., 420., BOTH_TAILS );
    TESTCHECKFE( tResult.probability, 0.070, &ok, 0.001 );
    TESTCHECKFE( tResult.t, 1.95, &ok, 0.01 );
    TESTCHECK( tResult.degreesOfFreedom, 14, &ok );
    cout << "VarianceTest( 13, 51.84, 225., BOTH_TAILS )" << endl;
    ChiSquareTestResult chiSquareResult
            = VarianceTest( 13, 51.84, 225., BOTH_TAILS );
    TESTCHECKFE( chiSquareResult.probability, 0.0060, &ok, 0.0001 );
    TESTCHECKFE( chiSquareResult.chiSquare, 2.765, &ok, 0.001 );
    TESTCHECK( chiSquareResult.degreesOfFreedom, 12, &ok );
    //!!!KolmogorovSmirnovTest
    int digitsSampleArr[] = { 0, 15, 0, 76, 479, 183, 8, 23, 0 };
    vector< int > digitsSample( digitsSampleArr, digitsSampleArr
                                + ARRAY_LENGTH( digitsSampleArr ) );
#if 0
    vector< double > expectedDigits( 9 );
    for ( int i = 1; i <= 9; ++i )  //Benford's law (of leading digits)
        expectedDigits[ i - 1 ] = log10( 1.  +  1. / i );
#else
    double expectedDigitsArr[] = { 0.301, 0.176, 0.125, 0.097, 0.079,
                                   0.067, 0.058, 0.051, 0.046 };
    vector< double > expectedDigits( expectedDigitsArr, expectedDigitsArr
                                     + ARRAY_LENGTH( expectedDigitsArr ) );
#endif
    cout << "ChiSquareGoodnessOfFitTest( digitsSample, expectedDigits )"
         << endl;
    chiSquareResult
          = ChiSquareGoodnessOfFitTest( digitsSample, expectedDigits, true, 0 );
    TESTCHECKFE( chiSquareResult.probability, 0., &ok, 0.0001 );
    TESTCHECKF( chiSquareResult.chiSquare, 3650.251, &ok );
    TESTCHECK( chiSquareResult.degreesOfFreedom, 8, &ok );
    cout << "MeansTest( 70, 418.5, 2070.25, 73, 403.7, 936.36, "
            "false, UPPER_TAIL )" << endl;
    tResult = MeansTest( 70, 418.5, 2070.25,   //McGwire's 1998 HR distances
                         73, 403.7, 936.36,    //Bonds's 2001 HR distances
                         false, UPPER_TAIL );
    TESTCHECKFE( tResult.probability, 0.0124, &ok, 0.0001 );
    TESTCHECKFE( tResult.t, 2.273, &ok, 0.0001 );
    cout << "VariancesTest( 25, 47.01983, 16, 18.1489, BOTH_TAILS )" << endl;
    FTestResult fResult = VariancesTest( 25, 47.01983, 16, 18.1489,
                                         BOTH_TAILS );
    TESTCHECKFE( fResult.probability, 0.059856, &ok, 0.000002 );
    TESTCHECKF( fResult.F, 2.590782, &ok );
    TESTCHECK( fResult.degreesOfFreedom1, 24, &ok );
    TESTCHECK( fResult.degreesOfFreedom2, 15, &ok );
    //!!!MediansTest
    //!!!KolmogorovSmirnov
    array< double, 2 > corrSampleArr[]
            = { {{ 68., 53. }}, {{ 68., 38. }}, {{ 67., 35. }},
                {{ 70., 49. }}, {{ 71., 42. }}, {{ 73., 60. }},
                {{ 76., 54. }}, {{ 81., 67. }}, {{ 83., 82. }},
                {{ 84., 78 }} };
    vector< array< double, 2 > > corrVec;
    for ( int i = 0; i < ARRAY_LENGTH( corrSampleArr ); ++i )
        corrVec.push_back( corrSampleArr[i] );
    cout << "LinearCorrelationTest( corrVec, BOTH_TAILS )" << endl;
    CorrelationTestResult corrRslt
            = LinearCorrelationTest( corrVec, BOTH_TAILS );
    TESTCHECKFE( corrRslt.tResult.probability, 0., &ok, 0.001 );
    TESTCHECKFE( corrRslt.r, 0.922, &ok, 0.001 );
    TESTCHECKFE( corrRslt.tResult.t, 6.712, &ok, 0.0001 );
    array< double, 2 > regressionSampleArr[]
            = { {{ 1.47, 52.21 }}, {{ 1.50, 53.12 }}, {{ 1.52, 54.48 }},
                {{ 1.55, 55.84 }}, {{ 1.57, 57.20 }}, {{ 1.60, 58.57 }},
                {{ 1.63, 59.93 }}, {{ 1.65, 61.29 }}, {{ 1.68, 63.11 }},
                {{ 1.70, 64.47 }}, {{ 1.73, 66.28 }}, {{ 1.75, 68.10 }},
                {{ 1.78, 69.92 }}, {{ 1.80, 72.19 }}, {{ 1.83, 74.46 }} };
    vector< array< double, 2 > > regressionVec;
    for ( int i = 0; i < ARRAY_LENGTH( regressionSampleArr ); ++i )
        regressionVec.push_back( regressionSampleArr[i] );
    corrRslt = LinearCorrelationTest( regressionVec, BOTH_TAILS );
    SimpleRegressionResult regressionRslt
            = SimpleLinearRegression( (int)regressionVec.size(),
                                      corrRslt.mean0, corrRslt.mean1,
                                      corrRslt.variance0, corrRslt.variance1,
                                      corrRslt.r );
    TESTCHECKFE( regressionRslt.alpha, -39.062, &ok, 0.001 );
    TESTCHECKFE( regressionRslt.beta, 61.272, &ok, 0.001 );
    TESTCHECKFE( regressionRslt.varianceAlpha, 8.63185, &ok, 0.00001 );
    TESTCHECKFE( regressionRslt.varianceBeta, 3.1539, &ok, 0.0001 );
    TESTCHECKFE( regressionRslt.residualVariance, 0.5762, &ok, 0.0001 );
    array< double, 2 > rankCorrSampleArr[]
            = { {{ 71., 125. }}, {{ 70.5, 119. }}, {{ 71., 128. }},
                {{ 72., 128. }}, {{ 70., 119. }}, {{ 70., 127. }},
                {{ 66.5, 105. }}, {{ 70., 123. }}, {{ 71., 115. }} };
    vector< array< double, 2 > > rankCorrVec;
    for ( int i = 0; i < ARRAY_LENGTH( rankCorrSampleArr ); ++i )
        rankCorrVec.push_back( rankCorrSampleArr[i] );
    cout << "SpearmansRankCorrelationTest( rankCorrVec )" << endl;
    corrRslt = SpearmansRankCorrelationTest( rankCorrVec, BOTH_TAILS );
    //!!!prob
    TESTCHECKFE( corrRslt.r, 0.557, &ok, 0.001 );
    //!!!t
    //!!!KendallsTau
    int contingSampleArr[2][2] = { { 560, 308 }, { 240, 92 } };
    TwoDArray< int > conting2DArr( 2, 2 );
    for ( int i = 0; i < 2; ++i )
        for ( int j = 0; j < 2; ++j )
            conting2DArr[i][j] = contingSampleArr[i][j];
    cout << "ChiSquareContingencyTableTest( conting2DArr )" << endl;
    ContingencyTableResult contingencyRslt
            = ChiSquareContingencyTableTest( conting2DArr );
    TESTCHECKFE( contingencyRslt.chiSquareResult.probability, 0.011, &ok,
                 0.001 );
    TESTCHECKFE( contingencyRslt.chiSquareResult.chiSquare, 6.529, &ok,
                 0.0001 );
    vector< vector< int > > contingVec;
    for ( int i = 0; i < 2; ++i )
    {
        vector< int > v;
        for ( int j = 0; j < 2; ++j )
            v.push_back( contingSampleArr[i][j] );
        contingVec.push_back( v );
    }
    cout << "ChiSquareContingencyTableTest( contingVec )" << endl;
    contingencyRslt = ChiSquareContingencyTableTest( contingVec );
    TESTCHECKFE( contingencyRslt.chiSquareResult.probability, 0.011, &ok,
                 0.001 );
    TESTCHECKFE( contingencyRslt.chiSquareResult.chiSquare, 6.529, &ok,
                 0.0001 );
    cout << "Fisher's exact test" << endl;
    array< array< int, 2 >, 2 > fisherTable
            = {{ {{ 7, 2 }}, {{ 1, 4 }} }};
    cout << " (one tail)" << endl;
    double prob = FishersExactTest( fisherTable, 1 );
    TESTCHECKFE( prob, 0.063, &ok, 0.001 );
    cout << " (two tail)" << endl;
    prob = FishersExactTest( fisherTable, 2 );
    TESTCHECKFE( prob, 0.091, &ok, 0.001 );
    cout << "ChiSquareContingencyTableTest( fisherTable, true )" << endl;
    contingencyRslt = ChiSquareContingencyTableTest( fisherTable, true );
    TESTCHECKFE( contingencyRslt.chiSquareResult.probability, 0.126, &ok,
                 0.001 );
    TESTCHECKFE( contingencyRslt.chiSquareResult.chiSquare, 2.340, &ok,
                 0.001 );
    TESTCHECK( contingencyRslt.chiSquareResult.degreesOfFreedom, 1, &ok );
    TESTCHECKFE( contingencyRslt.minExpectedCellFreq, 2.142857, &ok,
                 0.000001 );
    TESTCHECK( contingencyRslt.sampleTotal, 14, &ok );

    if ( ok )
        cout << "Statistical Tests PASSED." << endl << endl;
    else
        cout << "Statistical Tests FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  StatisticalTests.cpp
  Copyright (C) 2007 David M. Anderson

  Basic statistical estimates and hypothesis tests.
*/


#include "StatisticalTests.hpp"
#include "Factorial.hpp"
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

double 
MeanTest( int sampleSize, double sampleMean, double sampleVariance,
          double hypothMean, double * pT )
{
    Assert( sampleSize > 1 );
    Assert( sampleVariance > 0. );
    double t = (sampleMean - hypothMean)
            * sqrt( static_cast< double >( sampleSize ) )
            / sqrt( sampleVariance );
    if ( pT )
        *pT = t;
    //Two-tailed test
    double prob = StudentsT_DF( t, sampleSize - 1 );
    if ( t < 0 )
        return 2. * prob;
    else
        return 2. * (1. - prob);
}

//-----------------------------------------------------------------------------

double 
VarianceTest( int sampleSize, double sampleVariance, double hypothVariance,
              double * pChiSquare )
{
    Assert( sampleSize > 1 );
    Assert( sampleVariance > 0. );
    Assert( hypothVariance > 0. );
    double chiSquare = sampleVariance * (sampleSize - 1.) / hypothVariance;
    if ( pChiSquare )
        *pChiSquare = chiSquare;
    //Two-tailed test
    double prob = ChiSquare_DF( chiSquare, sampleSize - 1 );
    if ( sampleVariance < hypothVariance )
        return 2. * prob;
    else
        return 2. * (1. - prob);
}

//=============================================================================

double 
ChiSquareGoodnessOfFitTest( const vector< int > & sampleFreqs,
                            const vector< double > & hypothFreqs,
                            bool probabilities, int constraints,
                            double * pChiSquare )
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
            return 0.;  //hypothesis is clearly false
        if ( (hypFreq < 0.5) && (sampleFreqs[i] == 0) )
        {
            --degreesOfFreedom;
            continue;
        }
        double diff = sampleFreqs[i] - hypFreq;
        chiSquare += diff * diff / hypFreq;
    }
    if ( pChiSquare )
        *pChiSquare = chiSquare;
    return 1. - ChiSquare_DF( chiSquare, degreesOfFreedom );
}

//=============================================================================

double 
MeansTest( int sampleSize1, double sampleMean1, double sampleVariance1,
           int sampleSize2, double sampleMean2, double sampleVariance2,
           bool equalVariances, double * pT )
{
    Assert( sampleSize1 > 1. );
    Assert( sampleSize2 > 1. );
    Assert( sampleVariance1 > 0. );
    Assert( sampleVariance2 > 0. );
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
        degreesOfFreedom = static_cast< int >( dof );
    }
    double t = (sampleMean1 - sampleMean2) / sqrt( pooledVariance );
    if ( pT )
        *pT = t;
    //Two-tailed test
    double prob = StudentsT_DF( t, degreesOfFreedom );
    if ( t < 0 )
        return 2. * prob;
    else
        return 2. * (1. - prob);
}

//-----------------------------------------------------------------------------

double 
VariancesTest( int sampleSize1, double sampleVariance1,
               int sampleSize2, double sampleVariance2,
               double * pF )
{
    Assert( sampleSize1 > 1 );
    Assert( sampleSize2 > 1 );
    Assert( sampleVariance1 > 0. );
    Assert( sampleVariance2 > 0. );
    double f = sampleVariance1 / sampleVariance2;
    if ( pF )
        *pF = f;
    double prob = F_DF( f, (sampleSize1 - 1), (sampleSize2 - 1) );
    //Two-tailed test
    if ( prob <= 0.5 )
        return 2. * prob;
    else
        return 2. * (1. - prob);
}

//=============================================================================

double 
MediansTest( const vector< double > & sample1,
             const vector< double > & sample2 )
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
    int x = min( numBelowMedian, numAboveMedian );
    double prob = Hypergeometric_DF( x, grandSize, grandSize / 2,
                                     sampleSize1 );
    //Two-tailed test
    return 2 * prob;
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

double 
ChiSquareContingencyTableTest( const TwoDArray< int > & table,
                               double * pChiSquare,
                               double * pCramersV, double * pPearsonsC,
                               double * pTschuprovsT, double * pPhi2 )
{
    return ChiSquareContingencyTableTest( table, table.NumRows(),
                                          table.NumColumns(),
                                          pChiSquare,
                                          pCramersV, pPearsonsC,
                                          pTschuprovsT, pPhi2 );
}

//.............................................................................

double 
ChiSquareContingencyTableTest( const vector< vector< int > > & table,
                               double * pChiSquare,
                               double * pCramersV, double * pPearsonsC,
                               double * pTschuprovsT, double * pPhi2 )
{
    return ChiSquareContingencyTableTest( table, table.size(), table[0].size(),
                                          pChiSquare,
                                          pCramersV, pPearsonsC,
                                          pTschuprovsT, pPhi2 );
}

//-----------------------------------------------------------------------------

double 
FishersExactTest( const array< array< int, 2 >, 2 > & table,
                  double * pKendallsQ )
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
    if ( pKendallsQ )
        *pKendallsQ = (a * d  -  b * c) / (a * d  +  b * c);
    double pr = exp( LogFactorial( b + d ) + LogFactorial( c + d )
                    - LogFactorial( d ) - LogFactorial( a + b + c + d ) );
    double prob = pr;
    for ( int i = 0; i < a; ++i )
    {
        pr *= exp( LogFactorial( a + b - i ) + LogFactorial( a + c - i )
                   - LogFactorial( i + 1 ) - LogFactorial( d - a + i - 1 ) );
        prob += pr;
    }
    return prob;
}

//=============================================================================

double 
LinearCorrelationTest( const vector< array< double, 2 > > & samples,
                       double * pPearsonsR, double * pT )
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
    double sum01 = 0.;
    double sum00 = 0.;
    double sum11 = 0.;
    for ( iter p = samples.begin(); p != samples.end(); ++p )
    {
        double diff0 = (*p)[0] - mean0;
        double diff1 = (*p)[1] - mean1;
        sum01 += diff0 * diff1;
        sum00 += diff0 * diff0;
        sum11 += diff1 * diff1;
    }
    double r = sum01 / sqrt( sum00 * sum11 );
    if ( pPearsonsR )
        *pPearsonsR = r;
    if ( fabs( r ) >= 1. )
        return 0.;
    double t = r * sqrt( (sampleSize - 2.) / (1. - r * r) );
    if ( pT )
        *pT = t;
    double prob = StudentsT_DF( t, sampleSize - 2 );
    //Two-sided test
    if ( t < 0 )
        return 2. * prob;
    else
        return 2. * (1. - prob);
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

double 
SpearmansRankCorrelationTest( const vector< array< double, 2 > > & samples,
                              double * pSpearmansR, double * pT )
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
    return LinearCorrelationTest( ranks, pSpearmansR, pT );
}

//-----------------------------------------------------------------------------

double 
KendallsTauTest( const std::vector< array< double, 2 > > & samples,
                 double * pKendallsTau )
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
    if ( pKendallsTau )
        *pKendallsTau = tau;
    double var = (4. * sampleSize  +  10.)
            / (9. * sampleSize * (sampleSize - 1.));
    double prob = Normal_DF( tau, 0., var );
    //Two-tailed test
    if ( tau < 0 )
        return 2. * prob;
    else
        return 2. * (1. - prob);
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
    cout << "MeanTest( 5, 767., 81225., 1000. )" << endl;
    double t;
    double prob = MeanTest( 5, 767., 81225., 1000., &t );
    TESTCHECKFE( prob, 0.142, &ok, 0.001 );
    TESTCHECKFE( t, -1.83, &ok, 0.01 );
    cout << "MeanTest( 15, 442.2, 1936., 420. )" << endl;
    prob = MeanTest( 15, 442.2, 1936., 420., &t );
    TESTCHECKFE( prob, 0.070, &ok, 0.001 );
    TESTCHECKFE( t, 1.95, &ok, 0.01 );
    cout << "VarianceTest( 13, 51.84, 225. )" << endl;
    double chiSquare;
    prob = VarianceTest( 13, 51.84, 225., &chiSquare );
    TESTCHECKFE( prob, 0.0060, &ok, 0.0001 );
    TESTCHECKFE( chiSquare, 2.765, &ok, 0.001 );
    //!!!KolmogorovSmirnovTest
    int digitsSampleArr[] = { 0, 15, 0, 76, 479, 183,8, 23, 0 };
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
    prob = ChiSquareGoodnessOfFitTest( digitsSample, expectedDigits, true,
                                       0, &chiSquare );
    TESTCHECKFE( prob, 0., &ok, 0.0001 );
    TESTCHECKF( chiSquare, 3650.251, &ok );
    cout << "MeansTest( 70, 418.5, 2070.25, 73, 403.7, 936.36 )" << endl;
    prob = MeansTest( 70, 418.5, 2070.25,   //McGwire's 1998 HR distances
                      73, 403.7, 936.36,    //Bonds's 2001 HR distances
                      false, &t );
    TESTCHECKFE( prob, 0.0248, &ok, 0.0001 );
    TESTCHECKFE( t, 2.273, &ok, 0.0001 );
    cout << "VariancesTest( 25, 47.01983, 16, 18.1489 )" << endl;
    double F;
    prob = VariancesTest( 25, 47.01983, 16, 18.1489, &F );
    TESTCHECKFE( prob, 0.059856, &ok, 0.000002 );
    TESTCHECKF( F, 2.590782, &ok );
    //!!!MediansTest
    //!!!KolmogorovSmirnov
    int contingSampleArr[2][2] = { { 560, 308 }, { 240, 92 } };
    TwoDArray< int > conting2DArr( 2, 2 );
    for ( int i = 0; i < 2; ++i )
        for ( int j = 0; j < 2; ++j )
            conting2DArr[i][j] = contingSampleArr[i][j];
    cout << "ChiSquareContingencyTableTest( conting2DArr )" << endl;
    prob = ChiSquareContingencyTableTest( conting2DArr, &chiSquare );
    TESTCHECKFE( prob, 0.011, &ok, 0.001 );
    TESTCHECKFE( chiSquare, 6.529, &ok, 0.0001 );
    vector< vector< int > > contingVec;
    for ( int i = 0; i < 2; ++i )
    {
        vector< int > v;
        for ( int j = 0; j < 2; ++j )
            v.push_back( contingSampleArr[i][j] );
        contingVec.push_back( v );
    }
    cout << "ChiSquareContingencyTableTest( contingVec )" << endl;
    prob = ChiSquareContingencyTableTest( contingVec, &chiSquare );
    TESTCHECKFE( prob, 0.011, &ok, 0.001 );
    TESTCHECKFE( chiSquare, 6.529, &ok, 0.0001 );
    //!!!FishersExactTest
    array< double, 2 > corrSampleArr[]
            = { {{ 68., 53. }}, {{ 68., 38. }}, {{ 67., 35. }},
                {{ 70., 49. }}, {{ 71., 42. }}, {{ 73., 60. }},
                {{ 76., 54. }}, {{ 81., 67. }}, {{ 83., 82. }},
                {{ 84., 78 }} };
    vector< array< double, 2 > > corrVec;
    for ( int i = 0; i < ARRAY_LENGTH( corrSampleArr ); ++i )
        corrVec.push_back( corrSampleArr[i] );
    cout << "LinearCorrelationTest( corrVec )" << endl;
    double r;
    prob = LinearCorrelationTest( corrVec, &r, &t );
    TESTCHECKFE( prob, 0., &ok, 0.001 );
    TESTCHECKFE( r, 0.922, &ok, 0.001 );
    TESTCHECKFE( t, 6.712, &ok, 0.0001 );
    array< double, 2 > rankCorrSampleArr[]
            = { {{ 71., 125. }}, {{ 70.5, 119. }}, {{ 71., 128. }},
                {{ 72., 128. }}, {{ 70., 119. }}, {{ 70., 127. }},
                {{ 66.5, 105. }}, {{ 70., 123. }}, {{ 71., 115. }} };
    vector< array< double, 2 > > rankCorrVec;
    for ( int i = 0; i < ARRAY_LENGTH( rankCorrSampleArr ); ++i )
        rankCorrVec.push_back( rankCorrSampleArr[i] );
    cout << "SpearmansRankCorrelationTest( rankCorrVec )" << endl;
    prob = SpearmansRankCorrelationTest( rankCorrVec, &r, &t );
    //!!!prob
    TESTCHECKFE( r, 0.557, &ok, 0.001 );
    //!!!t
    //!!!KendallsTau

    if ( ok )
        cout << "Statistical Tests PASSED." << endl << endl;
    else
        cout << "Statistical Tests FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

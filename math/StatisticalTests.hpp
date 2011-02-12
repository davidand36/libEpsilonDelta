#ifndef STATISTICALTESTS_HPP
#define STATISTICALTESTS_HPP
/*
  StatisticalTests.hpp
  Copyright (C) 2007 David M. Anderson

  Basic statistical estimates and hypothesis tests.
  NOTES:
  1. Variance() returns the unbiased estimate, dividing by (N-1), rather than
     by N, as is sometimes done.
  2. Median() modifies the sample data only to the extent that it reorders
     the elements.
  3. Hypothesis tests return the probability of the observed result, given
     the null hypothesis. So a smaller value suggests rejection of the null
     hypothesis and acceptance of the alternative.
  4. MeanTest() tests the hypothesis that the population mean is equal to the
     hypothetical mean. It is valid for a normal distribution or a large
     sample size. Likewise for VarianceTest().
  5. KolmogorovSmirnovTest() tests the hypothesis that the population
     distribution function is that given by the hypothDist. It sorts the
     sample data if not already sorted.
  6. ChiSquareGoodnessOfFitTest() tests whether binned or categorized data
     comes from a distribution with the hypothesized frequencies. If, as is
     commonly the case, the hypothesis specifies probabilities, leave
     probabilities = true. If, however, the expected frequencies (counts) are
     directly known, set probabilities=false. If other parameters of the
     expected distribution were determined from the data, constraints should
     be set to the number of such free parameters that have been fitted.
  7. MeansTest() tests the hypothesis that the means of two populations are
     the same. It is valid for normal distributions or large sample sizes.
     Set equalVariances = true if the variances of both distributions are
     believed to be the same; false if they may be significantly different.
     VariancesTest() can be used to decide this.
  8. MediansTest() tests the hypothesis that the medians of two populations
     are equal.
  9. The second form of KolmogorovSmirnovTest() tests the hypothesis that
     two samples are drawn from populations with the same distribution.
  10. LinearCorrelationTest() tests whether two continuous variables are
     linearly dependent. It is valid for jointly normal distributions, or
     reasonable approximations thereof.
  11. SimpleLinearRegression() estimates the coefficients of the equation
     Y = α + ßX + ε (where ε is a residual “error” term with a mean of zero)
     that best fit the data.
  12. SpearmansRankCorrelationTest() and KendallsTauTest() both test whether
     two continuous variables are dependent. Both are based on the order,
     rather than the numeric value, of the data, so they are non-parametric.
  13. ChiSquareContingencyTableTest() tests for independence of two
     categorical variables. Each variable has a small number of possible
     values and the table contains the counts, i.e. the observed frequency of
     each combination of values of the variables.
  14. FishersExactTest() also tests a contingency table for independence, but
     only for 2x2 tables. This test is most appropriate when the margins (row
     and column totals) are considered fixed, but may be too conservative
     otherwise.
*/


#include "ProbabilityDistributions.hpp"
#include "Array.hpp"
#include "Assert.hpp"
#include <vector>
#include <tr1/array>
#include <algorithm>
#include <cmath>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


//Point estimates:
double Mean( const std::vector< double > & sample );
double Variance( const std::vector< double > & sample, double mean );
double Median( std::vector< double > & sample );

//=============================================================================

enum Tail
{
    LOWER_TAIL,     //H₀ is ≥, H₁ is <
    UPPER_TAIL,     //H₀ is ≤, H₁ is >
    BOTH_TAILS      //H₀ is =, H₁ is ≠
};

//-----------------------------------------------------------------------------

//Test return types:
struct TTestResult
{
    double probability;
    double t;
    int degreesOfFreedom;
};

//-----------------------------------------------------------------------------

struct FTestResult
{
    double probability;
    double F;
    int degreesOfFreedom1;
    int degreesOfFreedom2;
};

//-----------------------------------------------------------------------------

struct ChiSquareTestResult
{
    double probability;
    double chiSquare;
    int degreesOfFreedom;
};

//-----------------------------------------------------------------------------

struct ContingencyTableResult
{
    ChiSquareTestResult chiSquareResult;
    double minExpectedCellFreq;
    int sampleTotal;
};

//-----------------------------------------------------------------------------

struct CorrelationTestResult
{
    TTestResult tResult;
    double r;
    double mean0;
    double mean1;
    double variance0;
    double variance1;
};

//-----------------------------------------------------------------------------

struct KendallsTauTestResult
{
    double probability;
    double kendallsTau;
};

//-----------------------------------------------------------------------------

struct SimpleRegressionResult
{
    double alpha;
    double beta;
    double varianceAlpha;
    double varianceBeta;
    double residualVariance;
};

//=============================================================================

//One-sample tests:
TTestResult MeanTest( int sampleSize, double sampleMean, double sampleVariance,
                      double hypothMean, Tail tail );
ChiSquareTestResult VarianceTest( int sampleSize, double sampleVariance,
                                  double hypothVariance, Tail tail );
template < typename DistribFunc >
double KolmogorovSmirnovTest( std::vector< double > & sample,
                              DistribFunc hypothFunc, bool sorted = false );
ChiSquareTestResult ChiSquareGoodnessOfFitTest(
    const std::vector< int > & sampleFreqs,
    const std::vector< double > & hypothFreqs,
    bool probabilities = true,
    int constraints = 0 );

//Two-sample comparison tests:
TTestResult MeansTest( int sampleSize1, double sampleMean1,
                       double sampleVariance1,
                       int sampleSize2, double sampleMean2,
                       double sampleVariance2,
                       bool equalVariances, Tail tail );
FTestResult VariancesTest( int sampleSize1, double sampleVariance1,
                           int sampleSize2, double sampleVariance2,
                           Tail tail );
double MediansTest( const std::vector< double > & sample1,
                    const std::vector< double > & sample2, Tail tail );
double KolmogorovSmirnovTest( std::vector< double > & sample1,
                              std::vector< double > & sample2,
                              bool sorted = false );

//Correlation tests:
CorrelationTestResult LinearCorrelationTest(
    const std::vector< std::tr1::array< double, 2 > > & samples, Tail tail );
SimpleRegressionResult SimpleLinearRegression( int sampleSize,
                                               double meanX, double meanY,
                                               double varianceX,
                                               double varianceY,
                                               double pearsonsR );
CorrelationTestResult SpearmansRankCorrelationTest(
    const std::vector< std::tr1::array< double, 2 > > & samples, Tail tail );
KendallsTauTestResult KendallsTauTest(
    const std::vector< std::tr1::array< double, 2 > > & samples, Tail tail );

//Contingency tables:
ContingencyTableResult ChiSquareContingencyTableTest(
    const TwoDArray< int > & table, bool yatesCorrection = false );
ContingencyTableResult ChiSquareContingencyTableTest(
    const std::vector< std::vector< int > > & table,
    bool yatesCorrection = false );
template < typename C >
ContingencyTableResult ChiSquareContingencyTableTest(
    const C & table, int numRows, int numColumns, bool yatesCorrection );
template < unsigned long M, unsigned long N >
ContingencyTableResult ChiSquareContingencyTableTest(
    const std::tr1::array< std::tr1::array< int, M >, N > & table,
    bool yatesCorrection = false );
double FishersExactTest(
    const std::tr1::array< std::tr1::array< int, 2 >, 2 > & table, int tails );

#ifdef DEBUG
bool TestStatisticalTests( );
#endif


//#############################################################################


template < typename DistribFunc >
double 
KolmogorovSmirnovTest( std::vector< double > & sample, DistribFunc hypothDist,
                       bool sorted )
{
    if ( ! sorted )
        std::sort( sample.begin(), sample.end() );
    double d = 0.;
    double recipSize = 1. / sample.size();
    double sampleDF0 = 0.;
    double sampleDF1 = 0.;
    typedef std::vector< double >::const_iterator iter;
    for ( iter p = sample.begin(); p != sample.end(); ++p )
    {
        sampleDF1 += recipSize;
        double hypothDF = hypothDist( *p );
        double diff = std::max( sampleDF1 - hypothDF, hypothDF - sampleDF0 );
        if ( diff > d )
            d = diff;
        sampleDF0 = sampleDF1;
    }
    double sqrtSize = std::sqrt( static_cast< double >( sample.size() ) );
    double x = d * (sqrtSize  +  0.12  +  0.11 / sqrtSize);
    return 1. - KolmogorovSmirnov_DF( x );
}

//=============================================================================

template < typename C >
ContingencyTableResult
ChiSquareContingencyTableTest( const C & table, int numRows, int numColumns,
                               bool yatesCorrection )
{
    Assert( numRows > 1 );
    Assert( numColumns > 1 );
    int sampleTotal = 0;
    std::vector< int > rowTotals( numRows );
    std::vector< int > columnTotals( numColumns );
    for ( int i = 0; i < numRows; ++i )
    {
        for ( int j = 0; j < numColumns; ++j )
        {
            int n = table[i][j];
            sampleTotal += n;
            rowTotals[i] += n;
            columnTotals[j] += n;
        }
    }
    Assert( sampleTotal > 0 );
    int nRows = numRows;
    int nCols = numColumns;
    double chiSquare = 0.;
    double minExpectedCellFreq = sampleTotal;
    for ( int i = 0; i < numRows; ++i )
    {
        if ( rowTotals[i] == 0 )
        {
            --nRows;
            continue;
        }
        for ( int j = 0; j < numColumns; ++j )
        {
            if ( columnTotals[j] == 0 )
                continue;
            double expected = rowTotals[i] * columnTotals[j]
                    / static_cast< double >( sampleTotal );
            if ( expected < minExpectedCellFreq )
                minExpectedCellFreq = expected;
            double diff = table[i][j] - expected;
            if ( yatesCorrection )
                diff = std::fabs( diff ) - 0.5;
            if ( expected > 0. )
                chiSquare += diff * diff / expected;
        }
    }
    for ( int j = 0; j < numColumns; ++j )
        if ( columnTotals[j] == 0 )
            --nCols;
    Assert( nRows > 1 );
    Assert( nCols > 1 );
    int degreesOfFreedom = (nRows - 1) * (nCols - 1);
    double prob = 1. - ChiSquare_DF( chiSquare, degreesOfFreedom );
    ContingencyTableResult result = { { prob, chiSquare, degreesOfFreedom },
                                      minExpectedCellFreq, sampleTotal };

    return result;
}

//.............................................................................

template < unsigned long M, unsigned long N >
ContingencyTableResult
ChiSquareContingencyTableTest(
    const std::tr1::array< std::tr1::array< int, M >, N > & table,
    bool yatesCorrection )
{
    return ChiSquareContingencyTableTest( table, M, N, yatesCorrection );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //STATISTICALTESTS_HPP

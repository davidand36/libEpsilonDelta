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
     the (null) hypothesis. So a smaller value suggests rejection.
  4. MeanTest() tests the hypothesis that the population mean is equal to the
     hypothetical mean. It is valid for a normal distribution or a large
     sample size. Likewise for VarianceTest().
  5. KolmogorovSmirnovTest() tests the hypothesis that the population
     distribution function is that given by the hypothDist. It sorts the
     sample data if not already sorted.
  6. ChiSquareGoodnessOfFitTest() tests whether binned or categorized data
     comes from a distribution with the hypothesized frequencies. If, as is
     commonly the case, the hypothesis specifies probabilities, leave
     probabilities = true. If, however, the expected frequencies are directly
     known, set probabilities=false. If other parameters of the expected
     distribution were determined from the data, constraints should be set
     to the number of such free parameters have been fitted.
  7. MeansTest() tests the hypothesis that the means of two populations are
     the same. It is valid for normal distributions or large sample sizes.
     Set equalVariances = true if the variances of both distributions are
     believed to be the same; false if they are significantly different.
     VariancesTest() can be used to decide this.
  8. MediansTest() tests the hypothesis that the medians of two populations
     are equal.
  9. The second form of KolmogorovSmirnovTest() tests the hypothesis that
     two samples are drawn from populations with the same distribution.
  10. ContingencyTableTest() 
*/


#include "Array.hpp"
#include "ProbabilityDistributions.hpp"
#include "Assert.hpp"
#include <vector>
#include <algorithm>
#include <cmath>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


//Point estimates:
double Mean( const std::vector< double > & sample );
double Variance( const std::vector< double > & sample, double mean );
double Median( std::vector< double > & sample );

//One-sample tests:
double MeanTest( int sampleSize, double sampleMean, double sampleVariance,
                 double hypothMean = 0., double * pT = 0 );
double VarianceTest( int sampleSize, double sampleVariance,
                     double hypothVariance, double * pChiSquare = 0 );
template < typename DistribFunc >
double KolmogorovSmirnovTest( std::vector< double > & sample,
                              DistribFunc hypothFunc, bool sorted = false );
double ChiSquareGoodnessOfFitTest( const std::vector< int > & sampleFreqs,
                                   const std::vector< double > & hypothFreqs,
                                   bool probabilities = true,
                                   int constraints = 0,
                                   double * pChiSquare = 0 );

//Two-sample comparison tests:
double MeansTest( int sampleSize1, double sampleMean1, double sampleVariance1,
                  int sampleSize2, double sampleMean2, double sampleVariance2,
                  bool equalVariances = false, double * pT = 0 );
double VariancesTest( int sampleSize1, double sampleVariance1,
                      int sampleSize2, double sampleVariance2,
                      double * pF = 0 );
double MediansTest( const std::vector< double > & sample1,
                    const std::vector< double > & sample2 );
double KolmogorovSmirnovTest( std::vector< double > & sample1,
                              std::vector< double > & sample2,
                              bool sorted = false );

//Contingency tables:
double ChiSquareContingencyTableTest( const TwoDArray< int > & table,
                                      double * pChiSquare = 0,
                                      double * pCramersV = 0,
                                      double * pPearsonsC = 0,
                                      double * pTschuprovsT = 0,
                                      double * pPhi2 = 0 );
double ChiSquareContingencyTableTest(
                               const std::vector< std::vector< int > > & table,
                               double * pChiSquare = 0,
                               double * pCramersV = 0, double * pPearsonsC = 0,
                               double * pTschuprovsT = 0, double * pPhi2 = 0 );
template < typename C >
double ChiSquareContingencyTableTest( const C & table,
                                      int numRows, int numColumns,
                                      double * pChiSquare,
                                      double * pCramersV, double * pPearsonsC,
                                      double * pTschuprovsT, double * pPhi2 );
double FishersExactTest( const array< array< int, 2 >, 2 > & table,
                         double * pKendallsQ = 0 );

//Correlation tests:
double LinearCorrelationTest(
    const std::vector< array< double, 2 > > & samples,
    double * pPearsonsR = 0, double * pT = 0 );
double SpearmansRankCorrelationTest(
    const std::vector< array< double, 2 > > & samples,
    double * pSpearmansR = 0, double * pT = 0 );
double KendallsTauTest( const std::vector< array< double, 2 > > & samples,
                        double * pKendallsTau = 0 );

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
double 
ChiSquareContingencyTableTest( const C & table, int numRows, int numColumns,
                               double * pChiSquare,
                               double * pCramersV, double * pPearsonsC,
                               double * pTschuprovsT, double * pPhi2 )
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
            double diff = table[i][j] - expected;
            if ( expected > 0. )
                chiSquare += diff * diff / expected;
        }
    }
    for ( int j = 0; j < numColumns; ++j )
        if ( columnTotals[j] == 0 )
            --nCols;
    if ( pChiSquare )
        *pChiSquare = chiSquare;
    double phi2 = chiSquare / sampleTotal;
    if ( pCramersV )
        *pCramersV = std::sqrt( phi2
                                / std::min( numRows - 1, numColumns - 1 ) );
    if ( pPearsonsC )
        *pPearsonsC = std::sqrt( chiSquare / (chiSquare + sampleTotal) );
    if ( pTschuprovsT )
        *pTschuprovsT = std::sqrt( phi2
                              / std::sqrt( static_cast< double >( (numRows - 1)
                                                      * (numColumns - 1) ) ) );
    if ( pPhi2 )
        *pPhi2 = phi2;
    Assert( nRows > 1 );
    Assert( nCols > 1 );
    int degreesOfFreedom = (nRows - 1) * (nCols - 1);
    return 1. - ChiSquare_DF( chiSquare, degreesOfFreedom );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //STATISTICALTESTS_HPP

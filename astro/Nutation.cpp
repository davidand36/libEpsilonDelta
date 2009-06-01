/*
  Nutation.cpp
  Copyright (C) 2007 David M. Anderson

  Nutation class: represents coordinate correction due the short-period
  (principally 18.6-year) motion of the Earth's rotation axis about the
  mean axis.
*/


#include "Nutation.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "JPLEphemeris.hpp"
#include "Obliquity.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Matrix3D 
Nutation::Matrix( Angle meanObliquity, bool inverse ) const
{
    //Explanatory Supplement (3222-2)
    Angle trueObliquity = meanObliquity + m_nutObliquity;
    //Explanatory Supplement (3222-4)
    double cosDPsi = m_nutLongitude.Cos( );
    double sinDPsi = m_nutLongitude.Sin( );
    double cosMEps = meanObliquity.Cos( );
    double sinMEps = meanObliquity.Sin( );
    double cosEps = trueObliquity.Cos( );
    double sinEps = trueObliquity.Sin( );
    double n00 = cosDPsi;
    double n01 = - sinDPsi * cosMEps;
    double n02 = - sinDPsi * sinMEps;
    double n10 = sinDPsi * cosEps;
    double n11 = cosDPsi * cosEps * cosMEps  +  sinEps * sinMEps;
    double n12 = cosDPsi * cosEps * sinMEps  -  sinEps * cosMEps;
    double n20 = sinDPsi * sinEps;
    double n21 = cosDPsi * sinEps * cosMEps  -  cosEps * sinMEps;
    double n22 = cosDPsi * sinEps * sinMEps  +  cosEps * cosMEps;
    if ( inverse )
        //The matrix is orthogonal, so the inverse is the transpose.
        return Matrix3D( n00, n10, n20,  n01, n11, n21,  n02, n12, n22 );
    else
        return Matrix3D( n00, n01, n02,  n10, n11, n12,  n20, n21, n22 );
}

//=============================================================================

#ifdef DEBUG

bool 
Nutation::Test( JPLEphemeris & ephem )
{
    bool ok = true;
    cout << "Testing Nutation" << endl;

    //Paul J. Heafner, "Fundamental Ephemeris Computations", p. 44
    double jd = 2447160.5;
    cout << "JD: " << jd << endl;
    Nutation nutation;
    bool ephRslt = ephem.GetNutation( jd, &nutation );
    Assert( ephRslt );
    Angle meanObliquity = MeanObliquity( jd );
    Matrix3D nutMat = nutation.Matrix( meanObliquity );
    TESTCHECKF( nutMat(0,0), 1.0, &ok );
    TESTCHECKFE( nutMat(0,1), -0.00000458, &ok, 3.e-3 );
    TESTCHECKFE( nutMat(0,2), -0.00000199, &ok, 5.e-3 );
    TESTCHECKFE( nutMat(1,0), 0.00000458, &ok, 3.e-3 );
    TESTCHECKF( nutMat(1,1), 1.0, &ok );
    TESTCHECKFE( nutMat(1,2), -0.00004134, &ok, 3.e-4 );
    TESTCHECKFE( nutMat(2,0), 0.00000199, &ok, 5.e-3 );
    TESTCHECKFE( nutMat(2,1), 0.00004134, &ok, 3.e-4 );
    TESTCHECKF( nutMat(2,2), 1.0, &ok );

    if ( ok )
        cout << "Nutation PASSED." << endl << endl;
    else
        cout << "Nutation FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

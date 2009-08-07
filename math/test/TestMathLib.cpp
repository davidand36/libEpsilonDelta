/*
  TestMathLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta math library.
*/


#include "Assert.hpp"
#include "TestCheck.hpp"
#include "DivMod.hpp"
#include "Polynomial.hpp"
#include "RootFinder.hpp"
#include "PrecisionTests.hpp"
#include "Angle.hpp"
#include "AngleDMS.hpp"
#include "AngleHMS.hpp"
#include "Point2.hpp"
#include "Point3.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix2.hpp"
#include "Matrix3.hpp"
#include "Matrix4.hpp"
#include "RotationMatrix2.hpp"
#include "RotationMatrix3.hpp"
#include "Quaternion.hpp"
#include "EulerAngles.hpp"
#include "Polar.hpp"
#include "Spherical.hpp"
#include "Factorial.hpp"
#include "Gamma.hpp"
#include "ProbabilityDistributions.hpp"
#include "StatisticalTests.hpp"
#include "Permutation.hpp"
#include "ODE.hpp"
#include <cstdio>
#include <iostream>
using namespace std;
using namespace EpsilonDelta;


int Main( int /*argc*/, char ** /*argv*/ );


//*****************************************************************************


int main( int argc, char ** argv )
{
    try
    {
        int res = Main( argc, argv );
        return res;
    }
    catch ( Exception & except )
    {
        cout << except.Description( ) << endl;
    }
    catch ( exception & except )
    {
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    catch ( ... )
    {
        Exception exc( "Unknown exception!" ); //!!!
        cout << exc.Description( ) << endl;
    }
    return  2;
}

//-----------------------------------------------------------------------------


int Main( int /*argc*/, char ** /*argv*/ )
{
    bool ok = true;

#ifdef DEBUG
    if ( ! TestDivMod( ) )
        ok = false;
    if ( ! TestPolynomial( ) )
        ok = false;
    if ( ! RootFinder::Test( ) )
        ok = false;
    if ( ! Angle::Test( ) )
        ok = false;
    if ( ! AngleDMS::Test( ) )
        ok = false;
    if ( ! AngleHMS::Test( ) )
        ok = false;
    if ( ! TestPoint2( ) )
        ok = false;
    if ( ! TestPoint3( ) )
        ok = false;
    if ( ! TestVector2( ) )
        ok = false;
    if ( ! TestVector3( ) )
        ok = false;
    if ( ! TestVector4( ) )
        ok = false;
    if ( ! TestMatrix2( ) )
        ok = false;
    if ( ! TestMatrix3( ) )
        ok = false;
    if ( ! TestMatrix4( ) )
        ok = false;
    if ( ! TestRotationMatrix2( ) )
        ok = false;
    if ( ! TestRotationMatrix3( ) )
        ok = false;
    if ( ! TestQuaternion( ) )
        ok = false;
    if ( ! EulerAngles::Test( ) )
        ok = false;
    if ( ! Polar::Test( ) )
        ok = false;
    if ( ! Spherical::Test( ) )
        ok = false;
    if ( ! TestFactorial( ) )
        ok = false;
    if ( ! TestGamma( ) )
        ok = false;
    if ( ! TestProbabilityDistributions( ) )
        ok = false;
    if ( ! TestStatisticalTests( ) )
        ok = false;
    if ( ! Permutation::Test( ) )
        ok = false;
    if ( ! ODE::Test( ) )
        ok = false;
    DoPrecisionTests( );
#endif //DEBUG

    if ( ok )
    {
        cout << endl << "All tests PASSED!" << endl;
        return  0;
    }
    else
    {
        cout << endl << "Some test(s) FAILED!" << endl;
        return  1;
    }
}


//*****************************************************************************

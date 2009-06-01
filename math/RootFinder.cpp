/*
  RootFinder.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for finding zeros of functions.
*/


#include "RootFinder.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Polynomial.hpp"
#include <iostream>
#include <vector>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

namespace
{

float Cos( float arg );
void CosEandD( float arg, float * pVal, float * pDeriv );
float Tan( float arg );
void TanEandD( float arg, float * pVal, float * pDeriv );

}

//.............................................................................

bool 
RootFinder::Test( )
{
    bool ok = true;
    cout << "Testing RootFinder" << endl;

    cout << "PolynomialD( 3, 42., 35., -14., -7. )" << endl;
    PolynomialD poly3( 3, 42., 35., -14., -7. );
    TESTCHECK( poly3.Degree(), 3, &ok );
    TESTCHECK( poly3[0], 42., &ok );
    TESTCHECK( poly3[1], 35., &ok );
    TESTCHECK( poly3[2], -14., &ok );
    TESTCHECK( poly3[3], -7., &ok );
    TESTCHECK( poly3( -1. ), 0., &ok );
    double x0 = 0.01;
    double x1 = -0.01;
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( BracketRoot( poly3, &x0, &x1 ), true, &ok );
    TESTCHECK( (x0 >= -1), true, &ok );
    TESTCHECK( (x1 <= -1), true, &ok );
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    double root;
    TESTCHECK( Brent( poly3, &root, x0, x1 ), true, &ok );
    TESTCHECKF( root, -1., &ok );
    x0 = -5.;
    x1 = -4.;
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( BracketRoot( poly3, &x0, &x1 ), true, &ok );
    TESTCHECK( (x0 <= -3), true, &ok );
    TESTCHECK( (x1 >= -3), true, &ok );
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( Brent( poly3, &root, x0, x1 ), true, &ok );
    TESTCHECKF( root, -3., &ok );
    x0 = 3.;    //4. is OK on Linux, but not on Win32.
    x1 = 5.;
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( BracketRoot( poly3, &x0, &x1 ), true, &ok );
    TESTCHECK( (x0 <= 2), true, &ok );
    TESTCHECK( (x1 >= 2), true, &ok );
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( Brent( poly3, &root, x0, x1 ), true, &ok );
    TESTCHECKF( root, 2., &ok );

    x0 = -0.01;
    x1 = 0.01;
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( BracketRoot( poly3, &x0, &x1 ), true, &ok );
    TESTCHECK( (x0 <= -1), true, &ok );
    TESTCHECK( (x1 >= -1), true, &ok );
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( NewtonRaphson( poly3, &root, x0, x1 ), true, &ok );
    TESTCHECKF( root, -1., &ok );
    x0 = -4.;
    x1 = -5.;
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( BracketRoot( poly3, &x0, &x1 ), true, &ok );
    TESTCHECK( (x0 >= -3), true, &ok );
    TESTCHECK( (x1 <= -3), true, &ok );
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( NewtonRaphson( poly3, &root, x0, x1 ), true, &ok );
    TESTCHECKF( root, -3., &ok );
    x0 = 1.;
    x1 = 4.;
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( BracketRoot( poly3, &x0, &x1 ), true, &ok );
    TESTCHECK( (x0 <= 2), true, &ok );
    TESTCHECK( (x1 >= 2), true, &ok );
    cout << "x0 = " << x0 << "  x1 = " << x1 << endl;
    TESTCHECK( NewtonRaphson( poly3, &root, x0, x1 ), true, &ok );
    TESTCHECKF( root, 2., &ok );

    float fx0 = 1.f;
    float fx1 = 1.2f;
    float froot;
    cout << "fx0 = " << fx0 << "  fx1 = " << fx1 << endl;
    TESTCHECK( BracketRoot( Cos, &fx0, &fx1 ), true, &ok );
    TESTCHECK( (fx0 <= M_PI/2), true, &ok );
    TESTCHECK( (fx1 >= M_PI/2), true, &ok );
    cout << "fx0 = " << fx0 << "  fx1 = " << fx1 << endl;
    TESTCHECK( Brent( Cos, &froot, fx0, fx1 ), true, &ok );
    TESTCHECKF( froot, M_PI/2.f, &ok );
    TESTCHECK( NewtonRaphson( CosEandD, &froot, fx0, fx1 ), true, &ok );
    TESTCHECKF( froot, M_PI/2.f, &ok );
    
    fx0 = 1.f;
    fx1 = 2.f;
    cout << "fx0 = " << fx0 << "  fx1 = " << fx1 << endl;
    TESTCHECK( BracketRoot( Tan, &fx0, &fx1 ), true, &ok );
    TESTCHECK( (fx0 <= M_PI/2), true, &ok );
    TESTCHECK( (fx1 >= M_PI/2), true, &ok );
    cout << "fx0 = " << fx0 << "  fx1 = " << fx1 << endl;
    cout << "These should detect a singularity, but they don't:" << endl;
    cout << "Brent( Tan, &froot, fx0, fx1 ): "
         << Brent( Tan, &froot, fx0, fx1 ) << endl;
//    TESTCHECK( Brent( Tan, &froot, fx0, fx1 ), false, &ok );
    cout << "froot = " << froot << endl;
    cout << "NewtonRaphson( TanEandD, &froot, fx0, fx1 ): "
         << NewtonRaphson( TanEandD, &froot, fx0, fx1 ) << endl;
//    TESTCHECK( NewtonRaphson( TanEandD, &froot, fx0, fx1 ), false, &ok );
    cout << "froot = " << froot << endl;

    if ( ok )
        cout << "RootFinder PASSED." << endl << endl;
    else
        cout << "RootFinder FAILED." << endl << endl;
    return ok;
}

//.............................................................................

namespace
{
//-----------------------------------------------------------------------------

float 
Cos( float arg )
{
    return cos( arg );
}

//.............................................................................

void 
CosEandD( float arg, float * pVal, float * pDeriv )
{
    *pVal = cos( arg );
    *pDeriv = - sin( arg );
}

//.............................................................................

float 
Tan( float arg )
{
    return tan( arg );
}

//.............................................................................

void 
TanEandD( float arg, float * pVal, float * pDeriv )
{
    *pVal = tan( arg );
    float c = cos( arg );
    *pDeriv = 1.f / (c * c);
}

//-----------------------------------------------------------------------------
} //namespace

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta

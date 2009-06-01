/*
  ODE.cpp
  Copyright (C) 2009 David M. Anderson

  Routines for solving ordinary differential equations.
*/


#include "ODE.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

//-----------------------------------------------------------------------------

namespace
{                                                                   //namespace

void exponentialDerivFunc( double x, const Vector< double, 1 > & y,
                           Vector< double, 1 > * pDeriv );
void trigDerivFunc( double x, const Vector< double, 2 > & y,
                    Vector< double, 2 > * pDeriv );

}                                                                   //namespace

//-----------------------------------------------------------------------------

bool 
ODE::Test( )
{
    bool ok = true;
    cout << "Testing ODE" << endl;

    //This is the differential equation for y(x) = exp(x).
    double x = 0.;
    Vector< double, 1 > y1;
    y1[0] = 1.;
    double deltaX = 0.01;
    while ( x < 1. )
    {
        RungeKutta4( &x, &y1, exponentialDerivFunc, deltaX );
    }
    TESTCHECKF( x, 1., &ok );
    TESTCHECKF( y1[0], 2.718281828459, &ok );

    //This is the differential equation for y(x) = sin(x).
    x = 0.;
    Vector< double, 2 > y2;
    y2[0] = 0.;
    y2[1] = 1.;
    while ( x < 1. )
    {
        RungeKutta4( &x, &y2, trigDerivFunc, deltaX );
    }
    TESTCHECKF( x, 1., &ok );
    TESTCHECKF( y2[0], std::sin( 1. ), &ok );
    TESTCHECKF( y2[1], std::cos( 1. ), &ok );

    //This is the differential equation for y(x) = cos(x).
    x = 0.;
    y2[0] = 1.;
    y2[1] = 0.;
    while ( x < 2. )
    {
        RungeKutta4( &x, &y2, trigDerivFunc, deltaX );
    }
    TESTCHECKF( x, 2., &ok );
    TESTCHECKF( y2[0], std::cos( 2. ), &ok );
    TESTCHECKF( y2[1], - std::sin( 2. ), &ok );

    if ( ok )
        cout << "ODE PASSED." << endl << endl;
    else
        cout << "ODE FAILED." << endl << endl;
    return ok;
    
    return true;
}

//-----------------------------------------------------------------------------

namespace
{                                                                   //namespace

void 
exponentialDerivFunc( double /*x*/, const Vector< double, 1 > & y,
                      Vector< double, 1 > * pDeriv )
{
    (*pDeriv)[0] = y[0];    //y'(x) = y(x)
}

//-----------------------------------------------------------------------------

void 
trigDerivFunc( double /*x*/, const Vector< double, 2 > & y,
               Vector< double, 2 > * pDeriv )
{
    (*pDeriv)[0] = y[1];
    (*pDeriv)[1] = -y[0];    //y"(x) = -y(x)
}

}                                                                   //namespace

//-----------------------------------------------------------------------------

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

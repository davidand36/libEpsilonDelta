/*
  Factorial.cpp
  Copyright (C) 2007 David M. Anderson

  Factorial (n!) and related functions.
*/


#include "Factorial.hpp"
#include "Gamma.hpp"
#include "Assert.hpp"
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


double 
LogFactorial( int n )
{
    static double smallLF[ 100 ];
    if ( n <= 1 )
        return 0.;
    if ( n < 100 )
    {
        if ( smallLF[n] != 0. )
            return smallLF[n];
        else
            return (smallLF[n] = LogGamma( n + 1. ));
    }
    return LogGamma( n + 1.);
}

//=============================================================================

double 
BinomialCoefficient( int n, int k )
{
    return floor( exp( LogFactorial( n )
                       - LogFactorial( k ) - LogFactorial( n - k ) ) + 0.5 );
}

//=============================================================================

#ifdef DEBUG

bool 
TestFactorial( )
{
    bool ok = true;
    cout << "Testing Factorial" << endl;

    TESTCHECK( Factorial( 12 ), 479001600, &ok );
    TESTCHECK( Factorial( 20LL ), 2432902008176640000LL, &ok );
    TESTCHECK( Factorial( 20. ), 2432902008176640000., &ok );
    TESTCHECKF( Factorial( 50. ), 3.0414093203509456e64, &ok );
    TESTCHECKFE( Factorial( 100. ), 9.3326e157, &ok, 1.e-5 );
    TESTCHECK( BinomialCoefficient( 19, 7 ), 50388., &ok );
    TESTCHECK( BinomialCoefficient( 20, 10 ), 184756., &ok );

    if ( ok )
        cout << "Factorial PASSED." << endl << endl;
    else
        cout << "Factorial FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

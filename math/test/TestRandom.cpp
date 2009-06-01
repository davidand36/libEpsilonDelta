/*
  TestMathLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta math library Random functions.
*/


#include "Assert.hpp"
#include "TestCheck.hpp"
#include "Random.hpp"
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
    if ( ! RandomNumberGenerator::Test( ) )
        ok = false;
    if ( ! QuickRandomNumberGenerator::Test( ) )
        ok = false;
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

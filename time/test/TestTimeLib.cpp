/*
  TestTimeLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta time library.
*/


#include <cstdio>
#include <iostream>
#include "Exception.hpp"
#include "Assert.hpp"
#include "TestCheck.hpp"
#include "TimeIncrement.hpp"
#include "Time.hpp"
#include "ModifiedJulianDay.hpp"
#include "Epoch.hpp"
#include "DateJD.hpp"
#include "GregorianDate.hpp"
#include "DateTime.hpp"
#include "StdTime.hpp"
#include "StdClock.hpp"
#include "Timer.hpp"
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
    if ( ! TimeIncrement::Test( ) )
        ok = false;
    if ( ! Time::Test( ) )
        ok = false;
    if ( ! TestMJD( ) )
        ok = false;
    if ( ! Epoch::Test( ) )
        ok = false;
    if ( ! DateJD::Test( ) )
        ok = false;
    if ( ! TestGregorianDate( ) )
        ok = false;
    if ( ! DateTime::Test( ) )
        ok = false;
    if ( ! StdTime::Test( ) )
        ok = false;
    if ( ! StdClock::Test( ) )
        ok = false;
    if ( ! Timer::Test( ) )
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

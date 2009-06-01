/*
  TestAstroLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta astro library.
*/


#include <cstdio>
#include <iostream>
#include "Assert.hpp"
#include "TestCheck.hpp"
#include "AstroCoordTransformations.hpp"
#include "JPLEphemeris.hpp"
#include "SiderealTime.hpp"
#include "Precession.hpp"
#include "Obliquity.hpp"
#include "Nutation.hpp"
#include "CoordinateReduction.hpp"
#include "ApparentEphemeris.hpp"
#include "AstroPhenomena.hpp"
#include "Seasons.hpp"
#include "MoonPhases.hpp"
#include "RiseSet.hpp"
#include "LunarVisibility.hpp"
#include "EquationOfTime.hpp"
#include "GeodeticLocation.hpp"
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


int Main( int /*argc*/, char ** argv )
{
    bool ok = true;

#ifdef DEBUG
    if ( ! TestAstroCoordTransformations( ) )
        ok = false;

    string libBasePath = argv[0];
    int slashPos = libBasePath.rfind( '/' );
    if ( slashPos == static_cast<int>( string::npos ) )
        libBasePath = "";
    else
        libBasePath.erase( slashPos + 1 );
    libBasePath += "../";

    JPLEphemeris de200( libBasePath + "astrodata/JPL_DE200.be", true );
    if ( ! de200.Test( libBasePath + "astro/test/testpo.200" ) )
        ok = false;
    JPLEphemeris de405( libBasePath + "astrodata/JPL_DE405.be", true );
    if ( ! de405.Test( libBasePath + "astro/test/testpo.405" ) )
        ok = false;
    JPLEphemeris de406( libBasePath + "astrodata/JPL_DE406.be", true );
    if ( ! de406.Test( libBasePath + "astro/test/testpo.406" ) )
        ok = false;

    JPLEphemeris::RegisterEphemeris( de405 );
    JPLEphemeris::RegisterEphemeris( de406 );

    if ( ! TestSiderealTime( de405 ) )
        ok = false;
    if ( ! Precession::Test( ) )
        ok = false;
    if ( ! TestObliquity( de405 ) )
        ok = false;
    if ( ! Nutation::Test( de405 ) )
        ok = false;
    if ( ! TestCoordinateReduction( de200, de405 ) )
        ok = false;
    if ( ! TestApparentEphemeris( de200, de405 ) )
        ok = false;
    if ( ! TestAstroPhenomena( ) )
        ok = false;
    if ( ! Seasons::Test( ) )
        ok = false;
    if ( ! MoonPhases::Test( ) )
        ok = false;
    if ( ! RiseSet::Test( ) )
        ok = false;
    if ( ! LunarVisibility::Test( ) )
        ok = false;
    if ( ! TestEquationOfTime( ) )
        ok = false;
    if ( ! TestGeodeticLocation( ) )
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


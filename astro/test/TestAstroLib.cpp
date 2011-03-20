/*
  TestAstroLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta astro library.
*/


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
#include "Constellations.hpp"
#include "Platform.hpp"
#include "FileReader.hpp"
#include <cstdio>
#include <iostream>
#include <tr1/memory>
using namespace std;
using namespace std::tr1;
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
    JPLEphemeris::Log().SetVerbosity( Logger::Info );
    RiseSet::Log().SetVerbosity( Logger::Info );
    AstroPhenomenaLog().SetVerbosity( Logger::Info );

    if ( ! TestAstroCoordTransformations( ) )
        ok = false;

    string libBasePath = argv[0];
    int slashPos = libBasePath.rfind( '/' );
    if ( slashPos == static_cast<int>( string::npos ) )
        libBasePath = "";
    else
        libBasePath.erase( slashPos + 1 );
    libBasePath += "../";
#ifdef COMPILER_MSC
    libBasePath += "../";
#endif

    if ( ! JPLEphemeris::Test( ) )
        ok = false;

    shared_ptr< Reader > spReader(
        new FileReader( libBasePath + "astrodata/JPL_DE200.be" ) );
    shared_ptr< JPLEphemeris > de200be( new JPLEphemeris( spReader, true ) );
    if ( ! de200be->Test( libBasePath + "astro/test/testpo.200" ) )
        ok = false;
    spReader.reset( new FileReader( libBasePath + "astrodata/JPL_DE405.be" ) );
    shared_ptr< JPLEphemeris > de405be( new JPLEphemeris( spReader, true ) );
    if ( ! de405be->Test( libBasePath + "astro/test/testpo.405" ) )
        ok = false;
    spReader.reset( new FileReader( libBasePath + "astrodata/JPL_DE406.be" ) );
    shared_ptr< JPLEphemeris > de406be( new JPLEphemeris( spReader, true ) );
    if ( ! de406be->Test( libBasePath + "astro/test/testpo.406" ) )
        ok = false;
    spReader.reset( new FileReader( libBasePath + "astrodata/JPL_DE405.le" ) );
    shared_ptr< JPLEphemeris > de405le( new JPLEphemeris( spReader, true ) );
    if ( ! de405le->Test( libBasePath + "astro/test/testpo.405" ) )
        ok = false;
    spReader.reset( new FileReader( libBasePath + "astrodata/JPL_DE406.le" ) );
    shared_ptr< JPLEphemeris > de406le( new JPLEphemeris( spReader, true ) );
    if ( ! de406le->Test( libBasePath + "astro/test/testpo.406" ) )
        ok = false;
    spReader.reset( new FileReader( libBasePath
                                    + "astrodata/JPL_DE405_2011_2020.le" ) );
    shared_ptr< JPLEphemeris > de405_2011_2020le(
        new JPLEphemeris( spReader, true ) );
    if ( ! de405_2011_2020le->Test( libBasePath
                                    + "astro/test/testpo.405_2011_2020" ) )
        ok = false;

    JPLEphemeris::RegisterEphemeris( de405le );
    JPLEphemeris::RegisterEphemeris( de406be );

    if ( ! TestSiderealTime( de405le ) )
        ok = false;
    if ( ! Precession::Test( ) )
        ok = false;
    if ( ! TestObliquity( de405le ) )
        ok = false;
    if ( ! Nutation::Test( de405le ) )
        ok = false;
    if ( ! TestCoordinateReduction( de200be, de405le ) )
        ok = false;
    if ( ! TestApparentEphemeris( de200be, de405le ) )
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
    if ( ! TestConstellations( ) )
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


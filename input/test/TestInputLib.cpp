/*
  TestInputLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta input library.
*/


#include "Input.hpp"
#include "InputDeviceMap.hpp"
#include "InputEventMap.hpp"
#include "Exception.hpp"
#include "Assert.hpp"
#include "TestCheck.hpp"
#if USE_SDL
#include "Graphics2D.hpp"
#endif
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
    Graphics2D::Instance().Init( );
    Graphics2D::Instance().SetupScreen( 640, 480, "Test Input Lib" );

    if ( ! InputDeviceMap::Test( ) )
        ok = false;
    if ( ! InputEventMap::Test( ) )
        ok = false;
    if ( ! Input::Test( ) )
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

/*
  TestMMBaseLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta mmbase library.
*/


#include "Exception.hpp"
#ifdef USE_SDL
#include "SDL.hpp"
#include "SDLClock.hpp"
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
#ifdef USE_SDL
    if ( ! SDL::Instance().Test( ) )
        ok = false;
    if ( ! SDLClock::Instance().Test( ) )
        ok = false;
#endif //USE_SDL
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

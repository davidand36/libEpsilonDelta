/*
  SDLClock.cpp
  Copyright (C) 2007 David M. Anderson

  SDLClock class: A ticker giving the time elapsed since the start of the
  application, based on the Simple DirectMedia Layer (SDL) library.
*/


#ifdef USE_SDL


#include "SDLClock.hpp"
#include "SDL.hpp"
#include "SDLException.hpp"
#include <SDL.h>
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


SDLClock::SDLClock( )
{
    SDL::Instance();    //to force construction
}

//-----------------------------------------------------------------------------

SDLClock::~SDLClock( )
{
    Shutdown( );
}

//=============================================================================

void 
SDLClock::Init( )
{
    SDL::Instance().Init( );
    if ( SDL_WasInit( SDL_INIT_TIMER ) == 0 )
    {
        int initRslt = SDL_InitSubSystem( SDL_INIT_TIMER );

        if ( initRslt != 0 )
            throw SDLException( "SDL_InitSubSystem( SDL_INIT_TIMER )" );
    }
}

//-----------------------------------------------------------------------------

void 
SDLClock::Shutdown( )
{
    if ( SDL_WasInit( SDL_INIT_TIMER ) != 0 )
        SDL_QuitSubSystem( SDL_INIT_TIMER );
}

//=============================================================================

double 
SDLClock::Seconds( ) const
{
    return  SDL_GetTicks( ) / 1000.;
}

//=============================================================================

#ifdef DEBUG

bool 
SDLClock::Test( )
{
    bool ok = true;
    cout << "Testing SDLClock" << endl;

    cout << "Init()" << endl;
    SDLClock::Instance().Init( );

    cout << "MaxSeconds(): " << SDLClock::Instance().MaxSeconds() << endl;

    double now = SDLClock::Instance().Seconds( );
    cout << "now = SDLClock::Instance().Seconds( ): " << now << endl;
    TESTCHECK( (now >= 0.), true, &ok );
    //This test should be run within an hour of the test program's  beginning.
    TESTCHECK( (now < 3600.), true, &ok );  
    cout << "Waiting for 1.5 seconds..." << endl;
    while ( SDLClock::Instance().SecondsSince( now ) < 1.5 )
        ;
    TESTCHECKF( SDLClock::Instance().Seconds( ), (now + 1.5), &ok );

    double start = SDLClock::Instance().Seconds( );
    int numTicks = 0;
    double old = start;
    do
    {
        now = SDLClock::Instance().Seconds( );
        if ( old != now )
        {
            ++numTicks;
            old = now;
        }
    } while ( now < start + 10. );
    cout << "Ticks per second (est.): " << (numTicks / 10.) << endl;

    cout << "Shutdown()" << endl;
    SDLClock::Instance().Shutdown( );
    cout << "Shutdown() again" << endl;
    SDLClock::Instance().Shutdown( );

    if ( ok )
        cout << "SDLClock PASSED." << endl << endl;
    else
        cout << "SDLClock FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL


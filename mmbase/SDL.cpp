/*
  SDL.hpp
  Copyright (C) 2007 David M. Anderson

  SDL class: Handles initialization and shutdown of the Simple DirectMedia
  Layer (SDL).
*/


#ifdef USE_SDL


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


SDL::SDL( )
    :   m_initialized( false )
{
}

//-----------------------------------------------------------------------------

SDL::~SDL( )
{
    if ( m_initialized )
        SDL_Quit( );
}

//=============================================================================

void 
SDL::Init( )
{
    if ( ! m_initialized )
    {
        int initRslt = SDL_Init( 0 );  //Subsystems are initialized separately.

        if ( initRslt != 0 )
            throw SDLException( "SDL_Init(0)" );
        m_initialized = true;
    }
}

//=============================================================================

#ifdef DEBUG

bool 
SDL::Test( )
{
    bool ok = true;
    cout << "Testing SDL" << endl;

    try
    {
        cout << "Init( )" << endl;
        SDL::Instance().Init( );
        cout << "Init( ) again" << endl;
        SDL::Instance().Init( );
    }
    catch ( SDLException )
    {
        ok = false;
    }

    if ( ok )
        cout << "SDL PASSED." << endl << endl;
    else
        cout << "SDL FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL

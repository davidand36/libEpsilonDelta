/*
  Mouse.cpp
  Copyright (C) 2007 David M. Anderson

  Mouse class: the mouse input device.
*/


#include "Mouse.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const MouseState & 
Mouse::State( )
{
#ifdef USE_SDL
    int x, y;
    Uint8 buttonState = SDL_GetMouseState( &x, &y );
    m_state.Set( Vector2I( x, y ), buttonState );
#endif
    return m_state;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


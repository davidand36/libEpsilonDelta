/*
  Keyboard.cpp
  Copyright (C) 2007 David M. Anderson

  Keyboard class: the keyboard input device.
*/


#include "Keyboard.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const KeyboardState & 
Keyboard::State( )
{
#ifdef USE_SDL
    int numKeys;
    Uint8 * stateArray = SDL_GetKeyState( &numKeys );
    m_state.Set( stateArray, numKeys );
#endif
    return m_state;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


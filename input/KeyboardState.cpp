/*
  KeyboardState.cpp
  Copyright (C) 2007 David M. Anderson

  KeyboardState class: which keys are currently down (pressed, not yet
  released).
*/


#include "KeyboardState.hpp"
#include "Assert.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


KeyboardState::~KeyboardState( )
{
}

//=============================================================================

#ifdef USE_SDL

void 
KeyboardState::Set( uint8_t * keyStates, int numKeys )
{
    m_keyStates = keyStates;
    m_numKeys = numKeys;
}

#endif

//=============================================================================

bool 
KeyboardState::IsKeyDown( int key ) const
{
#ifdef USE_SDL
    Assert( (key >= 0) && (key < m_numKeys) );
    return  (m_keyStates[ key ] != 0);
#endif
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


/*
  MouseState.cpp
  Copyright (C) 2007 David M. Anderson

  MouseState class: position and which buttons are currently down.
*/


#include "MouseState.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


MouseState::~MouseState( )
{
}

//=============================================================================

#ifdef USE_SDL

void 
MouseState::Set( Vector2I position, uint8_t buttonState )
{
    m_position = position;
    m_buttonState = buttonState;
}

#endif

//=============================================================================

Vector2I
MouseState::Position( ) const
{
    return m_position;
}

//-----------------------------------------------------------------------------

bool 
MouseState::IsButtonDown( int button ) const
{
    Assert( (button >= 0) && (button < 8 * sizeof(m_buttonState)) );
    return  ( (m_buttonState & (1 << button)) != 0 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

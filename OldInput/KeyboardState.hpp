#ifndef KEYBOARDSTATE_HPP
#define KEYBOARDSTATE_HPP
/*
  KeyboardState.hpp
  Copyright (C) 2007 David M. Anderson

  KeyboardState class: which keys are currently down (pressed, not yet
  released).
*/


#include "InputDeviceState.hpp"
#include "StdInt.hpp"

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class KeyboardState
    :   public InputDeviceState
{
public:
    virtual ~KeyboardState( );

#ifdef USE_SDL
    void Set( uint8_t * keyStates, int numKeys );
#endif
    int NumKeys( ) const;
    bool IsKeyDown( int key ) const;

private:
#ifdef USE_SDL
    uint8_t * m_keyStates;
    int m_numKeys;
#endif
};


//*****************************************************************************


inline 
int 
KeyboardState::NumKeys( ) const
{
    return  m_numKeys;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //KEYBOARDSTATE_HPP

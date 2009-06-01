#ifndef MOUSESTATE_HPP
#define MOUSESTATE_HPP
/*
  MouseState.hpp
  Copyright (C) 2007 David M. Anderson

  MouseState class: position and which buttons are currently down.
*/


#include "InputDeviceState.hpp"
#include "Vector2.hpp"
#include "StdInt.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MouseState
    :   public InputDeviceState
{
public:
    virtual ~MouseState( );

#ifdef USE_SDL
    void Set( Vector2I position, uint8_t buttonState );
#endif
    Vector2I Position( ) const;
    bool IsButtonDown( int button ) const;

private:
#ifdef USE_SDL
    Vector2I m_position;
    uint8_t m_buttonState;
#endif
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MOUSESTATE_HPP

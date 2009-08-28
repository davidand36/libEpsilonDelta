#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
/*
  Keyboard.hpp
  Copyright (C) 2007 David M. Anderson

  Keyboard class: the keyboard input device.
*/


#include "InputDevice.hpp"
#include "KeyboardState.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Keyboard
    :   public InputDevice
{
public:
    virtual ~Keyboard( );

    virtual EDeviceType Type( ) const;
    virtual std::string Name( ) const;

    virtual const KeyboardState & State( );

private:
    KeyboardState m_state;
};


//*****************************************************************************


inline 
Keyboard::~Keyboard( )
{
}

//=============================================================================

inline
InputDevice::EDeviceType 
Keyboard::Type( ) const
{
    return KeyboardDevice;
}

//-----------------------------------------------------------------------------

inline
std::string 
Keyboard::Name( ) const
{
    return "keyboard";
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //KEYBOARD_HPP

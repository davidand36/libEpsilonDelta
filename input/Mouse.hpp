#ifndef MOUSE_HPP
#define MOUSE_HPP
/*
  Mouse.hpp
  Copyright (C) 2007 David M. Anderson

  Mouse class: the mouse input device.
*/


#include "InputDevice.hpp"
#include "MouseState.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Mouse
    :   public InputDevice
{
public:
    virtual ~Mouse( );

    virtual EDeviceType Type( ) const;
    virtual std::string Name( ) const;

    virtual const MouseState & State( );

private:
    MouseState m_state;
};


//*****************************************************************************


inline 
Mouse::~Mouse( )
{
}

//=============================================================================

inline
InputDevice::EDeviceType 
Mouse::Type( ) const
{
    return MouseDevice;
}

//-----------------------------------------------------------------------------

inline
std::string 
Mouse::Name( ) const
{
    return "mouse";
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MOUSE_HPP

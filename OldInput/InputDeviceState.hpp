#ifndef INPUTDEVICESTATE_HPP
#define INPUTDEVICESTATE_HPP
/*
  InputDeviceState.hpp
  Copyright (C) 2007 David M. Anderson

  InputDeviceState abstract base class: representing the current state
  (keys, position, axes) of an input device.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputDeviceState
{
public:
    virtual ~InputDeviceState( );
};

//*****************************************************************************


inline 
InputDeviceState::~InputDeviceState( )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTDEVICESTATE_HPP

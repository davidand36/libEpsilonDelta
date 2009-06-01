#ifndef INPUTDEVICE_HPP
#define INPUTDEVICE_HPP
/*
  InputDevice.hpp
  Copyright (C) 2007 David M. Anderson
  
  InputDevice abstact base class.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class InputDeviceState;

//*****************************************************************************


class InputDevice
{
public:
    virtual ~InputDevice( );

    enum EDeviceType { KeyboardDevice, MouseDevice, JoystickDevice,
                       WiimoteDevice };
    virtual EDeviceType Type( ) const = 0;
    virtual std::string Name( ) const = 0;

    virtual const InputDeviceState & State( ) = 0;
};


//*****************************************************************************


inline 
InputDevice::~InputDevice( )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTDEVICE_HPP

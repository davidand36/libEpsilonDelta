#ifndef GAMEPAD_HPP
#define GAMEPAD_HPP
/*
  Gamepad.hpp
  Copyright (C) 2009 David M. Anderson

  Gamepad class: a standard gamepad, a.k.a. joystick.
*/


#include "InputDevice.hpp"
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class GamepadImpl;


//*****************************************************************************


class Gamepad
    :   public InputDevice
{
public:
    Gamepad( const std::string & name, int index );
    virtual ~Gamepad( );

    virtual int NumButtons( ) const;
    virtual bool ButtonDown( int button ) const;

    virtual int NumAxes( ) const;
    virtual double Axis( int index = 0 ) const;

private:
    std::tr1::shared_ptr< GamepadImpl >     m_pImpl;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GAMEPAD_HPP

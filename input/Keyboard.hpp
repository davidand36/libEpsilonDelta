#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
/*
  Keyboard.hpp
  Copyright (C) 2009 David M. Anderson

  Keyboard class: a standard keyboard.
*/


#include "InputDevice.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class KeyboardImpl;


//*****************************************************************************


class Keyboard
    :   public InputDevice
{
public:
    Keyboard( const std::string & name );
    virtual ~Keyboard( );

    virtual int NumButtons( ) const;
    virtual bool ButtonDown( int button ) const;

private:
    shared_ptr< KeyboardImpl >  m_pImpl;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //KEYBOARD_HPP

#ifndef KEYPRESSEVENT_HPP
#define KEYPRESSEVENT_HPP
/*
  KeyPressEvent.hpp
  Copyright (C) 2007 David M. Anderson

  KeyPressEvent class: represents the press of a key or button on an input
  device (keyboard or joystick).
*/


#include "InputEvent.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class KeyPressEvent
    :   public InputEvent
{
public:
    KeyPressEvent( shared_ptr< InputDevice > device, int key );
    virtual ~KeyPressEvent( );

    int Key( ) const;

private:
    int m_key;
};


//*****************************************************************************


inline
KeyPressEvent::KeyPressEvent( shared_ptr< InputDevice > device, int key )
    :   InputEvent( device ),
        m_key( key )
{
}

//-----------------------------------------------------------------------------

inline
KeyPressEvent::~KeyPressEvent( )
{
}

//=============================================================================

inline
int 
KeyPressEvent::Key( ) const
{
    return m_key;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //KEYPRESSEVENT_HPP

#ifndef INPUTEVENT_HPP
#define INPUTEVENT_HPP
/*
  InputEvent.hpp
  Copyright (C) 2009 David M. Anderson

  InputEvent class:
*/


#include "InputDevice.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputEvent
{
public:
    InputEvent( const shared_ptr< InputDevice > device, int button );

    const shared_ptr< InputDevice > Device( ) const;
    int Button( ) const;

private:
    const shared_ptr< InputDevice > m_device;
    int                             m_button;
};


//*****************************************************************************


inline
InputEvent::InputEvent( const shared_ptr< InputDevice > device, int button )
    :   m_device( device ),
        m_button( button )
{
}

//=============================================================================

inline
const shared_ptr< InputDevice > 
InputEvent::Device( ) const
{
    return m_device;
}

//-----------------------------------------------------------------------------

inline
int 
InputEvent::Button( ) const
{
    return m_button;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTEVENT_HPP

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
    InputEvent( shared_ptr< InputDevice const > device, int button );

    shared_ptr< InputDevice const > Device( ) const;
    int Button( ) const;

private:
    shared_ptr< InputDevice const > m_device;
    int                             m_button;
};


//*****************************************************************************


inline
InputEvent::InputEvent( shared_ptr< InputDevice const > device, int button )
    :   m_device( device ),
        m_button( button )
{
}

//=============================================================================

inline
shared_ptr< InputDevice const > 
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

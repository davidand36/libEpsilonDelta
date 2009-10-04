#ifndef INPUTEVENT_HPP
#define INPUTEVENT_HPP
/*
  InputEvent.hpp
  Copyright (C) 2009 David M. Anderson

  InputEvent class:
*/


#include "InputDevice.hpp"
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputEvent
{
public:
    InputEvent( std::tr1::shared_ptr< InputDevice const > device, int button );

    std::tr1::shared_ptr< InputDevice const > Device( ) const;
    int Button( ) const;

private:
    std::tr1::shared_ptr< InputDevice const > m_device;
    int                             m_button;
};


//*****************************************************************************


inline
InputEvent::InputEvent( std::tr1::shared_ptr< InputDevice const > device,
                        int button )
    :   m_device( device ),
        m_button( button )
{
}

//=============================================================================

inline
std::tr1::shared_ptr< InputDevice const > 
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

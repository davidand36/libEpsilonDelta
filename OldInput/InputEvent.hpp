#ifndef INPUTEVENT_HPP
#define INPUTEVENT_HPP
/*
  InputEvent.hpp
  Copyright (C) 2007 David M. Anderson

  InputEvent abstract base class.
*/


#include "SmartPtr.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class InputDevice;

//*****************************************************************************


class InputEvent
{
public:
    InputEvent( shared_ptr< InputDevice > device );
    virtual ~InputEvent( );

    shared_ptr< InputDevice > Device( ) const;

private:
    shared_ptr< InputDevice > m_device;
};


//=============================================================================


typedef shared_ptr< const InputEvent >  InputEventSPtr;


//*****************************************************************************

inline 
InputEvent::InputEvent( shared_ptr< InputDevice > device )
    :   m_device( device )
{
}

//-----------------------------------------------------------------------------

inline 
InputEvent::~InputEvent( )
{
}

//=============================================================================

inline 
shared_ptr< InputDevice >
InputEvent::Device( ) const
{
    return m_device;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTEVENT_HPP

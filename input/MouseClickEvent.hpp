#ifndef MOUSECLICKEVENT_HPP
#define MOUSECLICKEVENT_HPP
/*
  MouseClickEvent.hpp
  Copyright (C) 2007 David M. Anderson

  MouseClickEvent class: represents the press of a mouse button.
*/


#include "InputEvent.hpp"
#include "Vector2.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MouseClickEvent
    :   public InputEvent
{
public:
    MouseClickEvent( shared_ptr< InputDevice > device,
                     int button, Vector2I position );
    virtual ~MouseClickEvent( );

    int Button( ) const;
    Vector2I Position( ) const;

private:
    int m_button;
    Vector2I m_position;
};


//*****************************************************************************


inline 
MouseClickEvent::MouseClickEvent( shared_ptr< InputDevice > device,
                                  int button, Vector2I position )
    :   InputEvent( device ),
        m_button( button ),
        m_position( position )
{
}

//-----------------------------------------------------------------------------

inline 
MouseClickEvent::~MouseClickEvent( )
{
}

//=============================================================================

inline 
int 
MouseClickEvent::Button( ) const
{
    return m_button;
}

//-----------------------------------------------------------------------------

inline 
Vector2I 
MouseClickEvent::Position( ) const
{
    return m_position;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MOUSECLICKEVENT_HPP

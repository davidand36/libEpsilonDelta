#ifndef MOUSECLICKEVENT_HPP
#define MOUSECLICKEVENT_HPP
/*
  MouseClickEvent.hpp
  Copyright (C) 2007 David M. Anderson

  MouseClickEvent class: represents the press of a mouse button.
*/


#include "InputEvent.hpp"
#include "Point2.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MouseClickEvent
    :   public InputEvent
{
public:
    MouseClickEvent( shared_ptr< InputDevice > device,
                     int button, const Point2I & position );
    virtual ~MouseClickEvent( );

    int Button( ) const;
    const Point2I & Position( ) const;

private:
    int         m_button;
    Point2I     m_position;
};


//*****************************************************************************


inline 
MouseClickEvent::MouseClickEvent( shared_ptr< InputDevice > device,
                                  int button, const Point2I & position )
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
const Point2I &
MouseClickEvent::Position( ) const
{
    return m_position;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MOUSECLICKEVENT_HPP

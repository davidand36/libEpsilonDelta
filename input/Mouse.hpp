#ifndef MOUSE_HPP
#define MOUSE_HPP
/*
  Mouse.hpp
  Copyright (C) 2009 David M. Anderson

  Mouse class: a standard mouse.
*/


#include "InputDevice.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MouseImpl;


//*****************************************************************************


class Mouse
    :   public InputDevice
{
public:
    Mouse( const std::string & name );
    virtual ~Mouse( );

    enum Button
    {
        LeftButton = 0,
        MiddleButton,
        RightButton,
        WheelUp,
        WheelDown
    };

    virtual int NumButtons( ) const;
    virtual bool ButtonDown( int button ) const;

    virtual int NumPointers( ) const;
    virtual Point2I Pointer( int index = 0 ) const;

private:
    shared_ptr< MouseImpl >  m_pImpl;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MOUSE_HPP

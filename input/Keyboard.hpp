#ifndef KEYBOARD_HPP
#define KEYBOARD_HPP
/*
  Keyboard.hpp
  Copyright (C) 2009 David M. Anderson

  Keyboard class: a standard keyboard.
*/


#include "InputDevice.hpp"
#ifdef USE_SDL
#include <SDL_keysym.h>
#endif


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

#ifdef USE_SDL
    enum Key
    {
        Left        = SDLK_LEFT,
        Right       = SDLK_RIGHT,
        Up          = SDLK_UP,
        Down        = SDLK_DOWN,
        Home        = SDLK_HOME,
        End         = SDLK_END,
        PageUp      = SDLK_PAGEUP,
        PageDown    = SDLK_PAGEDOWN,
        Insert      = SDLK_INSERT,
        Delete      = SDLK_DELETE,
        Backspace   = SDLK_BACKSPACE,
        Return      = SDLK_RETURN,
        Enter       = SDLK_KP_ENTER,
        Tab         = SDLK_TAB,
        Escape      = SDLK_ESCAPE,
        F1          = SDLK_F1,
        F2          = SDLK_F2,
        F3          = SDLK_F3,
        F4          = SDLK_F4,
        F5          = SDLK_F5,
        F6          = SDLK_F6,
        F7          = SDLK_F7,
        F8          = SDLK_F8,
        F9          = SDLK_F9,
        F10         = SDLK_F10,
        F11         = SDLK_F11,
        F12         = SDLK_F12,
        CapsLock    = SDLK_CAPSLOCK,
        NumLock     = SDLK_NUMLOCK,
        ScrollLock  = SDLK_SCROLLOCK,
        LeftShift   = SDLK_LSHIFT,
        RightShift  = SDLK_RSHIFT,
        LeftCtrl    = SDLK_LCTRL,
        RightCtrl   = SDLK_RCTRL,
        LeftAlt     = SDLK_LALT,
        RightAlt    = SDLK_RALT,
        LeftMeta    = SDLK_LMETA,
        RightMeta   = SDLK_RMETA,
        LeftSuper   = SDLK_LSUPER,
        RightSuper  = SDLK_RSUPER,
        PrintScrn   = SDLK_PRINT,
        SysReq      = SDLK_SYSREQ,
        Break       = SDLK_BREAK,
        Menu        = SDLK_MENU
    };
#endif //SDL

private:
    shared_ptr< KeyboardImpl >  m_pImpl;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //KEYBOARD_HPP

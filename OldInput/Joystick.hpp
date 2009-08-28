#ifndef JOYSTICK_HPP
#define JOYSTICK_HPP


/*
  Joystick.hpp
  Copyright (C) 2007 David M. Anderson

  Joystick class: the joystick input device.
*/


#include "InputDevice.hpp"
#include "JoystickState.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Joystick
    :   public InputDevice
{
public:
#ifdef USE_SDL
    Joystick( int index );
#endif
    virtual ~Joystick( );

    virtual EDeviceType Type( ) const;
    virtual std::string Name( ) const;
#ifdef USE_SDL
    int Index( ) const;
#endif

    virtual const JoystickState & State( );
    int NumButtons( ) const;
    int NumAxes( ) const;

private:
#ifdef USE_SDL
    int m_index;
    SDL_Joystick * m_pSDL_Joystick;
    int m_numButtons;
    int m_numAxes;
#endif
    JoystickState m_state;
};


//*****************************************************************************


inline
InputDevice::EDeviceType 
Joystick::Type( ) const
{
    return JoystickDevice;
}

//-----------------------------------------------------------------------------

#ifdef USE_SDL

inline 
int 
Joystick::Index( ) const
{
    return m_index;
}

#endif

//=============================================================================

inline 
int 
Joystick::NumButtons( ) const
{
#ifdef USE_SDL
    return m_numButtons;
#else
    return 0;
#endif
}

//-----------------------------------------------------------------------------

inline 
int 
Joystick::NumAxes( ) const
{
#ifdef USE_SDL
    return m_numAxes;
#else
    return 0;
#endif
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JOYSTICK_HPP

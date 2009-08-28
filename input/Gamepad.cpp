/*
  Gamepad.cpp
  Copyright (C) 2009 David M. Anderson

  Gamepad class: a standard gamepad, a.k.a. joystick.
*/


#include "Gamepad.hpp"
#ifdef USE_SDL
#include "SDLException.hpp"
#include <SDL.h>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class GamepadImpl
{
public:
    GamepadImpl( int index );
    ~GamepadImpl( );

    int NumButtons( ) const;
    bool ButtonDown( int button ) const;

    int NumAxes( ) const;
    double Axis( int index ) const;
    
private:
#ifdef USE_SDL
    int m_index;
    ::SDL_Joystick *    m_pSDL_Joystick;
    int                 m_numButtons;
    int                 m_numAxes;
#endif
};


//*****************************************************************************


Gamepad::Gamepad( const std::string & name, int index )
    :   InputDevice( InputDevice::Gamepad, name ),
        m_pImpl( new GamepadImpl( index ) )
{
}

//-----------------------------------------------------------------------------

Gamepad::~Gamepad( )
{
}

//=============================================================================

int 
Gamepad::NumButtons( ) const
{
    return m_pImpl->NumButtons();
}

//-----------------------------------------------------------------------------

bool 
Gamepad::ButtonDown( int button ) const
{
    return m_pImpl->ButtonDown( button );
}

//=============================================================================

int 
Gamepad::NumAxes( ) const
{
    return m_pImpl->NumAxes();
}

//-----------------------------------------------------------------------------

double 
Gamepad::Axis( int index ) const
{
    return m_pImpl->Axis( index );
}


//*****************************************************************************


#ifdef USE_SDL

//=============================================================================


GamepadImpl::GamepadImpl( int index )
{
    m_pSDL_Joystick = ::SDL_JoystickOpen( index );
    if ( m_pSDL_Joystick == 0 )
        throw SDLException( "SDL_JoystickOpen" );
    m_numButtons = ::SDL_JoystickNumButtons( m_pSDL_Joystick );
    m_numAxes = ::SDL_JoystickNumAxes( m_pSDL_Joystick );
}

//-----------------------------------------------------------------------------

GamepadImpl::~GamepadImpl( )
{
    ::SDL_JoystickClose( m_pSDL_Joystick );
}

//=============================================================================

int 
GamepadImpl::NumButtons( ) const
{
    return m_numButtons;
}

//-----------------------------------------------------------------------------

bool 
GamepadImpl::ButtonDown( int button ) const
{
    return (::SDL_JoystickGetButton( m_pSDL_Joystick, button ) != 0);
}

//=============================================================================

int 
GamepadImpl::NumAxes( ) const
{
    return m_numAxes;
}

//-----------------------------------------------------------------------------

double 
GamepadImpl::Axis( int index ) const
{
    return (::SDL_JoystickGetAxis( m_pSDL_Joystick, index ) / 32768.);
}

//=============================================================================

#endif //USE_SDL

//*****************************************************************************

}                                                      //namespace EpsilonDelta

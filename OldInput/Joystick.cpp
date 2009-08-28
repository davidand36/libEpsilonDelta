/*
  Joystick.cpp
  Copyright (C) 2007 David M. Anderson

  Joystick class: the joystick input device.
*/


#include "Joystick.hpp"
#include "SDLException.hpp"
#include "SmartPtr.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef USE_SDL

Joystick::Joystick( int index )
    :   m_index( index )
{
    m_pSDL_Joystick = SDL_JoystickOpen( index );
    if ( m_pSDL_Joystick == 0 )
        throw SDLException( "SDL_JoystickOpen" );
    m_numButtons = SDL_JoystickNumButtons( m_pSDL_Joystick );
    m_numAxes = SDL_JoystickNumAxes( m_pSDL_Joystick );
}

#endif

//-----------------------------------------------------------------------------

Joystick::~Joystick( )
{
#ifdef USE_SDL
    SDL_JoystickClose( m_pSDL_Joystick );
#endif
}

//=============================================================================

std::string
Joystick::Name( ) const
{
#ifdef USE_SDL
    return  SDL_JoystickName( m_index );
#endif
}

//=============================================================================

const JoystickState & 
Joystick::State( )
{
#ifdef USE_SDL
    std::vector< bool > buttonStates;
    buttonStates.reserve( m_numButtons );
    for ( int i = 0; i < m_numButtons; ++i )
        buttonStates.push_back(
            SDL_JoystickGetButton( m_pSDL_Joystick, i ) != 0 );
    std::vector< int > axes;
    axes.reserve( m_numAxes );
    for ( int i = 0; i < m_numAxes; ++i )
        axes.push_back( SDL_JoystickGetAxis( m_pSDL_Joystick, i ) );
    m_state.Set( buttonStates, axes );
#endif
    return m_state;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


/*
  Keyboard.cpp
  Copyright (C) 2009 David M. Anderson

  Keyboard class: a standard keyboard.
*/


#include "Keyboard.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class KeyboardImpl
{
public:
    KeyboardImpl( );
    ~KeyboardImpl( );

    int NumButtons( ) const;
    bool ButtonDown( int button ) const;
    
private:
#ifdef USE_SDL
    int                 m_numKeys;
    const uint8_t *     m_keyArray; //Note: this belongs to SDL.
#endif
};


//*****************************************************************************


Keyboard::Keyboard( const std::string & name )
    :   InputDevice( InputDevice::Keyboard, name ),
        m_pImpl( new KeyboardImpl )
{
}

//-----------------------------------------------------------------------------

Keyboard::~Keyboard( )
{
}

//=============================================================================

int 
Keyboard::NumButtons( ) const
{
    return m_pImpl->NumButtons();
}

//-----------------------------------------------------------------------------

bool 
Keyboard::ButtonDown( int button ) const
{
    return m_pImpl->ButtonDown( button );
}


//*****************************************************************************


#ifdef USE_SDL

//=============================================================================


KeyboardImpl::KeyboardImpl( )
{
    m_keyArray = ::SDL_GetKeyState( &m_numKeys );
}

//-----------------------------------------------------------------------------

KeyboardImpl::~KeyboardImpl( )
{
}

//=============================================================================

int 
KeyboardImpl::NumButtons( ) const
{
    return m_numKeys;
}

//-----------------------------------------------------------------------------

bool 
KeyboardImpl::ButtonDown( int button ) const
{
    if ( (button < 0) || (button >= m_numKeys) )
        throw std::out_of_range( "Keyboard::ButtonDown()" );
    return (int)m_keyArray[ button ];
}

//=============================================================================

#endif //USE_SDL

//*****************************************************************************

}                                                      //namespace EpsilonDelta

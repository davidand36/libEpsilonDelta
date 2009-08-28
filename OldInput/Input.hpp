#ifndef INPUT_HPP
#define INPUT_HPP
/*
  Input.hpp
  Copyright (C) 2007 David M. Anderson

  Input class: user input subsystem.
  NOTES:
  1. If SUPPORT_WIIMOTE is defined, but an application doesn't want to support
     Wiimotes (especially waiting for a connection at startup with CWiiD),
     call SupportWiimotes( false ) before calling Init().
*/


#include "InputEvent.hpp"
#include "Singleton.hpp"
#include "SmartPtr.hpp"
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class InputEvent;
class Keyboard;
class Mouse;
class Joystick;
#if defined(SUPPORT_WIIMOTE)
class Wiimote;
#endif


//*****************************************************************************


class Input
    :   public Singleton< Input >
{
public:
    void Init( );
    void Shutdown( );

    void Pump( );
    InputEventSPtr CheckEvent( );

    typedef void (*QuitHandler)( );
    void SetQuitHandler( QuitHandler quitHandler );

    shared_ptr< Keyboard > GetKeyboard( );
    shared_ptr< Mouse > GetMouse( );
    int NumJoysticks( ) const;
    shared_ptr< Joystick > GetJoystick( int i );
#if defined(SUPPORT_WIIMOTE)
    void SupportWiimotes( bool supportWiimotes );
    int NumWiimotes( ) const;
    shared_ptr< Wiimote > GetWiimote( int i );
#endif

#ifdef DEBUG
    bool Test( );
#endif

private:
    Input( );
    ~Input( );

    shared_ptr< Keyboard >                  m_pKeyboard;
    shared_ptr< Mouse >                     m_pMouse;
    std::vector< shared_ptr< Joystick > >   m_joysticks;
#if defined(SUPPORT_WIIMOTE)
    bool                                    m_supportWiimotes;
    std::vector< shared_ptr< Wiimote > >    m_wiimotes;
#endif

    QuitHandler                             m_quitHandler;

    friend class Singleton< Input >;
};


//*****************************************************************************


inline
void 
Input::SetQuitHandler( QuitHandler quitHandler )
{
    m_quitHandler = quitHandler;
}

//=============================================================================

inline
shared_ptr< Keyboard > 
Input::GetKeyboard( )
{
    return m_pKeyboard;
}

//-----------------------------------------------------------------------------

inline
shared_ptr< Mouse > 
Input::GetMouse( )
{
    return m_pMouse;
}

//-----------------------------------------------------------------------------

inline
int 
Input::NumJoysticks( ) const
{
    return static_cast<int>( m_joysticks.size() );
}

//-----------------------------------------------------------------------------

inline
shared_ptr< Joystick >
Input::GetJoystick( int i )
{
    return m_joysticks.at( i );
}

//-----------------------------------------------------------------------------

#if defined(SUPPORT_WIIMOTE)

inline
void 
Input::SupportWiimotes( bool supportWiimotes )
{
    m_supportWiimotes = supportWiimotes;
}

//-----------------------------------------------------------------------------

inline
int 
Input::NumWiimotes( ) const
{
    return static_cast<int>( m_wiimotes.size() );
}

//-----------------------------------------------------------------------------

inline
shared_ptr< Wiimote >
Input::GetWiimote( int i )
{
    return m_wiimotes.at( i );
}

#endif



//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUT_HPP

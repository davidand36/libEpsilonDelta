#ifndef INPUT_HPP
#define INPUT_HPP
/*
  Input.hpp
  Copyright (C) 2007 David M. Anderson

  Input class: user input subsystem.
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

    int NumJoysticks( ) const;
    shared_ptr< Joystick > GetJoystick( int i );
#if defined(SUPPORT_WIIMOTE)
    int NumWiimotes( ) const;
    shared_ptr< Wiimote > GetWiimote( int i );
#endif

#ifdef DEBUG
    bool Test( );
#endif

private:
    Input( );
    ~Input( );

    shared_ptr< Keyboard > m_pKeyboard;
    shared_ptr< Mouse > m_pMouse;
    std::vector< shared_ptr< Joystick > > m_joysticks;
#if defined(SUPPORT_WIIMOTE)
    std::vector< shared_ptr< Wiimote > > m_wiimotes;
#endif

    QuitHandler m_quitHandler;

    friend class Singleton< Input >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUT_HPP

/*
  SDLInput.cpp
  Copyright (C) 2007 David M. Anderson

  Input class: user input subsystem based on the Simple DirectMedia Layer
  (SDL) library.
*/


#ifdef USE_SDL


#include "Input.hpp"
#include "SDL.hpp"
#include "SDLException.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Joystick.hpp"
#if defined(SUPPORT_WIIMOTE)
#include "Wiimote.hpp"
#endif
#include "KeyPressEvent.hpp"
#include "MouseClickEvent.hpp"
#include "Point2.hpp"
#include <SDL.h>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Timer.hpp"
#include "KeyboardState.hpp"
#include "MouseState.hpp"
#include "JoystickState.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

Input::Input( )
    :   m_quitHandler( 0 )
#if defined(SUPPORT_WIIMOTE)
    ,   m_supportWiimotes( true )
#endif
{
    SDL::Instance();    //to force construction
}

//-----------------------------------------------------------------------------

Input::~Input( )
{
    Shutdown( );
}

//=============================================================================

void 
Input::Init( )
{
#if defined(SUPPORT_WIIMOTE)
    if ( m_supportWiimotes )
    {
        //WiiYourself! needs to connect before SDL inits joysticks.
        shared_ptr< Wiimote > pWiimote( new Wiimote );
        if ( pWiimote->IsConnected() )
            m_wiimotes.push_back( pWiimote );
    }
#endif

    SDL::Instance().Init( );
    Assert( SDL_WasInit( SDL_INIT_VIDEO ) != 0 );
    if ( SDL_WasInit( SDL_INIT_JOYSTICK ) == 0 )
    {
        int initRslt = SDL_InitSubSystem( SDL_INIT_JOYSTICK );

        if ( initRslt != 0 )
            throw SDLException( "SDL_InitSubSystem( SDL_INIT_JOYSTICK )" );
    }
    int joystickEventState = SDL_JoystickEventState( SDL_ENABLE );
    if ( joystickEventState != SDL_ENABLE )
        throw SDLException( "SDL_JoystickEventState" );
    if ( m_pKeyboard == 0 )
        m_pKeyboard.reset( new Keyboard );
    if ( m_pMouse == 0 )
        m_pMouse.reset( new Mouse );
    int numJoysticks = SDL_NumJoysticks( );
    m_joysticks.clear();
    m_joysticks.reserve( numJoysticks );
    for ( int i = 0; i < numJoysticks; ++i )
        m_joysticks.push_back( shared_ptr< Joystick >( new Joystick( i ) ) );
}

//-----------------------------------------------------------------------------

void 
Input::Shutdown( )
{
    m_joysticks.clear();
    m_pMouse.reset();
    m_pKeyboard.reset();
    if ( SDL_WasInit( SDL_INIT_JOYSTICK ) != 0 )
        SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
}

//=============================================================================

void 
Input::Pump( )
{
    SDL_PumpEvents( );
#ifdef SUPPORT_WIIMOTE
    for ( int w = 0; w < (int)m_wiimotes.size(); ++w )
        m_wiimotes[ w ]->Update( );
#endif
}

//-----------------------------------------------------------------------------

InputEventSPtr 
Input::CheckEvent( )
{
    SDL_Event sdlEvent;
    int pollRslt = SDL_PollEvent( &sdlEvent );
    if ( pollRslt != 0 )
    {
        switch( sdlEvent.type )
        {
        case SDL_KEYDOWN:
        {
            shared_ptr< InputDevice > pDevice = m_pKeyboard;
            int key = sdlEvent.key.keysym.sym;
            return  InputEventSPtr( new KeyPressEvent( pDevice, key ) );
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            shared_ptr< InputDevice > pDevice = m_pMouse;
            int button = sdlEvent.button.button;
            Point2I position( sdlEvent.button.x, sdlEvent.button.y );
            return  InputEventSPtr(
                new MouseClickEvent( pDevice, button, position ) );
        }
        case SDL_JOYBUTTONDOWN:
        {
            Assert( sdlEvent.jbutton.which < m_joysticks.size() );
            shared_ptr< InputDevice > pDevice
                    = m_joysticks[ sdlEvent.jbutton.which ];
            int key = sdlEvent.jbutton.button;
            return  InputEventSPtr( new KeyPressEvent( pDevice, key ) );
        }
        case SDL_QUIT:
        {
            if ( m_quitHandler != 0 )
                m_quitHandler( );
            else
                exit( 0 );
            return  InputEventSPtr();
        }
        default:
            break;
        }
    }
#if defined(SUPPORT_WIIMOTE)
    for ( int w = 0; w < (int)m_wiimotes.size(); ++w )
    {
        shared_ptr< Wiimote > pWiimote = m_wiimotes[ w ];
        if ( pWiimote->WasButtonPressed() )
        {
            WiimoteState::Button button = pWiimote->GetButtonPressed( );
            return InputEventSPtr( new KeyPressEvent( pWiimote, button ) );
        }
    }
#endif
    return  InputEventSPtr(); //0 = no event
}

//=============================================================================

#ifdef DEBUG

bool 
Input::Test( )
{
    bool ok = true;
    cout << "Testing Input" << endl;

    try
    {
#if defined(USE_CWIID)
        cout << "To use a Wiimote, hold buttons 1 & 2 down." << endl;
#endif
        cout << "Init()" << endl;
        Input::Instance().Init( );

        cout << "Number of joysticks: " << Input::Instance().NumJoysticks()
             << endl;

#ifdef SUPPORT_WIIMOTE
        cout << "Number of wiimotes: " << Input::Instance().NumWiimotes()
             << endl;
#endif

        Timer realTime;
        while ( true )
        {
            Input::Instance().Pump( );
            InputEventSPtr spEvt = Input::Instance().CheckEvent( );
            if ( spEvt != 0 )
            {
                shared_ptr< const KeyPressEvent > pKPEvt
                        = dynamic_pointer_cast< const KeyPressEvent >( spEvt );
                if ( pKPEvt != 0 )
                {
                    cout << "KeyPressEvent key = " << pKPEvt->Key() << endl;
                    shared_ptr< InputDevice > pDev = pKPEvt->Device();
                    cout << " Device name = " << pDev->Name() << endl;

                    switch ( pDev->Type() )
                    {
                    case InputDevice::KeyboardDevice:
                    {
                        cout << " Device type = KeyboardDevice" << endl;
                        const KeyboardState & kbState
                                = dynamic_cast< const KeyboardState & >(
                                    pDev->State() );
                        int numKeys = kbState.NumKeys( );
                        for ( int k = 0; k < numKeys; ++k )
                            if ( kbState.IsKeyDown( k ) )
                                cout << " Key down: " << k << endl;
                        break;
                    }
                    case InputDevice::JoystickDevice:
                    {
                        cout << " Device type = JoystickDevice" << endl;
                        shared_ptr< Joystick > pJoystick
                                = dynamic_pointer_cast< Joystick >( pDev );
                        Assert( pJoystick != 0 );
                        cout << " Joystick index = " << pJoystick->Index()
                             << endl;
                        const JoystickState & jsState = pJoystick->State();
                        int numButtons = pJoystick->NumButtons( );
                        for ( int b = 0; b < numButtons; ++b )
                            if ( jsState.IsButtonDown( b ) )
                                cout << " Button down: " << b << endl;
                        int numAxes = pJoystick->NumAxes( );
                        for ( int a = 0; a < numAxes; ++a )
                            cout << " Axis " << a << " position: "
                                 << jsState.AxisPosition( a ) << endl;
                        break;
                    }
#ifdef SUPPORT_WIIMOTE
                    case InputDevice::WiimoteDevice:
                    {
                        cout << " Device type = WiimoteDevice" << endl;
                        shared_ptr< Wiimote > pWiimote
                                = dynamic_pointer_cast< Wiimote >( pDev );
                        Assert( pWiimote != 0 );
                        const WiimoteState & wmState = pWiimote->State();
                        for ( int i = 0; i < WiimoteState::NumButtons; ++i )
                        {
                            WiimoteState::Button b
                                    = static_cast< WiimoteState::Button >( i );
                            if ( wmState.IsButtonDown( b ) )
                                cout << " Button down: " << b << endl;
                        }
                        Vector3F acc = wmState.Acceleration();
                        cout << " Acceleration: (" << acc.X() << ", "
                             << acc.Y() << ", " << acc.Z() << " )" << endl;
                        Vector3F grav = wmState.Gravity();
                        cout << " Gravity: (" << grav.X() << ", "
                             << grav.Y() << ", " << grav.Z() << " )" << endl;
                        cout << " Pointer: NumLights: "
                             << wmState.NumPointerLights() << endl;
                        Point2F pos = wmState.PointerPos();
                        cout << "          Position: (" << pos.X() << ", "
                             << pos.Y() << " )"
                             << " Angle: " << wmState.PointerAngle().Degrees()
                             << " Distance: " << wmState.PointerDistance()
                             << endl;
                        if ( wmState.IsNunchukConnected() )
                        {
                            cout << " Nunchuk: ";
                            acc = wmState.NunchukAcceleration();
                            cout << " Acceleration: (" << acc.X() << ", "
                                 << acc.Y() << ", " << acc.Z() << " )" << endl;
                            grav = wmState.Gravity();
                            cout << " Gravity: (" << grav.X() << ", "
                                 << grav.Y() << ", " << grav.Z() << " )"
                                 << endl;
                            Point2F joystick = wmState.NunchukJoystickPos();
                            cout << "           Joystick: (" << joystick.X()
                                 << ", " << joystick.Y() << " )";
                            cout << endl;
                        }
                        break;
                    }
#endif
                    default:
                        cout << " Unexpected device type: " << pDev->Type()
                             << endl;
                        ok = false;
                    }

                }
                shared_ptr< const MouseClickEvent > pMCEvt
                      = dynamic_pointer_cast< const MouseClickEvent >( spEvt );
                if ( pMCEvt != 0 )
                {
                    cout << "MouseClickEvent button = " << pMCEvt->Button()
                         << endl;
                    Point2I pos = pMCEvt->Position();
                    cout << "                position = ( " << pos.X() << ", "
                         << pos.Y() << " )" << endl;
                    shared_ptr< InputDevice > pDev = pMCEvt->Device();
                    cout << " Device name=" << pDev->Name() << endl;
                    switch ( pDev->Type() )
                    {
                    case InputDevice::MouseDevice:
                    {
                        cout << " Device type=MouseDevice" << endl;
                        shared_ptr< Mouse > pMouse
                                = dynamic_pointer_cast< Mouse >( pDev );
                        Assert( pMouse != 0 );
                        const MouseState & mState
                                = dynamic_cast< const MouseState & >(
                                    pDev->State() );
                        pos = mState.Position();
                        cout << " Position=( " << pos.X() << ", " << pos.Y()
                             << " )" << endl;
                        for ( int b = 0; b < 8; ++b )
                            if ( mState.IsButtonDown( b ) )
                                cout << " Button down: " << b << endl;
                        break;
                    }
                    default:
                        cout << " Unexpected device type: " << pDev->Type()
                             << endl;
                        ok = false;
                    }
                }
            }
            if ( realTime.Seconds( ) > 60. )
                break;
        }
    }
    catch ( SDLException & except )
    {
        cout << except.Description( ) << endl;
        ok = false;
    }

    if ( ok )
        cout << "Input PASSED." << endl << endl;
    else
        cout << "Input FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL

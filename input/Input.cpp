/*
  Input.cpp
  Copyright (C) 2009 David M. Anderson

  Input class: Manager of the input devices.
*/


#include "Input.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Gamepad.hpp"
#include "Wiimote.hpp"
#ifdef USE_SDL
#include "SDL.hpp"
#include "SDLException.hpp"
#include <SDL.h>
#endif
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Timer.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputImpl
{
public:
    InputImpl( );
    ~InputImpl( );

    void Init( );
    void Shutdown( );

    void Update( );
    const shared_ptr< InputEvent > CheckEvent( );

    int NumDevices( ) const;
    const shared_ptr< InputDevice > Device( int index ) const;
    int DeviceIndex( InputDevice::Type type ) const;

    void SetTextInput( bool on );

private:
#ifdef USE_SDL
    //Under SDL there are exactly one keyboard and one mouse, which is true for
    // modern PCs, but not for many game consoles, phones, etc.
    shared_ptr< Keyboard >              m_pKeyboard;
    shared_ptr< Mouse >                 m_pMouse;
    vector< shared_ptr< Gamepad > >     m_joysticks;
    vector< shared_ptr< Wiimote > >     m_wiimotes;
    bool                                m_textInput;
#endif //USE_SDL
};


//*****************************************************************************


Input::Input( )
    :   m_quitHandler( 0 ),
        m_ignoreTypes( 0 ),
        m_pImpl( new InputImpl )
{
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
    m_pImpl->Init( );
}

//-----------------------------------------------------------------------------

void
Input::Shutdown( )
{
    m_pImpl->Shutdown( );
}

//=============================================================================

void 
Input::Update( )
{
    m_pImpl->Update( );
}

//=============================================================================

const shared_ptr< InputEvent > 
Input::CheckEvent( )
{
    return m_pImpl->CheckEvent( );
}

//=============================================================================

int 
Input::NumDevices( ) const
{
    return m_pImpl->NumDevices( );
}

//-----------------------------------------------------------------------------

const shared_ptr< InputDevice >
Input::Device( int index ) const
{
    return m_pImpl->Device( index );
}

//-----------------------------------------------------------------------------

int 
Input::DeviceIndex( InputDevice::Type type ) const
{
    return m_pImpl->DeviceIndex( type );
}

//=============================================================================

void 
Input::HandleQuit( )
{
    if ( m_quitHandler )
        m_quitHandler( );
    else
        exit( 0 );
}

//=============================================================================

uint32_t 
Input::IgnoreTypes( ) const
{
    return m_ignoreTypes;
}

//-----------------------------------------------------------------------------

void 
Input::SetTextInput( bool on )
{
    m_pImpl->SetTextInput( on );
}

//=============================================================================

#ifdef DEBUG

//-----------------------------------------------------------------------------

namespace
{                                                                   //namespace

string DeviceTypeName( InputDevice::Type type );

}                                                                   //namespace

//-----------------------------------------------------------------------------

bool 
Input::Test( )
{
    bool ok = true;
    cout << "Testing Input" << endl;

    Input & input = Input::Instance();

    try
    {
#if defined(USE_CWIID)
        cout << "To use a Wiimote, hold buttons 1 & 2 down." << endl;
#endif
        cout << "Init()" << endl;
        input.Init( );

        cout << "Number of devices: " << input.NumDevices() << endl;
        for ( int i = 0; i < input.NumDevices(); ++i )
        {
            const shared_ptr< InputDevice > pDev =  input.Device( i );
            Assert( pDev != 0 );
            cout << i << ")" << " Type=" << DeviceTypeName( pDev->GetType() )
                 << " Name=\"" << pDev->Name() << "\""
                 << " NumButtons=" << pDev->NumButtons()
                 << " NumPointers=" << pDev->NumPointers()
                 << " NumAxes=" << pDev->NumAxes()
                 << " NumAccels=" << pDev->NumAccelerometers()
                 << endl;
        }
        cout << endl;

        Timer realTime;
        while ( true )
        {
            input.Update( );
            const shared_ptr< InputEvent > pEvent = input.CheckEvent( );
            if ( pEvent )
            {
                const shared_ptr< InputDevice > pDev = pEvent->Device();
                int button = pEvent->Button();
                cout << "Button=" << button
                     << "  Device: Type=" << DeviceTypeName( pDev->GetType() )
                     << " Name=\"" << pDev->Name() << "\"" << endl;
                cout << "  Buttons down: ";
                for ( int i = 0; i < pDev->NumButtons(); ++i )
                    if ( pDev->ButtonDown( i ) )
                        cout << i << " ";
                cout << endl;
                if ( pDev->NumPointers() > 0 )
                {
                    cout << "  Pointers: ";
                    for ( int i = 0; i < pDev->NumPointers(); ++i )
                        cout << i << ": " << pDev->Pointer( i ) << "   ";
                    cout << endl;
                }
                if ( pDev->NumAxes() > 0 )
                {
                    cout << "  Axes: ";
                    for ( int i = 0; i < pDev->NumAxes(); ++i )
                        cout << i << ": " << pDev->Axis( i ) << "   ";
                    cout << endl;
                }
                if ( pDev->NumAccelerometers() > 0 )
                {
                    cout << "  Accelerometers: ";
                    for ( int i = 0; i < pDev->NumAccelerometers(); ++i )
                        cout << i << ": " << "Accel=" << pDev->Acceleration( i )
                             << " Gravity=" << pDev->Gravity( i ) << "   ";
                    cout << endl;
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

//-----------------------------------------------------------------------------

namespace
{                                                                   //namespace

string 
DeviceTypeName( InputDevice::Type type )
{
    switch ( type )
    {
    case InputDevice::Keyboard:
        return "Keyboard";
    case InputDevice::Mouse:
        return "Mouse";
    case InputDevice::Tablet:
        return "Tablet";
    case InputDevice::Gamepad:
        return "Gamepad";
    case InputDevice::Wiimote:
        return "Wiimote";
    }
}

}                                                                   //namespace

//-----------------------------------------------------------------------------

#endif //DEBUG


//*****************************************************************************


#ifdef USE_SDL

//=============================================================================


InputImpl::InputImpl( )
{
    SDL::Instance();    //to force construction
}

//-----------------------------------------------------------------------------

InputImpl::~InputImpl( )
{
}

//=============================================================================

void 
InputImpl::Init( )
{
    uint32_t ignoreTypes = Input::Instance().IgnoreTypes();

#ifdef SUPPORT_WIIMOTE
    Wiimote::FindAll( &m_wiimotes );
#endif

    SDL::Instance().Init( );
    Assert( ::SDL_WasInit( SDL_INIT_VIDEO ) != 0 );

    if ( (ignoreTypes & InputDevice::Keyboard) == 0 )
    {
        m_pKeyboard = shared_ptr< Keyboard >( new Keyboard( "Keyboard" ) );
    }
    if ( (ignoreTypes & InputDevice::Mouse) == 0 )
    {
        m_pMouse = shared_ptr< Mouse >( new Mouse( "Mouse" ) );
    }
    if ( (ignoreTypes & InputDevice::Gamepad) == 0 )
    {
        if ( ::SDL_WasInit( SDL_INIT_JOYSTICK ) == 0 )
        {
            int initRslt = ::SDL_InitSubSystem( SDL_INIT_JOYSTICK );

            if ( initRslt != 0 )
                throw SDLException( "SDL_InitSubSystem( SDL_INIT_JOYSTICK )" );
        }
        int joystickEventState = ::SDL_JoystickEventState( SDL_ENABLE );
        if ( joystickEventState != SDL_ENABLE )
            throw SDLException( "SDL_JoystickEventState" );

        for ( int i = 0; i < ::SDL_NumJoysticks( ); ++i )
        {
            
            shared_ptr< Gamepad > pJoy(
                new Gamepad( ::SDL_JoystickName( i ), i ) );
            m_joysticks.push_back( pJoy );
        }
    }
}

//-----------------------------------------------------------------------------

void 
InputImpl::Shutdown( )
{
    m_joysticks.clear();
    m_pMouse.reset();
    m_pKeyboard.reset();
    if ( Input::Instance().IgnoreTypes() & InputDevice::Gamepad == 0 )
        if ( ::SDL_WasInit( SDL_INIT_JOYSTICK ) != 0 )
            ::SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
}

//=============================================================================

void 
InputImpl::Update( )
{
    ::SDL_PumpEvents( );
#ifdef SUPPORT_WIIMOTE
    for ( int w = 0; w < (int)m_wiimotes.size(); ++w )
        m_wiimotes[ w ]->Update( );
#endif
}

//=============================================================================

const shared_ptr< InputEvent > 
InputImpl::CheckEvent( )
{
    ::SDL_Event sdlEvent;
    int pollRslt = ::SDL_PollEvent( &sdlEvent );
    if ( pollRslt != 0 )
    {
        switch( sdlEvent.type )
        {
        case SDL_KEYDOWN:
        {
            shared_ptr< Keyboard > pDevice = m_pKeyboard;
            int key = 0;
            if ( m_textInput )
                key = sdlEvent.key.keysym.unicode;
            else
                key = sdlEvent.key.keysym.sym;
            return shared_ptr< InputEvent >( new InputEvent( pDevice, key ) );
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            shared_ptr< Mouse > pDevice = m_pMouse;
            int button = sdlEvent.button.button;
            return shared_ptr< InputEvent >(
                new InputEvent( pDevice, button ) );
        }
        case SDL_JOYBUTTONDOWN:
        {
            Assert( sdlEvent.jbutton.which < m_joysticks.size() );
            shared_ptr< Gamepad > pDevice
                    = m_joysticks[ sdlEvent.jbutton.which ];
            int key = sdlEvent.jbutton.button;
            return shared_ptr< InputEvent >( new InputEvent( pDevice, key ) );
        }
        case SDL_QUIT:
        {
            Input::Instance().HandleQuit( );
            return shared_ptr< InputEvent >( );
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
            int button = pWiimote->GetButtonPressed( );
            return shared_ptr< InputEvent >(
                new InputEvent( pWiimote, button ) );
        }
    }
#endif
    return shared_ptr< InputEvent >( );
}

//=============================================================================

int 
InputImpl::NumDevices( ) const
{
    return 2 + m_joysticks.size() + m_wiimotes.size();
}

//-----------------------------------------------------------------------------

const shared_ptr< InputDevice >
InputImpl::Device( int index ) const
{
    if ( index < 0 )
        throw std::out_of_range( "Input::Device()" );
    if ( index == 0 )
        return m_pKeyboard;
    if ( index == 1 )
        return m_pMouse;
    index -= 2;
    if ( index < m_joysticks.size() )
        return m_joysticks[ index ];
    index -= m_joysticks.size();
    if ( index < m_wiimotes.size() )
        return m_wiimotes[ index ];
    throw std::out_of_range( "Input::Device()" );
}

//-----------------------------------------------------------------------------

int 
InputImpl::DeviceIndex( InputDevice::Type type ) const
{
    if ( type == InputDevice::Keyboard )
        return 0;
    if ( type == InputDevice::Mouse )
        return 1;
    if ( type == InputDevice::Gamepad )
        if ( m_joysticks.size() > 0 )
            return 2;
    if ( type == InputDevice::Wiimote )
        if ( m_wiimotes.size() > 0 )
            return (2 + m_joysticks.size());
    return -1;
}

//=============================================================================

void 
InputImpl::SetTextInput( bool on )
{
    m_textInput = on;
    ::SDL_EnableUNICODE( on  ?  1  :  0 );
    int repeatDelay = ( on  ?  SDL_DEFAULT_REPEAT_DELAY  :  0 );
    int repeatInterval = ( on  ?  SDL_DEFAULT_REPEAT_INTERVAL  :  0 );
    ::SDL_EnableKeyRepeat( repeatDelay, repeatInterval );
}


//=============================================================================

#endif //USE_SDL


//*****************************************************************************

}                                                      //namespace EpsilonDelta


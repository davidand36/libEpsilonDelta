/*
  Input.cpp
  Copyright (C) 2009 David M. Anderson

  Input class: Manager of the input devices.
*/


#include "Input.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Gamepad.hpp"
#ifdef SUPPORT_WIIMOTE
#include "Wiimote.hpp"
#endif
#ifdef USE_SDL
#include "SDL.hpp"
#include "SDLException.hpp"
#include <SDL.h>
#endif
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Timer.hpp"
#include "CodePointData.hpp"
#include <iomanip>
#endif
using namespace std;
using namespace std::tr1;


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
    shared_ptr< InputEvent const > CheckEvent( );

    int NumDevices( ) const;
    shared_ptr< InputDevice const > Device( int index ) const;
    int DeviceIndex( InputDevice::EType type ) const;

    void SetTextInput( bool on );

private:
#ifdef USE_SDL
    bool                                m_initialized;
    //Under SDL there are exactly one keyboard and one mouse, which is true for
    // modern PCs, but not for many game consoles, phones, etc.
    shared_ptr< Keyboard >              m_pKeyboard;
    shared_ptr< Mouse >                 m_pMouse;
    vector< shared_ptr< Gamepad > >     m_joysticks;
#ifdef SUPPORT_WIIMOTE
    vector< shared_ptr< Wiimote > >     m_wiimotes;
#endif
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

shared_ptr< InputEvent const > 
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

shared_ptr< InputDevice const >
Input::Device( int index ) const
{
    return m_pImpl->Device( index );
}

//-----------------------------------------------------------------------------

int 
Input::DeviceIndex( InputDevice::EType type ) const
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

string DeviceTypeName( InputDevice::EType type );

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
        cout << "Index\tType\t\tButtons\tPnters\tAxes\tAccels\tName" << endl;
        for ( int i = 0; i < input.NumDevices(); ++i )
        {
            shared_ptr< InputDevice const > pDev =  input.Device( i );
            Assert( pDev != 0 );
            cout << i << "\t" << DeviceTypeName( pDev->Type() ) << "   "
                 << "\t" << pDev->NumButtons() << "\t" << pDev->NumPointers()
                 << "\t" << pDev->NumAxes() << "\t" << pDev->NumAccelerometers()
                 << "\t" << pDev->Name() << endl;
        }
        cout << endl;

        cout << "Shutdown()" << endl;
        input.Shutdown( );

        cout << "Init()" << endl;
        input.Init( );
        for ( int i = 0; i < input.NumDevices(); ++i )
        {
            shared_ptr< InputDevice const > pDev =  input.Device( i );
            Assert( pDev != 0 );
        }
        cout << "Init() again" << endl;
        input.Init( );
        cout << endl;

        cout << "Number of devices: " << input.NumDevices() << endl;
        cout << "Index\tType\t\tButtons\tPnters\tAxes\tAccels\tName" << endl;
        for ( int i = 0; i < input.NumDevices(); ++i )
        {
            shared_ptr< InputDevice const > pDev =  input.Device( i );
            Assert( pDev != 0 );
            cout << i << "\t" << DeviceTypeName( pDev->Type() ) << "   "
                 << "\t" << pDev->NumButtons() << "\t" << pDev->NumPointers()
                 << "\t" << pDev->NumAxes() << "\t" << pDev->NumAccelerometers()
                 << "\t" << pDev->Name() << endl;
        }
        cout << endl;

        cout << "Ready to report events." << endl;
        Timer realTime;
        while ( true )
        {
            input.Update( );
            shared_ptr< InputEvent const > pEvent = input.CheckEvent( );
            if ( pEvent )
            {
                shared_ptr< InputDevice const > pDev = pEvent->Device();
                int button = pEvent->Button();
                cout << "Button=" << hex << setw(2) << setfill('0') << button
                     << dec << setw(0) << setfill(' ');
                if ( (pDev->Type() == InputDevice::Keyboard)
                     && (button <= 0xFF) && IsPrint( (char)button ) )
                    cout << " (" << (char)button << ")";
                cout << "  Device: Type=" << DeviceTypeName( pDev->Type() )
                     << " Name=\"" << pDev->Name() << "\"" << endl;
                cout << "  Buttons down: ";
                for ( int i = 0; i < pDev->NumButtons(); ++i )
                    if ( pDev->ButtonDown( i ) )
                        cout << hex << setw(2) << setfill('0') << i
                             << dec << setw(0) << setfill(' ') << " ";
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
            {
                cout << "Time's up" << endl;
                break;
            }
        }

        realTime.Reset();
        cout << "SetTextInput( true )" << endl;
        input.SetTextInput( true );
        while ( true )
        {
            input.Update( );
            shared_ptr< InputEvent const > pEvent = input.CheckEvent( );
            if ( pEvent )
            {
                if ( pEvent->Device()->Type() == InputDevice::Keyboard )
                {
                    int button = pEvent->Button();
                    if ( (button < MaximumCodePoint)
                         && IsPrint( (wchar_t)button ) )
                        wcout << (wchar_t)button << flush;
                    else if ( button > 0 )
                        cout << "[" << hex << setw(2) << setfill('0') << button
                             << dec << setw(0) << setfill(' ') << "]" << flush;
                }
            }
            if ( realTime.Seconds( ) > 60. )
            {
                cout << endl << "Time's up" << endl;
                break;
            }
        }
        wcout << endl;
    }
    catch ( Exception & except )
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
DeviceTypeName( InputDevice::EType type )
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
    default:
        Assert( 0 && "Unexpected input device type" );
        return "Undefined";
    }
}

}                                                                   //namespace

//-----------------------------------------------------------------------------

#endif //DEBUG


//*****************************************************************************


#ifdef USE_SDL

//=============================================================================


InputImpl::InputImpl( )
    :   m_initialized( false ),
        m_textInput( false )
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
    if ( m_initialized )
        return;
    m_initialized = true;

    uint32_t ignoreTypes = Input::Instance().IgnoreTypes();

#ifdef SUPPORT_WIIMOTE
    if ( (ignoreTypes & InputDevice::Wiimote) == 0 )
    {
        Wiimote::FindAll( &m_wiimotes );
    }
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
#ifdef SUPPORT_WIIMOTE
    m_wiimotes.clear();
#endif
    m_joysticks.clear();
    m_pMouse.reset();
    m_pKeyboard.reset();
    if ( (Input::Instance().IgnoreTypes() & InputDevice::Gamepad) == 0 )
        if ( ::SDL_WasInit( SDL_INIT_JOYSTICK ) != 0 )
            ::SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
    m_initialized = false;
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

shared_ptr< InputEvent const > 
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
            shared_ptr< Keyboard const > pDevice = m_pKeyboard;
            int key = 0;
            if ( m_textInput )
                key = sdlEvent.key.keysym.unicode;
            else
                key = sdlEvent.key.keysym.sym;
            return shared_ptr< InputEvent const >(
                new InputEvent( pDevice, key ) );
        }
        case SDL_MOUSEBUTTONDOWN:
        {
            shared_ptr< Mouse const > pDevice = m_pMouse;
            //SDL's mouse button definitions start at 1, but are in the
            // same order as ours, which start at 0.
            int button = sdlEvent.button.button - 1;
            return shared_ptr< InputEvent const >(
                new InputEvent( pDevice, button ) );
        }
        case SDL_JOYBUTTONDOWN:
        {
            Assert( sdlEvent.jbutton.which < m_joysticks.size() );
            shared_ptr< Gamepad const > pDevice
                    = m_joysticks[ sdlEvent.jbutton.which ];
            int key = sdlEvent.jbutton.button;
            return shared_ptr< InputEvent const >(
                new InputEvent( pDevice, key ) );
        }
        case SDL_QUIT:
        {
            Input::Instance().HandleQuit( );
            return shared_ptr< InputEvent const >( );
        }
        default:
            break;
        }
    }
#ifdef SUPPORT_WIIMOTE
    for ( int w = 0; w < (int)m_wiimotes.size(); ++w )
    {
        shared_ptr< Wiimote > pWiimote = m_wiimotes[ w ];
        if ( pWiimote->WasButtonPressed() )
        {
            int button = pWiimote->GetButtonPressed( );
            return shared_ptr< InputEvent const >(
                new InputEvent( pWiimote, button ) );
        }
    }
#endif
    return shared_ptr< InputEvent const >( );
}

//=============================================================================

int 
InputImpl::NumDevices( ) const
{
    return 2 + m_joysticks.size()
#ifdef SUPPORT_WIIMOTE
            + m_wiimotes.size()
#endif
            ;
}

//-----------------------------------------------------------------------------

shared_ptr< InputDevice const >
InputImpl::Device( int index ) const
{
    if ( index < 0 )
        throw std::out_of_range( "Input::Device()" );
    if ( index == 0 )
        return m_pKeyboard;
    if ( index == 1 )
        return m_pMouse;
    index -= 2;
    if ( index < (int)m_joysticks.size() )
        return m_joysticks[ index ];
    index -= m_joysticks.size();
#ifdef SUPPORT_WIIMOTE
    if ( index < (int)m_wiimotes.size() )
        return m_wiimotes[ index ];
#endif
    throw std::out_of_range( "Input::Device()" );
}

//-----------------------------------------------------------------------------

int 
InputImpl::DeviceIndex( InputDevice::EType type ) const
{
    if ( type == InputDevice::Keyboard )
        return 0;
    if ( type == InputDevice::Mouse )
        return 1;
    if ( type == InputDevice::Gamepad )
        if ( m_joysticks.size() > 0 )
            return 2;
#ifdef SUPPORT_WIIMOTE
    if ( type == InputDevice::Wiimote )
        if ( m_wiimotes.size() > 0 )
            return (2 + m_joysticks.size());
#endif
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


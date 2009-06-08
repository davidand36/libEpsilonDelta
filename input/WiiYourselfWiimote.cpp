/*
  WiiYourselfWiimote.cpp
  Copyright (C) 2007 David M. Anderson

  Wiimote class: the Wii remote input device.
  This implementation uses WiiYourself!, a Windows library.
*/


#ifdef SUPPORT_WIIMOTE
#ifdef USE_WIIYOURSELF


#include "Wiimote.hpp"
#include "Exception.hpp"
#include <cstdio>
#include <string>
#include <iostream>
#include <wiimote.h>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class WiimoteImpl
{
public:
    WiimoteImpl( int index );
    virtual ~WiimoteImpl( );

    bool IsConnected( ) const;
    virtual std::string Name( ) const;
    void Update( WiimoteState * pState,
                 std::queue< WiimoteState::Button > * pPressedQueue );

private:
    void Connect( );

    int        m_index;
    ::wiimote  m_wyMote;
    Vector2I   m_pointerSeparation;
};


//*****************************************************************************


Wiimote::Wiimote( int index )
    :   m_pImpl( new WiimoteImpl( index ) )
{
}

//-----------------------------------------------------------------------------

Wiimote::~Wiimote( )
{
    delete m_pImpl;
}

//=============================================================================

bool 
Wiimote::IsConnected( ) const
{
    return m_pImpl->IsConnected( );
}

//=============================================================================

std::string 
Wiimote::Name( ) const
{
    return m_pImpl->Name();
}

//=============================================================================

void 
Wiimote::Update( )
{
    return m_pImpl->Update( &m_state, &m_pressedQueue );
}


//*****************************************************************************


namespace
{                                                                   //namespace

const float pointerMeterSeparation = 250.f;

}                                                                   //namespace


//*****************************************************************************


WiimoteImpl::WiimoteImpl( int index )
    :   m_index( index )
{
    Connect( );
}

//-----------------------------------------------------------------------------

WiimoteImpl::~WiimoteImpl( )
{
    if ( m_wyMote.IsConnected() )
        m_wyMote.Disconnect( );
}

//=============================================================================

void 
WiimoteImpl::Connect( )
{
    m_wyMote.Connect( ::wiimote::FIRST_AVAILABLE );
    if ( m_wyMote.IsConnected( ) )
    {
        m_wyMote.SetReportType( wiimote::IN_BUTTONS_ACCEL_IR_EXT );
        m_pointerSeparation.Set( 300, 0 );    //!!!
    }
}


//=============================================================================

bool 
WiimoteImpl::IsConnected( ) const
{
    return m_wyMote.IsConnected();
}

//=============================================================================

std::string
WiimoteImpl::Name( ) const
{
    char name[ 50 ];
    std::sprintf( name, "Wii remote %d", m_index );
    return name;
}

//=============================================================================

void
WiimoteImpl::Update( WiimoteState * pState,
                     std::queue< WiimoteState::Button > * pPressedQueue )
{
    WiimoteState prevState = *pState;

    if ( m_wyMote.RefreshState() == ::NO_CHANGE )
        return;
    if ( (! m_wyMote.IsConnected()) || m_wyMote.ConnectionLost() )
    {
        Connect( );
        if ( ! m_wyMote.IsConnected() )
            return;
    }

    uint32_t buttons = 0;
    static const uint16_t wyButtons[]
            = { ::wiimote_state::buttons::_A, ::wiimote_state::buttons::_B,
                ::wiimote_state::buttons::UP, ::wiimote_state::buttons::DOWN,
                ::wiimote_state::buttons::LEFT, ::wiimote_state::buttons::RIGHT,
                ::wiimote_state::buttons::MINUS, ::wiimote_state::buttons::PLUS,
                ::wiimote_state::buttons::HOME, ::wiimote_state::buttons::ONE,
                ::wiimote_state::buttons::TWO };
    for ( int i = WiimoteState::A_Button; i <= WiimoteState::Two_Button; ++i )
        if ( (m_wyMote.Button.Bits & wyButtons[ i ]) != 0 )
            buttons |= (1 << i);
    if (  m_wyMote.ExtensionType == ::wiimote_state::NUNCHUK )
    {
        if ( m_wyMote.Nunchuk.C )
            buttons |= (1 << WiimoteState::C_Button);
        if ( m_wyMote.Nunchuk.Z )
            buttons |= (1 << WiimoteState::Z_Button);
    }
    pState->SetButtons( buttons );
    for ( int i = 0; i < WiimoteState::NumButtons; ++i )
    {
        WiimoteState::Button b = WiimoteState::Button( i );
        if ( pState->IsButtonDown( b ) && ! prevState.IsButtonDown( b ) )
            pPressedQueue->push( b );
    }

    Vector3F acceleration( -m_wyMote.Acceleration.X,
                           -m_wyMote.Acceleration.Z,
                           m_wyMote.Acceleration.Y );
    pState->SetAcceleration( acceleration );
    Vector3F gravity( -m_wyMote.Acceleration.Orientation.X,
                      -m_wyMote.Acceleration.Orientation.Z,
                      m_wyMote.Acceleration.Orientation.Y );
    pState->SetGravity( gravity );

    int numPointerLights = 0;
    Vector2I lights[ 2 ];
    if ( m_wyMote.IR.Dot[ 0 ].bVisible )
    {
        ++numPointerLights;
        lights[ 0 ].Set( m_wyMote.IR.Dot[ 0 ].RawX,
                         m_wyMote.IR.Dot[ 0 ].RawY );
        if ( m_wyMote.IR.Dot[ 1 ].bVisible )
        {
            ++numPointerLights;
            lights[ 1 ].Set( m_wyMote.IR.Dot[ 1 ].RawX,
                             m_wyMote.IR.Dot[ 1 ].RawY );
            m_pointerSeparation = lights[ 1 ] - lights[ 0 ];
        }
    }
    else if ( m_wyMote.IR.Dot[ 1 ].bVisible )
    {
        ++numPointerLights;
        lights[ 1 ].Set( m_wyMote.IR.Dot[ 1 ].RawX,
                         m_wyMote.IR.Dot[ 1 ].RawY );
        lights[ 0 ] = lights[ 1 ] - m_pointerSeparation;  //best guess
    }
    if ( numPointerLights == 0 )
    {
        pState->SetPointer( 0 );
    }
    else
    {
        Vector2I halfSep( m_pointerSeparation.X() / 2,
                          m_pointerSeparation.Y() / 2 );
        Vector2I rawMiddle = lights[ 0 ] + halfSep;
        float midX = 1.f -
                ((float) rawMiddle.X() / ::wiimote_state::ir::MAX_RAW_X);
        midX = max( 0.f, min( 1.f, midX ) );
        float midY = (float) rawMiddle.Y() / ::wiimote_state::ir::MAX_RAW_Y;
        midY = max( 0.f, min( 1.f, midY ) );
        Vector2F scaledMiddle( midX, midY );
        if ( numPointerLights == 1 )
        {
            pState->SetPointer( 1, scaledMiddle );
        }
        else //numPointerLights == 2
        {
            Angle angle = ArcTan( m_pointerSeparation.Y(),
                                  m_pointerSeparation.X() );
            Vector2F pointerSeparation( (float)m_pointerSeparation.X(),
                                        (float)m_pointerSeparation.Y() );
            float sepLen = pointerSeparation.Length();
            float distance = pointerMeterSeparation / sepLen;
            pState->SetPointer( 2, scaledMiddle, angle, distance );
        }
    }

    if (  m_wyMote.ExtensionType == ::wiimote_state::NUNCHUK )
    {
        acceleration.Set( -m_wyMote.Nunchuk.Acceleration.X,
                          -m_wyMote.Nunchuk.Acceleration.Z,
                          m_wyMote.Nunchuk.Acceleration.Y );
        gravity.Set( -m_wyMote.Nunchuk.Acceleration.Orientation.X,
                     -m_wyMote.Nunchuk.Acceleration.Orientation.Z,
                     m_wyMote.Nunchuk.Acceleration.Orientation.Y );
        Vector2F joystick( m_wyMote.Nunchuk.Joystick.X,
                           m_wyMote.Nunchuk.Joystick.Y );
        pState->SetNunchuk( true, acceleration, gravity, joystick );
    }
    else
    {
        pState->SetNunchuk( false );
    }
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_WIIYOURSELF
#endif //SUPPORT_WIIMOTE

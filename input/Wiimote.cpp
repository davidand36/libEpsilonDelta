/*
  Wiimote.cpp
  Copyright (C) 2009 David M. Anderson

  Wiimote class: a standard wiimote, a.k.a. joystick.
*/


#include "Wiimote.hpp"
#include "Graphics2D.hpp"
#include "Surface.hpp"
#include <vector>
#ifdef USE_CWIID
#include <cwiid.h>
#include <queue>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace
{                                                                   //namespace
//=============================================================================


class WiimoteState
{
public:
    WiimoteState( );

    enum Button
    {
        A_Button,
        B_Button,
        Up_Button,
        Down_Button,
        Left_Button,
        Right_Button,
        Minus_Button,
        Plus_Button,
        Home_Button,
        One_Button,
        Two_Button,
        //on nunchuk:
        C_Button,
        Z_Button,
        NumButtons
    };

    uint32_t ButtonsDown( ) const;
    bool IsButtonDown( Button button ) const;
    Vector3F Acceleration( ) const;
    Vector3F Gravity( ) const;
    int NumPointerLights( ) const;
    Point2F PointerPos( ) const;
    Angle PointerAngle( ) const;
    float PointerDistance( ) const;
    bool IsNunchukConnected( ) const;
    Vector3F NunchukAcceleration( ) const;
    Vector3F NunchukGravity( ) const;
    Point2F NunchukJoystickPos( ) const;

private:
    void SetButtons( uint32_t buttonsDown );
    void SetAcceleration( const Vector3F & acceleration );
    void SetGravity( const Vector3F & gravity );
    void SetPointer( int numLights, const Point2F & position = Point2F::Zero,
                     Angle angle = 0.f, float distance = 0.f );
    void SetNunchuk( bool connected,
                     const Vector3F & acceleration = -Vector3F::UnitY,
                     const Vector3F & gravity = -Vector3F::UnitY,
                     const Point2F & joystickPos = Point2F::Zero );

    uint32_t    m_buttonsDown;
    Vector3F    m_acceleration;
    Vector3F    m_gravity;

    struct Pointer
    {
        Pointer( );
        int         m_numLights;
        Point2F     m_position;
        Angle       m_angle;
        float       m_distance;
    };
    Pointer     m_pointer;

    struct Nunchuk
    {
        Nunchuk( );
        bool        m_connected;
        Vector3F    m_acceleration;
        Vector3F    m_gravity;
        Point2F     m_joystickPos;
    };
    Nunchuk     m_nunchuk;

    friend class EpsilonDelta::WiimoteImpl;
};


//=============================================================================


const float s_PointerMeterSeparation = 250.f;


//=============================================================================
}                                                                   //namespace


//*****************************************************************************


class WiimoteImpl
{
public:
    WiimoteImpl( );
    ~WiimoteImpl( );

    bool IsConnected( ) const;
    string Name( ) const;

    int NumButtons( ) const;
    bool ButtonDown( int button ) const;

    bool WasButtonPressed( );
    int GetButtonPressed( );

    int NumPointers( ) const;
    Point2I Pointer( int index ) const;

    int NumAxes( ) const;
    double Axis( int index ) const;
    
    int NumAccelerometers( ) const;
    Vector3D Acceleration( int index ) const;
    Vector3D Gravity( int index ) const;

    void Update( );

private:
#ifdef USE_CWIID
    struct CalibrationData
    {
        CalibrationData( );
        bool        m_isCalibrated;
        Vector3I    m_acc0;
        Vector3I    m_acc1;
        bool        m_isNunchukCalibrated;
        Vector3I    m_nkAcc0;
        Vector3I    m_nkAcc1;
        Point2I     m_nkJoyMid;
        Point2I     m_nkJoyRange;
    };

    class GravityEstimator
    {
    public:
        GravityEstimator( );
        Vector3F operator()( const Vector3F & curAccel );

    private:
        Vector3F    m_lastGravity;
        int         m_steadyCount;
    };

    void GetCalibrationData( );

    ::bdaddr_t          m_bdaddr;
    ::cwiid_wiimote_t * m_pCWiiD;
    WiimoteState        m_state;
    queue< int >        m_buttonQueue;
    CalibrationData     m_cal;
    GravityEstimator    m_gravityEstimator;
    GravityEstimator    m_nkGravityEstimator;
    Vector2I            m_pointerSeparation;
#endif
};


//*****************************************************************************


Wiimote::Wiimote( const std::string & name )
    :   InputDevice( InputDevice::Wiimote, name ),
        m_pImpl( new WiimoteImpl( ) )
{
    SetName( m_pImpl->Name() );
}

//-----------------------------------------------------------------------------

Wiimote::~Wiimote( )
{
}

//=============================================================================

bool 
Wiimote::IsConnected( )
{
    return m_pImpl->IsConnected( );
}

//=============================================================================

int 
Wiimote::NumButtons( ) const
{
    return m_pImpl->NumButtons();
}

//-----------------------------------------------------------------------------

bool 
Wiimote::ButtonDown( int button ) const
{
    return m_pImpl->ButtonDown( button );
}

//=============================================================================

bool 
Wiimote::WasButtonPressed( )
{
    return m_pImpl->WasButtonPressed();
}

//-----------------------------------------------------------------------------

int 
Wiimote::GetButtonPressed( )
{
    return m_pImpl->GetButtonPressed();
}

//=============================================================================

int 
Wiimote::NumPointers( ) const
{
    return m_pImpl->NumPointers();
}

//-----------------------------------------------------------------------------

Point2I 
Wiimote::Pointer( int index ) const
{
    return m_pImpl->Pointer( index );
}

//=============================================================================

int 
Wiimote::NumAxes( ) const
{
    return m_pImpl->NumAxes();
}

//-----------------------------------------------------------------------------

double 
Wiimote::Axis( int index ) const
{
    return m_pImpl->Axis( index );
}

//=============================================================================

int 
Wiimote::NumAccelerometers( ) const
{
    return m_pImpl->NumAccelerometers( );
}

//-----------------------------------------------------------------------------

Vector3D 
Wiimote::Acceleration( int index ) const
{
    return m_pImpl->Acceleration( index );
}

//-----------------------------------------------------------------------------

Vector3D 
Wiimote::Gravity( int index ) const
{
    return m_pImpl->Gravity( index );
}

//=============================================================================

void 
Wiimote::FindAll( std::vector< shared_ptr< Wiimote > > * pWiimotes )
{
    pWiimotes->clear( );
    //!!!At most one, for now anyway.
    Wiimote * pWiimote = new Wiimote( "Wiimote" );
    if ( pWiimote->IsConnected() )
        pWiimotes->push_back( shared_ptr< Wiimote >( pWiimote ) );
}

//=============================================================================

void 
Wiimote::Update( )
{
    m_pImpl->Update( );
}


//*****************************************************************************


namespace
{                                                                   //namespace

//=============================================================================

WiimoteState::WiimoteState( )
    :   m_buttonsDown( 0 ),
        m_acceleration( Vector3F::Zero )
{
}

//.............................................................................

WiimoteState::Pointer::Pointer( )
    :   m_numLights( 0 ),
        m_position( Point2F::Zero ),
        m_angle( 0.f ),
        m_distance( 1.f )
{
}

//.............................................................................

WiimoteState::Nunchuk::Nunchuk( )
    :   m_connected( false ),
        m_acceleration( Vector3F::Zero ),
        m_joystickPos( Point2F::Zero )
{
}

//=============================================================================

uint32_t 
WiimoteState::ButtonsDown( ) const
{
    return m_buttonsDown;
}

//-----------------------------------------------------------------------------

bool 
WiimoteState::IsButtonDown( Button button ) const
{
    return (m_buttonsDown & (1 << button)) != 0;
}

//-----------------------------------------------------------------------------

Vector3F
WiimoteState::Acceleration( ) const
{
    return m_acceleration;
}

//-----------------------------------------------------------------------------

Vector3F 
WiimoteState::Gravity( ) const
{
    return m_gravity;
}

//=============================================================================

int
WiimoteState::NumPointerLights( ) const
{
    return m_pointer.m_numLights;
}

//-----------------------------------------------------------------------------

Point2F 
WiimoteState::PointerPos( ) const
{
    return m_pointer.m_position;
}

//-----------------------------------------------------------------------------

Angle 
WiimoteState::PointerAngle( ) const
{
    return m_pointer.m_angle;
}

//-----------------------------------------------------------------------------

float 
WiimoteState::PointerDistance( ) const
{
    return m_pointer.m_distance;
}

//=============================================================================

bool 
WiimoteState::IsNunchukConnected( ) const
{
    return m_nunchuk.m_connected;
}

//-----------------------------------------------------------------------------

Vector3F 
WiimoteState::NunchukAcceleration( ) const
{
    return m_nunchuk.m_acceleration;
}

//-----------------------------------------------------------------------------

Vector3F 
WiimoteState::NunchukGravity( ) const
{
    return m_nunchuk.m_gravity;
}

//-----------------------------------------------------------------------------

Point2F 
WiimoteState::NunchukJoystickPos( ) const
{
    return m_nunchuk.m_joystickPos;
}

//=============================================================================

void 
WiimoteState::SetButtons( uint32_t buttonsDown )
{
    m_buttonsDown = buttonsDown;
}

//-----------------------------------------------------------------------------

void 
WiimoteState::SetAcceleration( const Vector3F & acceleration )
{
    m_acceleration = acceleration;
}

//-----------------------------------------------------------------------------

void 
WiimoteState::SetGravity( const Vector3F & gravity )
{
    m_gravity = gravity;
}

//-----------------------------------------------------------------------------

void 
WiimoteState::SetPointer( int numLights, const Point2F & position,
                          Angle angle, float distance )
{
    switch ( numLights )
    {
    case 2:
        m_pointer.m_angle = angle;
        m_pointer.m_distance = distance;
        //fall through
    case 1:
        m_pointer.m_position = position;
        //fall through
    case 0:
        m_pointer.m_numLights = numLights;
        break;
    default:
        Assert( 0 && "Unexpected number of Wiimote pointer lights" );
        break;
    }
}

//-----------------------------------------------------------------------------

void 
WiimoteState::SetNunchuk( bool connected,
                          const Vector3F & acceleration,
                          const Vector3F & gravity,
                          const Point2F & joystickPos )
{
    m_nunchuk.m_connected = connected;
    if ( connected )
    {
        m_nunchuk.m_acceleration = acceleration;
        m_nunchuk.m_gravity = gravity;
        m_nunchuk.m_joystickPos = joystickPos;
    }
}

//=============================================================================

}                                                                   //namespace


//*****************************************************************************


#ifdef USE_CWIID

//*****************************************************************************


WiimoteImpl::WiimoteImpl( )
    :   m_bdaddr( *BDADDR_ANY ),
        m_pCWiiD( 0 )
{
    m_pCWiiD = ::cwiid_open( &m_bdaddr, 0 );
    if ( m_pCWiiD != 0 )
    {
        uint8_t reportMode = CWIID_RPT_STATUS | CWIID_RPT_BTN | CWIID_RPT_ACC |
                CWIID_RPT_IR | CWIID_RPT_NUNCHUK;
        int rptRslt = ::cwiid_set_rpt_mode( m_pCWiiD, reportMode );
        Assert( rptRslt == 0 ); //!!!
        GetCalibrationData( );
        m_pointerSeparation.Set( 300, 0 );    //!!!
    }
}

//-----------------------------------------------------------------------------

WiimoteImpl::~WiimoteImpl( )
{
    if ( m_pCWiiD != 0 )
        ::cwiid_close( m_pCWiiD );
}

//=============================================================================

bool 
WiimoteImpl::IsConnected( ) const
{
    return (m_pCWiiD != 0);
}

//-----------------------------------------------------------------------------

string 
WiimoteImpl::Name( ) const
{
    if ( m_pCWiiD != 0 )
        return "Wii remote " + IntToString( ::cwiid_get_id( m_pCWiiD ) );
    else
        return "";
}

//=============================================================================

int 
WiimoteImpl::NumButtons( ) const
{
    int numButtons = 11;    //on main controller
    if ( m_state.IsNunchukConnected() )
        numButtons += 2;
    return numButtons;
}

//-----------------------------------------------------------------------------

bool 
WiimoteImpl::ButtonDown( int button ) const
{
    return m_state.IsButtonDown( (WiimoteState::Button)button );
}

//=============================================================================

bool 
WiimoteImpl::WasButtonPressed( )
{
    return ! m_buttonQueue.empty();
}

//-----------------------------------------------------------------------------

int 
WiimoteImpl::GetButtonPressed( )
{
    Assert( ! m_buttonQueue.empty() );
    int button = m_buttonQueue.front();
    m_buttonQueue.pop( );
    return button;
}

//=============================================================================

int 
WiimoteImpl::NumPointers( ) const
{
    return 1;
}

//-----------------------------------------------------------------------------

Point2I 
WiimoteImpl::Pointer( int index ) const
{
    Point2F propPos = m_state.PointerPos();
    //Convert to screen coordinates
    Rectangle screenRect = Graphics2D::Instance().Screen()->Extent();
    int w = screenRect.Width();
    int h = screenRect.Height();
    int x = (int)( (1.f - propPos.X()) * w );
    int y = (int)( propPos.Y() * h );
    return Point2I( x, y );
}

//=============================================================================

int 
WiimoteImpl::NumAxes( ) const
{
    int numAxes = 0;
    if ( m_state.IsNunchukConnected() )
        numAxes += 2;
    return numAxes;
}

//-----------------------------------------------------------------------------

double 
WiimoteImpl::Axis( int index ) const
{
    if ( (index >= 0) && (index < 2) )
        return m_state.NunchukJoystickPos().At( index );
    else
        throw std::out_of_range( "Wiimote::Axis()" );
}

//=============================================================================

int 
WiimoteImpl::NumAccelerometers( ) const
{
    int numAcc = 1;
    if ( m_state.IsNunchukConnected() )
        numAcc += 1;
    return numAcc;
}

//-----------------------------------------------------------------------------

Vector3D 
WiimoteImpl::Acceleration( int index ) const
{
    Vector3F accF;
    if ( index == 0 )
        accF = m_state.Acceleration();
    else if ( (index == 1) && m_state.IsNunchukConnected() )
        accF = m_state.NunchukAcceleration();
    else
        throw std::out_of_range( "Wiimote::Acceleration()" );
    return Vector3D( accF.X(), accF.Y(), accF.Z() );
}

//-----------------------------------------------------------------------------

Vector3D 
WiimoteImpl::Gravity( int index ) const
{
    Vector3F gravF;
    if ( index == 0 )
        gravF = m_state.Gravity();
    else if ( (index == 1) && m_state.IsNunchukConnected() )
        gravF = m_state.NunchukGravity();
    else
        throw std::out_of_range( "Wiimote::Gravity()" );
    return Vector3D( gravF.X(), gravF.Y(), gravF.Z() );
}

//=============================================================================

void
WiimoteImpl::Update( )
{
    Assert( m_pCWiiD );

    GetCalibrationData( );

    WiimoteState prevState = m_state;

    struct ::cwiid_state cwiidState;
    int stateRslt = ::cwiid_get_state( m_pCWiiD, &cwiidState );
    Assert( stateRslt == 0 ); //!!!

    uint32_t buttons = 0;
    static const uint16_t cwiidButtons[]
            = { CWIID_BTN_A, CWIID_BTN_B, CWIID_BTN_UP, CWIID_BTN_DOWN,
                CWIID_BTN_LEFT, CWIID_BTN_RIGHT, CWIID_BTN_MINUS,
                CWIID_BTN_PLUS, CWIID_BTN_HOME, CWIID_BTN_1, CWIID_BTN_2,
                CWIID_NUNCHUK_BTN_C, CWIID_NUNCHUK_BTN_Z };
    for ( int i = WiimoteState::A_Button; i <= WiimoteState::Two_Button; ++i )
        if ( (cwiidState.buttons & cwiidButtons[ i ]) != 0 )
            buttons |= (1 << i);
    if ( cwiidState.ext_type == ::CWIID_EXT_NUNCHUK )
        for ( int i = WiimoteState::C_Button; i <= WiimoteState::Z_Button; ++i )
            if ( (cwiidState.ext.nunchuk.buttons & cwiidButtons[ i ]) != 0 )
                buttons |= (1 << i);
    m_state.SetButtons( buttons );
    for ( int i = 0; i < WiimoteState::NumButtons; ++i )
    {
        WiimoteState::Button b = WiimoteState::Button( i );
        if ( m_state.IsButtonDown( b ) && ! prevState.IsButtonDown( b ) )
            m_buttonQueue.push( b );
    }


    Vector3F acceleration(
        -((float) cwiidState.acc[ 0 ] - m_cal.m_acc0[ 0 ]) / m_cal.m_acc1[ 0 ],
        -((float) cwiidState.acc[ 2 ] - m_cal.m_acc0[ 2 ]) / m_cal.m_acc1[ 2 ],
        ((float) cwiidState.acc[ 1 ] - m_cal.m_acc0[ 1 ]) / m_cal.m_acc1[ 1 ] );
    m_state.SetAcceleration( acceleration );
    Vector3F gravity = m_gravityEstimator( acceleration );
    m_state.SetGravity( gravity );

    int numPointerLights = 0;
    Point2I lights[ 2 ];
    if ( cwiidState.ir_src[ 0 ].valid )
    {
        ++numPointerLights;
        lights[ 0 ].Set( cwiidState.ir_src[ 0 ].pos[ 0 ],
                         cwiidState.ir_src[ 0 ].pos[ 1 ] );
        if ( cwiidState.ir_src[ 1 ].valid )
        {
            ++numPointerLights;
            lights[ 1 ].Set( cwiidState.ir_src[ 1 ].pos[ 0 ],
                             cwiidState.ir_src[ 1 ].pos[ 1 ] );
            m_pointerSeparation = lights[ 1 ] - lights[ 0 ];
        }
    }
    else if ( cwiidState.ir_src[ 1 ].valid )
    {
        ++numPointerLights;
        lights[ 1 ].Set( cwiidState.ir_src[ 1 ].pos[ 0 ],
                         cwiidState.ir_src[ 1 ].pos[ 1 ] );
        lights[ 0 ] = lights[ 1 ] - m_pointerSeparation;  //best guess
    }
    if ( numPointerLights == 0 )
    {
        m_state.SetPointer( 0 );
    }
    else
    {
        Vector2I halfSep( m_pointerSeparation.X() / 2,
                          m_pointerSeparation.Y() / 2 );
        Point2I rawMiddle = lights[ 0 ] + halfSep;
        Point2F scaledMiddle( (float) rawMiddle.X() / CWIID_IR_X_MAX,
                               (float) rawMiddle.Y() / CWIID_IR_Y_MAX );
        if ( numPointerLights == 1 )
        {
            m_state.SetPointer( 1, scaledMiddle );
        }
        else //numPointerLights == 2
        {
            Angle angle = ArcTan( m_pointerSeparation.Y(),
                                  m_pointerSeparation.X() );
            Vector2F pointerSeparation( m_pointerSeparation.X(),
                                        m_pointerSeparation.Y() );
            float sepLen = pointerSeparation.Length();
            float distance = s_PointerMeterSeparation / sepLen;
            m_state.SetPointer( 2, scaledMiddle, angle, distance );
        }
    }

    if ( cwiidState.ext_type == ::CWIID_EXT_NUNCHUK )
    {
        acceleration.Set(
            -((float) cwiidState.ext.nunchuk.acc[ 0 ] - m_cal.m_nkAcc0[ 0 ])
            / m_cal.m_nkAcc1[ 0 ],
            -((float) cwiidState.ext.nunchuk.acc[ 2 ] - m_cal.m_nkAcc0[ 2 ])
            / m_cal.m_nkAcc1[ 2 ],
            ((float) cwiidState.ext.nunchuk.acc[ 1 ] - m_cal.m_nkAcc0[ 1 ])
            / m_cal.m_nkAcc1[ 1 ] );
        gravity = m_nkGravityEstimator( acceleration );
        Point2F joystick(
            ((float) cwiidState.ext.nunchuk.stick[ 0 ] - m_cal.m_nkJoyMid[ 0 ])
            / m_cal.m_nkJoyRange[ 0 ],
            ((float) cwiidState.ext.nunchuk.stick[ 1 ] - m_cal.m_nkJoyMid[ 1 ])
            / m_cal.m_nkJoyRange[ 1 ] );
        m_state.SetNunchuk( true, acceleration, gravity, joystick );
    }
    else
    {
        m_state.SetNunchuk( false );
    }
}

//=============================================================================

WiimoteImpl::CalibrationData::CalibrationData( )
    :   m_isCalibrated( false ),
        m_acc0( 134, 134, 134 ),
        m_acc1( 28, 28, 28 ),
        m_isNunchukCalibrated( false ),
        m_nkAcc0( 128, 128, 128 ),
        m_nkAcc1( 50, 50, 50 ),
        m_nkJoyMid( 128, 128 ),
        m_nkJoyRange( 100, 100 )
{
}

//-----------------------------------------------------------------------------

void 
WiimoteImpl::GetCalibrationData( )
{
    ::acc_cal accCal;
    if ( ! m_cal.m_isCalibrated )
    {
        int calRslt = ::cwiid_get_acc_cal( m_pCWiiD, ::CWIID_EXT_NONE,
                                           &accCal );
        if ( calRslt == 0 )
        {
            m_cal.m_isCalibrated = true;
            //these are the values when there is no force (0G)
            m_cal.m_acc0[ 0 ] = accCal.zero[ 0 ];    
            m_cal.m_acc0[ 1 ] = accCal.zero[ 1 ];    
            m_cal.m_acc0[ 2 ] = accCal.zero[ 2 ];
            //this is the effect of a 1G force
            m_cal.m_acc1[ 0 ] = accCal.one[ 0 ] - accCal.zero[ 0 ];
            m_cal.m_acc1[ 1 ] = accCal.one[ 1 ] - accCal.zero[ 1 ];
            m_cal.m_acc1[ 2 ] = accCal.one[ 2 ] - accCal.zero[ 2 ];
            Assert( m_cal.m_acc1[ 0 ] != 0 );
            Assert( m_cal.m_acc1[ 1 ] != 0 );
            Assert( m_cal.m_acc1[ 2 ] != 0 );
        }
    }
    if ( ! m_cal.m_isNunchukCalibrated )
    {
        struct ::cwiid_state cwiidState;
        int stateRslt = ::cwiid_get_state( m_pCWiiD, &cwiidState );
        Assert( stateRslt == 0 ); //!!!
        if ( cwiidState.ext_type == ::CWIID_EXT_NUNCHUK )
        {
            int calRslt = ::cwiid_get_acc_cal( m_pCWiiD, ::CWIID_EXT_NUNCHUK,
                                               &accCal );
            if ( calRslt == 0 )
            {
                m_cal.m_isNunchukCalibrated = true;
                m_cal.m_nkAcc0[ 0 ] = accCal.zero[ 0 ];    
                m_cal.m_nkAcc0[ 1 ] = accCal.zero[ 1 ];    
                m_cal.m_nkAcc0[ 2 ] = accCal.zero[ 2 ];
                m_cal.m_nkAcc1[ 0 ] = accCal.one[ 0 ] - accCal.zero[ 0 ];
                m_cal.m_nkAcc1[ 1 ] = accCal.one[ 1 ] - accCal.zero[ 0 ];
                m_cal.m_nkAcc1[ 2 ] = accCal.one[ 2 ] - accCal.zero[ 0 ];
                Assert( m_cal.m_nkAcc1[ 0 ] != 0 );
                Assert( m_cal.m_nkAcc1[ 1 ] != 0 );
                Assert( m_cal.m_nkAcc1[ 2 ] != 0 );
            }
        }
    }
}

//=============================================================================

WiimoteImpl::GravityEstimator::GravityEstimator( )
    :   m_lastGravity( 0.f, -1.f, 0.f ),
        m_steadyCount( 0 )
{
}

//=============================================================================

Vector3F 
WiimoteImpl::GravityEstimator::operator()( const Vector3F & curAccel )
{
    //This is essentially the same algorithm used in the WiiYourself! library.
    const int SteadyCountMin = 2;
    float lenSqr = curAccel.LengthSquared();
    if ( (lenSqr > 0.8f) && (lenSqr < 1.2f) )   //near unity
    {
        if ( ++m_steadyCount >= SteadyCountMin )
        {
            m_lastGravity = curAccel;
            m_lastGravity.Normalize( std::sqrt( lenSqr ) );
        }
    }
    else
    {
        m_steadyCount = 0;
    }
    return m_lastGravity;
}


//=============================================================================

#endif //USE_CWIID


//*****************************************************************************

}                                                      //namespace EpsilonDelta

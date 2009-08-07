/*
  CWiiDWiimote.cpp
  Copyright (C) 2007 David M. Anderson

  Wiimote class: the Wii remote input device.
  This implementation uses libcwiid, the standard Wiimote library for Linux.
*/


#ifdef SUPPORT_WIIMOTE
#ifdef USE_CWIID


#include "Wiimote.hpp"
#include "Exception.hpp"
#include <cstdio>
#include <string>
#include <iostream>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta


//*****************************************************************************


namespace
{                                                                   //namespace

class GravityEstimator
{
public:
    GravityEstimator( );
    Vector3F operator()( const Vector3F & curAccel );

private:
    Vector3F    m_lastGravity;
    int         m_steadyCount;
};

}                                                                   //namespace


//*****************************************************************************


class WiimoteImpl
{
public:
    WiimoteImpl( ::bdaddr_t * pBDaddr );
    WiimoteImpl( const std::string & BDaddrStr );
    virtual ~WiimoteImpl( );

    bool IsConnected( ) const;
    virtual std::string Name( ) const;
    void Update( WiimoteState * pState,
                 std::queue< WiimoteState::Button > * pPressedQueue );

private:
    void GetCalibrationData( );
    void Connect( );

    ::bdaddr_t          m_bdaddr;
    ::cwiid_wiimote_t * m_pCWiiD;

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
    CalibrationData     m_cal;
    GravityEstimator    m_gravityEstimator;
    GravityEstimator    m_nkGravityEstimator;
    Vector2I            m_pointerSeparation;
};


//*****************************************************************************


Wiimote::Wiimote( ::bdaddr_t * pBDaddr )
    :   m_pImpl( new WiimoteImpl( pBDaddr ) )
{
}

//.............................................................................

Wiimote::Wiimote( const std::string & BDaddrStr )
    :   m_pImpl( new WiimoteImpl( BDaddrStr ) )
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

::cwiid_err_t CWiiDErrorHandler;
const float pointerMeterSeparation = 250.f;

}                                                                   //namespace


//*****************************************************************************


WiimoteImpl::WiimoteImpl( ::bdaddr_t * pBDaddr )
{
    if ( pBDaddr )
        m_bdaddr = *pBDaddr;
    else
        m_bdaddr = *BDADDR_ANY;
    Connect( );
}

//.............................................................................

WiimoteImpl::WiimoteImpl( const std::string & BDaddrStr )
{
    int parseRslt = ::str2ba( BDaddrStr.c_str(), &m_bdaddr );
    if ( parseRslt )
        Connect( );
    else
        throw Exception( "Invalid bluetooth address string" );
}

//-----------------------------------------------------------------------------

WiimoteImpl::~WiimoteImpl( )
{
    if ( m_pCWiiD != 0 )
        ::cwiid_close( m_pCWiiD );
}

//=============================================================================

void 
WiimoteImpl::Connect( )
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


//=============================================================================

bool 
WiimoteImpl::IsConnected( ) const
{
    return (m_pCWiiD != 0);
}

//=============================================================================

std::string
WiimoteImpl::Name( ) const
{
    Assert( m_pCWiiD );
    char name[ 50 ];
    std::sprintf( name, "Wii remote %d", ::cwiid_get_id( m_pCWiiD ) );
    return name;
}

//=============================================================================

void
WiimoteImpl::Update( WiimoteState * pState,
                     std::queue< WiimoteState::Button > * pPressedQueue )
{
    Assert( m_pCWiiD );

    GetCalibrationData( );

    WiimoteState prevState = *pState;

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
    pState->SetButtons( buttons );
    for ( int i = 0; i < WiimoteState::NumButtons; ++i )
    {
        WiimoteState::Button b = WiimoteState::Button( i );
        if ( pState->IsButtonDown( b ) && ! prevState.IsButtonDown( b ) )
            pPressedQueue->push( b );
    }


    Vector3F acceleration(
        -((float) cwiidState.acc[ 0 ] - m_cal.m_acc0[ 0 ]) / m_cal.m_acc1[ 0 ],
        -((float) cwiidState.acc[ 2 ] - m_cal.m_acc0[ 2 ]) / m_cal.m_acc1[ 2 ],
        ((float) cwiidState.acc[ 1 ] - m_cal.m_acc0[ 1 ]) / m_cal.m_acc1[ 1 ] );
    pState->SetAcceleration( acceleration );
    Vector3F gravity = m_gravityEstimator( acceleration );
    pState->SetGravity( gravity );

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
        pState->SetPointer( 0 );
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
            pState->SetPointer( 1, scaledMiddle );
        }
        else //numPointerLights == 2
        {
            Angle angle = ArcTan( m_pointerSeparation.Y(),
                                  m_pointerSeparation.X() );
            Vector2F pointerSeparation( m_pointerSeparation.X(),
                                        m_pointerSeparation.Y() );
            float sepLen = pointerSeparation.Length();
            float distance = pointerMeterSeparation / sepLen;
            pState->SetPointer( 2, scaledMiddle, angle, distance );
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
        pState->SetNunchuk( true, acceleration, gravity, joystick );
    }
    else
    {
        pState->SetNunchuk( false );
    }
}

//=============================================================================


namespace
{                                                                   //namespace

void CWiiDErrorHandler( ::cwiid_wiimote_t * pCWiiD,
                        const char * formatStr, va_list vaData )
{
    char buf[ 300 ];
    if ( pCWiiD != 0 )
        sprintf( buf, "Error on Wiimote %d: ", ::cwiid_get_id( pCWiiD ) );
    else
        sprintf( buf, "Error on Wiimote (none): " );
    std::string errorMsg( buf );
	vsprintf( buf, formatStr, vaData );
    errorMsg += buf;
    throw Exception( errorMsg );
}

}                                                                   //namespace


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


//*****************************************************************************

namespace
{                                                                   //namespace
//-----------------------------------------------------------------------------


GravityEstimator::GravityEstimator( )
    :   m_lastGravity( 0.f, -1.f, 0.f ),
        m_steadyCount( 0 )
{
}

//=============================================================================

Vector3F 
GravityEstimator::operator()( const Vector3F & curAccel )
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

//-----------------------------------------------------------------------------
}                                                                   //namespace


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_CWIID
#endif //SUPPORT_WIIMOTE

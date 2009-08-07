#ifndef WIIMOTESTATE_HPP
#define WIIMOTESTATE_HPP
/*
  WiimoteState.hpp
  Copyright (C) 2009 David M. Anderson

  WiimoteState: state of buttons, acceleration, pointer, joystick.
  NOTES:
  1. NumPointerLights() determines which pointer values have been reliably
     determined. If 0, not are reliable. If 1, PointerPos() is approximate.
     If 2, all values should be accurate.
  2. The Set... functions are designed so that if a value cannot be reliably
     determined, the old value is retained. For example, if fewer than two
     lights are visible to the pointer, then the angle and distance keep
     their initial or most recent reliable values.
  3. Acceleration and gravity are measured as multiples of g, the standard
     force of gravity at the Earth's surface.
  4. When the Wiimote (or Nunchuk) is being held still, a force of magnitude
     1g is being applied in opposition to gravity. The Gravity()
     (NunchukGravity()) methods report an estimate of this force vector,
     obtained the last time the device was presumed to be relatively
     motionless.
*/


#if defined(SUPPORT_WIIMOTE)


#include "InputDeviceState.hpp"
#include "StdInt.hpp"
#include "Point2.hpp"
#include "Vector3.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class WiimoteState
    :   public InputDeviceState
{
public:
    WiimoteState( );
    virtual ~WiimoteState( );

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

    friend class WiimoteImpl;
};


//*****************************************************************************


inline
WiimoteState::WiimoteState( )
    :   m_buttonsDown( 0 ),
        m_acceleration( Vector3F::Zero )
{
}

//.............................................................................

inline
WiimoteState::Pointer::Pointer( )
    :   m_numLights( 0 ),
        m_position( Point2F::Zero ),
        m_angle( 0.f ),
        m_distance( 1.f )
{
}

//.............................................................................

inline
WiimoteState::Nunchuk::Nunchuk( )
    :   m_connected( false ),
        m_acceleration( Vector3F::Zero ),
        m_joystickPos( Point2F::Zero )
{
}

//=============================================================================

inline
uint32_t 
WiimoteState::ButtonsDown( ) const
{
    return m_buttonsDown;
}

//-----------------------------------------------------------------------------

inline
bool 
WiimoteState::IsButtonDown( Button button ) const
{
    return (m_buttonsDown & (1 << button)) != 0;
}

//-----------------------------------------------------------------------------

inline 
Vector3F
WiimoteState::Acceleration( ) const
{
    return m_acceleration;
}

//-----------------------------------------------------------------------------

inline
Vector3F 
WiimoteState::Gravity( ) const
{
    return m_gravity;
}

//=============================================================================

inline 
int
WiimoteState::NumPointerLights( ) const
{
    return m_pointer.m_numLights;
}

//-----------------------------------------------------------------------------

inline 
Point2F 
WiimoteState::PointerPos( ) const
{
    return m_pointer.m_position;
}

//-----------------------------------------------------------------------------

inline 
Angle 
WiimoteState::PointerAngle( ) const
{
    return m_pointer.m_angle;
}

//-----------------------------------------------------------------------------

inline 
float 
WiimoteState::PointerDistance( ) const
{
    return m_pointer.m_distance;
}

//=============================================================================

inline 
bool 
WiimoteState::IsNunchukConnected( ) const
{
    return m_nunchuk.m_connected;
}

//-----------------------------------------------------------------------------

inline 
Vector3F 
WiimoteState::NunchukAcceleration( ) const
{
    return m_nunchuk.m_acceleration;
}

//-----------------------------------------------------------------------------

inline 
Point2F 
WiimoteState::NunchukJoystickPos( ) const
{
    return m_nunchuk.m_joystickPos;
}

//=============================================================================

inline
void 
WiimoteState::SetButtons( uint32_t buttonsDown )
{
    m_buttonsDown = buttonsDown;
}

//-----------------------------------------------------------------------------

inline
void 
WiimoteState::SetAcceleration( const Vector3F & acceleration )
{
    m_acceleration = acceleration;
}

//-----------------------------------------------------------------------------

inline
void 
WiimoteState::SetGravity( const Vector3F & gravity )
{
    m_gravity = gravity;
}

//-----------------------------------------------------------------------------

inline
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

inline
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


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SUPPORT_WIIMOTE
#endif //WIIMOTESTATE_HPP

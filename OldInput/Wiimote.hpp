#ifndef WIIMOTE_HPP
#define WIIMOTE_HPP
/*
  Wiimote.hpp
  Copyright (C) 2007 David M. Anderson

  Wiimote class: the Wii remote input device.
  NOTES:
  1. When using the Linux libcwiid, the Wiimote may be specified by its
     Bluetooth device address. By default, any connected Wiimote will be used.
     The address can be specified as string or (pointer to) a bdaddr_t
     struct (/usr/include/bluetooth/bluetooth.h).
  2. When using the Windows WiiYourself! library, the Wiimote is specified
     by its index. By default, the first available device is used.
  3. This object maintains a queue of button presses. It is polled regularly
     by Input.
*/


#ifdef SUPPORT_WIIMOTE

#include "InputDevice.hpp"
#include "WiimoteState.hpp"
#include "Assert.hpp"
#include <queue>
#ifdef USE_CWIID
#include <cwiid.h>
#include <string>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class WiimoteImpl;


//*****************************************************************************


class Wiimote
    :   public InputDevice
{
public:
#ifdef USE_CWIID
    Wiimote( ::bdaddr_t * pBDaddr = 0 );
    Wiimote( const std::string & BDaddrStr );
#endif
#ifdef USE_WIIYOURSELF
    Wiimote( int index = 0 );
#endif
    virtual ~Wiimote( );

    bool IsConnected( ) const;

    virtual EDeviceType Type( ) const;
    virtual std::string Name( ) const;

    virtual const WiimoteState & State( );

private:
    WiimoteImpl *   m_pImpl;
    WiimoteState    m_state;
    std::queue< WiimoteState::Button > m_pressedQueue;

    void Update( );
    bool WasButtonPressed( );
    WiimoteState::Button GetButtonPressed( );

    friend class Input;
};


//*****************************************************************************


inline
InputDevice::EDeviceType 
Wiimote::Type( ) const
{
    return WiimoteDevice;
}

//=============================================================================

inline 
const WiimoteState & 
Wiimote::State( )
{
    return m_state;
}

//=============================================================================

inline
bool 
Wiimote::WasButtonPressed( )
{
    return ! m_pressedQueue.empty();
}

//-----------------------------------------------------------------------------

inline
WiimoteState::Button 
Wiimote::GetButtonPressed( )
{
    Assert( ! m_pressedQueue.empty() );
    WiimoteState::Button button = m_pressedQueue.front();
    m_pressedQueue.pop( );
    return button;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SUPPORT_WIIMOTE
#endif //WIIMOTE_HPP

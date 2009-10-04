#ifndef INPUT_HPP
#define INPUT_HPP
/*
  Input.hpp
  Copyright (C) 2009 David M. Anderson

  Input class: Manager of the input devices.
  NOTES:
  1. Update should be called regularly (each frame, typically).
  2. DeviceIndex() returns the first device of the specified type,
     or -1 if none is present.
  3. IgnoreDeviceTypes() can be called to avoid detecting and using devices of
     the specified types. (E.g., Wiimotes, which take some time to check for
     with the CWiiD library.) It needs to be called before Init().
  4. When TextInput is true, keyboard events are returned as the Unicode
     characters they represent. This is off by default.
*/


#include "InputDevice.hpp"
#include "InputEvent.hpp"
#include "Singleton.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputImpl;


//*****************************************************************************


class Input
    :   public Singleton< Input >
{
public:
    void Init( );
    void Shutdown( );

    void Update( );
    std::tr1::shared_ptr< InputEvent const > CheckEvent( );

    int NumDevices( ) const;
    std::tr1::shared_ptr< InputDevice const > Device( int index ) const;
    int DeviceIndex( InputDevice::EType type ) const;

    typedef void (*QuitHandler)( );
    void SetQuitHandler( QuitHandler quitHandler );

    void IgnoreDeviceTypes( uint32_t typeBits );

    void SetTextInput( bool on );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    Input( );
    ~Input( );
    void HandleQuit( );
    uint32_t IgnoreTypes( ) const;

    QuitHandler                         m_quitHandler;
    uint32_t                            m_ignoreTypes;
    std::tr1::shared_ptr< InputImpl >   m_pImpl;

    friend class Singleton< Input >;
    friend class InputImpl;
};


//*****************************************************************************


inline
void 
Input::SetQuitHandler( QuitHandler quitHandler )
{
    m_quitHandler = quitHandler;
}

//-----------------------------------------------------------------------------

inline
void 
Input::IgnoreDeviceTypes( uint32_t typeBits )
{
    m_ignoreTypes = typeBits;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUT_HPP

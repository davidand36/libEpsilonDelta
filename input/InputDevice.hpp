#ifndef INPUTDEVICE_HPP
#define INPUTDEVICE_HPP
/*
  InputDevice.hpp
  Copyright (C) 2009 David M. Anderson

  InputDevice class: In input device and its state.
*/


#include "Point2.hpp"
#include "Vector3.hpp"
#include <string>
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputDevice
{
public:
    enum Type { Keyboard  = 1 << 0,
                Mouse     = 1 << 1,
                Tablet    = 1 << 2,
                Gamepad   = 1 << 3,
                Wiimote   = 1 << 4
              };

    InputDevice( Type type, const std::string & name );
    virtual ~InputDevice( );

    Type GetType( ) const;
    const std::string & Name( ) const;

    virtual int NumButtons( ) const;
    virtual bool ButtonDown( int button ) const;

    virtual int NumPointers( ) const;
    virtual Point2I Pointer( int index = 0 ) const;

    virtual int NumAxes( ) const;
    virtual double Axis( int index ) const;

    virtual int NumAccelerometers( ) const;
    virtual Vector3D Acceleration( int index = 0 ) const;
    virtual Vector3D Gravity( int index = 0 ) const;

protected:
    void SetName( const std::string & name );

private:
    virtual void Update( );

    Type                        m_type;
    std::string                 m_name;

    friend class Input;
};


//*****************************************************************************


inline
InputDevice::InputDevice( Type type, const std::string & name )
    :   m_type( type ),
        m_name( name )
{
}

//=============================================================================

inline
InputDevice::Type 
InputDevice::GetType( ) const
{
    return m_type;
}

//-----------------------------------------------------------------------------

inline
const std::string & 
InputDevice::Name( ) const
{
    return m_name;
}

//-----------------------------------------------------------------------------

inline
void 
InputDevice::SetName( const std::string & name )
{
    m_name = name;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTDEVICE_HPP

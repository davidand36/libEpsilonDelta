/*
  InputDevice.cpp
  Copyright (C) 2009 David M. Anderson

  InputDevice class: In input device and its state.
*/


#include "InputDevice.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


InputDevice::~InputDevice( )
{
}

//=============================================================================

int 
InputDevice::NumButtons( ) const
{
    return 0;
}

//-----------------------------------------------------------------------------

inline
bool 
InputDevice::ButtonDown( int button ) const
{
    throw std::out_of_range( "InputDevice::ButtonDown()" );
}

//=============================================================================

inline
int 
InputDevice::NumPointers( ) const
{
    return 0;
}

//-----------------------------------------------------------------------------

inline
Point2I 
InputDevice::Pointer( int index ) const
{
    throw std::out_of_range( "InputDevice::Pointer()" );
}

//=============================================================================

inline
int 
InputDevice::NumAxes( ) const
{
    return 0;
}

//-----------------------------------------------------------------------------

inline
double 
InputDevice::Axis( int index ) const
{
    throw std::out_of_range( "InputDevice::Axis()" );
}

//=============================================================================

inline
int 
InputDevice::NumAccelerometers( ) const
{
    return 0;
}

//-----------------------------------------------------------------------------

inline
Vector3D 
InputDevice::Acceleration( int index ) const
{
    throw std::out_of_range( "InputDevice::Acceleration()" );
}

//-----------------------------------------------------------------------------

inline
Vector3D 
InputDevice::Gravity( int index ) const
{
    throw std::out_of_range( "InputDevice::Gravity()" );
}

//=============================================================================

void 
InputDevice::Update( )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

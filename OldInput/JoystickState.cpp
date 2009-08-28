/*
  JoystickState.cpp
  Copyright (C) 2007 David M. Anderson

  JoystickState: position of axes and which buttons are currently down.
*/


#include "JoystickState.hpp"
#include "Assert.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


JoystickState::~JoystickState( )
{
}

//=============================================================================

#ifdef USE_SDL

void 
JoystickState::Set( const std::vector< bool > & buttonStates,
                    const std::vector< int > & axes )
{
    m_buttonStates = buttonStates;
    m_axes = axes;
}

#endif

//=============================================================================

bool 
JoystickState::IsButtonDown( int button ) const
{
    Assert( (button >= 0)
            && (button < static_cast<int>(m_buttonStates.size())) );
    return  m_buttonStates[ button ];
}

//-----------------------------------------------------------------------------

int 
JoystickState::AxisPosition( int axis ) const
{
    Assert( (axis >= 0) && (axis < static_cast<int>(m_axes.size())) );
    return  m_axes[ axis ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


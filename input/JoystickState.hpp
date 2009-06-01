#ifndef JOYSTICKSTATE_HPP
#define JOYSTICKSTATE_HPP
/*
  JoystickState.hpp
  Copyright (C) 2007 David M. Anderson

  JoystickState: position of axes and which buttons are currently down.
*/


#include "InputDeviceState.hpp"
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class JoystickState
    :   public InputDeviceState
{
public:
    virtual ~JoystickState( );

#ifdef USE_SDL
    void Set( const std::vector< bool > & buttonStates,
              const std::vector< int > & axes );
#endif
    bool IsButtonDown( int button ) const;
    int AxisPosition( int axis ) const;

private:
#ifdef USE_SDL
    std::vector< bool > m_buttonStates;
    std::vector< int > m_axes;
#endif
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JOYSTICKSTATE_HPP

#ifndef WIIMOTE_HPP
#define WIIMOTE_HPP
/*
  Wiimote.hpp
  Copyright (C) 2009 David M. Anderson

  Wiimote class: a standard wiimote, a.k.a. joystick.
*/


#include "InputDevice.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class WiimoteImpl;


//*****************************************************************************


class Wiimote
    :   public InputDevice
{
public:
    Wiimote( const std::string & name, shared_ptr< WiimoteImpl > pImpl );
    virtual ~Wiimote( );

    bool IsConnected( );

    virtual int NumButtons( ) const;
    virtual bool ButtonDown( int button ) const;

    bool WasButtonPressed( );
    int GetButtonPressed( );

    virtual int NumPointers( ) const;
    virtual Point2I Pointer( int index = 0 ) const;

    virtual int NumAxes( ) const;
    virtual double Axis( int index = 0 ) const;

    virtual int NumAccelerometers( ) const;
    virtual Vector3D Acceleration( int index = 0 ) const;
    virtual Vector3D Gravity( int index = 0 ) const;

    static void FindAll( std::vector< shared_ptr< Wiimote > > * pWiimotes );

private:
    virtual void Update( );

    shared_ptr< WiimoteImpl >  m_pImpl;

    friend class InputImpl;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //WIIMOTE_HPP

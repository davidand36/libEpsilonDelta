/*
  InputButtonMap.cpp
  Copyright (C) 2009 David M. Anderson

  InputButtonMap class: Associates button presses (on input devices)
  with actions.
*/


#include "InputButtonMap.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta


//*****************************************************************************


InputButtonMap::InputButtonMap( )
    :   m_map( &ButtonLess )
{
}

//=============================================================================

void 
InputButtonMap::Set( shared_ptr< InputDevice const > device, int button,
                     int action )
{
    DevButton devButton = { device, button };
    m_map[ devButton ] = action;
}

//-----------------------------------------------------------------------------

void 
InputButtonMap::Remove( shared_ptr< InputDevice const > device, int button )
{
    DevButton devButton = { device, button };
    m_map.erase( devButton );
}

//-----------------------------------------------------------------------------

void
InputButtonMap::Reset( )
{
    m_map.clear( );
}

//=============================================================================

int 
InputButtonMap::Action( shared_ptr< InputDevice const > device,
                        int button ) const
{
    DevButton devButton = { device, button };
    MapType::const_iterator pKV = m_map.find( devButton );
    if ( pKV != m_map.end() )
        return pKV->second;
    return NoAction;
}

//=============================================================================

bool 
InputButtonMap::ButtonLess( const DevButton & lhs, const DevButton & rhs )
{
    if ( lhs.m_button < rhs.m_button )
        return true;
    if ( lhs.m_button > rhs.m_button )
        return false;
    return (lhs.m_device < rhs.m_device);
}


//*****************************************************************************


#ifdef DEBUG

bool 
InputButtonMap::Test( )
{
    bool ok = true;
    cout << "Testing InputButtonMap" << endl;

    shared_ptr< InputDevice const > pDev1(
        new InputDevice( InputDevice::Keyboard, "FakeKeyboard" ) );
    shared_ptr< InputDevice const > pDev2(
        new InputDevice( InputDevice::Mouse, "FakeMouse" ) );
    InputEvent event110( pDev1, 10 );
    InputEvent event120( pDev1, 20 );
    InputEvent event210( pDev2, 10 );
    InputEvent event230( pDev2, 30 );

    InputButtonMap map1;

    cout << "Set( pDev1, 10, 110 )" << endl;
    map1.Set( pDev1, 10, 110 );
    cout << "Set( pDev2, 30, 230 )" << endl;
    map1.Set( pDev2, 30, 230 );
    TESTCHECK( map1.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map1.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 30 ), 230, &ok );
    cout << "Set( pDev2, 10, 210 )" << endl;
    map1.Set( pDev2, 10, 210 );
    TESTCHECK( map1.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map1.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 10 ), 210, &ok );
    TESTCHECK( map1.Action( pDev2, 30 ), 230, &ok );
    cout << "Remove( pDev2, 30 )" << endl;
    map1.Remove( pDev2, 30 );
    TESTCHECK( map1.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map1.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 10 ), 210, &ok );
    TESTCHECK( map1.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );

    InputButtonMap map2;

    cout << "Set( pDev1, 10, 1110 )" << endl;
    map2.Set( pDev1, 10, 1110 );
    cout << "Set( pDev1, 20, 1120 )" << endl;
    map2.Set( pDev1, 20, 1120 );
    TESTCHECK( map2.Action( pDev1, 10 ), 1110, &ok );
    TESTCHECK( map2.Action( pDev1, 20 ), 1120, &ok );
    TESTCHECK( map2.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map2.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );
    cout << "Set( pDev1, 10, 2110 )" << endl;
    map2.Set( pDev1, 10, 2110 );
    TESTCHECK( map2.Action( pDev1, 10 ), 2110, &ok );
    TESTCHECK( map2.Action( pDev1, 20 ), 1120, &ok );
    TESTCHECK( map2.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map2.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );

    cout << "map3 = map1" << endl;
    InputButtonMap map3 = map1;
    TESTCHECK( map3.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map3.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev2, 10 ), 210, &ok );
    TESTCHECK( map3.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );
    cout << "Remove( pDev1, 20 )" << endl;
    map3.Remove( pDev1, 20 );
    TESTCHECK( map3.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map3.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev2, 10 ), 210, &ok );
    TESTCHECK( map3.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );
    cout << "Remove( pDev2, 10 )" << endl;
    map3.Remove( pDev2, 10 );
    cout << "Set( pDev2, 30, 2230 )" << endl;
    map3.Set( pDev2, 30, 2230 );
    TESTCHECK( map3.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map3.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev2, 30 ), 2230, &ok );
    TESTCHECK( map1.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map1.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 10 ), 210, &ok );
    TESTCHECK( map1.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );
    cout << "Reset( )" << endl;
    map1.Reset( );
    TESTCHECK( map1.Action( pDev1, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map3.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev2, 30 ), 2230, &ok );
    cout << "Set( pDev1, 10, 110 )" << endl;
    map1.Set( pDev1, 10, 110 );
    cout << "Set( pDev2, 30, 230 )" << endl;
    map1.Set( pDev2, 30, 230 );
    TESTCHECK( map1.Action( pDev1, 10 ), 110, &ok );
    TESTCHECK( map1.Action( pDev1, 20 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map1.Action( pDev2, 30 ), 230, &ok );

    cout << "map3 = map2" << endl;
    map3 = map2;
    TESTCHECK( map3.Action( pDev1, 10 ), 2110, &ok );
    TESTCHECK( map3.Action( pDev1, 20 ), 1120, &ok );
    TESTCHECK( map3.Action( pDev2, 10 ), InputButtonMap::NoAction, &ok );
    TESTCHECK( map3.Action( pDev2, 30 ), InputButtonMap::NoAction, &ok );

    if ( ok )
        cout << "InputButtonMap PASSED." << endl << endl;
    else
        cout << "InputButtonMap FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

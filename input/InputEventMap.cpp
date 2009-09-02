/*
  InputEventMap.cpp
  Copyright (C) 2009 David M. Anderson

  InputEventMap class: Maps input events to user-defined values.
*/


#include "InputEventMap.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta


//*****************************************************************************


namespace
{                                                                   //namespace

bool InputEventLess( const InputEvent & lhs, const InputEvent & rhs );

}                                                                   //namespace

//*****************************************************************************


InputEventMap::InputEventMap( )
    :   m_map( &InputEventLess )
{
}

//=============================================================================

void 
InputEventMap::Set( const InputEvent & event, int value )
{
    m_map[ event ] = value;
}

//-----------------------------------------------------------------------------

void 
InputEventMap::Remove( const InputEvent & event )
{
    m_map.erase( event );
}

//=============================================================================

int 
InputEventMap::Value( const InputEvent & event ) const
{
    MapType::const_iterator pKV = m_map.find( event );
    if ( pKV != m_map.end() )
        return pKV->second;
    return NoValue;
}


//*****************************************************************************


namespace
{                                                                   //namespace

//=============================================================================

bool 
InputEventLess( const InputEvent & lhs, const InputEvent & rhs )
{
    if ( lhs.Button() < rhs.Button() )
        return true;
    if ( lhs.Button() > rhs.Button() )
        return false;
    return (lhs.Device() < rhs.Device());
}

//=============================================================================

}                                                                   //namespace


//*****************************************************************************


#ifdef DEBUG

bool 
InputEventMap::Test( )
{
    bool ok = true;
    cout << "Testing InputEventMap" << endl;

    shared_ptr< InputDevice const > pDev1(
        new InputDevice( InputDevice::Keyboard, "FakeKeyboard" ) );
    shared_ptr< InputDevice const > pDev2(
        new InputDevice( InputDevice::Mouse, "FakeMouse" ) );
    InputEvent event110( pDev1, 10 );
    InputEvent event120( pDev1, 20 );
    InputEvent event210( pDev2, 10 );
    InputEvent event230( pDev2, 30 );

    InputEventMap map1;

    cout << "Set( event110, 110 )" << endl;
    map1.Set( event110, 110 );
    cout << "Set( event230, 230 )" << endl;
    map1.Set( event230, 230 );
    TESTCHECK( map1.Value( event110 ), 110, &ok );
    TESTCHECK( map1.Value( event120 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map1.Value( event210 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map1.Value( event230 ), 230, &ok );
    cout << "Set( event210, 210 )" << endl;
    map1.Set( event210, 210 );
    TESTCHECK( map1.Value( event110 ), 110, &ok );
    TESTCHECK( map1.Value( event120 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map1.Value( event210 ), 210, &ok );
    TESTCHECK( map1.Value( event230 ), 230, &ok );
    cout << "Remove( event230 )" << endl;
    map1.Remove( event230 );
    TESTCHECK( map1.Value( event110 ), 110, &ok );
    TESTCHECK( map1.Value( event120 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map1.Value( event210 ), 210, &ok );
    TESTCHECK( map1.Value( event230 ), InputEventMap::NoValue, &ok );

    InputEventMap map2;

    cout << "Set( event110, 1110 )" << endl;
    map2.Set( event110, 1110 );
    cout << "Set( event120, 1120 )" << endl;
    map2.Set( event120, 1120 );
    TESTCHECK( map2.Value( event110 ), 1110, &ok );
    TESTCHECK( map2.Value( event120 ), 1120, &ok );
    TESTCHECK( map2.Value( event210 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map2.Value( event230 ), InputEventMap::NoValue, &ok );

    cout << "map3 = map1" << endl;
    InputEventMap map3 = map1;
    TESTCHECK( map3.Value( event110 ), 110, &ok );
    TESTCHECK( map3.Value( event120 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map3.Value( event210 ), 210, &ok );
    TESTCHECK( map3.Value( event230 ), InputEventMap::NoValue, &ok );
    cout << "Remove( event120 )" << endl;
    map3.Remove( event120 );
    TESTCHECK( map3.Value( event110 ), 110, &ok );
    TESTCHECK( map3.Value( event120 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map3.Value( event210 ), 210, &ok );
    TESTCHECK( map3.Value( event230 ), InputEventMap::NoValue, &ok );
    cout << "Remove( event210 )" << endl;
    map3.Remove( event210 );
    cout << "Set( event230, 2230 )" << endl;
    map3.Set( event230, 2230 );
    TESTCHECK( map3.Value( event110 ), 110, &ok );
    TESTCHECK( map3.Value( event120 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map3.Value( event210 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map3.Value( event230 ), 2230, &ok );
    TESTCHECK( map1.Value( event110 ), 110, &ok );
    TESTCHECK( map1.Value( event120 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map1.Value( event210 ), 210, &ok );
    TESTCHECK( map1.Value( event230 ), InputEventMap::NoValue, &ok );

    cout << "map3 = map2" << endl;
    map3 = map2;
    TESTCHECK( map3.Value( event110 ), 1110, &ok );
    TESTCHECK( map3.Value( event120 ), 1120, &ok );
    TESTCHECK( map3.Value( event210 ), InputEventMap::NoValue, &ok );
    TESTCHECK( map3.Value( event230 ), InputEventMap::NoValue, &ok );

    if ( ok )
        cout << "InputEventMap PASSED." << endl << endl;
    else
        cout << "InputEventMap FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  InputDeviceMap.cpp
  Copyright (C) 2009 David M. Anderson

  InputDeviceMap class: Associates input devices with owners.
*/


#include "InputDeviceMap.hpp"
#include "MappedInput.hpp"
#include <algorithm>
#ifdef DEBUG
#include "TestCheck.hpp"
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


InputDeviceMap::InputDeviceMap( )
{
}

//-----------------------------------------------------------------------------

InputDeviceMap::~InputDeviceMap( )
{
}

//=============================================================================

void 
InputDeviceMap::Set( shared_ptr< InputDevice const > pDevice, int owner )
{
    for ( size_t i = 0; i < m_ownedDevices.size(); ++i )
        if ( m_ownedDevices[ i ].m_pDevice == pDevice )
        {
            m_ownedDevices[ i ].m_owner = owner;
            return;
        }
    OwnedDevice ownedDev = { pDevice, owner };
    m_ownedDevices.push_back( ownedDev );
}

//-----------------------------------------------------------------------------

void 
InputDeviceMap::Remove( shared_ptr< InputDevice const > pDevice )
{
    for ( vector< OwnedDevice >::iterator pOwnedDev = m_ownedDevices.begin();
          pOwnedDev != m_ownedDevices.end(); )
    {
        if ( pOwnedDev->m_pDevice == pDevice )
            pOwnedDev = m_ownedDevices.erase( pOwnedDev );
        else
            ++pOwnedDev;
    }
}

//-----------------------------------------------------------------------------

void 
InputDeviceMap::Reset( )
{
    m_ownedDevices.clear( );
}

//=============================================================================

int 
InputDeviceMap::Owner( shared_ptr< InputDevice const > pDevice ) const
{
    for ( size_t i = 0; i < m_ownedDevices.size(); ++i )
        if ( m_ownedDevices[ i ].m_pDevice == pDevice )
            return m_ownedDevices[ i ].m_owner;
    return MappedInput::NoOwner;
}

//-----------------------------------------------------------------------------

vector< shared_ptr< InputDevice const > > 
InputDeviceMap::Devices( int owner ) const
{
    vector< shared_ptr< InputDevice const > > devices;
    for ( size_t i = 0; i < m_ownedDevices.size(); ++i )
        if ( m_ownedDevices[ i ].m_owner == owner )
            devices.push_back( m_ownedDevices[ i ].m_pDevice );
    return devices;
}

//=============================================================================

#ifdef DEBUG

bool 
InputDeviceMap::Test( )
{
    bool ok = true;
    cout << "Testing InputDeviceMap" << endl;

    InputDeviceMap & devOwners = InputDeviceMap::Instance();

    shared_ptr< InputDevice > pDev1( new InputDevice( InputDevice::Keyboard,
                                                      "FakeKeyboard" ) );
    shared_ptr< InputDevice > pDev2( new InputDevice( InputDevice::Mouse,
                                                      "FakeMouse" ) );

    TESTCHECK( devOwners.Owner( pDev1 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Set( pDev2, 20 )" << endl;
    devOwners.Set( pDev2, 20 );
    TESTCHECK( devOwners.Owner( pDev1 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 20, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Set( pDev1, 10 )" << endl;
    devOwners.Set( pDev1, 10 );
    TESTCHECK( devOwners.Owner( pDev1 ), 10, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 20, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)1, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Set( pDev2, 10 )" << endl;
    devOwners.Set( pDev2, 10 );
    TESTCHECK( devOwners.Owner( pDev1 ), 10, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 10, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)2, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Remove( pDev1 )" << endl;
    devOwners.Remove( pDev1 );
    TESTCHECK( devOwners.Owner( pDev1 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 10, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)1, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Set( pDev1, 20 )" << endl;
    devOwners.Set( pDev1, 20 );
    TESTCHECK( devOwners.Owner( pDev1 ), 20, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 10, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)1, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Reset()" << endl;
    devOwners.Reset( );
    TESTCHECK( devOwners.Owner( pDev1 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Set( pDev2, 20 )" << endl;
    devOwners.Set( pDev2, 20 );
    TESTCHECK( devOwners.Owner( pDev1 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 20, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Reset()" << endl;
    devOwners.Reset( );
    TESTCHECK( devOwners.Owner( pDev1 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), MappedInput::NoOwner, &ok );
    TESTCHECK( devOwners.Devices( MappedInput::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );

    if ( ok )
        cout << "InputDeviceMap PASSED." << endl << endl;
    else
        cout << "InputDeviceMap FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

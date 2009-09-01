/*
  DeviceOwners.cpp
  Copyright (C) 2009 David M. Anderson

  DeviceOwners class: Associates input devices with owners.
*/


#include "DeviceOwners.hpp"
#include <algorithm>
#ifdef DEBUG
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


DeviceOwners::DeviceOwners( )
{
}

//-----------------------------------------------------------------------------

DeviceOwners::~DeviceOwners( )
{
}

//=============================================================================

void 
DeviceOwners::Set( shared_ptr< InputDevice const > pDevice, int owner )
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

//=============================================================================

int 
DeviceOwners::Owner( shared_ptr< InputDevice const > pDevice ) const
{
    for ( size_t i = 0; i < m_ownedDevices.size(); ++i )
        if ( m_ownedDevices[ i ].m_pDevice == pDevice )
            return m_ownedDevices[ i ].m_owner;
    return NoOwner;
}

//-----------------------------------------------------------------------------

vector< shared_ptr< InputDevice const > > 
DeviceOwners::Devices( int owner ) const
{
    vector< shared_ptr< InputDevice const > > devices;
    for ( size_t i = 0; i < m_ownedDevices.size(); ++i )
        if ( m_ownedDevices[ i ].m_owner == owner )
            devices.push_back( m_ownedDevices[ i ].m_pDevice );
    return devices;
}

//=============================================================================

void 
DeviceOwners::Disown( shared_ptr< InputDevice const > pDevice )
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
DeviceOwners::Reset( )
{
    m_ownedDevices.clear( );
}

//=============================================================================

#ifdef DEBUG

bool 
DeviceOwners::Test( )
{
    bool ok = true;
    cout << "Testing DeviceOwners" << endl;

    DeviceOwners & devOwners = DeviceOwners::Instance();

    shared_ptr< InputDevice > pDev1( new InputDevice( InputDevice::Keyboard,
                                                      "FakeKeyboard" ) );
    shared_ptr< InputDevice > pDev2( new InputDevice( InputDevice::Mouse,
                                                      "FakeMouse" ) );

    TESTCHECK( devOwners.Owner( pDev1 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Set( pDev2, 20 )" << endl;
    devOwners.Set( pDev2, 20 );
    TESTCHECK( devOwners.Owner( pDev1 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 20, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Set( pDev1, 10 )" << endl;
    devOwners.Set( pDev1, 10 );
    TESTCHECK( devOwners.Owner( pDev1 ), 10, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 20, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)1, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Set( pDev2, 10 )" << endl;
    devOwners.Set( pDev2, 10 );
    TESTCHECK( devOwners.Owner( pDev1 ), 10, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 10, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)2, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Disown( pDev1 )" << endl;
    devOwners.Disown( pDev1 );
    TESTCHECK( devOwners.Owner( pDev1 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 10, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)1, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Set( pDev1, 20 )" << endl;
    devOwners.Set( pDev1, 20 );
    TESTCHECK( devOwners.Owner( pDev1 ), 20, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 10, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)1, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Reset()" << endl;
    devOwners.Reset( );
    TESTCHECK( devOwners.Owner( pDev1 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );
    cout << "Set( pDev2, 20 )" << endl;
    devOwners.Set( pDev2, 20 );
    TESTCHECK( devOwners.Owner( pDev1 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), 20, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)1, &ok );
    cout << "Reset()" << endl;
    devOwners.Reset( );
    TESTCHECK( devOwners.Owner( pDev1 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Owner( pDev2 ), DeviceOwners::NoOwner, &ok );
    TESTCHECK( devOwners.Devices( DeviceOwners::NoOwner ).size(), (size_t)0,
               &ok );
    TESTCHECK( devOwners.Devices( 10 ).size(), (size_t)0, &ok );
    TESTCHECK( devOwners.Devices( 20 ).size(), (size_t)0, &ok );

    if ( ok )
        cout << "DeviceOwners PASSED." << endl << endl;
    else
        cout << "DeviceOwners FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

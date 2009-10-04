/*
  MappedInput.cpp
  Copyright (C) 2009 David M. Anderson

  MappedInput class: Maps input events to owners and actions.
*/


#include "MappedInput.hpp"
#include "Input.hpp"
#include "InputDeviceMap.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Timer.hpp"
#include "Graphics2D.hpp"
#include "Surface.hpp"
#include "Rectangle.hpp"
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace
{                                                                   //namespace

shared_ptr< InputDevice const > GetPointerDevice( );

}                                                                   //namespace


//*****************************************************************************


MappedInput::MappedInput( )
{
}

//-----------------------------------------------------------------------------

MappedInput::~MappedInput( )
{
}

//=============================================================================

void 
MappedInput::SetButtonMap( shared_ptr< InputButtonMap const > pButtonMap )
{
    m_pButtonMap = pButtonMap;
}

//-----------------------------------------------------------------------------

void 
MappedInput::SetRegionMap( shared_ptr< RegionMap const > pRegionMap,
                           shared_ptr< InputDevice const > pDevice,
                           int button, int pointer )
{
    if ( ! pDevice )
        pDevice = GetPointerDevice( );
    if ( ! pDevice )
        return;
    Assert( pDevice->NumButtons() > button );
    Assert( pDevice->NumPointers() > pointer );
    for ( size_t i = 0; i < m_regionMaps.size(); ++i )
        if ( m_regionMaps[ i ].Matches( pDevice, button, pointer ) )
        {
            m_regionMaps[ i ].m_pRegionMap = pRegionMap;
            return;
        }
    RegMapItem regMapItem = { pDevice, button, pointer, pRegionMap };
    m_regionMaps.push_back( regMapItem );
}

//-----------------------------------------------------------------------------

void 
MappedInput::RemoveRegionMap( shared_ptr< InputDevice const > pDevice,
                              int button, int pointer )
{
    if ( ! pDevice )
        pDevice = GetPointerDevice( );
    if ( ! pDevice )
        return;
    for ( vector< RegMapItem >::iterator pRegMapItem
                  = m_regionMaps.begin(); pRegMapItem != m_regionMaps.end(); )
    {
        if ( pRegMapItem->Matches( pDevice, button, pointer ) )
            pRegMapItem = m_regionMaps.erase( pRegMapItem );
        else
            ++pRegMapItem;
    }
}

//=============================================================================

bool 
MappedInput::CheckEvent( int * pAction, int * pOwner ) const
{
    return MapEvent( Input::Instance().CheckEvent(), pAction, pOwner );
}

//=============================================================================

bool 
MappedInput::MapEvent( shared_ptr< InputEvent const > pEvent,
                       int * pAction, int * pOwner ) const
{
    if ( pEvent )
    {
        int action = NoAction;
        shared_ptr< InputDevice const > pDevice = pEvent->Device();
        Assert( pDevice );
        int button = pEvent->Button();
        Assert( pDevice->NumButtons() > button );

        if ( m_pButtonMap )
            action = m_pButtonMap->Action( pDevice, button );

        if ( action == NoAction )
            for ( size_t i = 0; i < m_regionMaps.size(); ++i )
                if ( m_regionMaps[ i ].Matches( pDevice, button ) )
                {
                    int pointer = m_regionMaps[ i ].m_pointer;
                    Assert( pDevice->NumPointers() > pointer );
                    Point2I point = pDevice->Pointer( pointer );
                    action = m_regionMaps[ i ].m_pRegionMap->Action( point );
                    break;
                }

        if ( pAction )
            *pAction = action;
        if ( pOwner )
            *pOwner = InputDeviceMap::Instance().Owner( pDevice );
        return true;
    }
    else
    {
        if ( pAction )
            *pAction = NoAction;
        if ( pOwner )
            *pOwner = NoOwner;
        return false;
    }
}

//=============================================================================

#ifdef DEBUG

bool 
MappedInput::Test( )
{
    bool ok = true;
    cout << "Testing MappedInput" << endl;

    Input & input = Input::Instance();
    MappedInput & mappedInput = MappedInput::Instance();

    try
    {
        input.Init( );

        int numDevices = input.NumDevices();
        shared_ptr< InputDevice const > pDev0;
        shared_ptr< InputDevice const > pDev1;
        for ( int i = 0; i < numDevices; ++i )
            if ( input.Device( i )->NumButtons() > 2 )
            {
                pDev0 = input.Device( i );
                cout << "Device " << i << " (" << pDev0->Name() << ")"
                     << " is assigned to owner 10." << endl;
                InputDeviceMap::Instance().Set( pDev0, 10 );
                break;
            }
        for ( int i = 0; i < numDevices; ++i )
            if ( input.Device( i )->NumPointers() > 0 )
            {
                pDev1 = input.Device( i );
                cout << "Device " << i << " (" << pDev1->Name() << ")"
                     << " is assigned to owner 20." << endl;
                InputDeviceMap::Instance().Set( pDev1, 20 );
                break;
            }
        cout << "All other devices have no owner ("
             << MappedInput::NoOwner << ")." << endl; 

        shared_ptr< InputButtonMap > pButtonMap0( new InputButtonMap );
        shared_ptr< InputButtonMap > pButtonMap1( new InputButtonMap );
        Timer timer;
        if ( pDev0 )
        {
            int actions[ 3 ] = { 15, 3, 1955 };
            for ( int i = 0; i < 3; ++i )
            {
                cout << "Press a key or button on any device." << endl;
                timer.Reset( );
                while ( true )
                {
                    input.Update( );
                    shared_ptr< InputEvent const > pEvent = input.CheckEvent();
                    if ( pEvent )
                    {
                        cout << "[Device \"" << pEvent->Device()->Name()
                             << "\" button " << pEvent->Button() << "]"
                             << " is assigned action " << actions[ i ] << endl;
                        pButtonMap0->Set( pEvent->Device(), pEvent->Button(),
                                         actions[ i ] );
                        pButtonMap1->Set( pEvent->Device(), pEvent->Button(),
                                         actions[ 2 - i ] );
                        break;
                    }
                    if ( timer.Seconds() > 5. )
                    {
                        cout << "Timed out" << endl;
                        break;
                    }
                }
            }
        }
        cout << "SetButtonMap()" << endl;
        mappedInput.SetButtonMap( pButtonMap0 );

        shared_ptr< RegionMap > pRegMap0( new RegionMap );
        shared_ptr< RegionMap > pRegMap1( new RegionMap );
        if ( pDev1 )
        {
            Rectangle screenRect = Graphics2D::Instance().Screen()->Extent();
            int sw = screenRect.Width();
            int sh = screenRect.Height();
            if ( pDev1->NumButtons() > 0 )
            {
                cout << "For Device \"" << pDev1->Name() << "\" button 0"
                     << endl
                     << " the right half of the screen has action 25" << endl
                     << " and the left half has no action ("
                     << MappedInput::NoAction << ")." << endl;
                pRegMap0->Set( shared_ptr< Rectangle >(
                                 new Rectangle( sw / 2, 0, sw / 2, sh ) ), 25 );
                cout << "SetRegionMap()" << endl;
                mappedInput.SetRegionMap( pRegMap0, pDev1, 0 );
            }
            if ( pDev1->NumButtons() > 1 )
            {
                cout << "For Device \"" << pDev1->Name() << "\" button 1"
                     << endl
                     << " the top half of the screen has action 2005" << endl
                     << " and the bottom half has action 1." << endl;
                pRegMap1->Set( shared_ptr< Rectangle >(
                                   new Rectangle( 0, 0, sw, sh / 2 ) ), 2005 );
                pRegMap1->Set( shared_ptr< Rectangle >(
                                 new Rectangle( 0, sh / 2, sw, sh / 2 ) ), 1 );
                cout << "SetRegionMap()" << endl;
                mappedInput.SetRegionMap( pRegMap1, pDev1, 1 );
            }
        }

        timer.Reset( );
        cout << "Ready to report mapped events." << endl;
        while ( true )
        {
            input.Update( );
            int action;
            int owner;
            if ( mappedInput.CheckEvent( &action, &owner ) )
            {
                cout << "Owner=" << owner << " Action=" << action << endl;
            }
            if ( timer.Seconds( ) > 60. )
            {
                cout << "Time's up" << endl;
                break;
            }
        }

        cout << "Switching button map" << endl;
        mappedInput.SetButtonMap( pButtonMap1 );
        cout << "Switching region map(s)" << endl;
        if ( pDev1->NumButtons() > 0 )
            mappedInput.SetRegionMap( pRegMap1, pDev1, 0 );
        if ( pDev1->NumButtons() > 1 )
            mappedInput.SetRegionMap( pRegMap0, pDev1, 1 );
        
        timer.Reset( );
        cout << "Ready to report mapped events." << endl;
        while ( true )
        {
            input.Update( );
            int action;
            int owner;
            if ( mappedInput.CheckEvent( &action, &owner ) )
            {
                cout << "Owner=" << owner << " Action=" << action << endl;
            }
            if ( timer.Seconds( ) > 30. )
            {
                cout << "Time's up" << endl;
                break;
            }
        }
    }
    catch ( Exception & except )
    {
        cout << except.Description( ) << endl;
        ok = false;
    }

    if ( ok )
        cout << "MappedInput PASSED." << endl << endl;
    else
        cout << "MappedInput FAILED." << endl << endl;
    return ok;
}

#endif



//*****************************************************************************


bool 
MappedInput::RegMapItem::Matches( shared_ptr< InputDevice const > pDevice,
                                  int button, int pointer ) const
{
    return ( (m_device == pDevice) && (m_button == button)
             && ((pointer < 0) || (m_pointer == pointer)) );
}


//*****************************************************************************


namespace
{                                                                   //namespace

//=============================================================================

shared_ptr< InputDevice const > 
GetPointerDevice( )
{
    const Input & input = Input::Instance();
    for ( int i = 0; i < input.NumDevices(); ++i )
        if ( input.Device( i )->NumPointers() > 0 )
            return input.Device( i );
    return shared_ptr< InputDevice >( );
}

//=============================================================================

}                                                                   //namespace


//*****************************************************************************

}                                                      //namespace EpsilonDelta

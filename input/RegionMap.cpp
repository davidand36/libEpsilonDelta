/*
  RegionMap.cpp
  Copyright (C) 2009 David M. Anderson

  RegionMap class: Associates regions with actions.
*/


#include "RegionMap.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Circle.hpp"
#include "Ellipse.hpp"
#include "Rectangle.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
RegionMap::Set( shared_ptr< Region > pRegion, int action )
{
    for ( vector< RegAct >::iterator pRegAct = m_regActs.begin();
          pRegAct != m_regActs.end(); ++pRegAct )
        if ( *(pRegAct->m_pRegion) == *pRegion )
        {
            pRegAct->m_action = action;
            return;
        }
    RegAct regAct = { pRegion, action };
    m_regActs.push_back( regAct );
}

//-----------------------------------------------------------------------------

void 
RegionMap::Remove( const Region & region )
{
    for ( vector< RegAct >::iterator pRegAct = m_regActs.begin();
          pRegAct != m_regActs.end(); )
    {
        if ( *(pRegAct->m_pRegion) == region )
            pRegAct = m_regActs.erase( pRegAct );
        else
            ++pRegAct;
    }
}

//-----------------------------------------------------------------------------

void 
RegionMap::Reset( )
{
    m_regActs.clear( );
}

//=============================================================================

int 
RegionMap::Action( const Region & region )
{
    for ( vector< RegAct >::iterator pRegAct = m_regActs.begin();
          pRegAct != m_regActs.end(); ++pRegAct )
        if ( *(pRegAct->m_pRegion) == region )
            return pRegAct->m_action;
    return NoAction;
}

//.............................................................................

int 
RegionMap::Action( const Point2I & point )
{
    for ( vector< RegAct >::iterator pRegAct = m_regActs.begin();
          pRegAct != m_regActs.end(); ++pRegAct )
        if ( pRegAct->m_pRegion->Contains( point ) )
            return pRegAct->m_action;
    return NoAction;
}

//=============================================================================

#ifdef DEBUG

bool 
RegionMap::Test( )
{
    bool ok = true;
    cout << "Testing RegionMap" << endl;

    shared_ptr< Region > pRegion1( new Circle( Point2I( 120, 50 ), 30 ) );
    shared_ptr< Region > pRegion2( new Ellipse( Point2I( 100, 50 ), 70, 20 ) );
    shared_ptr< Region > pRegion3( new Rectangle( 100, 70, 30, 30 ) );
    shared_ptr< Region > pRegion4( new Rectangle( 20, 40, 160, 100 ) );

    Point2I pt0( 10, 10 );
    Point2I pt12( 110, 35 );
    Point2I pt24( 160, 50 );
    Point2I pt124( 110, 60 );
    Point2I pt134( 120, 75 );

    RegionMap map1;

    cout << "Set( pRegion1, 110 )" << endl;
    map1.Set( pRegion1, 110 );
    cout << "Set( pRegion4, 230 )" << endl;
    map1.Set( pRegion4, 230 );
    TESTCHECK( map1.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map1.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion4 ), 230, &ok );
    TESTCHECK( map1.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt12 ), 110, &ok );
    TESTCHECK( map1.Action( pt24 ), 230, &ok );
    TESTCHECK( map1.Action( pt124 ), 110, &ok );
    TESTCHECK( map1.Action( pt134 ), 110, &ok );
    cout << "Set( pRegion3, 210 )" << endl;
    map1.Set( pRegion3, 210 );
    TESTCHECK( map1.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map1.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion3 ), 210, &ok );
    TESTCHECK( map1.Action( *pRegion4 ), 230, &ok );
    TESTCHECK( map1.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt12 ), 110, &ok );
    TESTCHECK( map1.Action( pt24 ), 230, &ok );
    TESTCHECK( map1.Action( pt124 ), 110, &ok );
    TESTCHECK( map1.Action( pt134 ), 110, &ok );
    cout << "Remove( *pRegion4 )" << endl;
    map1.Remove( *pRegion4 );
    TESTCHECK( map1.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map1.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion3 ), 210, &ok );
    TESTCHECK( map1.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt12 ), 110, &ok );
    TESTCHECK( map1.Action( pt24 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt124 ), 110, &ok );
    TESTCHECK( map1.Action( pt134 ), 110, &ok );

    RegionMap map2;

    cout << "Set( pRegion2, 1120 )" << endl;
    map2.Set( pRegion2, 1120 );
    cout << "Set( pRegion1, 1110 )" << endl;
    map2.Set( pRegion1, 1110 );
    TESTCHECK( map2.Action( *pRegion1 ), 1110, &ok );
    TESTCHECK( map2.Action( *pRegion2 ), 1120, &ok );
    TESTCHECK( map2.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map2.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map2.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map2.Action( pt12 ), 1120, &ok );
    TESTCHECK( map2.Action( pt24 ), 1120, &ok );
    TESTCHECK( map2.Action( pt124 ), 1120, &ok );
    TESTCHECK( map2.Action( pt134 ), 1110, &ok );
    cout << "Set( pRegion2, 2120 )" << endl;
    map2.Set( pRegion2, 2120 );
    TESTCHECK( map2.Action( *pRegion1 ), 1110, &ok );
    TESTCHECK( map2.Action( *pRegion2 ), 2120, &ok );
    TESTCHECK( map2.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map2.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map2.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map2.Action( pt12 ), 2120, &ok );
    TESTCHECK( map2.Action( pt24 ), 2120, &ok );
    TESTCHECK( map2.Action( pt124 ), 2120, &ok );
    TESTCHECK( map2.Action( pt134 ), 1110, &ok );

    cout << "map3 = map1" << endl;
    RegionMap map3 = map1;
    TESTCHECK( map3.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map3.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion3 ), 210, &ok );
    TESTCHECK( map3.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt12 ), 110, &ok );
    TESTCHECK( map3.Action( pt24 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt124 ), 110, &ok );
    TESTCHECK( map3.Action( pt134 ), 110, &ok );
    cout << "Remove( *pRegion2 )" << endl;
    map3.Remove( *pRegion2 );
    TESTCHECK( map3.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map3.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion3 ), 210, &ok );
    TESTCHECK( map3.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt12 ), 110, &ok );
    TESTCHECK( map3.Action( pt24 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt124 ), 110, &ok );
    TESTCHECK( map3.Action( pt134 ), 110, &ok );
    cout << "Remove( *pRegion3 )" << endl;
    map3.Remove( *pRegion3 );
    cout << "Set( pRegion4, 2230 )" << endl;
    map3.Set( pRegion4, 2230 );
    TESTCHECK( map3.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map3.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion4 ), 2230, &ok );
    TESTCHECK( map3.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt12 ), 110, &ok );
    TESTCHECK( map3.Action( pt24 ), 2230, &ok );
    TESTCHECK( map3.Action( pt124 ), 110, &ok );
    TESTCHECK( map3.Action( pt134 ), 110, &ok );
    TESTCHECK( map1.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map1.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion3 ), 210, &ok );
    TESTCHECK( map1.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt12 ), 110, &ok );
    TESTCHECK( map1.Action( pt24 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt124 ), 110, &ok );
    TESTCHECK( map1.Action( pt134 ), 110, &ok );
    cout << "Reset( )" << endl;
    map1.Reset( );
    TESTCHECK( map1.Action( *pRegion1 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt12 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt24 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt124 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt134 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map3.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion4 ), 2230, &ok );
    TESTCHECK( map3.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt12 ), 110, &ok );
    TESTCHECK( map3.Action( pt24 ), 2230, &ok );
    TESTCHECK( map3.Action( pt124 ), 110, &ok );
    TESTCHECK( map3.Action( pt134 ), 110, &ok );
    cout << "Set( pRegion1, 110 )" << endl;
    map1.Set( pRegion1, 110 );
    cout << "Set( pRegion4, 230 )" << endl;
    map1.Set( pRegion4, 230 );
    TESTCHECK( map1.Action( *pRegion1 ), 110, &ok );
    TESTCHECK( map1.Action( *pRegion2 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( *pRegion4 ), 230, &ok );
    TESTCHECK( map1.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map1.Action( pt12 ), 110, &ok );
    TESTCHECK( map1.Action( pt24 ), 230, &ok );
    TESTCHECK( map1.Action( pt124 ), 110, &ok );
    TESTCHECK( map1.Action( pt134 ), 110, &ok );

    cout << "map3 = map2" << endl;
    map3 = map2;
    TESTCHECK( map3.Action( *pRegion1 ), 1110, &ok );
    TESTCHECK( map3.Action( *pRegion2 ), 2120, &ok );
    TESTCHECK( map3.Action( *pRegion3 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( *pRegion4 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt0 ), RegionMap::NoAction, &ok );
    TESTCHECK( map3.Action( pt12 ), 2120, &ok );
    TESTCHECK( map3.Action( pt24 ), 2120, &ok );
    TESTCHECK( map3.Action( pt124 ), 2120, &ok );
    TESTCHECK( map3.Action( pt134 ), 1110, &ok );

    if ( ok )
        cout << "RegionMap PASSED." << endl << endl;
    else
        cout << "RegionMap FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

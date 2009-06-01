/*
  Point2.cpp
  Copyright (C) 2007 David M. Anderson

  Point2I class: Two-dimensional point.
*/


#include "Point2.hpp"
#include "Assert.hpp"
#include "Surface.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Point2I::Point2I( int * pCoords )
    :   m_coords( pCoords )
{
}

//=============================================================================

void 
Point2I::Set( int * pCoords )
{
    m_coords.Set( pCoords );
}

//=============================================================================

const int & 
Point2I::At( int index ) const
{
    return  m_coords.At( index );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

int & 
Point2I::At( int index )
{
    return  m_coords.At( index );
}

//=============================================================================

void 
Point2I::Draw( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Point2I::Draw( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Point2I::Draw( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//=============================================================================

#ifdef DEBUG

bool 
Point2I::Test( )
{
    bool ok = true;
    cout << "Testing Point2I" << endl;

    int x = 10;
    int y = -20;
    cout << "Point2I( " << x << ", " << y << " )" << endl;
    Point2I pt1( x, y );
    TESTCHECK( pt1.X(), x, &ok );
    TESTCHECK( pt1.Y(), y, &ok );
    TESTCHECK( pt1[0], x, &ok );
    TESTCHECK( pt1[1], y, &ok );
    TESTCHECK( pt1.At( 0 ), x, &ok );
    TESTCHECK( pt1.At( 1 ), y, &ok );
    TESTCHECK( pt1.Vector().X(), x, &ok );
    TESTCHECK( pt1.Vector().Y(), y, &ok );
    int ints[] = { 20, 40, 80 };
    cout << "Point2I( int * )" << endl;
    Point2I pt2( ints + 1 );
    TESTCHECK( pt2.X(), 40, &ok );
    TESTCHECK( pt2.Y(), 80, &ok );
    TESTCHECK( (pt1 == pt2), false, &ok );
    TESTCHECK( (pt1 == pt1), true, &ok );
    TESTCHECK( (pt1 != pt2), true, &ok );

    int l = 0;
    int t = 10;
    int w = 100;
    int h = 100;
    cout << "Rectangle( " << l << ", " << t << ", " << w << ", " << h
         << " )" << endl;
    Rectangle rect( l, t, w, h );
    TESTCHECK( pt1.Clip( rect ), false, &ok );
    TESTCHECK( pt2.Clip( rect ), true, &ok );

    cout << "Point2I( pt1 )" << endl;
    Point2I pt3( pt1 );
    TESTCHECK( pt3.X(), x, &ok );
    TESTCHECK( pt3.Y(), y, &ok );
    x = 55;
    y = 95;
    cout << "Set( " << x << ", " << y << " )" << endl;
    pt3.Set( x, y );
    TESTCHECK( pt3.X(), x, &ok );
    TESTCHECK( pt3.Y(), y, &ok );
    x = 45;
    y = 85;
    cout << "SetX( " << x << " )" << endl;
    cout << "SetY( " << y << " )" << endl;
    pt3.SetX( x );
    pt3.SetY( y );
    TESTCHECK( pt3.X(), x, &ok );
    TESTCHECK( pt3.Y(), y, &ok );
    
    if ( ok )
        cout << "Point2I PASSED." << endl << endl;
    else
        cout << "Point2I FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void 
Point2I::TestDraw( )
{
    Color3B red( 255, 0, 0 );
    Color3B green( 0, 255, 0 );

    Point2I( 10, -20 ).Draw( red );

    Point2I( 40, 80 ).Draw( green );
    Point2I( 41, 80 ).Draw( green );
    Point2I( 40, 81 ).Draw( green );
    Point2I( 41, 81 ).Draw( green );

    Point2I( 45, 85 ).Draw( red );
    Point2I( 46, 85 ).Draw( red );
    Point2I( 45, 86 ).Draw( red );
    Point2I( 46, 86 ).Draw( red );
}

#endif  //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  Circle.cpp
  Copyright (C) 2007 David M. Anderson

  Circle class.
*/


#include "Circle.hpp"
#if USE_SDL
#include "Surface.hpp"
#endif
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

bool 
Circle::operator==( const Region & rhs ) const
{
    const Circle * pCircle = dynamic_cast< Circle const * >( &rhs );
    return ( pCircle && (*this == *pCircle) );
}

//=============================================================================

void 
Circle::Draw( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Circle::Draw( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Circle::Draw( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//-----------------------------------------------------------------------------

void 
Circle::Fill( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Fill( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Circle::Fill( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Fill( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Circle::Fill( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Fill( *this, color );
}

//=============================================================================

#ifdef DEBUG

bool 
Circle::Test( )
{
    bool ok = true;
    cout << "Testing Circle" << endl;

    int x = 300;
    int y = 325;
    int r = 25;
    cout << "Circle( Point2I( " << x << ", " << y <<" ), "<< r << " )" << endl;
    Circle circle1( Point2I( x, y ), r );
    TESTCHECK( circle1.Center().X(), 300, &ok );
    TESTCHECK( circle1.Center().Y(), 325, &ok );
    TESTCHECK( circle1.Radius(), 25, &ok );
    Rectangle bounds = circle1.Bounds();
    TESTCHECK( bounds.Left(),275, &ok );
    TESTCHECK( bounds.Top(), 300, &ok );
    TESTCHECK( bounds.Right(), 325, &ok );
    TESTCHECK( bounds.Bottom(), 350, &ok );
    TESTCHECK( bounds.Width(), 51, &ok );
    TESTCHECK( bounds.Height(), 51, &ok );
    TESTCHECK( circle1.Contains( Point2I( 300, 350 ) ), true, &ok );
    TESTCHECK( circle1.Contains( Point2I( 300, 351 ) ), false, &ok );
    TESTCHECK( circle1.Contains( Point2I( 274, 325 ) ), false, &ok );
    TESTCHECK( circle1.Contains( Point2I( 275, 325 ) ), true, &ok );
    TESTCHECK( circle1.Contains( Point2I( 313, 313 ) ), true, &ok );
    TESTCHECK( circle1.Contains( Point2I( 275, 300 ) ), false, &ok );
    
    Circle circle2 = circle1;
    x = 375;
    y = 400;
    r = 50;
    cout << "Set( Point2I( " << x << ", " << y <<" ), "<< r << " )" << endl;
    circle2.Set( Point2I( x, y ), r );
    TESTCHECK( circle2.Center().X(), 375, &ok );
    TESTCHECK( circle2.Center().Y(), 400, &ok );
    TESTCHECK( circle2.Radius(), 50, &ok );
    bounds = circle2.Bounds();
    TESTCHECK( bounds.Left(), 325, &ok );
    TESTCHECK( bounds.Top(), 350, &ok );
    TESTCHECK( bounds.Right(), 425, &ok );
    TESTCHECK( bounds.Bottom(), 450, &ok );
    TESTCHECK( bounds.Width(), 101, &ok );
    TESTCHECK( bounds.Height(),101, &ok );
    TESTCHECK( circle1 == circle2, false, &ok );
    TESTCHECK( circle2 == circle2, true, &ok );
    TESTCHECK( circle1 != circle2, true, &ok );
    TESTCHECK( circle1 != circle1, false, &ok );

    if ( ok )
        cout << "Circle PASSED." << endl << endl;
    else
        cout << "Circle FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void 
Circle::TestDraw( )
{
    Rectangle oldClip = Surface::Current()->ClippingRect();

    Color3B gray( 100, 100, 100 );
    uint32_t white = 0xFFFFFFFF;
    Color3B brown( 100, 100, 0 );
    Color3B yellow( 255, 255, 0 );
    Color3B darkblue( 0, 0, 100 );
    Color3B blue( 0, 0, 255 );

    int x = 300;
    int y = 325;
    int r = 25;
    Circle circle1( Point2I( x, y ), r );
    Circle circle2 = circle1;
    x = 375;
    y = 400;
    r = 50;
    circle2.Set( Point2I( x, y ), r );
    
    circle1.Fill( gray );
    circle1.Draw( white );
    circle2.Fill( gray );
    circle2.Draw( white );
    Surface::Current()->SetClippingRect( Rectangle( 290, 330, 50, 40 ) );
    circle1.Fill( darkblue );
    circle1.Draw( blue );
    circle2.Fill( darkblue );
    circle2.Draw( blue );
    Surface::Current()->SetClippingRect( Rectangle( 310, 340, 80, 90 ) );
    circle1.Fill( yellow );
    circle1.Draw( brown );
    circle2.Fill( yellow );
    circle2.Draw( brown );

    Surface::Current()->SetClippingRect( oldClip );
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta



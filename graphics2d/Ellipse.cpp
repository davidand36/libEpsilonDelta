/*
  Ellipse.cpp
  Copyright (C) 2007 David M. Anderson

  Ellipse class: representing an axis-aligned ellipse.
*/


#include "Ellipse.hpp"
#include "Surface.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool 
Ellipse::Contains( const Point2I & point ) const
{
    int x = point.X() - m_center.X();
    int y = point.Y() - m_center.Y();
    const int ASq = m_semiWidth * m_semiWidth;
    const int BSq = m_semiHeight * m_semiHeight;
    const int ABSq = ASq * BSq;
    return  ( BSq * x * x  +  ASq * y * y  <=  ABSq );
}

//=============================================================================

void 
Ellipse::Draw( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Ellipse::Draw( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Ellipse::Draw( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//-----------------------------------------------------------------------------

void 
Ellipse::Fill( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Fill( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Ellipse::Fill( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Fill( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Ellipse::Fill( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Fill( *this, color );
}

//=============================================================================

#ifdef DEBUG

bool 
Ellipse::Test( )
{
    bool ok = true;
    cout << "Testing Ellipse" << endl;

    int x = 100;
    int y = 350;
    int sw = 25;
    int sh = 50;
    cout << "Ellipse( Point2I( " << x << ", " << y <<" ), "<< sw << " ," << sh << " )" << endl;
    Ellipse ellipse1( Point2I( x, y ), sw, sh );
    TESTCHECK( ellipse1.Center().X(), 100, &ok );
    TESTCHECK( ellipse1.Center().Y(), 350, &ok );
    TESTCHECK( ellipse1.SemiAxes().first, 25, &ok );
    TESTCHECK( ellipse1.SemiAxes().second, 50, &ok );
    Rectangle bounds = ellipse1.Bounds();
    TESTCHECK( bounds.Left(), 75, &ok );
    TESTCHECK( bounds.Top(), 300, &ok );
    TESTCHECK( bounds.Right(), 125, &ok );
    TESTCHECK( bounds.Bottom(), 400, &ok );
    TESTCHECK( bounds.Width(), 51, &ok );
    TESTCHECK( bounds.Height(), 101, &ok );
    TESTCHECK( ellipse1.Contains( Point2I( 100, 400 ) ), true, &ok );
    TESTCHECK( ellipse1.Contains( Point2I( 100, 401 ) ), false, &ok );
    TESTCHECK( ellipse1.Contains( Point2I( 74, 350 ) ), false, &ok );
    TESTCHECK( ellipse1.Contains( Point2I( 75, 350 ) ), true, &ok );
    TESTCHECK( ellipse1.Contains( Point2I( 87, 325 ) ), true, &ok );
    TESTCHECK( ellipse1.Contains( Point2I( 75, 300 ) ), false, &ok );
    int r = 200;
    int b = 400;
    cout << "Rectangle( Point2I( " << x << ", " << y << " ), Point2I( " << r << ", " << b << " ) )" << endl;
    Rectangle rect( Point2I( x, y ), Point2I( r, b ) );
    cout << "Ellipse( rect )" << endl;
    Ellipse ellipse2( rect );
    TESTCHECK( ellipse2.Center().X(), 150, &ok );
    TESTCHECK( ellipse2.Center().Y(), 375, &ok );
    TESTCHECK( ellipse2.SemiAxes().first, 50, &ok );
    TESTCHECK( ellipse2.SemiAxes().second, 25, &ok );
    bounds = ellipse2.Bounds();
    TESTCHECK( bounds.Left(), 100, &ok );
    TESTCHECK( bounds.Top(), 350, &ok );
    TESTCHECK( bounds.Right(), 200, &ok );
    TESTCHECK( bounds.Bottom(), 400, &ok );
    TESTCHECK( bounds.Width(), 101, &ok );
    TESTCHECK( bounds.Height(), 51, &ok );
    TESTCHECK( ellipse1 == ellipse2, false, &ok );
    TESTCHECK( ellipse2 == ellipse2, true, &ok );
    TESTCHECK( ellipse1 != ellipse2, true, &ok );
    TESTCHECK( ellipse1 != ellipse1, false, &ok );

    if ( ok )
        cout << "Ellipse PASSED." << endl << endl;
    else
        cout << "Ellipse FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void 
Ellipse::TestDraw( )
{
    Rectangle oldClip = Surface::Current()->ClippingRect();

    Color3B gray( 100, 100, 100 );
    uint32_t white = 0xFFFFFFFF;
    Color3B brown( 100, 100, 0 );
    Color3B yellow( 255, 255, 0 );
    Color3B darkblue( 0, 0, 100 );
    Color3B blue( 0, 0, 255 );

    int x = 100;
    int y = 350;
    int sw = 25;
    int sh = 50;
    Ellipse ellipse1( Point2I( x, y ), sw, sh );
    int r = 200;
    int b = 400;
    Rectangle rect( Point2I( x, y ), Point2I( r, b ) );
    Ellipse ellipse2( rect );
    x = 50;
    y = 375;
    sw = 0;
    sh = 50;
    Ellipse ellipse3( Point2I( x, y ), sw, sh );
    
    ellipse1.Fill( gray );
    ellipse1.Draw( white );
    ellipse2.Fill( gray );
    ellipse2.Draw( white );
    ellipse3.Fill( gray );
    ellipse3.Draw( white );
    Surface::Current()->SetClippingRect( Rectangle( 90, 325, 80, 40 ) );
    ellipse1.Fill( yellow );
    ellipse1.Draw( brown );
    ellipse2.Fill( yellow );
    ellipse2.Draw( brown );
    ellipse3.Fill( yellow );
    ellipse3.Draw( brown );
    Surface::Current()->SetClippingRect( Rectangle( 110, 370, 100, 40 ) );
    ellipse1.Fill( darkblue );
    ellipse1.Draw( blue );
    ellipse2.Fill( darkblue );
    ellipse2.Draw( blue );
    ellipse3.Fill( darkblue );
    ellipse3.Draw( blue );

    Surface::Current()->SetClippingRect( oldClip );
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

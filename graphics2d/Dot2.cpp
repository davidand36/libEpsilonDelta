/*
  Dot2.cpp
  Copyright (C) 2007 David M. Anderson

  Dot2I class: Two-dimensional dot, i.e. graphical representation of a point.
*/


#include "Dot2.hpp"
#include "Assert.hpp"
#include "Surface.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void 
Dot2I::Draw( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Dot2I::Draw( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Dot2I::Draw( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//=============================================================================

#ifdef DEBUG

bool 
Dot2I::Test( )
{
    bool ok = true;
    cout << "Testing Dot2I" << endl;

    int x = 10;
    int y = -20;
    cout << "Dot2I( Point2I( " << x << ", " << y << " ) )" << endl;
    Dot2I dot1( Point2I( x, y ) );
    Dot2I dot2;
    cout << "Set( Point2I( 40, 80 ) )" << endl;
    dot2.Set( Point2I( 40, 80 ) );
    TESTCHECK( dot2.Point().X(), 40, &ok );
    TESTCHECK( dot2.Point().Y(), 80, &ok );

    int l = 0;
    int t = 10;
    int w = 100;
    int h = 100;
    cout << "Rectangle( " << l << ", " << t << ", " << w << ", " << h
         << " )" << endl;
    Rectangle rect( l, t, w, h );
    TESTCHECK( dot1.Clip( rect ), false, &ok );
    TESTCHECK( dot2.Clip( rect ), true, &ok );

    cout << "Dot2I( dot1 )" << endl;
    Dot2I dot3( dot1 );
    TESTCHECK( dot3.Point().X(), x, &ok );
    TESTCHECK( dot3.Point().Y(), y, &ok );
    
    if ( ok )
        cout << "Dot2I PASSED." << endl << endl;
    else
        cout << "Dot2I FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void 
Dot2I::TestDraw( )
{
    Color3B red( 255, 0, 0 );
    Color3B green( 0, 255, 0 );

    Dot2I( Point2I( 10, -20 ) ).Draw( red );

    Dot2I( Point2I( 40, 80 ) ).Draw( green );
    Dot2I( Point2I( 41, 80 ) ).Draw( green );
    Dot2I( Point2I( 40, 81 ) ).Draw( green );
    Dot2I( Point2I( 41, 81 ) ).Draw( green );

    Dot2I( Point2I( 45, 85 ) ).Draw( red );
    Dot2I( Point2I( 46, 85 ) ).Draw( red );
    Dot2I( Point2I( 45, 86 ) ).Draw( red );
    Dot2I( Point2I( 46, 86 ) ).Draw( red );
}

#endif  //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

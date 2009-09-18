/*
  Sprite.cpp
  Copyright (C) 2007 David M. Anderson

  Sprite class: A graphic element (surface) to be rendered at a specific
  position.
*/


#include "Sprite.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Circle.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta
//*****************************************************************************


bool 
Sprite::operator==( const Region & rhs ) const
{
    const Sprite * pSprite = dynamic_cast< Sprite const * >( &rhs );
    return ( pSprite && (*this == *pSprite) );
}

//=============================================================================

bool 
Sprite::Contains( const Point2I & point ) const
{
    Point2I adjPt = Translate( point, m_position );
    return m_graphic->Contains( adjPt );
}

//=============================================================================

#ifdef DEBUG

bool 
Sprite::Test( )
{
    bool ok = true;
    cout << "Testing Sprite" << endl;

    shared_ptr< Surface > pSurf( new Surface( 75, 100, PixelType8888 ) );
    pSurf->Fill( pSurf->Extent(), Color4B( 0, 0, 0, 0 ) );
    Circle( Point2I( 37, 37 ), 37 ).Fill( Color3B( 0, 0, 255 ), pSurf.get() );

    int x = 10;
    int y = -20;
    cout << "Sprite( pSurf, Point2I( " << x << ", " << y << " ) )" << endl;
    Sprite sprite( pSurf, Point2I( x, y ) );
    TESTCHECK( sprite.Position().X(), 10, &ok );
    TESTCHECK( sprite.Position().Y(), -20, &ok );
    TESTCHECK( sprite.IsVisible(), true, &ok );
    TESTCHECK( sprite.Contains( Point2I( 0, 0 ) ), false, &ok );
    TESTCHECK( sprite.Contains( Point2I( 40, 40 ) ), true, &ok );
    TESTCHECK( sprite.Contains( Point2I( 80, 120 ) ), false, &ok );
    TESTCHECK( sprite.Contains( Point2I( 50, 0 ) ), true, &ok );
    TESTCHECK( sprite.Contains( Point2I( 15, -15 ) ), false, &ok );
    cout << "Hide()" << endl;
    sprite.Hide( );
    TESTCHECK( sprite.IsVisible(), false, &ok );
    cout << "SetPosition( Point2I( 40, 80 ) )" << endl;
    sprite.SetPosition( Point2I( 40, 80 ) );
    TESTCHECK( sprite.Position().X(), 40, &ok );
    TESTCHECK( sprite.Position().Y(), 80, &ok );
    TESTCHECK( sprite.Contains( Point2I( 0, 0 ) ), false, &ok );
    TESTCHECK( sprite.Contains( Point2I( 40, 40 ) ), false, &ok );
    TESTCHECK( sprite.Contains( Point2I( 80, 120 ) ), true, &ok );
    TESTCHECK( sprite.Contains( Point2I( 50, 0 ) ), false, &ok );
    TESTCHECK( sprite.Contains( Point2I( 15, -15 ) ), false, &ok );
    TESTCHECK( sprite.IsVisible(), false, &ok );
    cout << "Show()" << endl;
    sprite.Show( );
    TESTCHECK( sprite.IsVisible(), true, &ok );

    if ( ok )
        cout << "Sprite PASSED." << endl << endl;
    else
        cout << "Sprite FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************
}                                                      //namespace EpsilonDelta

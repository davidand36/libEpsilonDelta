/*
  Sprite.cpp
  Copyright (C) 2007 David M. Anderson

  Sprite class: A graphic element (surface) to be rendered at a specific
  position.
*/


#include "Sprite.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta
//*****************************************************************************


#ifdef DEBUG

bool 
Sprite::Test( )
{
    bool ok = true;
    cout << "Testing Sprite" << endl;

    shared_ptr< Surface > pSurf;
    int x = 10;
    int y = -20;
    cout << "Sprite( pSurf, Point2I( " << x << ", " << y << " ) )" << endl;
    Sprite sprite( pSurf, Point2I( x, y ) );
    TESTCHECK( sprite.Position().X(), 10, &ok );
    TESTCHECK( sprite.Position().Y(), -20, &ok );
    cout << "SetPosition( Point2I( 40, 80 ) )" << endl;
    sprite.SetPosition( Point2I( 40, 80 ) );
    TESTCHECK( sprite.Position().X(), 40, &ok );
    TESTCHECK( sprite.Position().Y(), 80, &ok );
    
    if ( ok )
        cout << "Sprite PASSED." << endl << endl;
    else
        cout << "Sprite FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************
}                                                      //namespace EpsilonDelta

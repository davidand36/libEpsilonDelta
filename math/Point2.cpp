/*
  Point2.cpp
  Copyright (C) 2007 David M. Anderson

  Point2 template class: 2-dimensional point (Cartesian coordinates).
*/


#include "Point2.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestPoint2( )
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
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( pt1.At( 2 ), 0, &ok );
        cout << "At(2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    TESTCHECK( pt1.Array()[ 0 ], x, &ok );
    TESTCHECK( pt1.Array()[ 1 ], y, &ok );
    int ints[] = { 20, 40, 80 };
    cout << "Point2I( int * )" << endl;
    Point2I pt2( ints + 1 );
    TESTCHECK( pt2.X(), 40, &ok );
    TESTCHECK( pt2.Y(), 80, &ok );
    TESTCHECK( (pt1 == pt2), false, &ok );
    TESTCHECK( (pt1 == pt1), true, &ok );
    TESTCHECK( (pt1 != pt2), true, &ok );

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

    cout << "Point2F( pt3 )" << endl;
    Point2F ptf1( pt3 );
    TESTCHECK( ptf1.X(), 45.f, &ok );
    TESTCHECK( ptf1.Y(), 85.f, &ok );

    float xf = 7.7f;
    float yf = -5.2;
    cout << "Point2F( " << xf << ", " << yf << " )" << endl;
    Point2F ptf2( xf, yf );
    TESTCHECK( ptf2.X(), xf, &ok );
    TESTCHECK( ptf2.Y(), yf, &ok );
    cout << "Point2I( ptf2 )" << endl;
    Point2I pt4( ptf2 );
    TESTCHECK( pt4.X(), 7, &ok );   //truncates
    TESTCHECK( pt4.Y(), -5, &ok );

    int dx = 10;
    int dy = -10;
    cout << "Vector2I( " << dx << ", " << dy << " )" << endl;
    Vector2I vec1( dx, dy );
    cout << "+= vec1" << endl;
    pt3 += vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 55, 75 ]" ), &ok );
    cout << "-= vec1" << endl;
    pt3 -= vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 45, 85 ]" ), &ok );
    cout << "pt2 + vec1" << endl;
    pt3 = pt2 + vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 50, 70 ]" ), &ok );
    cout << "pt2 - vec1" << endl;
    pt3 = pt2 - vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 30, 90 ]" ), &ok );
    cout << "pt2 - pt1" << endl;
    vec1 = pt2 - pt1;
    TESTCHECK( ToJSON( vec1 ), string( "[ 30, 100 ]" ), &ok );
    
    if ( ok )
        cout << "Point2I PASSED." << endl << endl;
    else
        cout << "Point2I FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

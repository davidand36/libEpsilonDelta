/*
  Point3.cpp
  Copyright (C) 2007 David M. Anderson

  Point3 template class: 3-dimensional point (Cartesian coordinates).
*/


#include "Point3.hpp"
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
TestPoint3( )
{
    bool ok = true;
    cout << "Testing Point3I" << endl;

    int x = 10;
    int y = -20;
    int z = 0;
    cout << "Point3I( " << x << ", " << y << ", " << z << " )" << endl;
    Point3I pt1( x, y, z );
    TESTCHECK( pt1.X(), x, &ok );
    TESTCHECK( pt1.Y(), y, &ok );
    TESTCHECK( pt1.Z(), z, &ok );
    TESTCHECK( pt1[0], x, &ok );
    TESTCHECK( pt1[1], y, &ok );
    TESTCHECK( pt1[2], z, &ok );
    TESTCHECK( pt1.At( 0 ), x, &ok );
    TESTCHECK( pt1.At( 1 ), y, &ok );
    TESTCHECK( pt1.At( 2 ), z, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( pt1.At( 3 ), 0, &ok );
        cout << "At(3) should have thrown an exception." << endl;
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
    TESTCHECK( pt1.Array()[ 2 ], z, &ok );
    int ints[] = { 20, 40, 80, 100 };
    cout << "Point3I( int * )" << endl;
    Point3I pt2( ints + 1 );
    TESTCHECK( pt2.X(), 40, &ok );
    TESTCHECK( pt2.Y(), 80, &ok );
    TESTCHECK( pt2.Z(), 100, &ok );
    TESTCHECK( (pt1 == pt2), false, &ok );
    TESTCHECK( (pt1 == pt1), true, &ok );
    TESTCHECK( (pt1 != pt2), true, &ok );

    cout << "Point3I( pt1 )" << endl;
    Point3I pt3( pt1 );
    TESTCHECK( pt3.X(), x, &ok );
    TESTCHECK( pt3.Y(), y, &ok );
    TESTCHECK( pt3.Z(), z, &ok );
    x = 55;
    y = 95;
    z = -45;
    cout << "Set( " << x << ", " << y << ", " << z << " )" << endl;
    pt3.Set( x, y, z );
    TESTCHECK( pt3.X(), x, &ok );
    TESTCHECK( pt3.Y(), y, &ok );
    TESTCHECK( pt3.Z(), z, &ok );
    x = 45;
    y = 85;
    z = -5;
    cout << "SetX( " << x << " )" << endl;
    cout << "SetY( " << y << " )" << endl;
    cout << "SetZ( " << z << " )" << endl;
    pt3.SetX( x );
    pt3.SetY( y );
    pt3.SetZ( z );
    TESTCHECK( pt3.X(), x, &ok );
    TESTCHECK( pt3.Y(), y, &ok );
    TESTCHECK( pt3.Z(), z, &ok );

    int dx = 10;
    int dy = -10;
    int dz = 20;
    cout << "Vector3I( " << dx << ", " << dy << ", " << dz << " )" << endl;
    Vector3I vec1( dx, dy, dz );
    cout << "+= vec1" << endl;
    pt3 += vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 55, 75, 15 ]" ), &ok );
    cout << "-= vec1" << endl;
    pt3 -= vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 45, 85, -5 ]" ), &ok );
    cout << "pt2 + vec1" << endl;
    pt3 = pt2 + vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 50, 70, 120 ]" ), &ok );
    cout << "pt2 - vec1" << endl;
    pt3 = pt2 - vec1;
    TESTCHECK( ToJSON( pt3 ), string( "[ 30, 90, 80 ]" ), &ok );
    cout << "pt2 - pt1" << endl;
    vec1 = pt2 - pt1;
    TESTCHECK( ToJSON( vec1 ), string( "[ 30, 100, 100 ]" ), &ok );
    
    if ( ok )
        cout << "Point3I PASSED." << endl << endl;
    else
        cout << "Point3I FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

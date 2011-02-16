/*
  Vector4.cpp
  Copyright (C) 2007 David M. Anderson

  Vector4 template class: 4-dimensional vector (Cartesian coordinates).
*/


#include "Vector4.hpp"
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
TestVector4( )
{
    bool ok = true;
    cout << "Testing Vector4" << endl;

    cout << "Vector4<float>() [default constructor]" << endl;
    Vector4<float> vf0;
    cout << "Set( )" << endl;
    vf0.Set( );
    TESTCHECK( vf0.X(), 0.f, &ok );
    TESTCHECK( vf0.Y(), 0.f, &ok );
    TESTCHECK( vf0.Z(), 0.f, &ok );
    TESTCHECK( vf0.W(), 0.f, &ok );
    TESTCHECK( vf0[0], 0.f, &ok );
    TESTCHECK( vf0[1], 0.f, &ok );
    TESTCHECK( vf0[2], 0.f, &ok );
    TESTCHECK( vf0[3], 0.f, &ok );
    TESTCHECK( vf0.At(0), 0.f, &ok );
    TESTCHECK( vf0.At(1), 0.f, &ok );
    TESTCHECK( vf0.At(2), 0.f, &ok );
    TESTCHECK( vf0.At(3), 0.f, &ok );
    try
    {
        TESTCHECK( vf0.At(4), 0.f, &ok );
        cout << "At(4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assert here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( (vf0 == vf0), true, &ok );
    TESTCHECK( (vf0 == Vector4F::Zero), true, &ok );
    float xf = 37.f;
    float yf = -17.f;
    float zf = -3.f;
    float wf = 13.f;
    cout << "Set( " << xf << ", " << yf << ", " << zf << ", " << wf << " ) :"
         << endl;
    vf0.Set( xf, yf, zf, wf );
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    TESTCHECK( (vf0 == vf0), true, &ok );
    xf = 11.f;
    cout << "vf0[0] = " << xf << " [index assignment]" << endl;
    vf0[0] = xf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    yf = -11.f;
    cout << "vf0[1] = " << yf << " [index assignment]" << endl;
    vf0[1] = yf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    zf = 14.f;
    cout << "vf0[2] = " << zf << " [index assignment]" << endl;
    vf0[2] = zf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    wf = 100.f;
    cout << "vf0[3] = " << wf << " [index assignment]" << endl;
    vf0[3] = wf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    xf = 111.f;
    cout << "vf0.At(0) = " << xf << " [index assignment]" << endl;
    vf0.At(0) = xf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    yf = -111.f;
    cout << "vf0.At(1) = " << yf << " [index assignment]" << endl;
    vf0.At(1) = yf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    zf = 114.f;
    cout << "vf0.At(2) = " << zf << " [index assignment]" << endl;
    vf0.At(2) = zf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    wf = -3000.f;
    cout << "vf0.At(3) = " << wf << " [index assignment]" << endl;
    vf0.At(3) = wf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( vf0.At(3), wf, &ok );
    try
    {
        vf0.At(4) = 33.f;
        cout << "At(4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "pf = vf0.Array()" << endl;
    float * pf = vf0.Array();
    TESTCHECK( pf[0], xf, &ok );
    TESTCHECK( pf[1], yf, &ok );
    TESTCHECK( pf[2], zf, &ok );
    TESTCHECK( pf[3], wf, &ok );
    xf = 32.f;
    cout << "pf[0] = " << xf << endl;
    pf[0] = xf;
    yf = 23.f;
    cout << "pf[1] = " << yf << endl;
    pf[1] = yf;
    zf = -10.f;
    cout << "pf[2] = " << zf << endl;
    pf[2] = zf;
    wf = -29.f;
    cout << "pf[3] = " << wf << endl;
    pf[3] = wf;
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0[3], wf, &ok );
    TESTCHECKF( vf0.Length( ), 49.93996396f, &ok );
    TESTCHECK( ToJSON( vf0 ),
               string( "[ +3.20000e+01, +2.30000e+01,"
                       " -1.00000e+01, -2.90000e+01 ]" ),
               &ok );
    cout << "FromJSON( \"[ 32, 23, -10, -29 ]\", &vf0 )" << endl;
    FromJSON( "[ 32, 23, -10, -29 ]", &vf0 );
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0.W(), wf, &ok );
    cout << "Normalize()" << endl;
    vf0.Normalize( );
    TESTCHECKF( vf0.X(), 0.64076939f, &ok );
    TESTCHECKF( vf0.Y(), 0.46055299f, &ok );
    TESTCHECKF( vf0.Z(), -0.20024043f, &ok );
    TESTCHECKF( vf0.W(), -0.58069725f, &ok );
    TESTCHECKF( vf0.Length(), 1.f, &ok );
    cout << "Set( ) : " << endl;
    vf0.Set( );
    TESTCHECK( vf0.X(), 0.f, &ok );
    TESTCHECK( vf0.Y(), 0.f, &ok );
    TESTCHECK( vf0.Z(), 0.f, &ok );
    TESTCHECK( vf0.W(), 0.f, &ok );
    TESTCHECK( vf0[0], 0.f, &ok );
    TESTCHECK( vf0[1], 0.f, &ok );
    TESTCHECK( vf0[2], 0.f, &ok );
    TESTCHECK( vf0[3], 0.f, &ok );
    TESTCHECK( vf0.At(0), 0.f, &ok );
    TESTCHECK( vf0.At(1), 0.f, &ok );
    TESTCHECK( vf0.At(2), 0.f, &ok );
    TESTCHECK( vf0.At(3), 0.f, &ok );
    TESTCHECK( vf0.Length(), 0.f, &ok );
    try
    {
        cout << "Normalize()" << endl;
        vf0.Normalize( );
        cout << "Normalize() should have thrown an exception." << endl;
        ok = false;
    }
    catch( NullVectorException & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    xf = -11.f;
    yf = 4.f;
    zf = 7.f;
    wf = 100.f;
    cout << "Vector4<float>( " << xf << ", " << yf << ", " << zf << ", " << wf
         << " ) [x,y,z,w constructor]" << endl;
    Vector4<float> vf1( xf, yf, zf, wf );
    TESTCHECK( vf1.X(), xf, &ok );
    TESTCHECK( vf1.Y(), yf, &ok );
    TESTCHECK( vf1.Z(), zf, &ok );
    TESTCHECK( vf1.W(), wf, &ok );
    TESTCHECK( vf1[0], xf, &ok );
    TESTCHECK( vf1[1], yf, &ok );
    TESTCHECK( vf1[2], zf, &ok );
    TESTCHECK( vf1[3], wf, &ok );
    TESTCHECK( vf1.At(0), xf, &ok );
    TESTCHECK( vf1.At(1), yf, &ok );
    TESTCHECK( vf1.At(2), zf, &ok );
    TESTCHECK( vf1.At(3), wf, &ok );
    TESTCHECK( (vf1 == vf1), true, &ok );
    TESTCHECK( (vf0 == vf1), false, &ok );
    const float fArr0[] = { 21.f, -55.f, -1000.f, -123.f };
    cout << "Vector4<float>( { " << fArr0[0] << ", " << fArr0[1] << ", "
         << fArr0[2] << ", " << fArr0[3] << " } ) [array constructor]" << endl;
    Vector4<float> vf2( fArr0 );
    TESTCHECK( vf2.X(), fArr0[0], &ok );
    TESTCHECK( vf2.Y(), fArr0[1], &ok );
    TESTCHECK( vf2.Z(), fArr0[2], &ok );
    TESTCHECK( vf2.W(), fArr0[3], &ok );
    TESTCHECK( vf2[0], fArr0[0], &ok );
    TESTCHECK( vf2[1], fArr0[1], &ok );
    TESTCHECK( vf2[2], fArr0[2], &ok );
    TESTCHECK( vf2[3], fArr0[3], &ok );
    TESTCHECK( vf2.At(0), fArr0[0], &ok );
    TESTCHECK( vf2.At(1), fArr0[1], &ok );
    TESTCHECK( vf2.At(2), fArr0[2], &ok );
    TESTCHECK( vf2.At(3), fArr0[3], &ok );
    try
    {
        TESTCHECK( vf2.At(4), 0.f, &ok );
        cout << "At(4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    const float fArr1[] = { -12.f, 1955.f, 1729.f, -.125f };
    cout << "Set( { " << fArr1[0] << ", " << fArr1[1] << ", " << fArr1[2]
         << " } )" << endl;
    vf2.Set( fArr1 );
    TESTCHECK( vf2.X(), fArr1[0], &ok );
    TESTCHECK( vf2.Y(), fArr1[1], &ok );
    TESTCHECK( vf2.Z(), fArr1[2], &ok );
    TESTCHECK( vf2.W(), fArr1[3], &ok );
    TESTCHECK( vf2[0], fArr1[0], &ok );
    TESTCHECK( vf2[1], fArr1[1], &ok );
    TESTCHECK( vf2[2], fArr1[2], &ok );
    TESTCHECK( vf2[3], fArr1[3], &ok );
    TESTCHECK( vf2.At(0), fArr1[0], &ok );
    TESTCHECK( vf2.At(1), fArr1[1], &ok );
    TESTCHECK( vf2.At(2), fArr1[2], &ok );
    TESTCHECK( vf2.At(3), fArr1[3], &ok );
    cout << "Vector4<float>( vf2 ) [copy constructor]" << endl;
    Vector4<float> vf3( vf2 );
    TESTCHECK( (vf3 == vf2), true, &ok );
    TESTCHECK( vf3.X(), fArr1[0], &ok );
    TESTCHECK( vf3.Y(), fArr1[1], &ok );
    TESTCHECK( vf3.Z(), fArr1[2], &ok );
    TESTCHECK( vf3.W(), fArr1[3], &ok );
    cout << "= vf1 [assignment]" << endl;
    vf3 = vf1;
    TESTCHECK( (vf3 == vf2), false, &ok );
    TESTCHECK( (vf3 == vf1), true, &ok );
    TESTCHECK( vf3.X(), xf, &ok );
    TESTCHECK( vf3.Y(), yf, &ok );
    TESTCHECK( vf3.Z(), zf, &ok );
    TESTCHECK( vf3.W(), wf, &ok );
    cout << "= - vf1 [negation]" << endl;
    vf3 = - vf1;
    TESTCHECK( (vf3 == vf1), false, &ok );
    TESTCHECK( vf3.X(), -xf, &ok );
    TESTCHECK( vf3.Y(), -yf, &ok );
    TESTCHECK( vf3.Z(), -zf, &ok );
    TESTCHECK( vf3.W(), -wf, &ok );
    cout << "+= vf1" << endl;
    vf3 += vf1;
    TESTCHECK( (vf3 == vf0), true, &ok );
    TESTCHECK( vf3.X(), 0.f, &ok );
    TESTCHECK( vf3.Y(), 0.f, &ok );
    TESTCHECK( vf3.Z(), 0.f, &ok );
    TESTCHECK( vf3.W(), 0.f, &ok );
    cout << "-= vf1" << endl;
    vf3 -= vf1;
    TESTCHECK( (vf3 == -vf1), true, &ok );
    TESTCHECK( vf3.X(), -xf, &ok );
    TESTCHECK( vf3.Y(), -yf, &ok );
    TESTCHECK( vf3.Z(), -zf, &ok );
    TESTCHECK( vf3.W(), -wf, &ok );
    cout << "vf1 + vf3" << endl;
    vf2 = vf1 + vf3;
    TESTCHECK( (vf2 == vf0), true, &ok );
    TESTCHECK( vf2.X(), 0.f, &ok );
    TESTCHECK( vf2.Y(), 0.f, &ok );
    TESTCHECK( vf2.Z(), 0.f, &ok );
    TESTCHECK( vf2.W(), 0.f, &ok );
    cout << "vf2 - vf1" << endl;
    vf2 = vf2 - vf1;
    TESTCHECK( (vf2 == vf3), true, &ok );
    TESTCHECK( vf2.X(), -xf, &ok );
    TESTCHECK( vf2.Y(), -yf, &ok );
    TESTCHECK( vf2.Z(), -zf, &ok );
    TESTCHECK( vf2.W(), -wf, &ok );
    float af = -1;
    cout << "*= " << af << endl;
    vf2 *= af;
    TESTCHECK( (vf2 == vf1), true, &ok );
    TESTCHECK( vf2.X(), xf, &ok );
    TESTCHECK( vf2.Y(), yf, &ok );
    TESTCHECK( vf2.Z(), zf, &ok );
    TESTCHECK( vf2.W(), wf, &ok );
    af = 8.f;
    cout << "*= " << af << endl;
    vf2 *= af;
    TESTCHECK( (vf2 == vf1), false, &ok );
    TESTCHECK( vf2.X(), -88.f, &ok );
    TESTCHECK( vf2.Y(), 32.f, &ok );
    TESTCHECK( vf2.Z(), 56.f, &ok );
    TESTCHECK( vf2.W(), 800.f, &ok );
    af = -2.f;
    cout << "vf2 * " << af << endl;
    vf2 = vf2 * af;
    TESTCHECK( vf2.X(), 176.f, &ok );
    TESTCHECK( vf2.Y(), -64.f, &ok );
    TESTCHECK( vf2.Z(), -112.f, &ok );
    TESTCHECK( vf2.W(), -1600.f, &ok );
    af = - 1.f/16.f;
    cout << af << "* vf2" << endl;
    vf2 = af * vf2;
    TESTCHECK( vf2.X(), xf, &ok );
    TESTCHECK( vf2.Y(), yf, &ok );
    TESTCHECK( vf2.Z(), zf, &ok );
    TESTCHECK( vf2.W(), wf, &ok );
    TESTCHECK( vf0 * vf1, 0.f, &ok );
    TESTCHECK( vf1 * vf1, 10186.f, &ok );
    TESTCHECK( vf2 * vf3, -10186.f, &ok );
    xf = 23.f;
    yf = -47.f;
    zf = 2.f;
    cout << "Vector3F( " << xf << ", " << yf << ", " << zf << " )" << endl;
    Vector3F v3f1( xf, yf, zf );
    cout << "Vector4F( v3f1 ) [Vector3 constructor]" << endl;
    Vector4F vf4( v3f1 );
    TESTCHECK( vf4.X(), xf, &ok );
    TESTCHECK( vf4.Y(), yf, &ok );
    TESTCHECK( vf4.Z(), zf, &ok );
    TESTCHECK( vf4.W(), 0.f, &ok );
    xf = -11.f;
    yf = 22.f;
    zf = -3.f;
    cout << "Point3F( " << xf << ", " << yf << ", " << zf << " )" << endl;
    Point3F p3f( xf, yf, zf );
    cout << "Set( p3f )" << endl;
    vf4.Set( p3f );
    TESTCHECK( vf4.X(), xf, &ok );
    TESTCHECK( vf4.Y(), yf, &ok );
    TESTCHECK( vf4.Z(), zf, &ok );
    TESTCHECK( vf4.W(), 1.f, &ok );
    xf = 37.f;
    yf = -17.f;
    zf = -3.f;
    wf = 13.f;
    cout << "Set( " << xf << ", " << yf << ", " << zf << ", " << wf << " ) :"
         << endl;
    vf4.Set( xf, yf, zf, wf );
    TESTCHECK( vf4.X(), xf, &ok );
    TESTCHECK( vf4.Y(), yf, &ok );
    TESTCHECK( vf4.Z(), zf, &ok );
    TESTCHECK( vf4.W(), wf, &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << vf4;
    TESTCHECK( ost.str(), string( "[ 37, -17, -3, 13 ]" ), &ok );
    cout << "Set( p3f )" << endl;
    vf4.Set( p3f );
    TESTCHECK( vf4.X(), p3f[0], &ok );
    TESTCHECK( vf4.Y(), p3f[1], &ok );
    TESTCHECK( vf4.Z(), p3f[2], &ok );
    TESTCHECK( vf4.W(), 1.f, &ok );
    cout << "Homogenize( )" << endl;
    vf1.Homogenize( );
    TESTCHECKF( vf1.X(), -0.11f, &ok );
    TESTCHECKF( vf1.Y(), 0.04f, &ok );
    TESTCHECKF( vf1.Z(), 0.07f, &ok );
    TESTCHECK( vf1.W(), 1.f, &ok );
    cout << "Project3( )" << endl;
    v3f1 = vf1.Project3( );
    TESTCHECKF( v3f1.X(), -0.11f, &ok );
    TESTCHECKF( v3f1.Y(), 0.04f, &ok );
    TESTCHECKF( v3f1.Z(), 0.07f, &ok );

    cout << "Vector4<long> vl1( -352, 128, 224, 1000000 )" << endl;
    Vector4<long> vl1( -352, 128, 224, 1000000 );
    cout << "Vector4F( vl1 )" << endl;
    Vector4F vf5( vl1 );
    TESTCHECK( vf5.X(), -352.f, &ok );
    TESTCHECK( vf5.Y(), 128.f, &ok );
    TESTCHECK( vf5.Z(), 224.f, &ok );
    TESTCHECK( vf5.W(), 1000000.f, &ok );

    xf = 7.7f;
    yf = -5.2;
    zf = -3.6f;
    wf = 10.0f;
    cout << "Vector4F( " << xf << ", " << yf << ", " << zf << ", " << wf
         << " )" << endl;
    Vector4F vf6( xf, yf, zf, wf );
    TESTCHECK( vf6.X(), xf, &ok );
    TESTCHECK( vf6.Y(), yf, &ok );
    TESTCHECK( vf6.Z(), zf, &ok );
    TESTCHECK( vf6.W(), wf, &ok );
    cout << "Vector3<long>( vf6 )" << endl;
    Vector4<long> vl2( vf6 );
    TESTCHECK( vl2.X(), 7L, &ok );
    TESTCHECK( vl2.Y(), -5L, &ok );
    TESTCHECK( vl2.Z(), -3L, &ok );
    TESTCHECK( vl2.W(), 10L, &ok );
    cout << "Round()" << endl;
    Vector4F vf7 = Round( vf6 );
    TESTCHECK( vf7.X(), 8.f, &ok );
    TESTCHECK( vf7.Y(), -5.f, &ok );
    TESTCHECK( vf7.Z(), -4.f, &ok );
    TESTCHECK( vf7.W(), 10.f, &ok );
    cout << "Vector4<short>( vf7 )" << endl;
    Vector4<long> vl3( vf7 );
    TESTCHECK( vl3.X(), 8L, &ok );
    TESTCHECK( vl3.Y(), -5L, &ok );
    TESTCHECK( vl3.Z(), -4L, &ok );
    TESTCHECK( vl3.W(), 10L, &ok );

    TESTCHECK( Vector4F::UnitX.X(), 1.f, &ok );
    TESTCHECK( Vector4F::UnitX.Y(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitX.Z(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitX.W(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitY.X(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitY.Y(), 1.f, &ok );
    TESTCHECK( Vector4F::UnitY.Z(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitY.W(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitZ.X(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitZ.Y(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitZ.Z(), 1.f, &ok );
    TESTCHECK( Vector4F::UnitZ.W(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitW.X(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitW.Y(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitW.Z(), 0.f, &ok );
    TESTCHECK( Vector4F::UnitW.W(), 1.f, &ok );

    if ( ok )
        cout << "Vector4 PASSED." << endl << endl;
    else
        cout << "Vector4 FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

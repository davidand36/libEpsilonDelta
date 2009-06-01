/*
  Vector3.cpp
  Copyright (C) 2007 David M. Anderson

  Vector3 template class: 3-dimensional vector (Cartesian coordinates).
*/


#include "Vector3.hpp"
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
TestVector3( )
{
    bool ok = true;
    cout << "Testing Vector3" << endl;

    cout << "Vector3<short>() [default constructor]" << endl;
    Vector3<short> vs0;
    cout << "Set( )" << endl;
    vs0.Set( );
    TESTCHECK( vs0.X(), 0, &ok );
    TESTCHECK( vs0.Y(), 0, &ok );
    TESTCHECK( vs0.Z(), 0, &ok );
    TESTCHECK( vs0[0], 0, &ok );
    TESTCHECK( vs0[1], 0, &ok );
    TESTCHECK( vs0[2], 0, &ok );
    TESTCHECK( vs0.At(0), 0, &ok );
    TESTCHECK( vs0.At(1), 0, &ok );
    TESTCHECK( vs0.At(2), 0, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( vs0.At(3), 0, &ok );
        cout << "At(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    TESTCHECK( vs0.Array()[0], 0, &ok );
    TESTCHECK( vs0.Array()[1], 0, &ok );
    TESTCHECK( vs0.Array()[2], 0, &ok );
    TESTCHECK( (vs0 == vs0), true, &ok );
    TESTCHECK( (vs0 == Vector3<short>::Zero), true, &ok );
    short xs = 37;
    short ys = -17;
    short zs = -3;
    cout << "Set( " << xs << ", " << ys << ", " << zs << " ) :" << endl;
    vs0.Set( xs, ys, zs );
    TESTCHECK( vs0.X(), xs, &ok );
    TESTCHECK( vs0.Y(), ys, &ok );
    TESTCHECK( vs0.Z(), zs, &ok );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( vs0.At(0), xs, &ok );
    TESTCHECK( vs0.At(1), ys, &ok );
    TESTCHECK( vs0.At(2), zs, &ok );
    TESTCHECK( (vs0 == vs0), true, &ok );
    xs = 11;
    cout << "vs0[0] = " << xs << " [index assignment]" << endl;
    vs0[0] = xs;
    TESTCHECK( vs0.X(), xs, &ok );
    TESTCHECK( vs0.Y(), ys, &ok );
    TESTCHECK( vs0.Z(), zs, &ok );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( vs0.At(0), xs, &ok );
    TESTCHECK( vs0.At(1), ys, &ok );
    TESTCHECK( vs0.At(2), zs, &ok );
    ys = -11;
    cout << "vs0[1] = " << ys << " [index assignment]" << endl;
    vs0[1] = ys;
    TESTCHECK( vs0.X(), xs, &ok );
    TESTCHECK( vs0.Y(), ys, &ok );
    TESTCHECK( vs0.Z(), zs, &ok );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( vs0.At(0), xs, &ok );
    TESTCHECK( vs0.At(1), ys, &ok );
    TESTCHECK( vs0.At(2), zs, &ok );
    zs = 14;
    cout << "vs0[2] = " << zs << " [index assignment]" << endl;
    vs0[2] = zs;
    TESTCHECK( vs0.X(), xs, &ok );
    TESTCHECK( vs0.Y(), ys, &ok );
    TESTCHECK( vs0.Z(), zs, &ok );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( vs0.At(0), xs, &ok );
    TESTCHECK( vs0.At(1), ys, &ok );
    TESTCHECK( vs0.At(2), zs, &ok );
    xs = 111;
    cout << "vs0.At(0) = " << xs << " [index assignment]" << endl;
    vs0.At(0) = xs;
    TESTCHECK( vs0.X(), xs, &ok );
    TESTCHECK( vs0.Y(), ys, &ok );
    TESTCHECK( vs0.Z(), zs, &ok );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( vs0.At(0), xs, &ok );
    TESTCHECK( vs0.At(1), ys, &ok );
    TESTCHECK( vs0.At(2), zs, &ok );
    ys = -111;
    cout << "vs0.At(1) = " << ys << " [index assignment]" << endl;
    vs0.At(1) = ys;
    TESTCHECK( vs0.X(), xs, &ok );
    TESTCHECK( vs0.Y(), ys, &ok );
    TESTCHECK( vs0.Z(), zs, &ok );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( vs0.At(0), xs, &ok );
    TESTCHECK( vs0.At(1), ys, &ok );
    TESTCHECK( vs0.At(2), zs, &ok );
    zs = 114;
    cout << "vs0.At(2) = " << zs << " [index assignment]" << endl;
    vs0.At(2) = zs;
    TESTCHECK( vs0.X(), xs, &ok );
    TESTCHECK( vs0.Y(), ys, &ok );
    TESTCHECK( vs0.Z(), zs, &ok );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( vs0.At(0), xs, &ok );
    TESTCHECK( vs0.At(1), ys, &ok );
    TESTCHECK( vs0.At(2), zs, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        vs0.At(3) = 33;
        cout << "At(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "ps = vs0.Array()" << endl;
    short * ps = vs0.Array();
    TESTCHECK( ps[0], xs, &ok );
    TESTCHECK( ps[1], ys, &ok );
    xs = 32;
    cout << "ps[0] = " << xs << endl;
    ps[0] = xs;
    ys = 23;
    cout << "ps[1] = " << ys << endl;
    ps[1] = ys;
    zs = 434;
    cout << "ps[2] = " << zs << endl;
    ps[2] = zs;
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    TESTCHECK( ToJSON( vs0 ), string( "[ 32, 23, 434 ]" ), &ok );
    cout << "FromJSON( \"[ 32, 23, 434 ]\", &vs0 )" << endl;
    FromJSON( "[ 32, 23, 434 ]", &vs0 );
    TESTCHECK( vs0[0], xs, &ok );
    TESTCHECK( vs0[1], ys, &ok );
    TESTCHECK( vs0[2], zs, &ok );
    cout << "Set( ) : " << endl;
    vs0.Set( );
    TESTCHECK( vs0.X(), 0, &ok );
    TESTCHECK( vs0.Y(), 0, &ok );
    TESTCHECK( vs0.Z(), 0, &ok );
    TESTCHECK( vs0[0], 0, &ok );
    TESTCHECK( vs0[1], 0, &ok );
    TESTCHECK( vs0[2], 0, &ok );
    TESTCHECK( vs0.At(0), 0, &ok );
    TESTCHECK( vs0.At(1), 0, &ok );
    TESTCHECK( vs0.At(2), 0, &ok );
    xs = -11;
    ys = 4;
    zs = 7;
    cout << "Vector3<short>( " << xs << ", " << ys << ", " << zs << " ) [x,y,z constructor]" << endl;
    Vector3<short> vs1( xs, ys, zs );
    TESTCHECK( vs1.X(), xs, &ok );
    TESTCHECK( vs1.Y(), ys, &ok );
    TESTCHECK( vs1.Z(), zs, &ok );
    TESTCHECK( vs1[0], xs, &ok );
    TESTCHECK( vs1[1], ys, &ok );
    TESTCHECK( vs1[2], zs, &ok );
    TESTCHECK( vs1.At(0), xs, &ok );
    TESTCHECK( vs1.At(1), ys, &ok );
    TESTCHECK( vs1.At(2), zs, &ok );
    TESTCHECK( (vs1 == vs1), true, &ok );
    TESTCHECK( (vs0 == vs1), false, &ok );
    const short sArr0[] = { 21, -55, -1000 };
    cout << "Vector3<short>( { " << sArr0[0] << ", " << sArr0[1] << ", " << sArr0[2] << " } ) [array constructor]" << endl;
    Vector3<short> vs2( sArr0 );
    TESTCHECK( vs2.X(), sArr0[0], &ok );
    TESTCHECK( vs2.Y(), sArr0[1], &ok );
    TESTCHECK( vs2.Z(), sArr0[2], &ok );
    TESTCHECK( vs2[0], sArr0[0], &ok );
    TESTCHECK( vs2[1], sArr0[1], &ok );
    TESTCHECK( vs2[2], sArr0[2], &ok );
    TESTCHECK( vs2.At(0), sArr0[0], &ok );
    TESTCHECK( vs2.At(1), sArr0[1], &ok );
    TESTCHECK( vs2.At(2), sArr0[2], &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( vs2.At(3), 0, &ok );
        cout << "At(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    const short sArr1[] = { -12, 1955, 1729 };
    cout << "Set( { " << sArr1[0] << ", " << sArr1[1] << ", " << sArr1[2] << " } )" << endl;
    vs2.Set( sArr1 );
    TESTCHECK( vs2.X(), sArr1[0], &ok );
    TESTCHECK( vs2.Y(), sArr1[1], &ok );
    TESTCHECK( vs2.Z(), sArr1[2], &ok );
    TESTCHECK( vs2[0], sArr1[0], &ok );
    TESTCHECK( vs2[1], sArr1[1], &ok );
    TESTCHECK( vs2[2], sArr1[2], &ok );
    TESTCHECK( vs2.At(0), sArr1[0], &ok );
    TESTCHECK( vs2.At(1), sArr1[1], &ok );
    TESTCHECK( vs2.At(2), sArr1[2], &ok );
    cout << "Vector3<short>( vs2 ) [copy constructor]" << endl;
    Vector3<short> vs3( vs2 );
    TESTCHECK( (vs3 == vs2), true, &ok );
    TESTCHECK( vs3.X(), sArr1[0], &ok );
    TESTCHECK( vs3.Y(), sArr1[1], &ok );
    TESTCHECK( vs3.Z(), sArr1[2], &ok );
    cout << "= vs1 [assignment]" << endl;
    vs3 = vs1;
    TESTCHECK( (vs3 == vs2), false, &ok );
    TESTCHECK( (vs3 == vs1), true, &ok );
    TESTCHECK( vs3.X(), xs, &ok );
    TESTCHECK( vs3.Y(), ys, &ok );
    TESTCHECK( vs3.Z(), zs, &ok );
    cout << "= - vs1 [negation]" << endl;
    vs3 = - vs1;
    TESTCHECK( (vs3 == vs1), false, &ok );
    TESTCHECK( vs3.X(), -xs, &ok );
    TESTCHECK( vs3.Y(), -ys, &ok );
    TESTCHECK( vs3.Z(), -zs, &ok );
    cout << "+= vs1" << endl;
    vs3 += vs1;
    TESTCHECK( (vs3 == vs0), true, &ok );
    TESTCHECK( vs3.X(), 0, &ok );
    TESTCHECK( vs3.Y(), 0, &ok );
    TESTCHECK( vs3.Z(), 0, &ok );
    cout << "-= vs1" << endl;
    vs3 -= vs1;
    TESTCHECK( (vs3 == -vs1), true, &ok );
    TESTCHECK( vs3.X(), -xs, &ok );
    TESTCHECK( vs3.Y(), -ys, &ok );
    TESTCHECK( vs3.Z(), -zs, &ok );
    cout << "vs1 + vs3" << endl;
    vs2 = vs1 + vs3;
    TESTCHECK( (vs2 == vs0), true, &ok );
    TESTCHECK( vs2.X(), 0, &ok );
    TESTCHECK( vs2.Y(), 0, &ok );
    TESTCHECK( vs2.Z(), 0, &ok );
    cout << "vs2 - vs1" << endl;
    vs2 = vs2 - vs1;
    TESTCHECK( (vs2 == vs3), true, &ok );
    TESTCHECK( vs2.X(), -xs, &ok );
    TESTCHECK( vs2.Y(), -ys, &ok );
    TESTCHECK( vs2.Z(), -zs, &ok );
    short as = -1;
    cout << "*= " << as << endl;
    vs2 *= as;
    TESTCHECK( (vs2 == vs1), true, &ok );
    TESTCHECK( vs2.X(), xs, &ok );
    TESTCHECK( vs2.Y(), ys, &ok );
    TESTCHECK( vs2.Z(), zs, &ok );
    as = 8;
    cout << "*= " << as << endl;
    vs2 *= as;
    TESTCHECK( (vs2 == vs1), false, &ok );
    TESTCHECK( vs2.X(), -88, &ok );
    TESTCHECK( vs2.Y(), 32, &ok );
    TESTCHECK( vs2.Z(), 56, &ok );
    as = -2;
    cout << "vs2 * " << as << endl;
    vs2 = vs2 * as;
    TESTCHECK( vs2.X(), 176, &ok );
    TESTCHECK( vs2.Y(), -64, &ok );
    TESTCHECK( vs2.Z(), -112, &ok );
    cout << as << "* vs2" << endl;
    vs2 = as * vs2;
    TESTCHECK( vs2.X(), -352, &ok );
    TESTCHECK( vs2.Y(), 128, &ok );
    TESTCHECK( vs2.Z(), 224, &ok );
    TESTCHECK( vs0 * vs1, 0, &ok );
    TESTCHECK( vs1 * vs1, 186, &ok );
    TESTCHECK( vs2 * vs3, -5952, &ok );

    cout << "Vector3<float>() [default constructor]" << endl;
    Vector3<float> vf0;
    cout << "Set( )" << endl;
    vf0.Set( );
    TESTCHECK( vf0.X(), 0.f, &ok );
    TESTCHECK( vf0.Y(), 0.f, &ok );
    TESTCHECK( vf0.Z(), 0.f, &ok );
    TESTCHECK( vf0[0], 0.f, &ok );
    TESTCHECK( vf0[1], 0.f, &ok );
    TESTCHECK( vf0[2], 0.f, &ok );
    TESTCHECK( vf0.At(0), 0.f, &ok );
    TESTCHECK( vf0.At(1), 0.f, &ok );
    TESTCHECK( vf0.At(2), 0.f, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( vf0.At(3), 0.f, &ok );
        cout << "At(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assert here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    TESTCHECK( (vf0 == vf0), true, &ok );
    float xf = 37.f;
    float yf = -17.f;
    float zf = -3.f;
    cout << "Set( " << xf << ", " << yf << ", " << zf << " ) :" << endl;
    vf0.Set( xf, yf, zf );
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    TESTCHECK( (vf0 == vf0), true, &ok );
    xf = 11.f;
    cout << "vf0[0] = " << xf << " [index assignment]" << endl;
    vf0[0] = xf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    yf = -11.f;
    cout << "vf0[1] = " << yf << " [index assignment]" << endl;
    vf0[1] = yf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    zf = 14.f;
    cout << "vf0[2] = " << zf << " [index assignment]" << endl;
    vf0[2] = zf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    xf = 111.f;
    cout << "vf0.At(0) = " << xf << " [index assignment]" << endl;
    vf0.At(0) = xf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    yf = -111.f;
    cout << "vf0.At(1) = " << yf << " [index assignment]" << endl;
    vf0.At(1) = yf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
    zf = 114.f;
    cout << "vf0.At(2) = " << zf << " [index assignment]" << endl;
    vf0.At(2) = zf;
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    TESTCHECK( vf0.Z(), zf, &ok );
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECK( vf0.At(0), xf, &ok );
    TESTCHECK( vf0.At(1), yf, &ok );
    TESTCHECK( vf0.At(2), zf, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        vf0.At(3) = 33.f;
        cout << "At(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "pf = vf0.Array()" << endl;
    float * pf = vf0.Array();
    TESTCHECK( pf[0], xf, &ok );
    TESTCHECK( pf[1], yf, &ok );
    TESTCHECK( pf[2], zf, &ok );
    TESTCHECK( ToJSON( vf0 ),
               string( "[ +1.11000e+02, -1.11000e+02, +1.14000e+02 ]" ), &ok );
    cout << "FromJSON( \"[ 111, -111, 114 ]\", &vf0 )" << endl;
    FromJSON( "[ 111, -111, 114 ]", &vf0 );
    TESTCHECK( vf0.X(), xf, &ok );
    TESTCHECK( vf0.Y(), yf, &ok );
    xf = 32.f;
    cout << "pf[0] = " << xf << endl;
    pf[0] = xf;
    yf = 23.f;
    cout << "pf[1] = " << yf << endl;
    pf[1] = yf;
    zf = -10.f;
    cout << "pf[2] = " << zf << endl;
    pf[2] = zf;
    TESTCHECK( vf0[0], xf, &ok );
    TESTCHECK( vf0[1], yf, &ok );
    TESTCHECK( vf0[2], zf, &ok );
    TESTCHECKF( vf0.Length( ), 40.6571027f, &ok );
    cout << "Normalize()" << endl;
    vf0.Normalize( );
    TESTCHECKF( vf0.X(), 0.78707035f, &ok );
    TESTCHECKF( vf0.Y(), 0.56570681f, &ok );
    TESTCHECKF( vf0.Z(), -0.24595948f, &ok );
    TESTCHECKF( vf0.Length(), 1.f, &ok );
    cout << "Set( ) : " << endl;
    vf0.Set( );
    TESTCHECK( vf0.X(), 0.f, &ok );
    TESTCHECK( vf0.Y(), 0.f, &ok );
    TESTCHECK( vf0.Z(), 0.f, &ok );
    TESTCHECK( vf0[0], 0.f, &ok );
    TESTCHECK( vf0[1], 0.f, &ok );
    TESTCHECK( vf0[2], 0.f, &ok );
    TESTCHECK( vf0.At(0), 0.f, &ok );
    TESTCHECK( vf0.At(1), 0.f, &ok );
    TESTCHECK( vf0.At(2), 0.f, &ok );
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
    cout << "Vector3<float>( " << xf << ", " << yf << ", " << zf << " ) [x,y,z constructor]" << endl;
    Vector3<float> vf1( xf, yf, zf );
    TESTCHECK( vf1.X(), xf, &ok );
    TESTCHECK( vf1.Y(), yf, &ok );
    TESTCHECK( vf1.Z(), zf, &ok );
    TESTCHECK( vf1[0], xf, &ok );
    TESTCHECK( vf1[1], yf, &ok );
    TESTCHECK( vf1[2], zf, &ok );
    TESTCHECK( vf1.At(0), xf, &ok );
    TESTCHECK( vf1.At(1), yf, &ok );
    TESTCHECK( vf1.At(2), zf, &ok );
    TESTCHECK( (vf1 == vf1), true, &ok );
    TESTCHECK( (vf0 == vf1), false, &ok );
    const float fArr0[] = { 21.f, -55.f, -1000.f };
    cout << "Vector3<float>( { " << fArr0[0] << ", " << fArr0[1] << ", " << fArr0[2] << " } ) [array constructor]" << endl;
    Vector3<float> vf2( fArr0 );
    TESTCHECK( vf2.X(), fArr0[0], &ok );
    TESTCHECK( vf2.Y(), fArr0[1], &ok );
    TESTCHECK( vf2.Z(), fArr0[2], &ok );
    TESTCHECK( vf2[0], fArr0[0], &ok );
    TESTCHECK( vf2[1], fArr0[1], &ok );
    TESTCHECK( vf2[2], fArr0[2], &ok );
    TESTCHECK( vf2.At(0), fArr0[0], &ok );
    TESTCHECK( vf2.At(1), fArr0[1], &ok );
    TESTCHECK( vf2.At(2), fArr0[2], &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( vf2.At(3), 0.f, &ok );
        cout << "At(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    const float fArr1[] = { -1.f, 5.f, 10.f };
    cout << "Set( { " << fArr1[0] << ", " << fArr1[1] << ", " << fArr1[2] << " } )" << endl;
    vf2.Set( fArr1 );
    TESTCHECK( vf2.X(), fArr1[0], &ok );
    TESTCHECK( vf2.Y(), fArr1[1], &ok );
    TESTCHECK( vf2.Z(), fArr1[2], &ok );
    TESTCHECK( vf2[0], fArr1[0], &ok );
    TESTCHECK( vf2[1], fArr1[1], &ok );
    TESTCHECK( vf2[2], fArr1[2], &ok );
    TESTCHECK( vf2.At(0), fArr1[0], &ok );
    TESTCHECK( vf2.At(1), fArr1[1], &ok );
    TESTCHECK( vf2.At(2), fArr1[2], &ok );
    cout << "Vector3<float>( vf2 ) [copy constructor]" << endl;
    Vector3<float> vf3( vf2 );
    TESTCHECK( (vf3 == vf2), true, &ok );
    TESTCHECK( vf3.X(), fArr1[0], &ok );
    TESTCHECK( vf3.Y(), fArr1[1], &ok );
    TESTCHECK( vf3.Z(), fArr1[2], &ok );
    cout << "= vf1 [assignment]" << endl;
    vf3 = vf1;
    TESTCHECK( (vf3 == vf2), false, &ok );
    TESTCHECK( (vf3 == vf1), true, &ok );
    TESTCHECK( vf3.X(), xf, &ok );
    TESTCHECK( vf3.Y(), yf, &ok );
    TESTCHECK( vf3.Z(), zf, &ok );
    cout << "= - vf1 [negation]" << endl;
    vf3 = - vf1;
    TESTCHECK( (vf3 == vf1), false, &ok );
    TESTCHECK( vf3.X(), -xf, &ok );
    TESTCHECK( vf3.Y(), -yf, &ok );
    TESTCHECK( vf3.Z(), -zf, &ok );
    cout << "+= vf1" << endl;
    vf3 += vf1;
    TESTCHECK( (vf3 == vf0), true, &ok );
    TESTCHECK( vf3.X(), 0.f, &ok );
    TESTCHECK( vf3.Y(), 0.f, &ok );
    TESTCHECK( vf3.Z(), 0.f, &ok );
    cout << "-= vf1" << endl;
    vf3 -= vf1;
    TESTCHECK( (vf3 == -vf1), true, &ok );
    TESTCHECK( vf3.X(), -xf, &ok );
    TESTCHECK( vf3.Y(), -yf, &ok );
    TESTCHECK( vf3.Z(), -zf, &ok );
    cout << "vf1 + vf3" << endl;
    vf2 = vf1 + vf3;
    TESTCHECK( (vf2 == vf0), true, &ok );
    TESTCHECK( vf2.X(), 0.f, &ok );
    TESTCHECK( vf2.Y(), 0.f, &ok );
    TESTCHECK( vf2.Z(), 0.f, &ok );
    cout << "vf2 - vf1" << endl;
    vf2 = vf2 - vf1;
    TESTCHECK( (vf2 == vf3), true, &ok );
    TESTCHECK( vf2.X(), -xf, &ok );
    TESTCHECK( vf2.Y(), -yf, &ok );
    TESTCHECK( vf2.Z(), -zf, &ok );
    float af = -1;
    cout << "*= " << af << endl;
    vf2 *= af;
    TESTCHECK( (vf2 == vf1), true, &ok );
    TESTCHECK( vf2.X(), xf, &ok );
    TESTCHECK( vf2.Y(), yf, &ok );
    TESTCHECK( vf2.Z(), zf, &ok );
    af = 8.f;
    cout << "*= " << af << endl;
    vf2 *= af;
    TESTCHECK( (vf2 == vf1), false, &ok );
    TESTCHECK( vf2.X(), -88.f, &ok );
    TESTCHECK( vf2.Y(), 32.f, &ok );
    TESTCHECK( vf2.Z(), 56.f, &ok );
    af = -2.f;
    cout << "vf2 * " << af << endl;
    vf2 = vf2 * af;
    TESTCHECK( vf2.X(), 176.f, &ok );
    TESTCHECK( vf2.Y(), -64.f, &ok );
    TESTCHECK( vf2.Z(), -112.f, &ok );
    af = - 1.f/16.f;
    cout << af << "* vf2" << endl;
    vf2 = af * vf2;
    TESTCHECK( vf2.X(), xf, &ok );
    TESTCHECK( vf2.Y(), yf, &ok );
    TESTCHECK( vf2.Z(), zf, &ok );
    TESTCHECK( vf0 * vf1, 0.f, &ok );
    TESTCHECK( vf1 * vf1, 186.f, &ok );
    TESTCHECK( vf2 * vf3, -186.f, &ok );
    cout << "Cross( vf1, vf1 )" << endl;
    vf0 = Cross( vf1, vf1 );
    TESTCHECK( vf0.X(), 0.f, &ok );
    TESTCHECK( vf0.Y(), 0.f, &ok );
    TESTCHECK( vf0.Z(), 0.f, &ok );
    cout << "Cross( vf2, vf3 )" << endl;
    vf0 = Cross( vf2, vf3 );
    TESTCHECK( vf0.X(), 0.f, &ok );
    TESTCHECK( vf0.Y(), 0.f, &ok );
    TESTCHECK( vf0.Z(), 0.f, &ok );
    cout << "Set( { " << fArr1[0] << ", " << fArr1[1] << ", " << fArr1[2] << " } )" << endl;
    vf2.Set( fArr1 );
    TESTCHECK( vf2.X(), fArr1[0], &ok );
    TESTCHECK( vf2.Y(), fArr1[1], &ok );
    TESTCHECK( vf2.Z(), fArr1[2], &ok );
    cout << "Cross( vf1, vf2 )" << endl;
    vf2 = Cross( vf1, vf2 );
    TESTCHECK( vf2.X(), 5.f, &ok );
    TESTCHECK( vf2.Y(), 103.f, &ok );
    TESTCHECK( vf2.Z(), -51.f, &ok );
    float m00 = 0.f;
    float m01 = 1.f;
    float m02 = -2.f;
    float m10 = 3.f;
    float m11 = -4.f;
    float m12 = 5.f;
    float m20 = -6.f;
    float m21 = 7.f;
    float m22 = 8.f;
    cout << "Matrix3F( " << m00 << ", " << m01 << ", " << m02 << ", " << m10 << ", " << m11 << ", " << m12 << ", " << m20 << ", " << m21 << ", " << m22 << " )" << endl;
    Matrix3F mat( m00, m01, m02, m10, m11, m12, m20, m21, m22 );
    cout << "*= mat" << endl;
    vf3 *= mat;
    TESTCHECK( vf3.X(), 10.f, &ok );
    TESTCHECK( vf3.Y(), 14.f, &ok );
    TESTCHECK( vf3.Z(), -150.f, &ok );
    cout << "mat * vf2" << endl;
    vf3 = mat * vf2;
    TESTCHECK( vf3.X(), 205.f, &ok );
    TESTCHECK( vf3.Y(), -652.f, &ok );
    TESTCHECK( vf3.Z(), 283.f, &ok );
    cout << "vf2 * mat" << endl;
    vf3 = vf2 * mat;
    TESTCHECK( vf3.X(), 615.f, &ok );
    TESTCHECK( vf3.Y(), -764.f, &ok );
    TESTCHECK( vf3.Z(), 97.f, &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << vf3;
    TESTCHECK( ost.str(), string( "[ 615, -764, 97 ]" ), &ok );
    TESTCHECK( Vector3F::UnitX.X(), 1.f, &ok );
    TESTCHECK( Vector3F::UnitX.Y(), 0.f, &ok );
    TESTCHECK( Vector3F::UnitX.Z(), 0.f, &ok );
    TESTCHECK( Vector3F::UnitY.X(), 0.f, &ok );
    TESTCHECK( Vector3F::UnitY.Y(), 1.f, &ok );
    TESTCHECK( Vector3F::UnitY.Z(), 0.f, &ok );
    TESTCHECK( Vector3F::UnitZ.X(), 0.f, &ok );
    TESTCHECK( Vector3F::UnitZ.Y(), 0.f, &ok );
    TESTCHECK( Vector3F::UnitZ.Z(), 1.f, &ok );

    if ( ok )
        cout << "Vector3 PASSED." << endl << endl;
    else
        cout << "Vector3 FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

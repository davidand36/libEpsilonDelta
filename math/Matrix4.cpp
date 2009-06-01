/*
  Matrix4.cpp
  Copyright (C) 2007 David M. Anderson

  Matrix4 template class: a 4x4 matrix.
*/


#include "Matrix4.hpp"
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
TestMatrix4( )
{
    bool ok = true;
    cout << "Testing Matrix4" << endl;

    cout << "Matrix4F( ) [default constructor]" << endl;
    Matrix4F mat0;
    cout << "Set( )" << endl;
    mat0.Set( );
    TESTCHECK( mat0(0,0), 0.f, &ok );
    TESTCHECK( mat0(0,1), 0.f, &ok );
    TESTCHECK( mat0(0,2), 0.f, &ok );
    TESTCHECK( mat0(0,3), 0.f, &ok );
    TESTCHECK( mat0(1,0), 0.f, &ok );
    TESTCHECK( mat0(1,1), 0.f, &ok );
    TESTCHECK( mat0(1,2), 0.f, &ok );
    TESTCHECK( mat0(1,3), 0.f, &ok );
    TESTCHECK( mat0(2,0), 0.f, &ok );
    TESTCHECK( mat0(2,1), 0.f, &ok );
    TESTCHECK( mat0(2,2), 0.f, &ok );
    TESTCHECK( mat0(2,3), 0.f, &ok );
    TESTCHECK( mat0(3,0), 0.f, &ok );
    TESTCHECK( mat0(3,1), 0.f, &ok );
    TESTCHECK( mat0(3,2), 0.f, &ok );
    TESTCHECK( mat0(3,3), 0.f, &ok );
    TESTCHECK( mat0.Element(0,0), 0.f, &ok );
    TESTCHECK( mat0.Element(0,1), 0.f, &ok );
    TESTCHECK( mat0.Element(0,2), 0.f, &ok );
    TESTCHECK( mat0.Element(0,3), 0.f, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( mat0.Element(0,4), 0.f, &ok );
        cout << "Element(0,4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    TESTCHECK( mat0.Element(1,0), 0.f, &ok );
    TESTCHECK( mat0.Element(1,1), 0.f, &ok );
    TESTCHECK( mat0.Element(2,2), 0.f, &ok );
    TESTCHECK( mat0.Element(3,3), 0.f, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( mat0.Element(4,0), 0.f, &ok );
        cout << "Element(4,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "Row(0)" << endl;
    Vector4F v0 = mat0.Row(0);
    TESTCHECK( v0[0], 0.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    TESTCHECK( v0[3], 0.f, &ok );
    cout << "Row(1)" << endl;
    Vector4F v1 = mat0.Row(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 0.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    TESTCHECK( v1[3], 0.f, &ok );
    cout << "Row(2)" << endl;
    Vector4F v2 = mat0.Row(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 0.f, &ok );
    TESTCHECK( v2[3], 0.f, &ok );
    cout << "Row(3)" << endl;
    Vector4F v3 = mat0.Row(3);
    TESTCHECK( v3[0], 0.f, &ok );
    TESTCHECK( v3[1], 0.f, &ok );
    TESTCHECK( v3[2], 0.f, &ok );
    TESTCHECK( v3[3], 0.f, &ok );
    cout << "Column(0)" << endl;
    v0 = mat0.Column(0);
    TESTCHECK( v0[0], 0.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    TESTCHECK( v0[3], 0.f, &ok );
    cout << "Column(1)" << endl;
    v1 = mat0.Column(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 0.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    TESTCHECK( v1[3], 0.f, &ok );
    cout << "Column(2)" << endl;
    v2 = mat0.Column(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 0.f, &ok );
    TESTCHECK( v2[3], 0.f, &ok );
    cout << "Column(3)" << endl;
    v3 = mat0.Column(3);
    TESTCHECK( v3[0], 0.f, &ok );
    TESTCHECK( v3[1], 0.f, &ok );
    TESTCHECK( v3[2], 0.f, &ok );
    TESTCHECK( v3[3], 0.f, &ok );
    cout << "Array()" << endl;
    float * arr0 = mat0.Array();
    TESTCHECK( arr0[0], 0.f, &ok );
    TESTCHECK( arr0[1], 0.f, &ok );
    TESTCHECK( arr0[2], 0.f, &ok );
    TESTCHECK( arr0[3], 0.f, &ok );
    TESTCHECK( arr0[4], 0.f, &ok );
    TESTCHECK( arr0[5], 0.f, &ok );
    TESTCHECK( arr0[6], 0.f, &ok );
    TESTCHECK( arr0[7], 0.f, &ok );
    TESTCHECK( arr0[8], 0.f, &ok );
    TESTCHECK( arr0[9], 0.f, &ok );
    TESTCHECK( arr0[10], 0.f, &ok );
    TESTCHECK( arr0[11], 0.f, &ok );
    TESTCHECK( arr0[12], 0.f, &ok );
    TESTCHECK( arr0[13], 0.f, &ok );
    TESTCHECK( arr0[14], 0.f, &ok );
    TESTCHECK( arr0[15], 0.f, &ok );
    TESTCHECK( (mat0 == Matrix4F::Zero), true, &ok );
    float f = 1.f;
    cout << "Matrix4F( " << f << " ) [T constructor]" << endl;
    Matrix4F mat1( f );
    TESTCHECK( mat1(0,0), f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(0,3), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(1,3), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), f, &ok );
    TESTCHECK( mat1(2,3), 0.f, &ok );
    TESTCHECK( mat1(3,0), 0.f, &ok );
    TESTCHECK( mat1(3,1), 0.f, &ok );
    TESTCHECK( mat1(3,2), 0.f, &ok );
    TESTCHECK( mat1(3,3), f, &ok );
    TESTCHECK( mat1.Element(0,0), 1.f, &ok );
    TESTCHECK( mat1.Element(0,1), 0.f, &ok );
    TESTCHECK( mat1.Element(0,2), 0.f, &ok );
    TESTCHECK( mat1.Element(0,3), 0.f, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( mat1.Element(0,4), 0.f, &ok );
        cout << "Element(0,4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    TESTCHECK( mat1.Element(1,0), 0.f, &ok );
    TESTCHECK( mat1.Element(1,1), 1.f, &ok );
    TESTCHECK( mat1.Element(2,2), 1.f, &ok );
    TESTCHECK( mat1.Element(3,3), 1.f, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( mat1.Element(4,0), 0.f, &ok );
        cout << "Element(4,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "Row(0)" << endl;
    v0 = mat1.Row(0);
    TESTCHECK( v0[0], 1.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    TESTCHECK( v0[3], 0.f, &ok );
    cout << "Row(1)" << endl;
    v1 = mat1.Row(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 1.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    TESTCHECK( v1[3], 0.f, &ok );
    cout << "Row(2)" << endl;
    v2 = mat1.Row(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 1.f, &ok );
    TESTCHECK( v2[3], 0.f, &ok );
    cout << "Row(3)" << endl;
    v3 = mat1.Row(3);
    TESTCHECK( v3[0], 0.f, &ok );
    TESTCHECK( v3[1], 0.f, &ok );
    TESTCHECK( v3[2], 0.f, &ok );
    TESTCHECK( v3[3], 1.f, &ok );
    cout << "Column(0)" << endl;
    v0 = mat1.Column(0);
    TESTCHECK( v0[0], 1.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    TESTCHECK( v0[3], 0.f, &ok );
    cout << "Column(1)" << endl;
    v1 = mat1.Column(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 1.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    TESTCHECK( v1[3], 0.f, &ok );
    cout << "Column(2)" << endl;
    v2 = mat1.Column(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 1.f, &ok );
    TESTCHECK( v2[3], 0.f, &ok );
    cout << "Column(3)" << endl;
    v3 = mat1.Column(3);
    TESTCHECK( v3[0], 0.f, &ok );
    TESTCHECK( v3[1], 0.f, &ok );
    TESTCHECK( v3[2], 0.f, &ok );
    TESTCHECK( v3[3], 1.f, &ok );
    cout << "Array()" << endl;
    arr0 = mat1.Array();
    TESTCHECK( arr0[0], 1.f, &ok );
    TESTCHECK( arr0[1], 0.f, &ok );
    TESTCHECK( arr0[2], 0.f, &ok );
    TESTCHECK( arr0[3], 0.f, &ok );
    TESTCHECK( arr0[4], 0.f, &ok );
    TESTCHECK( arr0[5], 1.f, &ok );
    TESTCHECK( arr0[6], 0.f, &ok );
    TESTCHECK( arr0[7], 0.f, &ok );
    TESTCHECK( arr0[8], 0.f, &ok );
    TESTCHECK( arr0[9], 0.f, &ok );
    TESTCHECK( arr0[10], 1.f, &ok );
    TESTCHECK( arr0[11], 0.f, &ok );
    TESTCHECK( arr0[12], 0.f, &ok );
    TESTCHECK( arr0[13], 0.f, &ok );
    TESTCHECK( arr0[14], 0.f, &ok );
    TESTCHECK( arr0[15], 1.f, &ok );
    f = 4.f;
    cout << "Set( " << f << " )" << endl;
    mat1.Set( f );
    TESTCHECK( mat1(0,0), f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(0,3), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(1,3), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), f, &ok );
    TESTCHECK( mat1(2,3), 0.f, &ok );
    TESTCHECK( mat1(3,0), 0.f, &ok );
    TESTCHECK( mat1(3,1), 0.f, &ok );
    TESTCHECK( mat1(3,2), 0.f, &ok );
    TESTCHECK( mat1(3,3), f, &ok );
    f = 0.25f;
    cout << "*= " << f << endl;
    mat1 *= f;
    TESTCHECK( mat1(0,0), 1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(0,3), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), 1.f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(1,3), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), 1.f, &ok );
    TESTCHECK( mat1(2,3), 0.f, &ok );
    TESTCHECK( mat1(3,0), 0.f, &ok );
    TESTCHECK( mat1(3,1), 0.f, &ok );
    TESTCHECK( mat1(3,2), 0.f, &ok );
    TESTCHECK( mat1(3,3), 1.f, &ok );
    TESTCHECK( (mat1 == Matrix4F::Identity), true, &ok );
    TESTCHECK( ToJSON( mat1 ),
               string( "[ [ +1.00000e+00, +0.00000e+00, +0.00000e+00,"
                       " +0.00000e+00 ],"
                       " [ +0.00000e+00, +1.00000e+00, +0.00000e+00,"
                       " +0.00000e+00 ],"
                       " [ +0.00000e+00, +0.00000e+00, +1.00000e+00,"
                       " +0.00000e+00 ],"
                       " [ +0.00000e+00, +0.00000e+00, +0.00000e+00,"
                       " +1.00000e+00 ] ]" ),
               &ok );
    cout << "FromJSON( ToJSON( mat1 ), &mat1 )" << endl;
    FromJSON( ToJSON( mat1 ), &mat1 );
    TESTCHECK( mat1(0,0), 1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(0,3), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), 1.f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(1,3), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), 1.f, &ok );
    TESTCHECK( mat1(2,3), 0.f, &ok );
    TESTCHECK( mat1(3,0), 0.f, &ok );
    TESTCHECK( mat1(3,1), 0.f, &ok );
    TESTCHECK( mat1(3,2), 0.f, &ok );
    TESTCHECK( mat1(3,3), 1.f, &ok );
    float e00 = -417.f;
    float e01 = -528.f;
    float e02 =  360.f;
    float e03 = -852.f;
    float e10 = -639.f;
    float e11 = -741.f;
    float e12 =  420.f;
    float e13 = -309.f;
    float e20 =  820.f;
    float e21 = -369.f;
    float e22 = -134.f;
    float e23 =  307.f;
    float e30 = -847.f;
    float e31 =  348.f;
    float e32 = -793.f;
    float e33 =  273.f;
    cout << "Matrix4F( " << endl;
    cout << e00 << ", " << e01 << ", " << e02 << ", " << e03 << ", " << endl;
    cout << e10 << ", " << e11 << ", " << e12 << ", " << e13 << ", " << endl;
    cout << e20 << ", " << e21 << ", " << e22 << ", " << e23 << ", " << endl;
    cout << e30 << ", " << e31 << ", " << e32 << ", " << e33 << endl;
    cout << " ) [element-by-element constructor]" << endl;
    Matrix4F mat2( e00, e01, e02, e03,
                   e10, e11, e12, e13,
                   e20, e21, e22, e23,
                   e30, e31, e32, e33 );
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(0,2), e02, &ok );
    TESTCHECK( mat2(0,3), e03, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    TESTCHECK( mat2(1,2), e12, &ok );
    TESTCHECK( mat2(1,3), e13, &ok );
    TESTCHECK( mat2(2,0), e20, &ok );
    TESTCHECK( mat2(2,1), e21, &ok );
    TESTCHECK( mat2(2,2), e22, &ok );
    TESTCHECK( mat2(2,3), e23, &ok );
    TESTCHECK( mat2(3,0), e30, &ok );
    TESTCHECK( mat2(3,1), e31, &ok );
    TESTCHECK( mat2(3,2), e32, &ok );
    TESTCHECK( mat2(3,3), e33, &ok );
    TESTCHECK( mat2.Element(0,0), e00, &ok );
    TESTCHECK( mat2.Element(0,1), e01, &ok );
    TESTCHECK( mat2.Element(0,2), e02, &ok );
    TESTCHECK( mat2.Element(0,3), e03, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( mat2.Element(0,4), 0.f, &ok );
        cout << "Element(0,4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    TESTCHECK( mat2.Element(1,0), e10, &ok );
    TESTCHECK( mat2.Element(1,1), e11, &ok );
    TESTCHECK( mat2.Element(1,2), e12, &ok );
    TESTCHECK( mat2.Element(1,3), e13, &ok );
    TESTCHECK( mat2.Element(2,0), e20, &ok );
    TESTCHECK( mat2.Element(2,1), e21, &ok );
    TESTCHECK( mat2.Element(2,2), e22, &ok );
    TESTCHECK( mat2.Element(2,3), e23, &ok );
    TESTCHECK( mat2.Element(3,0), e30, &ok );
    TESTCHECK( mat2.Element(3,1), e31, &ok );
    TESTCHECK( mat2.Element(3,2), e32, &ok );
    TESTCHECK( mat2.Element(3,3), e33, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( mat2.Element(4,0), 0.f, &ok );
        cout << "Element(4,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "Row(0)" << endl;
    v0 = mat2.Row(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e01, &ok );
    TESTCHECK( v0[2], e02, &ok );
    TESTCHECK( v0[3], e03, &ok );
    cout << "Row(1)" << endl;
    v1 = mat2.Row(1);
    TESTCHECK( v1[0], e10, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e12, &ok );
    TESTCHECK( v1[3], e13, &ok );
    cout << "Row(2)" << endl;
    v2 = mat2.Row(2);
    TESTCHECK( v2[0], e20, &ok );
    TESTCHECK( v2[1], e21, &ok );
    TESTCHECK( v2[2], e22, &ok );
    TESTCHECK( v2[3], e23, &ok );
    cout << "Row(3)" << endl;
    v3 = mat2.Row(3);
    TESTCHECK( v3[0], e30, &ok );
    TESTCHECK( v3[1], e31, &ok );
    TESTCHECK( v3[2], e32, &ok );
    TESTCHECK( v3[3], e33, &ok );
    cout << "Column(0)" << endl;
    v0 = mat2.Column(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e10, &ok );
    TESTCHECK( v0[2], e20, &ok );
    TESTCHECK( v0[3], e30, &ok );
    cout << "Column(1)" << endl;
    v1 = mat2.Column(1);
    TESTCHECK( v1[0], e01, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e21, &ok );
    TESTCHECK( v1[3], e31, &ok );
    cout << "Column(2)" << endl;
    v2 = mat2.Column(2);
    TESTCHECK( v2[0], e02, &ok );
    TESTCHECK( v2[1], e12, &ok );
    TESTCHECK( v2[2], e22, &ok );
    TESTCHECK( v2[3], e32, &ok );
    cout << "Column(3)" << endl;
    v3 = mat2.Column(3);
    TESTCHECK( v3[0], e03, &ok );
    TESTCHECK( v3[1], e13, &ok );
    TESTCHECK( v3[2], e23, &ok );
    TESTCHECK( v3[3], e33, &ok );
    cout << "Array()" << endl;
    arr0 = mat2.Array();
    TESTCHECK( arr0[0], e00, &ok );
    TESTCHECK( arr0[1], e10, &ok );
    TESTCHECK( arr0[2], e20, &ok );
    TESTCHECK( arr0[3], e30, &ok );
    TESTCHECK( arr0[4], e01, &ok );
    TESTCHECK( arr0[5], e11, &ok );
    TESTCHECK( arr0[6], e21, &ok );
    TESTCHECK( arr0[7], e31, &ok );
    TESTCHECK( arr0[8], e02, &ok );
    TESTCHECK( arr0[9], e12, &ok );
    TESTCHECK( arr0[10], e22, &ok );
    TESTCHECK( arr0[11], e32, &ok );
    TESTCHECK( arr0[12], e03, &ok );
    TESTCHECK( arr0[13], e13, &ok );
    TESTCHECK( arr0[14], e23, &ok );
    TESTCHECK( arr0[15], e33, &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << mat2;
    TESTCHECK( ost.str(), string( "[ [ -417, -528, 360, -852 ],"
                                  " [ -639, -741, 420, -309 ],"
                                  " [ 820, -369, -134, 307 ],"
                                  " [ -847, 348, -793, 273 ] ]" ), &ok );
    TESTCHECK( ToJSON( mat2 ),
               string( "[ [ -4.17000e+02, -6.39000e+02, +8.20000e+02,"
                       " -8.47000e+02 ],"
                       " [ -5.28000e+02, -7.41000e+02, -3.69000e+02,"
                       " +3.48000e+02 ],"
                       " [ +3.60000e+02, +4.20000e+02, -1.34000e+02,"
                       " -7.93000e+02 ],"
                       " [ -8.52000e+02, -3.09000e+02, +3.07000e+02,"
                       " +2.73000e+02 ] ]" ),
               &ok );
    cout << "FromJSON( ToJSON( mat1 ), &mat1 )" << endl;
    FromJSON( ToJSON( mat1 ), &mat1 );
    TESTCHECK( mat2.Element(1,0), e10, &ok );
    TESTCHECK( mat2.Element(1,1), e11, &ok );
    TESTCHECK( mat2.Element(1,2), e12, &ok );
    TESTCHECK( mat2.Element(1,3), e13, &ok );
    TESTCHECK( mat2.Element(2,0), e20, &ok );
    TESTCHECK( mat2.Element(2,1), e21, &ok );
    TESTCHECK( mat2.Element(2,2), e22, &ok );
    TESTCHECK( mat2.Element(2,3), e23, &ok );
    TESTCHECK( mat2.Element(3,0), e30, &ok );
    TESTCHECK( mat2.Element(3,1), e31, &ok );
    TESTCHECK( mat2.Element(3,2), e32, &ok );
    TESTCHECK( mat2.Element(3,3), e33, &ok );
    e00 = -7.f;
    e01 = 5.f;
    e02 = 3.f;
    e03 = -8.f;
    e10 = -3.f;
    e11 = 2.f;
    e12 = -8.f;
    e13 = 1.f;
    e20 = 7.f;
    e21 = 6.f;
    e22 = -8.f;
    e23 = -4.f;
    e30 = -2.f;
    e31 = -9.f;
    e32 = 4.f;
    e33 = 1.f;
    cout << "Set( " << endl;
    cout << e00 << ", " << e01 << ", " << e02 << ", " << e03 << ", " << endl;
    cout << e10 << ", " << e11 << ", " << e12 << ", " << e13 << ", " << endl;
    cout << e20 << ", " << e21 << ", " << e22 << ", " << e23 << ", " << endl;
    cout << e30 << ", " << e31 << ", " << e32 << ", " << e33 << endl;
    cout << " )" << endl;
    mat2.Set( e00, e01, e02, e03,
              e10, e11, e12, e13,
              e20, e21, e22, e23,
              e30, e31, e32, e33 );
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(0,2), e02, &ok );
    TESTCHECK( mat2(0,3), e03, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    TESTCHECK( mat2(1,2), e12, &ok );
    TESTCHECK( mat2(1,3), e13, &ok );
    TESTCHECK( mat2(2,0), e20, &ok );
    TESTCHECK( mat2(2,1), e21, &ok );
    TESTCHECK( mat2(2,2), e22, &ok );
    TESTCHECK( mat2(2,3), e23, &ok );
    TESTCHECK( mat2(3,0), e30, &ok );
    TESTCHECK( mat2(3,1), e31, &ok );
    TESTCHECK( mat2(3,2), e32, &ok );
    TESTCHECK( mat2(3,3), e33, &ok );
    cout << "Matrix4F( " << endl;
    cout << e00 << ", " << e01 << ", " << e02 << ", " << e03 << ", " << endl;
    cout << e10 << ", " << e11 << ", " << e12 << ", " << e13 << ", " << endl;
    cout << e20 << ", " << e21 << ", " << e22 << ", " << e23 << ", " << endl;
    cout << e30 << ", " << e31 << ", " << e32 << ", " << e33 << ", " << endl;
    cout << " true ) [element-by-element constructor, column-major]" << endl;
    Matrix4F mat3( e00, e01, e02, e03,
                   e10, e11, e12, e13,
                   e20, e21, e22, e23,
                   e30, e31, e32, e33, true );
    TESTCHECK( mat3(0,0), e00, &ok );
    TESTCHECK( mat3(0,1), e10, &ok );
    TESTCHECK( mat3(0,2), e20, &ok );
    TESTCHECK( mat3(0,3), e30, &ok );
    TESTCHECK( mat3(1,0), e01, &ok );
    TESTCHECK( mat3(1,1), e11, &ok );
    TESTCHECK( mat3(1,2), e21, &ok );
    TESTCHECK( mat3(1,3), e31, &ok );
    TESTCHECK( mat3(2,0), e02, &ok );
    TESTCHECK( mat3(2,1), e12, &ok );
    TESTCHECK( mat3(2,2), e22, &ok );
    TESTCHECK( mat3(2,3), e32, &ok );
    TESTCHECK( mat3(3,0), e03, &ok );
    TESTCHECK( mat3(3,1), e13, &ok );
    TESTCHECK( mat3(3,2), e23, &ok );
    TESTCHECK( mat3(3,3), e33, &ok );
    e00 = 1.f;
    e01 = 10.f;
    e02 = 100.f;
    e03 = 1000.f;
    e10 = 2.f;
    e11 = 20.f;
    e12 = 200.f;
    e13 = 2000.f;
    e20 = 3.f;
    e21 = 30.f;
    e22 = 300.f;
    e23 = 3000.f;
    e30 = 4.f;
    e31 = 40.f;
    e32 = 400.f;
    e33 = 4000.f;
    cout << "Set( " << endl;
    cout << e00 << ", " << e01 << ", " << e02 << ", " << e03 << ", " << endl;
    cout << e10 << ", " << e11 << ", " << e12 << ", " << e13 << ", " << endl;
    cout << e20 << ", " << e21 << ", " << e22 << ", " << e23 << ", " << endl;
    cout << e30 << ", " << e31 << ", " << e32 << ", " << e33 << ", " << endl;
    cout << "true )" << endl;
    mat3.Set( e00, e01, e02, e03,
              e10, e11, e12, e13,
              e20, e21, e22, e23,
              e30, e31, e32, e33, true );
    TESTCHECK( mat3(0,0), e00, &ok );
    TESTCHECK( mat3(0,1), e10, &ok );
    TESTCHECK( mat3(0,2), e20, &ok );
    TESTCHECK( mat3(0,3), e30, &ok );
    TESTCHECK( mat3(1,0), e01, &ok );
    TESTCHECK( mat3(1,1), e11, &ok );
    TESTCHECK( mat3(1,2), e21, &ok );
    TESTCHECK( mat3(1,3), e31, &ok );
    TESTCHECK( mat3(2,0), e02, &ok );
    TESTCHECK( mat3(2,1), e12, &ok );
    TESTCHECK( mat3(2,2), e22, &ok );
    TESTCHECK( mat3(2,3), e32, &ok );
    TESTCHECK( mat3(3,0), e03, &ok );
    TESTCHECK( mat3(3,1), e13, &ok );
    TESTCHECK( mat3(3,2), e23, &ok );
    TESTCHECK( mat3(3,3), e33, &ok );
    cout << "arr1[] = { 10.f, 11.f, 12.f, 13.f," << endl;
    cout << "           21.f, 22.f, 23.f, 24.f," << endl;
    cout << "           31.f, 32.f, 33.f, 34.f," << endl;
    cout << "           41.f, 42.f, 43.f, 44.f" << endl;
    float arr1[] = { 10.f, 11.f, 12.f, 13.f,
                     21.f, 22.f, 23.f, 24.f,
                     31.f, 32.f, 33.f, 34.f,
                     41.f, 42.f, 43.f, 44.f };
    cout << "Matrix4F( arr1 ) [array constructor]" << endl;
    Matrix4F mat4( arr1 );
    TESTCHECK( mat4(0,0), arr1[0], &ok );
    TESTCHECK( mat4(0,1), arr1[4], &ok );
    TESTCHECK( mat4(0,2), arr1[8], &ok );
    TESTCHECK( mat4(0,3), arr1[12], &ok );
    TESTCHECK( mat4(1,0), arr1[1], &ok );
    TESTCHECK( mat4(1,1), arr1[5], &ok );
    TESTCHECK( mat4(1,2), arr1[9], &ok );
    TESTCHECK( mat4(1,3), arr1[13], &ok );
    TESTCHECK( mat4(2,0), arr1[2], &ok );
    TESTCHECK( mat4(2,1), arr1[6], &ok );
    TESTCHECK( mat4(2,2), arr1[10], &ok );
    TESTCHECK( mat4(2,3), arr1[14], &ok );
    TESTCHECK( mat4(3,0), arr1[3], &ok );
    TESTCHECK( mat4(3,1), arr1[7], &ok );
    TESTCHECK( mat4(3,2), arr1[11], &ok );
    TESTCHECK( mat4(3,3), arr1[15], &ok );
    cout << "Set( arr0 )" << endl;
    mat4.Set( arr0 );
    TESTCHECK( mat4(0,0), mat2(0,0), &ok );
    TESTCHECK( mat4(0,1), mat2(0,1), &ok );
    TESTCHECK( mat4(0,2), mat2(0,2), &ok );
    TESTCHECK( mat4(0,3), mat2(0,3), &ok );
    TESTCHECK( mat4(1,0), mat2(1,0), &ok );
    TESTCHECK( mat4(1,1), mat2(1,1), &ok );
    TESTCHECK( mat4(1,2), mat2(1,2), &ok );
    TESTCHECK( mat4(1,3), mat2(1,3), &ok );
    TESTCHECK( mat4(2,0), mat2(2,0), &ok );
    TESTCHECK( mat4(2,1), mat2(2,1), &ok );
    TESTCHECK( mat4(2,2), mat2(2,2), &ok );
    TESTCHECK( mat4(2,3), mat2(2,3), &ok );
    TESTCHECK( mat4(3,0), mat2(3,0), &ok );
    TESTCHECK( mat4(3,1), mat2(3,1), &ok );
    TESTCHECK( mat4(3,2), mat2(3,2), &ok );
    TESTCHECK( mat4(3,3), mat2(3,3), &ok );
    TESTCHECK( (mat4 == mat2), true, &ok );
    cout << "Matrix4F( arr1, false ) [array constructor, row-major]" << endl;
    Matrix4F mat5( arr1, false );
    TESTCHECK( mat5(0,0), arr1[0], &ok );
    TESTCHECK( mat5(0,1), arr1[1], &ok );
    TESTCHECK( mat5(0,2), arr1[2], &ok );
    TESTCHECK( mat5(0,3), arr1[3], &ok );
    TESTCHECK( mat5(1,0), arr1[4], &ok );
    TESTCHECK( mat5(1,1), arr1[5], &ok );
    TESTCHECK( mat5(1,2), arr1[6], &ok );
    TESTCHECK( mat5(1,3), arr1[7], &ok );
    TESTCHECK( mat5(2,0), arr1[8], &ok );
    TESTCHECK( mat5(2,1), arr1[9], &ok );
    TESTCHECK( mat5(2,2), arr1[10], &ok );
    TESTCHECK( mat5(2,3), arr1[11], &ok );
    TESTCHECK( mat5(3,0), arr1[12], &ok );
    TESTCHECK( mat5(3,1), arr1[13], &ok );
    TESTCHECK( mat5(3,2), arr1[14], &ok );
    TESTCHECK( mat5(3,3), arr1[15], &ok );
    cout << "Set( arr0, false )" << endl;
    mat5.Set( arr0, false );
    TESTCHECK( mat5(0,0), mat2(0,0), &ok );
    TESTCHECK( mat5(0,1), mat2(1,0), &ok );
    TESTCHECK( mat5(0,2), mat2(2,0), &ok );
    TESTCHECK( mat5(0,3), mat2(3,0), &ok );
    TESTCHECK( mat5(1,0), mat2(0,1), &ok );
    TESTCHECK( mat5(1,1), mat2(1,1), &ok );
    TESTCHECK( mat5(1,2), mat2(2,1), &ok );
    TESTCHECK( mat5(1,3), mat2(3,1), &ok );
    TESTCHECK( mat5(2,0), mat2(0,2), &ok );
    TESTCHECK( mat5(2,1), mat2(1,2), &ok );
    TESTCHECK( mat5(2,2), mat2(2,2), &ok );
    TESTCHECK( mat5(2,3), mat2(3,2), &ok );
    TESTCHECK( mat5(3,0), mat2(0,3), &ok );
    TESTCHECK( mat5(3,1), mat2(1,3), &ok );
    TESTCHECK( mat5(3,2), mat2(2,3), &ok );
    TESTCHECK( mat5(3,3), mat2(3,3), &ok );
    TESTCHECK( (mat5 == mat2), false, &ok );
    e00 = -10.f;
    e01 = -20.f;
    e02 = -30.f;
    e03 = -40.f;
    e10 = -11.f;
    e11 = -21.f;
    e12 = -31.f;
    e13 = 41.f;
    e20 = -12.f;
    e21 = -22.f;
    e22 = 32.f;
    e23 = 42.f;
    e30 = 13.f;
    e31 = 23.f;
    e32 = -33.f;
    e33 = -43.f;
    cout << "v0.Set( " << e00 << ", " << e10 << ", " << e20 << ", " << e30 << " )" << endl;
    v0.Set( e00, e10, e20, e30 );
    cout << "v1.Set( " << e01 << ", " << e11 << ", " << e21 << ", " << e31 << " )" << endl;
    v1.Set( e01, e11, e21, e31 );
    cout << "v2.Set( " << e02 << ", " << e12 << ", " << e22 << ", " << e32 << " )" << endl;
    v2.Set( e02, e12, e22, e32 );
    cout << "v3.Set( " << e03 << ", " << e13 << ", " << e23 << ", " << e33 << " )" << endl;
    v3.Set( e03, e13, e23, e33 );
    cout << "Matrix4F( v0, v1, v2, v3 ) [column vector constructor]" << endl;
    Matrix4F mat6( v0, v1, v2, v3 );
    TESTCHECK( mat6(0,0), e00, &ok );
    TESTCHECK( mat6(0,1), e01, &ok );
    TESTCHECK( mat6(0,2), e02, &ok );
    TESTCHECK( mat6(0,3), e03, &ok );
    TESTCHECK( mat6(1,0), e10, &ok );
    TESTCHECK( mat6(1,1), e11, &ok );
    TESTCHECK( mat6(1,2), e12, &ok );
    TESTCHECK( mat6(1,3), e13, &ok );
    TESTCHECK( mat6(2,0), e20, &ok );
    TESTCHECK( mat6(2,1), e21, &ok );
    TESTCHECK( mat6(2,2), e22, &ok );
    TESTCHECK( mat6(2,3), e23, &ok );
    TESTCHECK( mat6(3,0), e30, &ok );
    TESTCHECK( mat6(3,1), e31, &ok );
    TESTCHECK( mat6(3,2), e32, &ok );
    TESTCHECK( mat6(3,3), e33, &ok );
    e00 = 210.f;
    e01 = 320.f;
    e02 = 430.f;
    e03 = 540.f;
    e10 = 650.f;
    e11 = 760.f;
    e12 = 870.f;
    e13 = 980.f;
    e20 = 120.f;
    e21 = 230.f;
    e22 = 340.f;
    e23 = 450.f;
    e30 = 560.f;
    e31 = 670.f;
    e32 = 780.f;
    e33 = 890.f;
    cout << "v0.Set( " << e00 << ", " << e10 << ", " << e20 << ", " << e30 << " )" << endl;
    v0.Set( e00, e10, e20, e30 );
    cout << "v1.Set( " << e01 << ", " << e11 << ", " << e21 << ", " << e31 << " )" << endl;
    v1.Set( e01, e11, e21, e31 );
    cout << "v2.Set( " << e02 << ", " << e12 << ", " << e22 << ", " << e32 << " )" << endl;
    v2.Set( e02, e12, e22, e32 );
    cout << "v3.Set( " << e03 << ", " << e13 << ", " << e23 << ", " << e33 << " )" << endl;
    v3.Set( e03, e13, e23, e33 );
    cout << "Set( v0, v1, v2, v3 )" << endl;
    cout << "Set( v0, v1, v2, v3 )" << endl;
    mat6.Set( v0, v1, v2, v3 );
    TESTCHECK( mat6(0,0), e00, &ok );
    TESTCHECK( mat6(0,1), e01, &ok );
    TESTCHECK( mat6(0,2), e02, &ok );
    TESTCHECK( mat6(0,3), e03, &ok );
    TESTCHECK( mat6(1,0), e10, &ok );
    TESTCHECK( mat6(1,1), e11, &ok );
    TESTCHECK( mat6(1,2), e12, &ok );
    TESTCHECK( mat6(1,3), e13, &ok );
    TESTCHECK( mat6(2,0), e20, &ok );
    TESTCHECK( mat6(2,1), e21, &ok );
    TESTCHECK( mat6(2,2), e22, &ok );
    TESTCHECK( mat6(2,3), e23, &ok );
    TESTCHECK( mat6(3,0), e30, &ok );
    TESTCHECK( mat6(3,1), e31, &ok );
    TESTCHECK( mat6(3,2), e32, &ok );
    TESTCHECK( mat6(3,3), e33, &ok );
    e00 = 17.f;
    e01 = -28.f;
    e02 = 39.f;
    e03 = -40.f;
    e10 = 51.f;
    e11 = -62.f;
    e12 = -73.f;
    e13 = 84.f;
    e20 = 95.f;
    e21 = -25.f;
    e22 = 36.f;
    e23 = -47.f;
    e30 = 58.f;
    e31 = -69.f;
    e32 = -70.f;
    e33 = 14.f;
    cout << "v0.Set( " << e00 << ", " << e01 << ", " << e02 << ", " << e03 << " )" << endl;
    v0.Set( e00, e01, e02, e03 );
    cout << "v1.Set( " << e10 << ", " << e11 << ", " << e12 << ", " << e13 << " )" << endl;
    v1.Set( e10, e11, e12, e13 );
    cout << "v2.Set( " << e20 << ", " << e21 << ", " << e22 << ", " << e23 << " )" << endl;
    v2.Set( e20, e21, e22, e23 );
    cout << "v3.Set( " << e30 << ", " << e31 << ", " << e32 << ", " << e33 << " )" << endl;
    v3.Set( e30, e31, e32, e33 );
    cout << "Matrix4F( v0, v1, v2, v3, false ) [row vector constructor]" << endl;
    Matrix4F mat7( v0, v1, v2, v3, false );
    TESTCHECK( mat7(0,0), e00, &ok );
    TESTCHECK( mat7(0,1), e01, &ok );
    TESTCHECK( mat7(0,2), e02, &ok );
    TESTCHECK( mat7(0,3), e03, &ok );
    TESTCHECK( mat7(1,0), e10, &ok );
    TESTCHECK( mat7(1,1), e11, &ok );
    TESTCHECK( mat7(1,2), e12, &ok );
    TESTCHECK( mat7(1,3), e13, &ok );
    TESTCHECK( mat7(2,0), e20, &ok );
    TESTCHECK( mat7(2,1), e21, &ok );
    TESTCHECK( mat7(2,2), e22, &ok );
    TESTCHECK( mat7(2,3), e23, &ok );
    TESTCHECK( mat7(3,0), e30, &ok );
    TESTCHECK( mat7(3,1), e31, &ok );
    TESTCHECK( mat7(3,2), e32, &ok );
    TESTCHECK( mat7(3,3), e33, &ok );
    e00 = 1.f;
    e01 = 2.f;
    e02 = 0.f;
    e03 = -1.f;
    e10 = 0.f;
    e11 = 0.f;
    e12 = 1.f;
    e13 = 0.f;
    e20 = 0.f;
    e21 = 1.f;
    e22 = 7.f;
    e23 = 1.f;
    e30 = -2.f;
    e31 = 0.f;
    e32 = 3.f;
    e33 = 5.f;
    cout << "v0.Set( " << e00 << ", " << e01 << ", " << e02 << ", " << e03 << " )" << endl;
    v0.Set( e00, e01, e02, e03 );
    cout << "v1.Set( " << e10 << ", " << e11 << ", " << e12 << ", " << e13 << " )" << endl;
    v1.Set( e10, e11, e12, e13 );
    cout << "v2.Set( " << e20 << ", " << e21 << ", " << e22 << ", " << e23 << " )" << endl;
    v2.Set( e20, e21, e22, e23 );
    cout << "v3.Set( " << e30 << ", " << e31 << ", " << e32 << ", " << e33 << " )" << endl;
    v3.Set( e30, e31, e32, e33 );
    cout << "Matrix4F( v0, v1, v2, v3, false ) [row vector constructor]" << endl;
    cout << "Set( v0, v1, v2, v3, false )" << endl;
    mat7.Set( v0, v1, v2, v3, false );
    TESTCHECK( mat7(0,0), e00, &ok );
    TESTCHECK( mat7(0,1), e01, &ok );
    TESTCHECK( mat7(0,2), e02, &ok );
    TESTCHECK( mat7(0,3), e03, &ok );
    TESTCHECK( mat7(1,0), e10, &ok );
    TESTCHECK( mat7(1,1), e11, &ok );
    TESTCHECK( mat7(1,2), e12, &ok );
    TESTCHECK( mat7(1,3), e13, &ok );
    TESTCHECK( mat7(2,0), e20, &ok );
    TESTCHECK( mat7(2,1), e21, &ok );
    TESTCHECK( mat7(2,2), e22, &ok );
    TESTCHECK( mat7(2,3), e23, &ok );
    TESTCHECK( mat7(3,0), e30, &ok );
    TESTCHECK( mat7(3,1), e31, &ok );
    TESTCHECK( mat7(3,2), e32, &ok );
    TESTCHECK( mat7(3,3), e33, &ok );
    cout << "Row(0)" << endl;
    v0 = mat7.Row(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e01, &ok );
    TESTCHECK( v0[2], e02, &ok );
    TESTCHECK( v0[3], e03, &ok );
    cout << "Row(1)" << endl;
    v1 = mat7.Row(1);
    TESTCHECK( v1[0], e10, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e12, &ok );
    TESTCHECK( v1[3], e13, &ok );
    cout << "Row(2)" << endl;
    v2 = mat7.Row(2);
    TESTCHECK( v2[0], e20, &ok );
    TESTCHECK( v2[1], e21, &ok );
    TESTCHECK( v2[2], e22, &ok );
    TESTCHECK( v2[3], e23, &ok );
    cout << "Row(3)" << endl;
    v3 = mat7.Row(3);
    TESTCHECK( v3[0], e30, &ok );
    TESTCHECK( v3[1], e31, &ok );
    TESTCHECK( v3[2], e32, &ok );
    TESTCHECK( v3[3], e33, &ok );
    cout << "Column(0)" << endl;
    v0 = mat7.Column(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e10, &ok );
    TESTCHECK( v0[2], e20, &ok );
    TESTCHECK( v0[3], e30, &ok );
    cout << "Column(1)" << endl;
    v1 = mat7.Column(1);
    TESTCHECK( v1[0], e01, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e21, &ok );
    TESTCHECK( v1[3], e31, &ok );
    cout << "Column(2)" << endl;
    v2 = mat7.Column(2);
    TESTCHECK( v2[0], e02, &ok );
    TESTCHECK( v2[1], e12, &ok );
    TESTCHECK( v2[2], e22, &ok );
    TESTCHECK( v2[3], e32, &ok );
    cout << "Column(3)" << endl;
    v3 = mat7.Column(3);
    TESTCHECK( v3[0], e03, &ok );
    TESTCHECK( v3[1], e13, &ok );
    TESTCHECK( v3[2], e23, &ok );
    TESTCHECK( v3[3], e33, &ok );
    cout << "Matrix4F( mat6 ) [copy constructor]" << endl;
    Matrix4F mat9( mat6 );
    TESTCHECK( mat9(0,0), mat6(0,0), &ok );
    TESTCHECK( mat9(0,1), mat6(0,1), &ok );
    TESTCHECK( mat9(0,2), mat6(0,2), &ok );
    TESTCHECK( mat9(0,3), mat6(0,3), &ok );
    TESTCHECK( mat9(1,0), mat6(1,0), &ok );
    TESTCHECK( mat9(1,1), mat6(1,1), &ok );
    TESTCHECK( mat9(1,2), mat6(1,2), &ok );
    TESTCHECK( mat9(1,3), mat6(1,3), &ok );
    TESTCHECK( mat9(2,0), mat6(2,0), &ok );
    TESTCHECK( mat9(2,1), mat6(2,1), &ok );
    TESTCHECK( mat9(2,2), mat6(2,2), &ok );
    TESTCHECK( mat9(2,3), mat6(2,3), &ok );
    TESTCHECK( mat9(3,0), mat6(3,0), &ok );
    TESTCHECK( mat9(3,1), mat6(3,1), &ok );
    TESTCHECK( mat9(3,2), mat6(3,2), &ok );
    TESTCHECK( mat9(3,3), mat6(3,3), &ok );
    TESTCHECK( (mat9 == mat9), true, &ok );
    TESTCHECK( (mat9 == mat6), true, &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    TESTCHECK( (mat9 == mat2), false, &ok );
    cout << "= mat4" << endl;
    mat9 = mat4;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(0,3), mat4(0,3), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(1,3), mat4(1,3), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( mat9(2,3), mat4(2,3), &ok );
    TESTCHECK( mat9(3,0), mat4(3,0), &ok );
    TESTCHECK( mat9(3,1), mat4(3,1), &ok );
    TESTCHECK( mat9(3,2), mat4(3,2), &ok );
    TESTCHECK( mat9(3,3), mat4(3,3), &ok );
    TESTCHECK( (mat9 == mat9), true, &ok );
    TESTCHECK( (mat9 == mat6), false, &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    TESTCHECK( (mat9 == mat2), true, &ok );
    f = 121.f;
    cout << "mat9(0,0) = " << f << endl;
    mat9(0,0) = f;
    TESTCHECK( mat9(0,0), f, &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(0,0) = mat4(0,0)" << endl;
    mat9.Element(0,0) = mat4(0,0);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "mat9(0,1) = " << f << endl;
    mat9(0,1) = f;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), f, &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(0,1) = mat4(0,1)" << endl;
    mat9.Element(0,1) = mat4(0,1);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        cout << "mat9.Element(0,4) = " << f << endl;
        mat9.Element(0,4) = f;
        cout << "Element(0,4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "mat9(1,0) = " << f << endl;
    mat9(1,0) = f;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), f, &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(1,0) = mat4(1,0)" << endl;
    mat9.Element(1,0) = mat4(1,0);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "mat9(3,3) = " << f << endl;
    mat9(3,3) = f;
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( mat9(2,3), mat4(2,3), &ok );
    TESTCHECK( mat9(3,2), mat4(3,2), &ok );
    TESTCHECK( mat9(3,3), f, &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(3,3) = mat4(3,3)" << endl;
    mat9.Element(3,3) = mat4(3,3);
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( mat9(2,3), mat4(2,3), &ok );
    TESTCHECK( mat9(3,2), mat4(3,2), &ok );
    TESTCHECK( mat9(3,3), mat4(3,3), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        cout << "mat9.Element(4,0) = " << f << endl;
        mat9.Element(4,0) = f;
        cout << "Element(4,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "+= mat2" << endl;
    mat9 += mat2;
    TESTCHECK( mat9(0,0), -14.f, &ok );
    TESTCHECK( mat9(0,1), 10.f, &ok );
    TESTCHECK( mat9(0,2), 6.f, &ok );
    TESTCHECK( mat9(0,3), -16.f, &ok );
    TESTCHECK( mat9(1,0), -6.f, &ok );
    TESTCHECK( mat9(1,1), 4.f, &ok );
    TESTCHECK( mat9(1,2), -16.f, &ok );
    TESTCHECK( mat9(1,3), 2.f, &ok );
    TESTCHECK( mat9(2,0), 14.f, &ok );
    TESTCHECK( mat9(2,1), 12.f, &ok );
    TESTCHECK( mat9(2,2), -16.f, &ok );
    TESTCHECK( mat9(2,3), -8.f, &ok );
    TESTCHECK( mat9(3,0), -4.f, &ok );
    TESTCHECK( mat9(3,1), -18.f, &ok );
    TESTCHECK( mat9(3,2), 8.f, &ok );
    TESTCHECK( mat9(3,3), 2.f, &ok );
    cout << "-= mat4" << endl;
    mat9 -= mat4;
    TESTCHECK( mat9(0,0), -7.f, &ok );
    TESTCHECK( mat9(0,1), 5.f, &ok );
    TESTCHECK( mat9(0,2), 3.f, &ok );
    TESTCHECK( mat9(0,3), -8.f, &ok );
    TESTCHECK( mat9(1,0), -3.f, &ok );
    TESTCHECK( mat9(1,1), 2.f, &ok );
    TESTCHECK( mat9(1,2), -8.f, &ok );
    TESTCHECK( mat9(1,3), 1.f, &ok );
    TESTCHECK( mat9(2,0), 7.f, &ok );
    TESTCHECK( mat9(2,1), 6.f, &ok );
    TESTCHECK( mat9(2,2), -8.f, &ok );
    TESTCHECK( mat9(2,3), -4.f, &ok );
    TESTCHECK( mat9(3,0), -2.f, &ok );
    TESTCHECK( mat9(3,1), -9.f, &ok );
    TESTCHECK( mat9(3,2), 4.f, &ok );
    TESTCHECK( mat9(3,3), 1.f, &ok );
    TESTCHECK( (mat9 == mat6), false, &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    TESTCHECK( (mat9 == mat2), true, &ok );
    cout << "+= mat0" << endl;
    mat9 += mat0;
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "-= mat0" << endl;
    mat9 -= mat0;
    TESTCHECK( (mat9 == mat4), true, &ok );
    f = -2.f;
    cout << "*= " << f << endl;
    mat9 *= f;
    TESTCHECK( mat9(0,0), 14.f, &ok );
    TESTCHECK( mat9(0,1), -10.f, &ok );
    TESTCHECK( mat9(0,2), -6.f, &ok );
    TESTCHECK( mat9(0,3), 16.f, &ok );
    TESTCHECK( mat9(1,0), 6.f, &ok );
    TESTCHECK( mat9(1,1), -4.f, &ok );
    TESTCHECK( mat9(1,2), 16.f, &ok );
    TESTCHECK( mat9(1,3), -2.f, &ok );
    TESTCHECK( mat9(2,0), -14.f, &ok );
    TESTCHECK( mat9(2,1), -12.f, &ok );
    TESTCHECK( mat9(2,2), 16.f, &ok );
    TESTCHECK( mat9(2,3), 8.f, &ok );
    TESTCHECK( mat9(3,0), 4.f, &ok );
    TESTCHECK( mat9(3,1), 18.f, &ok );
    TESTCHECK( mat9(3,2), -8.f, &ok );
    TESTCHECK( mat9(3,3), -2.f, &ok );
    cout << "Matrix4F mat8( -1.f )" << endl;
    Matrix4F mat8( -1.f );
    cout << "*= mat8" << endl;
    mat9 *= mat8;
    TESTCHECK( mat9(0,0), -14.f, &ok );
    TESTCHECK( mat9(0,1), 10.f, &ok );
    TESTCHECK( mat9(0,2), 6.f, &ok );
    TESTCHECK( mat9(0,3), -16.f, &ok );
    TESTCHECK( mat9(1,0), -6.f, &ok );
    TESTCHECK( mat9(1,1), 4.f, &ok );
    TESTCHECK( mat9(1,2), -16.f, &ok );
    TESTCHECK( mat9(1,3), 2.f, &ok );
    TESTCHECK( mat9(2,0), 14.f, &ok );
    TESTCHECK( mat9(2,1), 12.f, &ok );
    TESTCHECK( mat9(2,2), -16.f, &ok );
    TESTCHECK( mat9(2,3), -8.f, &ok );
    TESTCHECK( mat9(3,0), -4.f, &ok );
    TESTCHECK( mat9(3,1), -18.f, &ok );
    TESTCHECK( mat9(3,2), 8.f, &ok );
    TESTCHECK( mat9(3,3), 2.f, &ok );
    cout << "*= mat1" << endl;
    mat9 *= mat1;
    TESTCHECK( mat9(0,0), -14.f, &ok );
    TESTCHECK( mat9(0,1), 10.f, &ok );
    TESTCHECK( mat9(0,2), 6.f, &ok );
    TESTCHECK( mat9(0,3), -16.f, &ok );
    TESTCHECK( mat9(1,0), -6.f, &ok );
    TESTCHECK( mat9(1,1), 4.f, &ok );
    TESTCHECK( mat9(1,2), -16.f, &ok );
    TESTCHECK( mat9(1,3), 2.f, &ok );
    TESTCHECK( mat9(2,0), 14.f, &ok );
    TESTCHECK( mat9(2,1), 12.f, &ok );
    TESTCHECK( mat9(2,2), -16.f, &ok );
    TESTCHECK( mat9(2,3), -8.f, &ok );
    TESTCHECK( mat9(3,0), -4.f, &ok );
    TESTCHECK( mat9(3,1), -18.f, &ok );
    TESTCHECK( mat9(3,2), 8.f, &ok );
    TESTCHECK( mat9(3,3), 2.f, &ok );
    f = 0.5f;
    cout << "*= Matrix4F( " << f << " )" << endl;
    mat9 *= Matrix4F( f );
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "- mat9 [negation]" << endl;
    mat9 = - mat9;
    TESTCHECK( mat9(0,0), 7.f, &ok );
    TESTCHECK( mat9(0,1), -5.f, &ok );
    TESTCHECK( mat9(0,2), -3.f, &ok );
    TESTCHECK( mat9(0,3), 8.f, &ok );
    TESTCHECK( mat9(1,0), 3.f, &ok );
    TESTCHECK( mat9(1,1), -2.f, &ok );
    TESTCHECK( mat9(1,2), 8.f, &ok );
    TESTCHECK( mat9(1,3), -1.f, &ok );
    TESTCHECK( mat9(2,0), -7.f, &ok );
    TESTCHECK( mat9(2,1), -6.f, &ok );
    TESTCHECK( mat9(2,2), 8.f, &ok );
    TESTCHECK( mat9(2,3), 4.f, &ok );
    TESTCHECK( mat9(3,0), 2.f, &ok );
    TESTCHECK( mat9(3,1), 9.f, &ok );
    TESTCHECK( mat9(3,2), -4.f, &ok );
    TESTCHECK( mat9(3,3), -1.f, &ok );
    cout << "- mat9 [negation]" << endl;
    mat9 = - mat9;
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "Transpose()" << endl;
    mat9 = mat9.Transpose();
    TESTCHECK( mat9(0,0), -7.f, &ok );
    TESTCHECK( mat9(0,1), -3.f, &ok );
    TESTCHECK( mat9(0,2), 7.f, &ok );
    TESTCHECK( mat9(0,3), -2.f, &ok );
    TESTCHECK( mat9(1,0), 5.f, &ok );
    TESTCHECK( mat9(1,1), 2.f, &ok );
    TESTCHECK( mat9(1,2), 6.f, &ok );
    TESTCHECK( mat9(1,3), -9.f, &ok );
    TESTCHECK( mat9(2,0), 3.f, &ok );
    TESTCHECK( mat9(2,1), -8.f, &ok );
    TESTCHECK( mat9(2,2), -8.f, &ok );
    TESTCHECK( mat9(2,3), 4.f, &ok );
    TESTCHECK( mat9(3,0), -8.f, &ok );
    TESTCHECK( mat9(3,1), 1.f, &ok );
    TESTCHECK( mat9(3,2), -4.f, &ok );
    TESTCHECK( mat9(3,3), 1.f, &ok );
    TESTCHECK( (mat9 == mat5), true, &ok );
    cout << "Transpose()" << endl;
    mat9 = mat9.Transpose();
    TESTCHECK( (mat9 == mat4), true, &ok );
    TESTCHECK( (mat5 == mat2.Transpose()), true, &ok );
    TESTCHECK( (mat5.Transpose() == mat2), true, &ok );
    TESTCHECK( mat1.Trace(), 4.f, &ok );
    TESTCHECK( mat1.Determinant(), 1.f, &ok );
    TESTCHECK( mat0.Trace(), 0.f, &ok );
    TESTCHECK( mat0.Determinant(), 0.f, &ok );
    TESTCHECK( mat7.Trace(), 13.f, &ok );
    TESTCHECK( mat7.Determinant(), 1.f, &ok );
    TESTCHECK( mat9.Trace(), -12.f, &ok );
    TESTCHECK( mat9.Determinant(), 6351.f, &ok );
    cout << "Inverse()" << endl;
    mat3 = mat7.Inverse();
    TESTCHECK( mat3(0,0), -5.f, &ok );
    TESTCHECK( mat3(0,1), -61.f, &ok );
    TESTCHECK( mat3(0,2), 10.f, &ok );
    TESTCHECK( mat3(0,3), -3.f, &ok );
    TESTCHECK( mat3(1,0), 2.f, &ok );
    TESTCHECK( mat3(1,1), 18.f, &ok );
    TESTCHECK( mat3(1,2), -3.f, &ok );
    TESTCHECK( mat3(1,3), 1.f, &ok );
    TESTCHECK( mat3(2,0), 0.f, &ok );
    TESTCHECK( mat3(2,1), 1.f, &ok );
    TESTCHECK( mat3(2,2), 0.f, &ok );
    TESTCHECK( mat3(2,3), 0.f, &ok );
    TESTCHECK( mat3(3,0), -2.f, &ok );
    TESTCHECK( mat3(3,1), -25.f, &ok );
    TESTCHECK( mat3(3,2), 4.f, &ok );
    TESTCHECK( mat3(3,3), -1.f, &ok );
    f = mat7.Determinant();
    cout << "Inverse( " << f << " )" << endl;
    mat3 = mat7.Inverse( f );
    TESTCHECK( mat3(0,0), -5.f, &ok );
    TESTCHECK( mat3(0,1), -61.f, &ok );
    TESTCHECK( mat3(0,2), 10.f, &ok );
    TESTCHECK( mat3(0,3), -3.f, &ok );
    TESTCHECK( mat3(1,0), 2.f, &ok );
    TESTCHECK( mat3(1,1), 18.f, &ok );
    TESTCHECK( mat3(1,2), -3.f, &ok );
    TESTCHECK( mat3(1,3), 1.f, &ok );
    TESTCHECK( mat3(2,0), 0.f, &ok );
    TESTCHECK( mat3(2,1), 1.f, &ok );
    TESTCHECK( mat3(2,2), 0.f, &ok );
    TESTCHECK( mat3(2,3), 0.f, &ok );
    TESTCHECK( mat3(3,0), -2.f, &ok );
    TESTCHECK( mat3(3,1), -25.f, &ok );
    TESTCHECK( mat3(3,2), 4.f, &ok );
    TESTCHECK( mat3(3,3), -1.f, &ok );
    try
    {
        cout << "mat0.Inverse()" << endl;
        mat3 = mat0.Inverse();
        cout << "mat0.Inverse() should have thrown an exception." << endl;
        ok = false;
    }
    catch( SingularMatrixException & exceptn )
    {
        cout << "SingularMatrixException here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    cout << "mat6 = mat9 + mat7" << endl;
    mat6 = mat9 + mat7;
    TESTCHECK( mat6(0,0), -6.f, &ok );
    TESTCHECK( mat6(0,1), 7.f, &ok );
    TESTCHECK( mat6(0,2), 3.f, &ok );
    TESTCHECK( mat6(0,3), -9.f, &ok );
    TESTCHECK( mat6(1,0), -3.f, &ok );
    TESTCHECK( mat6(1,1), 2.f, &ok );
    TESTCHECK( mat6(1,2), -7.f, &ok );
    TESTCHECK( mat6(1,3), 1.f, &ok );
    TESTCHECK( mat6(2,0), 7.f, &ok );
    TESTCHECK( mat6(2,1), 7.f, &ok );
    TESTCHECK( mat6(2,2), -1.f, &ok );
    TESTCHECK( mat6(2,3), -3.f, &ok );
    TESTCHECK( mat6(3,0), -4.f, &ok );
    TESTCHECK( mat6(3,1), -9.f, &ok );
    TESTCHECK( mat6(3,2), 7.f, &ok );
    TESTCHECK( mat6(3,3), 6.f, &ok );
    cout << "mat6 = mat9 - mat7" << endl;
    mat6 = mat9 - mat7;
    TESTCHECK( mat6(0,0), -8.f, &ok );
    TESTCHECK( mat6(0,1), 3.f, &ok );
    TESTCHECK( mat6(0,2), 3.f, &ok );
    TESTCHECK( mat6(0,3), -7.f, &ok );
    TESTCHECK( mat6(1,0), -3.f, &ok );
    TESTCHECK( mat6(1,1), 2.f, &ok );
    TESTCHECK( mat6(1,2), -9.f, &ok );
    TESTCHECK( mat6(1,3), 1.f, &ok );
    TESTCHECK( mat6(2,0), 7.f, &ok );
    TESTCHECK( mat6(2,1), 5.f, &ok );
    TESTCHECK( mat6(2,2), -15.f, &ok );
    TESTCHECK( mat6(2,3), -5.f, &ok );
    TESTCHECK( mat6(3,0), 0.f, &ok );
    TESTCHECK( mat6(3,1), -9.f, &ok );
    TESTCHECK( mat6(3,2), 1.f, &ok );
    TESTCHECK( mat6(3,3), -4.f, &ok );
    f = -2.f;
    cout << "mat6 = " << f << " * mat9" << endl;
    mat6 = f * mat9;
    TESTCHECK( mat6(0,0), 14.f, &ok );
    TESTCHECK( mat6(0,1), -10.f, &ok );
    TESTCHECK( mat6(0,2), -6.f, &ok );
    TESTCHECK( mat6(0,3), 16.f, &ok );
    TESTCHECK( mat6(1,0), 6.f, &ok );
    TESTCHECK( mat6(1,1), -4.f, &ok );
    TESTCHECK( mat6(1,2), 16.f, &ok );
    TESTCHECK( mat6(1,3), -2.f, &ok );
    TESTCHECK( mat6(2,0), -14.f, &ok );
    TESTCHECK( mat6(2,1), -12.f, &ok );
    TESTCHECK( mat6(2,2), 16.f, &ok );
    TESTCHECK( mat6(2,3), 8.f, &ok );
    TESTCHECK( mat6(3,0), 4.f, &ok );
    TESTCHECK( mat6(3,1), 18.f, &ok );
    TESTCHECK( mat6(3,2), -8.f, &ok );
    TESTCHECK( mat6(3,3), -2.f, &ok );
    f = -0.5f;
    cout << "mat6 = mat6 * " << f << endl;
    mat6 = mat6 * f;
    TESTCHECK( (mat6 == mat4), true, &ok );
    cout << "mat6 = mat9 * mat7" << endl;
    mat6 = mat9 * mat7;
    TESTCHECK( mat6(0,0), 9.f, &ok );
    TESTCHECK( mat6(0,1), -11.f, &ok );
    TESTCHECK( mat6(0,2), 2.f, &ok );
    TESTCHECK( mat6(0,3), -30.f, &ok );
    TESTCHECK( mat6(1,0), -5.f, &ok );
    TESTCHECK( mat6(1,1), -14.f, &ok );
    TESTCHECK( mat6(1,2), -51.f, &ok );
    TESTCHECK( mat6(1,3), 0.f, &ok );
    TESTCHECK( mat6(2,0), 15.f, &ok );
    TESTCHECK( mat6(2,1), 6.f, &ok );
    TESTCHECK( mat6(2,2), -62.f, &ok );
    TESTCHECK( mat6(2,3), -35.f, &ok );
    TESTCHECK( mat6(3,0), -4.f, &ok );
    TESTCHECK( mat6(3,1), 0.f, &ok );
    TESTCHECK( mat6(3,2), 22.f, &ok );
    TESTCHECK( mat6(3,3), 11.f, &ok );
    cout << "mat6 = mat7 * mat9" << endl;
    mat6 = mat7 * mat9;
    TESTCHECK( mat6(0,0), -11.f, &ok );
    TESTCHECK( mat6(0,1), 18.f, &ok );
    TESTCHECK( mat6(0,2), -17.f, &ok );
    TESTCHECK( mat6(0,3), -7.f, &ok );
    TESTCHECK( mat6(1,0), 7.f, &ok );
    TESTCHECK( mat6(1,1), 6.f, &ok );
    TESTCHECK( mat6(1,2), -8.f, &ok );
    TESTCHECK( mat6(1,3), -4.f, &ok );
    TESTCHECK( mat6(2,0), 44.f, &ok );
    TESTCHECK( mat6(2,1), 35.f, &ok );
    TESTCHECK( mat6(2,2), -60.f, &ok );
    TESTCHECK( mat6(2,3), -26.f, &ok );
    TESTCHECK( mat6(3,0), 25.f, &ok );
    TESTCHECK( mat6(3,1), -37.f, &ok );
    TESTCHECK( mat6(3,2), -10.f, &ok );
    TESTCHECK( mat6(3,3), 9.f, &ok );
    cout << "mat6 = mat7 * mat3" << endl;
    mat6 = mat7 * mat3;
    TESTCHECK( (mat6 == mat1), true, &ok );
    cout << "mat6 = mat3 * mat7" << endl;
    mat6 = mat3 * mat7;
    TESTCHECK( (mat6 == mat1), true, &ok );
    cout << "mat6 = mat9 * mat9.Inverse()" << endl;
    mat6 = mat9 * mat9.Inverse();
    TESTCHECKF( mat6(0,0), 1.f, &ok );
    TESTCHECKF( mat6(0,1), 0.f, &ok );
    TESTCHECKF( mat6(0,2), 0.f, &ok );
    TESTCHECKF( mat6(0,3), 0.f, &ok );
    TESTCHECKF( mat6(1,0), 0.f, &ok );
    TESTCHECKF( mat6(1,1), 1.f, &ok );
    TESTCHECKF( mat6(1,2), 0.f, &ok );
    TESTCHECKF( mat6(1,3), 0.f, &ok );
    TESTCHECKF( mat6(2,0), 0.f, &ok );
    TESTCHECKF( mat6(2,1), 0.f, &ok );
    TESTCHECKF( mat6(2,2), 1.f, &ok );
    TESTCHECKF( mat6(2,3), 0.f, &ok );
    TESTCHECKF( mat6(3,0), 0.f, &ok );
    TESTCHECKF( mat6(3,1), 0.f, &ok );
    TESTCHECKF( mat6(3,2), 0.f, &ok );
    TESTCHECKF( mat6(3,3), 1.f, &ok );
    cout << "mat6 = mat9.Inverse() * mat9" << endl;
    mat6 = mat9.Inverse() * mat9;
    TESTCHECKF( mat6(0,0), 1.f, &ok );
    TESTCHECKF( mat6(0,1), 0.f, &ok );
    TESTCHECKF( mat6(0,2), 0.f, &ok );
    TESTCHECKF( mat6(0,3), 0.f, &ok );
    TESTCHECKF( mat6(1,0), 0.f, &ok );
    TESTCHECKF( mat6(1,1), 1.f, &ok );
    TESTCHECKF( mat6(1,2), 0.f, &ok );
    TESTCHECKF( mat6(1,3), 0.f, &ok );
    TESTCHECKF( mat6(2,0), 0.f, &ok );
    TESTCHECKF( mat6(2,1), 0.f, &ok );
    TESTCHECKF( mat6(2,2), 1.f, &ok );
    TESTCHECKF( mat6(2,3), 0.f, &ok );
    TESTCHECKF( mat6(3,0), 0.f, &ok );
    TESTCHECKF( mat6(3,1), 0.f, &ok );
    TESTCHECKF( mat6(3,2), 0.f, &ok );
    TESTCHECKF( mat6(3,3), 1.f, &ok );

    if ( ok )
        cout << "Matrix4 PASSED." << endl << endl;
    else
        cout << "Matrix4 FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

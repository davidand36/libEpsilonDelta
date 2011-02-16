/*
  Matrix3.cpp
  Copyright (C) 2007 David M. Anderson

  Matrix3 template class: a 2x2 matrix.
*/


#include "Matrix3.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "AxisAngle.hpp"
#include <sstream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestMatrix3( )
{
    bool ok = true;
    cout << "Testing Matrix3" << endl;

    cout << "Matrix3F( ) [default constructor]" << endl;
    Matrix3F mat0;
    cout << "Set( )" << endl;
    mat0.Set( );
    TESTCHECK( mat0(0,0), 0.f, &ok );
    TESTCHECK( mat0(0,1), 0.f, &ok );
    TESTCHECK( mat0(0,2), 0.f, &ok );
    TESTCHECK( mat0(1,0), 0.f, &ok );
    TESTCHECK( mat0(1,1), 0.f, &ok );
    TESTCHECK( mat0(1,2), 0.f, &ok );
    TESTCHECK( mat0(2,0), 0.f, &ok );
    TESTCHECK( mat0(2,1), 0.f, &ok );
    TESTCHECK( mat0(2,2), 0.f, &ok );
    TESTCHECK( mat0.Element(0,0), 0.f, &ok );
    TESTCHECK( mat0.Element(0,1), 0.f, &ok );
    TESTCHECK( mat0(0,2), 0.f, &ok );
    try
    {
        TESTCHECK( mat0.Element(0,3), 0.f, &ok );
        cout << "Element(0,3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( mat0.Element(1,0), 0.f, &ok );
    TESTCHECK( mat0.Element(1,1), 0.f, &ok );
    TESTCHECK( mat0.Element(1,2), 0.f, &ok );
    TESTCHECK( mat0.Element(2,0), 0.f, &ok );
    TESTCHECK( mat0.Element(2,1), 0.f, &ok );
    TESTCHECK( mat0.Element(2,2), 0.f, &ok );
    try
    {
        TESTCHECK( mat0.Element(3,0), 0.f, &ok );
        cout << "Element(3,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "Row(0)" << endl;
    Vector3F v0 = mat0.Row(0);
    TESTCHECK( v0[0], 0.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    cout << "Row(1)" << endl;
    Vector3F v1 = mat0.Row(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 0.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    cout << "Row(2)" << endl;
    Vector3F v2 = mat0.Row(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 0.f, &ok );
    cout << "Column(0)" << endl;
    v0 = mat0.Column(0);
    TESTCHECK( v0[0], 0.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    cout << "Column(1)" << endl;
    v1 = mat0.Column(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 0.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    cout << "Column(2)" << endl;
    v2 = mat0.Column(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 0.f, &ok );
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
    TESTCHECK( (mat0 == Matrix3F::Zero), true, &ok );
    float f = 1.f;
    cout << "Matrix3F( " << f << " ) [T constructor]" << endl;
    Matrix3F mat1( f );
    TESTCHECK( mat1(0,0), f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), f, &ok );
    TESTCHECK( mat1.Element(0,0), 1.f, &ok );
    TESTCHECK( mat1.Element(0,1), 0.f, &ok );
    TESTCHECK( mat1.Element(0,2), 0.f, &ok );
    try
    {
        TESTCHECK( mat1.Element(0,3), 0.f, &ok );
        cout << "Element(0,3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( mat1.Element(1,0), 0.f, &ok );
    TESTCHECK( mat1.Element(1,1), 1.f, &ok );
    TESTCHECK( mat1.Element(1,2), 0.f, &ok );
    TESTCHECK( mat1.Element(2,0), 0.f, &ok );
    TESTCHECK( mat1.Element(2,1), 0.f, &ok );
    TESTCHECK( mat1.Element(2,2), 1.f, &ok );
    try
    {
        TESTCHECK( mat1.Element(3,0), 0.f, &ok );
        cout << "Element(3,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "Row(0)" << endl;
    v0 = mat1.Row(0);
    TESTCHECK( v0[0], 1.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    cout << "Row(1)" << endl;
    v1 = mat1.Row(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 1.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    cout << "Row(2)" << endl;
    v2 = mat1.Row(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 1.f, &ok );
    cout << "Column(0)" << endl;
    v0 = mat1.Column(0);
    TESTCHECK( v0[0], 1.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    TESTCHECK( v0[2], 0.f, &ok );
    cout << "Column(1)" << endl;
    v1 = mat1.Column(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 1.f, &ok );
    TESTCHECK( v1[2], 0.f, &ok );
    cout << "Column(2)" << endl;
    v2 = mat1.Column(2);
    TESTCHECK( v2[0], 0.f, &ok );
    TESTCHECK( v2[1], 0.f, &ok );
    TESTCHECK( v2[2], 1.f, &ok );
    cout << "Array()" << endl;
    arr0 = mat1.Array();
    TESTCHECK( arr0[0], 1.f, &ok );
    TESTCHECK( arr0[1], 0.f, &ok );
    TESTCHECK( arr0[2], 0.f, &ok );
    TESTCHECK( arr0[3], 0.f, &ok );
    TESTCHECK( arr0[4], 1.f, &ok );
    TESTCHECK( arr0[5], 0.f, &ok );
    TESTCHECK( arr0[6], 0.f, &ok );
    TESTCHECK( arr0[7], 0.f, &ok );
    TESTCHECK( arr0[8], 1.f, &ok );
    f = 4.f;
    cout << "Set( " << f << " )" << endl;
    mat1.Set( f );
    TESTCHECK( mat1(0,0), f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), f, &ok );
    f = 0.25f;
    cout << "*= " << f << endl;
    mat1 *= f;
    TESTCHECK( mat1(0,0), 1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), 1.f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), 1.f, &ok );
    TESTCHECK( (mat1 == Matrix3F::Identity), true, &ok );
    TESTCHECK( ToJSON( mat1 ),
               string( "[ [ +1.00000e+00, +0.00000e+00, +0.00000e+00 ],"
                       " [ +0.00000e+00, +1.00000e+00, +0.00000e+00 ],"
                       " [ +0.00000e+00, +0.00000e+00, +1.00000e+00 ] ]" ),
               &ok );
    cout << "FromJSON( ToJSON( mat1 ), &mat1 )" << endl;
    FromJSON( ToJSON( mat1 ), &mat1 );
    TESTCHECK( mat1(0,0), 1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(0,2), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), 1.f, &ok );
    TESTCHECK( mat1(1,2), 0.f, &ok );
    TESTCHECK( mat1(2,0), 0.f, &ok );
    TESTCHECK( mat1(2,1), 0.f, &ok );
    TESTCHECK( mat1(2,2), 1.f, &ok );
    float e00 = -417.f;
    float e01 = -528.f;
    float e02 = -687.f;
    float e10 = -639.f;
    float e11 = -741.f;
    float e12 = -441.f;
    float e20 = -274.f;
    float e21 = -939.f;
    float e22 = -940.f;
    cout << "Matrix3F( " << e00 << ", " << e01 << ", " << e02 << ", " << e10
         << ", " << e11 << ", " << e12 << ", " << e20 << ", " << e21 << ", "
         << e22 << " ) [element-by-element constructor]" << endl;
    Matrix3F mat2( e00, e01, e02, e10, e11, e12, e20, e21, e22 );
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(0,2), e02, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    TESTCHECK( mat2(1,2), e12, &ok );
    TESTCHECK( mat2(2,0), e20, &ok );
    TESTCHECK( mat2(2,1), e21, &ok );
    TESTCHECK( mat2(2,2), e22, &ok );
    TESTCHECK( mat2.Element(0,0), e00, &ok );
    TESTCHECK( mat2.Element(0,1), e01, &ok );
    TESTCHECK( mat2.Element(0,2), e02, &ok );
    try
    {
        TESTCHECK( mat2.Element(0,3), 0.f, &ok );
        cout << "Element(0,3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( mat2.Element(1,0), e10, &ok );
    TESTCHECK( mat2.Element(1,1), e11, &ok );
    TESTCHECK( mat2.Element(1,2), e12, &ok );
    TESTCHECK( mat2.Element(2,0), e20, &ok );
    TESTCHECK( mat2.Element(2,1), e21, &ok );
    TESTCHECK( mat2.Element(2,2), e22, &ok );
    try
    {
        TESTCHECK( mat2.Element(3,0), 0.f, &ok );
        cout << "Element(3,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "Row(0)" << endl;
    v0 = mat2.Row(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e01, &ok );
    TESTCHECK( v0[2], e02, &ok );
    cout << "Row(1)" << endl;
    v1 = mat2.Row(1);
    TESTCHECK( v1[0], e10, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e12, &ok );
    cout << "Row(2)" << endl;
    v2 = mat2.Row(2);
    TESTCHECK( v2[0], e20, &ok );
    TESTCHECK( v2[1], e21, &ok );
    TESTCHECK( v2[2], e22, &ok );
    cout << "Column(0)" << endl;
    v0 = mat2.Column(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e10, &ok );
    TESTCHECK( v0[2], e20, &ok );
    cout << "Column(1)" << endl;
    v1 = mat2.Column(1);
    TESTCHECK( v1[0], e01, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e21, &ok );
    cout << "Column(2)" << endl;
    v2 = mat2.Column(2);
    TESTCHECK( v2[0], e02, &ok );
    TESTCHECK( v2[1], e12, &ok );
    TESTCHECK( v2[2], e22, &ok );
    cout << "Array()" << endl;
    arr0 = mat2.Array();
    TESTCHECK( arr0[0], e00, &ok );
    TESTCHECK( arr0[1], e10, &ok );
    TESTCHECK( arr0[2], e20, &ok );
    TESTCHECK( arr0[3], e01, &ok );
    TESTCHECK( arr0[4], e11, &ok );
    TESTCHECK( arr0[5], e21, &ok );
    TESTCHECK( arr0[6], e02, &ok );
    TESTCHECK( arr0[7], e12, &ok );
    TESTCHECK( arr0[8], e22, &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << mat2;
    TESTCHECK( ost.str(), string( "[ [ -417, -528, -687 ],"
                                  " [ -639, -741, -441 ],"
                                  " [ -274, -939, -940 ] ]" ), &ok );
    TESTCHECK( ToJSON( mat2 ),
               string( "[ [ -4.17000e+02, -6.39000e+02, -2.74000e+02 ],"
                       " [ -5.28000e+02, -7.41000e+02, -9.39000e+02 ],"
                       " [ -6.87000e+02, -4.41000e+02, -9.40000e+02 ] ]" ),
               &ok );
    cout << "FromJSON( ToJSON( mat1 ), &mat1 )" << endl;
    FromJSON( ToJSON( mat1 ), &mat1 );
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(0,2), e02, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    TESTCHECK( mat2(1,2), e12, &ok );
    TESTCHECK( mat2(2,0), e20, &ok );
    TESTCHECK( mat2(2,1), e21, &ok );
    TESTCHECK( mat2(2,2), e22, &ok );
    e00 = 10.f;
    e01 = -9.f;
    e02 = 7.f;  //breaks pattern, but otherwise singular
    e10 = -7.f;
    e11 = 6.f;
    e12 = -5.f;
    e20 = 4.f;
    e21 = -3.f;
    e22 = 2.f;
    cout << "Set( " << e00 << ", " << e01 << ", " << e02 << ", " << e10
         << ", " << e11 << ", " << e12 << ", " << e20 << ", " << e21 << ", "
         << e22 << " )" << endl;
    mat2.Set( e00, e01, e02, e10, e11, e12, e20, e21, e22 );
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(0,2), e02, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    TESTCHECK( mat2(1,2), e12, &ok );
    TESTCHECK( mat2(2,0), e20, &ok );
    TESTCHECK( mat2(2,1), e21, &ok );
    TESTCHECK( mat2(2,2), e22, &ok );
    cout << "Matrix3F( " << e00 << ", " << e01 << ", " << e02 << ", " << e10
         << ", " << e11 << ", " << e12 << ", " << e20 << ", " << e21 << ", "
         << e22 << ", true ) [element-by-element constructor, column-major]"
         << endl;
    Matrix3F mat3( e00, e01, e02, e10, e11, e12, e20, e21, e22, true );
    TESTCHECK( mat3(0,0), e00, &ok );
    TESTCHECK( mat3(0,1), e10, &ok );
    TESTCHECK( mat3(0,2), e20, &ok );
    TESTCHECK( mat3(1,0), e01, &ok );
    TESTCHECK( mat3(1,1), e11, &ok );
    TESTCHECK( mat3(1,2), e21, &ok );
    TESTCHECK( mat3(2,0), e02, &ok );
    TESTCHECK( mat3(2,1), e12, &ok );
    TESTCHECK( mat3(2,2), e22, &ok );
    e00 = 1.f;
    e01 = 10.f;
    e02 = 100.f;
    e10 = 2.f;
    e11 = 20.f;
    e12 = 200.f;
    e20 = 3.f;
    e21 = 30.f;
    e22 = 300.f;
    cout << "Set( " << e00 << ", " << e01 << ", " << e02 << ", " << e10
         << ", " << e11 << ", " << e12 << ", " << e20 << ", " << e21 << ", "
         << e22 << ", true )" << endl;
    mat3.Set( e00, e01, e02, e10, e11, e12, e20, e21, e22, true );
    TESTCHECK( mat3(0,0), e00, &ok );
    TESTCHECK( mat3(0,1), e10, &ok );
    TESTCHECK( mat3(0,2), e20, &ok );
    TESTCHECK( mat3(1,0), e01, &ok );
    TESTCHECK( mat3(1,1), e11, &ok );
    TESTCHECK( mat3(1,2), e21, &ok );
    TESTCHECK( mat3(2,0), e02, &ok );
    TESTCHECK( mat3(2,1), e12, &ok );
    TESTCHECK( mat3(2,2), e22, &ok );
    float arr1[] = { 10.f, 11.f, 12.f, 13.f, 14.f, 15.f, 16.f, 17.f, 18.f };
    cout << "Matrix3F( { " << arr1[0] << ", " << arr1[1] << ", " << arr1[2]
         << ", " << arr1[3] << ", " << arr1[4] << ", " << arr1[5] << ", "
         << arr1[6] << ", " << arr1[7] << ", " << arr1[8]
         << " } ) [array constructor]" << endl;
    Matrix3F mat4( arr1 );
    TESTCHECK( mat4(0,0), arr1[0], &ok );
    TESTCHECK( mat4(0,1), arr1[3], &ok );
    TESTCHECK( mat4(0,2), arr1[6], &ok );
    TESTCHECK( mat4(1,0), arr1[1], &ok );
    TESTCHECK( mat4(1,1), arr1[4], &ok );
    TESTCHECK( mat4(1,2), arr1[7], &ok );
    TESTCHECK( mat4(2,0), arr1[2], &ok );
    TESTCHECK( mat4(2,1), arr1[5], &ok );
    TESTCHECK( mat4(2,2), arr1[8], &ok );
    cout << "Set( { " << arr0[0] << ", " << arr0[1] << ", " << arr0[2] << ", "
         << arr0[3] << ", " << arr1[4] << ", " << arr1[5] << ", " << arr1[6]
         << ", " << arr1[7] << ", " << arr1[8] << " } )" << endl;
    mat4.Set( arr0 );
    TESTCHECK( mat4(0,0), mat2(0,0), &ok );
    TESTCHECK( mat4(0,1), mat2(0,1), &ok );
    TESTCHECK( mat4(0,2), mat2(0,2), &ok );
    TESTCHECK( mat4(1,0), mat2(1,0), &ok );
    TESTCHECK( mat4(1,1), mat2(1,1), &ok );
    TESTCHECK( mat4(1,2), mat2(1,2), &ok );
    TESTCHECK( mat4(2,0), mat2(2,0), &ok );
    TESTCHECK( mat4(2,1), mat2(2,1), &ok );
    TESTCHECK( mat4(2,2), mat2(2,2), &ok );
    TESTCHECK( (mat4 == mat2), true, &ok );
    cout << "Matrix3F( { " << arr1[0] << ", " << arr1[1] << ", " << arr1[2]
         << ", " << arr1[3] << ", " << arr1[4] << ", " << arr1[5] << ", "
         << arr1[6] << ", " << arr1[7] << ", " << arr1[8]
         << " }, false ) [array constructor, row-major]" << endl;
    Matrix3F mat5( arr1, false );
    TESTCHECK( mat5(0,0), arr1[0], &ok );
    TESTCHECK( mat5(0,1), arr1[1], &ok );
    TESTCHECK( mat5(0,2), arr1[2], &ok );
    TESTCHECK( mat5(1,0), arr1[3], &ok );
    TESTCHECK( mat5(1,1), arr1[4], &ok );
    TESTCHECK( mat5(1,2), arr1[5], &ok );
    TESTCHECK( mat5(2,0), arr1[6], &ok );
    TESTCHECK( mat5(2,1), arr1[7], &ok );
    TESTCHECK( mat5(2,2), arr1[8], &ok );
    cout << "Set( { " << arr0[0] << ", " << arr0[1] << ", " << arr0[2]
         << ", " << arr0[3] << ", " << arr1[4] << ", " << arr1[5] << ", "
         << arr1[6] << ", " << arr1[7] << ", " << arr1[8] << " }, false )"
         << endl;
    mat5.Set( arr0, false );
    TESTCHECK( mat5(0,0), mat2(0,0), &ok );
    TESTCHECK( mat5(0,1), mat2(1,0), &ok );
    TESTCHECK( mat5(0,2), mat2(2,0), &ok );
    TESTCHECK( mat5(1,0), mat2(0,1), &ok );
    TESTCHECK( mat5(1,1), mat2(1,1), &ok );
    TESTCHECK( mat5(1,2), mat2(2,1), &ok );
    TESTCHECK( mat5(2,0), mat2(0,2), &ok );
    TESTCHECK( mat5(2,1), mat2(1,2), &ok );
    TESTCHECK( mat5(2,2), mat2(2,2), &ok );
    TESTCHECK( (mat5 == mat2), false, &ok );
    e00 = -10.f;
    e01 = -20.f;
    e02 = -30.f;
    e10 = -40.f;
    e11 = -50.f;
    e12 = -60.f;
    e20 = -70.f;
    e21 = -80.f;
    e22 = -90.f;
    cout << "v0.Set( " << e00 << ", " << e10 << ", " << e20 << " )" << endl;
    v0.Set( e00, e10, e20 );
    cout << "v1.Set( " << e01 << ", " << e11 << ", " << e21 << " )" << endl;
    v1.Set( e01, e11, e21 );
    cout << "v2.Set( " << e02 << ", " << e12 << ", " << e22 << " )" << endl;
    v2.Set( e02, e12, e22 );
    cout << "Matrix3F( v0, v1, v2 ) [column vector constructor]" << endl;
    Matrix3F mat6( v0, v1, v2 );
    TESTCHECK( mat6(0,0), e00, &ok );
    TESTCHECK( mat6(0,1), e01, &ok );
    TESTCHECK( mat6(0,2), e02, &ok );
    TESTCHECK( mat6(1,0), e10, &ok );
    TESTCHECK( mat6(1,1), e11, &ok );
    TESTCHECK( mat6(1,2), e12, &ok );
    TESTCHECK( mat6(2,0), e20, &ok );
    TESTCHECK( mat6(2,1), e21, &ok );
    TESTCHECK( mat6(2,2), e22, &ok );
    e00 = 210.f;
    e01 = 320.f;
    e02 = 430.f;
    e10 = 540.f;
    e11 = 650.f;
    e12 = 760.f;
    e20 = 870.f;
    e21 = 980.f;
    e22 = 1090.f;
    cout << "v0.Set( " << e00 << ", " << e10 << ", " << e20 << " )" << endl;
    v0.Set( e00, e10, e20 );
    cout << "v1.Set( " << e01 << ", " << e11 << ", " << e21 << " )" << endl;
    v1.Set( e01, e11, e21 );
    cout << "v2.Set( " << e02 << ", " << e12 << ", " << e22 << " )" << endl;
    v2.Set( e02, e12, e22 );
    cout << "Set( v0, v1, v2 )" << endl;
    mat6.Set( v0, v1, v2 );
    TESTCHECK( mat6(0,0), e00, &ok );
    TESTCHECK( mat6(0,1), e01, &ok );
    TESTCHECK( mat6(0,2), e02, &ok );
    TESTCHECK( mat6(1,0), e10, &ok );
    TESTCHECK( mat6(1,1), e11, &ok );
    TESTCHECK( mat6(1,2), e12, &ok );
    TESTCHECK( mat6(2,0), e20, &ok );
    TESTCHECK( mat6(2,1), e21, &ok );
    TESTCHECK( mat6(2,2), e22, &ok );
    e00 = -17.f;
    e01 = -28.f;
    e02 = -39.f;
    e10 = -40.f;
    e11 = -51.f;
    e12 = -62.f;
    e20 = -73.f;
    e21 = -84.f;
    e22 = -95.f;
    cout << "v0.Set( " << e00 << ", " << e01 << ", " << e02 << " )" << endl;
    v0.Set( e00, e01, e02 );
    cout << "v1.Set( " << e10 << ", " << e11 << ", " << e12 << " )" << endl;
    v1.Set( e10, e11, e12 );
    cout << "v2.Set( " << e20 << ", " << e21 << ", " << e22 << " )" << endl;
    v2.Set( e20, e21, e22 );
    cout << "Matrix3F( v0, v1, v2, false ) [row vector constructor]" << endl;
    Matrix3F mat7( v0, v1, v2, false );
    TESTCHECK( mat7(0,0), e00, &ok );
    TESTCHECK( mat7(0,1), e01, &ok );
    TESTCHECK( mat7(0,2), e02, &ok );
    TESTCHECK( mat7(1,0), e10, &ok );
    TESTCHECK( mat7(1,1), e11, &ok );
    TESTCHECK( mat7(1,2), e12, &ok );
    TESTCHECK( mat7(2,0), e20, &ok );
    TESTCHECK( mat7(2,1), e21, &ok );
    TESTCHECK( mat7(2,2), e22, &ok );
    e00 = 1.f;
    e01 = 2.f;
    e02 = 3.f;
    e10 = 4.f;
    e11 = 5.f;
    e12 = 6.f;
    e20 = 7.f;
    e21 = 8.f;
    e22 = 9.f;
    cout << "v0.Set( " << e00 << ", " << e01 << ", " << e02 << " )" << endl;
    v0.Set( e00, e01, e02 );
    cout << "v1.Set( " << e10 << ", " << e11 << ", " << e12 << " )" << endl;
    v1.Set( e10, e11, e12 );
    cout << "v2.Set( " << e20 << ", " << e21 << ", " << e22 << " )" << endl;
    v2.Set( e20, e21, e22 );
    cout << "Set( v0, v1, v2, false ) [row vector constructor]" << endl;
    mat7.Set( v0, v1, v2, false );
    TESTCHECK( mat7(0,0), e00, &ok );
    TESTCHECK( mat7(0,1), e01, &ok );
    TESTCHECK( mat7(0,2), e02, &ok );
    TESTCHECK( mat7(1,0), e10, &ok );
    TESTCHECK( mat7(1,1), e11, &ok );
    TESTCHECK( mat7(1,2), e12, &ok );
    TESTCHECK( mat7(2,0), e20, &ok );
    TESTCHECK( mat7(2,1), e21, &ok );
    TESTCHECK( mat7(2,2), e22, &ok );
    cout << "Row(0)" << endl;
    v0 = mat7.Row(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e01, &ok );
    TESTCHECK( v0[2], e02, &ok );
    cout << "Row(1)" << endl;
    v1 = mat7.Row(1);
    TESTCHECK( v1[0], e10, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e12, &ok );
    cout << "Row(2)" << endl;
    v2 = mat7.Row(2);
    TESTCHECK( v2[0], e20, &ok );
    TESTCHECK( v2[1], e21, &ok );
    TESTCHECK( v2[2], e22, &ok );
    cout << "Column(0)" << endl;
    v0 = mat7.Column(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e10, &ok );
    TESTCHECK( v0[2], e20, &ok );
    cout << "Column(1)" << endl;
    v1 = mat7.Column(1);
    TESTCHECK( v1[0], e01, &ok );
    TESTCHECK( v1[1], e11, &ok );
    TESTCHECK( v1[2], e21, &ok );
    cout << "Column(2)" << endl;
    v2 = mat7.Column(2);
    TESTCHECK( v2[0], e02, &ok );
    TESTCHECK( v2[1], e12, &ok );
    TESTCHECK( v2[2], e22, &ok );
    cout << "v0.Set(1,0,0)" << endl;
    v0.Set( 1.f, 0.f, 0.f );
    cout << "v1.Set(0,1,0)" << endl;
    v1.Set( 0.f, 1.f, 0.f );
    cout << "v2.Set(0,0,1)" << endl;
    v2.Set( 0.f, 0.f, 1.f );
    double a = M_PI / 2;
    Angle angle( a );
    cout << "Matrix3F( AxisAngleF( v0, Angle( " << a
         << " ) ) ) [rotation constructor]" << endl;
    Matrix3F mat8( AxisAngleF( v0, angle ) );
    TESTCHECKF( (mat8 * v0).X(), v0.X(), &ok );
    TESTCHECKF( (mat8 * v0).Y(), v0.Y(), &ok );
    TESTCHECKF( (mat8 * v0).Z(), v0.Z(), &ok );
    TESTCHECKF( (mat8 * v1).X(), v2.X(), &ok );
    TESTCHECKF( (mat8 * v1).Y(), v2.Y(), &ok );
    TESTCHECKF( (mat8 * v1).Z(), v2.Z(), &ok );
    TESTCHECKF( (mat8 * v2).X(), -v1.X(), &ok );
    TESTCHECKF( (mat8 * v2).Y(), -v1.Y(), &ok );
    TESTCHECKF( (mat8 * v2).Z(), -v1.Z(), &ok );
    int i = 0;
    cout << "Matrix3F( " << i << ", Angle( " << a
         << " ) ) [coord axis rotation constructor]" << endl;
    Matrix3F mat10( i, angle );
    TESTCHECKF( (mat10 * v0).X(), v0.X(), &ok );
    TESTCHECKF( (mat10 * v0).Y(), v0.Y(), &ok );
    TESTCHECKF( (mat10 * v0).Z(), v0.Z(), &ok );
    TESTCHECKF( (mat10 * v1).X(), v2.X(), &ok );
    TESTCHECKF( (mat10 * v1).Y(), v2.Y(), &ok );
    TESTCHECKF( (mat10 * v1).Z(), v2.Z(), &ok );
    TESTCHECKF( (mat10 * v2).X(), -v1.X(), &ok );
    TESTCHECKF( (mat10 * v2).Y(), -v1.Y(), &ok );
    TESTCHECKF( (mat10 * v2).Z(), -v1.Z(), &ok );
    a = M_PI;
    angle.Set( a );
    cout << "Set( AxisAngleF( v1, Angle( " << a << " ) ) )" << endl;
    mat8.Set( AxisAngleF( v1, angle ) );
    TESTCHECKF( (mat8 * v0).X(), -v0.X(), &ok );
    TESTCHECKF( (mat8 * v0).Y(), -v0.Y(), &ok );
    TESTCHECKF( (mat8 * v0).Z(), -v0.Z(), &ok );
    TESTCHECKF( (mat8 * v1).X(), v1.X(), &ok );
    TESTCHECKF( (mat8 * v1).Y(), v1.Y(), &ok );
    TESTCHECKF( (mat8 * v1).Z(), v1.Z(), &ok );
    TESTCHECKF( (mat8 * v2).X(), -v2.X(), &ok );
    TESTCHECKF( (mat8 * v2).Y(), -v2.Y(), &ok );
    TESTCHECKF( (mat8 * v2).Z(), -v2.Z(), &ok );
    i = 1;
    cout << "Set( " << i << ", Angle( " << a << " ) )" << endl;
    mat10.Set( i, angle );
    TESTCHECKF( (mat10 * v0).X(), -v0.X(), &ok );
    TESTCHECKF( (mat10 * v0).Y(), -v0.Y(), &ok );
    TESTCHECKF( (mat10 * v0).Z(), -v0.Z(), &ok );
    TESTCHECKF( (mat10 * v1).X(), v1.X(), &ok );
    TESTCHECKF( (mat10 * v1).Y(), v1.Y(), &ok );
    TESTCHECKF( (mat10 * v1).Z(), v1.Z(), &ok );
    TESTCHECKF( (mat10 * v2).X(), -v2.X(), &ok );
    TESTCHECKF( (mat10 * v2).Y(), -v2.Y(), &ok );
    TESTCHECKF( (mat10 * v2).Z(), -v2.Z(), &ok );
    a = -3. * M_PI / 4.;
    angle.Set( a );
    cout << "Set( AxisAngleF( v2, Angle( " << a << " ) ) )" << endl;
    mat8.Set( AxisAngleF( v2, angle ) );
    TESTCHECKF( (mat8 * v0).X(), -sqrt(0.5f), &ok );
    TESTCHECKF( (mat8 * v0).Y(), -sqrt(0.5f), &ok );
    TESTCHECKF( (mat8 * v0).Z(), 0.f, &ok );
    TESTCHECKF( (mat8 * v1).X(), sqrt(0.5f), &ok );
    TESTCHECKF( (mat8 * v1).Y(), -sqrt(0.5f), &ok );
    TESTCHECKF( (mat8 * v1).Z(), 0.f, &ok );
    TESTCHECKF( (mat8 * v2).X(), v2.X(), &ok );
    TESTCHECKF( (mat8 * v2).Y(), v2.Y(), &ok );
    TESTCHECKF( (mat8 * v2).Z(), v2.Z(), &ok );
    i = 2;
    cout << "Set( " << i << ", Angle( " << a << " ) )" << endl;
    mat10.Set( i, angle );
    TESTCHECKF( (mat10 * v0).X(), -sqrt(0.5f), &ok );
    TESTCHECKF( (mat10 * v0).Y(), -sqrt(0.5f), &ok );
    TESTCHECKF( (mat10 * v0).Z(), 0.f, &ok );
    TESTCHECKF( (mat10 * v1).X(), sqrt(0.5f), &ok );
    TESTCHECKF( (mat10 * v1).Y(), -sqrt(0.5f), &ok );
    TESTCHECKF( (mat10 * v1).Z(), 0.f, &ok );
    TESTCHECKF( (mat10 * v2).X(), v2.X(), &ok );
    TESTCHECKF( (mat10 * v2).Y(), v2.Y(), &ok );
    TESTCHECKF( (mat10 * v2).Z(), v2.Z(), &ok );
    cout << "Matrix3F( mat6 ) [copy constructor]" << endl;
    Matrix3F mat9( mat6 );
    TESTCHECK( mat9(0,0), mat6(0,0), &ok );
    TESTCHECK( mat9(0,1), mat6(0,1), &ok );
    TESTCHECK( mat9(0,2), mat6(0,2), &ok );
    TESTCHECK( mat9(1,0), mat6(1,0), &ok );
    TESTCHECK( mat9(1,1), mat6(1,1), &ok );
    TESTCHECK( mat9(1,2), mat6(1,2), &ok );
    TESTCHECK( mat9(2,0), mat6(2,0), &ok );
    TESTCHECK( mat9(2,1), mat6(2,1), &ok );
    TESTCHECK( mat9(2,2), mat6(2,2), &ok );
    TESTCHECK( (mat9 == mat9), true, &ok );
    TESTCHECK( (mat9 == mat6), true, &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    TESTCHECK( (mat9 == mat2), false, &ok );
    cout << "= mat4" << endl;
    mat9 = mat4;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat9), true, &ok );
    TESTCHECK( (mat9 == mat6), false, &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    TESTCHECK( (mat9 == mat2), true, &ok );
    f = 121.f;
    cout << "mat9(0,0) = " << f << endl;
    mat9(0,0) = f;
    TESTCHECK( mat9(0,0), f, &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(0,0) = mat4(0,0)" << endl;
    mat9.Element(0,0) = mat4(0,0);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "mat9(0,2) = " << f << endl;
    mat9(0,2) = f;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), f, &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(0,2) = mat4(0,2)" << endl;
    mat9.Element(0,2) = mat4(0,2);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    try
    {
        cout << "mat9.Element(0,3) = " << f << endl;
        mat9.Element(0,3) = f;
        cout << "Element(0,3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "mat9(2,0) = " << f << endl;
    mat9(2,0) = f;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), f, &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(2,0) = mat4(2,0)" << endl;
    mat9.Element(2,0) = mat4(2,0);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "mat9(2,2) = " << f << endl;
    mat9(2,2) = f;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), f, &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(2,2) = mat4(2,2)" << endl;
    mat9.Element(2,2) = mat4(2,2);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(0,2), mat4(0,2), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( mat9(1,2), mat4(1,2), &ok );
    TESTCHECK( mat9(2,0), mat4(2,0), &ok );
    TESTCHECK( mat9(2,1), mat4(2,1), &ok );
    TESTCHECK( mat9(2,2), mat4(2,2), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    try
    {
        cout << "mat9.Element(3,0) = " << f << endl;
        mat9.Element(3,0) = f;
        cout << "Element(3,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "+= mat2" << endl;
    mat9 += mat2;
    TESTCHECK( mat9(0,0), 20.f, &ok );
    TESTCHECK( mat9(0,1), -18.f, &ok );
    TESTCHECK( mat9(0,2), 14.f, &ok );
    TESTCHECK( mat9(1,0), -14.f, &ok );
    TESTCHECK( mat9(1,1), 12.f, &ok );
    TESTCHECK( mat9(1,2), -10.f, &ok );
    TESTCHECK( mat9(2,0), 8.f, &ok );
    TESTCHECK( mat9(2,1), -6.f, &ok );
    TESTCHECK( mat9(2,2), 4.f, &ok );
    cout << "-= mat4" << endl;
    mat9 -= mat4;
    TESTCHECK( mat9(0,0), 10.f, &ok );
    TESTCHECK( mat9(0,1), -9.f, &ok );
    TESTCHECK( mat9(0,2), 7.f, &ok );
    TESTCHECK( mat9(1,0), -7.f, &ok );
    TESTCHECK( mat9(1,1), 6.f, &ok );
    TESTCHECK( mat9(1,2), -5.f, &ok );
    TESTCHECK( mat9(2,0), 4.f, &ok );
    TESTCHECK( mat9(2,1), -3.f, &ok );
    TESTCHECK( mat9(2,2), 2.f, &ok );
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
    TESTCHECK( mat9(0,0), -20.f, &ok );
    TESTCHECK( mat9(0,1), 18.f, &ok );
    TESTCHECK( mat9(0,2), -14.f, &ok );
    TESTCHECK( mat9(1,0), 14.f, &ok );
    TESTCHECK( mat9(1,1), -12.f, &ok );
    TESTCHECK( mat9(1,2), 10.f, &ok );
    TESTCHECK( mat9(2,0), -8.f, &ok );
    TESTCHECK( mat9(2,1), 6.f, &ok );
    TESTCHECK( mat9(2,2), -4.f, &ok );
    cout << "*= mat1" << endl;
    mat9 *= mat1;
    TESTCHECK( mat9(0,0), -20.f, &ok );
    TESTCHECK( mat9(0,1), 18.f, &ok );
    TESTCHECK( mat9(0,2), -14.f, &ok );
    TESTCHECK( mat9(1,0), 14.f, &ok );
    TESTCHECK( mat9(1,1), -12.f, &ok );
    TESTCHECK( mat9(1,2), 10.f, &ok );
    TESTCHECK( mat9(2,0), -8.f, &ok );
    TESTCHECK( mat9(2,1), 6.f, &ok );
    TESTCHECK( mat9(2,2), -4.f, &ok );
    f = -0.5f;
    cout << "*= Matrix3F( " << f << " )" << endl;
    mat9 *= Matrix3F( f );
    TESTCHECK( mat9(0,0), 10.f, &ok );
    TESTCHECK( mat9(0,1), -9.f, &ok );
    TESTCHECK( mat9(0,2), 7.f, &ok );
    TESTCHECK( mat9(1,0), -7.f, &ok );
    TESTCHECK( mat9(1,1), 6.f, &ok );
    TESTCHECK( mat9(1,2), -5.f, &ok );
    TESTCHECK( mat9(2,0), 4.f, &ok );
    TESTCHECK( mat9(2,1), -3.f, &ok );
    TESTCHECK( mat9(2,2), 2.f, &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "- mat9 [negation]" << endl;
    mat9 = - mat9;
    TESTCHECK( mat9(0,0), -10.f, &ok );
    TESTCHECK( mat9(0,1), 9.f, &ok );
    TESTCHECK( mat9(0,2), -7.f, &ok );
    TESTCHECK( mat9(1,0), 7.f, &ok );
    TESTCHECK( mat9(1,1), -6.f, &ok );
    TESTCHECK( mat9(1,2), 5.f, &ok );
    TESTCHECK( mat9(2,0), -4.f, &ok );
    TESTCHECK( mat9(2,1), 3.f, &ok );
    TESTCHECK( mat9(2,2), -2.f, &ok );
    cout << "- mat9 [negation]" << endl;
    mat9 = - mat9;
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "Transpose()" << endl;
    mat9 = mat9.Transpose();
    TESTCHECK( mat9(0,0), 10.f, &ok );
    TESTCHECK( mat9(0,1), -7.f, &ok );
    TESTCHECK( mat9(0,2), 4.f, &ok );
    TESTCHECK( mat9(1,0), -9.f, &ok );
    TESTCHECK( mat9(1,1), 6.f, &ok );
    TESTCHECK( mat9(1,2), -3.f, &ok );
    TESTCHECK( mat9(2,0), 7.f, &ok );
    TESTCHECK( mat9(2,1), -5.f, &ok );
    TESTCHECK( mat9(2,2), 2.f, &ok );
    TESTCHECK( (mat9 == mat5), true, &ok );
    cout << "Transpose()" << endl;
    mat9 = mat9.Transpose();
    TESTCHECK( (mat9 == mat4), true, &ok );
    TESTCHECK( (mat5 == mat2.Transpose()), true, &ok );
    TESTCHECK( (mat5.Transpose() == mat2), true, &ok );
    TESTCHECK( mat1.Trace(), 3.f, &ok );
    cout << "mat1.Determinant()" << endl;
    f = mat1.Determinant();
    TESTCHECK( f, 1.f, &ok );
    TESTCHECK( mat0.Trace(), 0.f, &ok );
    cout << "mat0.Determinant()" << endl;
    f = mat0.Determinant();
    TESTCHECK( f, 0.f, &ok );
    TESTCHECK( mat7.Trace(), 15.f, &ok );
    cout << "mat7.Determinant()" << endl;
    f = mat7.Determinant();
    TESTCHECK( f, 0.f, &ok );
    TESTCHECK( mat9.Trace(), 18.f, &ok );
    cout << "mat9.Determinant()" << endl;
    f = mat9.Determinant();
    TESTCHECK( f, 3.f, &ok );
    TESTCHECK( (mat1.Inverse()==mat1), true, &ok );
    cout << "Inverse()" << endl;
    mat3 = mat9.Inverse();
    TESTCHECKF( mat3(0,0), -1.f, &ok );
    TESTCHECKF( mat3(0,1), -1.f, &ok );
    TESTCHECKF( mat3(0,2), 1.f, &ok );
    TESTCHECKF( mat3(1,0), -2.f, &ok );
    TESTCHECKF( mat3(1,1), -2.66666667f, &ok );
    TESTCHECKF( mat3(1,2), 0.33333333f, &ok );
    TESTCHECKF( mat3(2,0), -1.f, &ok );
    TESTCHECKF( mat3(2,1), -2.f, &ok );
    TESTCHECKF( mat3(2,2), -1.f, &ok );
    cout << "Inverse( " << f << " )" << endl;
    mat3 = mat9.Inverse( f );
    TESTCHECKF( mat3(0,0), -1.f, &ok );
    TESTCHECKF( mat3(0,1), -1.f, &ok );
    TESTCHECKF( mat3(0,2), 1.f, &ok );
    TESTCHECKF( mat3(1,0), -2.f, &ok );
    TESTCHECKF( mat3(1,1), -2.66666667f, &ok );
    TESTCHECKF( mat3(1,2), 0.33333333f, &ok );
    TESTCHECKF( mat3(2,0), -1.f, &ok );
    TESTCHECKF( mat3(2,1), -2.f, &ok );
    TESTCHECKF( mat3(2,2), -1.f, &ok );
    try
    {
        cout << "mat7.Inverse()" << endl;
        mat3 = mat7.Inverse();
        cout << "mat7.Inverse() should have thrown an exception." << endl;
        ok = false;
    }
    catch( SingularMatrixException & exceptn )
    {
        cout << "SingularMatrixException here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    cout << "mat6 = mat9 + mat7" << endl;
    mat6 = mat9 + mat7;
    TESTCHECK( mat6(0,0), 11.f, &ok );
    TESTCHECK( mat6(0,1), -7.f, &ok );
    TESTCHECK( mat6(0,2), 10.f, &ok );
    TESTCHECK( mat6(1,0), -3.f, &ok );
    TESTCHECK( mat6(1,1), 11.f, &ok );
    TESTCHECK( mat6(1,2), 1.f, &ok );
    TESTCHECK( mat6(2,0), 11.f, &ok );
    TESTCHECK( mat6(2,1), 5.f, &ok );
    TESTCHECK( mat6(2,2), 11.f, &ok );
    cout << "mat6 = mat9 - mat7" << endl;
    mat6 = mat9 - mat7;
    TESTCHECK( mat6(0,0), 9.f, &ok );
    TESTCHECK( mat6(0,1), -11.f, &ok );
    TESTCHECK( mat6(0,2), 4.f, &ok );
    TESTCHECK( mat6(1,0), -11.f, &ok );
    TESTCHECK( mat6(1,1), 1.f, &ok );
    TESTCHECK( mat6(1,2), -11.f, &ok );
    TESTCHECK( mat6(2,0), -3.f, &ok );
    TESTCHECK( mat6(2,1), -11.f, &ok );
    TESTCHECK( mat6(2,2), -7.f, &ok );
    f = -2.f;
    cout << "mat6 = " << f << " * mat9" << endl;
    mat6 = f * mat9;
    TESTCHECK( mat6(0,0), -20.f, &ok );
    TESTCHECK( mat6(0,1), 18.f, &ok );
    TESTCHECK( mat6(0,2), -14.f, &ok );
    TESTCHECK( mat6(1,0), 14.f, &ok );
    TESTCHECK( mat6(1,1), -12.f, &ok );
    TESTCHECK( mat6(1,2), 10.f, &ok );
    TESTCHECK( mat6(2,0), -8.f, &ok );
    TESTCHECK( mat6(2,1), 6.f, &ok );
    TESTCHECK( mat6(2,2), -4.f, &ok );
    f = -0.5f;
    cout << "mat6 = mat6 * " << f << endl;
    mat6 = mat6 * f;
    TESTCHECK( (mat6 == mat4), true, &ok );
    cout << "mat6 = mat9 * mat7" << endl;
    mat6 = mat9 * mat7;
    TESTCHECK( mat6(0,0), 23.f, &ok );
    TESTCHECK( mat6(0,1), 31.f, &ok );
    TESTCHECK( mat6(0,2), 39.f, &ok );
    TESTCHECK( mat6(1,0), -18.f, &ok );
    TESTCHECK( mat6(1,1), -24.f, &ok );
    TESTCHECK( mat6(1,2), -30.f, &ok );
    TESTCHECK( mat6(2,0), 6.f, &ok );
    TESTCHECK( mat6(2,1), 9.f, &ok );
    TESTCHECK( mat6(2,2), 12.f, &ok );
    cout << "mat6 = mat7 * mat9" << endl;
    mat6 = mat7 * mat9;
    TESTCHECK( mat6(0,0), 8.f, &ok );
    TESTCHECK( mat6(0,1), -6.f, &ok );
    TESTCHECK( mat6(0,2), 3.f, &ok );
    TESTCHECK( mat6(1,0), 29.f, &ok );
    TESTCHECK( mat6(1,1), -24.f, &ok );
    TESTCHECK( mat6(1,2), 15.f, &ok );
    TESTCHECK( mat6(2,0), 50.f, &ok );
    TESTCHECK( mat6(2,1), -42.f, &ok );
    TESTCHECK( mat6(2,2), 27.f, &ok );
    cout << "mat6 = mat9 * mat3" << endl;
    mat6 = mat9 * mat3;
    TESTCHECKF( mat6(0,0), 1.f, &ok );
    TESTCHECKF( mat6(0,1), 0.f, &ok );
    TESTCHECKF( mat6(0,2), 0.f, &ok );
    TESTCHECKF( mat6(1,0), 0.f, &ok );
    TESTCHECKF( mat6(1,1), 1.f, &ok );
    TESTCHECKF( mat6(1,2), 0.f, &ok );
    TESTCHECKF( mat6(2,0), 0.f, &ok );
    TESTCHECKF( mat6(2,1), 0.f, &ok );
    TESTCHECKF( mat6(2,2), 1.f, &ok );
    cout << "mat6 = mat3 * mat9" << endl;
    mat6 = mat3 * mat9;
    TESTCHECKF( mat6(0,0), 1.f, &ok );
    TESTCHECKF( mat6(0,1), 0.f, &ok );
    TESTCHECKF( mat6(0,2), 0.f, &ok );
    TESTCHECKFE( mat6(1,0), 0.f, &ok, 1.5e-6f );
    TESTCHECKF( mat6(1,1), 1.f, &ok );
    TESTCHECKF( mat6(1,2), 0.f, &ok );
    TESTCHECKF( mat6(2,0), 0.f, &ok );
    TESTCHECKF( mat6(2,1), 0.f, &ok );
    TESTCHECKF( mat6(2,2), 1.f, &ok );

    if ( ok )
        cout << "Matrix3 PASSED." << endl << endl;
    else
        cout << "Matrix3 FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  Matrix2.cpp
  Copyright (C) 2007 David M. Anderson

  Matrix2 template class: a 2x2 matrix.
*/


#include "Matrix2.hpp"
#include "Assert.hpp"
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
TestMatrix2( )
{
    bool ok = true;
    cout << "Testing Matrix2" << endl;

    cout << "Matrix2F( ) [default constructor]" << endl;
    Matrix2F mat0;
    cout << "Set( )" << endl;
    mat0.Set( );
    TESTCHECK( mat0(0,0), 0.f, &ok );
    TESTCHECK( mat0(0,1), 0.f, &ok );
    TESTCHECK( mat0(1,0), 0.f, &ok );
    TESTCHECK( mat0(1,1), 0.f, &ok );
    TESTCHECK( mat0.Element(0,0), 0.f, &ok );
    TESTCHECK( mat0.Element(0,1), 0.f, &ok );
    try
    {
        TESTCHECK( mat0.Element(0,2), 0.f, &ok );
        cout << "Element(0,2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( mat0.Element(1,0), 0.f, &ok );
    TESTCHECK( mat0.Element(1,1), 0.f, &ok );
    try
    {
        TESTCHECK( mat0.Element(2,0), 0.f, &ok );
        cout << "Element(0,2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "Row(0)" << endl;
    Vector2F v0 = mat0.Row(0);
    TESTCHECK( v0[0], 0.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    cout << "Row(1)" << endl;
    Vector2F v1 = mat0.Row(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 0.f, &ok );
    cout << "Column(0)" << endl;
    v0 = mat0.Column(0);
    TESTCHECK( v0[0], 0.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    cout << "Column(1)" << endl;
    v1 = mat0.Column(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 0.f, &ok );
    cout << "Array()" << endl;
    float * arr0 = mat0.Array();
    TESTCHECK( arr0[0], 0.f, &ok );
    TESTCHECK( arr0[1], 0.f, &ok );
    TESTCHECK( arr0[2], 0.f, &ok );
    TESTCHECK( arr0[3], 0.f, &ok );
    TESTCHECK( (mat0 == Matrix2F::Zero), true, &ok );
    float f = 1.f;
    cout << "Matrix2F( " << f << " ) [T constructor]" << endl;
    Matrix2F mat1( f );
    TESTCHECK( mat1(0,0), f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), f, &ok );
    TESTCHECK( mat1.Element(0,0), 1.f, &ok );
    TESTCHECK( mat1.Element(0,1), 0.f, &ok );
    try
    {
        TESTCHECK( mat1.Element(0,2), 0.f, &ok );
        cout << "Element(0,2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( mat1.Element(1,0), 0.f, &ok );
    TESTCHECK( mat1.Element(1,1), 1.f, &ok );
    try
    {
        TESTCHECK( mat1.Element(2,0), 0.f, &ok );
        cout << "Element(2,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "Row(0)" << endl;
    v0 = mat1.Row(0);
    TESTCHECK( v0[0], 1.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    cout << "Row(1)" << endl;
    v1 = mat1.Row(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 1.f, &ok );
    cout << "Column(0)" << endl;
    v0 = mat1.Column(0);
    TESTCHECK( v0[0], 1.f, &ok );
    TESTCHECK( v0[1], 0.f, &ok );
    cout << "Column(1)" << endl;
    v1 = mat1.Column(1);
    TESTCHECK( v1[0], 0.f, &ok );
    TESTCHECK( v1[1], 1.f, &ok );
    cout << "Array()" << endl;
    arr0 = mat1.Array();
    TESTCHECK( arr0[0], 1.f, &ok );
    TESTCHECK( arr0[1], 0.f, &ok );
    TESTCHECK( arr0[2], 0.f, &ok );
    TESTCHECK( arr0[3], 1.f, &ok );
    f = 4.f;
    cout << "Set( " << f << " )" << endl;
    mat1.Set( f );
    TESTCHECK( mat1(0,0), f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), f, &ok );
    f = 0.25f;
    cout << "*= " << f << endl;
    mat1 *= f;
    TESTCHECK( mat1(0,0), 1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), 1.f, &ok );
    TESTCHECK( (mat1 == Matrix2F::Identity), true, &ok );
    TESTCHECK( ToJSON( mat1 ),
               string( "[ [ +1.00000e+00, +0.00000e+00 ],"
                       " [ +0.00000e+00, +1.00000e+00 ] ]" ),
               &ok );
    cout << "FromJSON( ToJSON( mat1 ), &mat1 )" << endl;
    FromJSON( ToJSON( mat1 ), &mat1 );
    TESTCHECK( mat1(0,0), 1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), 1.f, &ok );
    float e00 = -417.f;
    float e01 = -528.f;
    float e10 = -639.f;
    float e11 = -741.f;
    cout << "Matrix2F( " << e00 << ", " << e01 << ", " << e10 << ", " << e11 << " ) [element-by-element constructor]" << endl;
    Matrix2F mat2( e00, e01, e10, e11 );
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    TESTCHECK( mat2.Element(0,0), e00, &ok );
    TESTCHECK( mat2.Element(0,1), e01, &ok );
    try
    {
        TESTCHECK( mat2.Element(0,2), 0.f, &ok );
        cout << "Element(0,2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( mat2.Element(1,0), e10, &ok );
    TESTCHECK( mat2.Element(1,1), e11, &ok );
    try
    {
        TESTCHECK( mat2.Element(2,0), 0.f, &ok );
        cout << "Element(2,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "Row(0)" << endl;
    v0 = mat2.Row(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e01, &ok );
    cout << "Row(1)" << endl;
    v1 = mat2.Row(1);
    TESTCHECK( v1[0], e10, &ok );
    TESTCHECK( v1[1], e11, &ok );
    cout << "Column(0)" << endl;
    v0 = mat2.Column(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e10, &ok );
    cout << "Column(1)" << endl;
    v1 = mat2.Column(1);
    TESTCHECK( v1[0], e01, &ok );
    TESTCHECK( v1[1], e11, &ok );
    cout << "Array()" << endl;
    arr0 = mat2.Array();
    TESTCHECK( arr0[0], e00, &ok );
    TESTCHECK( arr0[1], e10, &ok );
    TESTCHECK( arr0[2], e01, &ok );
    TESTCHECK( arr0[3], e11, &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << mat2;
    TESTCHECK( ost.str(), string( "[ [ -417, -528 ], [ -639, -741 ] ]" ), &ok );
    TESTCHECK( ToJSON( mat2 ),
               string( "[ [ -4.17000e+02, -6.39000e+02 ],"
                       " [ -5.28000e+02, -7.41000e+02 ] ]" ),
               &ok );
    cout << "FromJSON( ToJSON( mat2 ), &mat2 )" << endl;
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    FromJSON( ToJSON( mat1 ), &mat1 );
    e00 = -7.f;
    e01 = 5.f;
    e10 = -3.f;
    e11 = 2.f;
    cout << "Set( " << e00 << ", " << e01 << ", " << e10 << ", " << e11 << " )" << endl;
    mat2.Set( e00, e01, e10, e11 );
    TESTCHECK( mat2(0,0), e00, &ok );
    TESTCHECK( mat2(0,1), e01, &ok );
    TESTCHECK( mat2(1,0), e10, &ok );
    TESTCHECK( mat2(1,1), e11, &ok );
    cout << "Matrix2F( " << e00 << ", " << e01 << ", " << e10 << ", " << e11 << ", true ) [element-by-element constructor, column-major]" << endl;
    Matrix2F mat3( e00, e01, e10, e11, true );
    TESTCHECK( mat3(0,0), e00, &ok );
    TESTCHECK( mat3(0,1), e10, &ok );
    TESTCHECK( mat3(1,0), e01, &ok );
    TESTCHECK( mat3(1,1), e11, &ok );
    e00 = 0.f;
    e01 = 10.f;
    e10 = 100.f;
    e11 = 1000.f;
    cout << "Set( " << e00 << ", " << e01 << ", " << e10 << ", " << e11 << ", true )" << endl;
    mat3.Set( e00, e01, e10, e11, true );
    TESTCHECK( mat3(0,0), e00, &ok );
    TESTCHECK( mat3(0,1), e10, &ok );
    TESTCHECK( mat3(1,0), e01, &ok );
    TESTCHECK( mat3(1,1), e11, &ok );
    float arr1[] = { 10.f, 11.f, 12.f, 13.f };
    cout << "Matrix2F( { " << arr1[0] << ", " << arr1[1] << ", " << arr1[2] << ", " << arr1[3] << " } ) [array constructor]" << endl;
    Matrix2F mat4( arr1 );
    TESTCHECK( mat4(0,0), arr1[0], &ok );
    TESTCHECK( mat4(0,1), arr1[2], &ok );
    TESTCHECK( mat4(1,0), arr1[1], &ok );
    TESTCHECK( mat4(1,1), arr1[3], &ok );
    cout << "Set( { " << arr0[0] << ", " << arr0[1] << ", " << arr0[2] << ", " << arr0[3] << " } )" << endl;
    mat4.Set( arr0 );
    TESTCHECK( mat4(0,0), mat2(0,0), &ok );
    TESTCHECK( mat4(0,1), mat2(0,1), &ok );
    TESTCHECK( mat4(1,0), mat2(1,0), &ok );
    TESTCHECK( mat4(1,1), mat2(1,1), &ok );
    TESTCHECK( (mat4 == mat2), true, &ok );
    cout << "Matrix2F( { " << arr1[0] << ", " << arr1[1] << ", " << arr1[2] << ", " << arr1[3] << " }, false ) [array constructor, row-major]" << endl;
    Matrix2F mat5( arr1, false );
    TESTCHECK( mat5(0,0), arr1[0], &ok );
    TESTCHECK( mat5(0,1), arr1[1], &ok );
    TESTCHECK( mat5(1,0), arr1[2], &ok );
    TESTCHECK( mat5(1,1), arr1[3], &ok );
    cout << "Set( { " << arr0[0] << ", " << arr0[1] << ", " << arr0[2] << ", " << arr0[3] << " }, false )" << endl;
    mat5.Set( arr0, false );
    TESTCHECK( mat5(0,0), mat2(0,0), &ok );
    TESTCHECK( mat5(0,1), mat2(1,0), &ok );
    TESTCHECK( mat5(1,0), mat2(0,1), &ok );
    TESTCHECK( mat5(1,1), mat2(1,1), &ok );
    TESTCHECK( (mat5 == mat2), false, &ok );
    e00 = -10.f;
    e01 = -20.f;
    e10 = -30.f;
    e11 = -40.f;
    cout << "v0.Set( " << e00 << ", " << e10 << " )" << endl;
    v0.Set( e00, e10 );
    cout << "v1.Set( " << e01 << ", " << e11 << " )" << endl;
    v1.Set( e01, e11 );
    cout << "Matrix2F( v0, v1 ) [column vector constructor]" << endl;
    Matrix2F mat6( v0, v1 );
    TESTCHECK( mat6(0,0), e00, &ok );
    TESTCHECK( mat6(0,1), e01, &ok );
    TESTCHECK( mat6(1,0), e10, &ok );
    TESTCHECK( mat6(1,1), e11, &ok );
    e00 = 210.f;
    e01 = 320.f;
    e10 = 430.f;
    e11 = 540.f;
    cout << "v0.Set( " << e00 << ", " << e10 << " )" << endl;
    v0.Set( e00, e10 );
    cout << "v1.Set( " << e01 << ", " << e11 << " )" << endl;
    v1.Set( e01, e11 );
    cout << "Set( v0, v1 )" << endl;
    mat6.Set( v0, v1 );
    TESTCHECK( mat6(0,0), e00, &ok );
    TESTCHECK( mat6(0,1), e01, &ok );
    TESTCHECK( mat6(1,0), e10, &ok );
    TESTCHECK( mat6(1,1), e11, &ok );
    e00 = -17.f;
    e01 = -28.f;
    e10 = -39.f;
    e11 = -41.f;
    cout << "v0.Set( " << e00 << ", " << e01 << " )" << endl;
    v0.Set( e00, e01 );
    cout << "v1.Set( " << e10 << ", " << e11 << " )" << endl;
    v1.Set( e10, e11 );
    cout << "Matrix2F( v0, v1, false ) [row vector constructor]" << endl;
    Matrix2F mat7( v0, v1, false );
    TESTCHECK( mat7(0,0), e00, &ok );
    TESTCHECK( mat7(0,1), e01, &ok );
    TESTCHECK( mat7(1,0), e10, &ok );
    TESTCHECK( mat7(1,1), e11, &ok );
    e00 = 1.f;
    e01 = 2.f;
    e10 = 3.f;
    e11 = 4.f;
    cout << "v0.Set( " << e00 << ", " << e01 << " )" << endl;
    v0.Set( e00, e01 );
    cout << "v1.Set( " << e10 << ", " << e11 << " )" << endl;
    v1.Set( e10, e11 );
    cout << "Set( v0, v1, false )" << endl;
    mat7.Set( v0, v1, false );
    TESTCHECK( mat7(0,0), e00, &ok );
    TESTCHECK( mat7(0,1), e01, &ok );
    TESTCHECK( mat7(1,0), e10, &ok );
    TESTCHECK( mat7(1,1), e11, &ok );
    cout << "Row(0)" << endl;
    v0 = mat7.Row(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e01, &ok );
    cout << "Row(1)" << endl;
    v1 = mat7.Row(1);
    TESTCHECK( v1[0], e10, &ok );
    TESTCHECK( v1[1], e11, &ok );
    cout << "Column(0)" << endl;
    v0 = mat7.Column(0);
    TESTCHECK( v0[0], e00, &ok );
    TESTCHECK( v0[1], e10, &ok );
    cout << "Column(1)" << endl;
    v1 = mat7.Column(1);
    TESTCHECK( v1[0], e01, &ok );
    TESTCHECK( v1[1], e11, &ok );
    double a = M_PI / 2;
    Angle angle( a );
    cout << "Matrix2F( Angle( " << a << " ) ) [angle constructor]" << endl;
    Matrix2F mat8( angle );
    TESTCHECKF( mat8(0,0), 0.f, &ok );
    TESTCHECKF( mat8(0,1), -1.f, &ok );
    TESTCHECKF( mat8(1,0), 1.f, &ok );
    TESTCHECKF( mat8(1,1), 0.f, &ok );
    a = M_PI;
    angle.Set( a );
    cout << "Set( Angle( " << a << " ) ) [angle constructor]" << endl;
    mat8.Set( angle );
    TESTCHECKF( mat8(0,0), -1.f, &ok );
    TESTCHECKF( mat8(0,1), 0.f, &ok );
    TESTCHECKF( mat8(1,0), 0.f, &ok );
    TESTCHECKF( mat8(1,1), -1.f, &ok );
    cout << "Matrix2F( mat6 ) [copy constructor]" << endl;
    Matrix2F mat9( mat6 );
    TESTCHECK( mat9(0,0), mat6(0,0), &ok );
    TESTCHECK( mat9(0,1), mat6(0,1), &ok );
    TESTCHECK( mat9(1,0), mat6(1,0), &ok );
    TESTCHECK( mat9(1,1), mat6(1,1), &ok );
    TESTCHECK( (mat9 == mat9), true, &ok );
    TESTCHECK( (mat9 == mat6), true, &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    TESTCHECK( (mat9 == mat2), false, &ok );
    cout << "= mat4" << endl;
    mat9 = mat4;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
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
    try
    {
        cout << "mat9.Element(0,2) = " << f << endl;
        mat9.Element(0,2) = f;
        cout << "Element(0,2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
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
    cout << "mat9(1,1) = " << f << endl;
    mat9(1,1) = f;
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), f, &ok );
    TESTCHECK( (mat9 == mat4), false, &ok );
    cout << "mat9.Element(1,1) = mat4(1,1)" << endl;
    mat9.Element(1,1) = mat4(1,1);
    TESTCHECK( mat9(0,0), mat4(0,0), &ok );
    TESTCHECK( mat9(0,1), mat4(0,1), &ok );
    TESTCHECK( mat9(1,0), mat4(1,0), &ok );
    TESTCHECK( mat9(1,1), mat4(1,1), &ok );
    TESTCHECK( (mat9 == mat4), true, &ok );
    try
    {
        cout << "mat9.Element(2,0) = " << f << endl;
        mat9.Element(2,0) = f;
        cout << "Element(2,0) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "+= mat2" << endl;
    mat9 += mat2;
    TESTCHECK( mat9(0,0), -14.f, &ok );
    TESTCHECK( mat9(0,1), 10.f, &ok );
    TESTCHECK( mat9(1,0), -6.f, &ok );
    TESTCHECK( mat9(1,1), 4.f, &ok );
    cout << "-= mat4" << endl;
    mat9 -= mat4;
    TESTCHECK( mat9(0,0), -7.f, &ok );
    TESTCHECK( mat9(0,1), 5.f, &ok );
    TESTCHECK( mat9(1,0), -3.f, &ok );
    TESTCHECK( mat9(1,1), 2.f, &ok );
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
    TESTCHECK( mat9(1,0), 6.f, &ok );
    TESTCHECK( mat9(1,1), -4.f, &ok );
    cout << "*= mat8" << endl;
    mat9 *= mat8;
    TESTCHECK( mat9(0,0), -14.f, &ok );
    TESTCHECK( mat9(0,1), 10.f, &ok );
    TESTCHECK( mat9(1,0), -6.f, &ok );
    TESTCHECK( mat9(1,1), 4.f, &ok );
    cout << "*= mat1" << endl;
    mat9 *= mat1;
    TESTCHECK( mat9(0,0), -14.f, &ok );
    TESTCHECK( mat9(0,1), 10.f, &ok );
    TESTCHECK( mat9(1,0), -6.f, &ok );
    TESTCHECK( mat9(1,1), 4.f, &ok );
    f = 0.5f;
    cout << "*= Matrix2F( " << f << " )" << endl;
    mat9 *= Matrix2F( f );
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "- mat9 [negation]" << endl;
    mat9 = - mat9;
    TESTCHECK( mat9(0,0), 7.f, &ok );
    TESTCHECK( mat9(0,1), -5.f, &ok );
    TESTCHECK( mat9(1,0), 3.f, &ok );
    TESTCHECK( mat9(1,1), -2.f, &ok );
    cout << "- mat9 [negation]" << endl;
    mat9 = - mat9;
    TESTCHECK( (mat9 == mat4), true, &ok );
    cout << "Transpose()" << endl;
    mat9 = mat9.Transpose();
    TESTCHECK( mat9(0,0), -7.f, &ok );
    TESTCHECK( mat9(0,1), -3.f, &ok );
    TESTCHECK( mat9(1,0), 5.f, &ok );
    TESTCHECK( mat9(1,1), 2.f, &ok );
    TESTCHECK( (mat9 == mat5), true, &ok );
    cout << "Transpose()" << endl;
    mat9 = mat9.Transpose();
    TESTCHECK( (mat9 == mat4), true, &ok );
    TESTCHECK( (mat5 == mat2.Transpose()), true, &ok );
    TESTCHECK( (mat5.Transpose() == mat2), true, &ok );
    TESTCHECK( mat1.Trace(), 2.f, &ok );
    cout << "mat1.Determinant()" << endl;
    f = mat1.Determinant();
    TESTCHECK( f, 1.f, &ok );
    TESTCHECK( mat0.Trace(), 0.f, &ok );
    cout << "mat0.Determinant()" << endl;
    f = mat0.Determinant();
    TESTCHECK( f, 0.f, &ok );
    TESTCHECK( mat7.Trace(), 5.f, &ok );
    cout << "mat7.Determinant()" << endl;
    f = mat7.Determinant();
    TESTCHECK( f, -2.f, &ok );
    TESTCHECK( mat9.Trace(), -5.f, &ok );
    cout << "mat9.Determinant()" << endl;
    f = mat9.Determinant();
    TESTCHECK( f, 1.f, &ok );
    cout << "Inverse()" << endl;
    mat3 = mat9.Inverse();
    TESTCHECK( mat3(0,0), 2.f, &ok );
    TESTCHECK( mat3(0,1), -5.f, &ok );
    TESTCHECK( mat3(1,0), 3.f, &ok );
    TESTCHECK( mat3(1,1), -7.f, &ok );
    cout << "Inverse( " << f << " )" << endl;
    mat3 = mat9.Inverse( f );
    TESTCHECK( mat3(0,0), 2.f, &ok );
    TESTCHECK( mat3(0,1), -5.f, &ok );
    TESTCHECK( mat3(1,0), 3.f, &ok );
    TESTCHECK( mat3(1,1), -7.f, &ok );
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
    TESTCHECK( mat6(1,0), 0.f, &ok );
    TESTCHECK( mat6(1,1), 6.f, &ok );
    cout << "mat6 = mat9 - mat7" << endl;
    mat6 = mat9 - mat7;
    TESTCHECK( mat6(0,0), -8.f, &ok );
    TESTCHECK( mat6(0,1), 3.f, &ok );
    TESTCHECK( mat6(1,0), -6.f, &ok );
    TESTCHECK( mat6(1,1), -2.f, &ok );
    f = -2.f;
    cout << "mat6 = " << f << " * mat9" << endl;
    mat6 = f * mat9;
    TESTCHECK( mat6(0,0), 14.f, &ok );
    TESTCHECK( mat6(0,1), -10.f, &ok );
    TESTCHECK( mat6(1,0), 6.f, &ok );
    TESTCHECK( mat6(1,1), -4.f, &ok );
    f = -0.5f;
    cout << "mat6 = mat6 * " << f << endl;
    mat6 = mat6 * f;
    TESTCHECK( (mat6 == mat4), true, &ok );
    cout << "mat6 = mat9 * mat7" << endl;
    mat6 = mat9 * mat7;
    TESTCHECK( mat6(0,0), 8.f, &ok );
    TESTCHECK( mat6(0,1), 6.f, &ok );
    TESTCHECK( mat6(1,0), 3.f, &ok );
    TESTCHECK( mat6(1,1), 2.f, &ok );
    cout << "mat6 = mat7 * mat9" << endl;
    mat6 = mat7 * mat9;
    TESTCHECK( mat6(0,0), -13.f, &ok );
    TESTCHECK( mat6(0,1), 9.f, &ok );
    TESTCHECK( mat6(1,0), -33.f, &ok );
    TESTCHECK( mat6(1,1), 23.f, &ok );
    cout << "mat6 = mat9 * mat3" << endl;
    mat6 = mat9 * mat3;
    TESTCHECK( (mat6 == mat1), true, &ok );
    cout << "mat6 = mat3 * mat9" << endl;
    mat6 = mat3 * mat9;
    TESTCHECK( (mat6 == mat1), true, &ok );
    cout << "mat6 = mat7 * mat7.Inverse()" << endl;
    mat6 = mat7 * mat7.Inverse();
    TESTCHECK( (mat6 == mat1), true, &ok );
    cout << "mat6 = mat7.Inverse() * mat7" << endl;
    mat6 = mat7.Inverse() * mat7;
    TESTCHECK( (mat6 == mat1), true, &ok );

    if ( ok )
        cout << "Matrix2 PASSED." << endl << endl;
    else
        cout << "Matrix2 FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

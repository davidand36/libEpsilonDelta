/*
  RotationMatrix2.cpp
  Copyright (C) 2007 David M. Anderson

  RotationMatrix2 template class: a 2x2 matrix representing a rotation about
  the origin.
*/


#include "RotationMatrix2.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Vector2.hpp"
#include <sstream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestRotationMatrix2( )
{
    bool ok = true;
    cout << "Testing RotationMatrix2" << endl;

    cout << "RotationMatrix2F( ) [default constructor]" << endl;
    RotationMatrix2F mat0;
    cout << "Set( )" << endl;
    mat0.Set( );
    TESTCHECK( mat0(0,0), 1.f, &ok );
    TESTCHECK( mat0(0,1), 0.f, &ok );
    TESTCHECK( mat0(1,0), 0.f, &ok );
    TESTCHECK( mat0(1,1), 1.f, &ok );
    TESTCHECK( mat0.GetAngle().Radians(), 0., &ok );
    cout << "Transpose()" << endl;
    mat0 = mat0.Transpose( );
    TESTCHECK( mat0(0,0), 1.f, &ok );
    TESTCHECK( mat0(0,1), 0.f, &ok );
    TESTCHECK( mat0(1,0), 0.f, &ok );
    TESTCHECK( mat0(1,1), 1.f, &ok );
    TESTCHECK( mat0.GetAngle().Radians(), 0., &ok );
    cout << "Inverse()" << endl;
    mat0 = mat0.Inverse( );
    TESTCHECK( mat0(0,0), 1.f, &ok );
    TESTCHECK( mat0(0,1), 0.f, &ok );
    TESTCHECK( mat0(1,0), 0.f, &ok );
    TESTCHECK( mat0(1,1), 1.f, &ok );
    TESTCHECK( mat0.GetAngle().Radians(), 0., &ok );
    cout << "Vector2F vecX( 1., 0. )" << endl;
    Vector2F vecX( 1.f, 0.f );
    cout << "mat0 * vecX" << endl;
    Vector2F prodVec = mat0 * vecX;
    TESTCHECK( prodVec.X(), 1.f, &ok );
    TESTCHECK( prodVec.Y(), 0.f, &ok );
    cout << "mat1 = - mat0" << endl;
    RotationMatrix2F mat1 = - mat0;
    TESTCHECK( mat1(0,0), -1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), -1.f, &ok );
    Angle angle = mat1.GetAngle();
    angle.NormalizePositive( );
    TESTCHECK( angle.Radians(), M_PI, &ok );
    cout << "Transpose()" << endl;
    mat1 = mat1.Transpose( );
    TESTCHECK( mat1(0,0), -1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), -1.f, &ok );
    angle = mat1.GetAngle();
    angle.NormalizePositive( );
    TESTCHECK( angle.Radians(), M_PI, &ok );
    cout << "Inverse()" << endl;
    mat1 = mat1.Inverse( );
    TESTCHECK( mat1(0,0), -1.f, &ok );
    TESTCHECK( mat1(0,1), 0.f, &ok );
    TESTCHECK( mat1(1,0), 0.f, &ok );
    TESTCHECK( mat1(1,1), -1.f, &ok );
    angle = mat1.GetAngle();
    angle.NormalizePositive( );
    TESTCHECK( angle.Radians(), M_PI, &ok );
    cout << "mat1 * vecX" << endl;
    prodVec = mat1 * vecX;
    TESTCHECK( prodVec.X(), -1.f, &ok );
    TESTCHECK( prodVec.Y(), 0.f, &ok );
    cout << "RotationMatrix2( M_PI / 4 )" << endl;
    RotationMatrix2F mat2( M_PI / 4. );
    float sqrt1_2 = sqrt( 0.5f );
    TESTCHECKF( mat2(0,0), sqrt1_2, &ok );
    TESTCHECKF( mat2(0,1), -sqrt1_2, &ok );
    TESTCHECKF( mat2(1,0), sqrt1_2, &ok );
    TESTCHECKF( mat2(1,1), sqrt1_2, &ok );
    TESTCHECKF( mat2.GetAngle().Radians(), M_PI / 4., &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << mat2;
    TESTCHECK( ost.str(),
               string( "[ [ 0.707107, -0.707107 ], [ 0.707107, 0.707107 ] ]" ),
               &ok );
    cout << "mat2 * vecX" << endl;
    prodVec = mat2 * vecX;
    TESTCHECKF( prodVec.X(), sqrt1_2, &ok );
    TESTCHECKF( prodVec.Y(), sqrt1_2, &ok );
    cout << "mat3 = -mat2" << endl;
    RotationMatrix2F mat3 = -mat2;
    TESTCHECKF( mat3(0,0), -sqrt1_2, &ok );
    TESTCHECKF( mat3(0,1), sqrt1_2, &ok );
    TESTCHECKF( mat3(1,0), -sqrt1_2, &ok );
    TESTCHECKF( mat3(1,1), -sqrt1_2, &ok );
    TESTCHECKF( mat3.GetAngle().Radians(), (-3. * M_PI / 4.), &ok );
    cout << "Transpose()" << endl;
    mat3 = mat3.Transpose( );
    TESTCHECKF( mat3(0,0), -sqrt1_2, &ok );
    TESTCHECKF( mat3(0,1), -sqrt1_2, &ok );
    TESTCHECKF( mat3(1,0), sqrt1_2, &ok );
    TESTCHECKF( mat3(1,1), -sqrt1_2, &ok );
    TESTCHECKF( mat3.GetAngle().Radians(), (3. * M_PI / 4.), &ok );
    cout << "Inverse()" << endl;
    mat3 = mat3.Inverse( );
    TESTCHECKF( mat3(0,0), -sqrt1_2, &ok );
    TESTCHECKF( mat3(0,1), sqrt1_2, &ok );
    TESTCHECKF( mat3(1,0), -sqrt1_2, &ok );
    TESTCHECKF( mat3(1,1), -sqrt1_2, &ok );
    TESTCHECKF( mat3.GetAngle().Radians(), (-3. * M_PI / 4.), &ok );
    cout << "mat3 * vecX" << endl;
    prodVec = mat3 * vecX;
    TESTCHECKF( prodVec.X(), -sqrt1_2, &ok );
    TESTCHECKF( prodVec.Y(), -sqrt1_2, &ok );
    cout << "mat2 * mat3" << endl;
    RotationMatrix2F mat4 = mat2 * mat3;
    TESTCHECKF( mat4(0,0), 0.f, &ok );
    TESTCHECKF( mat4(0,1), 1.f, &ok );
    TESTCHECKF( mat4(1,0), -1.f, &ok );
    TESTCHECKF( mat4(1,1), 0.f, &ok );
    TESTCHECKF( mat4.GetAngle().Radians(), (- M_PI / 2.), &ok );

    if ( ok )
        cout << "RotationMatrix2 PASSED." << endl << endl;
    else
        cout << "RotationMatrix2 FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta


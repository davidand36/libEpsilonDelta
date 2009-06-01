/*
  RotationMatrix3.cpp
  Copyright (C) 2007 David M. Anderson

  RotationMatrix3 template class: a 3x3 matrix representing a rotation about
  an axis.
*/


#include "RotationMatrix3.hpp"
#include "AxisAngle.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Vector3.hpp"
#include <sstream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestRotationMatrix3( )
{
    bool ok = true;
    cout << "Testing RotationMatrix3" << endl;

    cout << "RotationMatrix3F( ) [default constructor]" << endl;
    RotationMatrix3F mat0;
    double a = 2. * M_PI / 3.;
    float f = sqrt( 1.f/3.f );
    cout << "Set( AxisAngleF( Vector3F( " << f << ", " << f << ", " << f <<
            "), Angle( " << a << " ) ) )" << endl;
    mat0.Set( AxisAngleF( Vector3F( f, f, f ), Angle( a ) ) );
    TESTCHECKF( (mat0 * Vector3F::UnitX).X(), Vector3F::UnitY.X(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitX).Y(), Vector3F::UnitY.Y(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitX).Z(), Vector3F::UnitY.Z(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitY).X(), Vector3F::UnitZ.X(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitY).Y(), Vector3F::UnitZ.Y(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitY).Z(), Vector3F::UnitZ.Z(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitZ).X(), Vector3F::UnitX.X(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitZ).Y(), Vector3F::UnitX.Y(), &ok );
    TESTCHECKF( (mat0 * Vector3F::UnitZ).Z(), Vector3F::UnitX.Z(), &ok );
    AxisAngleF axisAngle = mat0.GetAxisAngle( );
    TESTCHECKF( axisAngle.Axis().X(), f, &ok );
    TESTCHECKF( axisAngle.Axis().Y(), f, &ok );
    TESTCHECKF( axisAngle.Axis().Z(), f, &ok );
    TESTCHECKF( axisAngle.GetAngle().Radians(), a, &ok );
    double a0 = 0.1;
    double a1 = -1.2;
    double a2 = 2.3;
    cout << "rot0 = RotationMatrix3F( 0, " << a0 << " )" << endl;
    RotationMatrix3F rot0( 0, a0 );
    cout << "rot1 = RotationMatrix3F( 1, " << a1 << " )" << endl;
    RotationMatrix3F rot1( 1, a1 );
    cout << "rot2 = RotationMatrix3F( 2, " << a2 << " )" << endl;
    RotationMatrix3F rot2( 2, a2 );
    cout << "mat1 = rot2 * rot0 * rot1" << endl;
    RotationMatrix3F mat1 = rot2 * rot0 * rot1;
    cout << "GetEulerAngles( EulerAngles::ZXY )" << endl;
    EulerAngles euler = mat1.GetEulerAngles( EulerAngles::ZXY );
    TESTCHECKF( euler[0].Radians(), a2, &ok );
    TESTCHECKF( euler[1].Radians(), a0, &ok );
    TESTCHECKF( euler[2].Radians(), a1, &ok );
    cout << "RotationMatrix3F mat2( EulerAngles( " << a2 << ", " << a0 << ", "
         << a1 << "), EulerAngles::ZXY )" << endl;
    RotationMatrix3F mat2( EulerAngles( a2, a0, a1 ), EulerAngles::ZXY );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::ZXY )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::ZXY );
    TESTCHECKF( euler[0].Radians(), a2, &ok );
    TESTCHECKF( euler[1].Radians(), a0, &ok );
    TESTCHECKF( euler[2].Radians(), a1, &ok );
    cout << "mat1 = rot1 * rot0 * rot2" << endl;
    mat1 = rot1 * rot0 * rot2;
    cout << "GetEulerAngles( EulerAngles::YXZ )" << endl;
    euler = mat1.GetEulerAngles( EulerAngles::YXZ );
    TESTCHECKF( euler[0].Radians(), a1, &ok );
    TESTCHECKF( euler[1].Radians(), a0, &ok );
    TESTCHECKF( euler[2].Radians(), a2, &ok );
    cout << "Set( EulerAngles( " << a1 << ", " << a0 << ", "
         << a2 << "), EulerAngles::YXZ )" << endl;
    mat2.Set( EulerAngles( a1, a0, a2 ), EulerAngles::YXZ );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::YXZ )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::YXZ );
    TESTCHECKF( euler[0].Radians(), a1, &ok );
    TESTCHECKF( euler[1].Radians(), a0, &ok );
    TESTCHECKF( euler[2].Radians(), a2, &ok );
    a0 = 0.1;
    a1 = -1.2;
    a2 = - M_PI / 2.;
    cout << "rot0.Set( 0, " << a0 << " )" << endl;
    rot0.Set( 0, a0 );
    cout << "rot1.Set( 1, " << a1 << " )" << endl;
    rot1.Set( 1, a1 );
    cout << "rot2.Set( 2, " << a2 << " )" << endl;
    rot2.Set( 2, a2 );
    cout << "mat1 = rot2 * rot0 * rot1" << endl;
    mat1 = rot1 * rot2 * rot0;
    cout << "GetEulerAngles( EulerAngles::YZX )" << endl;
    euler = mat1.GetEulerAngles( EulerAngles::YZX );
    TESTCHECKF( euler[0].Radians(), (a1 - a0), &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( EulerAngles( " << a1 << ", " << a2 << ", "
         << a0 << "), EulerAngles::YZX )" << endl;
    mat2.Set( EulerAngles( a1, a2, a0 ), EulerAngles::YZX );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::YZX )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::YZX );
    TESTCHECKF( euler[0].Radians(), (a1 - a0), &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( euler, EulerAngles::YZX )" << endl;
    mat2.Set( euler, EulerAngles::YZX );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::YZX )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::YZX );
    TESTCHECKF( euler[0].Radians(), (a1 - a0), &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "mat1 = rot0 * rot2 * rot1" << endl;
    mat1 = rot0 * rot2 * rot1;
    cout << "GetEulerAngles( EulerAngles::XZY )" << endl;
    euler = mat1.GetEulerAngles( EulerAngles::XZY );
    TESTCHECKF( euler[0].Radians(), (a0 + a1), &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( EulerAngles( " << a0 << ", " << a2 << ", "
         << a1 << "), EulerAngles::XZY )" << endl;
    mat2.Set( EulerAngles( a0, a2, a1 ), EulerAngles::XZY );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::XZY )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::XZY );
    TESTCHECKF( euler[0].Radians(), (a0 + a1), &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( euler, EulerAngles::XZY )" << endl;
    mat2.Set( euler, EulerAngles::XZY );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::XZY )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::XZY );
    TESTCHECKF( euler[0].Radians(), (a0 + a1), &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    a0 = 0.1;
    a1 = M_PI / 2;
    a2 = -2.3;
    cout << "rot0.Set( 0, " << a0 << " )" << endl;
    rot0.Set( 0, a0 );
    cout << "rot1.Set( 1, " << a1 << " )" << endl;
    rot1.Set( 1, a1 );
    cout << "rot2.Set( 2, " << a2 << " )" << endl;
    rot2.Set( 2, a2 );
    cout << "mat1 = rot0 * rot1 * rot2" << endl;
    mat1 = rot0 * rot1 * rot2;
    cout << "GetEulerAngles( EulerAngles::XYZ )" << endl;
    euler = mat1.GetEulerAngles( EulerAngles::XYZ );
    TESTCHECKF( euler[0].Radians(), (a0 + a2), &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( EulerAngles( " << a0 << ", " << a1 << ", "
         << a2 << "), EulerAngles::XYZ )" << endl;
    mat2.Set( EulerAngles( a0, a1, a2 ), EulerAngles::XYZ );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::XYZ )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::XYZ );
    TESTCHECKF( euler[0].Radians(), (a0 + a2), &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( euler, EulerAngles::XYZ )" << endl;
    mat2.Set( euler, EulerAngles::XYZ );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::XYZ )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::XYZ );
    TESTCHECKF( euler[0].Radians(), (a0 + a2), &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "mat1 = rot2 * rot1 * rot0" << endl;
    mat1 = rot2 * rot1 * rot0;
    cout << "GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = mat1.GetEulerAngles( EulerAngles::ZYX );
    TESTCHECKF( euler[0].Radians(), (a2 - a0), &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( EulerAngles( " << a2 << ", " << a1 << ", "
         << a0 << "), EulerAngles::ZYX )" << endl;
    mat2.Set( EulerAngles( a2, a1, a0 ), EulerAngles::ZYX );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::ZYX );
    TESTCHECKF( euler[0].Radians(), (a2 - a0), &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );
    cout << "Set( euler, EulerAngles::ZYX )" << endl;
    mat2.Set( euler, EulerAngles::ZYX );
    TESTCHECKF( mat2(0,0), mat1(0,0), &ok );
    TESTCHECKF( mat2(0,1), mat1(0,1), &ok );
    TESTCHECKF( mat2(0,2), mat1(0,2), &ok );
    TESTCHECKF( mat2(1,0), mat1(1,0), &ok );
    TESTCHECKF( mat2(1,1), mat1(1,1), &ok );
    TESTCHECKF( mat2(1,2), mat1(1,2), &ok );
    TESTCHECKF( mat2(2,0), mat1(2,0), &ok );
    TESTCHECKF( mat2(2,1), mat1(2,1), &ok );
    TESTCHECKF( mat2(2,2), mat1(2,2), &ok );
    cout << "GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = mat2.GetEulerAngles( EulerAngles::ZYX );
    TESTCHECKF( euler[0].Radians(), (a2 - a0), &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0., &ok );

    if ( ok )
        cout << "RotationMatrix3 PASSED." << endl << endl;
    else
        cout << "RotationMatrix3 FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

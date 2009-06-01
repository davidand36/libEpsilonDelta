/*
  Quaternion.cpp
  Copyright (C) 2007 David M. Anderson

  Quaternion template class: the four-dimensional equivalent of complex
  numbers.
*/


#include "Quaternion.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "AxisAngle.hpp"
#include "RotationMatrix3.hpp"
#include <sstream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestQuaternion( )
{
    bool ok = true;
    cout << "Testing Quaternion" << endl;

    cout << "Quaternion( ) [default constructor]" << endl;
    QuaternionF quat0;
    float w = 3.f;
    float x = 0.f;
    float y = 4.f;
    float z = 0.f;
    cout << "Set( " << w << ", " << x << ", " << y << ", " << z << " )" << endl;
    quat0.Set( w, x, y, z );
    TESTCHECK( quat0.W(), w, &ok );
    TESTCHECK( quat0.X(), x, &ok );
    TESTCHECK( quat0.Y(), y, &ok );
    TESTCHECK( quat0.Z(), z, &ok );
    TESTCHECK( quat0[0], w, &ok );
    TESTCHECK( quat0[1], x, &ok );
    TESTCHECK( quat0[2], y, &ok );
    TESTCHECK( quat0[3], z, &ok );
    TESTCHECK( quat0.At( 0 ), w, &ok );
    TESTCHECK( quat0.At( 1 ), x, &ok );
    TESTCHECK( quat0.At( 2 ), y, &ok );
    TESTCHECK( quat0.At( 3 ), z, &ok );
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        TESTCHECK( quat0.At(4), 0.f, &ok );
        cout << "At(4) should have thrown an exception." << endl;
        ok = false;
    }
    catch( AssertException & exceptn )
    {
        cout << "Assertion here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
#endif
    cout << "quat0.Array()" << endl;
    const float * pF = quat0.Array();
    TESTCHECK( pF[0], w, &ok );
    TESTCHECK( pF[1], x, &ok );
    TESTCHECK( pF[2], y, &ok );
    TESTCHECK( pF[3], z, &ok );
    TESTCHECK( quat0.Real(), w, &ok );
    TESTCHECK( quat0.Imaginary().X(), x, &ok );
    TESTCHECK( quat0.Imaginary().Y(), y, &ok );
    TESTCHECK( quat0.Imaginary().Z(), z, &ok );
    cout << "Conjugate( )" << endl;
    QuaternionF quat1 = quat0.Conjugate( );
    TESTCHECK( quat1.W(), w, &ok );
    TESTCHECK( quat1.X(), -x, &ok );
    TESTCHECK( quat1.Y(), -y, &ok );
    TESTCHECK( quat1.Z(), -z, &ok );
    TESTCHECK( quat0.Length(), 5.f, &ok );
    TESTCHECK( quat0.Norm(), 25.f, &ok );
    TESTCHECK( quat1.Length(), 5.f, &ok );
    TESTCHECK( quat1.Norm(), 25.f, &ok );
    TESTCHECK( (quat0 + quat1).W(), 6.f, &ok );
    TESTCHECK( (quat0 + quat1).X(), 0.f, &ok );
    TESTCHECK( (quat0 + quat1).Y(), 0.f, &ok );
    TESTCHECK( (quat0 + quat1).Z(), 0.f, &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << quat0;
    TESTCHECK( ost.str(), string( "( 3, 0, 4, 0 )" ), &ok );
    TESTCHECK( ToJSON( quat0 ),
               string( "[ +3.00000e+00, +0.00000e+00, +4.00000e+00,"
                       " +0.00000e+00 ]" ),
               &ok );
    FromJSON( "[ 3, 0, -4.0, 0. ]", &quat1 );
    TESTCHECK( quat1.W(), w, &ok );
    TESTCHECK( quat1.X(), -x, &ok );
    TESTCHECK( quat1.Y(), -y, &ok );
    TESTCHECK( quat1.Z(), -z, &ok );
    cout << "Normalize( )" << endl;
    quat1.Normalize( );
    TESTCHECKF( quat1.W(), 0.6f, &ok );
    TESTCHECKF( quat1.X(), 0.f, &ok );
    TESTCHECKF( quat1.Y(), -0.8f, &ok );
    TESTCHECKF( quat1.Z(), 0.f, &ok );
    TESTCHECKF( quat1.Norm(), 1.f, &ok );
    cout << "Inverse( )" << endl;
    quat1 = quat0.Inverse( );
    TESTCHECKF( quat1.W(), 0.12f, &ok );
    TESTCHECKF( quat1.X(), 0.f, &ok );
    TESTCHECKF( quat1.Y(), -0.16f, &ok );
    TESTCHECKF( quat1.Z(), 0.f, &ok );
    TESTCHECKF( (quat0 * quat1).W(), 1.f, &ok );
    TESTCHECKF( (quat0 * quat1).X(), 0.f, &ok );
    TESTCHECKF( (quat0 * quat1).Y(), 0.f, &ok );
    TESTCHECKF( (quat0 * quat1).Z(), 0.f, &ok );
    float coords[4] = { 0.5f, 0.5f, 0.5f, 0.5f };
    cout << "QuaternionF( coords ) [array constructor]" << endl;
    QuaternionF quat2( coords );
    TESTCHECKF( quat2.W(), 0.5f, &ok );
    TESTCHECKF( quat2.X(), 0.5f, &ok );
    TESTCHECKF( quat2.Y(), 0.5f, &ok );
    TESTCHECKF( quat2.Z(), 0.5f, &ok );
    TESTCHECKF( quat2.Norm(), 1.f, &ok );
    cout << "Normalize( )" << endl;
    quat2.Normalize( );
    TESTCHECKF( quat2.W(), 0.5f, &ok );
    TESTCHECKF( quat2.X(), 0.5f, &ok );
    TESTCHECKF( quat2.Y(), 0.5f, &ok );
    TESTCHECKF( quat2.Z(), 0.5f, &ok );
    TESTCHECKF( quat2.Norm(), 1.f, &ok );
    cout << "Conjugate( )" << endl;
    quat1 = quat2.Conjugate( );
    TESTCHECKF( quat1.W(), 0.5f, &ok );
    TESTCHECKF( quat1.X(), -0.5f, &ok );
    TESTCHECKF( quat1.Y(), -0.5f, &ok );
    TESTCHECKF( quat1.Z(), -0.5f, &ok );
    TESTCHECKF( quat1.Norm(), 1.f, &ok );
    TESTCHECKF( (quat1 + quat2).W(), 1.f, &ok );
    TESTCHECKF( (quat1 + quat2).X(), 0.f, &ok );
    TESTCHECKF( (quat1 + quat2).Y(), 0.f, &ok );
    TESTCHECKF( (quat1 + quat2).Z(), 0.f, &ok );
    TESTCHECKF( (quat1 - quat2).W(), 0.f, &ok );
    TESTCHECKF( (quat1 - quat2).X(), -1.f, &ok );
    TESTCHECKF( (quat1 - quat2).Y(), -1.f, &ok );
    TESTCHECKF( (quat1 - quat2).Z(), -1.f, &ok );
    TESTCHECKF( (quat1 * quat2).W(), 1.f, &ok );
    TESTCHECKF( (quat1 * quat2).X(), 0.f, &ok );
    TESTCHECKF( (quat1 * quat2).Y(), 0.f, &ok );
    TESTCHECKF( (quat1 * quat2).Z(), 0.f, &ok );
    cout << "Inverse( )" << endl;
    quat1 = quat2.Inverse( );
    TESTCHECKF( quat1.W(), 0.5f, &ok );
    TESTCHECKF( quat1.X(), -0.5f, &ok );
    TESTCHECKF( quat1.Y(), -0.5f, &ok );
    TESTCHECKF( quat1.Z(), -0.5f, &ok );
    TESTCHECKF( quat1.Norm(), 1.f, &ok );
    double a = M_PI / 2.;
    cout << "QuaternionF( AxisAngle( Vector3F::UnitZ, Angle( " << a
         << " ) ) ) [axis-angle constructor]" << endl;
    QuaternionF quat3( AxisAngleF( Vector3F::UnitZ, Angle( a ) ) );
    TESTCHECKF( quat3.W(), sqrt( 0.5f ), &ok );
    TESTCHECKF( quat3.X(), 0.f, &ok );
    TESTCHECKF( quat3.Y(), 0.f, &ok );
    TESTCHECKF( quat3.Z(), sqrt( 0.5f ), &ok );
    TESTCHECKF( quat3.Norm(), 1.f, &ok );
    cout << "GetAxisAngle( )" << endl;
    AxisAngleF axisAngle = quat3.GetAxisAngle( );
    TESTCHECKF( axisAngle.Axis().X(), 0.f, &ok );
    TESTCHECKF( axisAngle.Axis().Y(), 0.f, &ok );
    TESTCHECKF( axisAngle.Axis().Z(), 1.f, &ok );
    TESTCHECKF( axisAngle.GetAngle().Radians(), M_PI / 2., &ok );
    cout << "Set( 2, a )" << endl;
    quat1.Set( 2, a );
    TESTCHECKF( quat1.W(), sqrt( 0.5f ), &ok );
    TESTCHECKF( quat1.X(), 0.f, &ok );
    TESTCHECKF( quat1.Y(), 0.f, &ok );
    TESTCHECKF( quat1.Z(), sqrt( 0.5f ), &ok );
    TESTCHECKF( quat1.Norm(), 1.f, &ok );
    cout << "Log()" << endl;
    quat1 = quat3.Log( );
    TESTCHECKF( quat1.W(), 0.f, &ok );
    TESTCHECKF( quat1.X(), 0.f, &ok );
    TESTCHECKF( quat1.Y(), 0.f, &ok );
    TESTCHECKF( quat1.Z(), M_PI / 4.f, &ok );
    cout << "SetW( 1. )" << endl;
    quat1.SetW( 1.f );
    cout << "Exp()" << endl;
    quat1 = quat1.Exp( );
    TESTCHECKF( quat1.W(), exp( 1.f ) * sqrt( 0.5f ), &ok );
    TESTCHECKF( quat1.X(), 0.f, &ok );
    TESTCHECKF( quat1.Y(), 0.f, &ok );
    TESTCHECKF( quat1.Z(), exp( 1.f ) * sqrt( 0.5f ), &ok );
    TESTCHECK( (quat1 == quat3), false, &ok );
    cout << "Log()" << endl;
    quat1 = quat1.Log( );
    TESTCHECKF( quat1.W(), 1.f, &ok );
    TESTCHECKF( quat1.X(), 0.f, &ok );
    TESTCHECKF( quat1.Y(), 0.f, &ok );
    TESTCHECKF( quat1.Z(), M_PI / 4.f, &ok );
    a = 2. * M_PI / 3.;
    float f = static_cast<float>( sqrt( 1./3. ) );
    cout << "RotationMatrix3F( AxisAngleF( Vector3F( " << f << ", " << f
         << ", " << f << "), Angle( " << a << " ) ) )" << endl;
    axisAngle.Set( Vector3F( f, f, f ), Angle( a ) );
    RotationMatrix3F mat0( axisAngle );
    cout << "QuaternionF( mat0 ) [rotation matrix constructor]" << endl;
    QuaternionF quat4( mat0 );
    TESTCHECKF( quat4.W(), quat2.W(), &ok );
    TESTCHECKF( quat4.X(), quat2.X(), &ok );
    TESTCHECKF( quat4.Y(), quat2.Y(), &ok );
    TESTCHECKF( quat4.Z(), quat2.Z(), &ok );
    TESTCHECKF( quat4.Norm(), 1.f, &ok );
    cout << "GetAxisAngle( )" << endl;
    axisAngle = quat4.GetAxisAngle( );
    TESTCHECKF( axisAngle.Axis().X(), f, &ok );
    TESTCHECKF( axisAngle.Axis().Y(), f, &ok );
    TESTCHECKF( axisAngle.Axis().Z(), f, &ok );
    TESTCHECKF( axisAngle.GetAngle().Radians(), a, &ok );
    cout << "Matrix( )" << endl;
    RotationMatrix3F mat1 = quat4.Matrix( );
    TESTCHECKF( mat1(0,0), mat0(0,0), &ok );
    TESTCHECKF( mat1(0,1), mat0(0,1), &ok );
    TESTCHECKF( mat1(0,2), mat0(0,2), &ok );
    TESTCHECKF( mat1(1,0), mat0(1,0), &ok );
    TESTCHECKF( mat1(1,1), mat0(1,1), &ok );
    TESTCHECKF( mat1(1,2), mat0(1,2), &ok );
    TESTCHECKF( mat1(2,0), mat0(2,0), &ok );
    TESTCHECKF( mat1(2,1), mat0(2,1), &ok );
    TESTCHECKF( mat1(2,2), mat0(2,2), &ok );
    cout << "(quat4 * Quaternion( Vector3F::UnitX )"
         << "* quat4.Conjugate()).Imaginary() [rotation]" << endl;
    Vector3F vec1 = (quat4 * QuaternionF( Vector3F::UnitX )
                     * quat4.Conjugate()).Imaginary();
    TESTCHECKF( vec1.X(), 0.f, &ok );
    TESTCHECKF( vec1.Y(), 1.f, &ok );
    TESTCHECKF( vec1.Z(), 0.f, &ok );
    cout << "Rotate( Vector3F::UnitX )" << endl;
    vec1 = quat4.Rotate( Vector3F::UnitX );
    TESTCHECKF( vec1.X(), 0.f, &ok );
    TESTCHECKF( vec1.Y(), 1.f, &ok );
    TESTCHECKF( vec1.Z(), 0.f, &ok );
    float a0 = -2.9f;
    float a1 = 1.8f;
    float a2 = -0.7f;
    cout << "Quaternion qrot0( 0, " << a0 << " )" << endl;
    QuaternionF qrot0( 0, Angle( a0 ) );
    cout << "Quaternion qrot1( 1, " << a1 << " )" << endl;
    QuaternionF qrot1( 1, Angle( a1 ) );
    cout << "Quaternion qrot2( 2, " << a2 << " )" << endl;
    QuaternionF qrot2( 2, Angle( a2 ) );
    cout << "quat1 = qrot1 * qrot2 * qrot0" << endl;
    quat1 = qrot1 * qrot2 * qrot0;
    cout << "GetEulerAngles( EulerAngles::YZX )" << endl;
    EulerAngles euler = quat1.GetEulerAngles( EulerAngles::YZX );
    TESTCHECKF( euler[0].Radians(), a1, &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), a0, &ok );
    cout << "Quaternion( EulerAngles( " << a1 << ", " << a2 << ", " << a0
         << " ), EulerAngles::YZX ) [Euler angles constructor]" << endl;
    QuaternionF quat5( EulerAngles( a1, a2, a0 ), EulerAngles::YZX );
    TESTCHECKF( quat5.W(), quat1.W(), &ok );
    TESTCHECKF( quat5.X(), quat1.X(), &ok );
    TESTCHECKF( quat5.Y(), quat1.Y(), &ok );
    TESTCHECKF( quat5.Z(), quat1.Z(), &ok );
    cout << "GetEulerAngles( EulerAngles::YZX )" << endl;
    euler = quat5.GetEulerAngles( EulerAngles::YZX );
    TESTCHECKF( euler[0].Radians(), a1, &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), a0, &ok );
    a2 = static_cast<float>( - M_PI / 2. );
    cout << "qrot2.Set( 2, " << a2 << " )" << endl;
    qrot2.Set( 2, Angle( a2 ) );
    cout << "quat1 = qrot1 * qrot2 * qrot0" << endl;
    quat1 = qrot1 * qrot2 * qrot0;
    cout << "GetEulerAngles( EulerAngles::YZX )" << endl;
    euler = quat1.GetEulerAngles( EulerAngles::YZX );
    Angle a10( a1 - a0 );
    a10.Normalize();
    float a1_a0 = static_cast<float>( a10.Radians() );
    TESTCHECKF( euler[0].Radians(), a1_a0, &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0.f, &ok );
    cout << "Matrix().GetEulerAngles( EulerAngles::YZX )" << endl;
    euler = quat1.Matrix().GetEulerAngles( EulerAngles::YZX );
    TESTCHECKF( euler[0].Radians(), a1_a0, &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0.f, &ok );
    cout << "Set( EulerAngles( " << a1 << ", " << a2 << ", " << a0
         << " ), EulerAngles::YZX )" << endl;
    quat5.Set( EulerAngles( a1, a2, a0 ), EulerAngles::YZX );
    TESTCHECKF( quat5.W(), quat1.W(), &ok );
    TESTCHECKF( quat5.X(), quat1.X(), &ok );
    TESTCHECKF( quat5.Y(), quat1.Y(), &ok );
    TESTCHECKF( quat5.Z(), quat1.Z(), &ok );
    cout << "GetEulerAngles( EulerAngles::YZX )" << endl;
    euler = quat5.GetEulerAngles( EulerAngles::YZX );
    TESTCHECKF( euler[0].Radians(), a1_a0, &ok );
    TESTCHECKF( euler[1].Radians(), a2, &ok );
    TESTCHECKF( euler[2].Radians(), 0.f, &ok );
    a0 = 1.1f;
    a1 = -1.0f;
    a2 = 0.5f;
    cout << "qrot0.Set( 0, " << a0 << " )" << endl;
    qrot0.Set( 0, Angle( a0 ) );
    cout << "qrot1.Set( 1, " << a1 << " )" << endl;
    qrot1.Set( 1, Angle( a1 ) );
    cout << "qrot2.Set( 2, " << a2 << " )" << endl;
    qrot2.Set( 2, Angle( a2 ) );
    cout << "quat1 = qrot2 * qrot1 * qrot0" << endl;
    quat1 = qrot2 * qrot1 * qrot0;
    cout << "GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = quat1.GetEulerAngles( EulerAngles::ZYX );
    TESTCHECKF( euler[0].Radians(), a2, &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), a0, &ok );
    cout << "Matrix().GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = quat1.Matrix().GetEulerAngles( EulerAngles::ZYX );
    TESTCHECKF( euler[0].Radians(), a2, &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), a0, &ok );
    cout << "Set( EulerAngles( " << a2 << ", " << a1 << ", " << a0
         << " ), EulerAngles::ZYX )" << endl;
    quat5.Set( EulerAngles( a2, a1, a0 ), EulerAngles::ZYX );
    TESTCHECKF( quat5.W(), quat1.W(), &ok );
    TESTCHECKF( quat5.X(), quat1.X(), &ok );
    TESTCHECKF( quat5.Y(), quat1.Y(), &ok );
    TESTCHECKF( quat5.Z(), quat1.Z(), &ok );
    cout << "GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = quat5.GetEulerAngles( EulerAngles::ZYX );
    TESTCHECKF( euler[0].Radians(), a2, &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), a0, &ok );
    a1 = static_cast<float>( M_PI / 2. );
    cout << "qrot1.Set( 1, " << a1 << " )" << endl;
    qrot1.Set( 1, Angle( a1 ) );
    cout << "quat1 = qrot2 * qrot1 * qrot0" << endl;
    quat1 = qrot2 * qrot1 * qrot0;
    cout << "GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = quat1.GetEulerAngles( EulerAngles::ZYX );
    Angle a20 = Angle( a2 - a0 );
    a20.Normalize();
    float a2_a0 = static_cast<float>( a20.Radians() );
    TESTCHECKF( euler[0].Radians(), a2_a0, &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0.f, &ok );
    cout << "Set( EulerAngles( " << a2 << ", " << a1 << ", " << a0
         << " ), EulerAngles::ZYX )" << endl;
    quat5.Set( EulerAngles( a2, a1, a0 ), EulerAngles::ZYX );
    TESTCHECKF( quat5.W(), quat1.W(), &ok );
    TESTCHECKF( quat5.X(), quat1.X(), &ok );
    TESTCHECKF( quat5.Y(), quat1.Y(), &ok );
    TESTCHECKF( quat5.Z(), quat1.Z(), &ok );
    cout << "GetEulerAngles( EulerAngles::ZYX )" << endl;
    euler = quat5.GetEulerAngles( EulerAngles::ZYX );
    TESTCHECKF( euler[0].Radians(), a2_a0, &ok );
    TESTCHECKF( euler[1].Radians(), a1, &ok );
    TESTCHECKF( euler[2].Radians(), 0.f, &ok );
    
    if ( ok )
        cout << "Quaternion PASSED." << endl << endl;
    else
        cout << "Quaternion FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

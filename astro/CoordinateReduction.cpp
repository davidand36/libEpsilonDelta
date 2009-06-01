/*
  CoordinateReduction.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for correcting the position of a celestial body for light-time,
  light deflection, aberration, precession, and nutation.
*/


#include "CoordinateReduction.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "JPLEphemeris.hpp"
#include "Precession.hpp"
#include "Nutation.hpp"
#include "Obliquity.hpp"
#include "Equatorial.hpp"
#include "AngleHMS.hpp"
#include "AngleDMS.hpp"
#include "AstroConst.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Vector3D GetAstrometricPlace( double julianDay,
                              const Vector3D & starBarycentric,
                              const Vector3D & starVelocity,
                              const Vector3D & earthBarycentric,
                              double epoch )
{
    Vector3D starGeocentric = starBarycentric - earthBarycentric;
    starGeocentric += (julianDay - epoch) * starVelocity;
    return starGeocentric;
}

//-----------------------------------------------------------------------------

Vector3D GetVirtualPlace( double julianDay,
                          const Vector3D & starBarycentric,
                          const Vector3D & starVelocity,
                          const Vector3D & earthBarycentric,
                          const Vector3D & earthHeliocentric, 
                          const Vector3D & earthBarycentricVelocity,
                          double epoch )
{
    Vector3D starGeocentric = GetAstrometricPlace( julianDay,
                                                   starBarycentric,
                                                   starVelocity,
                                                   earthBarycentric,
                                                   epoch );
    starGeocentric = CorrectForLightDeflection( starGeocentric,
                                                earthHeliocentric );
    starGeocentric = CorrectForAberration( starGeocentric,
                                           earthBarycentricVelocity );
    return starGeocentric;
}

//-----------------------------------------------------------------------------

Vector3D GetApparentPlace( double julianDay,
                           const Vector3D & starBarycentric,
                           const Vector3D & starVelocity,
                           const Vector3D & earthBarycentric,
                           const Vector3D & earthHeliocentric, 
                           const Vector3D & earthBarycentricVelocity,
                           const Matrix3D & nutAndPrecMatrix, 
                           double epoch )
{
    Vector3D starGeocentric = GetVirtualPlace( julianDay,
                                               starBarycentric,
                                               starVelocity,
                                               earthBarycentric,
                                               earthHeliocentric, 
                                               earthBarycentricVelocity,
                                               epoch );
    starGeocentric = CorrectForNutationAndPrecession( starGeocentric,
                                                      nutAndPrecMatrix );
    return starGeocentric;
}

//=============================================================================

Vector3D 
CorrectForLightDeflection( const Vector3D & bodyGeocentric,
                                    const Vector3D & bodyHeliocentric,
                                    const Vector3D & earthHeliocentric )
{
    const double bgLen = bodyGeocentric.Length();
    if ( bgLen == 0.)
        return bodyGeocentric;
    const Vector3D bgu = bodyGeocentric * (1. / bgLen);
    const double bhLen = bodyHeliocentric.Length();
    if ( bhLen == 0.)
        return bodyGeocentric;
    const Vector3D bhu = bodyHeliocentric * (1. / bhLen);
    const double ehLen = earthHeliocentric.Length();
    const Vector3D ehu = earthHeliocentric * (1. / ehLen);
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    const double g1
            = 2. * AstroConst::HeliocentricGravitationalConstant()
            / (c * c * ehLen);
    const double g2 = 1.  +  bhu * ehu;
    if ( g2 == 0. )
        return bodyGeocentric;
    return  bgLen
           * (bgu  +  (g1 / g2) * ( (bgu * bhu) * ehu  -  (bgu * ehu) * bhu ));
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Vector3D 
CorrectForLightDeflection( const Vector3D & starGeocentric,
                           const Vector3D & earthHeliocentric )
{
    const double sgLen = starGeocentric.Length();
    Assert( sgLen > 0. );
    if ( sgLen == 0. )
        return starGeocentric;
    const Vector3D sgu = starGeocentric * (1. / sgLen);
    const double ehLen = earthHeliocentric.Length();
    Assert( ehLen > 0. );
    const Vector3D ehu = earthHeliocentric * (1. / ehLen);
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    const double g1
            = 2. * AstroConst::HeliocentricGravitationalConstant()
            / (c * c * ehLen);
    double den = 1.  +  sgu * ehu;
    if ( den == 0. )
        return starGeocentric;
    return  sgLen
            * (sgu  +  g1 * (ehu  -  (sgu * ehu) * sgu) * (1. / den));
}

//-----------------------------------------------------------------------------

Vector3D
CorrectForAberration( const Vector3D & bodyGeocentric,
                      const Vector3D & earthBarycentricVelocity )
{
    const double bgLen = bodyGeocentric.Length();
    if ( bgLen == 0. )
        return bodyGeocentric;
    const Vector3D bgu = bodyGeocentric * (1. / bgLen);
    static const double cInv = 1.
            / (AstroConst::SpeedOfLight() * (86400. / 1000.));  //day / km
    const Vector3D ebv = earthBarycentricVelocity * cInv;
    const double ebvLen = ebv.Length();
    Assert( ebvLen <= 1. );
    const double betaInv = sqrt( 1.  -  ebvLen * ebvLen );
    const double f1 = bgu * ebv;
    const double f2 = 1.  +  f1 / (1. + betaInv);
    return  (betaInv * bodyGeocentric  +  f2 * bodyGeocentric.Length() * ebv)
            * (1. / (1. + f1));
}

//-----------------------------------------------------------------------------

Vector3D
CorrectForPrecession( const Vector3D & bodyGeocentric,
                      const Matrix3D & precessionMatrix )
{
    return precessionMatrix * bodyGeocentric;
}

//-----------------------------------------------------------------------------

Vector3D
CorrectForNutation( const Vector3D & bodyGeocentric,
                    const Matrix3D & nutationMatrix )
{
    return nutationMatrix * bodyGeocentric;
}

//-----------------------------------------------------------------------------

Vector3D
CorrectForNutationAndPrecession( const Vector3D & bodyGeocentric,
                                 const Matrix3D & nutAndPrecMatrix )
{
    return nutAndPrecMatrix * bodyGeocentric;
}


//=============================================================================

#ifdef DEBUG

bool 
TestCoordinateReduction( JPLEphemeris & de200, JPLEphemeris & de405 )
{
    bool ok = true;
    cout << "Testing Coordinate Reduction" << endl;

    const double astroUnit = AstroConst::AstronomicalUnit() / 1000.; //km
//    const double astroUnit = de200.AUinKM();
    Vector3D earthBarycentric;
    Vector3D earthHeliocentric;
    Vector3D earthBarycentricVelocity;
    Vector3D sunBarycentric;
    Matrix3D precessionMatrix;
    Nutation nutation;
    Matrix3D nutationMatrix;
    Matrix3D nutAndPrecMatrix;
    Vector3D bodyPos;
    Equatorial bodyEquatorial;

    //Paul J. Heafner, "Fundamental Ephemeris Computations", p. 183
    //Test case uses DE200.
    //Distance results don't seem sufficiently accurate.
    JPLBarycentricEphemeris sunEphem200( &de200, JPLEphemeris::Sun );
    JPLBarycentricEphemeris mercuryEphem200( &de200, JPLEphemeris::Mercury );
    JPLBarycentricEphemeris venusEphem200( &de200, JPLEphemeris::Venus );
    JPLBarycentricEphemeris marsEphem200( &de200, JPLEphemeris::Mars );
    JPLBarycentricEphemeris jupiterEphem200( &de200, JPLEphemeris::Jupiter );
    JPLBarycentricEphemeris saturnEphem200( &de200, JPLEphemeris::Saturn );
    JPLBarycentricEphemeris uranusEphem200( &de200, JPLEphemeris::Uranus );
    JPLBarycentricEphemeris neptuneEphem200( &de200, JPLEphemeris::Neptune );
    JPLBarycentricEphemeris plutoEphem200( &de200, JPLEphemeris::Pluto );
    JPLGeocentricEphemeris moonEphem200( &de200, JPLEphemeris::Moon );
    double jd = 2450100.5;
    cout << "JD: " << jd << endl;
    bool ephRslt = de200.GetBodyPosition( jd, JPLEphemeris::Earth,
                                          JPLEphemeris::SolarSystemBarycenter,
                                          &earthBarycentric, 
                                          &earthBarycentricVelocity );
    Assert( ephRslt );
    sunBarycentric = sunEphem200( jd );
    earthHeliocentric = earthBarycentric - sunBarycentric;
    precessionMatrix = Precession( jd ).Matrix( );
    ephRslt = de200.GetNutation( jd, &nutation );
    Assert( ephRslt );
    nutationMatrix = nutation.Matrix( MeanObliquity( jd ) );
    nutAndPrecMatrix = nutationMatrix * precessionMatrix;
    cout << "Mercury" << endl;
    bodyPos = GetApparentPlace( jd, mercuryEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 20, 2, 59.385 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -17, 22, 41.52 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 0.670418656,
                 &ok, 4.e-5 );
    cout << "Venus" << endl;
    bodyPos = GetApparentPlace( jd, venusEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 22, 23, 38.967 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -11, 40, 43.76 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 1.223997969,
                 &ok, 1.e-4 );
    cout << "Mars" << endl;
    bodyPos = GetApparentPlace( jd, marsEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 20, 40, 46.581 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -19, 27, 5.43 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 2.345744893,
                 &ok, 2.e-5 );
    cout << "Jupiter" << endl;
    bodyPos = GetApparentPlace( jd, jupiterEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 18, 14, 14.579 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -23, 10, 26.57 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 6.148002109,
                &ok, 2.e-6 );
    cout << "Saturn" << endl;
    bodyPos = GetApparentPlace( jd, saturnEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 23, 29, 14.021 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -5, 34, 36.59 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 10.123680407,
                 &ok, 5.e-6 );
    cout << "Uranus" << endl;
    bodyPos = GetApparentPlace( jd, uranusEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 20, 10, 36.177 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -20, 35, 55.61 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 20.730811657, &ok );
    cout << "Neptune" << endl;
    bodyPos = GetApparentPlace( jd, neptuneEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 19, 48, 43.892 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -20, 35, 37.47 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 31.148178810, &ok );
    cout << "Pluto" << endl;
    bodyPos = GetApparentPlace( jd, plutoEphem200, sunEphem200,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 16, 11, 43.785 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -7, 57, 11.27 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 30.422218805,
                 &ok, 2.e-6 );
    cout << "Moon" << endl;
    bodyPos = GetMoonApparentPlace( jd, moonEphem200, nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 17, 22, 38.167 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -18, 48, 56.72 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 0.002412280, &ok );
    cout << "Sun" << endl;
    bodyPos = GetSunApparentPlace( jd, sunEphem200, earthBarycentric,
                                   earthBarycentricVelocity,
                                   nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 19, 56, 51.445 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -20, 43, 29.61 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 0.983781396, &ok );

    //"The Astronomical Almanac for the Year 2003", p. B37.
    //Uses DE405.
    //The agreement, as indicated by the TESTCHECKFE tolerances, is very good.
    JPLBarycentricEphemeris sunEphem405( &de405, JPLEphemeris::Sun );
    JPLBarycentricEphemeris venusEphem405( &de405, JPLEphemeris::Venus );
    jd = 2452707.5;
    cout << "JD: " << jd << endl;
    ephRslt = de405.GetBodyPosition( jd, JPLEphemeris::Earth,
                                     JPLEphemeris::SolarSystemBarycenter,
                                     &earthBarycentric, 
                                     &earthBarycentricVelocity );
    Assert( ephRslt );
    sunBarycentric = sunEphem405( jd );
    earthHeliocentric = earthBarycentric - sunBarycentric;
    precessionMatrix = Precession( jd ).Matrix( );
    ephRslt = de405.GetNutation( jd, &nutation );
    Assert( ephRslt );
    nutationMatrix = nutation.Matrix( MeanObliquity( jd ) );
    nutAndPrecMatrix = nutationMatrix * precessionMatrix;
    cout << "Venus" << endl;
    bodyPos = GetApparentPlace( jd, venusEphem405, sunEphem405,
                                earthBarycentric, earthHeliocentric,
                                earthBarycentricVelocity,
                                nutAndPrecMatrix );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKFE( bodyEquatorial.RightAscension().Radians(),
                 Angle( AngleHMS( 20, 39, 43.14 ) ).Radians(), &ok, 7.e-8 );
    TESTCHECKFE( bodyEquatorial.Declination().Radians(),
                 Angle( AngleDMS( -17, 55, 32.7 ) ).Radians(), &ok, 6.e-7 );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 1.092098435, 
                 &ok, 5.e-9 );


    if ( ok )
        cout << "Coordinate Reduction PASSED." << endl << endl;
    else
        cout << "Coordinate Reduction FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

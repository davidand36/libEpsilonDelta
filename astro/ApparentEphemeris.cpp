/*
  ApparentEphemeris.cpp
  Copyright (C) 2007 David M. Anderson

  Function objects that return the apparent place of the Sun, Moon, and
  planets.
*/


#include "ApparentEphemeris.hpp"
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
using namespace std::tr1;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestApparentEphemeris( std::tr1::shared_ptr< JPLEphemeris > de200,
                       std::tr1::shared_ptr< JPLEphemeris > de405 )
{
    bool ok = true;
    cout << "Testing ApparentEphemeris" << endl;

    const double astroUnit = AstroConst::AstronomicalUnit() / 1000.; //km
    Matrix3D precessionMatrix;
    Nutation nutation;
    Matrix3D nutationMatrix;
    Matrix3D nutAndPrecMatrix;
    Point3D bodyPos;
    Equatorial bodyEquatorial;

    //Paul J. Heafner, "Fundamental Ephemeris Computations", p. 183
    //Test case uses DE200.
    //Distance results don't seem sufficiently accurate.
    JPLBarycentricEphemeris sunEphem200( de200, JPLEphemeris::Sun );
    JPLBarycentricEphemeris mercuryEphem200( de200, JPLEphemeris::Mercury );
    JPLBarycentricEphemeris venusEphem200( de200, JPLEphemeris::Venus );
    JPLBarycentricEphemeris earthEphem200( de200, JPLEphemeris::Earth );
    JPLBarycentricEphemeris marsEphem200( de200, JPLEphemeris::Mars );
    JPLBarycentricEphemeris jupiterEphem200( de200, JPLEphemeris::Jupiter );
    JPLBarycentricEphemeris saturnEphem200( de200, JPLEphemeris::Saturn );
    JPLBarycentricEphemeris uranusEphem200( de200, JPLEphemeris::Uranus );
    JPLBarycentricEphemeris neptuneEphem200( de200, JPLEphemeris::Neptune );
    JPLBarycentricEphemeris plutoEphem200( de200, JPLEphemeris::Pluto );
    JPLGeocentricEphemeris moonEphem200( de200, JPLEphemeris::Moon );
    double jd = 2450100.5;
    cout << "JD: " << jd << endl;
    precessionMatrix = Precession( jd ).Matrix( );
    bool ephRslt = de200->GetNutation( jd, &nutation );
    Assert( ephRslt );
    nutationMatrix = nutation.Matrix( MeanObliquity( jd ) );
    nutAndPrecMatrix = nutationMatrix * precessionMatrix;

    cout << "Mercury" << endl;
    ApparentEphemerisJPL mercuryApparentEphem200( mercuryEphem200,
                                                  sunEphem200, earthEphem200,
                                                  nutAndPrecMatrix );
    bodyPos = mercuryApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 20, 2, 59.385 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -17, 22, 41.52 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 0.670418656,
                 &ok, 4.e-5 );
    cout << "Venus" << endl;
    ApparentEphemerisJPL venusApparentEphem200( venusEphem200,
                                                sunEphem200, earthEphem200,
                                                nutAndPrecMatrix );
    bodyPos = venusApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 22, 23, 38.967 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -11, 40, 43.76 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 1.223997969,
                 &ok, 1.e-4 );
    cout << "Mars" << endl;
    ApparentEphemerisJPL marsApparentEphem200( marsEphem200,
                                               sunEphem200, earthEphem200,
                                               nutAndPrecMatrix );
    bodyPos = marsApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 20, 40, 46.581 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -19, 27, 5.43 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 2.345744893,
                 &ok, 2.e-5 );
    cout << "Jupiter" << endl;
    ApparentEphemerisJPL jupiterApparentEphem200( jupiterEphem200,
                                                  sunEphem200, earthEphem200,
                                                  nutAndPrecMatrix );
    bodyPos = jupiterApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 18, 14, 14.579 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -23, 10, 26.57 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 6.148002109,
                &ok, 2.e-6 );
    cout << "Saturn" << endl;
    ApparentEphemerisJPL saturnApparentEphem200( saturnEphem200,
                                                 sunEphem200, earthEphem200,
                                                 nutAndPrecMatrix );
    bodyPos = saturnApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 23, 29, 14.021 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -5, 34, 36.59 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 10.123680407,
                 &ok, 5.e-6 );
    cout << "Uranus" << endl;
    ApparentEphemerisJPL uranusApparentEphem200( uranusEphem200,
                                                 sunEphem200, earthEphem200,
                                                 nutAndPrecMatrix );
    bodyPos = uranusApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 20, 10, 36.177 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -20, 35, 55.61 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 20.730811657, &ok );
    cout << "Neptune" << endl;
    ApparentEphemerisJPL neptuneApparentEphem200( neptuneEphem200,
                                                  sunEphem200, earthEphem200,
                                                  nutAndPrecMatrix );
    bodyPos = neptuneApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 19, 48, 43.892 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -20, 35, 37.47 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 31.148178810, &ok );
    cout << "Pluto" << endl;
    ApparentEphemerisJPL plutoApparentEphem200( plutoEphem200,
                                                sunEphem200, earthEphem200,
                                                nutAndPrecMatrix );
    bodyPos = plutoApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 16, 11, 43.785 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -7, 57, 11.27 ) ).Radians(), &ok );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 30.422218805,
                 &ok, 2.e-6 );
    cout << "Moon" << endl;
    MoonApparentEphemerisJPL moonApparentEphem200( moonEphem200,
                                                   nutAndPrecMatrix );
    bodyPos = moonApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 17, 22, 38.167 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -18, 48, 56.72 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 0.002412280, &ok );
    cout << "Sun" << endl;
    SunApparentEphemerisJPL sunApparentEphem200( sunEphem200,
                                                 earthEphem200,
                                                 nutAndPrecMatrix );
    bodyPos = sunApparentEphem200( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKF( bodyEquatorial.RightAscension().Radians(),
                Angle( AngleHMS( 19, 56, 51.445 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Declination().Radians(),
                Angle( AngleDMS( -20, 43, 29.61 ) ).Radians(), &ok );
    TESTCHECKF( bodyEquatorial.Distance() / astroUnit, 0.983781396, &ok );

    //"The Astronomical Almanac for the Year 2003", p. B37.
    //Uses DE405.
    //The agreement, as indicated by the TESTCHECKFE tolerances, is very good.
    JPLBarycentricEphemeris sunEphem405( de405, JPLEphemeris::Sun );
    JPLBarycentricEphemeris venusEphem405( de405, JPLEphemeris::Venus );
    JPLBarycentricEphemeris earthEphem405( de405, JPLEphemeris::Earth );
    jd = 2452707.5;
    cout << "JD: " << jd << endl;
    precessionMatrix = Precession( jd ).Matrix( );
    ephRslt = de405->GetNutation( jd, &nutation );
    Assert( ephRslt );
    nutationMatrix = nutation.Matrix( MeanObliquity( jd ) );
    nutAndPrecMatrix = nutationMatrix * precessionMatrix;
    cout << "Venus" << endl;
    ApparentEphemerisJPL venusApparentEphem405( venusEphem405,
                                                sunEphem405, earthEphem405,
                                                nutAndPrecMatrix );
    bodyPos = venusApparentEphem405( jd );
    bodyEquatorial.Set( bodyPos );
    TESTCHECKFE( bodyEquatorial.RightAscension().Radians(),
                 Angle( AngleHMS( 20, 39, 43.14 ) ).Radians(), &ok, 7.e-8 );
    TESTCHECKFE( bodyEquatorial.Declination().Radians(),
                 Angle( AngleDMS( -17, 55, 32.7 ) ).Radians(), &ok, 6.e-7 );
    TESTCHECKFE( bodyEquatorial.Distance() / astroUnit, 1.092098435, 
                 &ok, 5.e-9 );


    if ( ok )
        cout << "ApparentEphemeris PASSED." << endl << endl;
    else
        cout << "ApparentEphemeris FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

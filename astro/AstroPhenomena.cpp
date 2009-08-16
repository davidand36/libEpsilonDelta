/*
  AstroPhenomena.cpp
  Copyright (C) 2007 David M. Anderson

  Routines fundamental to computing times and circumstances of astronomical
  phenomena such as seasons, lunar phases, conjunctions, and oppositions.
*/


#include "AstroPhenomena.hpp"
#include "JPLEphemeris.hpp"
#include "CoordinateReduction.hpp"
#include "AstroCoordTransformations.hpp"
#include "Precession.hpp"
#include "Nutation.hpp"
#include "Obliquity.hpp"
#include "Polynomial.hpp"
#include "Epoch.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool
GetEarthBarycentric( double julianDay,
                     Point3D * pEarthBarycentric,
                     Vector3D * pEarthBarycentricVelocity,
                     JPLEphemeris * pEphemeris )
{
    return pEphemeris->GetBodyPosition( julianDay, JPLEphemeris::Earth,
                                        JPLEphemeris::SolarSystemBarycenter,
                                        pEarthBarycentric, 
                                        pEarthBarycentricVelocity );
}

//=============================================================================

bool 
GetNutPrecAndObliquity( double julianDay,
                        Matrix3D * pNutAndPrecMatrix,
                        Angle * pTrueObliquity, 
                        JPLEphemeris * pEphemeris )
{
    Matrix3D precessionMatrix = Precession( julianDay ).Matrix( );
    Matrix3D nutationMatrix;
    Angle meanObliquity = MeanObliquity( julianDay );
    if ( pEphemeris->NutationAvailable() )
    {
        Nutation nutation;
        bool ephRslt = pEphemeris->GetNutation( julianDay, &nutation );
        if ( ! ephRslt )
            return false;
        nutationMatrix = nutation.Matrix( meanObliquity );
        if ( pTrueObliquity )
            *pTrueObliquity = TrueObliquity( meanObliquity, nutation );
    }
    else
    {
        //We might compute nutation, but we'll treat it as 0 here.
        nutationMatrix.Set( 1. );
        if ( pTrueObliquity )
            *pTrueObliquity = meanObliquity;
    }
    if ( pNutAndPrecMatrix )
        *pNutAndPrecMatrix = nutationMatrix * precessionMatrix;
    return true;
}

//=============================================================================

Equatorial 
SolarEquatorialPosition( double julianDay,
                         const Point3D & earthBarycentric,
                         const Vector3D & earthBarycentricVelocity,
                         const Matrix3D & nutAndPrecMatrix,
                         JPLEphemeris * pEphemeris )
{
    JPLBarycentricEphemeris sunEphem( pEphemeris, JPLEphemeris::Sun );
    Point3D sunPos = GetSunApparentPlace( julianDay, sunEphem,
                                          earthBarycentric,
                                          earthBarycentricVelocity,
                                          nutAndPrecMatrix );
    return  Equatorial( sunPos );
}

//-----------------------------------------------------------------------------

Equatorial 
SolarEquatorialPosition( double julianDay )
{
    JPLEphemeris * pEphemeris = JPLEphemeris::GetEphemeris( julianDay );
    Assert( pEphemeris );
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
#ifdef DEBUG
    bool earthRslt = 
#endif
            GetEarthBarycentric( julianDay, &earthBarycentric,
                                 &earthBarycentricVelocity, pEphemeris );
    Assert( earthRslt );
    Matrix3D nutAndPrecMatrix;
#ifdef DEBUG
    bool nutPrecRslt =
#endif
            GetNutPrecAndObliquity( julianDay, &nutAndPrecMatrix,
                                    0, pEphemeris );
    Assert( nutPrecRslt );
    return  SolarEquatorialPosition( julianDay, earthBarycentric,
                                     earthBarycentricVelocity,
                                     nutAndPrecMatrix, pEphemeris );
}

//=============================================================================

Equatorial 
LunarEquatorialPosition( double julianDay,
                         const Matrix3D & nutAndPrecMatrix,
                         JPLEphemeris * pEphemeris )
{
    JPLGeocentricEphemeris moonEphem( pEphemeris, JPLEphemeris::Moon );
    Point3D moonPos = GetMoonApparentPlace( julianDay, moonEphem,
                                            nutAndPrecMatrix );
    return  Equatorial( moonPos );
}

//-----------------------------------------------------------------------------

Equatorial 
LunarEquatorialPosition( double julianDay )
{
    JPLEphemeris * pEphemeris = JPLEphemeris::GetEphemeris( julianDay );
    Assert( pEphemeris );
    Matrix3D nutAndPrecMatrix;
#ifdef DEBUG
    bool nutPrecRslt =
#endif
            GetNutPrecAndObliquity( julianDay, &nutAndPrecMatrix,
                                    0, pEphemeris );
    Assert( nutPrecRslt );
    return  LunarEquatorialPosition( julianDay, nutAndPrecMatrix, pEphemeris );
}

//=============================================================================

Equatorial 
PlanetEquatorialPosition( double julianDay,
                          SolarSystem::EBody body,
                          const Point3D & earthBarycentric,
                          const Point3D & earthHeliocentric,
                          const Vector3D & earthBarycentricVelocity,
                          const Matrix3D & nutAndPrecMatrix,
                          JPLEphemeris * pEphemeris )
{
    Assert( body != SolarSystem::Earth );
    JPLEphemeris::EBody jplBody = JPLEphemeris::SolarSystemToJPLBody( body );
    JPLBarycentricEphemeris bodyEphem( pEphemeris, jplBody );
    JPLBarycentricEphemeris sunEphem( pEphemeris, JPLEphemeris::Sun );
    Point3D bodyPos = GetApparentPlace( julianDay, bodyEphem, sunEphem,
                                        earthBarycentric, earthHeliocentric,
                                        earthBarycentricVelocity,
                                        nutAndPrecMatrix );
    return  Equatorial( bodyPos );
}

//-----------------------------------------------------------------------------

Equatorial 
PlanetEquatorialPosition( double julianDay,
                          SolarSystem::EBody body )
{
    JPLEphemeris * pEphemeris = JPLEphemeris::GetEphemeris( julianDay );
    Assert( pEphemeris );
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
#ifdef DEBUG
    bool earthRslt =
#endif
            GetEarthBarycentric( julianDay, &earthBarycentric,
                                 &earthBarycentricVelocity, pEphemeris );
    Assert( earthRslt );
    Point3D sunBarycentric;
#ifdef DEBUG
    bool sunRslt =
#endif
            pEphemeris->GetBodyPosition( julianDay, JPLEphemeris::Sun,
                                         JPLEphemeris::SolarSystemBarycenter,
                                         &sunBarycentric );
    Assert( sunRslt );
    Point3D earthHeliocentric = Translate( earthBarycentric, sunBarycentric );
    Matrix3D nutAndPrecMatrix;
#ifdef DEBUG
    bool nutPrecRslt =
#endif
            GetNutPrecAndObliquity( julianDay, &nutAndPrecMatrix,
                                    0, pEphemeris );
    Assert( nutPrecRslt );
    return  PlanetEquatorialPosition( julianDay, body, earthBarycentric,
                                      earthHeliocentric,
                                      earthBarycentricVelocity,
                                      nutAndPrecMatrix,
                                      pEphemeris );
}

//=============================================================================

Angle
SolarLongitude( double julianDay,
                const Point3D & earthBarycentric,
                const Vector3D & earthBarycentricVelocity,
                const Matrix3D & nutAndPrecMatrix,
                Angle obliquity, 
                JPLEphemeris * pEphemeris )
{
    JPLBarycentricEphemeris sunEphem( pEphemeris, JPLEphemeris::Sun );
    Point3D sunPos = GetSunApparentPlace( julianDay, sunEphem,
                                          earthBarycentric,
                                          earthBarycentricVelocity,
                                          nutAndPrecMatrix );
    return EclipticalLongitude( sunPos, obliquity );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Angle
SolarLongitude( double julianDay )
{
    JPLEphemeris * pEphemeris = JPLEphemeris::GetEphemeris( julianDay );
    if ( pEphemeris == 0 )
        return Angle( 0. ); //!!!
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
    Matrix3D nutAndPrecMatrix;
    Angle obliquity;
#ifdef DEBUG
    bool earthRslt =
#endif
            GetEarthBarycentric( julianDay, &earthBarycentric,
                                 &earthBarycentricVelocity, pEphemeris );
    Assert( earthRslt );
#ifdef DEBUG
    bool oblRslt =
#endif
            GetNutPrecAndObliquity( julianDay, &nutAndPrecMatrix,
                                    &obliquity, pEphemeris );
    Assert( oblRslt );
    return SolarLongitude( julianDay,
                           earthBarycentric, earthBarycentricVelocity,
                           nutAndPrecMatrix, obliquity, pEphemeris );
}

//=============================================================================

Angle 
MeanSolarLongitude( double julianDay )
{
    //Meeus, "Astronomical Algorithms", (27.2)
    static const Polynomial< double > meanLongPoly( 5,
                                      280.4664567, 360007.6982779,
                                      0.03032028, (1./49931.), -(1./15299.),
                                      -(1./1988000.) );
    double mill2000 = Century2000( julianDay ) / 10.;
    double degrees = meanLongPoly( mill2000 );
    Angle meanSolarLong( degrees, Angle::Degree );
    meanSolarLong.Normalize( );
    return meanSolarLong;
}

//=============================================================================

Angle
LunarLongitude( double julianDay, 
                const Matrix3D & nutAndPrecMatrix,
                Angle obliquity, 
                JPLEphemeris * pEphemeris )
{
    JPLGeocentricEphemeris moonEphem( pEphemeris, JPLEphemeris::Moon );
    Point3D moonPos = GetMoonApparentPlace( julianDay, moonEphem,
                                            nutAndPrecMatrix );
    return EclipticalLongitude( moonPos, obliquity );
}

//-----------------------------------------------------------------------------

Angle 
LunarPhase( double julianDay )
{
    JPLEphemeris * pEphemeris = JPLEphemeris::GetEphemeris( julianDay );
    if ( pEphemeris == 0 )
        return Angle( 0. ); //!!!
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
    Matrix3D nutAndPrecMatrix;
    Angle obliquity;
#ifdef DEBUG
    bool earthRslt =
#endif
            GetEarthBarycentric( julianDay, &earthBarycentric,
                                 &earthBarycentricVelocity, pEphemeris );
    Assert( earthRslt );
#ifdef DEBUG
    bool oblRslt =
#endif
            GetNutPrecAndObliquity( julianDay, &nutAndPrecMatrix,
                                    &obliquity, pEphemeris );
    Assert( oblRslt );
    Angle solarLong = SolarLongitude( julianDay, earthBarycentric,
                                     earthBarycentricVelocity,
                                     nutAndPrecMatrix, obliquity, pEphemeris );
    Angle lunarLong = LunarLongitude( julianDay,
                                     nutAndPrecMatrix, obliquity, pEphemeris );
    Angle phase = lunarLong - solarLong;
    return phase;
}

//-----------------------------------------------------------------------------

Angle 
LunarArcOfLight( double julianDay )
{
    JPLEphemeris * pEphemeris = JPLEphemeris::GetEphemeris( julianDay );
    if ( pEphemeris == 0 )
        return Angle( 0. ); //!!!
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
    Matrix3D nutAndPrecMatrix;
#ifdef DEBUG
    bool earthRslt =
#endif
            GetEarthBarycentric( julianDay, &earthBarycentric,
                                 &earthBarycentricVelocity, pEphemeris );
    Assert( earthRslt );
#ifdef DEBUG
    bool oblRslt =
#endif
            GetNutPrecAndObliquity( julianDay, &nutAndPrecMatrix,
                                    0, pEphemeris );
    Assert( oblRslt );
    Equatorial solarPos = SolarEquatorialPosition( julianDay,
                                                   earthBarycentric,
                                                   earthBarycentricVelocity,
                                                   nutAndPrecMatrix,
                                                   pEphemeris );
    Equatorial lunarPos = LunarEquatorialPosition( julianDay,
                                                   nutAndPrecMatrix,
                                                   pEphemeris );
    Angle diffRA = lunarPos.RightAscension() - solarPos.RightAscension();
    Angle diffDec = lunarPos.Declination() - solarPos.Declination();
    return ArcCos( diffRA.Cos( ) * diffDec.Cos( ) );
}

//=============================================================================

#ifdef DEBUG

bool 
TestAstroPhenomena( )
{
    bool ok = true;
    cout << "Testing AstroPhenomena" << endl;

    //!!!

    if ( ok )
        cout << "AstroPhenomena PASSED." << endl << endl;
    else
        cout << "AstroPhenomena FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

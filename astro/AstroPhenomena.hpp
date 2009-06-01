#ifndef ASTROPHENOMENA_HPP
#define ASTROPHENOMENA_HPP
/*
  AstroPhenomena.hpp
  Copyright (C) 2007 David M. Anderson

  Routines fundamental to computing times and circumstances of astronomical
  phenomena such as seasons, lunar phases, conjunctions, and oppositions.
*/


#include "Angle.hpp"
#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "Equatorial.hpp"
#include "SolarSystem.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class JPLEphemeris;


//=============================================================================


bool GetEarthBarycentric( double julianDay,
                          Vector3D * pEarthBarycentric,
                          Vector3D * pEarthBarycentricVelocity,
                          JPLEphemeris * pEphemeris );
bool GetNutPrecAndObliquity( double julianDay,
                             Matrix3D * pNutAndPrecMatrix,
                             Angle * pTrueObliquity, 
                             JPLEphemeris * pEphemeris );

Equatorial SolarEquatorialPosition( double julianDay,
                                    const Vector3D & earthBarycentric,
                                    const Vector3D & earthBarycentricVelocity,
                                    const Matrix3D & nutAndPrecMatrix,
                                    JPLEphemeris * pEphemeris );
Equatorial SolarEquatorialPosition( double julianDay );
                                    
Equatorial LunarEquatorialPosition( double julianDay,
                                    const Matrix3D & nutAndPrecMatrix,
                                    JPLEphemeris * pEphemeris );
Equatorial LunarEquatorialPosition( double julianDay );

Equatorial PlanetEquatorialPosition( double julianDay,
                                     SolarSystem::EBody body,
                                     const Vector3D & earthBarycentric,
                                     const Vector3D & earthHeliocentric,
                                     const Vector3D & earthBarycentricVelocity,
                                     const Matrix3D & nutAndPrecMatrix,
                                     JPLEphemeris * pEphemeris );
Equatorial PlanetEquatorialPosition( double julianDay,
                                     SolarSystem::EBody body );
                                     
Angle SolarLongitude( double julianDay,
                      const Vector3D & earthBarycentric,
                      const Vector3D & earthBarycentricVelocity,
                      const Matrix3D & nutAndPrecMatrix,
                      Angle obliquity, 
                      JPLEphemeris * pEphemeris );
Angle SolarLongitude( double julianDay );
Angle MeanSolarLongitude( double julianDay );

Angle LunarLongitude( double julianDay, 
                      const Matrix3D & nutAndPrecMatrix,
                      Angle obliquity, 
                      JPLEphemeris * pEphemeris );
Angle LunarPhase( double julianDay );
Angle LunarArcOfLight( double julianDay );

#ifdef DEBUG
bool TestAstroPhenomena( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ASTROPHENOMENA_HPP

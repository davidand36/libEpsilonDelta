#ifndef ASTROPHENOMENA_HPP
#define ASTROPHENOMENA_HPP
/*
  AstroPhenomena.hpp
  Copyright (C) 2007 David M. Anderson

  Routines fundamental to computing times and circumstances of astronomical
  phenomena such as seasons, lunar phases, conjunctions, and oppositions.
*/


#include "Angle.hpp"
#include "Point3.hpp"
#include "Matrix3.hpp"
#include "Equatorial.hpp"
#include "SolarSystem.hpp"
#include "Logger.hpp"
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class JPLEphemeris;


//=============================================================================


bool GetEarthBarycentric( double julianDay,
                          Point3D * pEarthBarycentric,
                          Vector3D * pEarthBarycentricVelocity,
                          std::tr1::shared_ptr< JPLEphemeris > spEphemeris );
bool GetNutPrecAndObliquity( double julianDay,
                             Matrix3D * pNutAndPrecMatrix,
                             Angle * pTrueObliquity, 
                             std::tr1::shared_ptr< JPLEphemeris > spEphemeris );

Equatorial SolarEquatorialPosition( double julianDay,
                             const Point3D & earthBarycentric,
                             const Vector3D & earthBarycentricVelocity,
                             const Matrix3D & nutAndPrecMatrix,
                             std::tr1::shared_ptr< JPLEphemeris > spEphemeris );
Equatorial SolarEquatorialPosition( double julianDay );
                                    
Equatorial LunarEquatorialPosition( double julianDay,
                             const Matrix3D & nutAndPrecMatrix,
                             std::tr1::shared_ptr< JPLEphemeris > spEphemeris );
Equatorial LunarEquatorialPosition( double julianDay );

Equatorial PlanetEquatorialPosition( double julianDay,
                             SolarSystem::EBody body,
                             const Point3D & earthBarycentric,
                             const Point3D & earthHeliocentric,
                             const Vector3D & earthBarycentricVelocity,
                             const Matrix3D & nutAndPrecMatrix,
                             std::tr1::shared_ptr< JPLEphemeris > spEphemeris );
Equatorial PlanetEquatorialPosition( double julianDay,
                                     SolarSystem::EBody body );
                                     
Angle SolarLongitude( double julianDay,
                      const Point3D & earthBarycentric,
                      const Vector3D & earthBarycentricVelocity,
                      const Matrix3D & nutAndPrecMatrix,
                      Angle obliquity, 
                      std::tr1::shared_ptr< JPLEphemeris > spEphemeris );
Angle SolarLongitude( double julianDay );
Angle MeanSolarLongitude( double julianDay );

Angle LunarLongitude( double julianDay, 
                      const Matrix3D & nutAndPrecMatrix,
                      Angle obliquity, 
                      std::tr1::shared_ptr< JPLEphemeris > spEphemeris );
Angle LunarPhase( double julianDay );
Angle LunarArcOfLight( double julianDay );

Logger & AstroPhenomenaLog( );

#ifdef DEBUG
bool TestAstroPhenomena( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ASTROPHENOMENA_HPP

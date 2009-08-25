#ifndef ASTROCOORDTRANSFORMATIONS_HPP
#define ASTROCOORDTRANSFORMATIONS_HPP
/*
  AstroCoordTransformations.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for transforming positions in space from one coordinate system to
  another. Includes transformations between various spherical coordinate
  systems and generation of matrices for transformations in rectangular
  coordinates.
  NOTES:
  1. Transformations between ecliptic and equatorial require the obliquity of
     the ecliptic. (For J2000.0, this is 23.4392911 degrees; for B1950.0, it
     is 23.4457889 degrees. These are mean obliquities, not including
     nutation.) For apparent place, nutation and aberation must be accounted
     for.
  2. Transformations between equatorial and horizontal require the local
     sidereal time and the geographic latitude. Sidereal time may be passed
     either as an angle (cf. AngleHMS) or in units of days.
  3. Transformations between equatorial and galactic are strictly valid for
     the equatorial system of B1950.0. For other epochs, precession must be
     accounted for.
  4. Several important astronomical phenomena are defined in terms of
     ecliptical longitude, inluding seasons, oppositions, and phases of the
     Moon. The function EclipticalLongitude() is provided so that this value
     can be computed directly from rectangular equatorial coordinates.
*/


#include "Equatorial.hpp"
#include "Ecliptical.hpp"
#include "Horizontal.hpp"
#include "Galactic.hpp"
#include "Matrix3.hpp"
#include "Point3.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Equatorial EclipticalToEquatorial( const Ecliptical & ecliptical,
                                   Angle obliquity );
Ecliptical EquatorialToEcliptical( const Equatorial & equatorial,
                                   Angle obliquity );
Horizontal EquatorialToHorizontal( const Equatorial & equatorial,
                                   Angle localSiderealTime,
                                   Angle geographicLatitude );
Horizontal EquatorialToHorizontal( const Equatorial & equatorial,
                                   double localSiderealTime,
                                   Angle geographicLatitude );
Equatorial HorizontalToEquatorial( const Horizontal & horizontal,
                                   Angle localSiderealTime,
                                   Angle geographicLatitude );
Equatorial HorizontalToEquatorial( const Horizontal & horizontal,
                                   double localSiderealTime,
                                   Angle geographicLatitude );
Galactic EquatorialToGalactic( const Equatorial & equatorial );
Equatorial GalacticToEquatorial( const Galactic & galactic );

Matrix3D EclipticalToEquatorialMatrix( Angle obliquity );
Matrix3D EquatorialToEclipticalMatrix( Angle obliquity );
Matrix3D EquatorialToHorizontalMatrix( Angle localSiderealTime,
                                       Angle geographicLatitude );
Matrix3D EquatorialToHorizontalMatrix( double localSiderealTime,
                                       Angle geographicLatitude );
Matrix3D HorizontalToEquatorialMatrix( Angle localSiderealTime,
                                       Angle geographicLatitude );
Matrix3D HorizontalToEquatorialMatrix( double localSiderealTime,
                                       Angle geographicLatitude );
Matrix3D EquatorialToGalacticMatrix( );
Matrix3D GalacticToEquatorialMatrix( );

Angle EclipticalLongitude( const Point3D & equatorialRect, Angle obliquity );

#ifdef DEBUG
bool TestAstroCoordTransformations( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ASTROCOORDTRANSFORMATIONS_HPP

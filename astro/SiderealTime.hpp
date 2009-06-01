/*
  SiderealTime.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for computing sidereal time.
  NOTES:
  1. Local sidereal time = Greenwich sidereal time + longitude.
*/


#ifndef SIDEREALTIME_HPP
#define SIDEREALTIME_HPP


#include "Angle.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class Nutation;
class JPLEphemeris;

//*****************************************************************************


Angle GreenwichMeanSiderealTime( double julianDay );
Angle GreenwichApparentSiderealTime( double julianDay,
                                     Angle nutLongitude, Angle meanObliquity );
Angle GreenwichApparentSiderealTime( double julianDay,
                                     const Nutation & nutation,
                                     Angle meanObliquity );

#ifdef DEBUG
bool TestSiderealTime( JPLEphemeris & ephem );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SIDEREALTIME_HPP

#ifndef MOONPHASES_HPP
#define MOONPHASES_HPP
/*
  MoonPhases.hpp
  Copyright (C) 2007 David M. Anderson

  Routine to determine the Julian day of the desired lunar phase on or after
  the specified Julian day.
  NOTES: 
  1. Time is UTC after 2441317.5 (1 Jan 1972); UT1 (approximate), before.
*/


#include "Angle.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace MoonPhases
{                                                      /*namespace MoonPhases*/

//*****************************************************************************

enum EPhase
    { New, FirstQuarter, Full, LastQuarter };

double FindNext( double julianDay, Angle phaseAngle );
double FindNext( double julianDay, EPhase phase );


#ifdef DEBUG
bool Test( );
#endif

//*****************************************************************************

}                                                      /*namespace MoonPhases*/

}                                                      //namespace EpsilonDelta

#endif //MOONPHASES_HPP

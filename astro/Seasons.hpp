#ifndef SEASONS_HPP
#define SEASONS_HPP
/*
  Seasons.hpp
  Copyright (C) 2007 David M. Anderson

  Routine to determine the Julian day of the desired equinox or solstice on or
  after the specified Julian day.
  NOTES: 
  1. Time is UTC after 2441317.5 (1 Jan 1972); UT1 (approximate), before.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace Seasons
{                                                         /*namespace Seasons*/

//*****************************************************************************


enum ESeason
    { SpringEquinox, SummerSolstice, AutumnalEquinox, WinterSolstice };

double FindNext( double julianDay, ESeason season );


#ifdef DEBUG
bool Test( );
#endif


//*****************************************************************************

}                                                         /*namespace Seasons*/

}                                                      //namespace EpsilonDelta

#endif //SEASONS_HPP

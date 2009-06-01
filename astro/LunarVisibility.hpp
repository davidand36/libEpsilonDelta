#ifndef LUNARVISIBILITY_HPP
#define LUNARVISIBILITY_HPP
/*
  LunarVisibility.hpp
  Copyright (C) 2007 David M. Anderson

  Functions to determine whether the lunar crescent is visible.
*/


#include "RiseSet.hpp"
#include "GeodeticLocation.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace LunarVisibility
{                                                   //namespace LunarVisibility

//*****************************************************************************


enum ETime
{ Morning, Evening };

enum ECriterion
{ Shaukat/*, Yallop, SAAO*/ };

bool CheckNext( double julianDay, const GeodeticLocation & location,
                ETime timeOfDay );
bool CheckNext( double julianDay, const GeodeticLocation & location );
void SetCriterion( ECriterion criterion );
ECriterion Criterion( );

#ifdef DEBUG
bool Test( );
#endif


//*****************************************************************************

}                                                   //namespace LunarVisibility

}                                                      //namespace EpsilonDelta

#endif //LUNARVISIBILITY_HPP

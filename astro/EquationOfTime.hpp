#ifndef EQUATIONOFTIME_HPP
#define EQUATIONOFTIME_HPP
/*
  EquationOfTime.hpp
  Copyright (C) 2007 David M. Anderson

  Routine for computing the "equation of time", the difference between the
  apparent and mean solar time.
*/


#include "TimeIncrement.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


TimeIncrement EquationOfTime( double julianDay );


#ifdef DEBUG
bool TestEquationOfTime( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EQUATIONOFTIME_HPP

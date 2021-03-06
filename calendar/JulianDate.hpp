#ifndef JULIANDATE_HPP
#define JULIANDATE_HPP
/*
  JulianDate.hpp
  Copyright (C) 2007 David M. Anderson

  JulianDate class, representing a date in the Julian calendar.
*/


#include "DMYWDate.hpp"
#include "JulianCalendar.hpp"
#include "WesternWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DMYWDate< JulianCalendar, WesternWeek >  JulianDate;


//=============================================================================

#ifdef DEBUG
bool TestJulianDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JULIANDATE_HPP

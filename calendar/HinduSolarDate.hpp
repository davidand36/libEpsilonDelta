#ifndef HINDUSOLARDATE_HPP
#define HINDUSOLARDATE_HPP
/*
  HinduSolarDate.hpp
  Copyright (C) 2007 David M. Anderson

  HinduSolarDate class, representing a date in the Hindu solar calendar.
*/


#include "DateDMYW.hpp"
#include "HinduSolarCalendar.hpp"
#include "HinduWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< HinduSolarCalendar, HinduWeek >  HinduSolarDate;


//=============================================================================

#ifdef DEBUG
bool TestHinduSolarDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HINDUSOLARDATE_HPP

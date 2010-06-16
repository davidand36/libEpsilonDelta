#ifndef HINDUSOLARDATE_HPP
#define HINDUSOLARDATE_HPP
/*
  HinduSolarDate.hpp
  Copyright (C) 2007 David M. Anderson

  HinduSolarDate class, representing a date in the Hindu solar calendar.
*/


#include "DMYWDate.hpp"
#include "HinduSolarCalendar.hpp"
#include "HinduWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DMYWDate< HinduSolarCalendar, HinduWeek >  HinduSolarDate;

//=============================================================================

template<>
void DMYDate< HinduSolarCalendar >::Set( bool today );

//=============================================================================

#ifdef DEBUG
bool TestHinduSolarDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HINDUSOLARDATE_HPP

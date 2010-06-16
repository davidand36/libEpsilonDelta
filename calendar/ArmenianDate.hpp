#ifndef ARMENIANDATE_HPP
#define ARMENIANDATE_HPP
/*
  ArmenianDate.hpp
  Copyright (C) 2007 David M. Anderson

  ArmenianDate class, representing a date in the Armenian calendar.
*/


#include "DMYWDate.hpp"
#include "ArmenianCalendar.hpp"
#include "ArmenianWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DMYWDate< ArmenianCalendar, ArmenianWeek >  ArmenianDate;


//=============================================================================


#ifdef DEBUG
bool TestArmenianDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ARMENIANDATE_HPP

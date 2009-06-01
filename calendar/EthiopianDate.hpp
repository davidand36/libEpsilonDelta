#ifndef ETHIOPIANDATE_HPP
#define ETHIOPIANDATE_HPP
/*
  EthiopianDate.hpp
  Copyright (C) 2007 David M. Anderson

  EthiopianDate class, representing a date in the Ethiopian calendar.
*/


#include "DateDMYW.hpp"
#include "EthiopianCalendar.hpp"
#include "EthiopianWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< EthiopianCalendar, EthiopianWeek >  EthiopianDate;


//=============================================================================

#ifdef DEBUG
bool TestEthiopianDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ETHIOPIANDATE_HPP

#ifndef PERSIANDATE_HPP
#define PERSIANDATE_HPP
/*
  PersianDate.hpp
  Copyright (C) 2007 David M. Anderson

  PersianDate class, representing a date in the Persian calendar.
*/


#include "DMYWDate.hpp"
#include "PersianCalendar.hpp"
#include "PersianWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DMYWDate< PersianCalendar, PersianWeek >  PersianDate;


//=============================================================================

#ifdef DEBUG
bool TestPersianDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PERSIANDATE_HPP

#ifndef BAHAIDATE_HPP
#define BAHAIDATE_HPP
/*
  BahaiDate.hpp
  Copyright (C) 2007 David M. Anderson

  BahaiDate class, representing a date in the Bahai calendar.
*/


#include "DMYWDate.hpp"
#include "BahaiCalendar.hpp"
#include "BahaiWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DMYWDate< BahaiCalendar, BahaiWeek >  BahaiDate;


//=============================================================================

#ifdef DEBUG
bool TestBahaiDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BAHAIDATE_HPP

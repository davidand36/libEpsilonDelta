#ifndef EGYPTIANDATE_HPP
#define EGYPTIANDATE_HPP
/*
  EgyptianDate.hpp
  Copyright (C) 2007 David M. Anderson

  EgyptianDate class, representing a date in the Egyptian calendar.
*/


#include "DMYWDate.hpp"
#include "EgyptianCalendar.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DMYDate< EgyptianCalendar >  EgyptianDate;


//=============================================================================

#ifdef DEBUG
bool TestEgyptianDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EGYPTIANDATE_HPP

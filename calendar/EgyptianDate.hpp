#ifndef EGYPTIANDATE_HPP
#define EGYPTIANDATE_HPP
/*
  EgyptianDate.hpp
  Copyright (C) 2007 David M. Anderson

  EgyptianDate class, representing a date in the Egyptian calendar.
*/


#include "DateDMYW.hpp"
#include "EgyptianCalendar.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMY< EgyptianCalendar >  EgyptianDate;


//=============================================================================

#ifdef DEBUG
bool TestEgyptianDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EGYPTIANDATE_HPP

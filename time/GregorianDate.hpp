#ifndef GREGORIANDATE_HPP
#define GREGORIANDATE_HPP
/*
  GregorianDate.hpp
  Copyright (C) 2007 David M. Anderson

  GregorianDate class, representing a date in the Gregorian calendar.
*/


#include "DMYWDate.hpp"
#include "GregorianCalendar.hpp"
#include "WesternWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DMYWDate< GregorianCalendar, WesternWeek >  GregorianDate;


//=============================================================================


template<>
void DMYDate< GregorianCalendar >::Set( bool today );

//=============================================================================

#ifdef DEBUG
bool TestGregorianDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GREGORIANDATE_HPP

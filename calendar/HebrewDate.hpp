#ifndef HEBREWDATE_HPP
#define HEBREWDATE_HPP
/*
  HebrewDate.hpp
  Copyright (C) 2007 David M. Anderson

  HebrewDate class, representing a date in the Hebrew calendar.
*/


#include "DateDMYW.hpp"
#include "HebrewCalendar.hpp"
#include "HebrewWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< HebrewCalendar, HebrewWeek >  HebrewDate;


//=============================================================================


template<>
void DateDMY< HebrewCalendar >::Set( bool today );

//=============================================================================

#ifdef DEBUG
bool TestHebrewDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HEBREWDATE_HPP

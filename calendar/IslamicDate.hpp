#ifndef ISLAMICDATE_HPP
#define ISLAMICDATE_HPP
/*
  IslamicDate.hpp
  Copyright (C) 2007 David M. Anderson

  IslamicDate class, representing a date in the Islamic calendar.
*/


#include "DateDMYW.hpp"
#include "IslamicCalendar.hpp"
#include "IslamicWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< IslamicCalendar, IslamicWeek >  IslamicDate;


//=============================================================================

#ifdef DEBUG
bool TestIslamicDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ISLAMICDATE_HPP

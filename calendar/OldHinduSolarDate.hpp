#ifndef OLDHINDUSOLARDATE_HPP
#define OLDHINDUSOLARDATE_HPP
/*
  OldHinduSolarDate.hpp
  Copyright (C) 2007 David M. Anderson

  Old HinduSolarDate class, representing a date in the old Hindu solar calendar.
*/


#include "DateDMYW.hpp"
#include "OldHinduSolarCalendar.hpp"
#include "HinduWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< OldHinduSolarCalendar, HinduWeek >  OldHinduSolarDate;


//=============================================================================


template<>
void DateDMY< OldHinduSolarCalendar >::Set( bool today );

//=============================================================================

#ifdef DEBUG
bool TestOldHinduSolarDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //OLDHINDUSOLARDATE_HPP

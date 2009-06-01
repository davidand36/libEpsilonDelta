#ifndef PERSIANASTRONOMICALDATE_HPP
#define PERSIANASTRONOMICALDATE_HPP
/*
  PersianAstronomicalDate.hpp
  Copyright (C) 2007 David M. Anderson

  PersianAstronomicalDate class, representing a date in the Persian
  astronomical calendar.
*/


#include "DateDMYW.hpp"
#include "PersianAstronomicalCalendar.hpp"
#include "PersianWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< PersianAstronomicalCalendar, PersianWeek >
        PersianAstronomicalDate;


//=============================================================================

#ifdef DEBUG
bool TestPersianAstronomicalDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PERSIANASTRONOMICALDATE_HPP

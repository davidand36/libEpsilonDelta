#ifndef PERSIANARITHMETICDATE_HPP
#define PERSIANARITHMETICDATE_HPP
/*
  PersianArithmeticDate.hpp
  Copyright (C) 2007 David M. Anderson

  PersianArithmeticDate class, representing a date in the Persian arithmetic
  calendar.
*/


#include "DateDMYW.hpp"
#include "PersianArithmeticCalendar.hpp"
#include "PersianWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< PersianArithmeticCalendar, PersianWeek >
        PersianArithmeticDate;


//=============================================================================

#ifdef DEBUG
bool TestPersianArithmeticDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PERSIANARITHMETICDATE_HPP

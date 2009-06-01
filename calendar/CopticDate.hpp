#ifndef COPTICDATE_HPP
#define COPTICDATE_HPP
/*
  CopticDate.hpp
  Copyright (C) 2007 David M. Anderson

  CopticDate class, representing a date in the Coptic calendar.
*/


#include "DateDMYW.hpp"
#include "CopticCalendar.hpp"
#include "CopticWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< CopticCalendar, CopticWeek >  CopticDate;


//=============================================================================

#ifdef DEBUG
bool TestCopticDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //COPTICDATE_HPP

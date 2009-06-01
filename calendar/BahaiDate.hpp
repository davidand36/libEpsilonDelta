#ifndef BAHAIDATE_HPP
#define BAHAIDATE_HPP
/*
  BahaiDate.hpp
  Copyright (C) 2007 David M. Anderson

  BahaiDate class, representing a date in the Bahai calendar.
*/


#include "DateDMYW.hpp"
#include "BahaiCalendar.hpp"
#include "BahaiWeek.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMYW< BahaiCalendar, BahaiWeek >  BahaiDate;


//=============================================================================

#ifdef DEBUG
bool TestBahaiDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BAHAIDATE_HPP

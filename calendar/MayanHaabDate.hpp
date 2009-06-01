#ifndef MAYANHAABDATE_HPP
#define MAYANHAABDATE_HPP
/*
  MayanHaabDate.hpp
  Copyright (C) 2007 David M. Anderson

  MayanHaabDate class, representing a date in the Mayan haab calendar.
*/


#include "DateDMYW.hpp"
#include "MayanHaabCalendar.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


typedef DateDMY< MayanHaabCalendar >  MayanHaabDate;


//=============================================================================

#ifdef DEBUG
bool TestMayanHaabDate( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MAYANHAABDATE_HPP

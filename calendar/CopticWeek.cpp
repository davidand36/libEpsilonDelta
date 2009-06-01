/*
  CopticWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class CopticWeek, representing the seven-day week of the Coptic calendar.
*/


#include "CopticWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
CopticWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_copticWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

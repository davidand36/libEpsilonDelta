/*
  ArmenianWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class ArmenianWeek, representing the seven-day week of the Armenian calendar.
*/


#include "ArmenianWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
ArmenianWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_armenianWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

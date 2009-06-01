/*
  BahaiWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class BahaiWeek, representing the seven-day week of the Bahai calendar.
*/


#include "BahaiWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
BahaiWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_bahaiWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

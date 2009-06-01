/*
  HinduWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class HinduWeek, representing the seven-day week of the Hindu calendar.
*/


#include "HinduWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
HinduWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_hinduWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

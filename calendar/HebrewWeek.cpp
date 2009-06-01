/*
  HebrewWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class HebrewWeek, representing the seven-day week of the Hebrew calendar.
*/


#include "HebrewWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
HebrewWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_hebrewWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

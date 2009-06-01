/*
  PersianWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class PersianWeek, representing the seven-day week of the Persian calendar.
*/


#include "PersianWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
PersianWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_persianWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

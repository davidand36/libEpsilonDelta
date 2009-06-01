/*
  IslamicWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class IslamicWeek, representing the seven-day week of the Islamic calendar.
*/


#include "IslamicWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
IslamicWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_islamicWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

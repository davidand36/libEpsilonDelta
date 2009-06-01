/*
  WesternWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class WesternWeek, representing the seven-day week of
  the Julian and Gregorian calendars.
*/


#include "WesternWeek.hpp"
#include "TimeLibText.hpp"
#include "Assert.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
WesternWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_westernWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

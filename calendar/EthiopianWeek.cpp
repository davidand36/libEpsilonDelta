/*
  EthiopianWeek.cpp
  Copyright (C) 2007 David M. Anderson

  Class EthiopianWeek, representing the seven-day week of the Ethiopian
  calendar.
*/


#include "EthiopianWeek.hpp"
#include "Assert.hpp"
#include "CalendarLibText.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const std::string &
EthiopianWeek::WeekDayName( int weekday )
{
    Assert( (weekday >= 0) && (weekday < DaysInWeek( )) );
    return g_ethiopianWeekDayNames[ weekday ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

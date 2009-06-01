#ifndef TIMELIBTEXT_HPP
#define TIMELIBTEXT_HPP
/*
  TimeLibText.hpp
  Copyright (C) 2007 David M. Anderson

  Strings, such as month and weekday names, used in TimeLib.
  In principle most of these should be included in the relevent classes,
  but keeping them all in one file makes localization and other maintainance
  easier.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


extern const std::string g_ampmAbbreviations[ 2 ];
extern const std::string g_AMPMAbbreviations[ 2 ];
extern const std::string g_impossibleTimeText;
extern const std::string g_indeterminateTimeText;
extern const std::string g_westernMonthNames[ 12 ];
extern const std::string g_westernWeekDayNames[ 7 ];


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //TIMELIBTEXT_HPP

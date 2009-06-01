#ifndef CALENDARLIBTEXT_HPP
#define CALENDARLIBTEXT_HPP
/*
  CalendarLibText.hpp
  Copyright (C) 2007 David M. Anderson

  Strings, such as month and weekday names, used in CalendarLib.
  In principle most of these should be included in the relevent classes,
  but keeping them all in one file makes localization and other maintainance
  easier.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


extern const std::string g_islamicMonthNames[ 12 ];
extern const std::string g_islamicWeekDayNames[ 7 ];
extern const std::string g_persianMonthNames[ 12 ];
extern const std::string g_persianWeekDayNames[ 7 ];
extern const std::string g_hebrewMonthNamesCY[ 12 ];
extern const std::string g_hebrewMonthNamesLY[ 13 ];
extern const std::string g_hebrewWeekDayNames[ 7 ];
extern const std::string g_copticMonthNames[ 13 ];
extern const std::string g_copticWeekDayNames[ 7 ];
extern const std::string g_ethiopianMonthNames[ 13 ];
extern const std::string g_ethiopianWeekDayNames[ 7 ];
extern const std::string g_bahaiDayNames[ 19 ];
extern const std::string g_bahaiMonthNames[ 20 ];
extern const std::string g_bahaiYearNames[ 19 ];
extern const std::string g_bahaiWeekDayNames[ 7 ];
extern const std::string g_chineseMajorSolarTermNames[ 12 ];
extern const std::string g_chineseMinorSolarTermNames[ 12 ];
extern const std::string g_chineseMajorSolarTermEnglishNames[ 12 ];
extern const std::string g_chineseMinorSolarTermEnglishNames[ 12 ];
extern const std::string g_chineseCelestialStemNames[ 10 ];
extern const std::string g_chineseTerrestrialBranchNames[ 12 ];
extern const std::string g_chineseCelestialStemEnglishNames[ 10 ];
extern const std::string g_chineseTerrestrialBranchEnglishNames[ 12 ];
extern const std::string g_chineseElementNames[ 5 ];
extern const std::string g_chineseElementEnglishNames[ 5 ];
extern const std::string g_hinduSolarMonthNames[ 12 ];
extern const std::string g_hinduLunarMonthNames[ 12 ];
extern const std::string g_hinduWeekDayNames[ 7 ];
extern const std::string g_egyptianMonthNames[ 13 ];
extern const std::string g_armenianMonthNames[ 13 ];
extern const std::string g_armenianWeekDayNames[ 13 ];
extern const std::string g_mayanHaabMonthNames[ 19 ];
extern const std::string g_mayanTzolkinVeintenaNames[ 20 ];
extern const std::string g_frenchRevolutionaryMonthNames[ 13 ];
extern const std::string g_frenchRevolutionaryDecadeNames[ 10 ];
extern const std::string g_frenchRevolutionarySansculottidesNames[ 6 ];


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CALENDARLIBTEXT_HPP

/*
  TimeLibText.cpp
  Copyright (C) 2007 David M. Anderson

  Strings used in TimeLib.
  In principle most of these should be included in the relevent classes,
  but keeping them all in one file makes localization and other maintainance
  easier.
*/


#include "TimeLibText.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const string g_ampmAbbreviations[ 2 ]
    = {
        "a.m.",
        "p.m."
    };
const string g_AMPMAbbreviations[ 2 ]
    = {
        "AM",
        "PM"
    };
const string g_impossibleTimeText
    = "Impossible time";
const string g_indeterminateTimeText
    = "Indeterminate time";

const string g_westernMonthNames[ 12 ]
    = {
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };
const string g_westernWeekDayNames[ 7 ]
    = {
        "Sunday",
        "Monday",
        "Tuesday",
        "Wednesday",
        "Thursday",
        "Friday",
        "Saturday"
    };


//*****************************************************************************

}                                                      //namespace EpsilonDelta

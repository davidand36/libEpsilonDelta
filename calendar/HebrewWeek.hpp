#ifndef HEBREWWEEK_HPP
#define HEBREWWEEK_HPP
/*
  HebrewWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class HebrewWeek, representing the seven-day week of the Hebrew calendar.
  The Hebrew week begins on Yom Reeshone (Sunday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class HebrewWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { YomRishon = 0, YomSheni, YomSelishi, YomRevii,
      YomHamishi, YomShishi, Shabbat };
};


//*****************************************************************************


inline 
int
HebrewWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
HebrewWeek::DayOfWeekOfJD0( )
{
    return YomSheni;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HEBREWWEEK_HPP

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
    { YomReeshone = 0, YomShaynee, YomShleeshee, YomReveeee,
      YomKhahmeeshee, YomHasheeshee, Shabbat };
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
    return YomShaynee;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HEBREWWEEK_HPP

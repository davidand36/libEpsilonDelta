#ifndef WESTERNWEEK_HPP
#define WESTERNWEEK_HPP
/*
  WesternWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class WesternWeek, representing the seven-day week of
  the Julian and Gregorian calendars.
  We regard the Western week as beginning on Sunday, though this is 
  not a universal standard.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class WesternWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay 
    { Sunday = 0, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };
};



//*****************************************************************************


inline 
int
WesternWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
WesternWeek::DayOfWeekOfJD0( )
{
    return Monday;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //WESTERNWEEK_HPP

#ifndef ARMENIANWEEK_HPP
#define ARMENIANWEEK_HPP
/*
  ArmenianWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class ArmenianWeek, representing the seven-day week of the Armenian calendar.
  The Armenian week begins on Miashabathi (Sunday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ArmenianWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { Miashabathi = 0, Erkoushabathi, Erekhshabathi, Chorekhshabathi,
      Hingshabathi, Urbath, Shabath };
};



//*****************************************************************************


inline 
int
ArmenianWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
ArmenianWeek::DayOfWeekOfJD0( )
{
    return Erkoushabathi;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ARMENIANWEEK_HPP

#ifndef PERSIANWEEK_HPP
#define PERSIANWEEK_HPP
/*
  PersianWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class PersianWeek, representing the seven-day week of the Persian calendar.
  The Persian week begins on Shanbeh (Saturday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class PersianWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { Shanbeh = 0, YekShanbeh, DoShanbeh, SeShanbeh, ChaharShanbeh,
      PanjShanbeh, Jomeh };
};


//*****************************************************************************


inline 
int
PersianWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
PersianWeek::DayOfWeekOfJD0( )
{
    return DoShanbeh;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PERSIANWEEK_HPP

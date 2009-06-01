#ifndef ISLAMICWEEK_HPP
#define ISLAMICWEEK_HPP
/*
  IslamicWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class IslamicWeek, representing the seven-day week of the Islamic calendar.
  The Islamic week begins on yawm al-'ahad (Sunday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class IslamicWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { YawmAlAhad = 0, YawmAlIthnayn, YawmAthThalatha, YawmAlArbaa,
      YawmAlKhamis, YawmAlJuma, YawmAsSabt };
};



//*****************************************************************************


inline 
int
IslamicWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
IslamicWeek::DayOfWeekOfJD0( )
{
    return YawmAlIthnayn;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ISLAMICWEEK_HPP

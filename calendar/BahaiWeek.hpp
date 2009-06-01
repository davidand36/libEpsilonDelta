#ifndef BAHAIWEEK_HPP
#define BAHAIWEEK_HPP
/*
  BahaiWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class BahaiWeek, representing the seven-day week of the Bahai calendar.
  The Bahai week begins on Jalal (Saturday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class BahaiWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { Jalal = 0, Jamal, Kamal, Fidal, Idal, Istijlal, Istiqlal };
};



//*****************************************************************************


inline 
int
BahaiWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
BahaiWeek::DayOfWeekOfJD0( )
{
    return Kamal;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BAHAIWEEK_HPP

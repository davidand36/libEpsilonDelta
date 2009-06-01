#ifndef ETHIOPIANWEEK_HPP
#define ETHIOPIANWEEK_HPP
/*
  EthiopianWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class EthiopianWeek, representing the seven-day week of the Ethiopian
  calendar.
  The Ethiopian week begins on Ihud (Sunday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class EthiopianWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { Ihud = 0, Sanyo, Maksanyo, Rob, Hamus, Arb, Kidamme };
};


//*****************************************************************************


inline 
int
EthiopianWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
EthiopianWeek::DayOfWeekOfJD0( )
{
    return Sanyo;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ETHIOPIANWEEK_HPP

#ifndef HINDUWEEK_HPP
#define HINDUWEEK_HPP
/*
  HinduWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class HinduWeek, representing the seven-day week of the Hindu calendar.
  The Hindu week begins on Jalal (Saturday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class HinduWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { Ravivara = 0, Chandravara, Mangalavara, Buddhavara,
      Brihaspatvara, Sukravara, Sanivara };
};



//*****************************************************************************


inline 
int
HinduWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
HinduWeek::DayOfWeekOfJD0( )
{
    return Chandravara;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HINDUWEEK_HPP

#ifndef COPTICWEEK_HPP
#define COPTICWEEK_HPP
/*
  CopticWeek.hpp
  Copyright (C) 2007 David M. Anderson

  Class CopticWeek, representing the seven-day week of the Coptic calendar.
  The Coptic week begins on Tkyriake (Sunday).
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class CopticWeek
{
public:
    static int DaysInWeek( );
    static int DayOfWeekOfJD0( );
    static const std::string & WeekDayName( int weekday );

    enum EWeekDay
    { Tkyriake = 0, Pesnau, Pshoment, Peftoou, Ptiou, Psoou, Psabbaton };
};


//*****************************************************************************


inline 
int
CopticWeek::DaysInWeek( )
{
    return 7;
}

//-----------------------------------------------------------------------------

inline 
int
CopticWeek::DayOfWeekOfJD0( )
{
    return Pesnau;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //COPTICWEEK_HPP

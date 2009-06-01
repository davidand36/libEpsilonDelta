#ifndef OLDHINDUSOLARCALENDAR_HPP
#define OLDHINDUSOLARCALENDAR_HPP
/*
  OldHinduSolarCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class OldHinduSolarCalendar, representing the old Hindu solar calendar.
*/


#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class OldHinduSolarCalendar
{
public:
    static void JulianDayToDMY( int julianDay, 
                                int * pDay, int * pMonth, int * pYear );
    static int DMYToJulianDay( int day, int month, int year );
    static int MonthsInYear( int year );
    static int DaysInMonth( int month, int year );
    static const std::string & MonthName( int month, int year );

    enum EMonth
    { Mesha = 1, Vrishabha, Mithuna, Karka, Simha, Kanya,
      Tula, Vrischika, Dhanu, Makara, Kumbha, Mina };
};


//*****************************************************************************


inline 
int 
OldHinduSolarCalendar::MonthsInYear( int /*year*/ )
{
    return 12;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //OLDHINDUSOLARCALENDAR_HPP

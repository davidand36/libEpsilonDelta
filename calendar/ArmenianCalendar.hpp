#ifndef ARMENIANCALENDAR_HPP
#define ARMENIANCALENDAR_HPP
/*
  ArmenianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class ArmenianCalendar, which defines the Armenian calendar.

  The calendar of the ancient Armenians.
*/


#include "Assert.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ArmenianCalendar
{
public:
    static void JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear );
    static int DMYToJulianDay( int day, int month, int year );
    static int MonthsInYear( int year = 0 );
    static int DaysInMonth( int month, int year = 0 );
    static const std::string & MonthName( int month, int year );
    static const std::string & MonthName( int month );

    enum EMonth
    { Nawasardi = 1, Hori, Sahmi, Tre, Kaloch, Arach, Mehekani,
      Areg, Ahekani, Mareri, Margach, Hrotich, Aweleach };
};


//*****************************************************************************


inline
int
ArmenianCalendar::DaysInMonth( int month, int /*year*/ )
{
    Assert( (month > 0) && (month <= MonthsInYear( )) );
    return  ( (month < 13)  ?  30  :  5 );
}

//-----------------------------------------------------------------------------

inline
int
ArmenianCalendar::MonthsInYear( int /*year*/ )
{
    return 13;  //including the Epagomenae.
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ARMENIANCALENDAR_HPP

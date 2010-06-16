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
    static void JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static int MonthsInYear( long year = 0 );
    static int DaysInMonth( int month, long year = 0 );
    static const std::string & MonthName( int month, long year );
    static const std::string & MonthName( int month );

    enum EMonth
    { Nawasardi = 1, Hori, Sahmi, Tre, Kaloch, Arach, Mehekani,
      Areg, Ahekani, Mareri, Margach, Hrotich, Aweleach };
};


//*****************************************************************************


inline
int
ArmenianCalendar::DaysInMonth( int month, long /*year*/ )
{
    Assert( (month > 0) && (month <= MonthsInYear( )) );
    return  ( (month < 13)  ?  30  :  5 );
}

//-----------------------------------------------------------------------------

inline
int
ArmenianCalendar::MonthsInYear( long /*year*/ )
{
    return 13;  //including the Epagomenae.
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ARMENIANCALENDAR_HPP

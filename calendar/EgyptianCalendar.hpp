#ifndef EGYPTIANCALENDAR_HPP
#define EGYPTIANCALENDAR_HPP
/*
  EgyptianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class EgyptianCalendar, which defines the Egyptian calendar.

  The calendar of the ancient Egyptians.
*/


#include "Assert.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class EgyptianCalendar
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
    { Thoth = 1, Phaophi, Athyr, Choiak, Tybi, Mechir, Phamenoth,
      Pharmuthi, Pachon, Payni, Epiphi, Mesori, Epagomenae };
};


//*****************************************************************************


inline
int
EgyptianCalendar::DaysInMonth( int month, long /*year*/ )
{
    Assert( (month > 0) && (month <= MonthsInYear( )) );
    return  ( (month < 13)  ?  30  :  5 );
}

//-----------------------------------------------------------------------------

inline
int
EgyptianCalendar::MonthsInYear( long /*year*/ )
{
    return 13;  //including the Epagomenae.
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EGYPTIANCALENDAR_HPP

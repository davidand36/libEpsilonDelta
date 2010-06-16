#ifndef COPTICCALENDAR_HPP
#define COPTICCALENDAR_HPP
/*
  CopticCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class CopticCalendar, which defines the Coptic calendar.

  The calendar of the Egyptian Christians, the Copts, was established in the
  fourth century A.D. It is very similar to the ancient Egyptian civil
  calendar. Its epoch is JD 1,825,030. The day begins at sunset.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class CopticCalendar
{
public:
    static void JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static int MonthsInYear( long year );
    static int DaysInMonth( int month, long year );
    static const std::string & MonthName( int month, long year );
    static const std::string & MonthName( int month );
    static bool IsLeapYear( long year );

    enum EMonth
    { Thoout = 1, Paope, Athor, Koiak, Tobe, Meshir, Paremotep,
      Parmoute, Pashons, Paone, Epap, Mesore, Epagomene };
};


//*****************************************************************************


inline
int
CopticCalendar::MonthsInYear( long /*year*/ )
{
    return 13;  //including Epagomene.
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //COPTICCALENDAR_HPP

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
    static void JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear );
    static int DMYToJulianDay( int day, int month, int year );
    static int MonthsInYear( int year );
    static int DaysInMonth( int month, int year );
    static const std::string & MonthName( int month, int year );
    static const std::string & MonthName( int month );
    static bool IsLeapYear( int year );

    enum EMonth
    { Thoout = 1, Paope, Athor, Koiak, Tobe, Meshir, Paremotep,
      Parmoute, Pashons, Paone, Epap, Mesore, Epagomene };
};


//*****************************************************************************


inline
int
CopticCalendar::MonthsInYear( int /*year*/ )
{
    return 13;  //including Epagomene.
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //COPTICCALENDAR_HPP

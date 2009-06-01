#ifndef ETHIOPIANCALENDAR_HPP
#define ETHIOPIANCALENDAR_HPP
/*
  EthiopianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class EthiopianCalendar, which defines the Ethiopian calendar.

  The calendar of the Ethiopian Christians is the same as the Coptic
  calendar, except for its epoch, which is JD 1,724,221. The day begins at
  sunset. Month names are also different.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class EthiopianCalendar
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
    { Maskaram = 1, Teqemt, Khedar, Takhsas, Ter, Yakatit,
      Magabit, Miyazya, Genbot, Sane, Hamle, Nahase, Paguemen };
};


//*****************************************************************************


inline
int
EthiopianCalendar::MonthsInYear( int /*year*/ )
{
    return 13;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ETHIOPIANCALENDAR_HPP

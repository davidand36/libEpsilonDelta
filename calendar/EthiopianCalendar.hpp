#ifndef ETHIOPIANCALENDAR_HPP
#define ETHIOPIANCALENDAR_HPP
/*
  EthiopianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class EthiopianCalendar, which defines the Ethiopian calendar.

  The calendar of the Ethiopian Christians is the same as the Coptic
  calendar, except for its epoch, which is JD 1,724,221 (27 August 8 C.E.
  Gregorian), calculated to be the year of the Annunciation of Jesus.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class EthiopianCalendar
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
    { Maskaram = 1, Teqemt, Khedar, Takhsas, Ter, Yakatit,
      Magabit, Miyazya, Genbot, Sane, Hamle, Nahase, Paguemen };
};


//*****************************************************************************


inline
int
EthiopianCalendar::MonthsInYear( long /*year*/ )
{
    return 13;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ETHIOPIANCALENDAR_HPP

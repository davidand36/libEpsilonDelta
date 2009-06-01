#ifndef PERSIANASTRONOMICALCALENDAR_HPP
#define PERSIANASTRONOMICALCALENDAR_HPP
/*
  PersianAstronomicalCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class PersianAstronomicalCalendar, which defines the Persian calendar,
  as determined by the true astronomical equinox.

  The modern Persian calander, a.k.a. the Persian Solar or Iranian Solar
  calendar, was established on 11 Farvardin 1304 A.H.S. (Anno Hegirae Solis
  or Anno Hijri Shamsi) = 31 March 1925 A.D. (Gregorian) under
  Reza Shah Pahlavi. The epoch is JD 1,948,321, the year of the Hijra.
  For two years around 1976 A.D., the epoch was changed so that 1355 A.H.S.
  became 2535 Sh. (Shahinshah Era) and 1356 A.H.S. became 2536 Sh.
  The day begins at sunset.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class PersianAstronomicalCalendar
{
public:
    static void JulianDayToDMY( int julianDay,
                                int * pDay, int * pMonth, int * pYear );
    static int DMYToJulianDay( int day, int month, int year );
    static int MonthsInYear( int year );
    static int DaysInMonth( int month, int year );
    static const std::string & MonthName( int month, int year = 0 );
    static bool IsLeapYear( int year );

    enum EMonth
    { Farvardin = 1, Ordibehesht, Khordad, Tir, Mordad, Shahrivar,
      Mehr, Aban, Azar, Dey, Bahman, Esfand };
};


//*****************************************************************************


inline
int
PersianAstronomicalCalendar::MonthsInYear( int /*year*/ )
{
    return 12;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PERSIANASTRONOMICALCALENDAR_HPP

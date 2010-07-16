#ifndef JULIANCALENDAR_HPP
#define JULIANCALENDAR_HPP
/*
  JulianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class JulianCalendar, which defines the Julian calendar.

  The Julian calendar was established by Julius Caeser, in consultation with
  Sisogenes of Alexandria, on 1 January 709 A.U.C. (ad urbe condita)
  = 1 January 45 B.C. (Julian) = 30 December 45 B.C.E. (Gregorian).
  The form presented here is not that used in ancient Rome, however, but that
  developed by the Roman Catholic church. That church replaced it with the
  Gregorian calendar in 1582 C.E., but the Julian calendar remained in use in
  many Protestant countries and Eastern Europe much longer. Most of the
  Orthodox Christian church continues to use the Julian calendar for the
  determination of religious holidays (movable feasts).

  The A.D. (anno Domini) epoch was developed by Dionysius Exiguus of Rome in
  the sixth century C.E., based on his estimate of the birth year of Jesus.
  This epoch is JD 1,721,424. The day begins at midnight.

  The Julian calendar is a solar calendar. It uses leap years, one every four
  years, in an effort to remain synchronized with the seasons (i.e., the
  tropical year). Over time the approximation proved insufficient, leading to
  the development of the Gregorian calendar. Although the month was originally
  inspired by the lunar cycle, the Julian calendar makes no real effort to
  stay synchronized with the phases of the Moon; instead the month lengths
  were set to fit the solar year.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class JulianCalendar
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
    { January = 1, February, March, April, May, June,
      July, August, September, October, November, December };
};


//*****************************************************************************


inline
int
JulianCalendar::MonthsInYear( long /*year*/ )
{
    return 12;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JULIANCALENDAR_HPP

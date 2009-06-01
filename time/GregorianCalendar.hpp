#ifndef GREGORIANCALENDAR_HPP
#define GREGORIANCALENDAR_HPP
/*
  GregorianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class GregorianCalendar, which defines the Gregorian calendar.

  The Gregorian calendar was established by Pope Gregory XIII on
  15 October 1582 A.D. (which would have been 5 October 1582 A.D. in the
  Julian calendar). It was adopted immediately in Catholic states, and
  gradually by Protestant ones. (Great Britain and her colonies, including
  the United States, waited until 1752; Russia, until 1918; and Turkey,
  until 1927.) It is the standard civil calendar in most of the world today.
  Its epoch (A.D., anno Domini) is JD 1,721,426. The day begins at midnight.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename Cal> class DateDMY;


//*****************************************************************************


class GregorianCalendar
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
    { January = 1, February, March, April, May, June,
      July, August, September, October, November, December };

private:
    static void Today( int * pDay, int * pMonth, int * pYear );

    friend class DateJD;
    friend class DateDMY<GregorianCalendar>;
};


//*****************************************************************************


inline 
int
GregorianCalendar::MonthsInYear( int /*year*/ )
{
    return 12;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GREGORIANCALENDAR_HPP

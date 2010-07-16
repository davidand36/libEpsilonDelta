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
  the United States, waited until 1752; Russia, until 1918.) It is the
  standard civil calendar in most of the world today.
  Its epoch (A.D., anno Domini) is JD 1,721,426. The day begins at midnight.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template <typename Cal> class DMYDate;


//*****************************************************************************


class GregorianCalendar
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

private:
    static void Today( int * pDay, int * pMonth, long * pYear );

    friend class JDDate;
    friend class DMYDate<GregorianCalendar>;
};


//*****************************************************************************


inline 
int
GregorianCalendar::MonthsInYear( long /*year*/ )
{
    return 12;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GREGORIANCALENDAR_HPP

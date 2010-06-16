#ifndef JULIANCALENDAR_HPP
#define JULIANCALENDAR_HPP
/*
  JulianCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class JulianCalendar, which defines the Julian calendar.

  The Julian calendar was established by Julius Caeser, in consultation with
  Sisogenes of Alexandria, on 1 January 709 A.U.C. (ad urbe condita)
  = 1 January 45 B.C. The A.D. (anno Domini) epoch was developed by
  Dionysius Exiguus of Rome in the sixth century A.D., based on his estimate
  of the birth year of Jesus. This epoch is JD 1,721,424. The day begins at
  midnight.
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

#ifndef HEBREWCALENDAR_HPP
#define HEBREWCALENDAR_HPP
/*
  HebrewCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class HebrewCalendar, which defines the Hebrew calendar.

  The Hebrew calander, a.k.a. the Jewish calendar, was promulgated in the
  mid-fourth century (perhaps 359) A.D. by the Patriarch, Hillel II.
  It is a lunisolar calendar with a number of complications. For one thing,
  the year begins with month seven, Tishri. For another, in addition to 
  leap months (Adar II), two of the months, Heshvan (8) and Kislev (9)
  are of variable length. They are adjusted to conform with several rules,
  e.g., Yom Kippur (10 Tishri) cannot fall on a Friday or Sunday. The epoch,
  1 A.M. (Anno Mundi, the traditional year of the world since creation),
  is JD 347,998. The day begins at sunset for religious purposes, which is
  deemed 6 p.m. for civil purposes.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class HebrewCalendar
{
public:
    static void JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static int MonthsInYear( long year );
    static int DaysInMonth( int month, long year );
    static const std::string & MonthName( int month, long year );
    static bool IsLeapYear( long year );

    enum EMonth
    { Nisan = 1, Iyyar, Sivan, Tammuz, Av, Elul, 
      Tishri, Heshvan, Kislev, Teveth, Shevat, Adar, AdarII };
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HEBREWCALENDAR_HPP

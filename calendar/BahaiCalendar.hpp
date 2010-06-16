#ifndef BAHAICALENDAR_HPP
#define BAHAICALENDAR_HPP
/*
  BahaiCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class BahaiCalendar, which defines the Baha'i calendar.

  The Baha'i calendar is the calendar of the Baha'i faith. It was developed 
  some time between 1844 and 1850 A.D. by al-Bab (Mirza Ali Mohammad), 
  the founder of the faith. Some details were clarified by his disciple, 
  Baha'Ullah (Mirza Husayn Ali Nuri). The calendar is strongly influenced by 
  the abjad numeralogical system, and emphasizes the number 19. There are 
  still some variations on the calendar. In Iran, Naw Ruz (the new year) 
  occurs on the actual Spring equinox, like the modern Persian calendar, 
  while in the West the calendar is synchronized with the Gregorian calendar 
  so that Naw Ruz begins on 21 March. We follow the latter practice here. 
  One variation, implemented as BadiCalendar, uses cycles of cycles of years. 
  The more commonly used sequential numbering of years is implemented here.
  The epoch (B.E., Baha'i Era) is JD 2,394,647, 21 March 1844 A.D., 
  the year of al-Bab's declaration.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class BahaiCalendar
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
    { Baha = 1, Jalal, Jamal, Azamat, Nur, Rahmat, Kalimat, Kamal, Asma,
      Izzat, Mashiyyat, Ilm, Qudrat, Qawl, Masail, Sharaf, Sultan, Mulk,
      AyyamiHa, Ala };
};


//*****************************************************************************


inline
int
BahaiCalendar::MonthsInYear( long /*year*/ )
{
    return 20;  //Includes the intercalary period, Ayyam-i-Ha.
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BAHAICALENDAR_HPP

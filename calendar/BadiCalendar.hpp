#ifndef BADICALENDAR_HPP
#define BADICALENDAR_HPP
/*
  BadiCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class BadiCalendar, which defines the Badi calendar.

  The Badi calendar is a variant of the Baha'i calendar. Neither calendar has 
  been completely standardized, but we use the term here to denote the
  variation that uses cycles (Kull-i-Shay) of 19 cycles (Vahid) of 19 years.
  See BahaiCalendar.hpp for more information.
  The epoch (B.E., Baha'i Era) is JD 2,394,647, 21 March 1844 A.D., 
  the year of al-Bab's declaration.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class BadiCalendar
{
public:
    static void JulianDayToDMYVK( long julianDay,
                                  int * pDay, int * pMonth, int * pYear,
                                  int * pVahid, long * pKulliShay );
    static long DMYVKToJulianDay( int day, int month,
                                 int year, int vahid, long kulliShay );
    static int DaysInMonth( int month, int year, int vahid, long kulliShay );
    static int MonthsInYear( );
    static int YearsInVahid( );
    static int VahidsInKulliShay( );
    static const std::string & DayName( int day );
    static const std::string & MonthName( int month );
    static const std::string & YearName( int year );
    static bool IsLeapYear( int year, int vahid, long kulliShay );

    static void YearToYVK( long seqYear,
                           int * pYear, int * pVahid, long * pKulliShay );
    static long YVKToYear( int year, int vahid, long kulliShay );

    enum EMonth
    { Baha = 1, Jalal, Jamal, Azamat, Nur, Rahmat, Kalimat, Kamal, Asma,
      Izzat, Mashiyyat, Ilm, Qudrat, Qawl, Masail, Sharaf, Sultan, Mulk,
      AyyamiHa, Ala };
};


//*****************************************************************************


inline
int
BadiCalendar::MonthsInYear( )
{
    return 20;  //Includes the intercalary period, Ayyam-i-Ha.
}

//-----------------------------------------------------------------------------

inline 
int 
BadiCalendar::YearsInVahid( )
{
    return 19;
}

//-----------------------------------------------------------------------------

inline 
int 
BadiCalendar::VahidsInKulliShay( )
{
    return 19;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BADICALENDAR_HPP

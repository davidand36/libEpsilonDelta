#ifndef FRENCHREVOLUTIONARYCALENDAR_HPP
#define FRENCHREVOLUTIONARYCALENDAR_HPP
/*
  FrenchRevolutionaryCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class FrenchRevolutionaryCalendar, which defines the French Revolutionary
  calendar.

  The calendar of the French Revolution.
*/


#include "Assert.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FrenchRevolutionaryCalendar
{
public:
    static void JulianDayToDMY( long julianDay,
                                int * pDay, int * pMonth, long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static bool IsLeapYear( long year );
    static int MonthsInYear( long year = 0 );
    static int DaysInMonth( int month, long year );
    static const std::string & MonthName( int month, long year = 0 );
    static int DaysInDecade( );
    static const std::string & DayName( int day, int month, long year = 0 );

    enum EMonth
    { Vendemiaire = 1, Brumaire, Frimaire,
      Nicose, Pluviose, Ventose,
      Germinal, Floreal, Prairial,
      Messidor, Thermidor, Fructidor,
      Sanscullottides };
};


//*****************************************************************************


inline
int
FrenchRevolutionaryCalendar::MonthsInYear( long /*year*/ )
{
    return 13;  //including the sansculottides
}

//-----------------------------------------------------------------------------

inline 
int 
FrenchRevolutionaryCalendar::DaysInDecade( )
{
    return 10;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FRENCHREVOLUTIONARYCALENDAR_HPP

#ifndef ISO8601CALENDAR_HPP
#define ISO8601CALENDAR_HPP
/*
  ISO8601Calendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class ISO8601Calendar, representing the ISO 8601 calendar.

  This calendar is prescribed by the International Standards Organization (ISO)
  in the publication "Data Elements and Interchange Formats--Information 
  Interchange--Representation of Dates," ISO 8601 (1988). This standard has 
  been adopted for various purposes, especially in Sweden and other European 
  countries. 
  The ISO standard is closely connected to the Gregorian calendar, but eschews 
  months in favor of weeks. The year always begins on a Monday, as does the
  week.
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ISO8601Calendar
{
public:
    static void JulianDayToDWY( int julianDay,
                                int * pDay, int * pWeek, int * pYear );
    static int DWYToJulianDay( int day, int week, int year );
    static int WeeksInYear( int year );
    static int DaysInWeek( );

    enum EWeekDay 
    { Monday = 1, Tuesday, Wednesday, Thursday, Friday, Saturday, Sunday };
};


//*****************************************************************************


inline 
int 
ISO8601Calendar::DaysInWeek( )
{
    return 7;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ISO8601CALENDAR_HPP

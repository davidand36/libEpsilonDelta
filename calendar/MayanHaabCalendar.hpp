#ifndef MAYANHAABCALENDAR_HPP
#define MAYANHAABCALENDAR_HPP
/*
  MayanHaabCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  MayanHaabCalendar class, representing the Mayan haab calendar.

  The haab is one of the three main calendars of the Mayan
  civilization, which was at its peak from about 250 to 900 C.E.
  It was the civil calendar based on an approximation of the solar year,
  with 18 "months" of 20 days, followed by five additional days to complete
  a 365-day cycle. This five-day Wayeb' (or Uayeb) was considered unlucky.
  Day numbers are zero-based. The first use of the haab was probably around
  550 B.C.E.

  There is no year number as such, so the haab could not be used for long-term
  record keeping. We introduce a year field here, somewhat arbitrarily using
  an epoch near that of the long count. This may allow interpretation of a
  particular haab date when the "year" can be estimated from other evidence,
  such as a nearby long count or other event.
*/


#include "Assert.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MayanHaabCalendar
{
public:
    static void JulianDayToDMY( long julianDay, int * pDay, int * pMonth,
                                 long * pYear = 0 );
    static long DMYToJulianDay( int day, int month, long year );
    static int DaysInMonth( int month, long year = 0 );
    static int MonthsInYear( long year = 0 );
    static const std::string & MonthName( int month, long year = 0 );

    enum EMonth 
    { Pop = 1, Uo, Zip, Zotz, Tzec, Xul, Yaxkin, Mol, Chen, Yax,
      Zac, Ceh, Mac, Kankin, Muan, Pax, Kayab, Cumku, Uayeb };
};


//*****************************************************************************


inline 
int 
MayanHaabCalendar::DaysInMonth( int month, long /*year*/ )
{
    Assert( (month >= 1) && (month <= 19) );
    return  ((month < 19)  ?  20  :  5);
}

//-----------------------------------------------------------------------------

inline 
int 
MayanHaabCalendar::MonthsInYear( long /*year*/ )
{
    return 19;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MAYANHAABCALENDAR_HPP

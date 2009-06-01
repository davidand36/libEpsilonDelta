#ifndef MAYANHAABCALENDAR_HPP
#define MAYANHAABCALENDAR_HPP
/*
  MayanHaabCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  MayanHaabCalendar class, representing the Mayan haab calendar.
*/


#include "Assert.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MayanHaabCalendar
{
public:
    static void JulianDayToDMY( int julianDay, int * pDay, int * pMonth,
                                 int * pYear = 0 );
    static int DMYToJulianDay( int day, int month, int year );
    static int DaysInMonth( int month, int year = 0 );
    static int MonthsInYear( int year = 0 );
    static const std::string & MonthName( int month, int year = 0 );

    enum EMonth 
    { Pop = 1, Uo, Zip, Zotz, Tzec, Xul, Yaxkin, Mol, Chen, Yax,
      Zac, Ceh, Mac, Kankin, Muan, Pax, Kayab, Cumku, Uayeb };
};


//*****************************************************************************


inline 
int 
MayanHaabCalendar::DaysInMonth( int month, int /*year*/ )
{
    Assert( (month >= 1) && (month <= 19) );
    return  ((month < 19)  ?  20  :  5);
}

//-----------------------------------------------------------------------------

inline 
int 
MayanHaabCalendar::MonthsInYear( int /*year*/ )
{
    return 19;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MAYANHAABCALENDAR_HPP

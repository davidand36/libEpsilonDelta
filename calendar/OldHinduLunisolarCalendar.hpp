#ifndef OLDHINDULUNISOLARCALENDAR_HPP
#define OLDHINDULUNISOLARCALENDAR_HPP
/*
  OldHinduLunisolarCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class OldHinduLunisolarCalendar, representing the old Hindu lunisolar
  calendar.
*/


#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class OldHinduLunisolarCalendar
{
public:
    static void JulianDayToDMLY( int julianDay, 
                                 int * pDay, int * pMonth, bool * pMonthLeap,
                                 int * pYear );
    static int DMLYToJulianDay( int day, int month, bool monthLeap, int year );

    static bool Valid( int day, int month, bool monthLeap, int year );
    static int MakeValid( int * pDay, int * pMonth, bool * pMonthLeap,
                          int * pYear, DateFixup::EMethod fixupMethod );
    static int LastDayOfMonth( int month, bool monthLeap, int year );
    static int LostDay( int month, bool monthLeap, int year );
    static int LeapMonth( int year );

    static const std::string & MonthName( int month );

    enum EMonth
    { Chaitra = 1, Vaisakha, Jyaishtha, Ashadha, Sravana, Bhadrapada,
      Asvina, Karttika, Margasira, Pausha, Magha, Phalguna };
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //OLDHINDULUNISOLARCALENDAR_HPP

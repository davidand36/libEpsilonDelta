#ifndef HINDULUNISOLARCALENDAR_HPP
#define HINDULUNISOLARCALENDAR_HPP
/*
  HinduLunisolarCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class HinduLunisolarCalendar, representing the Hindu lunisolar calendar.
*/


#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class HinduLunisolarCalendar
{
public:
    static void JulianDayToDLMLY( int julianDay, 
                                  int * pDay, bool * pDayLeap,
                                  int * pMonth, bool * pMonthLeap,
                                  int * pYear );
    static int DLMLYToJulianDay( int day, bool dayLeap,
                                 int month, bool monthLeap, int year );

    static bool Valid( int day, bool dayLeap, int month, bool monthLeap,
                       int year );
    static int MakeValid( int * pDay, bool *pDayLeap,
                          int * pMonth, bool * pMonthLeap,
                          int * pYear, DateFixup::EMethod fixupMethod );
    static int LastDayOfMonth( int month, bool monthLeap, int year );
    static int LostDay( int month, bool monthLeap, int year );
    static int LeapDay( int month, bool monthLeap, int year );
    static bool IsMonthLeap( int month, int year );

    static const std::string & MonthName( int month );

    enum EMonth
    { Chaitra = 1, Vaisakha, Jyaishtha, Ashadha, Sravana, Bhadrapada,
      Asvina, Karttika, Margasira, Pausha, Magha, Phalguna };

    enum { LDNone = 100 };

    enum EVersion
    {
        Modern,
        Old,
        NumVersions
    };

    static void SetVersion( EVersion version );
    static EVersion GetVersion( );

private:
    static EVersion ms_version;
};


//*****************************************************************************


inline
void
HinduLunisolarCalendar::SetVersion( EVersion version )
{
    ms_version = version;
}

//-----------------------------------------------------------------------------

inline
HinduLunisolarCalendar::EVersion
HinduLunisolarCalendar::GetVersion( )
{
    return ms_version;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HINDULUNISOLARCALENDAR_HPP

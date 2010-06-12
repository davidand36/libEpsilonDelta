#ifndef HINDUSOLARCALENDAR_HPP
#define HINDUSOLARCALENDAR_HPP
/*
  HinduSolarCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class HinduSolarCalendar, representing the modern Hindu solar calendar.
*/


#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class HinduSolarCalendar
{
public:
    static void JulianDayToDMY( int julianDay, 
                                int * pDay, int * pMonth, int * pYear );
    static int DMYToJulianDay( int day, int month, int year );
    static int MonthsInYear( int year );
    static int DaysInMonth( int month, int year );
    static const std::string & MonthName( int month, int year );

    enum EMonth
    { Mesha = 1, Vrishabha, Mithuna, Karka, Simha, Kanya,
      Tula, Vrischika, Dhanu, Makara, Kumbha, Mina };

    enum EVersion
    { Modern, Old };

    static void SetVersion( EVersion version );
    static EVersion GetVersion( );

private:
    static int KaliYugaYear( double kaliYugaTime );

    static EVersion ms_version;
    
    friend class HinduLunisolarCalendar;
};


//*****************************************************************************


inline 
int 
HinduSolarCalendar::MonthsInYear( int /*year*/ )
{
    return 12;
}

//=============================================================================

inline
void
HinduSolarCalendar::SetVersion( EVersion version )
{
    ms_version = version;
}

//-----------------------------------------------------------------------------

inline
HinduSolarCalendar::EVersion
HinduSolarCalendar::GetVersion( )
{
    return ms_version;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HINDUSOLARCALENDAR_HPP

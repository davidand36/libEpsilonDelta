#ifndef HINDUSOLARCALENDAR_HPP
#define HINDUSOLARCALENDAR_HPP
/*
  HinduSolarCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  Class HinduSolarCalendar, representing the Hindu solar calendar.
*/


#include "DateFixupMethod.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class HinduSolarCalendar
{
public:
    static void JulianDayToDMY( long julianDay, 
                                int * pDay, int * pMonth, long * pYear );
    static long DMYToJulianDay( int day, int month, long year );
    static int MonthsInYear( long year );
    static int DaysInMonth( int month, long year );
    static const std::string & MonthName( int month, long year );

    enum EMonth
    { Mesha = 1, Vrishabha, Mithuna, Karka, Simha, Kanya,
      Tula, Vrischika, Dhanu, Makara, Kumbha, Mina };

    enum EVersion
    {
        Modern,
        Old,
        NumVersions
    };

    static void SetVersion( EVersion version );
    static EVersion GetVersion( );

private:
    static long KaliYugaYear( double kaliYugaTime );

    static EVersion ms_version;
    
    friend class HinduLunisolarCalendar;
};


//*****************************************************************************


inline 
int 
HinduSolarCalendar::MonthsInYear( long /*year*/ )
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

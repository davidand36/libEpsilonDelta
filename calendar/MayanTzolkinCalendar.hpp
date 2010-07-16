#ifndef MAYANTZOLKINCALENDAR_HPP
#define MAYANTZOLKINCALENDAR_HPP
/*
  MayanTzolkinCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  MayanTzolkinCalendar class, representing the Mayan tzolk'in calendar.

  The tzolk'in is one of the three main calendars of the Mayan
  civilization, which was at its peak from about 250 to 900 C.E.
  It was the sacred calendar, with a 260-day cycle generated by a 13-day
  cycle of numbers and a 20-day cycle of names. Its origins are unclear--
  speculations have ranged from astronomy and numerology to agriculture and
  the length of human pregnancy--and it probably originated with the Olmecs
  before 1200 B.C.E.
*/


#include "Assert.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MayanTzolkinCalendar
{
public:
    static void JulianDayToTzolkin( long julianDay,
                                    int * pTrecena, int * pVeintena );
    static const std::string & VeintenaName( int veintena );

    enum EVeintena
    { Imix = 1, Ik, Akbal, Kan, Chicchan, Cimi, Manik, Lamat,
      Muluc, Oc, Chuen, Eb, Ben, Ix, Men, Cib,
      Caban, Etznab, Cauac, Ahau };

#ifdef DEBUG
    static bool Test( );
#endif
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MAYANTZOLKINCALENDAR_HPP

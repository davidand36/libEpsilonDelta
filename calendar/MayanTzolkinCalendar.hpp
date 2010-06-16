#ifndef MAYANTZOLKINCALENDAR_HPP
#define MAYANTZOLKINCALENDAR_HPP
/*
  MayanTzolkinCalendar.hpp
  Copyright (C) 2007 David M. Anderson

  MayanTzolkinCalendar class, representing the Mayan tzolkin calendar.
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

#ifndef HINDUASTRO_HPP
#define HINDUASTRO_HPP
/*
  HinduAstro.hpp
  Copyright (C) 2007 David M. Anderson

  HinduAstro class: Astronomical routines based on the Surya Siddhanta and the
  basis of the Hindu calendars (solar and lunisolar). These are based on a
  model of the Solar System simliar to Ptolemy's--geocentric, and with
  circular orbits modified by epicycles. The Surya Siddhanta was last updated
  in A.D. 1603.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class HinduAstro
{
public:
    static double SiderealYear( );
    static double SolarLongitude( double kaliYugaTime );
    static int Zodiac( double kaliYugaTime );
    static double SynodicMonth( );
    static double LunarLongitude( double kaliYugaTime );
    static double PriorNewMoon( double kaliYugaTime );
    static int LunarDay( double kaliYugaTime );
    static double Sunrise( int kaliYugaDay );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //HINDUASTRO_HPP

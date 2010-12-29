#ifndef TIMESTANDARDS_HPP
#define TIMESTANDARDS_HPP
/*
  TimeStandards.hpp
  Copyright (C) 2007 David M. Anderson

  Routines relating various time standards:
  TAI (International Atomic Time): Maintained by the Bureau International
      des Poids et Measures (Sevres, France). Data from various high-precision 
      atomic clocks are combined and corrected to maintain a unit of time as
      close as possible to the SI (Systeme International) second, obtained on
      on the rotating geoid. Officially introduced in 1972, but available
      since 1955.
  TT (Terrestrial Time, previously TDT, Terrestrial Dynamical Time): The
      theoretical timescale of apparent geocentric ephemerides. It is also
      based on the SI second on the rotating geoid, but offset from TAI to
      match older ephemeris time standards. (This is sometimes designated more
      specifically as TT(TAI).) Adopted by the IAU (International Astronomcal
      Union) in 1976.
  TDB (Barycentric Dynamical Time): A timescale for equations of motion
      referred to the barycenter of the solar system. It varies from TT only
      in periodic terms reflecting the influence of solar system bodies. It is
      the timescale of JPL (Jet Propulsion Laboratories) ephemerides, among
      others. Adopted by the IAU in 1976.
  TCG (Geocentric Coordinate Time): Represents coordinate time in a
      nonrotating frame at the Earth's center of mass. Synchronized with TT on
      1 January 1977, but with a secular general relativistic correction
      reflecting the effect of Earth's gravity at the geoid on TT. Used for
      analysis of Earth-orbiting satellites. Adopted by the IAU in 1991.
  TCB (Barycentric Coordinate Time): Represents coordinate time in a
      nonrotating frame at the solar system barycenter. Synchronized with TT on
      1 January 1977, but with both periodic and secular general relativistic
      corrections reflecting the effects of solar system bodies on TT. Probably
      the most suitable available time system for solar system ephemerides.
      Adopted by the IAU in 1991.
  UT1 (a.k.a. UT (Universal Time)): Based on the rotation of the Earth. It is
      scaled to correspond to the mean solar day, but is defined in terms of
      GMST (Greenwich Mean Sidereal Time), which is maintained through
      astronomical observations.
  UTC (Coordinated Universal Time): The basis of the world system of civil
      time, available through radio and Internet time signals. UTC differs
      from TAI by an integral number of seconds, the difference being adjusted
      by the occasional addition or removal of "leap seconds" so that UTC
      never differs from UT1 by more than 0.9 s. (The difference
      DUT1 = UT1 - UTC is also broadcast.) Introduced in its present form in
      1972.
  GPST (Global Positioning System Time): The time used on the atomic clocks on
      GPS satellites. Matched UTC in 1980, but does not incorporate leap
      seconds, thus maintaining a constant offset from TAI (19 seconds).
  NOTES:
  1. These functions are named XXX_YYY, where XXX and YYY are the two standards
     and the functions return the difference (XXX - YYY) as a TimeIncrement.
  2. (i) TT - TAI is a constant, but nonnegligible, 32.184 s.
     (ii) TAI - UT1 was 0 on 1 Jan 1958, but is subject to significant
     fluctuations, both periodic and chaotic, in the Earth's rate of rotation,
     as well as a secular slowing. The former has resulted in increases of as
     much as 1 s / year recently. The latter means that the difference is large
     and accelerating in the distant past and future.
     (iii) Because of (i) and (ii), TT - UTC is currently greater than one
     minute.
     (iv) TDB - TT is always less than 0.0017 s.
     TCG - TT was 0 on 1 Jan 1977, and increases at about 0.022 s / year.
     TCB - TT was 0 on 1 Jan 1977, and increases at about 0.489 s / year.
  3. The routines involving UTC are not applicable before 1972, when the
     system was introduced. They are also not valid much into the future, as
     they rely on a table that must be updated whenever a leap second is
     introduced, which is not easily predicted. 
     (See http://www.boulder.nist.gov/timefreq/pubs/bulletin/leapsecond.htm.)
     Over this limited range, however, TAI_UTC() and TT_UTC() are precise.
  4. The routine UT1_TT() uses formulas derived from empirical data and are
     approximate and valid from 390 B.C. to "the present". (The formulas were
     published in the mid-1980's.) For more precise or current values, tables
     such as those in "Astronomical Algorithms" or the "Astronomical Almanac"
     should be consulted.
  5. TDB_UT returns (TDB - UTC) after 2441317.5 (1 Jan 1972), and
     (TDB - UT1) before that.
  6. Generally the differences between these time standards is sufficiently
     small, and varies sufficiently slowly, that it doesn't matter which
     is used for the argument to these functions (for an accuracy of a few
     microseconds). The occasional exception concerns UTC at the moments when
     leap second are introduced; in this instance, UTC is the appropriate
     time system for the argument.
*/


#include "TimeIncrement.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


TimeIncrement TT_TAI( );
TimeIncrement TDB_TT( double julianDay );
TimeIncrement TCG_TT( double julianDay );
TimeIncrement TCB_TDB( double julianDay );
TimeIncrement TT_UT1( double julianDay );
TimeIncrement TAI_UTC( double julianDay );
TimeIncrement TT_UTC( double julianDay );
TimeIncrement TDB_UTC( double julianDay );
TimeIncrement TDB_UT( double julianDay );
TimeIncrement TAI_GPST( );


//*****************************************************************************


inline 
TimeIncrement 
TT_TAI( )
{
    return TimeIncrement( 0, 0, 32.184 );
}

//=============================================================================

inline
TimeIncrement
TAI_GPST( )
{
    return TimeIncrement( 0, 0, 19. );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //TIMESTANDARDS_HPP

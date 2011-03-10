#ifndef OBLIQUITY_HPP
#define OBLIQUITY_HPP
/*
  Obliquity.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for computing the obliquity of the ecliptic.
*/


#include "Angle.hpp"
#include "Nutation.hpp"
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class Nutation;
class JPLEphemeris;

//*****************************************************************************


Angle MeanObliquity( double julianDay );
Angle TrueObliquity( double julianDay, Angle nutObliquity );
Angle TrueObliquity( double julianDay, const Nutation & nutation );
Angle TrueObliquity( Angle meanObliquity, Angle nutObliquity );
Angle TrueObliquity( Angle meanObliquity, const Nutation & nutation );

#ifdef DEBUG
bool TestObliquity( std::tr1::shared_ptr< JPLEphemeris > spEphem );
#endif


//*****************************************************************************


inline
Angle 
TrueObliquity( Angle meanObliquity, Angle nutObliquity )
{
    return  meanObliquity + nutObliquity;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Angle 
TrueObliquity( Angle meanObliquity, const Nutation & nutation )
{
    return  TrueObliquity( meanObliquity, nutation.NutObliquity( ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //OBLIQUITY_HPP

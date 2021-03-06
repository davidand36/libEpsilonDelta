#ifndef PRECESSION_HPP
#define PRECESSION_HPP
/*
  Precession.hpp
  Copyright (C) 2007 David M. Anderson

  Precession class: represents coordinate correction due to lunisolar
  precession (the smooth, long-period motion of the pole and equator,
  regressing the vernal equinox about 50" / year) and planetary precession
  (the motion of the ecliptic due the planets at about 47" / century).
  NOTES:
  1. Matrix() returns the matrix for conversion of equatorial rectangular
     coordinates from the equinox of the epoch to the equinox of date (i.e.
     of the julianDay).
     Matrix( false ) returns the matrix for conversion from the equinox of
     date to the equinox of the epoch.
  2. Reduce() converts equatorial coordinates from the equinox of the epoch
     to the epoch of date. Reduce( , false ) converts from the equinox of date
     to the equinox of the epoch.
  3. Nutation, the short-period motion of the pole, is handled in a separate
     module.
*/


#include "Matrix3.hpp"
#include "Angle.hpp"
#include "Epoch.hpp"
#include "Equatorial.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Precession
{
public:
    Precession( double julianDay, double epoch = J2000 );
    Matrix3D Matrix( bool fromEpoch = true ) const;
    Equatorial Reduce( Equatorial equatorial, bool fromEpoch = true ) const;
    
#ifdef DEBUG
    static bool Test( );
#endif


private:
    Angle m_zeta;
    Angle m_z;
    Angle m_theta;
    double m_cosTheta;
    double m_sinTheta;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PRECESSION_HPP

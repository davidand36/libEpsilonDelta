#ifndef NUTATION_HPP
#define NUTATION_HPP
/*
  Nutation.hpp
  Copyright (C) 2007 David M. Anderson

  Nutation class: represents coordinate correction due the short-period
  (principally 18.6-year) motion of the Earth's rotation axis about the
  mean axis.
  NOTES:
  1. Nutation in longitude is usually written delta-psi; nutation in obliquity
     is written delta-epsilon.
*/


#include "Angle.hpp"
#include "Matrix3.hpp"
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class JPLEphemeris;

//*****************************************************************************


class Nutation
{
public:
    Nutation( );
    Nutation( Angle nutLongitude, Angle nutObliquity );
    void Set( Angle nutLongitude, Angle nutObliquity );
    Angle NutLongitude( ) const;
    Angle NutObliquity( ) const;
    Matrix3D Matrix( Angle meanObliquity, bool inverse = false ) const;

#ifdef DEBUG
    static bool Test( std::tr1::shared_ptr< JPLEphemeris > spEphem );
#endif


private:
    Angle m_nutLongitude;
    Angle m_nutObliquity;
};


//*****************************************************************************

inline
Nutation::Nutation( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Nutation::Nutation( Angle nutLongitude, Angle nutObliquity )
    :   m_nutLongitude( nutLongitude ),
        m_nutObliquity( nutObliquity )
{
}

//-----------------------------------------------------------------------------

inline 
void 
Nutation::Set( Angle nutLongitude, Angle nutObliquity )
{
    m_nutLongitude = nutLongitude;
    m_nutObliquity = nutObliquity;
}

//=============================================================================

inline 
Angle 
Nutation::NutLongitude( ) const
{
    return m_nutLongitude;
}

//-----------------------------------------------------------------------------

inline 
Angle 
Nutation::NutObliquity( ) const
{
    return m_nutObliquity;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //NUTATION_HPP

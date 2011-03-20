#ifndef EPOCH_HPP
#define EPOCH_HPP
/*
  Epoch.hpp
  Copyright (C) 2007 David M. Anderson

  Epoch class: Denotes a particular instant used as a reference, mainly in
  astronomical systems.
  NOTES:
  1. Besselian epochs are based on the tropical year. The length of the
     tropical year on B1900.0 is used here.
     Julian epochs are based on a fixed-length year of 365.25 days. This is
     now the standard system.
  2. The Julian Days of the most commonly used epochs, B1950.0 and J2000.0,
     are provided as constants.
  3. Century2000() returns the number of Julian centuries (36525 days) since
     J2000.0.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Epoch
{
public:
    enum ESystem { Besselian, Julian };

    Epoch( double year, ESystem system );
    double Year( ) const;
    ESystem System( ) const;
    double JulianDay( ) const;

#ifdef DEBUG
    static bool Test( );
#endif


private:
    double m_year;
    ESystem m_system;
};


//-----------------------------------------------------------------------------


double Century2000( double julianDay );


//=============================================================================


//Julian Days for some standard epochs:

extern const double J2000;
extern const double B1950;
extern const double B1875;


//*****************************************************************************


inline 
Epoch::Epoch( double year, ESystem system )
    :   m_year( year ),
        m_system( system )
{
}

//=============================================================================

inline 
double 
Epoch::Year( ) const
{
    return m_year;
}

//-----------------------------------------------------------------------------

inline 
Epoch::ESystem 
Epoch::System( ) const
{
    return m_system;
}

//-----------------------------------------------------------------------------

inline 
double 
Epoch::JulianDay( ) const
{
    //Paul J Heafner, "Fundamental Ephemeris Computations", p. 18.
    if ( m_system == Besselian )
        return  (m_year - 1900.) * 365.242198781731  +  2415020.31352;
    else //m_system == Julian
        return  (m_year - 2000.) * 365.25  +  2451545.;
}


//=============================================================================

inline 
double 
Century2000( double julianDay )
{
    return ((julianDay - J2000) / 36525.);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //EPOCH_HPP

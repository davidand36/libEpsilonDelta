#ifndef ASTROCONST_HPP
#define ASTROCONST_HPP
/*
  AstroConst.hpp
  Copyright (C) 2007 David M. Anderson

  Physical and astronomical constants.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class AstroConst
{
public:
    static double SpeedOfLight( );                      //c (m / s)
    static double GaussianGravitationalConstant( );     //k (AU^3 / S D^2)^.5
    static double GravitationalConstant( );             //G (m^3 / kg s^2)
    static double AstronomicalUnit( );                  //A or AU (m)
    static double LightTimeForAU( );                    //A/c (s)
    static double GeocentricGravitationalConstant( );   //GE (m^3 / s^2)
    static double HeliocentricGravitationalConstant( ); //GS (m^3 / s^2)
    static double SolarMass( );                         //S (kg)
    static double EarthRadius( );                       //a (m) (equatorial)
    static double EarthFlattening( );                   //f (dimensionless)
    static double MoonEarthMassRatio( );                //mu (dimensionless)
};


//*****************************************************************************


inline 
double 
AstroConst::SpeedOfLight( )
{
    return 299792458.;   //Exact (definition of meter)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::GaussianGravitationalConstant( )
{
    return 0.01720209895;   //Exact in IAU (1976) system
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::GravitationalConstant( )
{
//    return 6.672e-11;   //IAU (1976)
    return 6.67428e-11; //CODATA (2006) (+/- 0.00067)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::AstronomicalUnit( )
{
    return 1.49597870e+11;   //IAU (1976)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::LightTimeForAU( )
{
    return 499.004782;  //IAU (1976)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::GeocentricGravitationalConstant( )
{
    return 3.986005e+14; //IAU (1976)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::HeliocentricGravitationalConstant( )
{
    return 1.32712438e+20;   //IAU (1976)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::SolarMass( )
{
    return 1.9891e+30;   //IAU (1976)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::EarthRadius( )
{
    return 6378140.;    //IAU (1976)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::EarthFlattening( )
{
    return 0.00335281;  //IAU (1976)
}

//-----------------------------------------------------------------------------

inline 
double 
AstroConst::MoonEarthMassRatio( )
{
    return 0.01230002;  //IAU (1976)
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ASTROCONST_HPP

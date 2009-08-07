#ifndef JPLEPHEMERIS_HPP
#define JPLEPHEMERIS_HPP
/*
  JPLEphemeris.hpp
  Copyright (C) 2007 David M. Anderson

  JPLEphemeris class: derives planetary and lunar positions from a JPL
  ephemeris data file.
  JPLBarycentricEphemeris class: function object that returns the (Solar
  System) barycentric position of a body.
  JPLGeocentricEphemeris class: function object that returns the geocentric
  position of a body.
  NOTES:
  1. These routines are designed for the JPL Planetary and Lunar Ephemerides
     binary files, such as those on the CD-ROM sold by Willman-Bell, Inc., and
     available at ftp://ssd.jpl.nasa.gov/pub/eph/export/ (this URL seems to
     change from time to time). The current standard ephemerides are:
     DE405, covering JD 2305424.5 (9 Dec 1599) to 2525008.5 (20 Feb 2201), and
     including nutation and physical libration of the Moon.
     DE406, covering JD 626360.5 (23 Feb -3000) to 2816912.5 (6 May 3000),
     with slightly lower interpolation accuracy (still within 25 m for planets
     and 1 m for the Moon), and without nutation or libration.
  2. The time arguments for these ephemerides are TDB (Barycentric Dynamical
     Time). (See TimeStandards.hpp in the time library.)
  3. Two forms of each function are provided. The first simply takes a single
     Julian Day argument, and should be sufficiently accurate for most
     purposes.The second form takes two time arguments. In principle, any two
     values whose sum is the desired time may be used, but for highest
     accuracy, a good approach is for the first argument to be the previous
     midnight or a nearby epoch, allowing the second argument to represent a
     fraction of a day or a few days to high precision.
  4. Positions are returned in kilometers, velocities in km / day. They may be
     converted to AU by dividing by AUinKM(). These are equatorial coordinates.
     Nutation in longitude and obliquity (dPsi and dEpsilon) are returned in
     radians, as are the libration Euler angles.
  5. DE405 and DE406 are based upon the ICRF (International Celestial Reference
     Frame), adopted by the IAU (International Astonomical Union) as of 1998.
     This frame is consistant with the FK5 J2000.0 optical system, but is
     based on VLBI observations of extragalactic compact radio sources. (See
     http://www.iers.org/iers/products/icrf/ .)
  6. (i) These positions are obtained directly from the ephemeris: Sun,
     Mercury, Venus, Earth-Moon barycenter (EMB), Mars, Jupiter, Saturn,
     Uranus, Neptune, and Pluto, with the origin at the solar-system barycenter
     (SSB); and the Moon with the origin at the Earth.
     (ii) The position of the Earth or Moon relative to the EMB is computed
     from the Moon's geocentric position, multiplied by -(1./(1.+EMR)) or
     EMR/(1.+EMR), respectively, where EMR is EarthMoonRatio().
     (iii) The position of the Earth or Moon relative to the SSB is computed
     from the Moon's geocentric position and the (solar-system barycentric)
     EMB position, using GetEarthBarycentric() or GetMoonBarycentric().
     (iv) In other cases, the solary-system barycentric positions of the "body"
     B and origin O are obtained as just described, and the relative position
     is then given by (B - O).
     (v) In some generally uninteresting cases, the position BO of a body
     relative to an origin is computed by obtaining the position OB of
     the origin relative to the body and taking BO = -OB.
     (vi) These remarks are also true for velocities.
  7. The JPLBarycentricEphemeris and JPLGeocentricEphemeris function objects
     provides a simple interface useful in routines such as those in the
     CoordinateReduction module. In light of Note 6, the latter is useful
     primarily for the Moon, and the former for all other bodies.
  8. RegisterEphemeris() adds a JPLEphemeris object to a list, and
     GetEphemeris() selects the first one from the list that covers the
     specified date. So, generally, the more accurate, but smaller-range
     ephemerides should be registered first. (E.g., a DE405 and then a DE406
     ephemeris.
  9. The enum EBody here differs from the SolarSystem::EBody enumeration.
     Functions are provided to make the translations. Note that the latter
     does not include barycenters.
*/


#include "Vector3.hpp"
#include "Angle.hpp"
#include "Nutation.hpp"
#include "SolarSystem.hpp"
#include "SmartPtr.hpp"
#include <string>
#include <cstdio>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class JPLEphemeris
{
public:
    JPLEphemeris( );
    JPLEphemeris( const std::string & fileName, bool storeConstants = false );
    ~JPLEphemeris( );

    void Init( const std::string & fileName, bool storeConstants = false );

    enum EBody { SolarSystemBarycenter, Sun, Mercury, Venus,
                 Earth, Moon, EarthMoonBarycenter, 
                 Mars, Jupiter, Saturn, Uranus, Neptune, Pluto,
                 NumBodies };

    static EBody SolarSystemToJPLBody( SolarSystem::EBody body );
    static SolarSystem::EBody JPLToSolarSystemBody( EBody body );

    bool GetBodyPosition( double julianDay,
                          EBody body, EBody origin,
                          Vector3D * pPosition, Vector3D * pVelocity = 0 );
    bool GetBodyPosition( double julianDay0, double julianDay1,
                          EBody body, EBody origin,
                          Vector3D * pPosition, Vector3D * pVelocity = 0 );
    bool GetNutation( double julianDay,
                      Nutation * pNutation, Nutation * pDerivative = 0 );
    bool GetNutation( double julianDay0, double julianDay1,
                      Nutation * pNutation, Nutation * pDerivative = 0 );
    bool GetLibration( double julianDay,
                       Vector3< Angle > * pComponents,
                       Vector3< Angle > * pVelocity = 0 );
    bool GetLibration( double julianDay0, double julianDay1,
                       Vector3< Angle > * pComponents,
                       Vector3< Angle > * pVelocity = 0 );
    void GetEarthBarycentric( Vector3D * pEarthPos,
                              const Vector3D & moonGeoPos,
                              const Vector3D & embPos ) const;
    void GetEarthBarycentric( Vector3D * pEarthPos,
                              const Vector3D & moonGeoPos,
                              const Vector3D & embPos,
                              Vector3D * pEarthVelocity,
                              const Vector3D & moonGeoVel,
                              const Vector3D & embVel ) const;
    void GetMoonBarycentric( Vector3D * pMoonPos,
                             const Vector3D & moonGeoPos,
                             const Vector3D & embPos ) const;
    void GetMoonBarycentric( Vector3D * pMoonPos,
                             const Vector3D & moonGeoPos,
                             const Vector3D & embPos,
                             Vector3D * pMoonVelocity,
                             const Vector3D & moonGeoVel,
                             const Vector3D & embVel ) const;

    double firstJulianDay( ) const;
    double lastJulianDay( ) const;
    bool NutationAvailable( ) const;
    bool LibrationAvailable( ) const;
    double AUinKM( ) const;     //Astronomical Unit in kilometers
    double EarthMoonRatio( ) const;

    static const int NumTitles = 3;
    std::string Title( int index ) const;
    int Version( ) const;

    struct Constant
    {
        std::string m_name;
        double m_value;
    };
    const std::vector< Constant > & Constants( ) const;

    static void RegisterEphemeris( JPLEphemeris & ephem );
    static JPLEphemeris * GetEphemeris( double jd );

#ifdef DEBUG
    bool Test( const std::string & testFileName, bool verbose = false );
#endif

//.............................................................................

private:
    enum ETarget { Mer, Ven, EMBary, Mar, Jup, Sat, Ura, Nep, Plu,
                   MoonGeo, Sol, Nut, Lib, NumTargets };

    bool OpenBinaryFile( const std::string & fileName );
    bool ReadBinaryFileHeader( bool storeConstants = false );
    bool LoadCoeffBlock( double julianDay0, double julianDay1 );
    bool GetTargetCoefficients( double julianDay0, double julianDay1, 
                                ETarget target );
    double EvalChebyshev( const double * coeffs, int numCoeffs,
                          double timeFrac,
                          double * pDerivative = 0 );
    bool ComputeComponents( double julianDay0, double julianDay1,
                            ETarget target,
                            Vector3D * pComponents, 
                            Vector3D * pDerivatives = 0 );

    struct CoefficientLayout
    {
        int m_offset;
        int m_numCoeffs;
        int m_numComponents;
        int m_numSubIntervals;
    };

    std::FILE * m_file;

    //data from header
    double m_jdStart;
    double m_jdEnd;
    double m_astronomicalUnit;  //km
    double m_earthMoonRatio;
    std::string m_titles[ NumTitles ];
    std::vector< Constant > m_constants;
    int m_version;  //e.g. 405 for the DE405 ephemeris

    double m_blockInterval;
    bool m_wrongEndian;
    CoefficientLayout m_coeffLayouts[ NumTargets ];
    int m_coeffsPerBlock;
    int m_dataOffset;

    //data obtained from subroutines
    shared_array< const double > m_coeffBlocks[2];
    int m_curBlock;
    const double * m_targetCoeffs;
    double m_timeFrac;
    double m_subInterval;
    static const int MaxCoeffs = 18;
    double m_chebyVals[ MaxCoeffs ];
    double m_chebyDerivs[ MaxCoeffs ];
    int m_numChebys;
    int m_numDerivs;
    double m_t2;
};


//*****************************************************************************


class JPLBarycentricEphemeris
{
public:
    JPLBarycentricEphemeris( JPLEphemeris * pEphemeris,
                             JPLEphemeris::EBody body );
    Vector3D operator()( double julianDay );
    Vector3D operator()( double julianDay0, double julianDay1 );
    void operator()( double julianDay,
                     Vector3D * pPosition, Vector3D * pVelocity );
    void operator()( double julianDay0, double julianDay1, 
                     Vector3D * pPosition, Vector3D * pVelocity );

private:
    JPLEphemeris * m_pEphemeris;
    JPLEphemeris::EBody m_body;
};


//*****************************************************************************


class JPLGeocentricEphemeris
{
public:
    JPLGeocentricEphemeris( JPLEphemeris * pEphemeris,
                            JPLEphemeris::EBody body );
    Vector3D operator()( double julianDay );
    Vector3D operator()( double julianDay0, double julianDay1 );
    void operator()( double julianDay,
                     Vector3D * pPosition, Vector3D * pVelocity );
    void operator()( double julianDay0, double julianDay1, 
                     Vector3D * pPosition, Vector3D * pVelocity );

private:
    JPLEphemeris * m_pEphemeris;
    JPLEphemeris::EBody m_body;
};


//*****************************************************************************


inline
double 
JPLEphemeris::AUinKM( ) const
{
    return m_astronomicalUnit;
}

//-----------------------------------------------------------------------------

inline
double 
JPLEphemeris::EarthMoonRatio( ) const
{
    return m_earthMoonRatio;
}


//*****************************************************************************


inline 
JPLBarycentricEphemeris::JPLBarycentricEphemeris( JPLEphemeris * pEphemeris,
                                                  JPLEphemeris::EBody body )
    :   m_pEphemeris( pEphemeris ),
        m_body( body )
{
}


//*****************************************************************************


inline 
JPLGeocentricEphemeris::JPLGeocentricEphemeris( JPLEphemeris * pEphemeris,
                                                JPLEphemeris::EBody body )
    :   m_pEphemeris( pEphemeris ),
        m_body( body )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JPLEPHEMERIS_HPP

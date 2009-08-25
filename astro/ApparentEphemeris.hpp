#ifndef APPARENTEPHEMERIS_HPP
#define APPARENTEPHEMERIS_HPP
/*
  ApparentEphemeris.hpp
  Copyright (C) 2007 David M. Anderson

  Function objects that return the apparent place of the Sun, Moon, and
  planets.
  NOTES:
  1. (i) The BodyBaryFunc and SunBaryFunc template parameters allow for the use
     of either ordinary functions or function objects, such as instances of
     the JPLBarycentricPosition class. They should be of the form
     Point3D func( double julianDay ),
     or (for higher precision)
     Point3D func( double julianDay0, double julianDay1 ),
     and return the mean barycentric place of the body or Sun.
     (ii) The MoonGeoFunc template parameter is the same, except that it
     returns the mean geocentric place of the Moon.
     (iii) The EarthBaryFunc template parameters are similar, but should be
     of the form
     void func( double julianDay, Point3D & pPosition, Vector3D & pVelocity ),
     or
     void func( double julianDay0, double julianDay1,
                Point3D & pPosition, Vector3D & pVelocity ).
*/


#include "CoordinateReduction.hpp"
#include "JPLEphemeris.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename SunBaryFunc, typename EarthBaryFunc >
class SunApparentEphemeris
{
public:
    SunApparentEphemeris( SunBaryFunc sunBaryFunc,
                          EarthBaryFunc earthBaryFunc,
                          const Matrix3D & nutAndPrecMatrix );
    Point3D operator()( double julianDay );
    Point3D operator()( double julianDay0, double julianDay1 );

private:
    //Undefined, to avoid warning:
    SunApparentEphemeris & operator=( const SunApparentEphemeris & );

    SunBaryFunc m_sunBaryFunc;
    EarthBaryFunc m_earthBaryFunc;
    const Matrix3D & m_nutAndPrecMatrix;
};


//*****************************************************************************


template < typename MoonGeoFunc >
class MoonApparentEphemeris
{
public:
    MoonApparentEphemeris( MoonGeoFunc moonGeoFunc,
                           const Matrix3D & nutAndPrecMatrix );
    Point3D operator()( double julianDay );
    Point3D operator()( double julianDay0, double julianDay1 );

private:
    //Undefined, to avoid warning:
    MoonApparentEphemeris & operator=( const MoonApparentEphemeris & );

    MoonGeoFunc m_moonGeoFunc;
    const Matrix3D & m_nutAndPrecMatrix;
};


//*****************************************************************************


template < typename BodyBaryFunc, typename SunBaryFunc,
           typename EarthBaryFunc >
class ApparentEphemeris
{
public:
    ApparentEphemeris( BodyBaryFunc bodyBaryFunc, 
                       SunBaryFunc sunBaryFunc,
                       EarthBaryFunc earthBaryFunc,
                       const Matrix3D & nutAndPrecMatrix );
    Point3D operator()( double julianDay );
    Point3D operator()( double julianDay0, double julianDay1 );

private:
    //Undefined, to avoid warning:
    ApparentEphemeris & operator=( const ApparentEphemeris & );

    BodyBaryFunc m_bodyBaryFunc;
    SunBaryFunc m_sunBaryFunc;
    EarthBaryFunc m_earthBaryFunc;
    const Matrix3D & m_nutAndPrecMatrix;
};


//*****************************************************************************


typedef SunApparentEphemeris< JPLBarycentricEphemeris,
                              JPLBarycentricEphemeris >
        SunApparentEphemerisJPL;
typedef MoonApparentEphemeris< JPLGeocentricEphemeris >
        MoonApparentEphemerisJPL;
typedef ApparentEphemeris< JPLBarycentricEphemeris, JPLBarycentricEphemeris,
                           JPLBarycentricEphemeris >
        ApparentEphemerisJPL;


//*****************************************************************************


#ifdef DEBUG
bool TestApparentEphemeris( JPLEphemeris & de200, JPLEphemeris & de405 );
#endif


//*****************************************************************************
//*****************************************************************************


template < typename SunBaryFunc, typename EarthBaryFunc >
SunApparentEphemeris< SunBaryFunc, EarthBaryFunc >
        ::SunApparentEphemeris( SunBaryFunc sunBaryFunc,
                                EarthBaryFunc earthBaryFunc,
                                const Matrix3D & nutAndPrecMatrix )
    :   m_sunBaryFunc( sunBaryFunc ),
        m_earthBaryFunc( earthBaryFunc ),
        m_nutAndPrecMatrix( nutAndPrecMatrix )
{
}

//=============================================================================

template < typename SunBaryFunc, typename EarthBaryFunc >
Point3D 
SunApparentEphemeris< SunBaryFunc, EarthBaryFunc >
        ::operator()( double julianDay )
{
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
    m_earthBaryFunc( julianDay, &earthBarycentric, &earthBarycentricVelocity );
    return GetSunApparentPlace( julianDay, m_sunBaryFunc,
                                earthBarycentric, earthBarycentricVelocity, 
                                m_nutAndPrecMatrix );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename SunBaryFunc, typename EarthBaryFunc >
Point3D 
SunApparentEphemeris< SunBaryFunc, EarthBaryFunc >
        ::operator()( double julianDay0, double julianDay1 )
{
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
    m_earthBaryFunc( julianDay0, julianDay1,
                     &earthBarycentric, &earthBarycentricVelocity );
    return GetSunApparentPlace( julianDay0, julianDay1, m_sunBaryFunc,
                                earthBarycentric, earthBarycentricVelocity, 
                                m_nutAndPrecMatrix );
}


//*****************************************************************************


template < typename MoonGeoFunc >
MoonApparentEphemeris< MoonGeoFunc >
        ::MoonApparentEphemeris( MoonGeoFunc moonGeoFunc,
                                            const Matrix3D & nutAndPrecMatrix )
    :   m_moonGeoFunc( moonGeoFunc ),
        m_nutAndPrecMatrix( nutAndPrecMatrix )
{
}

//=============================================================================

template < typename MoonGeoFunc >
Point3D 
MoonApparentEphemeris< MoonGeoFunc >
        ::operator()( double julianDay )
{
    return GetMoonApparentPlace( julianDay, m_moonGeoFunc, 
                                 m_nutAndPrecMatrix );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename MoonGeoFunc >
Point3D 
MoonApparentEphemeris< MoonGeoFunc >
        ::operator()( double julianDay0, double julianDay1 )
{
    return GetMoonApparentPlace( julianDay0, julianDay1, m_moonGeoFunc, 
                                 m_nutAndPrecMatrix );
}


//*****************************************************************************


template < typename BodyBaryFunc, typename SunBaryFunc,
           typename EarthBaryFunc >
ApparentEphemeris< BodyBaryFunc, SunBaryFunc, EarthBaryFunc >
        ::ApparentEphemeris( BodyBaryFunc bodyBaryFunc, 
                                      SunBaryFunc sunBaryFunc,
                                      EarthBaryFunc earthBaryFunc,
                                      const Matrix3D & nutAndPrecMatrix )
    :   m_bodyBaryFunc( bodyBaryFunc ),
        m_sunBaryFunc( sunBaryFunc ),
        m_earthBaryFunc( earthBaryFunc ),
        m_nutAndPrecMatrix( nutAndPrecMatrix )
{
}

//=============================================================================

template < typename BodyBaryFunc, typename SunBaryFunc,
           typename EarthBaryFunc >
Point3D 
ApparentEphemeris< BodyBaryFunc, SunBaryFunc, EarthBaryFunc >
        ::operator()( double julianDay )
{
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
    m_earthBaryFunc( julianDay, &earthBarycentric, &earthBarycentricVelocity );
    Point3D sunBarycentric = m_sunBaryFunc( julianDay );
    Point3D earthHeliocentric = Translate( earthBarycentric, sunBarycentric );
    return GetApparentPlace( julianDay,
                             m_bodyBaryFunc, m_sunBaryFunc,
                             earthBarycentric, earthHeliocentric,
                             earthBarycentricVelocity, 
                             m_nutAndPrecMatrix );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename BodyBaryFunc, typename SunBaryFunc,
           typename EarthBaryFunc >
Point3D 
ApparentEphemeris< BodyBaryFunc, SunBaryFunc, EarthBaryFunc >
        ::operator()( double julianDay0, double julianDay1 )
{
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
    m_earthBaryFunc( julianDay0, julianDay1,
                     &earthBarycentric, &earthBarycentricVelocity );
    Point3D sunBarycentric = m_sunBaryFunc( julianDay0, julianDay1 );
    Point3D earthHeliocentric = Translate( earthBarycentric, sunBarycentric );
    return GetApparentPlace( julianDay0, julianDay1, 
                             m_bodyBaryFunc, m_sunBaryFunc,
                             earthBarycentric, earthHeliocentric,
                             earthBarycentricVelocity, 
                             m_nutAndPrecMatrix );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //APPARENTEPHEMERIS_HPP

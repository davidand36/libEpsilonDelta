#ifndef POSITIONREDUCTION_HPP
#define POSITIONREDUCTION_HPP
/*
  CoordinateReduction.hpp
  Copyright (C) 2007 David M. Anderson

  Routines for correcting the position of a celestial body for light-time,
  light deflection, aberration, precession, and nutation.
  NOTES:
  1. Some terminology:
     (i) Mean place is the barycentric position of a body at a given instant,
     referred to the mean equator and equinox of a fixed epoch, such as
     J2000.0. This is the datum typically obtained from a fundamental
     ephemeris such as the JPL ephemerides.
     (ii) Astrometric place is the mean place corrected for light-time, i.e.
     the body's position at a somewhat earlier instant such that light
     originating from the body then would arrive at the geocenter at the
     designated instant. This is sometimes used in differential work on the
     assumption that for sufficiently small fields the other corrections would
     affect all of the objects in the same way, leaving their relative
     positions unaffected. The light-time correction is only applied to
     objects in the solar system; for stars, corrections are made only for
     space motion (proper motion and radial velocity).
     (iii) Virtual place is the astrometric place corrected, additionally, for
     relativistic light deflection and annual aberration. It is essentially the
     apparent place expressed in terms of the fixed reference epoch.
     (iv) Apparent place is the virtual place with coordinates corrected for
     precession and nutation.
     (v) Local place is the same as the virtual place, but corrected for the
     position (and velocity) of the observer relative to the geocenter. It is
     essentially the topocentric place expressed in terms of the fixed eopch.
     For high-precision differential astrometric measurement, local place
     should be used.
     (vi) Topocentric place is the same as apparent place, but corrected for
     the position (and velocity) of the observer relative to the geocenter.
     Equivalently, it is the local place corrected for precession and
     nutation.
  2. Routines are provided separately for the Sun, Moon, planets (and other
     Solar System bodies), and stars (and other distant objects). The routines
     for the Sun are simplified in that they do not need to correct for
     relativistic light time or deflection. (Classical light time is corrected
     for.) The routine for the Moon also does not make these relativistic
     corrections, which should be quite minor, nor for annual aberration, since
     the Moon shares the Earth's velocity around the Solar System barycenter.
     (Hence no distinction is made between the Moon's astrometric and virtual
     place.)
  3. (i) The BodyBaryFunc and SunBaryFunc template parameters allow for the use
     of either ordinary functions or function objects, such as instances of
     the JPLBarycentricPosition class. They should be of the form
     Vector3D func( double julianDay ),
     or (for higher precision)
     Vector3D func( double julianDay0, double julianDay1 ),
     and return the mean barycentric place of the body or Sun.
     (ii) The MoonGeoFunc template parameter is the same, except that it
     returns the mean geocentric place of the Moon.
  4. The optional argument pBodyHeliocentric to GetAstrometricPlace()
     allows the user to retrieve an intermediate result of the computation
     for later use, particularly for CorrectForLightDeflection().
     GetVirtualPlace() takes advantage of this.
  5. The nutAndPrecMatrix argument to CorrectForNutationAndPrecession() and
     GetApparentPlace() is the product nutationMatrix * precessionMatrix.
     Precession is relative to the epoch of the ephemeris; for the DE405 and
     DE406 JPL Ephemerides, this is J2000.0, the default epoch of the
     Precession class. (See Precession.hpp.)
  6. The user may also need to convert between civil time (e.g., UTC) and
     ephemeris time (e.g., TDB).
  7. No correction is made for atmospheric refraction in any of these
     reductions.
*/


#include "Vector3.hpp"
#include "Matrix3.hpp"
#include "AstroConst.hpp"
#include "Epoch.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class JPLEphemeris;

//*****************************************************************************

//Sun:
template < typename SunBaryFunc >
Vector3D GetSunAstrometricPlace( double julianDay,
                                 SunBaryFunc sunBaryFunc,
                                 const Vector3D & earthBarycentric );
template < typename SunBaryFunc >
Vector3D GetSunAstrometricPlace( double julianDay0, double julianDay1,
                                 SunBaryFunc sunBaryFunc,
                                 const Vector3D & earthBarycentric );
template < typename SunBaryFunc >
Vector3D GetSunVirtualPlace( double julianDay,
                             SunBaryFunc sunBaryFunc,
                             const Vector3D & earthBarycentric,
                             const Vector3D & earthBarycentricVelocity );
template < typename SunBaryFunc >
Vector3D GetSunVirtualPlace( double julianDay0, double julianDay1,
                             SunBaryFunc sunBaryFunc,
                             const Vector3D & earthBarycentric,
                             const Vector3D & earthBarycentricVelocity );
template < typename SunBaryFunc >
Vector3D GetSunApparentPlace( double julianDay,
                              SunBaryFunc sunBaryFunc,
                              const Vector3D & earthBarycentric,
                              const Vector3D & earthBarycentricVelocity, 
                              const Matrix3D & nutAndPrecMatrix );
template < typename SunBaryFunc >
Vector3D GetSunApparentPlace( double julianDay0, double julianDay1,
                              SunBaryFunc sunBaryFunc,
                              const Vector3D & earthBarycentric,
                              const Vector3D & earthBarycentricVelocity, 
                              const Matrix3D & nutAndPrecMatrix );

//Moon:
template < typename MoonGeoFunc >
Vector3D GetMoonAstrometricPlace( double julianDay,
                                  MoonGeoFunc moonGeoFunc );
template < typename MoonGeoFunc >
Vector3D GetMoonAstrometricPlace( double julianDay0, double julianDay1,
                                  MoonGeoFunc moonGeoFunc );
template < typename MoonGeoFunc >
Vector3D GetMoonApparentPlace( double julianDay,
                               MoonGeoFunc moonGeoFunc, 
                               const Matrix3D & nutAndPrecMatrix );
template < typename MoonGeoFunc >
Vector3D GetMoonApparentPlace( double julianDay0, double julianDay1,
                               MoonGeoFunc moonGeoFunc, 
                               const Matrix3D & nutAndPrecMatrix );
                                  
//Planets and other bodies:
template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D GetAstrometricPlace( double julianDay,
                              BodyBaryFunc bodyBaryFunc,
                              SunBaryFunc sunBaryFunc,
                              const Vector3D & earthBarycentric,
                              const Vector3D & earthHeliocentric, 
                              Vector3D * pBodyHeliocentric = 0 );
template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D GetAstrometricPlace( double julianDay0, double julianDay1, 
                              BodyBaryFunc bodyBaryFunc,
                              SunBaryFunc sunBaryFunc,
                              const Vector3D & earthBarycentric,
                              const Vector3D & earthHeliocentric, 
                              Vector3D * pBodyHeliocentric = 0 );
template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D GetVirtualPlace( double julianDay,
                          BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                          const Vector3D & earthBarycentric,
                          const Vector3D & earthHeliocentric,
                          const Vector3D & earthBarycentricVelocity );
template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D GetVirtualPlace( double julianDay0, double julianDay1, 
                          BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                          const Vector3D & earthBarycentric,
                          const Vector3D & earthHeliocentric, 
                          const Vector3D & earthBarycentricVelocity );
template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D GetApparentPlace( double julianDay,
                           BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                           const Vector3D & earthBarycentric,
                           const Vector3D & earthHeliocentric,
                           const Vector3D & earthBarycentricVelocity, 
                           const Matrix3D & nutAndPrecMatrix );
template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D GetApparentPlace( double julianDay0, double julianDay1, 
                           BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                           const Vector3D & earthBarycentric,
                           const Vector3D & earthHeliocentric, 
                           const Vector3D & earthBarycentricVelocity, 
                           const Matrix3D & nutAndPrecMatrix );

//Stars:
Vector3D GetAstrometricPlace( double julianDay,
                              const Vector3D & starBarycentric,
                              const Vector3D & starVelocity,
                              const Vector3D & earthBarycentric,
                              double epoch = J2000 );
Vector3D GetVirtualPlace( double julianDay,
                          const Vector3D & starBarycentric,
                          const Vector3D & starVelocity,
                          const Vector3D & earthBarycentric,
                          const Vector3D & earthHeliocentric, 
                          const Vector3D & earthBarycentricVelocity,
                          double epoch = J2000 );
Vector3D GetApparentPlace( double julianDay,
                           const Vector3D & starBarycentric,
                           const Vector3D & starVelocity,
                           const Vector3D & earthBarycentric,
                           const Vector3D & earthHeliocentric, 
                           const Vector3D & earthBarycentricVelocity,
                           const Matrix3D & nutAndPrecMatrix, 
                           double epoch = J2000 );

//Underlying reduction routines: (Correction for light-time is made in the
// GetAstrometricPlace() functions.)
Vector3D CorrectForLightDeflection( const Vector3D & bodyGeocentric,
                                    const Vector3D & bodyHeliocentric,
                                    const Vector3D & earthHeliocentric );
Vector3D CorrectForLightDeflection( const Vector3D & starGeocentric,
                                    const Vector3D & earthHeliocentric );
Vector3D CorrectForAberration( const Vector3D & bodyGeocentric,
                               const Vector3D & earthBarycentricVelocity );
Vector3D CorrectForPrecession( const Vector3D & bodyGeocentric,
                               const Matrix3D & precessionMatrix );
Vector3D CorrectForNutation( const Vector3D & bodyGeocentric,
                             const Matrix3D & nutationMatrix );
Vector3D CorrectForNutationAndPrecession( const Vector3D & bodyGeocentric,
                                          const Matrix3D & nutAndPrecMatrix );

#ifdef DEBUG
bool TestCoordinateReduction( JPLEphemeris & de200, JPLEphemeris & de405 );
#endif


//*****************************************************************************


template < typename SunBaryFunc >
Vector3D GetSunAstrometricPlace( double julianDay,
                                 SunBaryFunc sunBaryFunc,
                                 const Vector3D & earthBarycentric )
{
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    const double epsilon = 1.e-8;
    Vector3D sunBarycentric = sunBaryFunc( julianDay );
    Vector3D sunGeocentric = sunBarycentric - earthBarycentric;
    double lightTime = sunGeocentric.Length() / c;
    double oldLightTime;
    do
    {
        oldLightTime = lightTime;
        sunBarycentric = sunBaryFunc( julianDay - lightTime );
        sunGeocentric = sunBarycentric - earthBarycentric;
        lightTime = sunGeocentric.Length() / c;
    } while ( fabs( lightTime - oldLightTime ) > epsilon );
    return sunGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename SunBaryFunc >
Vector3D GetSunAstrometricPlace( double julianDay0, double julianDay1,
                                 SunBaryFunc sunBaryFunc,
                                 const Vector3D & earthBarycentric )
{
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    const double epsilon = 1.e-8;
    Vector3D sunBarycentric = sunBaryFunc( julianDay0, julianDay1 );
    Vector3D sunGeocentric = sunBarycentric - earthBarycentric;
    double lightTime = sunGeocentric.Length() / c;
    double oldLightTime;
    do
    {
        oldLightTime = lightTime;
        sunBarycentric = sunBaryFunc( julianDay0, julianDay1 - lightTime );
        sunGeocentric = sunBarycentric - earthBarycentric;
        lightTime = sunGeocentric.Length() / c;
    } while ( fabs( lightTime - oldLightTime ) > epsilon );
    return sunGeocentric;
}

//-----------------------------------------------------------------------------

template < typename SunBaryFunc >
Vector3D GetSunVirtualPlace( double julianDay,
                             SunBaryFunc sunBaryFunc,
                             const Vector3D & earthBarycentric,
                             const Vector3D & earthBarycentricVelocity )
{
    Vector3D sunGeocentric = GetSunAstrometricPlace( julianDay,
                                                     sunBaryFunc,
                                                     earthBarycentric );
    sunGeocentric = CorrectForAberration( sunGeocentric,
                                          earthBarycentricVelocity );
    return sunGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename SunBaryFunc >
Vector3D GetSunVirtualPlace( double julianDay0, double julianDay1,
                             SunBaryFunc sunBaryFunc,
                             const Vector3D & earthBarycentric,
                             const Vector3D & earthBarycentricVelocity )
{
    Vector3D sunGeocentric = GetSunAstrometricPlace( julianDay0, julianDay1, 
                                                     sunBaryFunc,
                                                     earthBarycentric );
    sunGeocentric = CorrectForAberration( sunGeocentric,
                                          earthBarycentricVelocity );
    return sunGeocentric;
}

//-----------------------------------------------------------------------------

template < typename SunBaryFunc >
Vector3D GetSunApparentPlace( double julianDay,
                              SunBaryFunc sunBaryFunc,
                              const Vector3D & earthBarycentric,
                              const Vector3D & earthBarycentricVelocity, 
                              const Matrix3D & nutAndPrecMatrix )
{
    Vector3D sunGeocentric = GetSunVirtualPlace( julianDay,
                                                 sunBaryFunc,
                                                 earthBarycentric,
                                                 earthBarycentricVelocity );
    sunGeocentric = CorrectForNutationAndPrecession( sunGeocentric,
                                                     nutAndPrecMatrix );
    return sunGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename SunBaryFunc >
Vector3D GetSunApparentPlace( double julianDay0, double julianDay1,
                              SunBaryFunc sunBaryFunc,
                              const Vector3D & earthBarycentric,
                              const Vector3D & earthBarycentricVelocity, 
                              const Matrix3D & nutAndPrecMatrix )
{
    Vector3D sunGeocentric = GetSunVirtualPlace( julianDay0, julianDay1, 
                                                 sunBaryFunc,
                                                 earthBarycentric,
                                                 earthBarycentricVelocity );
    sunGeocentric = CorrectForNutationAndPrecession( sunGeocentric,
                                                     nutAndPrecMatrix );
    return sunGeocentric;
}

//=============================================================================

template < typename MoonGeoFunc >
Vector3D GetMoonAstrometricPlace( double julianDay,
                                  MoonGeoFunc moonGeoFunc )
{
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    const double epsilon = 1.e-8;
    Vector3D moonGeocentric = moonGeoFunc( julianDay );
    double lightTime = moonGeocentric.Length() / c;
    double oldLightTime;
    do
    {
        oldLightTime = lightTime;
        moonGeocentric = moonGeoFunc( julianDay - lightTime );
        lightTime = moonGeocentric.Length() / c;
    } while ( fabs( lightTime - oldLightTime ) > epsilon );
    return moonGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename MoonGeoFunc >
Vector3D GetMoonAstrometricPlace( double julianDay0, double julianDay1,
                                  MoonGeoFunc moonGeoFunc )
{
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    const double epsilon = 1.e-8;
    Vector3D moonGeocentric = moonGeoFunc( julianDay0, julianDay1 );
    double lightTime = moonGeocentric.Length() / c;
    double oldLightTime;
    do
    {
        oldLightTime = lightTime;
        moonGeocentric = moonGeoFunc( julianDay0, julianDay1 - lightTime );
        lightTime = moonGeocentric.Length() / c;
    } while ( fabs( lightTime - oldLightTime ) > epsilon );
    return moonGeocentric;
}

//-----------------------------------------------------------------------------

template < typename MoonGeoFunc >
Vector3D GetMoonApparentPlace( double julianDay,
                               MoonGeoFunc moonGeoFunc, 
                               const Matrix3D & nutAndPrecMatrix )
{
    Vector3D moonGeocentric = GetMoonAstrometricPlace( julianDay,
                                                       moonGeoFunc );
    moonGeocentric = CorrectForNutationAndPrecession( moonGeocentric,
                                                      nutAndPrecMatrix );
    return moonGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename MoonGeoFunc >
Vector3D GetMoonApparentPlace( double julianDay0, double julianDay1,
                               MoonGeoFunc moonGeoFunc, 
                               const Matrix3D & nutAndPrecMatrix )
{
    Vector3D moonGeocentric = GetMoonAstrometricPlace( julianDay0, julianDay1,
                                                       moonGeoFunc );
    moonGeocentric = CorrectForNutationAndPrecession( moonGeocentric,
                                                      nutAndPrecMatrix );
    return moonGeocentric;
}

//=============================================================================

template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D 
GetAstrometricPlace( double julianDay,
                     BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                     const Vector3D & earthBarycentric,
                     const Vector3D & earthHeliocentric, 
                     Vector3D * pBodyHeliocentric )
{
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    static const double m
            = 2. * AstroConst::HeliocentricGravitationalConstant() / (c * c);
    const double e = earthHeliocentric.Length();
    const double epsilon = 1.e-8;
    Vector3D bodyBarycentric = bodyBaryFunc( julianDay );
    Vector3D bodyGeocentric = bodyBarycentric - earthBarycentric;
    Vector3D bodyHeliocentric;
    double lightTime = bodyGeocentric.Length() / c;
    double oldLightTime;
    do
    {
        oldLightTime = lightTime;
        bodyBarycentric = bodyBaryFunc( julianDay - lightTime );
        bodyGeocentric = bodyBarycentric - earthBarycentric;
        bodyHeliocentric = bodyBarycentric
                - sunBaryFunc( julianDay - lightTime );
        double u = bodyGeocentric.Length();
        double q = bodyHeliocentric.Length();
        double lightDist = u;
        if ( q != 0. )
            lightDist += m * log( (e + u + q) / (e - u + q) );
        lightTime = lightDist / c;
    } while ( fabs( lightTime - oldLightTime ) > epsilon );
    if ( pBodyHeliocentric )
        *pBodyHeliocentric = bodyHeliocentric;
    return bodyGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D 
GetAstrometricPlace( double julianDay0, double julianDay1, 
                     BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                     const Vector3D & earthBarycentric,
                     const Vector3D & earthHeliocentric, 
                     Vector3D * pBodyHeliocentric )
{
    static const double c = AstroConst::SpeedOfLight()
            * (86400. / 1000.);  //km / day
    static const double m
            = 2. * AstroConst::HeliocentricGravitationalConstant() / (c * c);
    const double e = earthHeliocentric.Length();
    const double epsilon = 1.e-8;
    Vector3D bodyBarycentric = bodyBaryFunc( julianDay0, julianDay1 );
    Vector3D bodyGeocentric = bodyBarycentric - earthBarycentric;
    Vector3D bodyHeliocentric;
    double lightTime = bodyGeocentric.Length() / c;
    double oldLightTime;
    do
    {
        oldLightTime = lightTime;
        bodyBarycentric = bodyBaryFunc( julianDay0, julianDay1 - lightTime );
        bodyGeocentric = bodyBarycentric - earthBarycentric;
        bodyHeliocentric = bodyBarycentric
                - sunBaryFunc( julianDay0, julianDay1 - lightTime );
        double u = bodyGeocentric.Length();
        double q = bodyHeliocentric.Length();
        double lightDist = u;
        if ( q != 0. )
            lightDist += m * log( (e + u + q) / (e - u + q) );
        lightTime = lightDist / c;
    } while ( fabs( lightTime - oldLightTime ) > epsilon );
    if ( pBodyHeliocentric )
        *pBodyHeliocentric = bodyHeliocentric;
    return bodyGeocentric;
}

//-----------------------------------------------------------------------------

template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D 
GetVirtualPlace( double julianDay,
                 BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                 const Vector3D & earthBarycentric,
                 const Vector3D & earthHeliocentric,
                 const Vector3D & earthBarycentricVelocity )
{
    Vector3D bodyHeliocentric;
    Vector3D bodyGeocentric = GetAstrometricPlace( julianDay,
                                                   bodyBaryFunc, sunBaryFunc,
                                                   earthBarycentric,
                                                   earthHeliocentric,
                                                   &bodyHeliocentric );
    bodyGeocentric = CorrectForLightDeflection( bodyGeocentric,
                                                bodyHeliocentric,
                                                earthHeliocentric );
    bodyGeocentric = CorrectForAberration( bodyGeocentric,
                                           earthBarycentricVelocity );
    return bodyGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D 
GetVirtualPlace( double julianDay0, double julianDay1, 
                 BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                 const Vector3D & earthBarycentric,
                 const Vector3D & earthHeliocentric, 
                 const Vector3D & earthBarycentricVelocity )
{
    Vector3D bodyHeliocentric;
    Vector3D bodyGeocentric = GetAstrometricPlace( julianDay0, julianDay1, 
                                                   bodyBaryFunc, sunBaryFunc,
                                                   earthBarycentric,
                                                   earthHeliocentric,
                                                   &bodyHeliocentric );
    bodyGeocentric = CorrectForLightDeflection( bodyGeocentric,
                                                bodyHeliocentric,
                                                earthHeliocentric );
    bodyGeocentric = CorrectForAberration( bodyGeocentric,
                                           earthBarycentricVelocity );
    return bodyGeocentric;
}
                     
//-----------------------------------------------------------------------------

template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D 
GetApparentPlace( double julianDay,
                  BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                  const Vector3D & earthBarycentric,
                  const Vector3D & earthHeliocentric,
                  const Vector3D & earthBarycentricVelocity, 
                  const Matrix3D & nutAndPrecMatrix )
{
    Vector3D bodyGeocentric = GetVirtualPlace( julianDay,
                                               bodyBaryFunc, sunBaryFunc, 
                                               earthBarycentric,
                                               earthHeliocentric,
                                               earthBarycentricVelocity );
    bodyGeocentric = CorrectForNutationAndPrecession( bodyGeocentric,
                                                      nutAndPrecMatrix );
    return bodyGeocentric;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename BodyBaryFunc, typename SunBaryFunc >
Vector3D 
GetApparentPlace( double julianDay0, double julianDay1, 
                  BodyBaryFunc bodyBaryFunc, SunBaryFunc sunBaryFunc,
                  const Vector3D & earthBarycentric,
                  const Vector3D & earthHeliocentric, 
                  const Vector3D & earthBarycentricVelocity, 
                  const Matrix3D & nutAndPrecMatrix )
{
    Vector3D bodyGeocentric = GetVirtualPlace( julianDay0, julianDay1, 
                                               bodyBaryFunc, sunBaryFunc, 
                                               earthBarycentric,
                                               earthHeliocentric,
                                               earthBarycentricVelocity );
    bodyGeocentric = CorrectForNutationAndPrecession( bodyGeocentric,
                                                      nutAndPrecMatrix );
    return bodyGeocentric;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //POSITIONREDUCTION_HPP

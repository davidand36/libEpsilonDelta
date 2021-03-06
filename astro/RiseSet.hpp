#ifndef RISESET_HPP
#define RISESET_HPP
/*
  RiseSet.hpp
  Copyright (C) 2007 David M. Anderson

  Routines to determine the next time (as Julian day) a body rises or sets,
  or reaches any specific altitude, such as twilight.
  NOTES:
  1. The BodyEquatorialFunc allows for the use of a function or a function
     object. It should be of the form
     Equatorial bodyEquatFunc( double julianDay );
  2. The default accuracies are generally much finer than is realistically
     needed, particularly for rise and set times, where variations in
     atmospheric refraction and the topography of the horizon have large
     effects, and twilight, since the darkening of the night sky is a gradual
     process. There should be no harm, however, since the algorithms typically
     converge quite rapidly.
  3. Times are ephemeris time, i.e. TDB.
*/


#include "Angle.hpp"
#include "AngleHMS.hpp"
#include "AngleDMS.hpp"
#include "GeodeticLocation.hpp"
#include "SolarSystem.hpp"
#include "SiderealTime.hpp"
#include "Equatorial.hpp"
#include "ConvergenceException.hpp"
#include "Logger.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace RiseSet
{                                                           //namespace RiseSet

//*****************************************************************************


enum EEvent
    { Rise, Set, Transit };
enum ETwilight
    { Civil, Nautical, Astronomical };
enum EBodyType
    { Sun, Moon, Planet, Star };
enum EStatus
    { OK, AlwaysUp, AlwaysDown };
struct Result
{
    EStatus m_status;
    double  m_julianDay;
};


template < typename BodyEquatorialFunc >
Result FindNext( double julianDay, EEvent event, Angle targetAltitude,
                 BodyEquatorialFunc bodyEquatFunc, EBodyType bodyType,
                 const GeodeticLocation & location, double accuracySecs );

Result FindNext( double julianDay, SolarSystem::EBody body,
                 EEvent event, Angle targetAltitude,
                 const GeodeticLocation & location, double accuracySecs = 1.0 );
Result FindNext( double julianDay, SolarSystem::EBody body, EEvent event,
                 const GeodeticLocation & location, double accuracySecs = 1.0 );
Result FindNext( double julianDay, const Equatorial & fixedPos, EEvent event,
                 const GeodeticLocation & location, double accuracySecs = 1.0 );
Result FindNextTwilight( double julianDay, EEvent event, ETwilight twilight,
                         const GeodeticLocation & location,
                         double accuracySecs = 30.0 );

Logger & Log( );

#ifdef DEBUG
bool Test( );
#endif


//*****************************************************************************
//*****************************************************************************


template < typename BodyEquatorialFunc >
Result 
FindNext( double julianDay, EEvent event, Angle targetAltitude,
          BodyEquatorialFunc bodyEquatFunc, EBodyType bodyType,
          const GeodeticLocation & location, double accuracySecs )
{
    Result result = { OK, 0. };
    double accuracyDays = accuracySecs / (24. * 60. * 60.);
    double sinTargetAlt = targetAltitude.Sin( );
    double sinLat = location.Latitude().Sin( );
    double cosLat = location.Latitude().Cos( );
    double jd = julianDay;
    Angle gmSidereal = GreenwichMeanSiderealTime( jd );
    Angle localSidereal = gmSidereal + location.Longitude();
    bool firstStep = true;
    bool pushedBack = false;
    double correction = 0.;
    Angle oldRA = 0.;
    int counter = 0;
    Angle targetHourAngle = 0.;
    
    do
    {
        Equatorial bodyEquatPos = bodyEquatFunc( jd );
        Angle bodyRA = bodyEquatPos.RightAscension();
        Angle bodyDec = bodyEquatPos.Declination();
        localSidereal += Angle( correction * 1.0027379, Angle::Cycle );
        Angle bodyHourAngle = localSidereal - bodyRA;
        if ( event != Transit )
        {
            double cosTargetHourAngle
                    = ( sinTargetAlt  -  sinLat * bodyDec.Sin( ) )
                    / ( cosLat * bodyDec.Cos( ) );
            if ( cosTargetHourAngle > 1. )
            {
                result.m_status = AlwaysDown;
                return result;
            }
            else if ( cosTargetHourAngle < -1. )
            {
                result.m_status = AlwaysUp;
                return result;
            }
            targetHourAngle = ArcCos( cosTargetHourAngle );
            if ( event == Rise )
                targetHourAngle = - targetHourAngle;
        }
        double haVariation = 1.;
        if ( firstStep )
        {
            firstStep = false;
            switch ( bodyType )
            {
            case Sun:
                haVariation = 1.;
                break;
            case Moon:
                haVariation = 1.0027379 - 0.0366;
                break;
            case Planet:
            case Star:
                haVariation = 1.0027379;
                break;
            }
        }
        else
        {
            switch ( bodyType )
            {
            case Sun:
                haVariation = 1.;
                break;
            case Star:
                haVariation = 1.0027379;
                break;
            case Moon:
            case Planet:
            {
                double dRAdT
                        = ( bodyRA - oldRA ).Cycles() / correction;
                haVariation = 1.0027379 - dRAdT;
                break;
            }
            }
        }
        correction
                = ( targetHourAngle - bodyHourAngle ).Cycles() / haVariation;
        jd += correction;
        double invVar = 1. / haVariation;
        while ( jd < julianDay )
        {
            jd += invVar;
            correction += invVar;
        }
        if ( ! pushedBack )
        {
            pushedBack = true;
            while ( jd > (julianDay + invVar) )
            {
                jd -= invVar;
                correction -= invVar;
            }
        }
        else
        {
            while ( jd > (julianDay + 1.1) )
            {
                jd -= invVar;
                correction -= invVar;
            }
        }
        oldRA = bodyRA;

        if ( ++counter > 1000 )
            throw ConvergenceException( "RiseSet::FindNext() never converged" );
    } while ( fabs( correction ) > accuracyDays );
    Log().Log( Logger::Debug, "Converged after %d iterations (accuracy=%fs)",
               counter, accuracySecs );

    result.m_julianDay = jd;
    return  result;
}


//*****************************************************************************

}                                                           //namespace RiseSet

}                                                      //namespace EpsilonDelta

#endif //RISESET_HPP

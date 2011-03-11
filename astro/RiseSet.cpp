/*
  RiseSet.cpp
  Copyright (C) 2007 David M. Anderson

  Routines to determine the next time (as Julian day) a body rises or sets,
  or reaches any specific altitude, such as twilight.
*/


#include "RiseSet.hpp"
#include "JPLEphemeris.hpp"
#include "AstroPhenomena.hpp"
#include "AngleDMS.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#include "DateTime.hpp"
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

namespace
{                                                                   //namespace

Logger s_log( "RiseSet" );

}                                                                   //namespace


//*****************************************************************************


class BodyEquatorialPos
{
public:
    BodyEquatorialPos( SolarSystem::EBody body,
                       shared_ptr< JPLEphemeris > spEphemeris,
                       const Matrix3D & nutAndPrecMatrix );

    Equatorial operator()( double julianDay );

private:
    //Undefined, to avoid warning:
    BodyEquatorialPos & operator=( const BodyEquatorialPos & );

    SolarSystem::EBody          m_body;
    shared_ptr< JPLEphemeris >  m_spEphemeris;
    const Matrix3D &            m_nutAndPrecMatrix;
};


//=============================================================================


BodyEquatorialPos::BodyEquatorialPos( SolarSystem::EBody body,
                                      shared_ptr< JPLEphemeris > spEphemeris,
                                      const Matrix3D & nutAndPrecMatrix )
    :   m_body( body ),
        m_spEphemeris( spEphemeris ),
        m_nutAndPrecMatrix( nutAndPrecMatrix )
{
    Assert( body != SolarSystem::Earth );
    Assert( spEphemeris );
}

//=============================================================================

Equatorial
BodyEquatorialPos::operator()( double julianDay )
{
    switch ( m_body )
    {
    case SolarSystem::Sun:
    {
        Point3D earthBarycentric;
        Vector3D earthBarycentricVelocity;
#ifdef DEBUG
        bool earthRslt =
#endif
                GetEarthBarycentric( julianDay, &earthBarycentric,
                                     &earthBarycentricVelocity, m_spEphemeris );
        Assert( earthRslt );
        return  SolarEquatorialPosition( julianDay, earthBarycentric,
                                         earthBarycentricVelocity,
                                         m_nutAndPrecMatrix, m_spEphemeris );
    }
    case SolarSystem::Moon:
    {
        return  LunarEquatorialPosition( julianDay, m_nutAndPrecMatrix,
                                         m_spEphemeris );
    }
    default:
    {
        Point3D earthBarycentric;
        Vector3D earthBarycentricVelocity;
#ifdef DEBUG
        bool earthRslt =
#endif
                GetEarthBarycentric( julianDay, &earthBarycentric,
                                     &earthBarycentricVelocity, m_spEphemeris );
        Assert( earthRslt );
        Point3D sunBarycentric;
#ifdef DEBUG
        bool sunRslt =
#endif
                m_spEphemeris->GetBodyPosition( julianDay, JPLEphemeris::Sun,
                                           JPLEphemeris::SolarSystemBarycenter,
                                           &sunBarycentric );
        Assert( sunRslt );
        Point3D earthHeliocentric
                = Translate( earthBarycentric, sunBarycentric );
        return  PlanetEquatorialPosition( julianDay, m_body,
                                          earthBarycentric,
                                          earthHeliocentric,
                                          earthBarycentricVelocity,
                                          m_nutAndPrecMatrix, m_spEphemeris );
    }
    }
}


//*****************************************************************************


class FixedEquatorialPos
{
public:
    FixedEquatorialPos( const Equatorial & pos );

    Equatorial operator()( double julianDay );

private:
    //Undefined, to avoid warning:
    FixedEquatorialPos & operator=( const FixedEquatorialPos & );

    const Equatorial &  m_pos;
};


//=============================================================================


inline
FixedEquatorialPos::FixedEquatorialPos( const Equatorial & pos )
    :   m_pos( pos )
{
}

//=============================================================================

inline 
Equatorial
FixedEquatorialPos::operator()( double /*julianDay*/ )
{
    return  m_pos;
}


//*****************************************************************************


RiseSet::Result 
RiseSet::FindNext( double julianDay, SolarSystem::EBody body,
                   EEvent event, Angle targetAltitude,
                   const GeodeticLocation & location, double accuracySecs )
{
    s_log( Logger::Debug, "FindNext JD=%11.2f body=%d event=%d alt=%4.2f",
           julianDay, body, event, targetAltitude.Degrees() );
    EBodyType bodyType;
    switch ( body )
    {
    case SolarSystem::Sun:
        bodyType = Sun;
        break;
    case SolarSystem::Moon:
        bodyType = Moon;
        break;
    default:
        bodyType = Planet;
        break;
    }
    shared_ptr< JPLEphemeris > spEphemeris
            = JPLEphemeris::GetEphemeris( julianDay );
    Assert( spEphemeris );
    Matrix3D nutAndPrecMatrix;
#ifdef DEBUG
    bool nutPrecRslt =
#endif
            GetNutPrecAndObliquity( julianDay, &nutAndPrecMatrix,
                                    0, spEphemeris );
    Assert( nutPrecRslt );
    BodyEquatorialPos bodyEquatorialPos( body, spEphemeris, nutAndPrecMatrix );
    return  FindNext( julianDay, event, targetAltitude, bodyEquatorialPos,
                      bodyType, location, accuracySecs );
}

//=============================================================================

RiseSet::Result 
RiseSet::FindNext( double julianDay, SolarSystem::EBody body, EEvent event,
                   const GeodeticLocation & location, double accuracySecs )
{
    Angle targetAltitude( 0. );
    switch ( body )
    {
    case SolarSystem::Sun:
        targetAltitude.Set( AngleDMS( 0, -50 ) );
        break;
    case SolarSystem::Moon:
        //Approximate, doesn't allow for variations in lunar parallax:
        targetAltitude.Set( 0.125, Angle::Degree );
        break;
    default:
        targetAltitude.Set( AngleDMS( 0, -34 ) );
        break;
    }
    if ( location.Height() > 0. )
        targetAltitude -= Angle( AngleDMS( 0.0353
                                          * std::sqrt( location.Height() ) ) );
    return FindNext( julianDay, body, event, targetAltitude, location,
                     accuracySecs );
}

//=============================================================================

RiseSet::Result 
RiseSet::FindNext( double julianDay, const Equatorial & fixedPos, EEvent event,
                   const GeodeticLocation & location, double accuracySecs )
{
    Angle targetAltitude( AngleDMS( 0, -34 ) );
    FixedEquatorialPos fixedEquatorialPos( fixedPos );
    return  FindNext( julianDay, event, targetAltitude, fixedEquatorialPos,
                      Star, location, accuracySecs );
}

//=============================================================================

RiseSet::Result 
RiseSet::FindNextTwilight( double julianDay, EEvent event, ETwilight twilight,
                           const GeodeticLocation & location,
                           double accuracySecs )
{
    Angle targetAltitude( 0. );
    switch ( twilight )
    {
    case Civil:
        targetAltitude.Set( -6., Angle::Degree );
        break;
    case Nautical:
        targetAltitude.Set( -12., Angle::Degree );
        break;
    case Astronomical:
        targetAltitude.Set( -18., Angle::Degree );
        break;
    }
    return FindNext( julianDay, SolarSystem::Sun, event, targetAltitude,
                     location, accuracySecs );
}

//=============================================================================

Logger &
RiseSet::Log( )
{
    return s_log;
}

//=============================================================================


#ifdef DEBUG

bool 
RiseSet::Test( )
{
    bool ok = true;
    cout << "Testing RiseSet" << endl;

    double mar20_1988 = DateTime( 20, March, 1988, 0, 0 ).JulianDay( );
    GeodeticLocation boston( Angle( -71.0833, Angle::Degree ),
                             Angle( 42.3333, Angle::Degree ) );
    cout << "FindNext( mar20_1988, SolarSystem::Venus, Rise, boston )" << endl;
    Result result = FindNext( mar20_1988, SolarSystem::Venus, Rise, boston );
    TESTCHECK( result.m_status, OK, &ok );
    TESTCHECKFE( result.m_julianDay,
                 DateTime( 20, March, 1988, 12, 25 ).JulianDay( ), &ok,
                 2e-10 );
    cout << "FindNext( mar20_1988, SolarSystem::Venus, Set, boston )" << endl;
    result = FindNext( mar20_1988, SolarSystem::Venus, Set, boston );
    TESTCHECK( result.m_status, OK, &ok );
    TESTCHECKFE( result.m_julianDay,
                 DateTime( 20, March, 1988, 2, 55 ).JulianDay( ), &ok,
                 2e-10 );
    cout << "FindNext( mar20_1988, SolarSystem::Venus, Transit, boston )"
         << endl;
    result = FindNext( mar20_1988, SolarSystem::Venus, Transit, boston );
    TESTCHECK( result.m_status, OK, &ok );
    TESTCHECKFE( result.m_julianDay,
                 DateTime( 20, March, 1988, 19, 41 ).JulianDay( ), &ok,
                 2e-10 );
    double jan13_2006 = DateTime( 13, January, 2006, 0, 0 ).JulianDay( );
    GeodeticLocation istanbul( Angle( 30., Angle::Degree ),
                               Angle( 40., Angle::Degree ) );
    cout << "FindNext( jan13_2006, SolarSystem::Sun, Rise, istanbul )" << endl;
    result = FindNext( jan13_2006, SolarSystem::Sun, Rise, istanbul );
    TESTCHECK( result.m_status, OK, &ok );
    TESTCHECKFE( result.m_julianDay,
                 DateTime( 13, January, 2006, 5, 21 ).JulianDay( ), &ok,
                 2e-10 );
    cout << "FindNext( jan13_2006, SolarSystem::Sun, Set, istanbul )" << endl;
    result = FindNext( jan13_2006, SolarSystem::Sun, Set, istanbul );
    TESTCHECK( result.m_status, OK, &ok );
    TESTCHECKFE( result.m_julianDay,
                 DateTime( 13, January, 2006, 14, 57 ).JulianDay( ), &ok,
                 2e-10 );
    double aug12_2006 = DateTime( 12, August, 2006, 0, 0 ).JulianDay( );
    GeodeticLocation greenwich( Angle( 0. ),
                                Angle( 50., Angle::Degree ) );
    cout << "FindNext( aug12_2006, SolarSystem::Moon, Rise, greenwich )" << endl;
    result = FindNext( aug12_2006, SolarSystem::Moon, Rise, greenwich );
    TESTCHECK( result.m_status, OK, &ok );
    TESTCHECKFE( result.m_julianDay,
                 DateTime( 12, August, 2006, 20, 43 ).JulianDay( ), &ok,
                 2e-10 );
    cout << "FindNext( aug12_2006, SolarSystem::Moon, Set, greenwich )" << endl;
    result = FindNext( aug12_2006, SolarSystem::Moon, Set, greenwich );
    TESTCHECK( result.m_status, OK, &ok );
    TESTCHECKFE( result.m_julianDay,
                 DateTime( 12, August, 2006, 8, 38 ).JulianDay( ), &ok,
                 2e-10 );

    if ( ok )
        cout << "RiseSet PASSED." << endl << endl;
    else
        cout << "RiseSet FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

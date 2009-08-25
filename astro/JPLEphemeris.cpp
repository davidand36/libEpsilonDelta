/*
  JPLEphemeris.cpp
  Copyright (C) 2007 David M. Anderson

  JPLEphemeris class: derives lunar and planetary positions from a JPL
  ephemeris data file.
  NOTES:
  1. Certain data and computations are retained (cached) to allow some
     performance optimization.
     First, the last two blocks of Chebyshev coefficients are cached. This
     means that for multiple positions at the same epoch, or within 32 days
     (for DE405) or 64 days (for DE406), disk reads will be minimized.
     Second, the evaluation of Chebyshev polynomials for the latest timeFrac
     are retained. However, the timeFrac depends on both the Julian Day and
     the number of sub-intervals for the body. To take advantage of this, it
     may be worthwhile to obtain the positions at a given epoch in the
     following order: EMB, Sun, Venus, Mars, Jupiter, Saturn, Uranus, Neptune,
     Pluto, Moon, Mercury, nutation, and libration, where the origin is the SSB
     except for the Moon (origin is Earth). This optimization is fairly
     unimportant, especially since it is inapplicable when computing
     apparent positions, for which positions at different times must be
     calculated to allow for light-time.
*/


#include "JPLEphemeris.hpp"
#include "Exception.hpp"
#include "Assert.hpp"
#include "Vector3.hpp"
#include "StringUtil.hpp"
#include "FixEndian.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


JPLEphemeris::JPLEphemeris( )
    :   m_file( 0 ),
        m_wrongEndian( false ),
        m_curBlock( 0 )
{
}

//.............................................................................


JPLEphemeris::JPLEphemeris( const std::string & fileName, bool storeConstants )
    :   m_file( 0 ),
        m_wrongEndian( false ),
        m_curBlock( 0 )
{
    Init( fileName, storeConstants );
}

//.............................................................................

void 
JPLEphemeris::Init( const std::string & fileName, bool storeConstants )
{
    bool openRslt = OpenBinaryFile( fileName );
    if ( ! openRslt )
        throw Exception( "Unable to open JPL ephemeris file." );

    bool headerRslt = ReadBinaryFileHeader( storeConstants );
    if ( ! headerRslt )
        throw Exception( "Unable to read JPL ephemeris file header." );

    m_chebyVals[0] = 1.0;
    m_chebyVals[1] = -1000.0;
    m_chebyDerivs[0] = 0.0;
    m_chebyDerivs[1] = 1.0;
}

//.............................................................................

bool 
JPLEphemeris::OpenBinaryFile( const std::string & fileName )
{
    if ( m_file )
        fclose( m_file );
    m_file = fopen( fileName.c_str(), "rb" );
    return ( m_file != 0 );
}

//.............................................................................

bool 
JPLEphemeris::ReadBinaryFileHeader( bool storeConstants )
{                                                        //ReadBinaryFileHeader
    const int TitleLen = 84;
    const int MaxConstants = 400;
    const int ConstNameLen = 6;

    int bytesRead = 0;

    char title[ TitleLen + 1 ];
    title[ TitleLen ] = 0;
    for ( int i = 0; i < NumTitles; ++i )
    {
        bytesRead = fread( title, 1, TitleLen, m_file );
        if ( bytesRead != TitleLen )
            return false;
        if ( storeConstants )
        {
            TrimTrailing( title );
            m_titles[i] = title;
        }
    }

    m_constants.clear( );
    char constName[ ConstNameLen + 1 ];
    constName[ ConstNameLen ] = 0;
    for ( int i = 0; i < MaxConstants; ++i )
    {
        bytesRead = fread( constName, 1, ConstNameLen, m_file );
        if ( bytesRead != ConstNameLen )
            return false;
        if ( storeConstants )
        {
            TrimTrailing( constName );
            if ( strlen( constName ) > 0 )
            {
                Constant constant;
                constant.m_name = constName;
                constant.m_value = 0.;
                m_constants.push_back( constant );
            }
        }
    }

    bytesRead = fread( &m_jdStart, 1, sizeof(m_jdStart), m_file );
    if ( bytesRead != sizeof(m_jdStart) )
        return false;
    bytesRead = fread( &m_jdEnd, 1, sizeof(m_jdEnd), m_file );
    if ( bytesRead != sizeof(m_jdEnd) )
        return false;
    bytesRead = fread( &m_blockInterval, 1, sizeof(m_blockInterval), m_file );
    if ( bytesRead != sizeof(m_blockInterval) )
        return false;

    uint32_t numConstants;
    bytesRead = fread( &numConstants, 1, sizeof(numConstants), m_file );
    if ( bytesRead != sizeof(numConstants) )
        return false;

    bytesRead = fread( &m_astronomicalUnit, 1, sizeof(m_astronomicalUnit), m_file );
    if ( bytesRead != sizeof(m_astronomicalUnit) )
        return false;
    
    bytesRead = fread( &m_earthMoonRatio, 1, sizeof(m_earthMoonRatio), m_file );
    if ( bytesRead != sizeof(m_earthMoonRatio) )
        return false;

    for ( int i = 0; i < Lib; ++i )
    {
        for ( int j = 0; j < 3; ++j )
        {
            uint32_t ui;
            bytesRead = fread( &ui, 1, sizeof(ui), m_file );
            if ( bytesRead != sizeof(ui) )
                return false;
            if ( j == 0 )
                m_coeffLayouts[i].m_offset = static_cast<int>( ui );
            else if ( j == 1 )
                m_coeffLayouts[i].m_numCoeffs = static_cast<int>( ui );
            else
                m_coeffLayouts[i].m_numSubIntervals = static_cast<int>( ui );
        }
    }

    uint32_t ver;
    bytesRead = fread( &ver, 1, sizeof(ver), m_file );
    if ( bytesRead != sizeof(ver) )
        return false;
    m_version = ver;

    //For some reason, libration data follow the DE version.
    for ( int j = 0; j < 3; ++j )
    {
        uint32_t ui;
        bytesRead = fread( &ui, 1, sizeof(ui), m_file );
        if ( bytesRead != sizeof(ui) )
            return false;
        if ( j == 0 )
            m_coeffLayouts[ Lib ].m_offset = static_cast<int>( ui );
        else if ( j == 1 )
            m_coeffLayouts[ Lib ].m_numCoeffs = static_cast<int>( ui );
        else
            m_coeffLayouts[ Lib ].m_numSubIntervals = static_cast<int>( ui );
    }

    for ( int i = 0; i < NumTargets; ++i )
        m_coeffLayouts[i].m_numComponents = ( i == Nut  ?  2  :  3 );

    if ( numConstants > MaxConstants )
    {
        m_wrongEndian = true;
        SwapEndian( &numConstants );
    }
    if ( numConstants > MaxConstants )
        return false;
    if ( storeConstants && (numConstants != m_constants.size()) )
        return false;

    for ( int i = 0; i < MaxConstants; ++i )
    {
        double val;
        bytesRead = fread( &val, 1, sizeof(val), m_file );
        if ( bytesRead != sizeof(val) )
            return false;
        if ( (i < static_cast<int>(numConstants)) && storeConstants )
            m_constants[i].m_value = val;
    }

    if ( m_wrongEndian )
    {
        SwapEndian( &m_jdStart );
        SwapEndian( &m_jdEnd );
        SwapEndian( &m_blockInterval );
        SwapEndian( &m_astronomicalUnit );
        SwapEndian( &m_earthMoonRatio );
        for ( int i = 0; i < NumTargets; ++i )
        {
            SwapEndian( &m_coeffLayouts[i].m_offset );
            SwapEndian( &m_coeffLayouts[i].m_numCoeffs );
            SwapEndian( &m_coeffLayouts[i].m_numSubIntervals );
        }
        SwapEndian( &m_version );
        if ( storeConstants )
            for ( int i = 0; i < static_cast<int>(numConstants); ++i )
                SwapEndian( &m_constants[i].m_value );
    }

    int maxOffset = 0;
    int maxIndex = 0;
    for ( int i = 0; i < NumTargets; ++i )
    {
        --m_coeffLayouts[i].m_offset;   //Need 0-based rather than 1-based
        if ( m_coeffLayouts[i].m_offset > maxOffset )
        {
            maxOffset = m_coeffLayouts[i].m_offset;
            maxIndex = i;
        }
        //If this assertion fails, probably need to increase MaxCoeffs.
        Assert( m_coeffLayouts[ Lib ].m_numCoeffs <= MaxCoeffs );
    }
    m_coeffsPerBlock = m_coeffLayouts[ maxIndex ].m_offset
            +  m_coeffLayouts[ maxIndex ].m_numCoeffs
            * m_coeffLayouts[ maxIndex ].m_numComponents
            * m_coeffLayouts[ maxIndex ].m_numSubIntervals;
    m_dataOffset = 2 * m_coeffsPerBlock * sizeof( double );

    return true;
}                                                        //ReadBinaryFileHeader

//-----------------------------------------------------------------------------

JPLEphemeris::~JPLEphemeris( )
{
    if ( m_file )
        fclose( m_file );
}

//=============================================================================

JPLEphemeris::EBody 
JPLEphemeris::SolarSystemToJPLBody( SolarSystem::EBody body )
{
    const EBody ssToJPL[ SolarSystem::NumBodies ]
            = { Sun, Moon, Mercury, Venus, Earth, Mars, Jupiter, Saturn,
                Uranus, Neptune, Pluto };
    return  ssToJPL[ body ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

SolarSystem::EBody 
JPLEphemeris::JPLToSolarSystemBody( EBody body )
{
    const SolarSystem::EBody jplToSS[ NumBodies ]
            = { SolarSystem::NumBodies, SolarSystem::Sun,
                SolarSystem::Mercury, SolarSystem::Venus,
                SolarSystem::Earth, SolarSystem::Moon,
                SolarSystem::NumBodies, SolarSystem::Mars,
                SolarSystem::Jupiter, SolarSystem::Saturn,
                SolarSystem::Uranus, SolarSystem::Neptune,
                SolarSystem::Pluto };
    return  jplToSS[ body ];
}

//=============================================================================

bool 
JPLEphemeris::GetBodyPosition( double julianDay,
                               EBody body, EBody origin,
                               Point3D * pPosition, Vector3D * pVelocity )
{
    return GetBodyPosition( julianDay, 0.0, body, origin,
                            pPosition, pVelocity );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool 
JPLEphemeris::GetBodyPosition( double julianDay0, double julianDay1,
                               EBody body, EBody origin,
                               Point3D * pPosition, Vector3D * pVelocity )
{                                                           /*GetBodyPosition*/
    Assert( pPosition != 0 );

    if ( body == origin )
    {
        pPosition->Set( 0., 0., 0. );
        if ( pVelocity != 0 )
            pVelocity->Set( 0., 0., 0. );
        return true;
    }

    if ( (body == SolarSystemBarycenter)
         || ((body == Earth) && (origin != SolarSystemBarycenter))
         || ((body == Moon) && (origin != SolarSystemBarycenter)
             && (origin != Earth) && (origin != EarthMoonBarycenter))
         || ((body == EarthMoonBarycenter)
             && (origin != SolarSystemBarycenter) && (origin != Earth)) )
    {
        Point3D origPos;
        bool posRslt = GetBodyPosition( julianDay0, julianDay1, origin, body,
                                        &origPos, pVelocity );
        pPosition->Set( - origPos.ToVector() );
        if ( pVelocity != 0 )
            *pVelocity = - *pVelocity;
        return posRslt;
    }

    Vector3D bodyPos;
    Vector3D bodyVel;
    Vector3D originPos;
    Vector3D originVel;
    Vector3D moonPos;
    Vector3D moonVel;
    Vector3D embPos;
    Vector3D embVel;

    if ( body == Earth )
    {
        Assert( origin == SolarSystemBarycenter );
        if ( pVelocity == 0 )
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1,
                                               MoonGeo, &moonPos );
            if ( ! compRslt )
                return false;
            compRslt = ComputeComponents( julianDay0, julianDay1,
                                          EMBary, &embPos );
            Assert( compRslt );
            GetEarthBarycentric( pPosition,
                                 Point3D( moonPos ), Point3D( embPos ) );
        }
        else
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1,
                                               MoonGeo, &moonPos, &moonVel );
            if ( ! compRslt )
                return false;
            compRslt = ComputeComponents( julianDay0, julianDay1,
                                          EMBary, &embPos, &embVel );
            Assert(  compRslt );
            GetEarthBarycentric( pPosition,
                                 Point3D( moonPos ), Point3D( embPos ),
                              pVelocity, moonVel, embVel );
        }
    }
    else if ( body == Moon )
    {
        if ( origin == Earth )
        {
            bool compRslt =  ComputeComponents( julianDay0, julianDay1, MoonGeo,
                                                &moonPos, pVelocity );
            if ( compRslt )
                pPosition->Set( moonPos );
            return compRslt;
        }
        if ( pVelocity == 0 )
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1,
                                               MoonGeo, &moonPos );
            if ( ! compRslt )
                return false;
            if ( origin == EarthMoonBarycenter )
            {
                moonPos *= (m_earthMoonRatio / (1.0 + m_earthMoonRatio));
                pPosition->Set( moonPos );
            }
            else
            {
                Assert( origin == SolarSystemBarycenter );
                compRslt = ComputeComponents( julianDay0, julianDay1,
                                              EMBary, &embPos );
                Assert( compRslt );
                GetMoonBarycentric( pPosition,
                                    Point3D( moonPos ), Point3D( embPos ) );
            }
        }
        else
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1,
                                               MoonGeo, &moonPos, &moonVel );
            if ( ! compRslt )
                return false;
            if ( origin == EarthMoonBarycenter )
            {
                double emrr = m_earthMoonRatio / (1.0 + m_earthMoonRatio);
                moonPos *= emrr;
                pPosition->Set( moonPos );
                *pVelocity = moonVel * emrr;
            }
            else
            {
                Assert( origin == SolarSystemBarycenter );
                compRslt = ComputeComponents( julianDay0, julianDay1,
                                              EMBary, &embPos, &embVel );
                Assert( compRslt );
                GetMoonBarycentric( pPosition,
                                    Point3D( moonPos ), Point3D( embPos ),
                                    pVelocity, moonVel, embVel );
            }
        }
    }
    else if ( body == EarthMoonBarycenter )
    {
        if ( origin == SolarSystemBarycenter )
        {
            
            bool compRslt = ComputeComponents( julianDay0, julianDay1, EMBary,
                                               &bodyPos, pVelocity );
            if ( compRslt )
                pPosition->Set( bodyPos );
            return compRslt;
        }
        Assert( origin == Earth );
        if ( pVelocity == 0 )
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1,
                                               MoonGeo, &moonPos );
            if ( ! compRslt )
                return false;
            moonPos *= (1.0 / (1.0 + m_earthMoonRatio));
            pPosition->Set( moonPos );
        }
        else
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1,
                                               MoonGeo, &moonPos, &moonVel );
            if ( ! compRslt )
                return false;
            double emrr = 1.0 / (1.0 + m_earthMoonRatio);
            moonPos *= emrr;
            pPosition->Set( moonPos );
            *pVelocity = moonVel * emrr;
        }
    }
    else
    {
        ETarget bodyToTarget[ NumBodies ]
                = { NumTargets, Sol, Mer, Ven, NumTargets, NumTargets,
                    EMBary, Mar, Jup, Sat, Ura, Nep, Plu };
        ETarget target = bodyToTarget[ body ];
        Assert( target < NumTargets );
        if ( origin == SolarSystemBarycenter )
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1, target,
                                               &bodyPos, pVelocity );
            if ( compRslt )
                pPosition->Set( bodyPos );
            return compRslt;
        }
        if ( pVelocity == 0 )
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1, target,
                                               &bodyPos );
            if ( ! compRslt )
                return false;
            ETarget center = bodyToTarget[ origin ];
            if ( center < NumTargets )
            {
                compRslt = ComputeComponents( julianDay0, julianDay1, center,
                                              &originPos );
                Assert( compRslt );
            }
            else
            {
                compRslt = ComputeComponents( julianDay0, julianDay1,
                                              MoonGeo, &moonPos );
                Assert( compRslt );
                compRslt = ComputeComponents( julianDay0, julianDay1,
                                              EMBary, &embPos );
                Assert( compRslt );
                Point3D pos;
                if ( origin == Earth )
                {
                    GetEarthBarycentric( &pos, Point3D( moonPos ),
                                         Point3D( embPos ) );
                }
                else
                {
                    Assert( origin == Moon );
                    GetMoonBarycentric( &pos,
                                        Point3D( moonPos ), Point3D( embPos ) );
                }
                originPos = pos.ToVector();
            }
            pPosition->Set( bodyPos - originPos );
        }
        else
        {
            bool compRslt = ComputeComponents( julianDay0, julianDay1, target,
                                               &bodyPos, &bodyVel );
            if ( ! compRslt )
                return false;
            ETarget center = bodyToTarget[ origin ];
            if ( center < NumTargets )
            {
                compRslt = ComputeComponents( julianDay0, julianDay1, center,
                                              &originPos, &originVel );
                Assert( compRslt );
            }
            else
            {
                compRslt = ComputeComponents( julianDay0, julianDay1,
                                              MoonGeo, &moonPos, &moonVel );
                Assert( compRslt );
                compRslt = ComputeComponents( julianDay0, julianDay1,
                                              EMBary, &embPos, &embVel );
                Assert( compRslt );
                Point3D pos;
                if ( origin == Earth )
                {

                    GetEarthBarycentric( &pos,
                                         Point3D( moonPos ), Point3D( embPos ),
                                         &originVel, moonVel, embVel );
                }
                else
                {
                    Assert( origin == Moon );
                    GetMoonBarycentric( &pos,
                                        Point3D( moonPos ), Point3D( embPos ), 
                                        &originVel, moonVel, embVel );
                }
                originPos = pos.ToVector();
            }
            pPosition->Set( bodyPos - originPos );
            *pVelocity = bodyVel - originVel;
        }
    }
    return true;
}                                                           /*GetBodyPosition*/

//-----------------------------------------------------------------------------

bool
JPLEphemeris::GetNutation( double julianDay,
                           Nutation * pNutation, Nutation * pDerivative )
{
    return GetNutation( julianDay, 0.0, pNutation, pDerivative );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool 
JPLEphemeris::GetNutation( double julianDay0, double julianDay1,
                           Nutation * pNutation, Nutation * pDerivative )
{
    if ( ! NutationAvailable() )
        return false;
    Assert( pNutation != 0 );

    Vector3D comp;
    if ( pDerivative == 0 )
    {
        bool compRslt = ComputeComponents( julianDay0, julianDay1, Nut,
                                           &comp );
        pNutation->Set( Angle( comp[0] ), Angle( comp[1] ) );
        return compRslt;
    }
    else
    {
        Vector3D vel;
        bool compRslt = ComputeComponents( julianDay0, julianDay1, Nut,
                                           &comp, &vel );
        pNutation->Set( Angle( comp[0] ), Angle( comp[1] ) );
        pDerivative->Set( Angle( vel[0] ), Angle( vel[1] ) );
        return compRslt;
    }
}

//-----------------------------------------------------------------------------

bool 
JPLEphemeris::GetLibration( double julianDay,
                            Vector3< Angle > * pComponents,
                            Vector3< Angle > * pVelocity )
{
    return GetLibration( julianDay, 0.0, pComponents, pVelocity );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool 
JPLEphemeris::GetLibration( double julianDay0, double julianDay1,
                            Vector3< Angle > * pComponents,
                            Vector3< Angle > * pVelocity )
{
    if ( ! LibrationAvailable() )
        return false;
    Vector3D comp;
    if ( pVelocity == 0 )
    {
        bool compRslt =  ComputeComponents( julianDay0, julianDay1, Lib,
                                            &comp );
        pComponents->Set( Angle( comp[0] ), Angle( comp[1] ),
                          Angle( comp[2] ) );
        return compRslt;
    }
    else
    {
        Vector3D vel;
        bool compRslt =  ComputeComponents( julianDay0, julianDay1, Lib,
                                            &comp, &vel );
        pComponents->Set( Angle( comp[0] ), Angle( comp[1] ),
                          Angle( comp[2] ) );
        pVelocity->Set( Angle( vel[0] ), Angle( vel[1] ), Angle( vel[2] ) );
        return compRslt;
    }
}

//-----------------------------------------------------------------------------

void 
JPLEphemeris::GetEarthBarycentric( Point3D * pEarthPos,
                                   const Point3D & moonGeoPos,
                                   const Point3D & embPos ) const
{
    double emrr = 1.0 / (1.0 + m_earthMoonRatio);
    pEarthPos->Set( embPos.ToVector()  -  moonGeoPos.ToVector() * emrr );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
JPLEphemeris::GetEarthBarycentric( Point3D * pEarthPos,
                                   const Point3D & moonGeoPos,
                                   const Point3D & embPos,
                                   Vector3D * pEarthVelocity,
                                   const Vector3D & moonGeoVel,
                                   const Vector3D & embVel ) const
{
    double emrr = 1.0 / (1.0 + m_earthMoonRatio);
    pEarthPos->Set( embPos.ToVector()  -  moonGeoPos.ToVector() * emrr );
    *pEarthVelocity = embVel  -  moonGeoVel * emrr;
}

//-----------------------------------------------------------------------------

void 
JPLEphemeris::GetMoonBarycentric( Point3D * pMoonPos,
                                  const Point3D & moonGeoPos,
                                  const Point3D & embPos ) const
{
    double emrr = m_earthMoonRatio / (1.0 + m_earthMoonRatio);
    pMoonPos->Set( embPos.ToVector()  +  moonGeoPos.ToVector() * emrr );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
JPLEphemeris::GetMoonBarycentric( Point3D * pMoonPos,
                                  const Point3D & moonGeoPos,
                                  const Point3D & embPos,
                                  Vector3D * pMoonVelocity,
                                  const Vector3D & moonGeoVel,
                                  const Vector3D & embVel ) const
{
    double emrr = m_earthMoonRatio / (1.0 + m_earthMoonRatio);
    pMoonPos->Set( embPos.ToVector()  +  moonGeoPos.ToVector() * emrr );
    *pMoonVelocity = embVel  +  moonGeoVel * emrr;
}

//=============================================================================

double 
JPLEphemeris::firstJulianDay( ) const
{
    return m_jdStart;
}

//-----------------------------------------------------------------------------

double 
JPLEphemeris::lastJulianDay( ) const
{
    return m_jdEnd;
}

//=============================================================================

bool 
JPLEphemeris::NutationAvailable( ) const
{
    return (m_coeffLayouts[ Nut ].m_numCoeffs > 0);
}

//-----------------------------------------------------------------------------

bool 
JPLEphemeris::LibrationAvailable( ) const
{
    return (m_coeffLayouts[ Lib ].m_numCoeffs > 0);
}

//=============================================================================

std::string 
JPLEphemeris::Title( int index ) const
{
    Assert( index < NumTitles );
    return m_titles[ index ];
}

//-----------------------------------------------------------------------------

int
JPLEphemeris::Version( ) const
{
    return m_version;
}

//-----------------------------------------------------------------------------

const std::vector< JPLEphemeris::Constant > & 
JPLEphemeris::Constants( ) const
{
    return m_constants;
}

//=============================================================================

bool 
JPLEphemeris::LoadCoeffBlock( double julianDay0, double julianDay1 )
{
    //We maintain a cache of two blocks to avoid thrashing if we are computing
    // positions at a variety of times near a block boundary.
    //On success, m_coeffBlocks[ m_curBlock ] will be the appropriate block.
    shared_array< const double > block;
    if ( m_coeffBlocks[ m_curBlock ] == 0 )
    {
        m_coeffBlocks[ m_curBlock ].reset( new double[ m_coeffsPerBlock ] );
        block = m_coeffBlocks[ m_curBlock ];
    }
    else
    {
        block = m_coeffBlocks[ m_curBlock ];
        double blockDiff = julianDay0 - block[0];
        blockDiff += julianDay1;
        //block[0] and block[1] contain the start and end dates of the block.
        if ( (blockDiff >= 0.0) && (block[0] + blockDiff <= block[1]) )
            return true;
        else
        {
            m_curBlock = 1 - m_curBlock;  //toggle 0 <-> 1
            if ( m_coeffBlocks[ m_curBlock ] == 0 )
            {
                m_coeffBlocks[ m_curBlock ].reset(
                    new double[ m_coeffsPerBlock ] );
                block = m_coeffBlocks[ m_curBlock ];
            }
            else
            {
                block = m_coeffBlocks[ m_curBlock ];
                blockDiff = julianDay0 - block[0];
                blockDiff += julianDay1;
                if ( (blockDiff >= 0.0) && (block[0] + blockDiff <= block[1]) )
                    return true;
            }
        }
    }
    double ephemDiff = julianDay0 - m_jdStart;
    ephemDiff += julianDay1;
    if ( (ephemDiff < 0.0) || (m_jdStart + ephemDiff > m_jdEnd) )
    {
        throw Exception( "Date out of range of JPL ephemeris." );
        //return false;
    }
    int blockNumber = static_cast<int>( ephemDiff / m_blockInterval );
    if ( m_jdStart + ephemDiff == m_jdEnd )
        --blockNumber;  //Special case at end of ephemeris.
    int blockSize = m_coeffsPerBlock * sizeof( double );
    Assert( m_file != 0 );
    if ( m_file == 0 )
        return false;
    int fileOffset = m_dataOffset + (blockNumber * blockSize);
    int seekRslt = fseek( m_file, fileOffset, SEEK_SET );
    if ( seekRslt != 0 )
    {
        throw Exception( "Seek failed." );
        //return false;
    }
    int bytesRead = fread( const_cast< double * >( block.get() ), 1, blockSize,
                           m_file );
    if ( bytesRead != blockSize )
    {
        throw Exception( "Read failed." );
        //return false;
    }
    if ( m_wrongEndian )
    {
        double * pCoeff = const_cast< double * >( block.get() );
        for ( int i = 0; i < m_coeffsPerBlock; ++i, ++pCoeff )
            SwapEndian( pCoeff );
    }
    return true;
}

//.............................................................................

bool 
JPLEphemeris::GetTargetCoefficients( double julianDay0, double julianDay1, 
                                     ETarget target )
{
    shared_array< const double > coeffBlock( m_coeffBlocks[ m_curBlock ] );
    m_targetCoeffs = coeffBlock.get() + m_coeffLayouts[ target ].m_offset;
    double blockDiff = julianDay0 - coeffBlock[0];
    blockDiff += julianDay1;
    double blockFrac = blockDiff / m_blockInterval;
    double numSubIntervals = static_cast< double >(
        m_coeffLayouts[ target ].m_numSubIntervals );
    double bf1 = static_cast< double >( static_cast< int >( blockFrac ) );
    //use bf1 to catch the special case blockFrac==1.0.
    int subIntervalIndex = static_cast< int >(
        blockFrac * numSubIntervals  -  bf1 );
    int offset = subIntervalIndex * m_coeffLayouts[ target ].m_numCoeffs
            * m_coeffLayouts[ target ].m_numComponents;
    m_targetCoeffs += offset;
    m_subInterval = m_blockInterval / numSubIntervals;
    double subIntervalDiff = blockDiff  -  subIntervalIndex * m_subInterval;
    m_timeFrac = subIntervalDiff / m_subInterval;
    Assert( (m_timeFrac >= 0.0) && (m_timeFrac <= 1.0) );
    return true;
}

//.............................................................................

double 
JPLEphemeris::EvalChebyshev( const double * coeffs, int numCoeffs,
                             double timeFrac,
                             double * pDerivative )
{
    Assert( (timeFrac >= 0.0) && (timeFrac <= 1.0) );
    double t = 2.0 * timeFrac  -  1.0;
    if ( t != m_chebyVals[1] )
    {
        //Chebyshev polynomials not cached; start from scratch.
        m_numChebys = 2;
        m_chebyVals[1] = t;
        m_t2 = t + t;
        m_numDerivs = 3;
        m_chebyDerivs[2] = m_t2 + m_t2;
    }
    if ( m_numChebys < numCoeffs )
    {
        //Compute remaining Chebyshev polynomials.
        double * pVal = m_chebyVals + m_numChebys;
        for ( int i = (numCoeffs - m_numChebys); i > 0; --i, ++pVal )
            *pVal = m_t2 * pVal[-1]  -  pVal[-2];
        m_numChebys = numCoeffs;
    }
    double x = 0.0;
    const double * pVal = m_chebyVals + numCoeffs;
    const double * pCoeff = coeffs + numCoeffs;
    for ( int i = numCoeffs; i > 0; --i )
        x += (*--pVal) * (*--pCoeff);

    if ( pDerivative != 0 )
    {
        if ( m_numDerivs < numCoeffs )
        {
            double * pDeriv = m_chebyDerivs + m_numDerivs;
            pVal = m_chebyVals + m_numDerivs - 1;
            for ( int i = (numCoeffs - m_numDerivs); i > 0;
                  --i, ++pDeriv, ++pVal )
                *pDeriv = m_t2 * pDeriv[-1]  +  *pVal + *pVal  -  pDeriv[-2];
            m_numDerivs = numCoeffs;
        }
        double v = 0.0;
        const double * pDeriv = m_chebyDerivs + numCoeffs;
        pCoeff = coeffs + numCoeffs;
        for ( int i = numCoeffs; i > 1; --i )
            v += (*--pDeriv) * (*--pCoeff);
        *pDerivative = 2.0 * v;
    }

    return x;
}

//.............................................................................

bool 
JPLEphemeris::ComputeComponents( double julianDay0, double julianDay1,
                                 ETarget target,
                                 Vector3D * pComponents,
                                 Vector3D * pDerivatives )
{
    Assert( pComponents != 0 );
    bool loadRslt = LoadCoeffBlock( julianDay0, julianDay1 );
    if ( ! loadRslt )
        return false;
    bool coeffRslt = GetTargetCoefficients( julianDay0, julianDay1, target );
    if ( ! coeffRslt )
        return false;
    int numComponents = m_coeffLayouts[ target ].m_numComponents;
    int numCoeffs = m_coeffLayouts[ target ].m_numCoeffs;
    for ( int i = 0; i < numComponents; ++i )
    {
        double & comp = (*pComponents)[i];
        if ( pDerivatives == 0 )
            comp = EvalChebyshev( m_targetCoeffs, numCoeffs, m_timeFrac );
        else
        {
            double & deriv = (*pDerivatives)[i];
            comp = EvalChebyshev( m_targetCoeffs, numCoeffs, m_timeFrac,
                                  &deriv );
            deriv /= m_subInterval;
        }
        m_targetCoeffs += numCoeffs;
    }
    return true;
}

//=============================================================================

namespace
{
std::vector< JPLEphemeris * > s_ephemerides;
} //namespace

//-----------------------------------------------------------------------------

void 
JPLEphemeris::RegisterEphemeris( JPLEphemeris & ephem )
{
    s_ephemerides.push_back( &ephem );
}

//-----------------------------------------------------------------------------

JPLEphemeris * 
JPLEphemeris::GetEphemeris( double jd )
{
    for ( int i = 0; i < static_cast<int>(s_ephemerides.size()); ++i )
    {
        if ( (s_ephemerides[i]->firstJulianDay() <= jd)
             && (jd <= s_ephemerides[i]->lastJulianDay()) )
            return s_ephemerides[i];
    }
    return 0;
}


//*****************************************************************************


Point3D 
JPLBarycentricEphemeris::operator()( double julianDay )
{
    Point3D bodyPos;
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay, m_body,
                                          JPLEphemeris::SolarSystemBarycenter,
                                          &bodyPos );
    if ( ! posRslt )
        throw Exception( "JPLBarycentricEphemeris failed." );
    return bodyPos;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Point3D 
JPLBarycentricEphemeris::operator()( double julianDay0, double julianDay1 )
{
    Point3D bodyPos;
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay0, julianDay1, m_body,
                                          JPLEphemeris::SolarSystemBarycenter,
                                          &bodyPos );
    if ( ! posRslt )
        throw Exception( "JPLBarycentricEphemeris failed." );
    return bodyPos;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
JPLBarycentricEphemeris::operator()( double julianDay,
                                     Point3D * pPosition,
                                     Vector3D * pVelocity )
{
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay, m_body,
                                          JPLEphemeris::SolarSystemBarycenter,
                                          pPosition, pVelocity );
    if ( ! posRslt )
        throw Exception( "JPLBarycentricEphemeris failed." );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
JPLBarycentricEphemeris::operator()( double julianDay0, double julianDay1, 
                                     Point3D * pPosition,
                                     Vector3D * pVelocity )
{
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay0, julianDay1, m_body,
                                          JPLEphemeris::SolarSystemBarycenter,
                                          pPosition, pVelocity );
    if ( ! posRslt )
        throw Exception( "JPLBarycentricEphemeris failed." );
}


//*****************************************************************************


Point3D 
JPLGeocentricEphemeris::operator()( double julianDay )
{
    Point3D bodyPos;
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay, m_body,
                                             JPLEphemeris::Earth, 
                                             &bodyPos );
    if ( ! posRslt )
        throw Exception( "JPLGeocentricEphemeris failed." );
    return bodyPos;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Point3D 
JPLGeocentricEphemeris::operator()( double julianDay0, double julianDay1 )
{
    Point3D bodyPos;
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay0, julianDay1, m_body,
                                             JPLEphemeris::Earth, 
                                             &bodyPos );
    if ( ! posRslt )
        throw Exception( "JPLGeocentricEphemeris failed." );
    return bodyPos;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
JPLGeocentricEphemeris::operator()( double julianDay,
                                    Point3D * pPosition,
                                    Vector3D * pVelocity )
{
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay, m_body,
                                             JPLEphemeris::Earth,
                                             pPosition, pVelocity );
    if ( ! posRslt )
        throw Exception( "JPLBarycentricEphemeris failed." );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
JPLGeocentricEphemeris::operator()( double julianDay0, double julianDay1, 
                                    Point3D * pPosition,
                                    Vector3D * pVelocity )
{
    bool posRslt
            = m_pEphemeris->GetBodyPosition( julianDay0, julianDay1, m_body,
                                             JPLEphemeris::Earth,
                                             pPosition, pVelocity );
    if ( ! posRslt )
        throw Exception( "JPLBarycentricEphemeris failed." );
}


//*****************************************************************************


#ifdef DEBUG

bool 
JPLEphemeris::Test( const std::string & testFileName, bool verbose )
{
    bool ok = true;
    cout << "Testing JPLEphemeris" << endl;

    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Sun ), Sun, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Moon ), Moon, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Mercury ), Mercury, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Venus ), Venus, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Earth ), Earth, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Mars ), Mars, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Jupiter ), Jupiter, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Saturn ), Saturn, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Uranus ), Uranus, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Neptune ), Neptune, &ok );
    TESTCHECK( SolarSystemToJPLBody( SolarSystem::Pluto ), Pluto, &ok );
    TESTCHECK( JPLToSolarSystemBody( Sun ), SolarSystem::Sun, &ok );
    TESTCHECK( JPLToSolarSystemBody( Moon ), SolarSystem::Moon, &ok );
    TESTCHECK( JPLToSolarSystemBody( Mercury ), SolarSystem::Mercury, &ok );
    TESTCHECK( JPLToSolarSystemBody( Venus ), SolarSystem::Venus, &ok );
    TESTCHECK( JPLToSolarSystemBody( Earth ), SolarSystem::Earth, &ok );
    TESTCHECK( JPLToSolarSystemBody( Mars ), SolarSystem::Mars, &ok );
    TESTCHECK( JPLToSolarSystemBody( Jupiter ), SolarSystem::Jupiter, &ok );
    TESTCHECK( JPLToSolarSystemBody( Saturn ), SolarSystem::Saturn, &ok );
    TESTCHECK( JPLToSolarSystemBody( Uranus ), SolarSystem::Uranus, &ok );
    TESTCHECK( JPLToSolarSystemBody( Neptune ), SolarSystem::Neptune, &ok );
    TESTCHECK( JPLToSolarSystemBody( Pluto ), SolarSystem::Pluto, &ok );

    char testLine[ 102 ];
    int lineNum = 0;

    for ( int i = 0; i < NumTitles; ++i )
        if ( Title( i ).size() > 0 )
            cout << Title( i ) << endl;

    cout << "Test file: " << testFileName << endl;
    FILE * testFile = fopen( testFileName.c_str(), "r" );
    if ( testFile == 0 )
    {
        cout << "Unable to open " << testFileName << endl;
        ok = false;
        goto finished;
    }

    while ( (fgets( testLine, 100, testFile ) != 0)
            && (memcmp( testLine, "EOT", 3 ) != 0) )
        ++lineNum;

    while ( fgets( testLine, 100, testFile ) != 0 )
    {
        double julianDay;
        int targNum;
        int origNum;
        int component;
        double correctValue;
        sscanf( (testLine + 15), " %lf %d %d %d %lf",
                &julianDay, &targNum, &origNum, &component, &correctValue );
        ++lineNum;

        --component;

        double computedValue;
        bool rslt;
        if ( targNum == 14 )
        {
            Nutation nutation;
            if ( component < 2 )
            {
                rslt = GetNutation( julianDay, &nutation );
                if ( ! rslt )
                {
                    cout << "GetNutation() failed. Line " << lineNum << endl;
                    ok = false;
                    goto finished;
                }
                if ( component == 0 )
                    computedValue = nutation.NutLongitude().Radians();
                else
                    computedValue = nutation.NutObliquity().Radians();
            }
            else
            {
                Nutation nutationDeriv;
                rslt = GetNutation( julianDay, &nutation, &nutationDeriv );
                if ( ! rslt )
                {
                    cout << "GetNutation() failed. Line " << lineNum << endl;
                    ok = false;
                    goto finished;
                }
                if ( component == 2 )
                    computedValue = nutationDeriv.NutLongitude().Radians();
                else
                    computedValue = nutationDeriv.NutObliquity().Radians();
            }
        }
        else if ( targNum == 15 )
        {
            Vector3< Angle > libration;
            if ( component < 3 )
            {
                rslt = GetLibration( julianDay, &libration );
                if ( ! rslt )
                {
                    cout << "GetLibration() failed. Line " << lineNum << endl;
                    ok = false;
                    goto finished;
                }
                computedValue = libration[ component ].Radians();
            }
            else
            {
                Vector3< Angle > librationVel;
                rslt = GetLibration( julianDay,
                                           &libration, &librationVel );
                if ( ! rslt )
                {
                    cout << "GetLibration() failed. Line " << lineNum << endl;
                    ok = false;
                    goto finished;
                }
                computedValue = librationVel[ component - 3 ].Radians();
            }
        }
        else
        {
            EBody numToBody[ 15 ]
                    = { Mercury, Venus, Earth, Mars, Jupiter, Saturn,
                        Uranus, Neptune, Pluto, Moon, Sun,
                        SolarSystemBarycenter, EarthMoonBarycenter,
                        NumBodies, NumBodies };

            EBody body = numToBody[ targNum - 1 ];
            Assert( body < NumBodies );
            EBody origin = numToBody[ origNum - 1 ];
            Assert( origin < NumBodies );

            Point3D position;
            if ( component < 3 )
            {
                rslt = GetBodyPosition( julianDay, body, origin,
                                              &position );
                if ( ! rslt )
                {
                    cout << "GetBodyPosition() failed. Line " << lineNum << endl;
                    ok = false;
                    goto finished;
                }
                computedValue = position[ component ] / AUinKM();
            }
            else
            {
                Vector3D velocity;
                rslt = GetBodyPosition( julianDay, body, origin,
                                        &position, &velocity );
                if ( ! rslt )
                {
                    cout << "GetBodyPosition() failed. Line " << lineNum << endl;
                    ok = false;
                    goto finished;
                }
                computedValue = velocity[ component - 3 ] / AUinKM();
            }
        }

        double valDiff = fabs( computedValue - correctValue );
        bool failed = (valDiff > 1.e-13);

        if ( verbose || failed )
        {
            cout << lineNum << " " << julianDay << " " << targNum << " "
                 << origNum << " " << (component + 1) << " " << correctValue;
            if ( failed )
                cout << " computed=" << computedValue << " FAILED" << endl;
            else
                cout << " OK" << endl;
        }
        
        if ( failed )
        {
            ok = false;
            cout << "Test file line " << lineNum << endl;
        }
    }

 finished:
    if ( testFile != 0 )
        fclose( testFile );

    if ( ok )
    {
        cout << lineNum << " test lines completed." << endl;
        cout << "JPLEphemeris PASSED." << endl << endl;
    }
    else
        cout << "JPLEphemeris FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

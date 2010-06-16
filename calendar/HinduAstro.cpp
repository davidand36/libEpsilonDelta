/*
  HinduAstro.cpp
  Copyright (C) 2007 David M. Anderson

  HinduAstro class: Astronomical routines based on the Surya Siddhanta and the
  basis of the Hindu calendars (solar and lunisolar).
*/


#include "HinduAstro.hpp"
#include "DivMod.hpp"
#include "RootFinder.hpp"
#include <cmath>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace 
{

const double s_siderealYear = 365.  +  279457. / 1080000.;
const double s_anomalisticYear = 1577917828000. / (4320000000. - 387.);
const double s_siderealMonth = 27.  +  4644439. / 14438334.;
const double s_synodicMonth = 29.  +  7087771. / 13358334.;
const double s_anomalisticMonth = 1577917828. / (57753336. - 488199.);
const double s_latitudeUjjain = 23.  +  9. / 60.;  //degrees
const double s_endOfCreation = 1955880000. * s_siderealYear; //before KY epoch

double SinTable( int index );
double Sin( double degrees );
double ArcSin( double s );
double MeanPosition( double kyTime, double period );
double TruePosition( double kyTime, double period,
                     double anomalisticPeriod, 
                     double epicycleSize, double epicycleChange );
double LunarPhase( double kyTime );
double TropicalSolarLongitude( double kyTime );
double AscensionalDifference( double kyTime, double latitude );
double DailyMotion( double kyTime );
double RisingSign( double kyTime );
double SolarSiderealDifference( double kyTime );
double EquationOfTime( double kyTime );

}


//=============================================================================

double 
HinduAstro::SiderealYear( )
{
    return s_siderealYear;
}

//=============================================================================

double 
HinduAstro::SolarLongitude( double kaliYugaTime )   //sidereal longitude
{
    return TruePosition( kaliYugaTime, s_siderealYear, s_anomalisticYear,
                         (14. / 360.), (1. / 42.) );
}

//-----------------------------------------------------------------------------

int 
HinduAstro::Zodiac( double kaliYugaTime )
{
    return (int)( floor( SolarLongitude( kaliYugaTime ) / 30. )  +  1 );
}

//=============================================================================

double 
HinduAstro::SynodicMonth( )
{
    return s_synodicMonth;
}

//=============================================================================

double 
HinduAstro::LunarLongitude( double kaliYugaTime )
{
    return TruePosition( kaliYugaTime, s_siderealMonth, s_anomalisticMonth,
                         (32. / 360.), (1. / 96.) );
}

//-----------------------------------------------------------------------------

double 
HinduAstro::PriorNewMoon( double kaliYugaTime )
{
    double offset = LunarPhase( kaliYugaTime ) * s_synodicMonth / 360.;
    double kyt = kaliYugaTime - offset;
    double lowEst = kyt - 1.;
    double highEst = min( kaliYugaTime, (kyt + 1.) );
    int lowZodiac = Zodiac( lowEst );
    int highZodiac = Zodiac( highEst );
    for ( int i = 0; i < 1000; ++i )
    {
        kyt = (lowEst + highEst) * 0.5;
        if ( lowZodiac == highZodiac )
            break;
        else if ( LunarPhase( kyt ) < 180. )
        {
            highEst = kyt;
            highZodiac = Zodiac( highEst );
        }
        else
        {
            lowEst = kyt;
            lowZodiac = Zodiac( lowEst );
        }
    }
    return kyt;
}

//-----------------------------------------------------------------------------

int 
HinduAstro::LunarDay( double kaliYugaTime )
{
    return (int)( floor( LunarPhase( kaliYugaTime ) / 12. ) )  +  1;
}

//=============================================================================

double 
HinduAstro::Sunrise( int kaliYugaDay )
{
    return kaliYugaDay  +  (1. / 4.)  -  EquationOfTime( kaliYugaDay )
            +  ((1577917828. / 1582237828.) / 360.)
            * ((SolarSiderealDifference( kaliYugaDay ) / 4.)
               +  AscensionalDifference( kaliYugaDay, s_latitudeUjjain ));
}

//=============================================================================

namespace

{                                                                 /*namespace*/

//-----------------------------------------------------------------------------

//Table for first quadrant. One entry for every 225 arcminutes.
//(90 * 60 / 24 = 225)
const int sinTableEntries = 25;
const double sinTable[ sinTableEntries ]
    = { 0., 225., 449., 671., 890., 1105., 1315., 1520., 1719.,
        1910., 2093., 2267., 2431., 2585., 2728., 2859., 2978.,
        3084., 3177., 3256., 3321., 3372., 3409., 3431., 3438. };

//-----------------------------------------------------------------------------

double 
SinTable( int index )
{
    while ( index > 48 )
        index -= 96;
    if ( index < 0 )
        return  - SinTable( -index );
    Assert( (index >= 0) && (index <= 48) );
    if ( index <= 24 )
        return sinTable[ index ];
    else
        return sinTable[ 48 - index ];
}

//-----------------------------------------------------------------------------

double 
Sin( double degrees )
{
    double t = degrees * 60. / 225.;
    double fract = ModRP( t, 1. );
    double s = fract * SinTable( (int)( ceil( t ) ) )
            + (1. - fract) * SinTable( (int)( floor( t ) ) );
    return s / 3438.;
}

//-----------------------------------------------------------------------------

double 
ArcSin( double s )
{
    if ( s < 0. )
        return  - ArcSin( -s );
    Assert( (s >= 0.) && (s <= 1.) );
    s *= 3438.;
    int i = 0;
    for ( ; i < sinTableEntries; ++i )
        if ( sinTable[i] >= s )
            break;
    Assert( i < sinTableEntries );
    if ( i == 0 )
        return 0.;
    double ts0 = sinTable[ i - 1 ];
    double ts1 = sinTable[ i ];
    return  (i - 1  +  (s - ts0) / (ts1 - ts0)) * 225. / 60.;
}

//=============================================================================

double 
MeanPosition( double kyTime, double period )
{
    double creationTime = kyTime + s_endOfCreation;
    return  ModRP( (creationTime / period), 1. ) * 360.;
}

//-----------------------------------------------------------------------------

double 
TruePosition( double kyTime, double period,
                          double anomalisticPeriod, 
                          double epicycleSize, double epicycleChange )
{
    double mean = MeanPosition( kyTime, period );
    double offset = Sin( MeanPosition( kyTime, anomalisticPeriod ) );
    double contraction = fabs( offset ) * epicycleChange * epicycleSize;
    double equation = ArcSin( offset * (epicycleSize - contraction) );
    return  ModRP( (mean - equation), 360. );
}

//=============================================================================

double 
LunarPhase( double kyTime )
{
    return ModRP( (HinduAstro::LunarLongitude( kyTime )
                   - HinduAstro::SolarLongitude( kyTime )), 360. );
}

//=============================================================================

double 
TropicalSolarLongitude( double kyTime )
{
    double midnight = floor( kyTime );
    double precession = 27. 
            -  fabs( 54.
                       -  ModRP( 27. + 108. * (600. / 1577917828.) * midnight,
                                 108. ) );
    return  ModRP( (HinduAstro::SolarLongitude( kyTime ) - precession), 360. );
}

//-----------------------------------------------------------------------------

double 
AscensionalDifference( double kyTime, double latitude )
{
    double sinDecl = (1397. / 3438.) * Sin( TropicalSolarLongitude( kyTime ) );
    double diurnalRadius = Sin( 90.- ArcSin( sinDecl ) );
    double tanLat = Sin( latitude ) / Sin( 90. + latitude );
    double earthSin = sinDecl * tanLat;
    return ArcSin( - earthSin / diurnalRadius );
}

//-----------------------------------------------------------------------------

double 
DailyMotion( double kyTime )
{
    double meanMotion = 360. / s_siderealYear;
    double anomaly = MeanPosition( kyTime, s_anomalisticYear );
    double epicycle = (14. / 360.)  -  fabs( Sin( anomaly ) ) / 1080.;
    int entry = (int)( floor( anomaly / (225. / 60.) ) );
    double sinTableStep = SinTable( entry + 1 )  -  SinTable( entry );
    double equationOfMotionFactor =  epicycle * sinTableStep * (-1. / 225.);
    return  meanMotion * (equationOfMotionFactor + 1.);
}

//-----------------------------------------------------------------------------

double 
RisingSign( double kyTime )
{
    const double risingSigns[ 6 ]
            = { 1670., 1795., 1935., 1935., 1795., 1670. };
    int i = (int)( floor( TropicalSolarLongitude( kyTime ) / 30. ) )  %  6;
    Assert( (i >= 0) && (i < 6) );
    return  risingSigns[ i ] / 1800.;
}

//-----------------------------------------------------------------------------

double 
SolarSiderealDifference( double kyTime )
{
    return  DailyMotion( kyTime ) * RisingSign( kyTime );
}

//-----------------------------------------------------------------------------

double 
EquationOfTime( double kyTime )
{
    double offset = Sin( MeanPosition( kyTime, s_anomalisticYear ) );
    double equationSun = offset * (3438. / 60.)
            * ( (fabs( offset ) / 1080.) - (14. / 360.) );
    return  - DailyMotion( kyTime ) * equationSun * s_siderealYear
            / (360. * 360.);
}

//-----------------------------------------------------------------------------

}                                                                 /*namespace*/

//*****************************************************************************

}                                                      //namespace EpsilonDelta

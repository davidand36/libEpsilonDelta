/*
  LunarVisibility.cpp
  Copyright (C) 2007 David M. Anderson

  Functions to determine whether the lunar crescent is visible.
*/


#include "LunarVisibility.hpp"
#include "AstroPhenomena.hpp"
#include "Horizontal.hpp"
#include "AstroCoordTransformations.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace LunarVisibility
{                                                   //namespace LunarVisibility

//*****************************************************************************


namespace
{

ECriterion s_criterion = Shaukat;

bool ComputeShaukat( double julianDay, const GeodeticLocation & location,
                     ETime timeOfDay );

}

//=============================================================================

bool 
CheckNext( double julianDay, const GeodeticLocation & location,
           ETime timeOfDay )
{
    switch ( s_criterion )
    {
    case Shaukat:
    default:
        return ComputeShaukat( julianDay, location, timeOfDay );
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

bool 
CheckNext( double julianDay, const GeodeticLocation & location )
{
    return CheckNext( julianDay, location, Evening );
}

//=============================================================================

void 
SetCriterion( ECriterion criterion )
{
    s_criterion = criterion;
}

//-----------------------------------------------------------------------------

ECriterion 
Criterion( )
{
    return s_criterion;
}


//=============================================================================

namespace
{                                                                   //namespace

//.............................................................................

bool 
ComputeShaukat( double julianDay, const GeodeticLocation & location,
                ETime timeOfDay )
{
    /*Adapted from Edward M. Reingold and Nachum Dershowitz,
      "Calendrical Calculations, Millennium Edition", p 205.
      But see Errata at www.calendarists.com.*/
    RiseSet::EEvent event
            = ( timeOfDay == Morning  ?  RiseSet::Rise  :  RiseSet::Set );
    RiseSet::Result sunsetRslt
            = RiseSet::FindNext( julianDay, SolarSystem::Sun,
                                 event, Angle( -4.5, Angle::Degree ),
                                 location );
    if ( sunsetRslt.m_status != RiseSet::OK )
        return true;
    julianDay = sunsetRslt.m_julianDay;
    Equatorial lunarEquatorial = LunarEquatorialPosition( julianDay );
    Angle siderealTime
            = GreenwichMeanSiderealTime( julianDay ) +  location.Longitude();
    Horizontal lunarHorizontal
            = EquatorialToHorizontal( lunarEquatorial, siderealTime,
                                      location.Latitude() );
    return ( (lunarHorizontal.Altitude().Degrees() > 4.1)
             && (LunarArcOfLight( julianDay ).Degrees() > 10.6) );
}

//.............................................................................

}                                                                   //namespace

//=============================================================================

#ifdef DEBUG

bool 
Test( )
{
    //I wish I had some data to test with.
    return true;    
}

#endif //DEBUG


//*****************************************************************************

}                                                   //namespace LunarVisibility

}                                                      //namespace EpsilonDelta

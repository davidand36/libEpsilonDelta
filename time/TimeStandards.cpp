/*
  TimeStandards.cpp
  Copyright (C) 2007 David M. Anderson

  Routines relating various time standards.
*/


#include "TimeStandards.hpp"
#include "Assert.hpp"
#include "Angle.hpp"
#include "Polynomial.hpp"
#include "Epoch.hpp"
#include <cmath>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <cstdio>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


TimeIncrement 
TDB_TT( double julianDay )
{
    //Explanatory Supplement (2.222-1). This is approximate, but "sufficient 
    // in most cases".
    Angle g( (357.53  +  0.9856003 * (julianDay - 2451545.0)),
             Angle::Degree );
    double s = 0.001658 * Sin( g )  +  0.000014 * Sin( 2. * g );
    return TimeIncrement( 0, 0, s );
}

//-----------------------------------------------------------------------------

TimeIncrement 
TCG_TT( double julianDay )
{
    //Explanatory Supplement (2.223-5)
    const double lg = 6.969291E-10 * 86400.;
    double s = lg * (julianDay - 2443144.5);
    return TimeIncrement( 0, 0, s );
}

//-----------------------------------------------------------------------------

TimeIncrement 
TCB_TDB( double julianDay )
{
    //Explanatory Supplement (2.223-2)
    const double lb = 1.550505E-8 * 86400.;
    double s = lb * (julianDay - 2443144.5);
    return TimeIncrement( 0, 0, s );
}

//-----------------------------------------------------------------------------

TimeIncrement 
TT_UT1( double julianDay )
{
    double s = 0.;
    if ( (julianDay < 1578982.0)        //before 390 B.C.
         || (julianDay > 2447162.0) )   // or after 1988 A.D.
    {   //Morrison & Stephenson (1982),
        // cited in Meeus, "Astronomical Algorithms", p. 73.
        double t = Century2000( julianDay );
        s = 102.3  +  t * (123.5  +  t * 32.5);
    }
    else if ( julianDay >= 2415020.0 )  //1900-1987 A.D.
    {   //Smadel & Zech, cited in Meeus, p. 74.
        double t = (julianDay - 2415020.0) / 36525;   //centuries since 1900.0
        static const Polynomial< double > poly( 7, -0.000020, 0.000297,
                0.025184, 0.181133, 0.553040, 0.861938, 0.677066, 0.212591 );
        s = poly( t ) * 86400.0;    //formula is given in days
    }
    else if ( julianDay >= 2378495.0 )  //1800-1899 A.D.
    {   //Smadel & Zech, cited in Meeus, p. 74.
        double t = (julianDay - 2415020.0) / 36525;   //centuries since 1900.0
        static const Polynomial< double > poly( 10, -0.000009, 0.003844,
                0.083563, 0.865736, 4.867575, 15.845535, 31.332267, 38.291999,
                28.316289, 11.636204, 2.043794 );
        s = poly( t ) * 86400.0;    //formula is given in days
    }
    else if ( julianDay >= 2341973.0 )  //1700-1799 A.D.
    {   //Errata and notes to Reingold & Dershowitz
        double t = (julianDay - 2341973.0 ) / 365.25;
        static const Polynomial< double > poly( 3, 8.118780842,
                0.005092142, 0.003336121, 0.0000266484 );
        s = poly( t );
    }
    else if ( julianDay >= 2312753.0 )  //1620-1699 A.D.
    {   //Dershowitz & Reingold, "Calendrical Calculations", p. 144.
        double t = (julianDay - 2305448.0) / 365.25;    //years since 1600.0
        s = 196.58333  +  t * (-4.0675  +  t * 0.0219167);
    }
    else if ( julianDay >= 2067310.0 )  //948-1599 A.D.
    {   //Stephenson & Holden (1986), cited in Meeus, p. 73.
        double t = Century2000( julianDay );
        s = 50.6  +  t * (67.5  +  t * 22.5);
    }
    else    //390 B.C. - 947 A.D.
    {   //Stephenson & Holden (1986), cited in Meeus, p. 73.
        double t = Century2000( julianDay );
        s = 2715.6  +  t * (573.36  +  t * 46.5);
    }
    return TimeIncrement( 0, 0, s );
}

//-----------------------------------------------------------------------------

namespace 
{
//.............................................................................

struct LeapSecond
{
    double julianDay;
    double step;
};

//NOTE: This table must be updated each time a leap second is introduced.
const LeapSecond s_kLeapSeconds[]
    = {
        { 2441499.5,  1. }, //30 Jun 1972
        { 2441683.5,  1. }, //31 Dec 1972
        { 2442048.5,  1. }, //31 Dec 1973
        { 2442413.5,  1. }, //31 Dec 1974
        { 2442778.5,  1. }, //31 Dec 1975
        { 2443144.5,  1. }, //31 Dec 1976
        { 2443509.5,  1. }, //31 Dec 1977
        { 2443874.5,  1. }, //31 Dec 1978
        { 2444239.5,  1. }, //31 Dec 1979
        { 2444786.5,  1. }, //30 Jun 1981
        { 2445151.5,  1. }, //30 Jun 1982
        { 2445516.5,  1. }, //30 Jun 1983
        { 2446247.5,  1. }, //30 Jun 1985
        { 2447161.5,  1. }, //31 Dec 1987
        { 2447892.5,  1. }, //31 Dec 1989
        { 2448257.5,  1. }, //31 Dec 1990
        { 2448804.5,  1. }, //30 Jun 1992
        { 2449169.5,  1. }, //30 Jun 1993
        { 2449534.5,  1. }, //30 Jun 1994
        { 2450083.5,  1. }, //31 Dec 1995
        { 2450630.5,  1. }, //30 Jun 1997
        { 2451179.5,  1. }  //31 Dec 1998
    };
const int s_kNumLeapSeconds
    = sizeof( s_kLeapSeconds ) / sizeof( s_kLeapSeconds[0] );
const double s_kLeapSecondsValidUntil = 2453736.5;

//.............................................................................
} //namespace

//.............................................................................

TimeIncrement 
TAI_UTC( double julianDay )
{
    double s = 10.;
    for ( int i = 0; i < s_kNumLeapSeconds; ++i )
        if ( s_kLeapSeconds[i].julianDay <= julianDay )
            s += s_kLeapSeconds[i].step;
        else
            break;
    return TimeIncrement( 0, 0, s );
}

//-----------------------------------------------------------------------------

TimeIncrement 
TT_UTC( double julianDay )
{
    return TT_TAI( ) + TAI_UTC( julianDay );
}

//-----------------------------------------------------------------------------

TimeIncrement 
TDB_UTC( double julianDay )
{
    return TDB_TT( julianDay ) + TT_UTC( julianDay );
}

//-----------------------------------------------------------------------------

TimeIncrement 
TDB_UT( double julianDay )
{
    if ( (julianDay < 2441317.5) || (julianDay > s_kLeapSecondsValidUntil) )
        return TDB_TT( julianDay ) + TT_UT1( julianDay );
    else
        return TDB_UTC( julianDay );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

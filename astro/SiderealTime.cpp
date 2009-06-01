/*
  SiderealTime.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for computing sidereal time.
*/


#include "SiderealTime.hpp"
#include "AngleHMS.hpp"
#include "Epoch.hpp"
#include "Nutation.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "JPLEphemeris.hpp"
#include "Obliquity.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Angle 
GreenwichMeanSiderealTime( double julianDay )
{
    //"Explanatory Supplement" (3.352-1)
    double t = Century2000( julianDay );
    double s = 67310.54841  +  t * (8640184.812866  +  t * (0.093104
                                                            +  t * -6.2e-6));
    s += (t * 876600.0) * 3600.;
    s /= 86400.;
    Angle angle( s, Angle::Cycle );
    angle.NormalizePositive( );
    return angle;
}

//-----------------------------------------------------------------------------

Angle 
GreenwichApparentSiderealTime( double julianDay,
                               Angle nutLongitude, Angle meanObliquity )
{
    return GreenwichMeanSiderealTime( julianDay )
            +  nutLongitude * meanObliquity.Cos( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Angle 
GreenwichApparentSiderealTime( double julianDay,
                               const Nutation & nutation,
                               Angle meanObliquity )
{
    return GreenwichApparentSiderealTime( julianDay, nutation.NutLongitude(),
                                          meanObliquity );
}

//-----------------------------------------------------------------------------

#ifdef DEBUG

bool 
TestSiderealTime( JPLEphemeris & ephem )
{
    bool ok = true;
    cout << "Testing Sidereal Time" << endl;

    //Jean Meeus, "Astronomical Algorithms", p. 84
    double jd = 2446895.5;
    cout << "GreenwichMeanSiderealTime(" << jd << ")" << endl;
    Angle stAngle = GreenwichMeanSiderealTime( jd );
    AngleHMS stHMS( stAngle );
    TESTCHECK( stHMS.Hours(), 13, &ok );
    TESTCHECK( stHMS.Minutes(), 10, &ok );
    TESTCHECKF( stHMS.Seconds(), 46.3668, &ok );
    cout << "GreenwichApparentSiderealTime" << endl;
    Nutation nutation;
    bool ephRslt = ephem.GetNutation( jd, &nutation );
    Assert( ephRslt );
    Angle meanObliquity = MeanObliquity( jd );
    stAngle = GreenwichApparentSiderealTime( jd, nutation, meanObliquity );
    stHMS.Set( stAngle );
    TESTCHECK( stHMS.Hours(), 13, &ok );
    TESTCHECK( stHMS.Minutes(), 10, &ok );
    TESTCHECKF( stHMS.Seconds(), 46.1351, &ok );

    jd = 2446896.30625;
    cout << "GreenwichMeanSiderealTime(" << jd << ")" << endl;
    stAngle = GreenwichMeanSiderealTime( jd );
    stHMS.Set( stAngle );
    TESTCHECK( stHMS.Hours(), 8, &ok );
    TESTCHECK( stHMS.Minutes(), 34, &ok );
    TESTCHECKF( stHMS.Seconds(), 57.0896, &ok );

    //Paul J. Heafner, "Fundamental Ephemeris Computations", p. 23
    jd = 2447399.5;
    cout << "GreenwichMeanSiderealTime(" << jd << ")" << endl;
    stAngle = GreenwichMeanSiderealTime( jd );
    stHMS.Set( stAngle );
    TESTCHECK( stHMS.Hours(), 22, &ok );
    TESTCHECK( stHMS.Minutes(), 17, &ok );
    TESTCHECKF( stHMS.Seconds(), 50.2719, &ok );
    cout << "GreenwichApparentSiderealTime" << endl;
    ephRslt = ephem.GetNutation( jd, &nutation );
    Assert( ephRslt );
    meanObliquity = MeanObliquity( jd );
    stAngle = GreenwichApparentSiderealTime( jd, nutation, meanObliquity );
    stHMS.Set( stAngle );
    TESTCHECK( stHMS.Hours(), 22, &ok );
    TESTCHECK( stHMS.Minutes(), 17, &ok );
    TESTCHECKF( stHMS.Seconds(), 50.6171, &ok );

    if ( ok )
        cout << "Sidereal Time PASSED." << endl << endl;
    else
        cout << "Sidereal Time FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

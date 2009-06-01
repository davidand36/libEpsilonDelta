/*
  Seasons.cpp
  Copyright (C) 2007 David M. Anderson

  Determine the time (UTC) of equinoxes and solstices.
*/


#include "Seasons.hpp"
#include "AstroPhenomena.hpp"
#include "Angle.hpp"
#include "RootFinder.hpp"
#include "TimeStandards.hpp"
#include "DivMod.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
#include "DateTime.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SeasonFunc
{
public:
    SeasonFunc( Angle seasonAngle );
    double operator()( double julianDay );

private:
    Angle m_seasonAngle;
};

//-----------------------------------------------------------------------------

SeasonFunc::SeasonFunc( Angle seasonAngle )
    :   m_seasonAngle( seasonAngle )
{
}

//-----------------------------------------------------------------------------

double 
SeasonFunc::operator()( double julianDay )
{
    Angle solarLong = SolarLongitude( julianDay );
    Angle diff = solarLong - m_seasonAngle;
    return diff.Radians();
}


//*****************************************************************************


double 
Seasons::FindNext( double julianDay, ESeason season )
{
    const Angle seasonAngles[4]
            = { Angle( 0 ), Angle( M_PI / 2. ),
                Angle( M_PI ), Angle( M_PI * 3. / 2. ) };
    const double season2000[4]
            = { 2451623.8159722, 2451716.575,
                2451810.22708333, 2451900.067361111 };
    const double tropicalYear = 365.2421896698;
    double offset = ModRP( (season2000[ season ] - julianDay), tropicalYear );
    double estimate = julianDay + offset;
    double lowEst = estimate - 0.01;
    double highEst = estimate + 0.01;
    SeasonFunc seasonFunc( seasonAngles[ season ] );
#ifdef DEBUG
    bool brcktRslt =
#endif
            RootFinder::BracketRoot( seasonFunc, &lowEst, &highEst );
    Assert( brcktRslt );
    double seasonJD;
#ifdef DEBUG
    bool rootRslt =
#endif
            RootFinder::Brent( seasonFunc, &seasonJD, lowEst, highEst, 1.e-4 );
    Assert( rootRslt );
    double ut_tdb = - TDB_UT( seasonJD ).Days();
    return  (seasonJD + ut_tdb);
}

//=============================================================================

#ifdef DEBUG

bool 
Seasons::Test( )
{
    bool ok = true;
    cout << "Testing Seasons" << endl;

    double jan2003 = DateTime( 1, January, 2003, 0, 0 ).JulianDay( );
    TESTCHECKFE( FindNext( jan2003, SpringEquinox ),
                 DateTime( 21, March, 2003, 1, 0 ).JulianDay(), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2003, SummerSolstice ),
                 DateTime( 21, June, 2003, 19, 10 ).JulianDay(), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2003, AutumnalEquinox ),
                 DateTime( 23, September, 2003, 10, 47 ).JulianDay(), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2003, WinterSolstice ),
                 DateTime( 22, December, 2003, 7, 4 ).JulianDay(), &ok,
                 2.e-10 );
    double jan2004 = DateTime( 1, January, 2004, 0, 0 ).JulianDay( );
    TESTCHECKFE( FindNext( jan2004, SpringEquinox ),
                 DateTime( 20, March, 2004, 6, 49 ).JulianDay(), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2004, SummerSolstice ),
                 DateTime( 21, June, 2004, 0, 57 ).JulianDay(), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2004, AutumnalEquinox ),
                 DateTime( 22, September, 2004, 16, 30 ).JulianDay(), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2004, WinterSolstice ),
                 DateTime( 21, December, 2004, 12, 42 ).JulianDay(), &ok,
                 2.e-10 );

    if ( ok )
        cout << "Seasons PASSED." << endl << endl;
    else
        cout << "Seasons FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

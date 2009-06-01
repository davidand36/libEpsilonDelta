/*
  MoonPhases.cpp
  Copyright (C) 2007 David M. Anderson

  Routine to determine the Julian day of the desired lunar phase on or after
  the specified Julian day.
*/


#include "MoonPhases.hpp"
#include "AstroPhenomena.hpp"
#include "RootFinder.hpp"
#include "TimeStandards.hpp"
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


class MoonPhaseFunc
{
public:
    MoonPhaseFunc( Angle phaseAngle );
    double operator()( double julianDay );

private:
    Angle m_phaseAngle;
};

//-----------------------------------------------------------------------------

MoonPhaseFunc::MoonPhaseFunc( Angle phaseAngle )
    :   m_phaseAngle( phaseAngle )
{
}

//-----------------------------------------------------------------------------

double 
MoonPhaseFunc::operator()( double julianDay )
{
    Angle phase = LunarPhase( julianDay );
    Angle diff = m_phaseAngle - phase;
    return diff.Radians();
}


//*****************************************************************************

double 
MoonPhases::FindNext( double julianDay, Angle phaseAngle )
{
    const double synodicMonth = 29.5305888531;
    Angle lunarPhase = LunarPhase( julianDay );
    Angle phaseDiff = phaseAngle - lunarPhase;
    phaseDiff.NormalizePositive( );
    double offset = phaseDiff.Cycles() * synodicMonth;
    double estimate = julianDay + offset;
    double lowEst = estimate - 0.5;
    double highEst = estimate + 0.5;
    MoonPhaseFunc phaseFunc( phaseAngle );
#ifdef DEBUG
    bool brcktRslt =
#endif
            RootFinder::BracketRoot( phaseFunc, &lowEst, &highEst );
    Assert( brcktRslt );
    double phaseJD;
#ifdef DEBUG
    bool rootRslt =
#endif
            RootFinder::Brent( phaseFunc, &phaseJD, lowEst, highEst, 1.e-4 );
    Assert( rootRslt );
    double ut_tdb = - TDB_UT( phaseJD ).Days();
    return  (phaseJD + ut_tdb);
}

//-----------------------------------------------------------------------------

double 
MoonPhases::FindNext( double julianDay, EPhase phase )
{
    const Angle phaseAngles[4]
            = { Angle( 0 ), Angle( M_PI / 2. ),
                Angle( M_PI ), Angle( M_PI * 3. / 2. ) };
    return  FindNext( julianDay, phaseAngles[ phase ] );
}

//=============================================================================

#ifdef DEBUG

bool 
MoonPhases::Test( )
{
    bool ok = true;
    cout << "Testing MoonPhases" << endl;

    double jan2000 = DateTime( 1, January, 2000, 0, 0 ).JulianDay( );
    TESTCHECKFE( FindNext( jan2000, New ),
                 DateTime( 6, January, 2000, 18, 14 ).JulianDay( ), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2000, FirstQuarter ),
                 DateTime( 14, January, 2000, 13, 34 ).JulianDay( ), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2000, Full ),
                 DateTime( 21, January, 2000, 4, 40 ).JulianDay( ), &ok,
                 2.e-10 );
    TESTCHECKFE( FindNext( jan2000, LastQuarter ),
                 DateTime( 28, January, 2000, 7, 57 ).JulianDay( ), &ok,
                 2.e-10 );

    if ( ok )
        cout << "MoonPhases PASSED." << endl << endl;
    else
        cout << "MoonPhases FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  EquationOfTime.cpp
  Copyright (C) 2007 David M. Anderson

  Routine for computing the "equation of time", the difference between the
  apparent and mean solar time.
*/


#include "EquationOfTime.hpp"
#include "JPLEphemeris.hpp"
#include "AstroPhenomena.hpp"
#include "CoordinateReduction.hpp"
#include "Precession.hpp"
#include "Nutation.hpp"
#include "Obliquity.hpp"
#include "Equatorial.hpp"
#include "Time.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
using namespace std;
using namespace std::tr1;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


TimeIncrement 
EquationOfTime( double julianDay )
{
    shared_ptr< JPLEphemeris > spEphem
            = JPLEphemeris::GetEphemeris( julianDay );
    if ( ! spEphem )
        return TimeIncrement( 0 );
    Angle meanSolarLong = MeanSolarLongitude( julianDay );
    Point3D earthBarycentric;
    Vector3D earthBarycentricVelocity;
#ifdef DEBUG
    bool earthRslt =
#endif
            GetEarthBarycentric( julianDay, &earthBarycentric,
                                 &earthBarycentricVelocity, spEphem );
    Assert( earthRslt );
    Matrix3D precessionMatrix = Precession( julianDay ).Matrix( );
    Nutation nutation( 0., 0. );
    if ( spEphem->NutationAvailable() )
    {
#ifdef DEBUG
        bool nutRslt =
#endif
                spEphem->GetNutation( julianDay, &nutation );
        Assert( nutRslt );
    }
    Angle meanObliquity = MeanObliquity( julianDay );
    Angle trueObliquity = TrueObliquity( meanObliquity, nutation );
    Matrix3D nutAndPrecMatrix = nutation.Matrix( meanObliquity )
            * precessionMatrix;
    JPLBarycentricEphemeris sunEphem( spEphem, JPLEphemeris::Sun );
    Point3D sunPos = GetSunApparentPlace( julianDay, sunEphem,
                                          earthBarycentric,
                                          earthBarycentricVelocity,
                                          nutAndPrecMatrix );
    Equatorial sunEquatorial( sunPos );
    Angle eotAngle = meanSolarLong  -  Angle( 0.0057183, Angle::Degree )
            -  sunEquatorial.RightAscension()
            +  nutation.NutLongitude() * Cos( trueObliquity );
    return TimeIncrement( eotAngle.Cycles() );
}

//=============================================================================

#ifdef DEBUG

bool 
TestEquationOfTime( )
{
    bool ok = true;
    cout << "Testing EquationOfTime" << endl;

    int prec = cout.precision( 12 );

    double jd = 2448908.5;
    cout << "jd = " << jd << endl;
    TESTCHECKFE( EquationOfTime( jd ).Days(),
                 TimeIncrement( 0, 13, 42.6 ).Days(), &ok, 1e-4 );

    cout.precision( prec );

    if ( ok )
        cout << "EquationOfTime PASSED." << endl << endl;
    else
        cout << "EquationOfTime FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

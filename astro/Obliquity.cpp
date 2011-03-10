/*
  Obliquity.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for computing the obliquity of the ecliptic.
*/


#include "Obliquity.hpp"
#include "AngleDMS.hpp"
#include "Epoch.hpp"
#include "Polynomial.hpp"
#ifdef DEBUG
#include <iostream>
#include "TestCheck.hpp"
#include "JPLEphemeris.hpp"
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Angle 
MeanObliquity( double julianDay )
{
    double t = Century2000( julianDay );
    if ( (t >= -10.) && (t <= 10.) )
    {
        //Explanatory Supplement (3222-1)
        double s = t * (-46.8150  +  t * (-0.00059  +  t * 0.001813));
        return Angle( AngleDMS( 23, 26, 21.448 ) )
                +  Angle( s, Angle::ArcSecond );
    }
    else
    {
        //J. Laskar, "Astronomy and Astrophysics", Vol. 157 (1986), p. 68,
        //cited in Jean Meeus, "Astronomical Algorithms", p. 135.
        static const Polynomial< double > oblPoly( 10, 0., 4680.93, -1.55,
                1999.25, -51.38, -249.67, -39.05, 7.12, 27.87, 5.79, 2.45 );
        double u = t / 100.;
        double s = oblPoly( u );
        return Angle( AngleDMS( 23, 26, 21.448 ) )
                +  Angle( s, Angle::ArcSecond );
    }
}

//=============================================================================

Angle 
TrueObliquity( double julianDay, Angle nutObliquity )
{
    return  TrueObliquity( MeanObliquity( julianDay ), nutObliquity );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Angle 
TrueObliquity( double julianDay, const Nutation & nutation )
{
    return  TrueObliquity( julianDay, nutation.NutObliquity( ) );
}

//=============================================================================

#ifdef DEBUG

bool 
TestObliquity( shared_ptr< JPLEphemeris > spEphem )
{
    bool ok = true;
    cout << "Testing Obliquity" << endl;

    //Meeus, p. 136
    double jd = 2446895.5;
    cout << "MeanObliquity(" << jd << ")" << endl;
    Angle meanObliq = MeanObliquity( jd );
    AngleDMS meanObliqDMS( meanObliq );
    TESTCHECK( meanObliqDMS.Degrees(), 23, &ok );
    TESTCHECK( meanObliqDMS.Minutes(), 26, &ok );
    TESTCHECKFE( meanObliqDMS.Seconds(), 27.407, &ok, 4.e-5 );
    Nutation nutation;
    bool ephRslt = spEphem->GetNutation( jd, &nutation );
    Assert( ephRslt );
    Angle trueObliq = TrueObliquity( jd, nutation );
    AngleDMS trueObliqDMS( trueObliq );
    TESTCHECK( trueObliqDMS.Degrees(), 23, &ok );
    TESTCHECK( trueObliqDMS.Minutes(), 26, &ok );
    TESTCHECKFE( trueObliqDMS.Seconds(), 36.850, &ok, 3.e-5 );

    if ( ok )
        cout << "Obliquity PASSED." << endl << endl;
    else
        cout << "Obliquity FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

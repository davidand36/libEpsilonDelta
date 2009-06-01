/*
  Epoch.cpp
  Copyright (C) 2007 David M. Anderson

  Epoch class: Denotes a particular instant used as a reference, mainly in
  astronomical systems.
*/


#include "Epoch.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "DateTime.hpp"
#include <iostream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


const double J2000 = Epoch( 2000., Epoch::Julian ).JulianDay( );
const double B1950 = Epoch( 1950., Epoch::Besselian ).JulianDay( );


//=============================================================================


#ifdef DEBUG

bool 
Epoch::Test( )
{
    bool ok = true;
    cout << "Testing Epoch" << endl;

    //Tests from Jean Meeus, "Astronomical Algorithms", p. 125.

    cout << "Epoch( 1900., Besselian )" << endl;
    Epoch b1900( 1900., Epoch::Besselian );
    TESTCHECK( b1900.Year(), 1900., &ok );
    TESTCHECK( b1900.System(), Epoch::Besselian, &ok );
    TESTCHECKF( b1900.JulianDay(), 2415020.3135, &ok );

    cout << "Epoch( 2050., Julian )" << endl;
    Epoch b2050( 2050., Epoch::Julian );
    TESTCHECK( b2050.Year(), 2050., &ok );
    TESTCHECK( b2050.System(), Epoch::Julian, &ok );
    TESTCHECK( b2050.JulianDay(), 2469807.5, &ok );
    
    TESTCHECK( J2000, 2451545., &ok );
    TESTCHECKF( B1950, 2433282.4235, &ok );

    DateTime dt2100( 1, January, 2100, 12, 0, 0. );
    TESTCHECK( Century2000( dt2100.JulianDay() ), 1.0, &ok );

    if ( ok )
        cout << "Epoch PASSED." << endl << endl;
    else
        cout << "Epoch FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

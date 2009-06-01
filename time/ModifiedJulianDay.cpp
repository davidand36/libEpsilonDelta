/*
  ModifiedJulianDay.cpp
  Copyright (C) 2007 David M. Anderson

  Routines to convert between regular and Modified Julian Day.
*/


#include "ModifiedJulianDay.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "DateTime.hpp"
#include <string>
#include <iostream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestMJD( )
{
    bool ok = true;
    cout << "Testing ModifiedJulianDay" << endl;

    cout << "DateTime( 1, 1, 2003, 0, 0, 0 )" << endl;
    DateTime dt2003( 1, 1, 2003, 0, 0, 0 );
    double jd = dt2003.JulianDay();
    cout << "Julian Day=" << jd << endl;
    TESTCHECK( JulianDayToMJD( jd ), 52640.0, &ok );
    TESTCHECK( MJDToJulianDay( 52640.0 ), jd, &ok );
    
    if ( ok )
        cout << "ModifiedJulianDay PASSED." << endl << endl;
    else
        cout << "ModifiedJulianDay FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

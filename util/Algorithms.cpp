/*
  Algorithms.cpp
  Copyright (C) 2007 David M. Anderson

  Some basic algorithms not in the STL.
*/


#include "Algorithms.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <string>
#include <map>
#include <iostream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestAlgorithms( )
{
    bool ok = true;
    cout << "Testing Algorithms" << endl;

    int numbers[4] = { 0, 1, 2, 3 };
    string numberNames[4] = { "zero", "one", "two", "three" };
    TESTCHECK( *(MinIter( numbers, numbers + 4 )), 0, &ok );
    TESTCHECK( *(MaxIter( numbers, numbers + 4 )), 3, &ok );
    TESTCHECK( *(MinIter( numberNames, numberNames + 4 )), string( "one" ),
               &ok );
    TESTCHECK( *(MaxIter( numberNames, numberNames + 4 )), string( "zero" ),
               &ok );
    TESTCHECK( *(MaxIter( numberNames + 1, numberNames + 4 )), string( "two" ),
               &ok );
    TESTCHECK( MinIndex( numbers, numbers + 4 ), 0, &ok );
    TESTCHECK( MaxIndex( numbers, numbers + 4 ), 3, &ok );
    TESTCHECK( MinIndex( numberNames, numberNames + 4 ), 1, &ok );
    TESTCHECK( MaxIndex( numberNames, numberNames + 4 ), 0, &ok );
    TESTCHECK( MaxIndex( numberNames + 1, numberNames + 4 ), 1, &ok );

    if ( ok )
        cout << "Algorithms PASSED." << endl << endl;
    else
        cout << "Algorithms FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

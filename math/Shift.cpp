/*
  Shift.cpp
  Copyright (C) 2009 David M. Anderson

  Template functions for arithmetic shift operations.
*/


#include "Shift.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestShift( )
{
    bool ok = true;
    cout << "Testing Shift" << endl;

    TESTCHECK( (ShiftLeft< uint32_t, 8 >( 0xA )), 0xA00, &ok );
    TESTCHECK( (ShiftLeft< uint32_t, -4 >( 0xA0 )), 0xA, &ok );
    TESTCHECK( (ShiftRight< uint16_t, 4 >( 0xC0 )), (uint16_t)0xC, &ok );
    TESTCHECK( (ShiftRight< uint16_t, -8 >( 0xC )), (uint16_t)0xC00, &ok );

    if ( ok )
        cout << "Shift PASSED." << endl << endl;
    else
        cout << "Shift FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

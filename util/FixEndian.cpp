/*
  FixEndian.cpp
  Copyright (C) 2007 David M. Anderson

  Routines for swapping bytes to correct wrong-endian data.
*/


#include "FixEndian.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

#ifdef LITTLE_ENDIAN
#define FixRightEndian  FixLittleEndian
#define FixWrongEndian  FixBigEndian
#else
#define FixRightEndian  FixBigEndian
#define FixWrongEndian  FixLittleEndian
#endif

//.............................................................................

bool 
TestFixEndian( )
{
    bool ok = true;
    cout << "Testing FixEndian" << endl;

    int8_t i8 = static_cast<int8_t>( 0x12 );
    cout << "int8_t " << i8 << endl;
    FixRightEndian( &i8 );
    TESTCHECK( i8, 0x12, &ok );
    FixWrongEndian( &i8 );
    TESTCHECK( i8, 0x12, &ok );

    uint16_t u16 = 0x1234;
    cout << "uint16_t " << u16 << endl;
    FixRightEndian( &u16 );
    TESTCHECK( u16, 0x1234, &ok );
    FixWrongEndian( &u16 );
    TESTCHECK( u16, 0x3412, &ok );

    int32_t i32 = 0x12345678;
    cout << "int32_t " << i32 << endl;
    FixRightEndian( &i32 );
    TESTCHECK( i32, 0x12345678, &ok );
    FixWrongEndian( &i32 );
    TESTCHECK( i32, 0x78563412, &ok );

    unsigned int u = 0x78563412U;
    cout << "unsigned int " << u << endl;
    FixRightEndian( &u );
    TESTCHECK( u, 0x78563412U, &ok );
    FixWrongEndian( &u );
    TESTCHECK( u, 0x12345678U, &ok );

    uint64_t u64 = 0x123456789ABCDEF0ULL;
    cout << "uint64_t " << u64 << endl;
    FixRightEndian( &u64 );
    TESTCHECK( u64, 0x123456789ABCDEF0ULL, &ok );
    FixWrongEndian( &u64 );
    TESTCHECK( u64, 0xF0DEBC9A78563412ULL, &ok );

    if ( ok )
        cout << "FixEndian PASSED." << endl << endl;
    else
        cout << "FixEndian FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  DivMod.cpp
  Copyright (C) 2007 David M. Anderson

  Utility functions for division and modulo with dependable signs.
*/


#include "DivMod.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <iostream>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

#ifdef DEBUG

bool
TestDivMod( )
{
    bool ok = true;
    cout << "Testing DivMod" << endl << endl;

#if defined(NATIVE_DIVMODP)
    cout << "NATIVE_DIVMODP" << endl << endl;
#elif defined(NATIVE_DIVMODF)
    cout << "NATIVE_DIVMODF" << endl << endl;
#elif defined(NATIVE_DIVMODC)
    cout << "NATIVE_DIVMODC" << endl << endl;
#elif defined(NATIVE_DIVMODA)
    cout << "NATIVE_DIVMODA" << endl << endl;
#endif

    long int a = 7;
    long int b = 3;
    long int quot, rem;
    cout << "DivModP( " << a << ", " << b << " ) :" << endl;
    DivModP( a, b, &quot, &rem );
    TESTCHECK( quot, 2L, &ok );
    TESTCHECK( rem, 1L, &ok );
    a = -7;
    b = 3;
    cout << "DivModP( " << a << ", " << b << " ) :" << endl;
    DivModP( a, b, &quot, &rem );
    TESTCHECK( quot, -3L, &ok );
    TESTCHECK( rem, 2L, &ok );
    a = 7;
    b = -3;
    cout << "DivModP( " << a << ", " << b << " ) :" << endl;
    DivModP( a, b, &quot, &rem );
    TESTCHECK( quot, -2L, &ok );
    TESTCHECK( rem, 1L, &ok );
    a = -7;
    b = -3;
    cout << "DivModP( " << a << ", " << b << " ) :" << endl;
    DivModP( a, b, &quot, &rem );
    TESTCHECK( quot, 3L, &ok );
    TESTCHECK( rem, 2L, &ok );

    TESTCHECK( DivP( 7L, 3L ), 2L, &ok );
    TESTCHECK( DivP( -7L, 3L ), -3L, &ok );
    TESTCHECK( DivP( 7L, -3L ), -2L, &ok );
    TESTCHECK( DivP( -7L, -3L ), 3L, &ok );

    TESTCHECK( ModP( 7L, 3L ), 1L, &ok );
    TESTCHECK( ModP( -7L, 3L ), 2L, &ok );
    TESTCHECK( ModP( 7L, -3L ), 1L, &ok );
    TESTCHECK( ModP( -7L, -3L ), 2L, &ok );

    long double fa = 0.7L;
    long double fb = 0.3L;
    long double fquot, frem;
    cout << "DivModRP( " << fa << ", " << fb << " ) :" << endl;
    DivModRP( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 2.0L, &ok );
    TESTCHECKF( frem, 0.1L, &ok );
    fa = -0.7L;
    fb = 0.3L;
    cout << "DivModRP( " << fa << ", " << fb << " ) :" << endl;
    DivModRP( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -3.0L, &ok );
    TESTCHECKF( frem, 0.2L, &ok );
    fa = 0.7L;
    fb = -0.3L;
    cout << "DivModRP( " << fa << ", " << fb << " ) :" << endl;
    DivModRP( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -2.0L, &ok );
    TESTCHECKF( frem, 0.1L, &ok );
    fa = -0.7L;
    fb = -0.3L;
    cout << "DivModRP( " << fa << ", " << fb << " ) :" << endl;
    DivModRP( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 3.0L, &ok );
    TESTCHECKF( frem, 0.2L, &ok );

    TESTCHECKF( DivRP( 0.7, 0.3 ), 2.0, &ok );
    TESTCHECKF( DivRP( -0.7, 0.3 ), -3.0, &ok );
    TESTCHECKF( DivRP( 0.7, -0.3 ), -2.0, &ok );
    TESTCHECKF( DivRP( -0.7, -0.3 ), 3.0, &ok );

    TESTCHECKF( ModRP( 0.7, 0.3 ), 0.1, &ok );
    TESTCHECKF( ModRP( -0.7, 0.3 ), 0.2, &ok );
    TESTCHECKF( ModRP( 0.7, -0.3 ), 0.1, &ok );
    TESTCHECKF( ModRP( -0.7, -0.3 ), 0.2, &ok );

    a = 7;
    b = 3;
    cout << "DivModF( " << a << ", " << b << " ) :" << endl;
    DivModF( a, b, &quot, &rem );
    TESTCHECK( quot, 2L, &ok );
    TESTCHECK( rem, 1L, &ok );
    a = -7;
    b = 3;
    cout << "DivModF( " << a << ", " << b << " ) :" << endl;
    DivModF( a, b, &quot, &rem );
    TESTCHECK( quot, -3L, &ok );
    TESTCHECK( rem, 2L, &ok );
    a = 7;
    b = -3;
    cout << "DivModF( " << a << ", " << b << " ) :" << endl;
    DivModF( a, b, &quot, &rem );
    TESTCHECK( quot, -3L, &ok );
    TESTCHECK( rem, -2L, &ok );
    a = -7;
    b = -3;
    cout << "DivModF( " << a << ", " << b << " ) :" << endl;
    DivModF( a, b, &quot, &rem );
    TESTCHECK( quot, 2L, &ok );
    TESTCHECK( rem, -1L, &ok );

    TESTCHECK( DivF( 7L, 3L ), 2L, &ok );
    TESTCHECK( DivF( -7L, 3L ), -3L, &ok );
    TESTCHECK( DivF( 7L, -3L ), -3L, &ok );
    TESTCHECK( DivF( -7L, -3L ), 2L, &ok );

    TESTCHECK( ModF( 7L, 3L ), 1L, &ok );
    TESTCHECK( ModF( -7L, 3L ), 2L, &ok );
    TESTCHECK( ModF( 7L, -3L ), -2L, &ok );
    TESTCHECK( ModF( -7L, -3L ), -1L, &ok );

    fa = 0.7L;
    fb = 0.3L;
    cout << "DivModRF( " << fa << ", " << fb << " ) :" << endl;
    DivModRF( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 2.0L, &ok );
    TESTCHECKF( frem, 0.1L, &ok );
    fa = -0.7L;
    fb = 0.3L;
    cout << "DivModRF( " << fa << ", " << fb << " ) :" << endl;
    DivModRF( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -3.0L, &ok );
    TESTCHECKF( frem, 0.2L, &ok );
    fa = 0.7L;
    fb = -0.3L;
    cout << "DivModRF( " << fa << ", " << fb << " ) :" << endl;
    DivModRF( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -3.0L, &ok );
    TESTCHECKF( frem, -0.2L, &ok );
    fa = -0.7L;
    fb = -0.3L;
    cout << "DivModRF( " << fa << ", " << fb << " ) :" << endl;
    DivModRF( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 2.0L, &ok );
    TESTCHECKF( frem, -0.1L, &ok );

    TESTCHECKF( DivRF( 0.7, 0.3 ), 2.0, &ok );
    TESTCHECKF( DivRF( -0.7, 0.3 ), -3.0, &ok );
    TESTCHECKF( DivRF( 0.7, -0.3 ), -3.0, &ok );
    TESTCHECKF( DivRF( -0.7, -0.3 ), 2.0, &ok );

    TESTCHECKF( ModRF( 0.7, 0.3 ), 0.1, &ok );
    TESTCHECKF( ModRF( -0.7, 0.3 ), 0.2, &ok );
    TESTCHECKF( ModRF( 0.7, -0.3 ), -0.2, &ok );
    TESTCHECKF( ModRF( -0.7, -0.3 ), -0.1, &ok );

    a = 7;
    b = 3;
    cout << "DivModC( " << a << ", " << b << " ) :" << endl;
    DivModC( a, b, &quot, &rem );
    TESTCHECK( quot, 3L, &ok );
    TESTCHECK( rem, -2L, &ok );
    a = -7;
    b = 3;
    cout << "DivModC( " << a << ", " << b << " ) :" << endl;
    DivModC( a, b, &quot, &rem );
    TESTCHECK( quot, -2L, &ok );
    TESTCHECK( rem, -1L, &ok );
    a = 7;
    b = -3;
    cout << "DivModC( " << a << ", " << b << " ) :" << endl;
    DivModC( a, b, &quot, &rem );
    TESTCHECK( quot, -2L, &ok );
    TESTCHECK( rem, 1L, &ok );
    a = -7;
    b = -3;
    cout << "DivModC( " << a << ", " << b << " ) :" << endl;
    DivModC( a, b, &quot, &rem );
    TESTCHECK( quot, 3L, &ok );
    TESTCHECK( rem, 2L, &ok );

    TESTCHECK( DivC( 7L, 3L ), 3L, &ok );
    TESTCHECK( DivC( -7L, 3L ), -2L, &ok );
    TESTCHECK( DivC( 7L, -3L ), -2L, &ok );
    TESTCHECK( DivC( -7L, -3L ), 3L, &ok );

    TESTCHECK( ModC( 7L, 3L ), -2L, &ok );
    TESTCHECK( ModC( -7L, 3L ), -1L, &ok );
    TESTCHECK( ModC( 7L, -3L ), 1L, &ok );
    TESTCHECK( ModC( -7L, -3L ), 2L, &ok );

    fa = 0.7L;
    fb = 0.3L;
    cout << "DivModRC( " << fa << ", " << fb << " ) :" << endl;
    DivModRC( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 3.0L, &ok );
    TESTCHECKF( frem, -0.2L, &ok );
    fa = -0.7L;
    fb = 0.3L;
    cout << "DivModRC( " << fa << ", " << fb << " ) :" << endl;
    DivModRC( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -2.0L, &ok );
    TESTCHECKF( frem, -0.1L, &ok );
    fa = 0.7L;
    fb = -0.3L;
    cout << "DivModRC( " << fa << ", " << fb << " ) :" << endl;
    DivModRC( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -2.0L, &ok );
    TESTCHECKF( frem, 0.1L, &ok );
    fa = -0.7L;
    fb = -0.3L;
    cout << "DivModRC( " << fa << ", " << fb << " ) :" << endl;
    DivModRC( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 3.0L, &ok );
    TESTCHECKF( frem, 0.2L, &ok );

    TESTCHECKF( DivRC( 0.7, 0.3 ), 3.0, &ok );
    TESTCHECKF( DivRC( -0.7, 0.3 ), -2.0, &ok );
    TESTCHECKF( DivRC( 0.7, -0.3 ), -2.0, &ok );
    TESTCHECKF( DivRC( -0.7, -0.3 ), 3.0, &ok );

    TESTCHECKF( ModRC( 0.7, 0.3 ), -0.2, &ok );
    TESTCHECKF( ModRC( -0.7, 0.3 ), -0.1, &ok );
    TESTCHECKF( ModRC( 0.7, -0.3 ), 0.1, &ok );
    TESTCHECKF( ModRC( -0.7, -0.3 ), 0.2, &ok );

    a = 7;
    b = 3;
    cout << "DivModA( " << a << ", " << b << " ) :" << endl;
    DivModA( a, b, &quot, &rem );
    TESTCHECK( quot, 2L, &ok );
    TESTCHECK( rem, 1L, &ok );
    a = -7;
    b = 3;
    cout << "DivModA( " << a << ", " << b << " ) :" << endl;
    DivModA( a, b, &quot, &rem );
    TESTCHECK( quot, -2L, &ok );
    TESTCHECK( rem, -1L, &ok );
    a = 7;
    b = -3;
    cout << "DivModA( " << a << ", " << b << " ) :" << endl;
    DivModA( a, b, &quot, &rem );
    TESTCHECK( quot, -2L, &ok );
    TESTCHECK( rem, 1L, &ok );
    a = -7;
    b = -3;
    cout << "DivModA( " << a << ", " << b << " ) :" << endl;
    DivModA( a, b, &quot, &rem );
    TESTCHECK( quot, 2L, &ok );
    TESTCHECK( rem, -1L, &ok );

    TESTCHECK( DivA( 7L, 3L ), 2L, &ok );
    TESTCHECK( DivA( -7L, 3L ), -2L, &ok );
    TESTCHECK( DivA( 7L, -3L ), -2L, &ok );
    TESTCHECK( DivA( -7L, -3L ), 2L, &ok );

    TESTCHECK( ModA( 7L, 3L ), 1L, &ok );
    TESTCHECK( ModA( -7L, 3L ), -1L, &ok );
    TESTCHECK( ModA( 7L, -3L ), 1L, &ok );
    TESTCHECK( ModA( -7L, -3L ), -1L, &ok );

    fa = 0.7L;
    fb = 0.3L;
    cout << "DivModRA( " << fa << ", " << fb << " ) :" << endl;
    DivModRA( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 2.0L, &ok );
    TESTCHECKF( frem, 0.1L, &ok );
    fa = -0.7L;
    fb = 0.3L;
    cout << "DivModRA( " << fa << ", " << fb << " ) :" << endl;
    DivModRA( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -2.0L, &ok );
    TESTCHECKF( frem, -0.1L, &ok );
    fa = 0.7L;
    fb = -0.3L;
    cout << "DivModRA( " << fa << ", " << fb << " ) :" << endl;
    DivModRA( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, -2.0L, &ok );
    TESTCHECKF( frem, 0.1L, &ok );
    fa = -0.7L;
    fb = -0.3L;
    cout << "DivModRA( " << fa << ", " << fb << " ) :" << endl;
    DivModRA( fa, fb, &fquot, &frem );
    TESTCHECKF( fquot, 2.0L, &ok );
    TESTCHECKF( frem, -0.1L, &ok );

    TESTCHECKF( DivRA( 0.7, 0.3 ), 2.0, &ok );
    TESTCHECKF( DivRA( -0.7, 0.3 ), -2.0, &ok );
    TESTCHECKF( DivRA( 0.7, -0.3 ), -2.0, &ok );
    TESTCHECKF( DivRA( -0.7, -0.3 ), 2.0, &ok );

    TESTCHECKF( ModRA( 0.7, 0.3 ), 0.1, &ok );
    TESTCHECKF( ModRA( -0.7, 0.3 ), -0.1, &ok );
    TESTCHECKF( ModRA( 0.7, -0.3 ), 0.1, &ok );
    TESTCHECKF( ModRA( -0.7, -0.3 ), -0.1, &ok );

    if ( ok )
        cout << "DivMod PASSED." << endl << endl;
    else
        cout << "DivMod FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG

//*****************************************************************************

}                                                      //namespace EpsilonDelta

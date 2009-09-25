/*
  FixedPoint.cpp
  Copyright (C) 2009 David M. Anderson

  FixedPoint template class: fixed-point representation of a (real) number.
*/


#include "FixedPoint.hpp"
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
TestFixedPoint( )
{
    bool ok = true;
    cout << "Testing FixedPoint" << endl;

    cout << "Fixed32_8() [default constructor]" << endl;
    Fixed32_8 f32_8_0;
    TESTCHECK( sizeof( Fixed32_8 ), 4, &ok );
    TESTCHECK( Fixed32_8::FractionBits(), 8, &ok );
    TESTCHECK( f32_8_0.Raw(), 0, &ok );
    TESTCHECK( f32_8_0.ToInt(), 0, &ok );
    TESTCHECK( f32_8_0.ToFloat(), 0.f, &ok );
    TESTCHECK( -f32_8_0.Raw(), 0, &ok );
    cout << "Fixed32_8(10) [int constructor]" << endl;
    Fixed32_8 f32_8_10( 10 );
    TESTCHECK( f32_8_10.Raw(), 0xA00, &ok );
    TESTCHECK( f32_8_10.ToInt(), 10, &ok );
    TESTCHECK( f32_8_10.ToFloat(), 10.f, &ok );
    cout << "Fixed32_8(40, false) [int, bool constructor]" << endl;
    Fixed32_8 f32_8_40( 40, false );
    TESTCHECK( f32_8_40.Raw(), 0x2800, &ok );
    TESTCHECK( f32_8_40.ToInt(), 40, &ok );
    TESTCHECK( f32_8_40.ToFloat(), 40.f, &ok );
    cout << "Fixed32_8(0x40, true) [int, bool constructor]" << endl;
    Fixed32_8 f32_8_040( 0x40, true );
    TESTCHECK( f32_8_040.Raw(), 0x40, &ok );
    TESTCHECK( f32_8_040.ToInt(), 0, &ok );
    TESTCHECK( f32_8_040.ToFloat(), 0.25f, &ok );
    cout << "Fixed32_8( 0.25 ) [double constructor]" << endl;
    Fixed32_8 f32_8_025( 0.25 );
    TESTCHECK( f32_8_025.Raw(), 0x40, &ok );
    TESTCHECK( f32_8_025.ToInt(), 0, &ok );
    TESTCHECK( f32_8_025.ToFloat(), 0.25f, &ok );
    TESTCHECK( f32_8_025 == f32_8_10, false, &ok );
    TESTCHECK( f32_8_025 != f32_8_10, true, &ok );
    TESTCHECK( f32_8_025 < f32_8_10, true, &ok );
    TESTCHECK( f32_8_025 <= f32_8_10, true, &ok );
    TESTCHECK( f32_8_025 > f32_8_10, false, &ok );
    TESTCHECK( f32_8_025 >= f32_8_10, false, &ok );
    TESTCHECK( f32_8_025 == f32_8_040, true, &ok );
    TESTCHECK( f32_8_025 != f32_8_040, false, &ok );
    TESTCHECK( f32_8_025 < f32_8_040, false, &ok );
    TESTCHECK( f32_8_025 <= f32_8_040, true, &ok );
    TESTCHECK( f32_8_025 > f32_8_040, false, &ok );
    TESTCHECK( f32_8_025 >= f32_8_040, true, &ok );
    cout << "Fixed32_8(f32_8_040) [FixedPoint constructor]"
            " & assignment" << endl;
    f32_8_025 = Fixed32_8( f32_8_040 );
    TESTCHECK( f32_8_025.Raw(), 0x40, &ok );
    TESTCHECK( f32_8_025.ToInt(), 0, &ok );
    TESTCHECK( f32_8_025.ToFloat(), 0.25f, &ok );
    cout << "Fixed32_6(1) [int constructor]" << endl;
    Fixed32_6 f32_6_1( 1 );
    TESTCHECK( sizeof( f32_6_1 ), 4, &ok );
    TESTCHECK( f32_6_1.FractionBits(), 6, &ok );
    TESTCHECK( f32_6_1.Raw(), 0x40, &ok );
    TESTCHECK( f32_6_1.ToInt(), 1, &ok );
    TESTCHECK( f32_6_1.ToFloat(), 1.f, &ok );
    cout << "Fixed32_8(f32_6_1) [FixedPoint<R,F1> constructor]" << endl;
    Fixed32_8 f32_8_1( f32_6_1 );
    TESTCHECK( f32_8_1.Raw(), 0x100, &ok );
    TESTCHECK( f32_8_1.ToInt(), 1, &ok );
    TESTCHECK( f32_8_1.ToFloat(), 1.f, &ok );
    cout << "Fixed32_12(4) [int constructor]" << endl;
    Fixed32_12 f32_12_4( 4 );
    TESTCHECK( sizeof( f32_12_4 ), 4, &ok );
    TESTCHECK( f32_12_4.FractionBits(), 12, &ok );
    TESTCHECK( f32_12_4.Raw(), 0x4000, &ok );
    TESTCHECK( f32_12_4.ToInt(), 4, &ok );
    TESTCHECK( f32_12_4.ToFloat(), 4.f, &ok );
    cout << "Fixed32_8(f32_12_4) [FixedPoint<R,F1> constructor]" << endl;
    Fixed32_8 f32_8_4( f32_12_4 );
    TESTCHECK( f32_8_4.Raw(), 0x400, &ok );
    TESTCHECK( f32_8_4.ToInt(), 4, &ok );
    TESTCHECK( f32_8_4.ToFloat(), 4.f, &ok );
    cout << "Fixed16_8(0.75) [double constructor]" << endl;
    Fixed16_8 f16_8_075( 0.75 );
    TESTCHECK( sizeof( f16_8_075 ), 2, &ok );
    TESTCHECK( f16_8_075.FractionBits(), 8, &ok );
    TESTCHECK( f16_8_075.Raw(), (int16_t)0xC0, &ok );
    TESTCHECK( f16_8_075.ToInt(), 0, &ok );
    TESTCHECK( f16_8_075.ToFloat(), 0.75f, &ok );
    cout << "Fixed32_8(f16_8_075) [FixedPoint<R1,F> constructor]" << endl;
    Fixed32_8 f32_8_075( f16_8_075 );
    TESTCHECK( f32_8_075.Raw(), 0xC0, &ok );
    TESTCHECK( f32_8_075.ToInt(), 0, &ok );
    TESTCHECK( f32_8_075.ToFloat(), 0.75f, &ok );
    cout << "Fixed16_12(5) [int constructor]" << endl;
    Fixed16_12 f16_12_5( 5 );
    TESTCHECK( sizeof( f16_12_5 ), 2, &ok );
    TESTCHECK( f16_12_5.FractionBits(), 12, &ok );
    TESTCHECK( f16_12_5.Raw(), (int16_t)0x5000, &ok );
    TESTCHECK( f16_12_5.ToInt(), 5, &ok );
    TESTCHECK( f16_12_5.ToFloat(), 5.f, &ok );
    cout << "Fixed32_8(f16_12_5) [FixedPoint<R1,F1> constructor]" << endl;
    Fixed32_8 f32_8_5( f16_12_5 );
    TESTCHECK( f32_8_5.Raw(), 0x500, &ok );
    TESTCHECK( f32_8_5.ToInt(), 5, &ok );
    TESTCHECK( f32_8_5.ToFloat(), 5.f, &ok );

    cout << "f32_8_10 += f32_8_025" << endl;
    f32_8_10 += f32_8_025;
    TESTCHECK( f32_8_10.Raw(), 0xA40, &ok );
    TESTCHECK( f32_8_10.ToFloat(), 10.25f, &ok );
    cout << "-= f32_8_025" << endl;
    f32_8_10 -= f32_8_025;
    TESTCHECK( f32_8_10.Raw(), 0xA00, &ok );
    TESTCHECK( f32_8_10.ToFloat(), 10.f, &ok );
    cout << "*= f32_8_4" << endl;
    f32_8_10 *= f32_8_4;
    TESTCHECK( f32_8_40.Raw(), 0x2800, &ok );
    TESTCHECK( f32_8_40.ToFloat(), 40.f, &ok );
    cout << "*= f32_8_025" << endl;
    f32_8_10 *= f32_8_025;
    TESTCHECK( f32_8_10.Raw(), 0xA00, &ok );
    TESTCHECK( f32_8_10.ToFloat(), 10.f, &ok );
    cout << "/= f32_8_4" << endl;
    f32_8_10 /= f32_8_4;
    TESTCHECK( f32_8_10.Raw(), 0x280, &ok );
    TESTCHECK( f32_8_10.ToFloat(), 2.5f, &ok );
    cout << "/= f32_8_025" << endl;
    f32_8_10 /= f32_8_025;
    TESTCHECK( f32_8_10.Raw(), 0xA00, &ok );
    TESTCHECK( f32_8_10.ToFloat(), 10.f, &ok );
    TESTCHECK( (-f32_8_10).ToFloat(), -10.f, &ok );
    TESTCHECK( (-f32_8_025).ToFloat(), -.25f, &ok );
    
    TESTCHECK( (f32_8_040 + f32_8_0).Raw(), 0x40, &ok );
    TESTCHECK( (f32_8_040 - f32_8_0).Raw(), 0x40, &ok );
    TESTCHECK( (f32_8_040 * f32_8_0).Raw(), 0, &ok );
    TESTCHECK( (f32_8_10 + f32_8_025).Raw(), 0xA40, &ok );
    TESTCHECK( (f32_8_10 + f32_8_025).ToFloat(), 10.25f, &ok );
    TESTCHECK( (f32_8_10 - f32_8_025).Raw(), 0x9C0, &ok );
    TESTCHECK( (f32_8_10 - f32_8_025).ToFloat(), 9.75f, &ok );
    TESTCHECK( (f32_8_025 - f32_8_10).ToFloat(), -9.75f, &ok );
    TESTCHECK( (f32_8_40 + -60).ToFloat(), -20.f, &ok );
    TESTCHECK( (f32_8_40 - -60).ToFloat(), 100.f, &ok );
    TESTCHECK( (-60 + f32_8_40).ToFloat(), -20.f, &ok );
    TESTCHECK( (-60 - f32_8_40).ToFloat(), -100.f, &ok );

    TESTCHECK( (f32_8_10 * f32_8_40).ToFloat(), 400.f, &ok );
    TESTCHECK( (f32_8_075 * f32_8_40).ToFloat(), 30.f, &ok );
    cout << "FixedPoint< int64_t, 32 >(10.5)" << endl;
    FixedPoint< int64_t, 32 > f64_32_105( 10.5f );
    cout << "FixedPoint< int64_t, 32 >(20.5)" << endl;
    FixedPoint< int64_t, 32 > f64_32_205( 20.5f );
    TESTCHECK( (f64_32_105 * f64_32_205).ToFloat(), 215.25f, &ok );
    cout << "FixedPoint< int8_t, 5 >( 1.5 )" << endl;
    FixedPoint< int8_t, 5 > f8_5_15( 1.5 );
    cout << "FixedPoint< int8_t, 5 >( 2.5 )" << endl;
    FixedPoint< int8_t, 5 > f8_5_25( 2.5 );
    TESTCHECK( (f8_5_15 * f8_5_25).ToFloat(), 3.75f, &ok );
    cout << "Fixed32_8(1,true)" << endl;
    Fixed32_8 f32_8_01( 1, true );
    TESTCHECK( (f32_8_40 * f32_8_01).Raw(), 40, &ok );
    TESTCHECK( (f16_8_075 * Fixed16_8( 20 )).ToFloat(), 15.f, &ok );
    TESTCHECK( (Fixed16_12( 2, true ) * f16_12_5).Raw(), 10, &ok );
    TESTCHECK( (f32_8_025 * 6).ToFloat(), 1.5f, &ok );
    TESTCHECK( (9 * f32_8_075).ToFloat(), 6.75f, &ok );
    TESTCHECK( (f32_8_40 / f32_8_4).ToFloat(), 10.f, &ok );
    TESTCHECK( (f32_8_10 / f32_8_025).ToFloat(), 40.f, &ok );
    TESTCHECK( (f32_8_075 / 3).ToFloat(), 0.25f, &ok );
    TESTCHECK( (3 / f32_8_075).ToFloat(), 4.f, &ok );

    if ( ok )
        cout << "FixedPoint PASSED." << endl << endl;
    else
        cout << "FixedPoint FAILED." << endl << endl;
    return ok;
}

#endif

//*****************************************************************************

}                                                      //namespace EpsilonDelta

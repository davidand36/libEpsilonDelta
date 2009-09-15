/*
  Color.hpp
  Copyright (C) 2007 David M. Anderson

  Color classes.
*/


#include "Color.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

bool 
TestColor( )
{
    bool ok = true;
    cout << "Testing Color" << endl;

    int r = 64;
    int g = 128;
    int b = 255;
    cout << "Color3B( " << r << ", " << g << ", " << b << " )" << endl;
    Color3B color3B( r, g, b );
    TESTCHECK( color3B.Red(), r, &ok );
    TESTCHECK( color3B.Green(), g, &ok );
    TESTCHECK( color3B.Blue(), b, &ok );
    r = 255;
    g = 128;
    b = 64;
    cout << "Set( " << r << ", " << g << ", " << b << " )" << endl;
    color3B.Set( r, g, b );
    TESTCHECK( color3B.Red(), r, &ok );
    TESTCHECK( color3B.Green(), g, &ok );
    TESTCHECK( color3B.Blue(), b, &ok );
    TESTCHECK( color3B.Array()[0], r, &ok );
    TESTCHECK( color3B.Array()[1], g, &ok );
    TESTCHECK( color3B.Array()[2], b, &ok );

    float rf = 1.00f;
    float gf = 0.50f;
    float bf = 0.25f;
    cout << "Color3F( " << rf << ", " << gf << ", " << bf << " )" << endl;
    Color3F color3F( rf, gf, bf );
    TESTCHECKF( color3F.Red(), rf, &ok );
    TESTCHECKF( color3F.Green(), gf, &ok );
    TESTCHECKF( color3F.Blue(), bf, &ok );
    rf = 0.25f;
    gf = 0.50f;
    bf = 1.00f;
    cout << "Set( " << rf << ", " << gf << ", " << bf << " )" << endl;
    color3F.Set( rf, gf, bf );
    TESTCHECKF( color3F.Red(), rf, &ok );
    TESTCHECKF( color3F.Green(), gf, &ok );
    TESTCHECKF( color3F.Blue(), bf, &ok );
    TESTCHECK( color3F.Array()[0], rf, &ok );
    TESTCHECK( color3F.Array()[1], gf, &ok );
    TESTCHECK( color3F.Array()[2], bf, &ok );

    cout << "Color3B( color3F )" << endl;
    Color3B color3Bf( color3F );
    TESTCHECK( color3Bf.Red(), 64, &ok );
    TESTCHECK( color3Bf.Green(), 128, &ok );
    TESTCHECK( color3Bf.Blue(), 255, &ok );
    cout << "Color3F( color3B )" << endl;
    Color3F color3Fb( color3B );
    TESTCHECKF( color3Fb.Red(), 1.00f, &ok );
    TESTCHECKF( color3Fb.Green(), 0.5019608f, &ok );
    TESTCHECKF( color3Fb.Blue(), 0.2509804f, &ok );

    r = 64;
    g = 128;
    b = 255;
    int a = 0;
    cout << "Color4B( " << r << ", " << g << ", " << b << ", " << a << " )"
         << endl;
    Color4B color4B( r, g, b, a );
    TESTCHECK( color4B.Red(), r, &ok );
    TESTCHECK( color4B.Green(), g, &ok );
    TESTCHECK( color4B.Blue(), b, &ok );
    TESTCHECK( color4B.Alpha(), a, &ok );
    r = 255;
    g = 128;
    b = 64;
    a = 191;
    cout << "Set( " << r << ", " << g << ", " << b << ", " << a << " )"
         << endl;
    color4B.Set( r, g, b, a );
    TESTCHECK( color4B.Red(), r, &ok );
    TESTCHECK( color4B.Green(), g, &ok );
    TESTCHECK( color4B.Blue(), b, &ok );
    TESTCHECK( color4B.Alpha(), a, &ok );
    TESTCHECK( color4B.Array()[0], r, &ok );
    TESTCHECK( color4B.Array()[1], g, &ok );
    TESTCHECK( color4B.Array()[2], b, &ok );
    TESTCHECK( color4B.Array()[3], a, &ok );
    cout << "Color4B( color3B, " << a << " )" << endl;
    Color4B color4Bba( color3B, a );
    TESTCHECK( color4Bba.Red(), r, &ok );
    TESTCHECK( color4Bba.Green(), g, &ok );
    TESTCHECK( color4Bba.Blue(), b, &ok );
    TESTCHECK( color4Bba.Alpha(), a, &ok );
    cout << "Color4B( color3B )" << endl;
    Color4B color4Bb( color3B );
    TESTCHECK( color4Bb.Red(), r, &ok );
    TESTCHECK( color4Bb.Green(), g, &ok );
    TESTCHECK( color4Bb.Blue(), b, &ok );
    TESTCHECK( color4Bb.Alpha(), 255, &ok );
    float af = 0.75f;
    cout << "Color4B( color3F, " << af << " )" << endl;
    Color4B color4Bfa( color3F, af );
    TESTCHECK( color4Bfa.Red(), 64, &ok );
    TESTCHECK( color4Bfa.Green(), 128, &ok );
    TESTCHECK( color4Bfa.Blue(), 255, &ok );
    TESTCHECK( color4Bfa.Alpha(), 191, &ok );
    cout << "Color3B( color4B )" << endl;
    Color3B color3Bb( color4B );
    TESTCHECK( color3Bb.Red(), r, &ok );
    TESTCHECK( color3Bb.Green(), g, &ok );
    TESTCHECK( color3Bb.Blue(), b, &ok );
    TESTCHECK( color3Bb == color3B, true, &ok );
    TESTCHECK( color3Bb == color3Bf, false, &ok );
    TESTCHECK( color3Bb != color3B, false, &ok );
    TESTCHECK( color3Bb != color3Bf, true, &ok );

    rf = 1.00f;
    gf = 0.50f;
    bf = 0.25f;
    af = 0.00f;
    cout << "Color4F( " << rf << ", " << gf << ", " << bf << ", " << af << " )"
         << endl;
    Color4F color4F( rf, gf, bf, af );
    TESTCHECKF( color4F.Red(), rf, &ok );
    TESTCHECKF( color4F.Green(), gf, &ok );
    TESTCHECKF( color4F.Blue(), bf, &ok );
    TESTCHECKF( color4F.Alpha(), af, &ok );
    rf = 0.25f;
    gf = 0.50f;
    bf = 1.00f;
    af = 0.75f;
    cout << "Set( " << rf << ", " << gf << ", " << bf << ", " << af << " )"
         << endl;
    color4F.Set( rf, gf, bf, af );
    TESTCHECKF( color4F.Red(), rf, &ok );
    TESTCHECKF( color4F.Green(), gf, &ok );
    TESTCHECKF( color4F.Blue(), bf, &ok );
    TESTCHECKF( color4F.Alpha(), af, &ok );
    TESTCHECK( color4F.Array()[0], rf, &ok );
    TESTCHECK( color4F.Array()[1], gf, &ok );
    TESTCHECK( color4F.Array()[2], bf, &ok );
    TESTCHECK( color4F.Array()[3], af, &ok );

    cout << "Color4B( color4F )" << endl;
    Color4B color4Bf( color4F );
    TESTCHECK( color4Bf.Red(), 64, &ok );
    TESTCHECK( color4Bf.Green(), 128, &ok );
    TESTCHECK( color4Bf.Blue(), 255, &ok );
    TESTCHECK( color4Bf.Alpha(), 191, &ok );
    TESTCHECK( color4Bf == color4Bfa, true, &ok );
    TESTCHECK( color4Bf == color4B, false, &ok );
    TESTCHECK( color4Bf != color4Bfa, false, &ok );
    TESTCHECK( color4Bf != color4B, true, &ok );
    
    cout << "Color4F( color4B )" << endl;
    Color4F color4Fb( color4B );
    TESTCHECKF( color4Fb.Red(), 1.00f, &ok );
    TESTCHECKF( color4Fb.Green(), 0.5019608f, &ok );
    TESTCHECKF( color4Fb.Blue(), 0.2509804f, &ok );
    TESTCHECKF( color4Fb.Alpha(), 0.74901961f, &ok );
    cout << "Color4F( color3F, " << af << " )" << endl;
    Color4F color4Ffa( color3F, af );
    TESTCHECK( color4Ffa.Red(), rf, &ok );
    TESTCHECK( color4Ffa.Green(), gf, &ok );
    TESTCHECK( color4Ffa.Blue(), bf, &ok );
    TESTCHECK( color4Ffa.Alpha(), af, &ok );
    cout << "Color4F( color3F )" << endl;
    Color4F color4Ff( color3F );
    TESTCHECK( color4Ff.Red(), rf, &ok );
    TESTCHECK( color4Ff.Green(), gf, &ok );
    TESTCHECK( color4Ff.Blue(), bf, &ok );
    TESTCHECK( color4Ff.Alpha(), 1.f, &ok );
    TESTCHECK( color4Ff == color4F, false, &ok );
    TESTCHECK( color4Ffa == color4F, true, &ok );
    TESTCHECK( color4Ff != color4F, true, &ok );
    TESTCHECK( color4Ffa != color4F, false, &ok );
    cout << "Color3F( color4F )" << endl;
    Color3F color3Ff = color4F;
    TESTCHECKF( color3Ff.Red(), rf, &ok );
    TESTCHECKF( color3Ff.Green(), gf, &ok );
    TESTCHECKF( color3Ff.Blue(), bf, &ok );
    TESTCHECK( color3Ff == color3F, true, &ok );
    TESTCHECK( color3Ff == color3Fb, false, &ok );
    TESTCHECK( color3Ff != color3F, false, &ok );
    TESTCHECK( color3Ff != color3Fb, true, &ok );

    if ( ok )
        cout << "Color PASSED." << endl << endl;
    else
        cout << "Color FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  PrecisionTests.cpp
  Copyright (C) 2007 David M. Anderson

  Various examinations of the precision (and speed) of computations.
*/


#include <iostream>
#include <cmath>
#include <limits>
using namespace std;

#ifndef M_PI1
//From GNU math.h
# define M_PIl		3.1415926535897932384626433832795029L  /* pi */
#endif


namespace
{
void DisplayLimits( );
void DoMeeusTests( );
}


//*****************************************************************************


void DoPrecisionTests( )
{
    std::streamsize oldPrecision = std::cout.precision( 35 );

    cout << "Precision tests" << endl << endl;
    DisplayLimits( );
    DoMeeusTests( );

    std::cout.precision( oldPrecision );
}

//-----------------------------------------------------------------------------

namespace
{
//-----------------------------------------------------------------------------

#define DISPLAY( exp ) \
    cout << #exp << " = " << exp << endl

//.............................................................................

void 
DisplayLimits( )
{
    cout << "Numeric limits" << endl;
    DISPLAY( numeric_limits<float>::digits );
    DISPLAY( numeric_limits<float>::digits10 );
    DISPLAY( numeric_limits<float>::min() );
    DISPLAY( numeric_limits<float>::max() );
    DISPLAY( numeric_limits<float>::epsilon() );
    DISPLAY( numeric_limits<float>::round_error() );
    DISPLAY( numeric_limits<float>::min_exponent );
    DISPLAY( numeric_limits<float>::min_exponent10 );
    DISPLAY( numeric_limits<float>::max_exponent );
    DISPLAY( numeric_limits<float>::max_exponent10 );
    DISPLAY( numeric_limits<double>::digits );
    DISPLAY( numeric_limits<double>::digits10 );
    DISPLAY( numeric_limits<double>::min() );
    DISPLAY( numeric_limits<double>::max() );
    DISPLAY( numeric_limits<double>::epsilon() );
    DISPLAY( numeric_limits<double>::round_error() );
    DISPLAY( numeric_limits<double>::min_exponent );
    DISPLAY( numeric_limits<double>::min_exponent10 );
    DISPLAY( numeric_limits<double>::max_exponent );
    DISPLAY( numeric_limits<double>::max_exponent10 );
    DISPLAY( numeric_limits<long double>::digits );
    DISPLAY( numeric_limits<long double>::digits10 );
    DISPLAY( numeric_limits<long double>::min() );
    DISPLAY( numeric_limits<long double>::max() );
    DISPLAY( numeric_limits<long double>::epsilon() );
    DISPLAY( numeric_limits<long double>::round_error() );
    DISPLAY( numeric_limits<long double>::min_exponent );
    DISPLAY( numeric_limits<long double>::min_exponent10 );
    DISPLAY( numeric_limits<long double>::max_exponent );
    DISPLAY( numeric_limits<long double>::max_exponent10 );
    cout << endl;
}

//-----------------------------------------------------------------------------

void 
DoMeeusTests( )
{
    cout << "Jean Meeus's accuracy tests" << endl;

    float f;
    double d;
    long double ld;

    cout << "Significant digits in mantissa (while x!=x+1)" << endl;
    int bits;
    for ( f = 1.F, bits = 0; bits < 1000; f *= 2.F, ++bits )
    {
        float ff = f + 1.F;
        if ( f == ff )
            break;
    }
    cout << "    float: bits = " << bits << "  decimal digits = "
         << (bits * log10(2.L)) << endl;
    for ( d = 1., bits = 0; bits < 1000; d *= 2., ++bits )
    {
        double dd = d + 1.;
        if ( d == dd )
            break;
    }
    cout << "    double: bits = " << bits << "  decimal digits = "
         << (bits * log10(2.L)) << endl;
    for ( ld = 1.L, bits = 0; bits < 1000; ld *= 2.L, ++bits )
    {
        long double ldd = ld + 1.;
        if ( ld == ldd )
            break;
    }
    cout << "    long double: bits = " << bits << "  decimal digits = "
         << (bits * log10(2.L)) << endl;
    cout << endl;

    cout << "Trig and pi (3.1415926535897932384626433832795029)" << endl;
    f = 4.F * atan( 1.F );
    cout << "4*atan(1.F) = " << f << "  4*atan(1.F)-pi = " << (f - M_PIl)
         << endl;
    d = 4. * atan( 1. );
    cout << "4*atan(1.) = " << d << "  4*atan(1.)-pi = " << (d - M_PIl)
         << endl;
    ld = 4.L * atan( 1.L );
    cout << "4*atan(1.L) = " << ld << "  4*atan(1.L)-pi = " << (ld - M_PIl)
         << endl;
    cout << endl;

    cout << "x=1/3. x=(9*x+1)*x-1 should remain at 1/3" << endl;
    cout << "float:" << endl;
    f = 1.F/3.F;
    for ( int i = 0; i < 16; ++i )
    {
        cout << i << ": " << f << endl;
        f = (9.F * f + 1.F) * f - 1.F;
    }
    cout << "double:" << endl;
    d = 1./3.;
    for ( int i = 0; i < 32; ++i )
    {
        cout << i << ": " << d << endl;
        d = (9. * d + 1.) * d - 1.;
    }
    cout << "long double:" << endl;
    ld = 1.L/3.L;
    for ( int i = 0; i < 38; ++i )
    {
        cout << i << ": " << ld << endl;
        ld = (9.L * ld + 1.L) * ld - 1.L;
    }
    cout << endl;

    cout << "1.0000001^54 should be 674530.4707... (repeated squaring)"
         << endl;
    f = 1.0000001F;
    for ( int i = 0; i < 27; ++i )
        f = f * f;
    cout << "float: " << f << endl;
    d = 1.0000001;
    for ( int i = 0; i < 27; ++i )
        d = d * d;
    cout << "double: " << d << endl;
    ld = 1.0000001L;
    for ( int i = 0; i < 27; ++i )
        ld = ld * ld;
    cout << "long double: " << ld << endl;
    cout << endl;

    cout << "2. + 0.2 + 0.2 + 0.2 + 0.2 + 0.2 - 3. should be 0" << endl;
    cout << "float: " << 2.f + 0.2f + 0.2f + 0.2f + 0.2f + 0.2f - 3.f << endl;
    cout << "double: " << 2. + 0.2 + 0.2 + 0.2 + 0.2 + 0.2 - 3. << endl;
    cout << "long double: " << 2.L + 0.2L + 0.2L + 0.2L + 0.2L + 0.2L - 3.L
         << endl;
    cout << "x = 0.2 + 0.2 + 0.2 + 0.2 + 0.2.  x += 2.  x -= 3." << endl;
    f = 0.2f + 0.2f + 0.2f + 0.2f + 0.2f;
    f += 2.f;
    f -= 3.f;
    cout << "float: " << f << endl;
    cout << endl;

    DISPLAY( static_cast<int>( 3.f * (1.f / 3.f) ) );
    DISPLAY( static_cast<int>( 3. * (1. / 3.) ) );
    DISPLAY( static_cast<int>( 3.L * (1.L / 3.L) ) );
    cout << endl;
    
    DISPLAY( sqrt(25.f) - 5. );
    DISPLAY( sqrt(25.) - 5. );
    DISPLAY( sqrt(25.L) - 5. );
    cout << endl;
}

//-----------------------------------------------------------------------------
} //namespace


//*****************************************************************************


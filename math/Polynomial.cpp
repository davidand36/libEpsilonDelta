/*
  Polynomial.cpp
  Copyright (C) 2007 David M. Anderson

  Polynomial template class: representing a polynomial over any type (field).
*/


#include "Polynomial.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG


typedef complex<float> ComplexF;
typedef complex<double> ComplexD;
typedef complex<long double> ComplexLD;

//.............................................................................

bool 
TestPolynomial( )
{
    bool ok = true;
    cout << "Testing Polynomial" << endl;

    cout << "PolynomialLD( ) [default constructor]" << endl;
    PolynomialLD polyLD0;
    TESTCHECK( polyLD0.Degree(), 0, &ok );
    TESTCHECK( polyLD0[0], 0.L, &ok );
    TESTCHECK( polyLD0.Coefficient( 0 ), 0.L, &ok );
    try
    {
        TESTCHECK( polyLD0.Coefficient( 1 ), 0.L, &ok );
        cout << "Coefficient(1) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( polyLD0( 5.L ), 0.L, &ok );
    long double valLD;
    long double derivLD;
    cout << "polyLD0( 5.L, &val, &deriv )" << endl;
    polyLD0( 5.L, &valLD, &derivLD );
    TESTCHECK( valLD, 0.L, &ok );
    TESTCHECK( derivLD, 0.L, &ok );
    float cf[] = { 2.5f, 10.f, 3.f, 2.f };
    cout << "cf = { " << cf[0] << ", " << cf[1] << ", " << cf[2] << ", "
         << cf[3] << " }" << endl;
    cout << "PolynomialF( cf, cf + 2 ) [array iterator constructor]" << endl;
    PolynomialF polyF1( cf, cf + 2 );
    TESTCHECK( polyF1.Degree(), 1, &ok );
    TESTCHECK( polyF1[0], 2.5f, &ok );
    TESTCHECK( polyF1[1], 10.f, &ok );
    TESTCHECK( polyF1.Coefficient( 0 ), 2.5f, &ok );
    TESTCHECK( polyF1.Coefficient( 1 ), 10.f, &ok );
    try
    {
        TESTCHECK( polyF1.Coefficient( 2 ), 0.f, &ok );
        cout << "Coefficient(2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( polyF1( 5.f ), 52.5f, &ok );
    TESTCHECK( ToJSON( polyF1 ), string( "[ +2.50000e+00, +1.00000e+01 ]" ),
               &ok );
    cout << "FromJSON( \"[ 2.5, 10. ]\", &polyF1 )" << endl;
    FromJSON( "[ 2.5, 10. ]", &polyF1 );
    TESTCHECK( polyF1.Degree(), 1, &ok );
    TESTCHECK( polyF1[0], 2.5f, &ok );
    TESTCHECK( polyF1[1], 10.f, &ok );
    
    float valF;
    float derivF;
    cout << "polyF1( 5.f, &val, &deriv )" << endl;
    polyF1( 5.f, &valF, &derivF );
    TESTCHECK( valF, 52.5f, &ok );
    TESTCHECK( derivF, 10.f, &ok );
    cout << "Polynomial( cf, cf + 4 ) [array iterator constructor]" << endl;
    PolynomialF polyF3( cf, cf + 4 );
    TESTCHECK( polyF3.Degree(), 3, &ok );
    TESTCHECK( polyF3.Coefficient( 0 ), 2.5f, &ok );
    TESTCHECK( polyF3.Coefficient( 1 ), 10.f, &ok );
    TESTCHECK( polyF3.Coefficient( 2 ), 3.f, &ok );
    TESTCHECK( polyF3.Coefficient( 3 ), 2.f, &ok );
    try
    {
        TESTCHECK( polyF3.Coefficient( 4 ), 0.f, &ok );
        cout << "Coefficient(2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( polyF3( 5.f ), 377.5f, &ok );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << polyF3;
    TESTCHECK( ost.str(), string( "( 2.5 + 10x + 3x^2 + 2x^3 )" ), &ok );
    TESTCHECK( ToJSON( polyF3 ),
               string( "[ +2.50000e+00, +1.00000e+01, +3.00000e+00,"
                       " +2.00000e+00 ]" ),
               &ok );
    cout << "FromJSON( \"[ 2.5, 10., 3.0, 2 ]\", &polyF3 )" << endl;
    FromJSON( "[ 2.5, 10., 3.0, 2 ]", &polyF3 );
    TESTCHECK( polyF3.Degree(), 3, &ok );
    TESTCHECK( polyF3.Coefficient( 0 ), 2.5f, &ok );
    TESTCHECK( polyF3.Coefficient( 1 ), 10.f, &ok );
    TESTCHECK( polyF3.Coefficient( 2 ), 3.f, &ok );
    TESTCHECK( polyF3.Coefficient( 3 ), 2.f, &ok );
    cout << "polyF3( 5.f, ...)" << endl;
    polyF3( 5.f, &valF, &derivF );
    TESTCHECK( valF, 377.5f, &ok );
    TESTCHECK( derivF, 190.f, &ok );
    vector<long double> vld;
    long double c0 = -6.L;
    long double c1 = -5.L;
    long double c2 = 2.L;
    long double c3 = 1.L;
    cout << "vld.push_back( " << c0 << " )" << endl;
    vld.push_back( c0 );
    cout << "vld.push_back( " << c1 << " )" << endl;
    vld.push_back( c1 );
    cout << "vld.push_back( " << c2 << " )" << endl;
    vld.push_back( c2 );
    cout << "PolynomialLD( vld.begin(), vld.end() ) [vector iterator constructor]" << endl;
    PolynomialLD polyLD2( vld.begin(), vld.end() );
    TESTCHECK( polyLD2.Degree(), 2, &ok );
    TESTCHECK( polyLD2.Coefficient( 0 ), -6.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 1 ), -5.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 2 ), 2.L, &ok );
    try
    {
        TESTCHECK( polyLD2.Coefficient( 3 ), 0.L, &ok );
        cout << "Coefficient(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "vld.push_back( " << c3 << " )" << endl;
    vld.push_back( c3 );
    cout << "PolynomialLD( vld.begin(), vld.end() ) [vector iterator constructor]" << endl;
    PolynomialLD polyLD3( vld.begin(), vld.end() );
    TESTCHECK( polyLD3.Degree(), 3, &ok );
    TESTCHECK( polyLD3[0], -6.L, &ok );
    TESTCHECK( polyLD3[1], -5.L, &ok );
    TESTCHECK( polyLD3[2], 2.L, &ok );
    TESTCHECK( polyLD3[3], 1.L, &ok );
    TESTCHECK( polyLD3( -1.L ), 0.L, &ok );
    TESTCHECK( polyLD3( 2.L ), 0.L, &ok );
    TESTCHECK( polyLD3( -3.L ), 0.L, &ok );
    cout << "PolynomialLD( polyLD3 ) [copy constructor]" << endl;
    PolynomialLD polyLDdup( polyLD3 );
    TESTCHECK( polyLDdup.Degree(), 3, &ok );
    TESTCHECK( polyLDdup[0], -6.L, &ok );
    TESTCHECK( polyLDdup[1], -5.L, &ok );
    TESTCHECK( polyLDdup[2], 2.L, &ok );
    TESTCHECK( polyLDdup[3], 1.L, &ok );
    cout << "= polyLD2 [assignment]" << endl;
    polyLDdup = polyLD2;
    TESTCHECK( polyLDdup.Degree(), 2, &ok );
    TESTCHECK( polyLDdup.Coefficient( 0 ), -6.L, &ok );
    TESTCHECK( polyLDdup.Coefficient( 1 ), -5.L, &ok );
    TESTCHECK( polyLDdup.Coefficient( 2 ), 2.L, &ok );
    try
    {
        TESTCHECK( polyLDdup.Coefficient( 3 ), 0.L, &ok );
        cout << "Coefficient(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( (polyLDdup == polyLD2), true, &ok );
    TESTCHECK( (polyLDdup == polyLD3), false, &ok );
    cout << "Set( vld.begin(), vld.end() )" << endl;
    polyLDdup.Set( vld.begin(), vld.end() );
    TESTCHECK( polyLDdup.Degree(), 3, &ok );
    TESTCHECK( polyLDdup[0], -6.L, &ok );
    TESTCHECK( polyLDdup[1], -5.L, &ok );
    TESTCHECK( polyLDdup[2], 2.L, &ok );
    TESTCHECK( polyLDdup[3], 1.L, &ok );
    TESTCHECK( (polyLDdup == polyLD2), false, &ok );
    TESTCHECK( (polyLDdup == polyLD3), true, &ok );
    cout << "polyLD3 += polyLD0" << endl;
    polyLD3 += polyLD0;
    TESTCHECK( polyLD3.Degree(), 3, &ok );
    TESTCHECK( polyLD3[0], -6.L, &ok );
    TESTCHECK( polyLD3[1], -5.L, &ok );
    TESTCHECK( polyLD3[2], 2.L, &ok );
    TESTCHECK( polyLD3[3], 1.L, &ok );
    cout << "polyLD3 -= polyLD0" << endl;
    polyLD3 -= polyLD0;
    TESTCHECK( polyLD3.Degree(), 3, &ok );
    TESTCHECK( polyLD3[0], -6.L, &ok );
    TESTCHECK( polyLD3[1], -5.L, &ok );
    TESTCHECK( polyLD3[2], 2.L, &ok );
    TESTCHECK( polyLD3[3], 1.L, &ok );
    cout << "polyLD3 += polyLD3" << endl;
    polyLD3 += polyLD3;
    TESTCHECK( polyLD3.Degree(), 3, &ok );
    TESTCHECK( polyLD3[0], -12.L, &ok );
    TESTCHECK( polyLD3[1], -10.L, &ok );
    TESTCHECK( polyLD3[2], 4.L, &ok );
    TESTCHECK( polyLD3[3], 2.L, &ok );
    cout << "polyLD3 -= polyLDdup" << endl;
    polyLD3 -= polyLDdup;
    TESTCHECK( polyLD3.Degree(), 3, &ok );
    TESTCHECK( polyLD3.Coefficient( 0 ), -6.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 1 ), -5.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 2 ), 2.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 3 ), 1.L, &ok );
    cout << "polyLD2 += polyLD3" << endl;
    polyLD2 += polyLD3;
    TESTCHECK( polyLD2.Degree(), 3, &ok );
    TESTCHECK( polyLD2.Coefficient( 0 ), -12.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 1 ), -10.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 2 ), 4.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 3 ), 1.L, &ok );
    cout << "polyLD2 -= polyLD3" << endl;
    polyLD2 -= polyLD3;
    TESTCHECK( polyLD2.Degree(), 3, &ok );
    TESTCHECK( polyLD2.Coefficient( 0 ), -6.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 1 ), -5.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 2 ), 2.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 3 ), 0.L, &ok );
    cout << "Trim()" << endl;
    polyLD2.Trim( );
    TESTCHECK( polyLD2.Degree(), 2, &ok );
    TESTCHECK( polyLD2.Coefficient( 0 ), -6.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 1 ), -5.L, &ok );
    TESTCHECK( polyLD2.Coefficient( 2 ), 2.L, &ok );
    long double dd = -4.L;
    cout << "*= " << dd << endl;
    polyLD3 *= dd;
    TESTCHECK( polyLD3.Degree(), 3, &ok );
    TESTCHECK( polyLD3.Coefficient( 0 ), 24.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 1 ), 20.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 2 ), -8.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 3 ), -4.L, &ok );
    cout << "/= " << dd << endl;
    polyLD3 /= dd;
    TESTCHECK( polyLD3.Degree(), 3, &ok );
    TESTCHECK( polyLD3.Coefficient( 0 ), -6.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 1 ), -5.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 2 ), 2.L, &ok );
    TESTCHECK( polyLD3.Coefficient( 3 ), 1.L, &ok );
    c0 = -6.L;
    c1 = 1.L;
    c2 = 1.L;
    cout << "PolynomialLD( " << 2 << ", " << c0 << ", " << c1 << ", " << c2
         << " ) [degree, coefficients constructor]" << endl;
    PolynomialLD polyLD2a( 2, c0, c1, c2 );
    TESTCHECK( polyLD2a.Degree(), 2, &ok );
    TESTCHECK( polyLD2a.Coefficient( 0 ), c0, &ok );
    TESTCHECK( polyLD2a.Coefficient( 1 ), c1, &ok );
    TESTCHECK( polyLD2a.Coefficient( 2 ), c2, &ok );
    cout << "Set( " << 2 << ", " << c0 << ", " << c1 << ", " << c2 << " )" << endl;
    polyLD2.Set( 2, c0, c1, c2 );
    TESTCHECK( (polyLD2 == polyLD2a), true, &ok );
#if 0   //Can't reliably pass floats as var args!
    cout << "PolynomialF( 2, -6.f, 1.f, 1.f ) [degree, coefficients constructor]" << endl;
    PolynomialF polyF2( 2, -6.f, 1.f, 1.f );
    TESTCHECK( polyF2.Degree(), 2, &ok );
    TESTCHECK( polyF2[0], -6.f, &ok );
    TESTCHECK( polyF2[1], 1.f, &ok );
    TESTCHECK( polyF2[2], 1.f, &ok );
#else   //Can pass in doubles and let them be converted
    cout << "PolynomialF( 2, -6., 1., 1. ) [degree, coefficients constructor]" << endl;
    PolynomialF polyF2( 2, -6., 1., 1. );
    TESTCHECK( polyF2.Degree(), 2, &ok );
    TESTCHECK( polyF2[0], -6.f, &ok );
    TESTCHECK( polyF2[1], 1.f, &ok );
    TESTCHECK( polyF2[2], 1.f, &ok );
#endif
    c0 = -1.L;
    c1 = 0.L;
    c2 = 0.5L;
    cout << "Set( " << 2 << ", " << c0 << ", " << c1 << ", " << c2 << " )" << endl;
    polyLD2a.Set( 2, c0, c1, c2 );
    PolynomialLD quot;
    PolynomialLD rem;
    cout << "polyLD3.DivMod( polyLD2a, &quot, &rem )" << endl;
    polyLD3.DivMod( polyLD2a, &quot, &rem );
    TESTCHECK( quot.Degree(), 1, &ok );
    TESTCHECK( quot.Coefficient( 0 ), 4.L, &ok );
    TESTCHECK( quot.Coefficient( 1 ), 2.L, &ok );
    TESTCHECK( rem.Degree(), 1, &ok );
    TESTCHECK( rem.Coefficient( 0 ), -2.L, &ok );
    TESTCHECK( rem.Coefficient( 1 ), -3.L, &ok );
    cout << "polyLD2a *= quot" << endl;
    polyLD2a *= quot;
    cout << "polyLD2a += rem" << endl;
    polyLD2a += rem;
    TESTCHECK( (polyLD2a == polyLD2), false, &ok );
    TESTCHECK( (polyLD2a == polyLD3), true, &ok );
    c0 = -1.L;
    cout << "DivideByMonomial( " << c0 << " )" << endl;
    polyLD2a.DivideByMonomial( c0 );
    TESTCHECK( (polyLD2a == polyLD2), true, &ok );
    TESTCHECK( (polyLD2a == polyLD3), false, &ok );
    cout << "MultiplyByMonomial( " << c0 << " )" << endl;
    polyLD2a.MultiplyByMonomial( c0 );
    TESTCHECK( (polyLD2a == polyLD2), false, &ok );
    TESTCHECK( (polyLD2a == polyLD3), true, &ok );
    cout << "polyLD2.DivMod( polyLD3, &quot, &rem )" << endl;
    polyLD2.DivMod( polyLD3, &quot, &rem );
    TESTCHECK( (quot == polyLD0), true, &ok );
    TESTCHECK( (rem == polyLD2), true, &ok );
    vector<long double> roots;
    cout << "PolynomialLD( 1, 4.L, 2.L ).RealRoots()" << endl;
    roots = PolynomialLD( 1, 4.L, 2.L ).RealRoots( );
    TESTCHECK( roots.size(), 1, &ok );
    TESTCHECK( roots[0], -2.L, &ok );
    cout << "polyLD2.RealRoots()" << endl;
    roots = polyLD2.RealRoots( );
    std::sort( roots.begin(), roots.end() );
    TESTCHECK( roots.size(), 2, &ok );
    TESTCHECK( roots[0], -3.L, &ok );
    TESTCHECK( roots[1], 2.L, &ok );
    cout << "polyLD3.RealRoots()" << endl;
    roots = polyLD3.RealRoots( );
    std::sort( roots.begin(), roots.end() );
    TESTCHECK( roots.size(), 3, &ok );
    TESTCHECKF( roots[0], -3.L, &ok );
    TESTCHECKF( roots[1], -1.L, &ok );
    TESTCHECKF( roots[2], 2.L, &ok );
    cout << "PolynomialLD( 3, 45.L, 15.L, 15.L, 5.L ).RealRoots()" << endl;
    roots = PolynomialLD( 3, 45.L, 15.L, 15.L, 5.L ).RealRoots( );
    TESTCHECK( roots.size(), 1, &ok );
    TESTCHECKF( roots[0], -3.L, &ok );
    cout << "PolynomialLD( 5, -30.L, -25.L, 4.L, 0.L, 2.L, 1.L ).RealRoots( )"
         << endl;
    roots = PolynomialLD( 5, -30.L, -25.L, 4.L, 0.L, 2.L, 1.L ).RealRoots( );
    std::sort( roots.begin(), roots.end() );
    TESTCHECK( roots.size(), 3, &ok );
    TESTCHECKF( roots[0], -3.L, &ok );
    TESTCHECKF( roots[1], -1.L, &ok );
    TESTCHECKF( roots[2], 2.L, &ok );

    //Complex polynomials
    cout << "PolynomialCLD( ) [default constructor]" << endl;
    PolynomialCLD polyCLD0;
    TESTCHECK( polyCLD0.Degree(), 0, &ok );
    TESTCHECK( real( polyCLD0[0] ), 0.L, &ok );
    TESTCHECK( imag( polyCLD0[0] ), 0.L, &ok );
    TESTCHECK( real( polyCLD0.Coefficient( 0 ) ), 0.L, &ok );
    TESTCHECK( imag( polyCLD0.Coefficient( 0 ) ), 0.L, &ok );
    try
    {
        TESTCHECK( real( polyCLD0.Coefficient( 1 ) ), 0.L, &ok );
        cout << "Coefficient(1) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( real( polyCLD0( ComplexLD( 5.L, -3.L ) ) ), 0.L, &ok );
    ComplexLD valCLD;
    ComplexLD derivCLD;
    cout << "polyCLD0( 5.L, &val, &deriv )" << endl;
    polyCLD0( ComplexLD( 5.L, -3.L ), &valCLD, &derivCLD );
    TESTCHECK( real( valCLD ), 0.L, &ok );
    TESTCHECK( imag( valCLD ), 0.L, &ok );
    TESTCHECK( real( derivCLD ), 0.L, &ok );
    TESTCHECK( imag( derivCLD ), 0.L, &ok );
    ComplexF ccf[] = { ComplexF( 2.5f, -1.f ), ComplexF( 10.f, 2.f ), 3.f, 2.f };
    cout << "ccf[] = { ComplexF( 2.5f, -1.f ), ComplexF( 10.f, 2.f ), 3.f, 2.f }" << endl;
    cout << "Polynomial( ccf, ccf + 2 ) [array iterator constructor]" << endl;
    PolynomialCF polyCF1( ccf, ccf + 2 );
    TESTCHECK( polyCF1.Degree(), 1, &ok );
    TESTCHECK( real( polyCF1[0] ), 2.5f, &ok );
    TESTCHECK( imag( polyCF1[0] ), -1.f, &ok );
    TESTCHECK( real( polyCF1[1] ), 10.f, &ok );
    TESTCHECK( imag( polyCF1[1] ), 2.f, &ok );
    TESTCHECK( real( polyCF1.Coefficient( 0 ) ), 2.5f, &ok );
    TESTCHECK( imag( polyCF1.Coefficient( 0 ) ), -1.f, &ok );
    TESTCHECK( real( polyCF1.Coefficient( 1 ) ), 10.f, &ok );
    TESTCHECK( imag( polyCF1.Coefficient( 1 ) ), 2.f, &ok );
    try
    {
        TESTCHECK( real( polyCF1.Coefficient( 2 ) ), 0.f, &ok );
        cout << "Coefficient(2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( real( polyCF1( 5.f ) ), 52.5f, &ok );
    TESTCHECK( imag( polyCF1( 5.f ) ), 9.f, &ok );
    ComplexF valCF;
    ComplexF derivCF;
    cout << "polyCF1( 5.f, &val, &deriv )" << endl;
    polyCF1( 5.f, &valCF, &derivCF );
    TESTCHECK( real( valCF ), 52.5f, &ok );
    TESTCHECK( imag( valCF ), 9.f, &ok );
    TESTCHECK( real( derivCF ), 10.f, &ok );
    TESTCHECK( imag( derivCF ), 2.f, &ok );
    cout << "Polynomial( ccf, ccf + 4 ) [array iterator constructor]" << endl;
    PolynomialCF polyCF3( ccf, ccf + 4 );
    TESTCHECK( polyCF3.Degree(), 3, &ok );
    TESTCHECK( real( polyCF3.Coefficient( 0 ) ), 2.5f, &ok );
    TESTCHECK( imag( polyCF3.Coefficient( 0 ) ), -1.f, &ok );
    TESTCHECK( real( polyCF3.Coefficient( 1 ) ), 10.f, &ok );
    TESTCHECK( imag( polyCF3.Coefficient( 1 ) ), 2.f, &ok );
    TESTCHECK( real( polyCF3.Coefficient( 2 ) ), 3.f, &ok );
    TESTCHECK( imag( polyCF3.Coefficient( 2 ) ), 0.f, &ok );
    TESTCHECK( real( polyCF3.Coefficient( 3 ) ), 2.f, &ok );
    TESTCHECK( imag( polyCF3.Coefficient( 3 ) ), 0.f, &ok );
    try
    {
        TESTCHECK( real( polyCF3.Coefficient( 4 ) ), 0.f, &ok );
        cout << "Coefficient(2) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( real( polyCF3( 5.f ) ), 377.5f, &ok );
    cout << "polyCF3( 5.f, &val, &deriv )" << endl;
    polyCF3( 5.f, &valCF, &derivCF );
    TESTCHECK( real( valCF ), 377.5f, &ok );
    TESTCHECK( imag( valCF ), 9.f, &ok );
    TESTCHECK( real( derivCF ), 190.f, &ok );
    TESTCHECK( imag( derivCF ), 2.f, &ok );
    vector< ComplexLD > cvld;
    ComplexLD cc0 = -6.L;
    ComplexLD cc1 = -5.L;
    ComplexLD cc2 = 2.L;
    ComplexLD cc3 = 1.L;
    cout << "cvld.push_back( " << cc0 << " )" << endl;
    cvld.push_back( cc0 );
    cout << "cvld.push_back( " << cc1 << " )" << endl;
    cvld.push_back( cc1 );
    cout << "cvld.push_back( " << cc2 << " )" << endl;
    cvld.push_back( cc2 );
    cout << "Polynomial( cvld.begin(), cvld.end() ) [vector iterator constructor]" << endl;
    PolynomialCLD polyCLD2( cvld.begin(), cvld.end() );
    TESTCHECK( polyCLD2.Degree(), 2, &ok );
    TESTCHECK( polyCLD2.Coefficient( 0 ), ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 1 ), ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 2 ), ComplexLD( 2.L ), &ok );
    try
    {
        TESTCHECK( polyCLD2.Coefficient( 3 ), ComplexLD( 0.L ), &ok );
        cout << "Coefficient(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    cout << "cvld.push_back( " << cc3 << " )" << endl;
    cvld.push_back( cc3 );
    cout << "Polynomial( vld.begin(), vld.end() ) [vector iterator constructor]" << endl;
    PolynomialCLD polyCLD3( cvld.begin(), cvld.end() );
    TESTCHECK( polyCLD3.Degree(), 3, &ok );
    TESTCHECK( polyCLD3[0], ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD3[1], ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD3[2], ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLD3[3], ComplexLD( 1.L ), &ok );
    TESTCHECK( polyCLD3( -1.L ), ComplexLD( 0.L ), &ok );
    TESTCHECK( polyCLD3( 2.L ), ComplexLD( 0.L ), &ok );
    TESTCHECK( polyCLD3( -3.L ), ComplexLD( 0.L ), &ok );
    cout << "Polynomial( polyCLD3 ) [copy constructor]" << endl;
    PolynomialCLD polyCLDdup( polyCLD3 );
    TESTCHECK( polyCLDdup.Degree(), 3, &ok );
    TESTCHECK( polyCLDdup[0], ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLDdup[1], ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLDdup[2], ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLDdup[3], ComplexLD( 1.L ), &ok );
    cout << "= polyCLD2 [assignment]" << endl;
    polyCLDdup = polyCLD2;
    TESTCHECK( polyCLDdup.Degree(), 2, &ok );
    TESTCHECK( polyCLDdup.Coefficient( 0 ), ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLDdup.Coefficient( 1 ), ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLDdup.Coefficient( 2 ), ComplexLD( 2.L ), &ok );
    try
    {
        TESTCHECK( polyCLDdup.Coefficient( 3 ), ComplexLD( 0.L ), &ok );
        cout << "Coefficient(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "out_of_range here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    TESTCHECK( (polyCLDdup == polyCLD2), true, &ok );
    TESTCHECK( (polyCLDdup == polyCLD3), false, &ok );
    cout << "Set( vld.begin(), vld.end() )" << endl;
    polyCLDdup.Set( vld.begin(), vld.end() );
    TESTCHECK( polyCLDdup.Degree(), 3, &ok );
    TESTCHECK( polyCLDdup[0], ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLDdup[1], ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLDdup[2], ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLDdup[3], ComplexLD( 1.L ), &ok );
    TESTCHECK( (polyCLDdup == polyCLD2), false, &ok );
    TESTCHECK( (polyCLDdup == polyCLD3), true, &ok );
    cout << "polyCLD3 += polyCLD0" << endl;
    polyCLD3 += polyCLD0;
    TESTCHECK( polyCLD3.Degree(), 3, &ok );
    TESTCHECK( polyCLD3[0], ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD3[1], ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD3[2], ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLD3[3], ComplexLD( 1.L ), &ok );
    cout << "polyCLD3 -= polyCLD0" << endl;
    polyCLD3 -= polyCLD0;
    TESTCHECK( polyCLD3.Degree(), 3, &ok );
    TESTCHECK( polyCLD3[0], ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD3[1], ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD3[2], ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLD3[3], ComplexLD( 1.L ), &ok );
    cout << "polyCLD3 += polyCLD3" << endl;
    polyCLD3 += polyCLD3;
    TESTCHECK( polyCLD3.Degree(), 3, &ok );
    TESTCHECK( polyCLD3[0], ComplexLD( -12.L ), &ok );
    TESTCHECK( polyCLD3[1], ComplexLD( -10.L ), &ok );
    TESTCHECK( polyCLD3[2], ComplexLD( 4.L ), &ok );
    TESTCHECK( polyCLD3[3], ComplexLD( 2.L ), &ok );
    cout << "polyCLD3 -= polyCLDdup" << endl;
    polyCLD3 -= polyCLDdup;
    TESTCHECK( polyCLD3.Degree(), 3, &ok );
    TESTCHECK( polyCLD3.Coefficient( 0 ), ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 1 ), ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 2 ), ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 3 ), ComplexLD( 1.L ), &ok );
    cout << "polyCLD2 += polyCLD3" << endl;
    polyCLD2 += polyCLD3;
    TESTCHECK( polyCLD2.Degree(), 3, &ok );
    TESTCHECK( polyCLD2.Coefficient( 0 ), ComplexLD( -12.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 1 ), ComplexLD( -10.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 2 ), ComplexLD( 4.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 3 ), ComplexLD( 1.L ), &ok );
    cout << "polyCLD2 -= polyCLD3" << endl;
    polyCLD2 -= polyCLD3;
    TESTCHECK( polyCLD2.Degree(), 3, &ok );
    TESTCHECK( polyCLD2.Coefficient( 0 ), ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 1 ), ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 2 ), ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 3 ), ComplexLD( 0.L ), &ok );
    cout << "Trim()" << endl;
    polyCLD2.Trim( );
    TESTCHECK( polyCLD2.Degree(), 2, &ok );
    TESTCHECK( polyCLD2.Coefficient( 0 ), ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 1 ), ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD2.Coefficient( 2 ), ComplexLD( 2.L ), &ok );
    ComplexLD cdd = ComplexLD( 0.L, -4.L );
    cout << "*= " << cdd << endl;
    polyCLD3 *= cdd;
    TESTCHECK( polyCLD3.Degree(), 3, &ok );
    TESTCHECK( polyCLD3.Coefficient( 0 ), ComplexLD( 0.L, 24.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 1 ), ComplexLD( 0.L, 20.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 2 ), ComplexLD( 0.L, -8.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 3 ), ComplexLD( 0.L, -4.L ), &ok );
    cout << "/= " << cdd << endl;
    polyCLD3 /= cdd;
    TESTCHECK( polyCLD3.Degree(), 3, &ok );
    TESTCHECK( polyCLD3.Coefficient( 0 ), ComplexLD( -6.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 1 ), ComplexLD( -5.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 2 ), ComplexLD( 2.L ), &ok );
    TESTCHECK( polyCLD3.Coefficient( 3 ), ComplexLD( 1.L ), &ok );
    c0 = -6.L;
    c1 = 1.L;
    c2 = 1.L;
    cout << "Polynomial( " << 2 << ", " << c0 << ", " << c1 << ", " << c2
         << " ) [degree, coefficients constructor]" << endl;
    PolynomialCLD polyCLD2a( 2, c0, c1, c2 );
    TESTCHECK( polyCLD2a.Degree(), 2, &ok );
    TESTCHECK( polyCLD2a.Coefficient( 0 ), ComplexLD( c0 ), &ok );
    TESTCHECK( polyCLD2a.Coefficient( 1 ), ComplexLD( c1 ), &ok );
    TESTCHECK( polyCLD2a.Coefficient( 2 ), ComplexLD( c2 ), &ok );
    cout << "Set( " << 2 << ", " << c0 << ", " << c1 << ", " << c2 << " )" << endl;
    polyCLD2.Set( 2, c0, c1, c2 );
    TESTCHECK( (polyCLD2 == polyCLD2a), true, &ok );
    c0 = -1.L;
    c1 = 0.L;
    c2 = 0.5L;
    cout << "Set( " << 2 << ", " << c0 << ", " << c1 << ", " << c2 << " )" << endl;
    polyCLD2a.Set( 2, c0, c1, c2 );
    PolynomialCLD quotc;
    PolynomialCLD remc;
    cout << "polyCLD3.DivMod( polyCLD2a, &quot, &rem )" << endl;
    polyCLD3.DivMod( polyCLD2a, &quotc, &remc );
    TESTCHECK( quotc.Degree(), 1, &ok );
    TESTCHECK( quotc.Coefficient( 0 ), ComplexLD( 4.L ), &ok );
    TESTCHECK( quotc.Coefficient( 1 ), ComplexLD( 2.L ), &ok );
    TESTCHECK( remc.Degree(), 1, &ok );
    TESTCHECK( remc.Coefficient( 0 ), ComplexLD( -2.L ), &ok );
    TESTCHECK( remc.Coefficient( 1 ), ComplexLD( -3.L ), &ok );
    cout << "polyCLD2a *= quot" << endl;
    polyCLD2a *= quotc;
    cout << "polyCLD2a += rem" << endl;
    polyCLD2a += remc;
    TESTCHECK( (polyCLD2a == polyCLD2), false, &ok );
    TESTCHECK( (polyCLD2a == polyCLD3), true, &ok );
    c0 = -1.L;
    cout << "DivideByMonomial( " << c0 << " )" << endl;
    polyCLD2a.DivideByMonomial( c0 );
    TESTCHECK( (polyCLD2a == polyCLD2), true, &ok );
    TESTCHECK( (polyCLD2a == polyCLD3), false, &ok );
    cout << "MultiplyByMonomial( " << c0 << " )" << endl;
    polyCLD2a.MultiplyByMonomial( c0 );
    TESTCHECK( (polyCLD2a == polyCLD2), false, &ok );
    TESTCHECK( (polyCLD2a == polyCLD3), true, &ok );
    cout << "polyCLD2.DivMod( polyCLD3, &quot, &rem )" << endl;
    polyCLD2.DivMod( polyCLD3, &quotc, &remc );
    TESTCHECK( (quotc == polyCLD0), true, &ok );
    TESTCHECK( (remc == polyCLD2), true, &ok );
    vector< ComplexLD > compRoots;
    c0 = 1.L;
    c1 = 0.L;
    c2 = 1.L;
    cout << "PolynomialCLD( 2, 1.L, 0.L, 1.L ).Roots()" << endl;
    compRoots = PolynomialCLD( 2, c0, c1, c2 ).Roots( );
    TESTCHECK( compRoots.size(), 2, &ok );
    if ( compRoots[0].imag() < 0.L )
        swap( compRoots[0], compRoots[1] );
    TESTCHECKF( real( compRoots[0] ), 0.L, &ok );
    TESTCHECKF( imag( compRoots[0] ), 1.L, &ok );
    TESTCHECKF( real( compRoots[1] ), 0.L, &ok );
    TESTCHECKF( imag( compRoots[1] ), -1.L, &ok );

    if ( ok )
        cout << "Polynomial PASSED." << endl << endl;
    else
        cout << "Polynomial FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

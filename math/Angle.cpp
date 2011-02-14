/*
  Angle.cpp
  Copyright (C) 2007 David M. Anderson

  Angle class: represents an angle in radians.
*/


#include "Angle.hpp"
#include "AngleDMS.hpp"
#include "AngleHMS.hpp"
#include "JSON.hpp"
#include "Exception.hpp"
#include "Assert.hpp"
#include <limits>
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool Angle::ms_operatorsNormalized = true;

//-----------------------------------------------------------------------------

namespace
{                                                                   //namespace

double infinity = numeric_limits< double >::infinity();

}                                                                   //namespace

//=============================================================================


Angle::Angle( const AngleDMS & dms )
{
    Set( dms );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Angle::Angle( const AngleHMS & hms )
{
    Set( hms );
}

//=============================================================================

void 
Angle::Set( const AngleDMS & dms )
{
    m_radians = dms.TotalDegrees( ) * M_PI / 180.;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Angle::Set( const AngleHMS & hms )
{
    m_radians = hms.TotalHours( ) * M_PI / 12.;
}

//=============================================================================

double 
Angle::Tan( ) const
{
    Angle a = *this;
    a.Normalize( );
    if( (a.m_radians == - M_PI / 2.) || (a.m_radians == M_PI / 2.) )
        throw LogicError( "Tan error: pi/2" );
    if ( a.m_radians == - M_PI / 2. )
        return - infinity;
    if ( a.m_radians == M_PI / 2. )
        return infinity;
    return tan( a.m_radians );
}

//-----------------------------------------------------------------------------

Angle 
ArcSin( double s )
{
    Assert( (-1. <= s) && (s <= 1.) );
    if ( s <= -1. )
        return Angle( - M_PI / 2. );
    if ( s >= 1. )
        return Angle( M_PI / 2. );
    Angle as = asin( s );
    as.Normalize( );
    return as;
}

//-----------------------------------------------------------------------------

Angle 
ArcCos( double c )
{
    Assert( (-1. <= c) && (c <= 1.) );
    if ( c <= -1. )
        return Angle( M_PI );
    if ( c >= 1. )
        return Angle( 0. );
    return Angle( acos( c ) );
}

//-----------------------------------------------------------------------------

Angle 
ArcTan( double num, double den )
{
    Assert( (num != 0.) || (den != 0.) );
    if ( (num == 0.) && (den == 0.) )
        return Angle( M_PI / 2.);
    return Angle( atan2( num, den ) );
}

//-----------------------------------------------------------------------------

Angle 
ArcHav( double h )
{
    Assert( (0. <= h) && (h <= 1.) );
    if ( h <= 0. )
        return Angle( 0. );
    if ( h >= 1. )
        return Angle( M_PI );
    return (2. * asin( sqrt( h ) ));
}

//=============================================================================

void 
MakeSinCosTables( Angle angle, int maxMultiple,
                  vector<double> * pSinTable,
                  vector<double> * pCosTable )
{
    if ( pSinTable )
        pSinTable->reserve( maxMultiple + 1 );
    if ( pCosTable )
        pCosTable->reserve( maxMultiple + 1 );
    double sinA = angle.Sin( );
    double cosA = angle.Cos( );
    double s = 0.;
    double c = 1.;
    if ( pSinTable )
        pSinTable->push_back( s );
    if ( pCosTable )
        pCosTable->push_back( c );
    for ( int i = 1; i <= maxMultiple; ++i )
    {
        double s1 = s * cosA + c * sinA;
        double c1 = c * cosA - s * sinA;
        s = s1;
        c = c1;
        if ( pSinTable )
            pSinTable->push_back( s );
        if ( pCosTable )
            pCosTable->push_back( c );
    }
}

//-----------------------------------------------------------------------------

void 
MakeSinCosTables( int divisions,
                  vector<double> * pSinTable,
                  vector<double> * pCosTable )
{
    Angle angle( 2. * M_PI / divisions );
    MakeSinCosTables( angle, divisions, pSinTable, pCosTable );
}

//=============================================================================

ostream & 
operator<<( ostream & out, Angle angle )
{
    return out << angle.Degrees() << "°";
}

//=============================================================================

string 
ToJSON( Angle angle )
{
    return ToJSON( angle.Radians() );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const std::string & json, Angle * pAngle )
{
    double d;
    FromJSON( json, &d );
    pAngle->Set( d );
}

//=============================================================================

#ifdef DEBUG

bool 
Angle::Test( )
{
    bool ok = true;
    cout << "Testing Angle" << endl;

    cout << "Angle( ) [default constructor]" << endl;
    Angle angle0;
    cout << "Set( )" << endl;
    angle0.Set( );
    TESTCHECK( angle0.Radians(), 0., &ok );
    TESTCHECK( angle0.Degrees(), 0., &ok );
    TESTCHECK( angle0.Grads(), 0., &ok );
    TESTCHECK( angle0.Cycles(), 0., &ok );
    TESTCHECK( angle0.Sin(), 0., &ok );
    TESTCHECK( angle0.Cos(), 1., &ok );
    TESTCHECK( angle0.Tan(), 0., &ok );
    TESTCHECK( angle0.Hav(), 0., &ok );
    double rads = M_PI / 4.;
    cout << "Angle( " << rads << " ) [radians constructor]" << endl;
    Angle angle45( rads );
    TESTCHECKF( angle45.Radians(), (M_PI / 4.), &ok );
    TESTCHECKF( angle45.Degrees(), 45., &ok );
    TESTCHECKF( angle45.Grads(), 50., &ok );
    TESTCHECKF( angle45.Cycles(), 0.125, &ok );
    TESTCHECKF( angle45.Sin(), sqrt( 0.5 ), &ok );
    TESTCHECKF( angle45.Cos(), sqrt( 0.5 ), &ok );
    TESTCHECKF( angle45.Tan(), 1., &ok );
    TESTCHECKF( angle45.Hav(), 0.1464466, &ok );
    double degs = 450.;
    cout << "Angle( " << degs << ", Degree ) [degrees constructor]" << endl;
    Angle angle90( degs, Degree );
    TESTCHECKF( angle90.Radians(), (5 * M_PI / 2.), &ok );
    TESTCHECKF( angle90.Degrees(), 450., &ok );
    TESTCHECKF( angle90.Grads(), 500., &ok );
    TESTCHECKF( angle90.Cycles(), 1.25, &ok );
    TESTCHECKF( angle90.Sin(), 1., &ok );
    TESTCHECKF( angle90.Cos(), 0., &ok );
    try
    {
        TESTCHECKF( angle90.Tan(), infinity, &ok );
        cout << "Tan() should have thrown an exception." << endl;
        ok = false;
    }
    catch( LogicError & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    TESTCHECKF( angle90.Hav(), 0.5, &ok );
    cout << "Normalize()" << endl;
    angle90.Normalize();
    TESTCHECKF( angle90.Radians(), (M_PI / 2.), &ok );
    TESTCHECKF( angle90.Degrees(), 90., &ok );
    TESTCHECKF( angle90.Grads(), 100., &ok );
    TESTCHECKF( angle90.Cycles(), 0.25, &ok );
    TESTCHECKF( angle90.Sin(), 1., &ok );
    TESTCHECKF( angle90.Cos(), 0., &ok );
    try
    {
        TESTCHECKF( angle90.Tan(), infinity, &ok );
        cout << "Tan() should have thrown an exception." << endl;
        ok = false;
    }
    catch( LogicError & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    TESTCHECKF( angle90.Hav(), 0.5, &ok );
    cout << "NormalizePositive()" << endl;
    angle90.NormalizePositive();
    TESTCHECKF( angle90.Radians(), (M_PI / 2.), &ok );
    TESTCHECKF( angle90.Degrees(), 90., &ok );
    TESTCHECKF( angle90.Grads(), 100., &ok );
    TESTCHECKF( angle90.Cycles(), 0.25, &ok );
    TESTCHECKF( angle90.Sin(), 1., &ok );
    TESTCHECKF( angle90.Cos(), 0., &ok );
    try
    {
        TESTCHECKF( angle90.Tan(), infinity, &ok );
        cout << "Tan() should have thrown an exception." << endl;
        ok = false;
    }
    catch( LogicError & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    TESTCHECKF( angle90.Hav(), 0.5, &ok );
    double grads = 250.;
    cout << "Angle( " << grads << ", Grad ) [grads constructor]" << endl;
    Angle angle225( grads, Grad );
    TESTCHECKF( angle225.Radians(), (5. * M_PI / 4.), &ok );
    TESTCHECKF( angle225.Degrees(), 225., &ok );
    TESTCHECKF( angle225.Grads(), 250., &ok );
    TESTCHECKF( angle225.Cycles(), 0.625, &ok );
    TESTCHECKF( angle225.Sin(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Tan(), 1., &ok );
    TESTCHECKF( angle225.Hav(), 0.85355339, &ok );
    cout << "Normalize()" << endl;
    angle225.Normalize();
    TESTCHECKF( angle225.Radians(), (-3. * M_PI / 4.), &ok );
    TESTCHECKF( angle225.Degrees(), -135., &ok );
    TESTCHECKF( angle225.Grads(), -150., &ok );
    TESTCHECKF( angle225.Cycles(), -0.375, &ok );
    TESTCHECKF( angle225.Sin(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Tan(), 1., &ok );
    TESTCHECKF( angle225.Hav(), 0.85355339, &ok );
    cout << "NormalizePositive()" << endl;
    angle225.NormalizePositive();
    TESTCHECKF( angle225.Radians(), (5. * M_PI / 4.), &ok );
    TESTCHECKF( angle225.Degrees(), 225., &ok );
    TESTCHECKF( angle225.Grads(), 250., &ok );
    TESTCHECKF( angle225.Cycles(), 0.625, &ok );
    TESTCHECKF( angle225.Sin(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Tan(), 1., &ok );
    TESTCHECKF( angle225.Hav(), 0.85355339, &ok );
    cout << "Normalize()" << endl;
    angle225.Normalize();
    TESTCHECKF( angle225.Radians(), (-3. * M_PI / 4.), &ok );
    TESTCHECKF( angle225.Degrees(), -135., &ok );
    TESTCHECKF( angle225.Grads(), -150., &ok );
    TESTCHECKF( angle225.Cycles(), -0.375, &ok );
    TESTCHECKF( angle225.Sin(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Tan(), 1., &ok );
    TESTCHECKF( angle225.Hav(), 0.85355339, &ok );
    cout << "Abs()" << endl;
    Angle angle135 = angle225.Abs();
    TESTCHECKF( angle135.Radians(), (3. * M_PI / 4.), &ok );
    TESTCHECKF( angle135.Degrees(), 135., &ok );
    TESTCHECKF( angle135.Grads(), 150., &ok );
    TESTCHECKF( angle135.Cycles(), 0.375, &ok );
    TESTCHECKF( angle135.Sin(), sqrt( 0.5 ), &ok );
    TESTCHECKF( angle135.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle135.Tan(), -1., &ok );
    TESTCHECKF( angle135.Hav(), 0.85355339, &ok );
    TESTCHECK( (angle135 == - angle225), true, &ok );
    cout << "+= angle225" << endl;
    angle135 += angle225;
    TESTCHECK( angle135.Radians(), 0., &ok );
    TESTCHECK( angle135.Degrees(), 0., &ok );
    TESTCHECK( angle135.Grads(), 0., &ok );
    TESTCHECK( angle135.Cycles(), 0., &ok );
    TESTCHECK( angle135.Sin(), 0., &ok );
    TESTCHECK( angle135.Cos(), 1., &ok );
    TESTCHECK( angle135.Tan(), 0., &ok );
    TESTCHECKF( angle135.Hav(), 0., &ok );
    TESTCHECK( (angle135 == angle0), true, &ok );
    cout << "-= angle225" << endl;
    angle135 -= angle225;
    TESTCHECKF( angle135.Radians(), (3. * M_PI / 4.), &ok );
    TESTCHECKF( angle135.Degrees(), 135., &ok );
    TESTCHECKF( angle135.Grads(), 150., &ok );
    TESTCHECKF( angle135.Cycles(), 0.375, &ok );
    TESTCHECKF( angle135.Sin(), sqrt( 0.5 ), &ok );
    TESTCHECKF( angle135.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle135.Tan(), -1., &ok );
    TESTCHECKF( angle135.Hav(), 0.85355339, &ok );
    TESTCHECK( (angle225.Abs() == angle135), true, &ok );
    TESTCHECK( (angle90 < angle135), true, &ok );
    TESTCHECK( (angle225 < angle90), true, &ok );
    cout << "NormalizeOperators( false )" << endl;
    Angle::NormalizeOperators( false );
    cout << "angle135 + angle90" << endl;
    angle225 = angle135 + angle90;
    TESTCHECKF( angle225.Radians(), (5. * M_PI / 4.), &ok );
    TESTCHECKF( angle225.Degrees(), 225., &ok );
    TESTCHECKF( angle225.Grads(), 250., &ok );
    TESTCHECKF( angle225.Cycles(), 0.625, &ok );
    TESTCHECKF( angle225.Sin(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle225.Tan(), 1., &ok );
    TESTCHECKF( angle225.Hav(), 0.85355339, &ok );
    cout << "NormalizeOperators( [true] )" << endl;
    Angle::NormalizeOperators( );
    cout << "angle225 - angle90" << endl;
    angle135 = angle225 - angle90;
    TESTCHECKF( angle135.Radians(), (3. * M_PI / 4.), &ok );
    TESTCHECKF( angle135.Degrees(), 135., &ok );
    TESTCHECKF( angle135.Grads(), 150., &ok );
    TESTCHECKF( angle135.Cycles(), 0.375, &ok );
    TESTCHECKF( angle135.Sin(), sqrt( 0.5 ), &ok );
    TESTCHECKF( angle135.Cos(), - sqrt( 0.5 ), &ok );
    TESTCHECKF( angle135.Tan(), -1., &ok );
    TESTCHECKF( angle135.Hav(), 0.85355339, &ok );
    cout << "angle45 * 2." << endl;
    angle90 = angle45 * 2.;
    TESTCHECKF( angle90.Radians(), (M_PI / 2.), &ok );
    TESTCHECKF( angle90.Degrees(), 90., &ok );
    TESTCHECKF( angle90.Grads(), 100., &ok );
    TESTCHECKF( angle90.Cycles(), 0.25, &ok );
    TESTCHECKF( angle90.Sin(), 1., &ok );
    TESTCHECKF( angle90.Cos(), 0., &ok );
    try
    {
        TESTCHECKF( angle90.Tan(), infinity, &ok );
        cout << "Tan() should have thrown an exception." << endl;
        ok = false;
    }
    catch( LogicError & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    TESTCHECKF( angle90.Hav(), 0.5, &ok );
    TESTCHECKF( EpsilonDelta::Sin( angle90 / 2. ), sqrt( 0.5 ), &ok );
    TESTCHECKF( EpsilonDelta::Cos( angle90 / 2 ), sqrt( 0.5 ), &ok );
    TESTCHECKF( EpsilonDelta::Tan( angle90 / 2. ), 1., &ok );
    TESTCHECKF( EpsilonDelta::Hav( angle90 / 2 ), 0.1464466, &ok );
    cout << "angle90 / 2." << endl;
    angle45 = angle90 / 2.;
    TESTCHECKF( angle45.Radians(), (M_PI / 4.), &ok );
    TESTCHECKF( angle45.Degrees(), 45., &ok );
    TESTCHECKF( angle45.Grads(), 50., &ok );
    TESTCHECKF( angle45.Cycles(), 0.125, &ok );
    TESTCHECKF( EpsilonDelta::Sin( angle45 ), sqrt( 0.5 ), &ok );
    TESTCHECKF( EpsilonDelta::Cos( angle45 ), sqrt( 0.5 ), &ok );
    TESTCHECKF( EpsilonDelta::Tan( angle45 ), 1., &ok );
    TESTCHECKF( EpsilonDelta::Hav( angle45 ), 0.1464466, &ok );
    TESTCHECKF( EpsilonDelta::Sin( angle45 * 2.), 1., &ok );
    TESTCHECKF( EpsilonDelta::Cos( 2. * angle45 ), 0., &ok );
    try
    {
        TESTCHECKF( EpsilonDelta::Tan( angle45 * 2. ), infinity, &ok );
        cout << "Tan() should have thrown an exception." << endl;
        ok = false;
    }
    catch( LogicError & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    TESTCHECKF( EpsilonDelta::Hav( 2. * angle45 ), 0.5, &ok );
    double ss = 0.5;
    cout << "ArcSin( " << ss << " )" << endl;
    Angle angle30 = ArcSin( ss );
    TESTCHECKF( angle30.Radians(), (M_PI / 6.), &ok );
    TESTCHECKF( angle30.Degrees(), 30., &ok );
    TESTCHECKF( angle30.Grads(), 100./3., &ok );
    TESTCHECKF( angle30.Cycles(), 1./12., &ok );
    TESTCHECKF( angle30.Sin(), ss, &ok );
    TESTCHECKF( angle30.Cos(), 0.8660254, &ok );
    TESTCHECKF( angle30.Tan(), 0.5773503, &ok );
    TESTCHECKF( angle30.Hav(), 0.0669873, &ok );
    double c = -0.5;
    cout << "ArcCos( " << c << " )" << endl;
    Angle angle120 = ArcCos( c );
    TESTCHECKF( angle120.Radians(), (2. * M_PI / 3.), &ok );
    TESTCHECKF( angle120.Degrees(), 120., &ok );
    TESTCHECKF( angle120.Grads(), 400./3., &ok );
    TESTCHECKF( angle120.Cycles(), 1./3., &ok );
    TESTCHECKF( angle120.Sin(), 0.8660254, &ok );
    TESTCHECKF( angle120.Cos(), c, &ok );
    TESTCHECKF( angle120.Tan(), -1.7320508, &ok );
    TESTCHECKF( angle120.Hav(), ((1-c)/2.), &ok );
    ss = -.5;
    c = angle30.Cos();
    cout << "ArcTan( " << ss << ", " << c << " )" << endl;
    angle30 = ArcTan( ss, c );
    TESTCHECKF( angle30.Radians(), (- M_PI / 6.), &ok );
    TESTCHECKF( angle30.Degrees(), -30., &ok );
    TESTCHECKF( angle30.Grads(), -100./3., &ok );
    TESTCHECKF( angle30.Cycles(), -1./12., &ok );
    TESTCHECKF( angle30.Sin(), ss, &ok );
    TESTCHECKF( angle30.Cos(), c, &ok );
    TESTCHECKF( angle30.Tan(), (ss / c), &ok );
    TESTCHECKF( angle30.Hav(), ((1-c)/2.), &ok );
    double t = 1.;
    cout << "ArcTan( " << t << " )" << endl;
    angle45 = ArcTan( t );
    TESTCHECKF( angle45.Radians(), (M_PI / 4.), &ok );
    TESTCHECKF( angle45.Degrees(), 45., &ok );
    TESTCHECKF( angle45.Grads(), 50., &ok );
    TESTCHECKF( angle45.Cycles(), 1./8., &ok );
    TESTCHECKF( angle45.Sin(), sqrt( 0.5 ), &ok );
    TESTCHECKF( angle45.Cos(), sqrt( 0.5 ), &ok );
    TESTCHECKF( angle45.Tan(), 1., &ok );
    TESTCHECKF( angle45.Hav(), 0.1464466, &ok );
    ss = -2.;
    c = 0.;
    cout << "ArcTan( " << ss << ", " << c << " )" << endl;
    Angle angle270 = ArcTan( ss, c );
    TESTCHECKF( angle270.Radians(), (- M_PI / 2.), &ok );
    TESTCHECKF( angle270.Degrees(), -90., &ok );
    TESTCHECKF( angle270.Grads(), -100., &ok );
    TESTCHECKF( angle270.Cycles(), -0.25, &ok );
    TESTCHECKF( angle270.Sin(), -1., &ok );
    TESTCHECKF( angle270.Cos(), 0., &ok );
    try
    {
        TESTCHECKF( angle270.Tan(), infinity, &ok );
        cout << "Tan() should have thrown an exception." << endl;
        ok = false;
    }
    catch( LogicError & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.Description() << endl;
    }
    TESTCHECKF( angle270.Hav(), 0.5, &ok );
    double hh = 0.5;;
    cout << "ArcHav( " << hh << " )" << endl;
    angle90 = ArcHav( hh );
    TESTCHECKF( angle90.Radians(), (M_PI / 2.), &ok );
    TESTCHECKF( angle90.Degrees(), 90., &ok );
    TESTCHECKF( angle90.Grads(), 100., &ok );
    TESTCHECKF( angle90.Cycles(), 0.25, &ok );
    TESTCHECKF( angle90.Sin(), 1., &ok );
    TESTCHECKF( angle90.Cos(), 0., &ok );
    TESTCHECKF( angle90.Hav(), hh, &ok );
    int d = 57;
    int m = 17;
    double s = 44.806247;
    cout << "Angle( AngleDMS( " << d << ", " << m << ", " << s << " ) )" << endl;
    Angle angle1( AngleDMS( d, m, s ) );
    TESTCHECKF( angle1.Radians(), 1., &ok );
    TESTCHECKF( angle1.Degrees(), 57.2957795, &ok );
    TESTCHECKF( angle1.Sin(), 0.8414710, &ok );
    cout << "AngleDMS( angle1 )" << endl;
    AngleDMS angleDMS( angle1 );
    TESTCHECK( angleDMS.Degrees(), d, &ok );
    TESTCHECK( angleDMS.Minutes(), m, &ok );
    TESTCHECKF( angleDMS.Seconds(), s, &ok );
    int h = 1;
    m = 30;
    s = 0.;
    cout << "Angle( AngleHMS( " << h << ", " << m << ", " << s << " ) )" << endl;
    Angle angle22( AngleHMS( h, m, s ) );
    TESTCHECKF( angle22.Radians(), (M_PI / 8.), &ok );
    TESTCHECKF( angle22.Degrees(), 22.5, &ok );
    TESTCHECKF( angle22.Grads(), 25., &ok );
    cout << "AngleHMS( angle22 )" << endl;
    AngleHMS angleHMS( angle22 );
    TESTCHECK( angleHMS.Hours(), h, &ok );
    TESTCHECK( angleHMS.Minutes(), m, &ok );
    TESTCHECKF( angleHMS.Seconds(), s, &ok );

    vector<double> sinTable;
    vector<double> cosTable;
    cout << "MakeSinCosTables( 24, ... ) [1 div = 15 deg]" << endl;
    MakeSinCosTables( 24, &sinTable, &cosTable );
    TESTCHECK( sinTable.size(), 25, &ok );
    TESTCHECK( cosTable.size(), 25, &ok );
    TESTCHECKF( sinTable[0], 0., &ok );
    TESTCHECKF( cosTable[0], 1., &ok );
    TESTCHECKF( sinTable[2], 0.5, &ok );
    TESTCHECKF( sinTable[3], sqrt( 0.5 ), &ok );
    TESTCHECKF( cosTable[3], sqrt( 0.5 ), &ok );
    TESTCHECKF( cosTable[4], 0.5, &ok );
    TESTCHECKF( sinTable[6], 1., &ok );
    TESTCHECKF( cosTable[6], 0., &ok );
    TESTCHECKF( sinTable[9], sqrt( 0.5 ), &ok );
    TESTCHECKF( cosTable[9], - sqrt( 0.5 ), &ok );
    TESTCHECKF( sinTable[12], 0., &ok );
    TESTCHECKF( cosTable[12], -1., &ok );
    TESTCHECKF( sinTable[18], -1., &ok );
    TESTCHECKF( cosTable[18], 0., &ok );
    TESTCHECKF( sinTable[24], 0., &ok );
    TESTCHECKF( cosTable[24], 1., &ok );

    ostringstream ost;
    ost.precision( 2 );
    cout << "operator<<" << endl;
    ost << angle90;
    TESTCHECK( ost.str(), string( "90°" ), &ok );

    angle1.Set( 1. );
    TESTCHECK( ToJSON( angle1 ), string( "+1.00000000000000e+000" ), &ok );
    cout << "FromJSON( \"1\", &angle1 )" << endl;
    FromJSON( "1", &angle1 );
    TESTCHECKF( angle1.Radians(), 1., &ok );
    TESTCHECKF( angle1.Degrees(), 57.2957795, &ok );

    if ( ok )
        cout << "Angle PASSED." << endl << endl;
    else
        cout << "Angle FAILED." << endl << endl;
    return ok;
}

#endif

//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  EulerAngles.cpp
  Copyright (C) 2007 David M. Anderson

  EulerAngles class: a triplet of angles.
  Includes EOrder: an enumeration of axis orders.
*/


#include "EulerAngles.hpp"
#include "JSON.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include <sstream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


ostream & 
operator<<( ostream & out, const EulerAngles & euler )
{
    return out << "( " << euler.At( 0 ) << ", " << euler.At( 1 ) << ", "
               << euler.At( 2 ) << " )";
}

//=============================================================================

string ToJSON( const EulerAngles & euler )
{
    return ToJSON( euler.m_angles );
}

//-----------------------------------------------------------------------------

void 
FromJSON( const string & json, EulerAngles * pEuler )
{
    FromJSON( json, &(pEuler->m_angles) );
}

//=============================================================================

#ifdef DEBUG

bool 
EulerAngles::Test( )
{
    bool ok = true;
    cout << "Testing EulerAngles" << endl;

    double a0 = 0.1;
    double a1 = -1.2;
    double a2 = 2.3;
    cout << "EulerAngles( Angle( " << a0 << " ), Angle( " << a1
         << " ), Angle( " << a2 << " ) )" << endl;
    //!!!Hmmm. The compiler thinks this is a function declaration:
//    EulerAngles euler0( Angle( a0 ), Angle( a1 ), Angle( a2 ) );
    // But we can do this:
    EulerAngles euler0( a0, a1,  a2 );
    // or even this:
//    EulerAngles euler0( a0, Angle( a1 ), Angle( a2 ) );
    TESTCHECK( euler0[0].Radians(), a0, &ok );
    TESTCHECK( euler0[1].Radians(), a1, &ok );
    TESTCHECK( euler0[2].Radians(), a2, &ok );
    TESTCHECK( euler0.At( 0 ).Radians(), a0, &ok );
    TESTCHECK( euler0.At( 1 ).Radians(), a1, &ok );
    TESTCHECK( euler0.At( 2 ).Radians(), a2, &ok );
    try
    {
        TESTCHECK( euler0.At( -1 ).Radians(), 0., &ok );
        cout << "At( -1 ) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    try
    {
        TESTCHECK( euler0.At( 3 ).Radians(), 0., &ok );
        cout << "At( 3 ) should have thrown an exception." << endl;
        ok = false;
    }
    catch( out_of_range & exceptn )
    {
        cout << "Exception here is OK" << endl;
        cout << exceptn.what() << endl;
    }
    Angle angles[3] = { -2.3, 1.2, -0.1 };
    cout << "EulerAngles( { " << angles[0].Radians() << ", "
         << angles[1].Radians() << ", " << angles[2].Radians()
         << " } ) [array constructor]" << endl;
    EulerAngles euler1( angles );
    TESTCHECK( euler1[0].Radians(), angles[0].Radians(), &ok );
    TESTCHECK( euler1[1].Radians(), angles[1].Radians(), &ok );
    TESTCHECK( euler1[2].Radians(), angles[2].Radians(), &ok );

    angles[0].Set( 45., Angle::Degree );
    angles[1].Set( 145., Angle::Degree );
    angles[2].Set( 245., Angle::Degree );
    euler1.Set( angles );
    ostringstream ost;
    cout << "operator<<" << endl;
    ost << euler1;
    TESTCHECK( ost.str(), string( "( 45°, 145°, 245° )" ), &ok );

    a0 = 0.5;
    a1 = 1.25;
    a2 = 2.75;
    euler0.Set( a0, a1, a2 );
    TESTCHECK( ToJSON( euler0 ),
               string( "[ +5.00000000000000e-001, +1.25000000000000e+000, "
                       "+2.75000000000000e+000 ]" ),
               &ok );
    cout << "FromJSON( ToJSON( euler0 ), &euler1 )" << endl;
    FromJSON( ToJSON( euler0 ), &euler1 );
    TESTCHECK( euler1[0].Radians(), a0, &ok );
    TESTCHECK( euler1[1].Radians(), a1, &ok );
    TESTCHECK( euler1[2].Radians(), a2, &ok );

    if ( ok )
        cout << "EulerAngles PASSED." << endl << endl;
    else
        cout << "EulerAngles FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta


/*
  Array.cpp
  Copyright (C) 2007 David M. Anderson

  ARRAY_LENGTH macro for built-in arrays.
  array template class: STL-compatible container using a built-in array.
  Allocators for 2-dimensional and 3-dimensional arrays.
*/


#include "Array.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Exception.hpp"
#include <tr1/array>
#include <iostream>
#include <cstring>
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

#ifdef DEBUG

class TestClass
{
public:
    TestClass( int i = 0, long double ld = 0., const char ch[5] = "" );
    void Set( int i = 0, long double ld = 0., const char ch[5] = "" );
    int I( ) const;
    long double LD( ) const;
    const char * Ch( ) const;

private:
    int m_i;
    long double m_ld;
    char m_ch[5];
};

//.............................................................................

bool operator==( const TestClass & lhs, const TestClass & rhs );
bool operator<( const TestClass & lhs, const TestClass & rhs );

//.............................................................................

TestClass::TestClass( int i, long double ld, const char ch[5] )
    :   m_i( i ),
        m_ld( ld )
{
    memcpy( m_ch, ch, sizeof(m_ch) );
}

//.............................................................................

void 
TestClass::Set( int i, long double ld, const char ch[5] )
{
    m_i = i;
    m_ld = ld;
    memcpy( m_ch, ch, sizeof(m_ch) );
}

//.............................................................................

int 
TestClass::I( ) const
{
    return  m_i;
}

//.............................................................................

long double 
TestClass::LD( ) const
{
    return  m_ld;
}

//.............................................................................

const char * 
TestClass::Ch( ) const
{
    return  m_ch;
}

//.............................................................................

bool 
operator==( const TestClass & lhs, const TestClass & rhs )
{
    return  ((lhs.I() == rhs.I()) && (lhs.LD() == rhs.LD()));
}

//.............................................................................

bool 
operator<( const TestClass & lhs, const TestClass & rhs )
{
    if ( lhs.I() < rhs.I() )
        return true;
    if ( lhs.LD() < rhs.LD() )
        return true;
    return false;
}

#endif

//=============================================================================

#ifdef DEBUG

bool TestArray( )
{
    bool ok = true;
    cout << "Testing Array" << endl;

    cout << "TestClass plainArray[3]  [plain array]" << endl;
    TestClass plainArray[3];
    TESTCHECK( ARRAY_LENGTH( plainArray ), 3, &ok );

    cout << "array< TestClass, 3 > array0  [default c'tor]" << endl;
    std::tr1::array< TestClass, 3 > array0
            = { { TestClass( 0, 0., "zero" ), TestClass( 1, 1., "one" ),
                TestClass( 2, 2., "two" ) } };
    TESTCHECK( array0[0].I(), 0, &ok );
    TESTCHECK( array0[1].LD(), 1.L, &ok );
    TESTCHECK( array0[2].Ch(), "two", &ok );
    TESTCHECK( array0.at(0).Ch(), "zero", &ok );
    TESTCHECK( array0.at(1).I(), 1, &ok );
    TESTCHECK( array0.at(2).LD(), 2.L, &ok );
    try
    {
        TESTCHECK( array0.at(-1).I(), -1, &ok );  //int->size_t warning OK
        cout << "at(-1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        TESTCHECK( array0.at(3).I(), 3, &ok );
        cout << "at(3) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    cout << "array0[0] = TestClass( 100, 100., \"nil\" )" << endl;
    array0[0] = TestClass( 100, 100., "nil" );
    TESTCHECK( array0[0].I(), 100, &ok );
    cout << "array0.at(1) = TestClass( 111, 111.1, \"uno\" )" << endl;
    array0.at(1) = TestClass( 111, 111.1, "uno" );
    TESTCHECK( array0.at(1).I(), 111, &ok );
    TESTCHECK( array0.front().I(), 100, &ok );
    cout << "array0.back() = TestClass( 222, 222.2, \"dos\" )" << endl;
    array0.back() = TestClass( 222, 222.2, "dos" );
    TESTCHECK( array0.back().I(), 222, &ok );
    TESTCHECK( array0.begin()->I(), 100, &ok );
    TESTCHECK( (array0.end() - 1)->I(), 222, &ok );
    TESTCHECK( array0.rbegin()->I(), 222, &ok );
    TESTCHECK( (array0.rend() - 1)->I(), 100, &ok );
    TESTCHECK( array0.begin(), static_cast< TestClass * >( &array0[0] ), &ok );
    TESTCHECK( array0.size(), 3, &ok );
    std::tr1::array< TestClass, 3 > array1
            = { { TestClass( 100, 100., "zero" ),
                  TestClass( 111, 111.1, "one" ),
                  TestClass( 222, 222.2, "two" ) } };
    TESTCHECK( (array1 == array0), true, &ok );
    TESTCHECK( (array1 < array0), false, &ok );
    array1[2] = TestClass( 222, -222.2, "-two" );
    TESTCHECK( (array1 == array0), false, &ok );
    TESTCHECK( (array1 < array0), true, &ok );

    if ( ok )
        cout << "Array PASSED." << endl << endl;
    else
        cout << "Array FAILED." << endl << endl;
    return ok;
}

#endif

//=============================================================================

#ifdef DEBUG

bool TestTwoDArray( )
{
    bool ok = true;
    cout << "Testing TwoDArray" << endl;

    cout << "TwoDArray< TestClass >( 3, 2 )" << endl;
    TwoDArray< TestClass > tcArray( 3, 2 );

    TESTCHECK( tcArray.NumRows(), 3, &ok );
    TESTCHECK( tcArray.NumColumns(), 2, &ok );
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 2; ++j )
        {
            cout << "tcArray[" << i << "][" << j << "].Set( "<< i << ", "
                 << j * 0.1 << " )" << endl;
            tcArray[i][j].Set( i, j * 0.1 );
        }
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 2; ++j )
            TESTCHECK( tcArray[i][j].I(), i, &ok );
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 2; ++j )
        {
            cout << "tcArray( " << i << ", " << j << " ).Set( "<< i << ", "
                 << j * 0.1 << " )" << endl;
            tcArray( i, j ).Set( i, j * 0.1 );
        }
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 2; ++j )
            TESTCHECK( tcArray( i, j ).I(), i, &ok );
    try
    {
        tcArray.at( -1, 1 );
        cout << "at(-1,1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 1, -1 );
        cout << "at(1,-1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 3, 1 );
        cout << "at(3,1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 1, 2 );
        cout << "at(1,) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 2; ++j )
        {
            cout << "at( " << i << ", " << j << " ).Set( "<< i << ", "
                 << j * 0.1 << " )" << endl;
            tcArray.at( i, j ).Set( i, j * 0.1 );
        }
    for ( int i = 0; i < 3; ++i )
        for ( int j = 0; j < 2; ++j )
            TESTCHECK( tcArray.at( i, j ).I(), i, &ok );

    if ( ok )
        cout << "TwoDArray PASSED." << endl << endl;
    else
        cout << "TwoDArray FAILED." << endl << endl;
    return ok;
}

#endif

//-----------------------------------------------------------------------------

#ifdef DEBUG

bool TestThreeDArray( )
{
    bool ok = true;
    cout << "Testing ThreeDArray" << endl;

    cout << "ThreeDArray< TestClass >( 4, 3, 2 )" << endl;
    ThreeDArray< TestClass > tcArray( 4, 3, 2 );

    TESTCHECK( tcArray.NumSlices(), 4, &ok );
    TESTCHECK( tcArray.NumRows(), 3, &ok );
    TESTCHECK( tcArray.NumColumns(), 2, &ok );
    for ( int s = 0; s < 4; ++s )
        for ( int i = 0; i < 3; ++i )
            for ( int j = 0; j < 2; ++j )
            {
                cout << "tcArray[" << s << "][" << i << "][" << j
                     << "].Set( " << s + i << ", " << j * 0.1 << " )" << endl;
                tcArray[s][i][j].Set( s + i, j * 0.1 );
            }
    for ( int s = 0; s < 4; ++s )
        for ( int i = 0; i < 3; ++i )
            for ( int j = 0; j < 2; ++j )
                TESTCHECK( tcArray[s][i][j].I(), s + i, &ok );
    for ( int s = 0; s < 4; ++s )
        for ( int i = 0; i < 3; ++i )
            for ( int j = 0; j < 2; ++j )
            {
                cout << "tcArray( " << s << ", " << i << ", " << j
                     << " ).Set( " << s + i << ", " << j * 0.1 << " )" << endl;
                tcArray( s, i, j ).Set( s + i, j * 0.1 );
            }
    for ( int s = 0; s < 4; ++s )
        for ( int i = 0; i < 3; ++i )
            for ( int j = 0; j < 2; ++j )
                TESTCHECK( tcArray( s, i, j ).I(), s + i, &ok );
    try
    {
        tcArray.at( -1, 1, 1 );
        cout << "at(-1,1,1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 1, -1, 1 );
        cout << "at(1,-1,1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 1, 1, -1 );
        cout << "at(1,1,-1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 4, 1, 1 );
        cout << "at(4,1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 1, 3, 1 );
        cout << "at(1,3,1) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    try
    {
        tcArray.at( 1, 1, 2 );
        cout << "at(1,1,2) should have thrown an exception." << endl;
        ok = false;
    }
    catch ( exception & except )
    {
        cout << "Assertion here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    for ( int s = 0; s < 4; ++s )
        for ( int i = 0; i < 3; ++i )
            for ( int j = 0; j < 2; ++j )
            {
                cout << "at( " << s << ", " << i << ", " << j
                     << " ).Set( " << s + i << ", " << j * 0.1 << " )" << endl;
                tcArray.at( s, i, j ).Set( s + i, j * 0.1 );
            }
    for ( int s = 0; s < 4; ++s )
        for ( int i = 0; i < 3; ++i )
            for ( int j = 0; j < 2; ++j )
                TESTCHECK( tcArray.at( s, i, j ).I(), s + i, &ok );

    if ( ok )
        cout << "ThreeDArray PASSED." << endl << endl;
    else
        cout << "ThreeDArray FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

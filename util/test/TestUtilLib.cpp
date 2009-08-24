/*
  TestUtilLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta util library.
*/


#include "Exception.hpp"
#include "TestCheck.hpp"
#include "Logger.hpp"
#include "FixEndian.hpp"
#include "StringUtil.hpp"
#include "UnicodeUtil.hpp"
#include "CSV.hpp"
#include "JSON.hpp"
#include "SmartPtr.hpp"
#include "Array.hpp"
#include "VMap.hpp"
#include "IndexedVector.hpp"
#include "Algorithms.hpp"
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <stack>
#include <queue>
#include <map>
#include <set>
using namespace std;
using namespace EpsilonDelta;


int Main( int /*argc*/, char ** /*argv*/ );
void DisplayDataSizes( );


//*****************************************************************************


int main( int argc, char ** argv )
{
    try
    {
        int res = Main( argc, argv );
        return res;
    }
    catch ( Exception & except )
    {
        cout << except.Description( ) << endl;
    }
    catch ( exception & except )
    {
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    catch ( ... )
    {
        Exception exc( "Unknown exception!" ); //!!!
        cout << exc.Description( ) << endl;
    }
    return  2;
}

//-----------------------------------------------------------------------------

int Main( int /*argc*/, char ** /*argv*/ )
{
    bool ok = true;

#ifdef DEBUG
    if ( ! Logger::Test( ) )
        ok = false;
    if ( ! TestFixEndian( ) )
        ok = false;
    if ( ! TestStringUtil( ) )
        ok = false;
    if ( ! TestUnicodeUtil( ) )
        ok = false;
    if ( ! TestCSV( ) )
        ok = false;
    if ( ! TestJSON( ) )
        ok = false;
    if ( ! TestSmartPtr( ) )
        ok = false;
    if ( ! TestArray( ) )
        ok = false;
    if ( ! TestTwoDArray( ) )
        ok = false;
    if ( ! TestThreeDArray( ) )
        ok = false;
    if ( ! TestVMap( ) )
        ok = false;
    if ( ! TestIndexedVector( ) )
        ok = false;
    if ( ! TestAlgorithms( ) )
        ok = false;
#endif //DEBUG

    DisplayDataSizes( );

    if ( ok )
    {
        cout << endl << "All tests PASSED!" << endl;
        return  0;
    }
    else
    {
        cout << endl << "Some test(s) FAILED!" << endl;
        return  1;
    }
}

//=============================================================================

void 
DisplayDataSizes( )
{
    cout << endl << "DataSizes:" << endl;
//Illegal:    cout << "sizeof( void ) = " << sizeof( void ) << endl;
    cout << "sizeof( bool ) = " << sizeof( bool ) << endl;
    cout << "sizeof( char ) = " << sizeof( char ) << endl;
    cout << "sizeof( wchar_t ) = " << sizeof( wchar_t ) << endl;
    cout << "sizeof( short int ) = " << sizeof( short int ) << endl;
    cout << "sizeof( int ) = " << sizeof( int ) << endl;
    cout << "sizeof( long int ) = " << sizeof( long int ) << endl;
    //Not standard:
    cout << "sizeof( long long int ) = " << sizeof( long long int ) << endl;
    cout << "sizeof( float ) = " << sizeof( float ) << endl;
    cout << "sizeof( double ) = " << sizeof( double ) << endl;
    cout << "sizeof( long double ) = " << sizeof( long double ) << endl;
    enum ESmall { sZero, sOne };
    cout << "sizeof( enum { sZero, sOne } ) = " << sizeof( ESmall ) << endl;
    enum EMedium { mZero, mThousand = 1000 };
    cout << "sizeof( enum { mZero, mThousand = 1000 } ) = " << sizeof( EMedium ) << endl;
    enum ELarge { lZero, lMillion = 1000000 };
    cout << "sizeof( enum { lZero, lMillion = 1000000 } ) = " << sizeof( ELarge ) << endl;
    cout << "sizeof( void * ) = " << sizeof( void * ) << endl;
    cout << "sizeof( char * ) = " << sizeof( char * ) << endl;
    cout << "sizeof( long * ) = " << sizeof( long * ) << endl;
    cout << "sizeof( long double * ) = " << sizeof( long double * ) << endl;
    cout << "sizeof( char & ) = " << sizeof( char & ) << endl;
    cout << "sizeof( int & ) = " << sizeof( int & ) << endl;
    cout << "sizeof( std::string ) = " << sizeof( std::string ) << endl;
    std::string textString( "text" );
    cout << "sizeof( std::string( \"text\" ) = " << sizeof( textString ) << endl;
    cout << "sizeof( std::wstring ) = " << sizeof( std::wstring ) << endl;
    cout << "sizeof( std::vector< char > ) = " << sizeof( std::vector< char > ) << endl;
    cout << "sizeof( std::vector< std::string > ) = " << sizeof( std::vector< std::string > ) << endl;
    cout << "sizeof( std::list< char > ) = " << sizeof( std::list< char > ) << endl;
    cout << "sizeof( std::deque< char > ) = " << sizeof( std::deque< char > ) << endl;
    cout << "sizeof( std::stack< char > ) = " << sizeof( std::stack< char > ) << endl;
    cout << "sizeof( std::queue< char > ) = " << sizeof( std::queue< char > ) << endl;
    cout << "sizeof( std::priority_queue< char > ) = " << sizeof( std::priority_queue< char > ) << endl;
    cout << "sizeof( std::map< std::string, int > ) = " << sizeof( std::map< std::string, int > ) << endl;
    cout << "sizeof( std::multimap< std::string, int > ) = " << sizeof( std::multimap< std::string, int > ) << endl;
    cout << "sizeof( std::set< int > ) = " << sizeof( std::set< int >  ) << endl;
    cout << "sizeof( std::multiset< int > ) = " << sizeof( std::multiset< int > ) << endl;
    cout << "sizeof( array< char, 7 > ) = " << sizeof( array< char, 7 > ) << endl;
    cout << "sizeof( EpsilonDelta::VMap< std::string, int > ) = "
         << sizeof( EpsilonDelta::VMap< std::string, int > ) << endl;
    cout << "sizeof( EpsilonDelta::IndexedVector< float > ) = "
         << sizeof( EpsilonDelta::IndexedVector< float > ) << endl;
    cout << endl;
}


//*****************************************************************************

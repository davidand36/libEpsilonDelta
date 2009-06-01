/*
  TestSQLLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta sql library.
*/


#include "Exception.hpp"
#ifdef USE_SQLITE
#include "SQLiteDatabase.hpp"
#endif
#ifdef USE_MYSQL
#include "MySQLDatabase.hpp"
#endif
#ifdef USE_POSTGRESQL
#include "PostgreSQLDatabase.hpp"
#endif
#include <cstdio>
#include <iostream>
using namespace std;
using namespace EpsilonDelta;


int Main( int /*argc*/, char ** /*argv*/ );


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

int Main( int /*argc*/, char ** argv )
{
    bool ok = true;

#ifdef DEBUG
    string libBasePath = argv[0];
    int slashPos = libBasePath.rfind( '/' );
    if ( slashPos == static_cast<int>( string::npos ) )
        libBasePath = "";
    else
        libBasePath.erase( slashPos + 1 );
    libBasePath += "../";

#ifdef USE_SQLITE
    if ( ! SQLiteDatabase::Test( ) )
        ok = false;
#endif
#ifdef USE_MYSQL
    if ( ! MySQLDatabase::Test( libBasePath
                                + "sql/test/MySQL.config" ) )
        ok = false;
#endif
#ifdef USE_POSTGRESQL
    if ( ! PostgreSQLDatabase::Test( libBasePath
                                     + "sql/test/PostgreSQL.config" ) )
        ok = false;
#endif
#endif //DEBUG

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


//*****************************************************************************

/*
  SQLiteDatabase.cpp
  Copyright (C) 2007 David M. Anderson

  SQLiteDatabase class, representing a database opened using SQLite.
*/


#ifdef USE_SQLITE


#include "SQLiteDatabase.hpp"
#include "SQLiteException.hpp"
#include "SQLiteResult.hpp"
#include "Assert.hpp"
#include "StringUtil.hpp"
#include <sqlite3.h>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Platform.hpp"
#include "DirUtil.hpp"
#include <iostream>
#include <cstdlib>
#ifndef OS_WINDOWS
#include <unistd.h>
#endif
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


SQLiteDatabase::SQLiteDatabase( const string & fileName )
    :   m_sqlDB( 0 )
{
    int openRslt = sqlite3_open( fileName.c_str(), &m_sqlDB );
    if ( openRslt != SQLITE_OK )
    {
        SQLiteException exception( sqlite3_errmsg( m_sqlDB ) );
        sqlite3_close( m_sqlDB );
        m_sqlDB = 0;
        throw exception;
    }
}

//-----------------------------------------------------------------------------

SQLiteDatabase::~SQLiteDatabase( )
{
    if ( m_sqlDB != 0 )
    {
        int closeRslt = sqlite3_close( m_sqlDB );
        //Note: this can fail if there are prepared statements that have not
        // been finalized.
        Assert( closeRslt == SQLITE_OK );
    }
}

//=============================================================================

void 
SQLiteDatabase::DoCommand( const string & command )
{
    char * errorMsg = 0;
    int execRslt = sqlite3_exec( m_sqlDB, command.c_str(), 0, 0, &errorMsg );
    if ( errorMsg != 0 )
    {
        string message = errorMsg;
        sqlite3_free( errorMsg );
        message += "\nCommand was:\n";
        message += command;
        SQLiteException exception( message );
        throw exception;
    }
    if ( execRslt != SQLITE_OK )
    {
        string message = sqlite3_errmsg( m_sqlDB );
        message += "\nCommand was:\n";
        message += command;
        SQLiteException exception( message );
        throw exception;
    }
}

//-----------------------------------------------------------------------------

shared_ptr< SQLResult >
SQLiteDatabase::DoQuery( const string & query )
{
    sqlite3_stmt * pStmt = 0;
    shared_ptr< SQLiteResult > pRslt;
    int stepRslt = 0;
    const char * queryTail;
    int prepareRslt = sqlite3_prepare( m_sqlDB, query.c_str(), query.size(),
                                       &pStmt, &queryTail );
    if ( prepareRslt != SQLITE_OK )
        goto error;
    stepRslt = sqlite3_step( pStmt );
    if ( (stepRslt != SQLITE_ROW) && (stepRslt != SQLITE_DONE) )
        goto error;
    pRslt.reset( new SQLiteResult( pStmt ) );
    while ( stepRslt == SQLITE_ROW )
    {
        pRslt->AddRecord( pStmt );
        stepRslt = sqlite3_step( pStmt );
    }
    if ( stepRslt == SQLITE_DONE )
    {
        sqlite3_finalize( pStmt );
        return  pRslt;
    }

 error:
    if ( pStmt != 0 )
        sqlite3_finalize( pStmt );
    string message = sqlite3_errmsg( m_sqlDB );
    message += "\nQuery was:\n";
    message += query;
    SQLiteException exception( message );
    throw exception;
}

//=============================================================================

vector< string > 
SQLiteDatabase::ListTables( )
{
    string query = "SELECT name FROM sqlite_master"
            " WHERE type == 'table' AND name NOT LIKE 'sqlite_%'"
            " ORDER BY name";
    shared_ptr< SQLResult > pTableResult = DoQuery( query );
    int numTables = pTableResult->NumRecords();
    vector< string > tableList;
    for ( int t = 0; t < numTables; ++t )
        tableList.push_back( pTableResult->Value( t, 0 ) );
    return tableList;
}

//=============================================================================

string 
SQLiteDatabase::DumpSQL( )
{
    string sqlDump = "BEGIN TRANSACTION;\n";
    string query = "SELECT name, sql FROM sqlite_master"
            " WHERE sql NOT NULL AND type == 'table' AND rootpage != 0";
    shared_ptr< SQLResult > pMasterResult = DoQuery( query );
    int numTables = pMasterResult->NumRecords();
    for ( int t = 0; t < numTables; ++t )
    {
        const string & tableName = pMasterResult->Value( t, 0 );
        const string & createSQL = pMasterResult->Value( t, 1 );
        if ( tableName.compare( 0, 7, "sqlite_" ) == 0 )
            continue;

        sqlDump += createSQL + ";\n";

        query = "PRAGMA table_info( " + tableName + " )";
        shared_ptr< SQLResult > pTableInfoResult = DoQuery( query );
        int numFields = pTableInfoResult->NumRecords();

        query = "SELECT 'INSERT INTO " + AddQuotes( tableName, '\"' )
                + " VALUES(' || ";
        for ( int f = 0; f < numFields; ++f )
        {
            const string & fieldName = pTableInfoResult->Value( f, 1 );
            query += "quote(" + AddQuotes( fieldName, '\"' ) + ") ";
            if ( f < numFields - 1 )
                query += "|| ', ' || ";
        }
        query += "|| ')' FROM  " + AddQuotes( tableName, '\"' );
        shared_ptr< SQLResult > pSelectResult = DoQuery( query );
        int numRecords = pSelectResult->NumRecords();

        for ( int r = 0; r < numRecords; ++r )
            sqlDump += pSelectResult->Value( r, 0 ) + ";\n";
    }
    sqlDump += "COMMIT;\n";
    return sqlDump;
}

//=============================================================================

#ifdef DEBUG

bool
SQLiteDatabase::Test( )
{
    bool ok = true;
    cout << "Testing SQLiteDatabase" << endl;

    const char dbFileName[] = "test.sqlite3";
    DeleteFile( dbFileName ); //In case it already exists

    cout << "SQLiteDatabase( \"" << dbFileName << "\" )" << endl;
    shared_ptr< SQLDatabase > pDB( new SQLiteDatabase( dbFileName ) );

    string command = "CREATE TABLE Troops\n"
            "    (\n"
            "        Name            TEXT NOT NULL,\n"
            "        Rank            TEXT,\n"
            "        SerialNumber    INTEGER PRIMARY KEY,\n"
            "        Company         TEXT DEFAULT 'C'\n"
            "    )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "INSERT INTO Troops"
            "   (Name, SerialNumber, Company)"
            "   VALUES ( 'Ailey, Alvin', 5432, 'A' )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "INSERT INTO Troops"
            "   (Name, Rank, SerialNumber, Company)"
            "   VALUES ( 'Boy, Bugle', 'PFC', 5432, 'B' )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    try
    {
        pDB->DoCommand( command );
        cout << "DoCommand() should have thrown an exception." << endl;
        ok = false;
    }
    catch ( SQLiteException & except )
    {
        cout << "Exception here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }

    command = "INSERT INTO Troops"
            "   (Name, Rank, SerialNumber, Company)"
            "   VALUES ( 'Boy, Bugle', 'PFC', 3456, 'B' )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "INSERT INTO Troops"
            "   (Name, Rank, SerialNumber)"
            "   VALUES ( 'Arms, At', 'Sergeant', 3142 )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "INSERT INTO Troops"
            "   VALUES ( 'Crunch, Kangaroo', 'Captain', 6789, 'C' )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "INSERT INTO Troops"
            "   (SerialNumber)"
            "   VALUES ( 1010 )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    try
    {
        pDB->DoCommand( command );
        cout << "DoCommand() should have thrown an exception." << endl;
        ok = false;
    }
    catch ( SQLiteException & except )
    {
        cout << "Exception here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }

    command = "CREATE TABLE Companies\n"
            "    (\n"
            "        Id          TEXT,\n"
            "        Name        TEXT\n"
            "    )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );
    
    command = "INSERT INTO Companies\n"
            "   VALUES( 'C', 'Charlie' )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );
    
    command = "INSERT INTO Companies"
            "   VALUES( 'B', 'Baker' )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "INSERT INTO Companies"
            "   VALUES( 'A', 'Abel' )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    string query = "SELECT Troops.Rank, Troops.Name, Companies.Name AS CompanyName, Troops.SerialNumber"
            "   FROM Troops"
            "   JOIN Companies ON Troops.Company == Companies.Id"
            "   ORDER BY Companies.Name, Troops.Name";
    cout << "DoQuery( \"" << query << "\" )" << endl;
    shared_ptr< SQLResult > result = pDB->DoQuery( query );

    TESTCHECK( (result != 0), true, &ok );
    TESTCHECK( result->NumFields(), 4, &ok );
    TESTCHECK( result->NumRecords(), 4, &ok );
#if 0   //Not unless SQLite compiled with special switch
    TESTCHECK( result->FieldName( 0 ), string( "Troops.Rank" ), &ok );
    TESTCHECK( result->FieldName( 1 ), string( "Troops.Name" ), &ok );
    TESTCHECK( result->FieldName( 2 ), string( "Companies.CompanyName" ), &ok );
    TESTCHECK( result->FieldName( 3 ), string( "Troops.SerialNumber" ), &ok );
#else   //SQLite names retain their case.
    TESTCHECK( result->FieldName( 0 ), string( "Rank" ), &ok );
    TESTCHECK( result->FieldName( 1 ), string( "Name" ), &ok );
    TESTCHECK( result->FieldName( 2 ), string( "CompanyName" ), &ok );
    TESTCHECK( result->FieldName( 3 ), string( "SerialNumber" ), &ok );
#endif
    TESTCHECK( result->FieldType( 0 ), SQL::Text, &ok );
    TESTCHECK( result->FieldType( 1 ), SQL::Text, &ok );
    TESTCHECK( result->FieldType( 2 ), SQL::Text, &ok );
    TESTCHECK( result->FieldType( 3 ), SQL::Integer, &ok );
    TESTCHECK( result->IsNull( 0, 0 ), true, &ok );
    TESTCHECK( result->IsNull( 0, 1 ), false, &ok );
    TESTCHECK( result->IsNull( 1, 0 ), false, &ok );
    TESTCHECK( result->Value( 0, 1 ), string( "Ailey, Alvin" ), &ok );
    TESTCHECK( result->Value( 0, 2 ), string( "Abel" ), &ok );
    TESTCHECK( result->Value( 2, 0 ), string( "Sergeant" ), &ok );
    TESTCHECK( atoi( result->Value( 2, 3 ).c_str() ), 3142, &ok );
    TESTCHECK( result->Value( 3, 2 ), string( "Charlie" ), &ok );

    cout << "SQLResult::Record( result, 3 )" << endl;
    SQLResult::Record record( result, 3 );
    TESTCHECK( record[ 0 ], string( "Captain" ), &ok );
    TESTCHECK( record[ 1 ], string( "Crunch, Kangaroo" ), &ok );
    TESTCHECK( atoi( record[ 3 ].c_str() ), 6789, &ok );
    cout << "SQLResult::Record( result, 2 )" << endl;
    record = SQLResult::Record( result, 2 );
    TESTCHECK( record[ "Rank" ], string( "Sergeant" ), &ok );
    TESTCHECK( record[ "Name" ], string( "Arms, At" ), &ok );
    TESTCHECK( atoi( record[ "SerialNumber" ].c_str() ), 3142, &ok );
    TESTCHECK( record[ "CompanyName" ], string( "Charlie" ), &ok );
    record.Set( 1 );
    TESTCHECK( record[ 1 ], string( "Boy, Bugle" ), &ok );
    TESTCHECK( record[ "CompanyName" ], string( "Baker" ), &ok );

    cout << "ListTables( )" << endl;
    vector< string > tableList
            = dynamic_pointer_cast< SQLiteDatabase >( pDB )->ListTables( );
    TESTCHECK( tableList.size(), 2, &ok );
    TESTCHECK( tableList[0], string( "Companies" ), &ok );
    TESTCHECK( tableList[1], string( "Troops" ), &ok );

    cout << "DumpSQL( )" << endl;
    string sqlDump = dynamic_pointer_cast< SQLiteDatabase >( pDB )->DumpSQL( );
    cout << "--- Start of dump ---" << endl;
    cout << sqlDump << endl;
    cout << "--- End of dump ---" << endl << endl;

    command = "DROP TABLE Troops";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "DROP TABLE Companies";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    cout << endl << "DoCommand( sqlDump )" << endl;
    pDB->DoCommand( sqlDump );

    cout << "DoQuery( \"" << query << "\" )" << endl;
    result = pDB->DoQuery( query );

    TESTCHECK( (result != 0), true, &ok );
    TESTCHECK( result->NumFields(), 4, &ok );
    TESTCHECK( result->NumRecords(), 4, &ok );
#if 0   //Not unless SQLite compiled with special switch
    TESTCHECK( result->FieldName( 0 ), string( "Troops.Rank" ), &ok );
    TESTCHECK( result->FieldName( 1 ), string( "Troops.Name" ), &ok );
    TESTCHECK( result->FieldName( 2 ), string( "Companies.CompanyName" ), &ok );
    TESTCHECK( result->FieldName( 3 ), string( "Troops.SerialNumber" ), &ok );
#else   //SQLite names retain their case.
    TESTCHECK( result->FieldName( 0 ), string( "Rank" ), &ok );
    TESTCHECK( result->FieldName( 1 ), string( "Name" ), &ok );
    TESTCHECK( result->FieldName( 2 ), string( "CompanyName" ), &ok );
    TESTCHECK( result->FieldName( 3 ), string( "SerialNumber" ), &ok );
#endif
    TESTCHECK( result->FieldType( 0 ), SQL::Text, &ok );
    TESTCHECK( result->FieldType( 1 ), SQL::Text, &ok );
    TESTCHECK( result->FieldType( 2 ), SQL::Text, &ok );
    TESTCHECK( result->FieldType( 3 ), SQL::Integer, &ok );
    TESTCHECK( result->IsNull( 0, 0 ), true, &ok );
    TESTCHECK( result->IsNull( 0, 1 ), false, &ok );
    TESTCHECK( result->IsNull( 1, 0 ), false, &ok );
    TESTCHECK( result->Value( 0, 1 ), string( "Ailey, Alvin" ), &ok );
    TESTCHECK( result->Value( 0, 2 ), string( "Abel" ), &ok );
    TESTCHECK( result->Value( 2, 0 ), string( "Sergeant" ), &ok );
    TESTCHECK( atoi( result->Value( 2, 3 ).c_str() ), 3142, &ok );
    TESTCHECK( result->Value( 3, 2 ), string( "Charlie" ), &ok );

    cout << "SQLResult::Record( result, 3 )" << endl;
    record = SQLResult::Record( result, 3 );
    TESTCHECK( record[ 0 ], string( "Captain" ), &ok );
    TESTCHECK( record[ 1 ], string( "Crunch, Kangaroo" ), &ok );
    TESTCHECK( atoi( record[ 3 ].c_str() ), 6789, &ok );
    cout << "SQLResult::Record( result, 2 )" << endl;
    record = SQLResult::Record( result, 2 );
    TESTCHECK( record[ "Rank" ], string( "Sergeant" ), &ok );
    TESTCHECK( record[ "Name" ], string( "Arms, At" ), &ok );
    TESTCHECK( atoi( record[ "SerialNumber" ].c_str() ), 3142, &ok );
    TESTCHECK( record[ "CompanyName" ], string( "Charlie" ), &ok );
    record.Set( 1 );
    TESTCHECK( record[ 1 ], string( "Boy, Bugle" ), &ok );
    TESTCHECK( record[ "CompanyName" ], string( "Baker" ), &ok );

    command = "DROP TABLE Troops";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    command = "DROP TABLE Companies";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    DeleteFile( dbFileName );

    if ( ok )
        cout << "SQLiteDatabase PASSED." << endl << endl;
    else
        cout << "SQLiteDatabase FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SQLITE 

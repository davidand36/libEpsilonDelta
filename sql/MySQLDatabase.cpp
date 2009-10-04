/*
  MySQLDatabase.cpp
  Copyright (C) 2007 David M. Anderson

  MySQLDatabase class, representing a database opened using MySQL.
*/


#ifdef USE_MYSQL


#include "MySQLDatabase.hpp"
#include "MySQLLibrary.hpp"
#include "MySQLException.hpp"
#include "MySQLResult.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "ConfigFile.hpp"
#include "Array.hpp"
#include "StringUtil.hpp"
#include <iostream>
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef USE_EMBEDDEDMYSQL

MySQLDatabase::MySQLDatabase( const std::string & database,
                              const std::vector< std::string > & options )
{
    MySQLLibrary::Instance().Init( options );
    m_mysql = mysql_init( 0 );
    Assert( m_mysql != 0 );
    mysql_options( m_mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, 0 );
    MYSQL * conn = mysql_real_connect( m_mysql, 0, 0, 0,
                                       database.c_str(), 0, 0, 0 );
    if ( conn == 0 )
    {
        MySQLException exception( mysql_error( m_mysql ) );
        mysql_close( m_mysql );
        m_mysql = 0;
        throw exception;
    }
}

//.............................................................................

#else   //!USE_EMBEDDEDMYSQL

MySQLDatabase::MySQLDatabase( const std::string & host,
                              const std::string & database,
                              const std::string & user,
                              const std::string & password  )
    :   m_mysql( 0 )
{
    MySQLLibrary::Instance().Init( );
    m_mysql = mysql_init( 0 );
    Assert( m_mysql != 0 );
    //Can set connection options with mysql_options( );
    MYSQL * conn = mysql_real_connect( m_mysql, host.c_str(),
                                       user.c_str(), password.c_str(),
                                       database.c_str(), 0, 0, 0 );
    if ( conn == 0 )
    {
        MySQLException exception( mysql_error( m_mysql ) );
        mysql_close( m_mysql );
        m_mysql = 0;
        throw exception;
    }
}

#endif //!USE_EMBEDDEDMYSQL

//-----------------------------------------------------------------------------

MySQLDatabase::~MySQLDatabase( )
{
    if ( m_mysql )
        mysql_close( m_mysql );
}

//=============================================================================

void 
MySQLDatabase::DoCommand( const std::string & command )
{
    int queryRslt = mysql_query( m_mysql, command.c_str() );
    if ( queryRslt != 0 )
    {
        string message = mysql_error( m_mysql );
        int end = message.length() - 1;
        if ( message[ end ] == '\n' )
            message.erase( end );
        message += "\nCommand was:\n";
        message += command;
        MySQLException exception( message );
        throw exception;
    }
}

//-----------------------------------------------------------------------------

shared_ptr< SQLResult >
MySQLDatabase::DoQuery( const std::string & query )
{
    MYSQL_RES * mysqlRes = 0;
    shared_ptr< MySQLResult > pResult;
    MYSQL_ROW row = 0;
    int queryRslt = mysql_query( m_mysql, query.c_str() );
    if ( queryRslt != 0 )
        goto error;
    mysqlRes = mysql_store_result( m_mysql );
    if ( mysqlRes == 0 )
        goto error;
    pResult.reset( new MySQLResult( mysqlRes ) );
    do
    {
        row = mysql_fetch_row( mysqlRes );
        if ( row != 0 )
        {
            unsigned long * lengths = mysql_fetch_lengths( mysqlRes );
            pResult->AddRecord( row, lengths );
        }
    } while ( row != 0 );
    mysql_free_result( mysqlRes );
    return  pResult;
   
 error:
    string message = mysql_error( m_mysql );
    int end = message.length() - 1;
    if ( message[ end ] == '\n' )
        message.erase( end );
    message += "\nQuery was:\n";
    message += query;
    MySQLException exception( message );
    throw exception;
}

//=============================================================================

#ifdef DEBUG

bool 
MySQLDatabase::Test( const string & configFileName )
{
    bool ok = true;
    cout << "Testing MySQLDatabase" << endl;

    ConfigFile config( configFileName );

#ifdef USE_EMBEDDEDMYSQL
    string database = config.Value( "database" );
    vector< string > options;
    options.push_back( "--datadir=/home/dma/tmp" );
    shared_ptr< SQLDatabase > pDB( new MySQLDatabase( database, options ) );
#else
    string host = config.Value( "host" );
    string database = config.Value( "database" );
    string user = config.Value( "user" );
    string password = config.Value( "password" );

    cout << "MySQLDatabase( \"" << host << "\", \"" << database << "\", "
         << user << "\", <password> )" << endl;
    shared_ptr< SQLDatabase > pDB(
        new MySQLDatabase( host, database, user, password ) );
#endif

    try
    {
        string command = "DROP TABLE Troops, Companies";
        pDB->DoCommand( command );
    }
    catch ( MySQLException & except )
    {
    }

    string command = "CREATE TABLE Troops"
            " ( Name TEXT NOT NULL,"
            "   Rank TEXT,"
            "   SerialNumber INTEGER PRIMARY KEY,"
            "   Company VARCHAR(255) DEFAULT 'C' )";
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
        cout << "DoCommand() should have thrown an exception. FAILED" << endl;
        ok = false;
    }
    catch ( MySQLException & except )
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

#if 0 //Doesn't fail on MySQL.
    command = "INSERT INTO Troops"
            "   (SerialNumber)"
            "   VALUES ( 1010 )";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    try
    {
        pDB->DoCommand( command );
        cout << "DoCommand() should have thrown an exception. FAILED" << endl;
        ok = false;
    }
    catch ( MySQLException & except )
    {
        cout << "Exception here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
#endif

    command = "CREATE TABLE Companies"
            " ( Id TEXT,"
            "   Name TEXT )";
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
            "   JOIN Companies ON Troops.Company = Companies.Id"
            "   ORDER BY Companies.Name, Troops.Name";
    cout << "DoQuery( \"" << query << "\" )" << endl;
    shared_ptr< SQLResult > result( pDB->DoQuery( query ) );

    TESTCHECK( (result != 0), true, &ok );
    TESTCHECK( result->NumFields(), 4, &ok );
    TESTCHECK( result->NumRecords(), 4, &ok );
    //MySQL names retain their case.
    TESTCHECK( result->FieldName( 0 ), string( "Rank" ), &ok );
    TESTCHECK( result->FieldName( 1 ), string( "Name" ), &ok );
    TESTCHECK( result->FieldName( 2 ), string( "CompanyName" ), &ok );
    TESTCHECK( result->FieldName( 3 ), string( "SerialNumber" ), &ok );
    //Note that TEXT fields are not distinguished from BLOBs.
    TESTCHECK( result->FieldType( 0 ), SQL::Blob, &ok );
    TESTCHECK( result->FieldType( 1 ), SQL::Blob, &ok );
    TESTCHECK( result->FieldType( 2 ), SQL::Blob, &ok );
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
    cout << "Record::Set( 1 )" << endl;
    record.Set( 1 );
    TESTCHECK( record[ 1 ], string( "Boy, Bugle" ), &ok );
    TESTCHECK( record[ "CompanyName" ], string( "Baker" ), &ok );

    command = "DROP TABLE Troops, Companies";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

    if ( ok )
        cout << "MySQLDatabase PASSED." << endl << endl;
    else
        cout << "MySQLDatabase FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL

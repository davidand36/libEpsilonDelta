/*
  PostgreSQLDatabase.cpp
  Copyright (C) 2007 David M. Anderson

  PostgreSQLDatabase class, representing a database opened via a connection to
  a PostgreSQL server.
*/


#ifdef USE_POSTGRESQL


#include "PostgreSQLDatabase.hpp"
#include "PostgreSQLException.hpp"
#include "PostgreSQLResult.hpp"
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


namespace
{
string 
BuildParametersString( const string & host,
                       const string & database,
                       const string & user,
                       const string & password );
}

//-----------------------------------------------------------------------------

PostgreSQLDatabase::PostgreSQLDatabase( const string & host,
                                        const string & database,
                                        const string & user,
                                        const string & password )
    :    m_conn( 0 )
{
    string parameters = BuildParametersString( host, database,
                                               user, password );
    m_conn = PQconnectdb( parameters.c_str() );

    ConnStatusType status = PQstatus( m_conn );
    if ( status != CONNECTION_OK )
    {
        PostgreSQLException exception( PQerrorMessage( m_conn ) );
        PQfinish( m_conn );
        m_conn = 0;
        throw exception;
    }
}

//-----------------------------------------------------------------------------

namespace
{
//-----------------------------------------------------------------------------

string 
BuildParametersString( const string & host,
                       const string & database,
                       const string & user,
                       const string & password )
{
    string parameters;
    if ( host.length() > 0 )
    {
        parameters += "host='";
        parameters += host;
        parameters += "' ";
    }
    if ( database.length() > 0 )
    {
        parameters += "dbname='";
        parameters += database;
        parameters += "' ";
    }
    if ( user.length() > 0 )
    {
        parameters += "user='";
        parameters += user;
        parameters += "' ";
    }
    if ( password.length() > 0 )
    {
        parameters += "password='";
        parameters += password;
        parameters += "' ";
    }
    return parameters;
}

//-----------------------------------------------------------------------------
} //namespace

//-----------------------------------------------------------------------------

PostgreSQLDatabase::~PostgreSQLDatabase( )
{
    PQfinish( m_conn );
}

//=============================================================================

void
PostgreSQLDatabase::DoCommand( const string & command )
{
    PGresult * result = PQexec( m_conn, command.c_str() );
    ExecStatusType status = PQresultStatus( result );
    if ( status != PGRES_COMMAND_OK )
    {
        string message = PQerrorMessage( m_conn );
        int end = message.length() - 1;
        if ( message[ end ] == '\n' )
            message.erase( end );
        message += "\nCommand was:\n";
        message += command;
        PostgreSQLException exception( message );
        PQclear( result );
        throw exception;
    }
    PQclear( result );
}

//-----------------------------------------------------------------------------

shared_ptr< SQLResult >
PostgreSQLDatabase::DoQuery( const string & query )
{
    PGresult * result = PQexec( m_conn, query.c_str() );
    ExecStatusType status = PQresultStatus( result );
    if ( status != PGRES_TUPLES_OK )
    {
        string message = PQerrorMessage( m_conn );
        int end = message.length() - 1;
        if ( message[ end ] == '\n' )
            message.erase( end );
        message += "\nQuery was:\n";
        message += query;
        PostgreSQLException exception( message );
        PQclear( result );
        throw exception;
    }
    return shared_ptr< SQLResult >( new PostgreSQLResult( result ) );
}

//=============================================================================

#ifdef DEBUG

#if 0

void
ShowTypeOids( SQLDatabase * pDB )
{
    const string typeNames[]
            = { //integer
                "smallint", "integer", "serial", 
                //long int
                "bigint", "bigserial",
                //real
                "real", "double precision", "decimal", "numeric",
                //text
                "text", "char(10)", "varchar(100)",
                //blob
                "bytea",
                //date
                "date",
                //time
                "time",
                //datetime
                "timestamp"
            };

    string command = "CREATE TABLE Types ( ";
    for ( int i = 0; i < ARRAYSIZE( typeNames ); ++i )
    {
        if ( i != 0 )
            command += ", ";
        command += "f" + IntToString( i ) + " " + typeNames[i];
    }
    command += " )";
    pDB->DoCommand( command );

    string query = "SELECT * FROM Types";
    SQLResult * result = pDB->DoQuery( query );

    cout << endl << "Type Oids:" << endl;
    for ( int i = 0; i < ARRAYSIZE( typeNames ); ++i )
    {
        cout << typeNames[i] << ": ";
        //Relies on FieldType displaying the Oid for this purpose.
        result->FieldType( i );
    }
    cout << endl;
}

#endif

//-----------------------------------------------------------------------------

bool
PostgreSQLDatabase::Test( const string & configFileName )
{
    bool ok = true;
    cout << "Testing PostgreSQLDatabase" << endl;

    ConfigFile config( configFileName );
    string host = config.Value( "host" );
    string database = config.Value( "database" );
    string user = config.Value( "user" );
    string password = config.Value( "password" );

    cout << "PostgreSQLDatabase( \"" << host << "\", \"" << database << "\", "
         << user << "\", <password> )" << endl;
    shared_ptr< SQLDatabase > pDB( 
        new PostgreSQLDatabase( host, database, user, password ) );

    try
    {
        string command = "DROP TABLE Troops, Companies";
        pDB->DoCommand( command );
    }
    catch ( PostgreSQLException & except )
    {
    }

    string command = "CREATE TABLE Troops"
            " ( Name TEXT NOT NULL,"
            "   Rank TEXT,"
            "   SerialNumber INTEGER PRIMARY KEY,"
            "   Company TEXT DEFAULT 'C' )";
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
    catch ( PostgreSQLException & except )
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
    catch ( PostgreSQLException & except )
    {
        cout << "Exception here is OK" << endl;
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }

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
    //Note that SQL names are converted to lower case.
    TESTCHECK( result->FieldName( 0 ), string( "rank" ), &ok );
    TESTCHECK( result->FieldName( 1 ), string( "name" ), &ok );
    TESTCHECK( result->FieldName( 2 ), string( "companyname" ), &ok );
    TESTCHECK( result->FieldName( 3 ), string( "serialnumber" ), &ok );
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
    TESTCHECK( record[ "rank" ], string( "Sergeant" ), &ok );
    TESTCHECK( record[ "name" ], string( "Arms, At" ), &ok );
    TESTCHECK( atoi( record[ "serialnumber" ].c_str() ), 3142, &ok );
    TESTCHECK( record[ "companyname" ], string( "Charlie" ), &ok );
    record.Set( 1 );
    TESTCHECK( record[ 1 ], string( "Boy, Bugle" ), &ok );
    TESTCHECK( record[ "companyname" ], string( "Baker" ), &ok );

    command = "DROP TABLE Troops, Companies";
    cout << "DoCommand( \"" << command << "\" )" << endl;
    pDB->DoCommand( command );

#if 0
    ShowTypeOids( pDB );
#endif

    if ( ok )
        cout << "PostgreSQLDatabase PASSED." << endl << endl;
    else
        cout << "PostgreSQLDatabase FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_POSTGRESQL

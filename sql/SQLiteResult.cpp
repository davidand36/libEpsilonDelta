/*
  SQLiteResult.cpp
  Copyright (C) 2007 David M. Anderson

  SQLiteResult class, representing the result of an SQL query to an SQLite
  database.
*/


#ifdef USE_SQLITE


#include "SQLiteResult.hpp"
#include "Assert.hpp"
#include <sqlite3.h>
#include <cstring>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


SQLiteResult::SQLiteResult( sqlite3_stmt * stmt )
{
    int numFields = sqlite3_column_count( stmt );
    if ( numFields > 0 )
    {
        m_fieldNames.reserve( numFields );
        for ( int i = 0; i < numFields; ++i )
        {
            string name;
#if 0   //These sqlite3 functions are generally not available
            const char * databaseName
                    = sqlite3_column_database_name( stmt, i );
            if ( (databaseName != 0) && (strlen( databaseName ) > 0) )
                name += string( databaseName ) + '.';
            const char * tableName = sqlite3_column_table_name( stmt, i );
            if ( (tableName != 0) && (strlen( tableName ) > 0) )
                name += string( tableName ) + '.';
#endif
            const char * fieldName = sqlite3_column_name( stmt, i );
            if ( (fieldName != 0) && (strlen( fieldName ) > 0) )
                name += string( fieldName );
            m_fieldNames.push_back( name );
        }
        m_fieldTypes.reserve( numFields );
        for ( int i = 0; i < numFields; ++i )
        {
            int sqliteType = sqlite3_column_type( stmt, i );
            SQL::EFieldType type = SQLiteToFieldType( sqliteType );
            m_fieldTypes.push_back( type );
        }
    }
}

//-----------------------------------------------------------------------------

SQLiteResult::~SQLiteResult( )
{
}

//=============================================================================

int 
SQLiteResult::NumFields( ) const
{
    return  m_fieldNames.size();
}

//-----------------------------------------------------------------------------

int 
SQLiteResult::NumRecords( ) const
{
    return  m_records.size();
}

//=============================================================================

std::string 
SQLiteResult::FieldName( int field ) const
{
    Assert( (field >= 0) && (field < static_cast<int>(m_fieldNames.size())) );
    return  m_fieldNames[ field ];
}

//-----------------------------------------------------------------------------

SQL::EFieldType 
SQLiteResult::FieldType( int field ) const
{
    Assert( (field >= 0) && (field < static_cast<int>(m_fieldTypes.size())) );
    return  m_fieldTypes[ field ];
}

//=============================================================================

string 
SQLiteResult::Value( int record, int field ) const
{
    Assert( (record >= 0) && (record < static_cast<int>(m_records.size())) );
    Assert( (field >= 0)
            && (field < static_cast<int>(m_records[ record ].size())) );
    return  m_records[ record ][ field ].m_value;
}

//-----------------------------------------------------------------------------

bool 
SQLiteResult::IsNull( int record, int field ) const
{
    Assert( (record >= 0) && (record < static_cast<int>(m_records.size())) );
    Assert( (field >= 0)
            && (field < static_cast<int>(m_records[ record ].size())) );
    return  m_records[ record ][ field ].m_isNull;
}

//=============================================================================

void 
SQLiteResult::AddRecord( sqlite3_stmt * stmt )
{
    vector< Field > record;
    int numFields = m_fieldTypes.size();
    record.reserve( numFields );
    for ( int i = 0; i < numFields; ++i )
    {
        Field field;
        int sqliteType = sqlite3_column_type( stmt, i );
        if ( sqliteType == SQLITE_NULL )
        {
            field.m_isNull = true;
        }
        else
        {
            field.m_isNull = false;
            if ( m_fieldTypes[i] == SQL::UnknownType )
                m_fieldTypes[i] = SQLiteToFieldType( sqliteType );
            const char * data = reinterpret_cast< const char * >
                    ( sqlite3_column_blob( stmt, i ) );
            int numBytes = sqlite3_column_bytes( stmt, i );
            field.m_value.assign( data, numBytes );
        }
        record.push_back( field );
    }
    m_records.push_back( record );
}

//=============================================================================

SQL::EFieldType 
SQLiteResult::SQLiteToFieldType( int sqliteType )
{
    switch ( sqliteType )
    {
    case SQLITE_INTEGER:
        return  SQL::Integer;
    case SQLITE_FLOAT:
        return  SQL::Real;
    case SQLITE_TEXT:
        return  SQL::Text;
    case SQLITE_BLOB:
        return  SQL::Blob;
    default:
        return  SQL::UnknownType;
    }
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SQLITE

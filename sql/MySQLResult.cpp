/*
  MySQLResult.cpp
  Copyright (C) 2007 David M. Anderson

  MySQLResult class, representing the result of an SQL query to
  a MySQL database.
*/


#ifdef USE_MYSQL


#include "MySQLResult.hpp"
#include "Assert.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


MySQLResult::MySQLResult( MYSQL_RES * mysqlRes )
{
    Assert( mysqlRes );
    int numFields = mysql_num_fields( mysqlRes );
    MYSQL_FIELD * fields = mysql_fetch_fields( mysqlRes );
    if ( numFields > 0 )
    {
        m_fieldNames.reserve( numFields );
        for ( int i = 0; i < numFields; ++i )
        {
            string name = fields[ i ].name;
            m_fieldNames.push_back( name );
        }
        m_fieldTypes.reserve( numFields );
        for ( int i = 0; i < numFields; ++i )
        {
            SQL::EFieldType type = MySQLToFieldType( fields[ i ].type );
            m_fieldTypes.push_back( type );
        }
    }
    int numRows = mysql_num_rows( mysqlRes );
    m_records.reserve( numRows );
}

//-----------------------------------------------------------------------------

MySQLResult::~MySQLResult( )
{
}

//=============================================================================

int 
MySQLResult::NumFields( ) const
{
    return  m_fieldNames.size();
}

//-----------------------------------------------------------------------------

int 
MySQLResult::NumRecords( ) const
{
    return  m_records.size();
}

//=============================================================================

std::string 
MySQLResult::FieldName( int field ) const
{
    Assert( (field >= 0) && (field < m_fieldNames.size()) );
    return  m_fieldNames[ field ];
}

//-----------------------------------------------------------------------------

SQL::EFieldType 
MySQLResult::FieldType( int field ) const
{
    Assert( (field >= 0) && (field < m_fieldTypes.size()) );
    return  m_fieldTypes[ field ];
}

//=============================================================================

std::string 
MySQLResult::Value( int record, int field ) const
{
    Assert( (record >= 0) && (record < m_records.size()) );
    Assert( (field >= 0) && (field < m_records[ record ].size()) );
    return  m_records[ record ][ field ].m_value;
}

//-----------------------------------------------------------------------------

bool 
MySQLResult::IsNull( int record, int field ) const
{
    Assert( (record >= 0) && (record < m_records.size()) );
    Assert( (field >= 0) && (field < m_records[ record ].size()) );
    return  m_records[ record ][ field ].m_isNull;
}

//=============================================================================

void 
MySQLResult::AddRecord( MYSQL_ROW & mysqlRow, unsigned long * lengths )
{
    vector< Field > record;
    int numFields = m_fieldTypes.size();
    record.reserve( numFields );
    for ( int i = 0; i < numFields; ++i )
    {
        Field field;
        if ( mysqlRow[ i ] == 0 )
            field.m_isNull = true;
        else
        {
            field.m_value.assign( mysqlRow[ i ], lengths[ i ] );
            field.m_isNull = false;
        }
        record.push_back( field );
    }
    m_records.push_back( record );
}

//=============================================================================

SQL::EFieldType 
MySQLResult::MySQLToFieldType( enum_field_types mysqlType )
{
    switch ( mysqlType )
    {
    case MYSQL_TYPE_TINY:
    case MYSQL_TYPE_SHORT:
    case MYSQL_TYPE_INT24:
    case MYSQL_TYPE_LONG:
    case MYSQL_TYPE_YEAR:
    case MYSQL_TYPE_ENUM:
        return  SQL::Integer;
    case MYSQL_TYPE_LONGLONG:
        return  SQL::LongInt;
    case MYSQL_TYPE_FLOAT:
    case MYSQL_TYPE_DOUBLE:
    case MYSQL_TYPE_DECIMAL:
    case MYSQL_TYPE_NEWDECIMAL:   //Note: not in 4.1
        return  SQL::Real;
    case MYSQL_TYPE_STRING:
    case MYSQL_TYPE_VAR_STRING:
        return  SQL::Text;
    case MYSQL_TYPE_BLOB:
        return  SQL::Blob;
    case MYSQL_TYPE_DATE:
        return  SQL::Date;
    case MYSQL_TYPE_TIME:
        return  SQL::Time;
    case MYSQL_TYPE_DATETIME:
    case MYSQL_TYPE_TIMESTAMP:
        return  SQL::DateTime;
    case MYSQL_TYPE_BIT:          //Note: not in 4.1
    case MYSQL_TYPE_SET:
    case MYSQL_TYPE_NULL:
    case MYSQL_TYPE_GEOMETRY:
    default:
        return  SQL::UnknownType;
    }
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL

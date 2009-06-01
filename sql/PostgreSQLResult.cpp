/*
  PostgreSQLResult.cpp
  Copyright (C) 2007 David M. Anderson

  PostgreSQLResult class, representing the result of an SQL query to
  a PostgreSQL database.
*/


#ifdef USE_POSTGRESQL


#include "PostgreSQLResult.hpp"
#include <iostream>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


PostgreSQLResult::PostgreSQLResult( PGresult * result )
    :    m_result( result )
{
}

//-----------------------------------------------------------------------------

PostgreSQLResult::~PostgreSQLResult( )
{
    PQclear( m_result );
}

//=============================================================================

int
PostgreSQLResult::NumFields( ) const
{
    return PQnfields( m_result );
}

//-----------------------------------------------------------------------------

int
PostgreSQLResult::NumRecords( ) const
{
    return PQntuples( m_result );
}

//-----------------------------------------------------------------------------

string
PostgreSQLResult::FieldName( int field ) const
{
    return string( PQfname( m_result, field ) );
}

//-----------------------------------------------------------------------------

SQL::EFieldType
PostgreSQLResult::FieldType( int field ) const
{
    Oid pqType = PQftype( m_result, field );
#if 0
    cout << "Oid = " << pqType << endl;
    return  SQL::kUnknownType;
#else
    //Note: these Oids are empirically determined. Are they reliable?
    switch ( pqType )
    {
    case 21:    //smallint
    case 23:    //integer, serial
        return  SQL::Integer;
    case 20:    //bigint, bigserial
        return  SQL::LongInt;
    case 700:   //real
    case 701:   //double precision
    case 1700:  //decimal, numeric
        return  SQL::Real;
    case 25:    //text
    case 1042:  //char(n)
    case 1043:  //varchar(n)
        return  SQL::Text;
    case 17:    //bytea
        return  SQL::Blob;
    case 1082:  //date
        return  SQL::Date;
    case 1083:  //time
        return  SQL::Time;
    case 1114:  //timestamp
        return  SQL::DateTime;
    default:
        return  SQL::UnknownType;
    }
#endif
}

//-----------------------------------------------------------------------------

string
PostgreSQLResult::Value( int record, int field ) const
{
    return string( PQgetvalue( m_result, record, field ) );
}

//-----------------------------------------------------------------------------

bool
PostgreSQLResult::IsNull( int record, int field ) const
{
    return ( PQgetisnull( m_result, record, field ) != 0 );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_POSTGRESQL

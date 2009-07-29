#ifndef POSTGRESQLRESULT_HPP
#define POSTGRESQLRESULT_HPP
/*
  PostgreSQLResult.hpp
  Copyright (C) 2007 David M. Anderson

  PostgreSQLResult class, representing the result of an SQL query to
  a PostgreSQL database.
*/


#ifdef USE_POSTGRESQL


#include "SQLResult.hpp"
#include <postgresql/libpq-fe.h>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class PostgreSQLResult
    :    public SQLResult
{
public:
    PostgreSQLResult( PGresult * result );
    virtual ~PostgreSQLResult( );
    virtual int NumFields( ) const;
    virtual int NumRecords( ) const;
    virtual std::string FieldName( int field ) const;
    virtual SQL::EFieldType FieldType( int field ) const;
    virtual std::string Value( int record, int field ) const;
    virtual bool IsNull( int record, int field ) const;

private:
    PGresult * m_result;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_POSTGRESQL
#endif //POSTGRESQLRESULT_HPP

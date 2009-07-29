#ifndef POSTGRESQLDATABASE_HPP
#define POSTGRESQLDATABASE_HPP
/*
  PostgreSQLDatabase.hpp
  Copyright (C) 2007 David M. Anderson

  PostgreSQLDatabase class, representing a database opened via a connection to
  a PostgreSQL server.
*/


#ifdef USE_POSTGRESQL


#include "SQLDatabase.hpp"
#include <string>
#include <postgresql/libpq-fe.h>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class PostgreSQLDatabase
    :    public SQLDatabase
{
public:
    PostgreSQLDatabase( const std::string & host,
                        const std::string & database,
                        const std::string & user,
                        const std::string & password );
    virtual ~PostgreSQLDatabase( );
    virtual void DoCommand( const std::string & command );
    virtual shared_ptr< SQLResult > DoQuery( const std::string & query );

#ifdef DEBUG
    static bool Test( const std::string & configFileName );
#endif

private:
    PGconn * m_conn;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_POSTGRESQL
#endif //POSTGRESQLDATABASE_HPP

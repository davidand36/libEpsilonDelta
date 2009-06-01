#ifndef MYSQLDATABASE_HPP
#define MYSQLDATABASE_HPP
/*
  MySQLDatabase.hpp
  Copyright (C) 2007 David M. Anderson

  MySQLDatabase class, representing a database opened using MySQL.
*/


#ifdef USE_MYSQL


#include "SQLDatabase.hpp"
#include <string>
#include <mysql.h>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MySQLDatabase
    :    public SQLDatabase
{
public:
#ifdef USE_EMBEDDEDMYSQL
    MySQLDatabase( const std::string & database,
                   const std::vector< std::string > & options );
#else
    MySQLDatabase( const std::string & host, const std::string & database,
                   const std::string & user, const std::string & password  );
#endif
    virtual ~MySQLDatabase( );
    virtual void DoCommand( const std::string & command );
    virtual shared_ptr< SQLResult > DoQuery( const std::string & query );

#ifdef DEBUG
    static bool Test( const std::string & configFileName );
#endif

private:
    MYSQL * m_mysql;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL
#endif //MYSQLDATABASE_HPP

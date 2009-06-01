#ifndef SQLDATABASE_HPP
#define SQLDATABASE_HPP
/*
  SQLDatabase.hpp
  Copyright (C) 2007 David M. Anderson

  SQLDatabase class: Abstract base class representing an SQL database,
  possibly including a connection to an SQL server.
  NOTES:
  1. The user is responsible for deleting the results of queries.
  2. These methods may throw SQLException.
*/


#include "SQLResult.hpp"
#include "SmartPtr.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SQLDatabase
{
public:
    SQLDatabase( ) { }
    virtual ~SQLDatabase( ) { }
    virtual void DoCommand( const std::string & command ) = 0;
    virtual shared_ptr< SQLResult > DoQuery( const std::string & query ) = 0;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SQLDATABASE_HPP

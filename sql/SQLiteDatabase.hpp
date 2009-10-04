#ifndef SQLITEDATABASE_HPP
#define SQLITEDATABASE_HPP
/*
  SQLiteDatabase.hpp
  Copyright (C) 2007 David M. Anderson

  SQLiteDatabase class, representing a database opened using SQLite.

  NOTES:
  1. A database can be opened in memory by using the file name ":memory:".
*/


#ifdef USE_SQLITE


#include "SQLDatabase.hpp"
#include <string>

struct sqlite3;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SQLiteDatabase
    :    public SQLDatabase
{
public:
    SQLiteDatabase( const std::string & fileName );
    virtual ~SQLiteDatabase( );
    virtual void DoCommand( const std::string & command );
    virtual std::tr1::shared_ptr< SQLResult > DoQuery(
        const std::string & query );
    virtual std::vector< std::string > ListTables( );
    virtual std::string DumpSQL( );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    ::sqlite3 * m_sqlDB;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta


#endif //USE_SQLITE
#endif //SQLITEDATABASE_HPP

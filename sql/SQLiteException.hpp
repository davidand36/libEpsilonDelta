#ifndef SQLITEEXCEPTION_HPP
#define SQLITEEXCEPTION_HPP
/*
  SQLiteException.hpp
  Copyright (C) 2007 David M. Anderson

  SQLiteException class, representing problems accessing an SQLite database.
*/


#ifdef USE_SQLITE


#include "SQLException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SQLiteException
    :    public SQLException
{
public:
    explicit SQLiteException( const std::string & errorMessage );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SQLITE
#endif //SQLITEEXCEPTION_HPP

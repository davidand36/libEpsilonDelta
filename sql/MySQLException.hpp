#ifndef MYSQLEXCEPTION_HPP
#define MYSQLEXCEPTION_HPP
/*
  MySQLException.hpp
  Copyright (C) 2007 David M. Anderson

  MySQLException class, representing problems accessing a MySQL database.
*/


#ifdef USE_MYSQL


#include "SQLException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MySQLException
    :    public SQLException
{
public:
    MySQLException( const std::string & errorMessage );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL
#endif //MYSQLEXCEPTION_HPP

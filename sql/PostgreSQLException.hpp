#ifndef POSTGRESQLEXCEPTION_HPP
#define POSTGRESQLEXCEPTION_HPP
/*
  PostgreSQLException.hpp
  Copyright (C) 2007 David M. Anderson

  PostgreSQLException class, representing problems accessing a
  PostgreSQL database.
*/


#ifdef USE_POSTGRESQL


#include "SQLException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class PostgreSQLException
    :    public SQLException
{
public:
    explicit PostgreSQLException( const std::string & errorMessage );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_POSTGRESQL
#endif //POSTGRESQLEXCEPTION_HPP

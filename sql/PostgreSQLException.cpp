/*
  PostgreSQLException.cpp
  Copyright (C) 2007 David M. Anderson

  PostgreSQLException class, representing problems accessing a
  PostgreSQL database.
*/


#ifdef USE_POSTGRESQL


#include "PostgreSQLException.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


PostgreSQLException::PostgreSQLException( const string & errorMessage )
    :    SQLException( "PostgreSQL error: " )
{
    string errorMsg = errorMessage;
    int end = errorMsg.length() - 1;
    if ( errorMsg[ end ] == '\n' )
        errorMsg.erase( end );
    m_description += errorMsg;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_POSTGRESQL

/*
  MySQLException.cpp
  Copyright (C) 2007 David M. Anderson

  MySQLException class, representing problems accessing a MySQL database.
*/


#ifdef USE_MYSQL


#include "MySQLException.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


MySQLException::MySQLException( const string & errorMessage )
    :    SQLException( "MySQL error: " )
{
    string errorMsg = errorMessage;
    int end = errorMsg.length() - 1;
    if ( errorMsg[ end ] == '\n' )
        errorMsg.erase( end );
    m_description += errorMsg;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL

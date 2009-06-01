/*
  SQLiteException.cpp
  Copyright (C) 2007 David M. Anderson

  SQLiteException class, representing problems accessing an SQLite database.
*/


#ifdef USE_SQLITE


#include "SQLiteException.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


SQLiteException::SQLiteException( const string & errorMessage )
    :    SQLException( "SQLite error: " )
{
    string errorMsg = errorMessage;
    int end = errorMsg.length() - 1;
    if ( errorMsg[ end ] == '\n' )
        errorMsg.erase( end );
    m_description += errorMsg;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SQLITE

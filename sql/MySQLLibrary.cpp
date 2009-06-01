/*
  MySQLLibrary.cpp
  Copyright (C) 2007 David M. Anderson

  MySQLLibrary class: Handles initialization and shutdown of an
  embedded MySQL server library.
*/


#ifdef USE_MYSQL


#include "MySQLLibrary.hpp"
#include "MySQLException.hpp"
#include "SmartPtr.hpp"
#include "StringUtil.hpp"
#include <mysql.h>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


MySQLLibrary::MySQLLibrary( )
    :   m_initialized( false )
{
}

//-----------------------------------------------------------------------------

MySQLLibrary::~MySQLLibrary( )
{
    Shutdown( );
}

//=============================================================================

void 
MySQLLibrary::Init( )
{
    if ( ! m_initialized )
    {
        int initRslt = mysql_library_init( 0, 0, 0 );
        if ( initRslt != 0 )
        {
            string msg( "mysql_library_init returned " );
            msg += IntToString( initRslt );
            MySQLException exception( msg );
            throw exception;
        }
        m_initialized = true;
    }
}

//.............................................................................

void 
MySQLLibrary::Init( const vector< string > & options )
{
    int numOptions = static_cast< int >( options.size() );
    if ( numOptions == 0 )
        Init( );
    else
    {
        char ** ppOptions( new char *[ numOptions + 1 ] );
        //The first "argument" is ignored
        ppOptions[0] = new char[ 1 ];
        ppOptions[0][0] = 0;
        for ( int i = 0; i < numOptions; ++i )
        {
            ppOptions[ i + 1 ] = new char[ options[i].size() + 1 ];
            strcpy( ppOptions[ i + 1 ], options[i].c_str() );
        }

        int initRslt = mysql_library_init( numOptions, ppOptions, 0 );

        for ( int i = 0; i < numOptions + 1; ++i )
            delete[] ppOptions[i];
        delete[] ppOptions;

        if ( initRslt != 0 )
        {
            string msg( "mysql_library_init returned " );
            msg += IntToString( initRslt );
            MySQLException exception( msg );
            throw exception;
        }
        m_initialized = true;
    }
}

//-----------------------------------------------------------------------------

void 
MySQLLibrary::Shutdown( )
{
    if ( m_initialized )
    {
        mysql_library_end( );
        m_initialized = false;
    }
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_MYSQL

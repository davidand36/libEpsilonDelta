/*
  MySQLLibrary.cpp
  Copyright (C) 2007 David M. Anderson

  MySQLLibrary class: Handles initialization and shutdown of an
  embedded MySQL server library.
*/


#ifdef USE_MYSQL


#include "MySQLLibrary.hpp"
#include "MySQLException.hpp"
#include "StringUtil.hpp"
#include <mysql.h>
#include <cstring>
#include <tr1/memory>
using namespace std;
using namespace std::tr1;


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
        vector< vector< char > > vvOptions( numOptions + 1 );
        vector< char * > vpOptions( numOptions + 1 );
        //The first "argument" is ignored
        vvOptions[ 0 ].resize( 1, 0 );
        vpOptions[ 0 ] = &(vvOptions[ 0 ][ 0 ]);
        for ( int i = 0; i < numOptions; ++i )
        {
            vvOptions[ i + 1 ].resize( options[ i ].size() + 1, 0 );
            vpOptions[ i + 1 ] = &(vvOptions[ i + 1 ][ 0 ]);
            strcpy( vpOptions[ i + 1 ], options[ i ].c_str() );
        }

        int initRslt = mysql_library_init( numOptions, &vpOptions[0], 0 );

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

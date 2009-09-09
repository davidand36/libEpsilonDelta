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
#include <cstring>
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
        shared_array< shared_array< char > > saOptions(
            new shared_array< char >[ numOptions + 1 ] );
        shared_array< char * > ppOptions( new char *[ numOptions + 1 ] );
        //The first "argument" is ignored
        saOptions[ 0 ].reset( new char[ 1 ] );
        ppOptions[ 0 ] = saOptions[ 0 ].get();
        ppOptions[ 0 ][ 0 ] = 0;
        for ( int i = 0; i < numOptions; ++i )
        {
            saOptions[ i + 1 ].reset( new char[ options[ i ].size() + 1 ] );
            ppOptions[ i + 1 ] = saOptions[ i + 1 ].get();
            strcpy( ppOptions[ i + 1 ], options[i].c_str() );
        }

        int initRslt = mysql_library_init( numOptions, ppOptions.get(), 0 );

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

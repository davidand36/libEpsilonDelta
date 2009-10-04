/*
  Logger.cpp
  Copyright (C) 2009 David M. Anderson

  Logger class: Selectively outputs error, warning, and other messages.
*/


#include "Logger.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "StringUtil.hpp"
#include <iostream>
#include <sstream>
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Logger::Logger( const string & domain, shared_ptr< OutputFunc > func )
    :   m_domain( domain ),
        m_verbosity( Warning )
{
    SetOutputFunc( func );
}

//=============================================================================

void 
Logger::operator()( int level, const string & message )
{
    Log( level, message );
}

//.............................................................................

void 
Logger::operator()( int level, const char * format, ... )
{
    if ( level <= m_verbosity )
    {
        va_list args;
        va_start( args, format );
        Log( level, format, args );
        va_end( args );
    }
}

//-----------------------------------------------------------------------------

void 
Logger::Log( int level, const string & message )
{
    if ( level <= m_verbosity )
    {
        Assert( m_pOutputFunc );
        (*m_pOutputFunc)( m_domain, level, message );
    }
}

//.............................................................................

void 
Logger::Log( int level, const char * format, ... )
{
    if ( level <= m_verbosity )
    {
        va_list args;
        va_start( args, format );
        Log( level, format, args );
        va_end( args );
    }
}

//.............................................................................

void 
Logger::Log( int level, const char * format, va_list args )
{
    if ( level <= m_verbosity )
    {
        char msg[ 1000 ];
        int prntRslt = vsnprintf( msg, sizeof( msg ), format, args );
        if ( prntRslt < 0 )
            msg[ 999 ] = '\0';
        Log( level, string( msg ) );
    }
}

//=============================================================================

void 
Logger::SetVerbosity( int maxLevel )
{
    if ( maxLevel >= NumLevels )
        maxLevel = NumLevels - 1;
    m_verbosity = maxLevel;
}

//-----------------------------------------------------------------------------

int 
Logger::GetVerbosity( ) const
{
    return m_verbosity;
}

//=============================================================================

void 
Logger::SetOutputFunc( shared_ptr< OutputFunc > func )
{
    if ( func )
        m_pOutputFunc = func;
    else
        m_pOutputFunc.reset( new OutputToStream );
    Assert( m_pOutputFunc );
}

//-----------------------------------------------------------------------------

void 
Logger::SetOutputStream( ostream & dest, int level )
{
    shared_ptr< OutputToStream > pOTS
            = dynamic_pointer_cast< OutputToStream >( m_pOutputFunc );
    if ( pOTS )
        pOTS->SetOutputStream( dest, level );
}


//*****************************************************************************


Logger::OutputToStream::OutputToStream( )
{
    for ( int i = Emergency; i <= Warning; ++i )
        m_destinations[ i ] = &cerr;
    for ( int i = Notice; i < NumLevels; ++i )
        m_destinations[ i ] = &cout;
}

//=============================================================================

void 
Logger::OutputToStream::operator()( const std::string & domain, int level,
                                    const std::string & message )
{
    static const string levelText[ NumLevels ]
            = { " >>>EMERGENCY<<< ", " >>ALERT<< ", " >CRITICAL< ",
                " ERROR: ", " Warning: ", " Notice: ", " Info: ",
                " Debug: ", " Debug1: ", " Debug2: " };
    *(m_destinations[ level ]) << "[" << domain << "]" << levelText[ level ]
                               << message << endl;
}

//=============================================================================

void 
Logger::OutputToStream::SetOutputStream( std::ostream & dest, int level )
{
    if ( level >= 0 )
    {
        if ( level < NumLevels )
            m_destinations[ level ] = &dest;
    }
    else
    {
        for ( int i = 0; i < NumLevels; ++i )
            m_destinations[ i ] = &dest;
    }
}


//*****************************************************************************


Logger g_generalLogger( "General" );


//*****************************************************************************


#ifdef DEBUG

namespace
{                                                                   //namespace
//-----------------------------------------------------------------------------

Logger log1( "One" );
Logger log2( "Two" );

//-----------------------------------------------------------------------------

void
Talk( )
{
    log1( Logger::Alert, string( "An alert" ) );
    log2( Logger::Alert, "Alert #%d", 2 );
    log1.Log( Logger::Error, "Can't do %s", "something" );
    log2.Log( Logger::Error, "Uh oh!" );
    log1( Logger::Warning, "This could be " + string( "bad" ) );
    log2( Logger::Info, "Interestingly..." );
}

//-----------------------------------------------------------------------------

class OutputToString
    :   public Logger::OutputFunc
{
public:
    OutputToString( shared_ptr< string > pStr );
    virtual void operator()( const std::string & domain, int level,
                             const std::string & message );

private:
    shared_ptr< string>     m_pStr;
};

//-----------------------------------------------------------------------------
}                                                                   //namespace

//-----------------------------------------------------------------------------

bool 
Logger::Test( )
{
    bool ok = true;
    cout << "Testing Logger" << endl;

    cout << "Five lines of log output should follow:" << endl;
    Talk( );

    ostringstream lss;
    cout << "log1.SetOutputStream( lss )" << endl;
    log1.SetOutputStream( lss );
    cout << "Two lines of log output (just [Two]) should follow:" << endl;
    Talk( );
    TESTCHECK( lss.str(),
               string( "[One] >>ALERT<< An alert\n"
                       "[One] ERROR: Can't do something\n"
                       "[One] Warning: This could be bad\n" ),
               &ok );

    cout << "log2.SetOutputStream( lss )" << endl;
    log2.SetOutputStream( lss );
    lss.str( "" );
    cout << "No more lines of log output outside of TESTCHECK." << endl;
    Talk( );
    TESTCHECK( lss.str(),
               string( "[One] >>ALERT<< An alert\n"
                       "[Two] >>ALERT<< Alert #2\n"
                       "[One] ERROR: Can't do something\n"
                       "[Two] ERROR: Uh oh!\n"
                       "[One] Warning: This could be bad\n" ),
               &ok );

    cout << "log2.SetVerbosity( Info )" << endl;
    log2.SetVerbosity( Logger::Info );
    lss.str( "" );
    Talk( );
    TESTCHECK( lss.str(),
               string( "[One] >>ALERT<< An alert\n"
                       "[Two] >>ALERT<< Alert #2\n"
                       "[One] ERROR: Can't do something\n"
                       "[Two] ERROR: Uh oh!\n"
                       "[One] Warning: This could be bad\n"
                       "[Two] Info: Interestingly...\n" ),
               &ok );

    cout << "log1.SetVerbosity( Error )" << endl;
    log1.SetVerbosity( Logger::Error );
    lss.str( "" );
    Talk( );
    TESTCHECK( lss.str(),
               string( "[One] >>ALERT<< An alert\n"
                       "[Two] >>ALERT<< Alert #2\n"
                       "[One] ERROR: Can't do something\n"
                       "[Two] ERROR: Uh oh!\n"
                       "[Two] Info: Interestingly...\n" ),
               &ok );

    shared_ptr< string > pStr2( new string );
    shared_ptr< OutputToString > pOutput( new OutputToString( pStr2 ) );
    cout << "log2.SetOutputFunc()" << endl;
    log2.SetOutputFunc( pOutput );
    lss.str( "" );
    Talk( );
    TESTCHECK( lss.str(),
               string( "[One] >>ALERT<< An alert\n"
                       "[One] ERROR: Can't do something\n" ),
               &ok );
    TESTCHECK( *pStr2,
               string( "(Two) {level 1} Alert #2\n"
                       "(Two) {level 3} Uh oh!\n"
                       "(Two) {level 6} Interestingly...\n" ),
               &ok );

    if ( ok )
        cout << "Logger PASSED." << endl << endl;
    else
        cout << "Logger FAILED." << endl << endl;
    return ok;
}


//-----------------------------------------------------------------------------

OutputToString::OutputToString( shared_ptr< string > pStr )
    :   m_pStr( pStr )
{
}

//-----------------------------------------------------------------------------

void 
OutputToString::operator()( const std::string & domain, int level,
                            const std::string & message )
{
    *m_pStr += "(" + domain + ")" + " {level " + IntToString( level ) + "} "
            + message + "\n";
}

//-----------------------------------------------------------------------------

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

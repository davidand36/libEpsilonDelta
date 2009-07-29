/*
  TestCGILib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta cgi library.
*/


#include "Exception.hpp"
#include "CGIRawInput.hpp"
#include "CGIOutput.hpp"
#include "Array.hpp"
#include "StringUtil.hpp"
#include <string>
#include <fstream>
using namespace std;
using namespace EpsilonDelta;


namespace
{

int Main( int /*argc*/, char ** argv );
void GetBaseLocation( );
void GetConfigGlobals( const string & progName );
void GetCgiGlobals( );
string WriteHttpResponse( );
string WritePrimes( );

string g_baseLocation;

string s_contentType = "text/plain";

}


//*****************************************************************************


int 
main( int argc, char ** argv )
{
    return Main( argc, argv );
}

//*****************************************************************************

namespace
{                                                                   //namespace

//=============================================================================

int 
Main( int /*argc*/, char ** argv )
{
    string errorMessage;
    try
    {
        bool firstTime = true;
        while ( CGIRawInput::Instance().ReadInput( ) )
        {
            CGIOutput::Instance().Reset( );
            if ( firstTime )
            {
                firstTime = false;
                GetBaseLocation( );
                GetConfigGlobals( argv[0] );
            }
            GetCgiGlobals( );
            string response = WriteHttpResponse( );
            CGIOutput::Instance().SetContentType( s_contentType );
            CGIOutput::Instance().Send( response );
        }
    }
    catch ( Exception & except )
    {
        errorMessage = except.Description();
    }
    catch ( exception & except )
    {
        errorMessage = except.what();
    }
    catch ( ... )
    {
        errorMessage = "Unknown exception!";
    }
    if ( ! errorMessage.empty() )
        CGIOutput::Instance().Send( errorMessage );

    return 0;
}

/*===========================================================================*/

void 
GetBaseLocation( )
{
    const char * location = getenv( "SCRIPT_NAME" );
    if ( location != 0 )
        g_baseLocation = location;
}

//=============================================================================

void 
GetConfigGlobals( const string & /*progName*/ )
{
}

//=============================================================================

void 
GetCgiGlobals( )
{
}

//=============================================================================

string
WriteHttpResponse( )
{
    static int counter = 0;
    string response;

    if ( CGIRawInput::Instance().Values( "ListPrimes" ).size() > 0 )
    {
        CGIOutput::Instance().SetFileName( "Primes.txt" );
        return WritePrimes( );
    }

    response += "Requests handled by this process: "
            + IntToString( ++counter ) + "\n\n";
    response += "### Environment ###\n";
    const char * envKeys[]
            = { "REQUEST_METHOD", "CONTENT_TYPE", "CONTENT_LENGTH",
                "QUERY_STRING" };
    for ( int i = 0; i < ARRAY_LENGTH( envKeys ); ++i )
    {
        response += envKeys[i];
        response += " = \'";
        const char * envVal = getenv( envKeys[i] );
        response += ( envVal  ?  envVal  :  "NULL" );
        response += "\'\n";
    }
    response += "### End of Environment ###\n\n";
    response += "### Raw Input ###\n";
    response += CGIRawInput::Instance().RawInput( );
    response += "\n### End of Raw Input ###\n\n";
    response += "### Parsed Input ###\n";
    try
    {
        const multimap< string, string > pairs
                = CGIRawInput::Instance().Pairs( );
        typedef multimap< string, string >::const_iterator  mmci;
        for ( mmci pPair = pairs.begin(); pPair != pairs.end(); ++pPair )
        {
            response += pPair->first;
            response += " = \'";
            response += pPair->second;
            response += "\'\n";
        }
    }
    catch ( Exception & except )
    {
        response += except.Description();
    }
    catch ( exception & except )
    {
        response += except.what();
    }
    catch ( ... )
    {
        response += "Unknown exception!";
    }
    response += "### End of Parsed Input ###\n\n";
    
    return response;
}

//-----------------------------------------------------------------------------

string
WritePrimes( )
{
    static int counter = 0;
    static int oldLimit = 2;
    static vector< bool > primes( 2, false );
    const int maxLimit = 10000000;

    string footnote = "\n\n(Requests handled by this process: "
            + IntToString( ++counter ) + ".  "
            + "OldLimit: " + IntToString( oldLimit ) + ")\n";

    int limit = 100;
    vector< string > limitVals = CGIRawInput::Instance().Values( "Limit" );
    if ( limitVals.size() > 0 )
        limit = atoi( limitVals[0].c_str() );
    if ( limit <= 2 )
        limit = 2;
    if ( limit > maxLimit )
        limit = maxLimit;

    if ( limit > oldLimit )
    {
        primes.resize( limit, true );
        int prime = 2;
        while ( prime < limit )
        {
            int firstMultiplier = max( 2, ((oldLimit - 1) / prime) + 1 );
            int firstMultiple = prime * firstMultiplier;
            for ( int i = firstMultiple; i < limit; i += prime )
                primes[i] = false;
            while ( (++prime < limit) && ! primes[prime] )
                ;
        }
        oldLimit = limit;
    }

    string primeString;
    int numPrimes = 0;
    for ( int i = 2; i < limit; ++i )
        if ( primes[i] )
        {
            primeString += IntToString( i ) + "\n";
            ++numPrimes;
        }
    string response = IntToString( numPrimes ) + " primes under "
            + IntToString( limit ) + ".\n" + primeString
            + footnote;

    return response;
}

//=============================================================================

}                                                                   //namespace

//*****************************************************************************

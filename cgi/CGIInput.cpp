/*
  CGIInput.cpp
  Copyright (C) 2007 David M. Anderson

  CGIInput class:  Reads and parses CGI input.
*/


#include "CGIInput.hpp"
#include "StringUtil.hpp"
#include "Exception.hpp"
#include "Assert.hpp"
#include "Platform.hpp"
#include <cstring>
#include <cstdlib>
#include <tr1/memory>
#ifdef USE_FASTCGI
#include <fcgi_stdio.h>
#else
#include <cstdio>
#endif
#ifdef OS_WINDOWS
#include <io.h>
#include <fcntl.h>
#endif
using namespace std;
using namespace std::tr1;

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


CGIInput::CGIInput( )
    :   m_initialized( false ),
        m_parseAndFree( true )
{
}

//-----------------------------------------------------------------------------

CGIInput::~CGIInput( )
{
}

//=============================================================================

void 
CGIInput::SetParseAndFree( bool parseAndFree )
{
    m_parseAndFree = parseAndFree;
}

//=============================================================================

bool 
CGIInput::ReadInput( int maxInputLength )
{
#if USE_FASTCGI
    bool gotNewInput = (FCGI_Accept( ) >= 0);
#else
    bool gotNewInput = ! m_initialized;
#endif
    if ( gotNewInput )
    {
        m_initialized = true;
        ReadRawInput( maxInputLength );
        if ( m_parseAndFree )
        {
            ParseRawInput( );
            FreeRawInput( );
        }
    }
    return gotNewInput;
}

//=============================================================================

void 
CGIInput::ReadRawInput( int maxInputLength )
{
    m_rawInput.erase();
    m_pairs.clear();

    int length = 0;
    const char * requestMethod = getenv( "REQUEST_METHOD" );
    if ( requestMethod == 0 )
        return;
    if ( strcmp( requestMethod, "POST" ) == 0 )
    {
        const char * contentLength = getenv( "CONTENT_LENGTH" );
        if ( contentLength == 0 )
            return;
        length = atoi( contentLength );
        Assert( length >= 0 );
        if ( length > maxInputLength )
            throw RuntimeError( "CGI input exceeds maximum allowed size." );
        vector< char > inputBuff( length + 1 );
#ifdef OS_WINDOWS
        _setmode( _fileno( stdin ), _O_BINARY );
#endif
#ifdef DEBUG
        int bytesRead =
#endif
                fread( &inputBuff[0], 1, length, stdin );
        Assert( bytesRead == length );
        inputBuff[ length ] = 0;
        m_rawInput.append( &inputBuff[0], length );
    }
    const char * queryString = getenv( "QUERY_STRING" );
    if ( queryString == 0 )
        return;
    length = strlen( queryString );
    if ( length > 0 )
    {
        if ( length > maxInputLength )
            throw RuntimeError( "CGI input exceeds maximum allowed size." );
        if ( m_rawInput.length( ) > 0 )
            m_rawInput.append( "&" );
        m_rawInput.append( queryString, length );
    }
}

//-----------------------------------------------------------------------------

void 
CGIInput::ParseRawInput( )
{
    const char * contentTypeEnv = getenv( "CONTENT_TYPE" );
    const string contentType( contentTypeEnv  ?  contentTypeEnv  :  "" );
    if ( contentType.find( "multipart/form-data" ) != string::npos )
        ParseMultipartFormData( m_rawInput );
    else
        ParseURLEncodedInput( m_rawInput );
}

//-----------------------------------------------------------------------------

void
CGIInput::ParseURLEncodedInput( const string & input )
{
    vector< string > pairs = Split( input, '&' );
    for ( vector< string >::const_iterator pPair = pairs.begin();
          pPair != pairs.end(); ++pPair )
    {
        vector< string > nameValue = Split( *pPair, '=' );
        if ( nameValue.size() == 1 )
        {
            m_pairs.insert( pair< string, string >( nameValue[0], "" ) );
        }
        else
        {
            Assert( nameValue.size() == 2 );
            string value;
            for ( string::iterator p = nameValue[1].begin();
                  p != nameValue[1].end(); ++p )
            {
                if ( *p == '+' )  //encoding for ' '
                {
                    value += ' ';
                }
                else if ( *p == '%' )  //hex-encoded char follows
                {
                    char ch = 0;
                    ++p;
                    Assert( p != nameValue[1].end() );
                    int n = *p;
                    ch += static_cast<char>((n >= 'A')
                                            ? (n - 'A' + 10) : (n - '0'));
                    ch *= 16;
                    ++p;
                    Assert( p != nameValue[1].end() );
                    n = *p;
                    ch += static_cast<char>((n >= 'A')
                                            ? (n - 'A' + 10) : (n - '0'));
                    value += ch;
                }
                else  //normal char
                    value += *p;
            }
            m_pairs.insert( pair< string, string >( nameValue[0], value ) );
        }
    }
}

//-----------------------------------------------------------------------------

void
CGIInput::ParseMultipartFormData( const string & input )
{
    const char * contentTypeEnv = getenv( "CONTENT_TYPE" );
    const string contentType( contentTypeEnv  ?  contentTypeEnv  :  "" );
    string::size_type boundaryPos = contentType.find( "boundary=" );
    Assert( boundaryPos != string::npos );
    string boundary = "--";
    boundary += contentType.substr( boundaryPos + strlen( "boundary=" ) );
    vector< string > blocks = Split( input, boundary );
    Assert( blocks[0].length() == 0 );
    int numBlocks = static_cast<int>( blocks.size() );
    for ( int i = 1; i < numBlocks - 1; ++i )
    {
        const string & block = blocks[i];
        const int blockLen = block.length();
        int pos = 0;
        while ( (block[pos] == '\n') || (block[pos] == '\r') )
            ++pos;
        const string dispositionTag(
            "Content-Disposition: form-data; name=\"" );
        const int dispositionTagLen = dispositionTag.length();
        Assert( block.compare( pos, dispositionTagLen, dispositionTag ) == 0 );
        pos += dispositionTagLen;
        string name;
        while ( (pos < blockLen) && (block[pos] != '\"') )
            name += block[pos++];
        Assert( block[pos] == '\"' );
        const string filenameTag( "filename=\"" );
        const int filenameTagLen = filenameTag.length();
        while ( (pos < blockLen) && (block[pos] != '\n') )
        {
            if ( block.compare( pos, filenameTagLen, filenameTag ) == 0 )
            {
                pos += filenameTagLen;
                string filename;
                while ( (pos < blockLen) && (block[pos] != '\"') )
                    filename += block[pos++];
                Assert( block[pos] == '\"' );
                string key = name + "_filename";
                m_pairs.insert( pair< string, string >( key, filename ) );
            }
            ++pos;
        }
        int valueBeginning = 0;
        while ( valueBeginning == 0 )
        {
            while ( (pos < blockLen) && (block[pos] != '\n') )
                ++pos;
            Assert( pos < blockLen + 2 );
            Assert( block[pos] == '\n' );
            ++pos;
            if ( block[pos] == '\n' )
                valueBeginning = pos + 1;
            else if ( (block[pos] == '\r') && (block[pos + 1] == '\n') )
                valueBeginning = pos + 2;
        }
        string value = block.substr( valueBeginning,
                                     blockLen - valueBeginning - 2 );
        m_pairs.insert( pair< string, string >( name, value ) );
    }
    //Parse query string appended at the end of raw input.
    if ( blocks[ numBlocks - 1 ].length() > 2 )
    {
        const string & block = blocks[ numBlocks - 1 ];
        const int blockLen = block.length();
        int pos = 2;
        while ( (pos < blockLen) && (block[pos] != '&') )
            ++pos;
        if ( pos < blockLen )
            ParseURLEncodedInput( block.substr( pos + 1 ) );
    }
}

//-----------------------------------------------------------------------------

void 
CGIInput::FreeRawInput( )
{
    m_rawInput.erase( );
}

//=============================================================================

const std::string &
CGIInput::RawInput( ) const
{
    Assert( m_initialized );
    return m_rawInput;
}

//=============================================================================

vector< string > 
CGIInput::Values( const string & name ) const
{
    Assert( m_initialized );
    typedef multimap< string, string >::const_iterator MCIter;
    pair< MCIter, MCIter > range = m_pairs.equal_range( name );
    vector< string > values;
    for ( MCIter p = range.first; p != range.second; ++p )
        values.push_back( p->second );
    return values;
}

//-----------------------------------------------------------------------------

string 
CGIInput::operator[]( const string & name ) const
{
    Assert( m_initialized );
    typedef multimap< string, string >::const_iterator MCIter;
    MCIter p = m_pairs.find( name );
    if ( p != m_pairs.end() )
        return p->second;
    else
        return "";
}

//-----------------------------------------------------------------------------

const std::multimap< std::string, std::string > & 
CGIInput::Pairs( )
{
    Assert( m_initialized );
    return m_pairs;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

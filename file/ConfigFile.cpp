/*
  ConfigFile.cpp
  Copyright (C) 2007 David M. Anderson

  ConfigFile class:  Reads, parses config file.
*/


#include "ConfigFile.hpp"
#include "Assert.hpp"
#include "File.hpp"
#include "StringUtil.hpp"
#include <cstdio>
#include <tr1/memory>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Platform.hpp"
#ifndef OS_WINDOWS
#include <unistd.h>
#endif
#endif
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta


//*****************************************************************************


ConfigFile::ConfigFile( const string & fileName )
    :    m_fileName( fileName )
{
    string rawInput;
    File::Load( fileName, &rawInput );
    ParseInput( rawInput );
}

//.............................................................................

void 
ConfigFile::ParseInput( const string & rawInput )
{
    string name = "";
    string value = "";
    string * pCurString = &name;
    string::const_iterator p = rawInput.begin();
    while ( p != rawInput.end() )
    {
        if ( *p == '\n' )  //pair separator
        {
            Trim( &name );
            if ( name.length() > 0 )
                m_pairs.insert( pair< string, string>( name, value ) );
            name.erase( );
            value.erase( );
            pCurString = &name;
            ++p;
            if ( (p != rawInput.end()) && (*p == '#') )
                //skip lines beginning with '#' (comments)
                while ( (p != rawInput.end()) && (*p++ != '\n') )
                    ;
        }
        else if ( (*p == '=') && (pCurString == &name) ) //name-value separator
        {
            pCurString = &value;
            ++p;
        }
        else  //normal char
            *pCurString += *p++;
    }
    Trim( &name );
    if ( name.length() > 0 )
        m_pairs.insert( pair< string, string>( name, value ) );
}

//=============================================================================

vector< string > 
ConfigFile::Values( const string & name ) const
{
    typedef multimap< string, string >::const_iterator tMCI;
    pair< tMCI, tMCI > range = m_pairs.equal_range( name );
    vector< string > values;
    for ( tMCI p = range.first; p != range.second; ++p )
        values.push_back( p->second );
    return values;
}

//-----------------------------------------------------------------------------

string 
ConfigFile::Value( const string & name ) const
{
    typedef multimap< string, string >::const_iterator tMCI;
    tMCI p = m_pairs.find( name );
    if ( p != m_pairs.end() )
        return p->second;
    else
        return "";
}

//=============================================================================

#ifdef DEBUG

bool
ConfigFile::Test( )
{
    bool ok = true;
    cout << "Testing ConfigFile" << endl;

    const char confFileName[] = "TestConfig.cfg";
    char testContents[ ]
            = "SingleKey =Single Value\n"
            "MultiKey = Multi Value 1\n"
            "#Comment=just a comment\n"
            "Equal Key=a=b\n"
            "EmptyKey\n"
            "\n"
            "MultiKey=Multi Value 2 \n"
            ;
    cout << confFileName << " contents:" << endl << "--------" << endl
         << testContents << endl << "--------" << endl;
    DataBuffer buff;
    buff.Add( testContents, sizeof( testContents ) );
    File::Save( confFileName, buff );

    ConfigFile confFile( confFileName );
    TESTCHECK( confFile.Value( "SingleKey" ), string( "Single Value" ), &ok );
    vector< string > multiValues = confFile.Values( "MultiKey" );
    cout << "Values( \"MultiKey\" )" << endl;
    TESTCHECK( multiValues.size(), 2, &ok );
    TESTCHECK( multiValues[0], string( " Multi Value 1" ), &ok );
    TESTCHECK( multiValues[1], string( "Multi Value 2 " ), &ok );
    TESTCHECK( confFile.Value( "Equal Key" ), string( "a=b" ), &ok );
    TESTCHECK( confFile.Value( "EmptyKey" ), string( "" ), &ok );
    TESTCHECK( confFile.Values( "EmptyKey" ).size(), 1, &ok );
    TESTCHECK( confFile.Value( "#Comment" ), string( "" ), &ok );
    TESTCHECK( confFile.Values( "#Comment" ).size(), 0, &ok );
    TESTCHECK( confFile.Value( "NoKey" ), string( "" ), &ok );
    TESTCHECK( confFile.Values( "NoKey" ).size(), 0, &ok );

    bool delRslt = File::Delete( confFileName );
    Assert( delRslt );

    if ( ok )
        cout << "ConfigFile PASSED." << endl << endl;
    else
        cout << "ConfigFile FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

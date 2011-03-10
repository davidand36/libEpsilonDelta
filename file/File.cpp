/*
  File.hpp
  Copyright (C) 2009 David M. Anderson

*/


#include "File.hpp"
#include "FileException.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
#include "FileReader.hpp"
#include "FileWriter.hpp"
#include "DirUtil.hpp"
#include "Array.hpp"
#include <iostream>
#include <cstring>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Logger File::ms_log( "File" );

//=============================================================================

File::File( const std::string & fileName, Mode mode, Type type )
{
    m_fileName = fileName;
#ifdef USE_CSTD_FILE
    char modeStr[3] = "\0\0";
    modeStr[0] = (mode == File::ReadMode  ?  'r'  :  'w');
    if ( type == File::Binary )
        modeStr[1] = 'b';
    m_file = fopen( fileName.c_str(), modeStr );

    const char * modeNames[] = { "Reading", "Writing" };
    if ( m_file != 0 )
    {
        ms_log( Logger::Info, "Opened %s for %s",
                m_fileName.c_str(), modeNames[ mode ] );
    }
    else
    {
        ms_log( Logger::Error, "Unable to open %s for %s",
                m_fileName.c_str(), modeNames[ mode ] );
        throw FileOpenException( m_fileName, mode );
    }
#endif
}

//-----------------------------------------------------------------------------

File::~File( )
{
#ifdef USE_CSTD_FILE
    fclose( m_file );
    ms_log( Logger::Info, "Closed %s", m_fileName.c_str() );
#endif
}

//=============================================================================

int
File::Seek( int offset, Origin origin )
{
#ifdef USE_CSTD_FILE
    int origins[3] = { SEEK_SET, SEEK_CUR, SEEK_END };
    int seekRslt = fseek( m_file, offset, origins[ origin ] );

    const char * originNames[] = { "Beginning", "Current", "End" };
    if ( seekRslt == 0 )
    {
        int location = (int) ftell( m_file );
        ms_log( Logger::Debug, "Seeked to %d (%d from %s) in %s",
                location, offset, originNames[ origin ], m_fileName.c_str() );
        return location;
    }
    else
    {
        ms_log( Logger::Error, "Unable to seek to %d from %s in %s",
                offset, originNames[ origin ], m_fileName.c_str() );
        throw FileSeekException( m_fileName, offset, origin );
    }
#endif
}

//=============================================================================

const std::string &
File::FileName( ) const
{
    return m_fileName;
}

//=============================================================================

#ifdef USE_CSTD_FILE

FILE *
File::Handle( )
{
    return m_file;
}

#endif

//=============================================================================

Logger &
File::Log( )
{
    return ms_log;
}

//=============================================================================

#ifdef DEBUG

bool
File::Test( )
{
    bool ok = true;
    cout << "Testing File" << endl;

    try
    {
        const string testDataFileName = "FileTest.dat";
        char testData[ 266 ]
                = { 0, '\r', '\n', '\r', '\n', '\n', '\r', '\r', 0, '\n' };
        for ( int i = 0; i < 256; ++i )
            testData[ i + 10 ] = static_cast< char >( i );
        DataBuffer buff;
        buff.Add( testData, 266 );
        {
            cout << "FileWriter( string ) constructor" << endl;
            FileWriter writer( testDataFileName );
            cout << "writer.Save( DataBuffer )" << endl;
            writer.Save( buff );
        }
        TESTCHECK( FileExists( testDataFileName ), true, &ok );
        TESTCHECK( FileSize( testDataFileName ),
                   (int) (ARRAY_LENGTH( testData )), &ok );
        DateTime now( true );
        DateTime then = now;
        then.Increment( 0, 0, 0, 0, -1 );   //1 minute ago
        TESTCHECK( (now < FileModDate( testDataFileName )), false, &ok );
        TESTCHECK( (then < FileModDate( testDataFileName )), true, &ok );
        buff.Clear( );
        {
            cout << "FileReader( string ) constructor" << endl;
            FileReader reader( testDataFileName );
            cout << "writer.Load( DataBuffer * )" << endl;
            reader.Load( &buff );
        }
        const vector< char > & testData1 = buff.Buffer();
        TESTCHECK( testData1.size(), ARRAY_LENGTH( testData ), &ok );
        TESTCHECK( memcmp( &testData1[0], testData, ARRAY_LENGTH( testData ) ),
                   0, &ok );
        cout << "DeleteFile( string )" << endl;
        DeleteFile( testDataFileName );
        TESTCHECK( FileExists( testDataFileName ), false, &ok );

        const string testTextFileName = "FileTest.txt";
        const string testText = "Four score and seven years ago\n"
                "our fathers set forth upon this continent\n\r"
                "a new nation,\r\n"
                "conceived in liberty\r"
                "and dedicated to the proposition\x0"
                "that all men are created equal.";
        {
            cout << "FileWriter( string, File::Text ) constructor" << endl;
            FileWriter writer( testTextFileName, File::Text );
            cout << "writer.Save( string )" << endl;
            writer.Save( testText );
        }
        TESTCHECK( FileExists( testTextFileName ), true, &ok );
        string testText1;
        {
            cout << "FileReader( string, File::Text ) constructor" << endl;
            FileReader reader( testTextFileName, File::Text );
            cout << "reader.Load( string * )" << endl;
            reader.Load( &testText1 );
        }
        TESTCHECK( (testText1 == testText), true, &ok );
        cout << "DeleteFile( string )" << endl;
        DeleteFile( testTextFileName );
        TESTCHECK( FileExists( testTextFileName ), false, &ok );
    }
    catch ( FileException & except )
    {
        cout << except.Description() << endl;
        ok = false;
    }
    
    if ( ok )
        cout << "File PASSED." << endl << endl;
    else
        cout << "File FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  FileUtil.cpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for files.
*/


#include "FileUtil.hpp"
#include "Assert.hpp"
#include "FileException.hpp"
#include "SmartPtr.hpp"
#include "StringUtil.hpp"
#include <cstdio>
#include <sys/stat.h>
#include <ctime>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Array.hpp"
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool 
FileExists( const std::string & fileName )
{
    FILE * file = fopen( fileName.c_str(), "r" );
    if ( file != 0 )
    {
        fclose( file );
        return true;
    }
    return false;
}

//=============================================================================

int 
FileSize( const std::string & fileName )
{
    struct ::stat fileStatus;
    int statRslt = ::stat( fileName.c_str(), &fileStatus );
    if ( statRslt != 0 )
        throw FileException( "Unable to get status of " + fileName );
    return static_cast< int >( fileStatus.st_size );
}

//-----------------------------------------------------------------------------

DateTime 
FileModDate( const std::string & fileName )
{
    struct ::stat fileStatus;
    int statRslt = ::stat( fileName.c_str(), &fileStatus );
    if ( statRslt != 0 )
        throw FileException( "Unable to get status of " + fileName );
    std::time_t modDate = fileStatus.st_mtime;
    return DateTime( modDate );
}

//=============================================================================

std::vector< char >
LoadFile( const std::string & fileName )
{
    FILE * file = fopen( fileName.c_str(), "rb" );
    if ( file == 0 )
        throw FileException( "Unable to open " + fileName + " for reading." );
    int res = fseek( file, 0, SEEK_END );
    if ( res != 0 )
        throw FileException( "Unable to seek in " + fileName );
    long size = ftell( file );
    res = fseek( file, 0, SEEK_SET );
    if ( res != 0 )
        throw FileException( "Unable to seek in " + fileName );
    scoped_array< char > buffer( new char[ size ] );
    int bytesRead = fread( buffer.get(), 1, size, file );
    if ( bytesRead != size )
        throw FileException( "Read of " + fileName + " failed: "
                         "size=" + IntToString( size )
                         + " bytesRead=" + IntToString( bytesRead ) );
    fclose( file );
    return std::vector< char >( buffer.get(), buffer.get() + size );
}

//-----------------------------------------------------------------------------

std::string 
LoadTextFile( const std::string & fileName )
{
    FILE * file = fopen( fileName.c_str(), "r" );
    if ( file == 0 )
        throw FileException( "Unable to open " + fileName + " for reading." );
    int res = fseek( file, 0, SEEK_END );
    if ( res != 0 )
        throw FileException( "Unable to seek in " + fileName );
    long size = ftell( file );
    res = fseek( file, 0, SEEK_SET );
    if ( res != 0 )
        throw FileException( "Unable to seek in " + fileName );
    shared_array< char > buffer( new char[ size ] );
    int bytesRead = fread( buffer.get(), 1, size, file );
    Assert( bytesRead <= size );
    fclose( file );
    string textStr( buffer.get(), bytesRead );
    return textStr;
}

//=============================================================================

void 
SaveFile( const std::string & fileName, const char * data, int dataSize )
{
    FILE * file = fopen( fileName.c_str(), "wb" );
    if ( file == 0 )
        throw FileException( "Unable to open " + fileName + " for writing." );
    size_t bytesWritten = fwrite( data, 1, dataSize, file );
    if ( bytesWritten != static_cast<size_t>( dataSize ) )
        throw FileException( "Write to " + fileName + " failed: "
                         "dataSize=" + IntToString( dataSize )
                         + " bytesWritten=" + IntToString( bytesWritten ) );
    fclose( file );
}

//-----------------------------------------------------------------------------

void 
SaveTextFile( const std::string & fileName, const std::string & text )
{
    FILE * file = fopen( fileName.c_str(), "w" );
    if ( file == 0 )
        throw FileException( "Unable to open " + fileName + " for writing." );
#ifdef DEBUG
    size_t bytesWritten =
#endif
            fwrite( text.c_str(), 1, text.size(), file );
    Assert( bytesWritten >= text.size() );
    fclose( file );
}

//=============================================================================

#ifdef DEBUG

bool 
TestFileUtil( )
{
    bool ok = true;
    cout << "Testing FileUtil" << endl;

    const string testDataFileName = "FileTest.dat";
    char testData[ 266 ]
            = { 0, '\r', '\n', '\r', '\n', '\n', '\r', '\r', 0, '\n' };
    for ( int i = 0; i < 256; ++i )
        testData[ i + 10 ] = static_cast< char >( i );
    cout << "SaveFile()" << endl;
    SaveFile( testDataFileName, testData, ARRAYSIZE( testData ) );
    TESTCHECK( FileExists( testDataFileName ), true, &ok );
    TESTCHECK( FileSize( testDataFileName ),
               static_cast<int>(ARRAYSIZE( testData )), &ok );
    DateTime now( true );
    DateTime then = now;
    then.Increment( 0, 0, 0, 0, -1 );
    TESTCHECK( (now < FileModDate( testDataFileName )), false, &ok );
    TESTCHECK( (then < FileModDate( testDataFileName )), true, &ok );
    cout << "LoadFile()" << endl;
    std::vector< char > testData1 = LoadFile( testDataFileName );
    TESTCHECK( testData1.size(), ARRAYSIZE( testData ), &ok );
    TESTCHECK( memcmp( &testData1[0], testData, ARRAYSIZE( testData ) ),
               0, &ok );
    int rmvRslt = remove( testDataFileName.c_str() );
    Assert( rmvRslt == 0 );
    TESTCHECK( FileExists( testDataFileName ), false, &ok );

    const string testTextFileName = "FileTest.txt";
    const string testText = "Four score and seven years ago\n"
            "our fathers set forth upon this continent\n\r"
            "a new nation,\r\n"
            "conceived in liberty\r"
            "and dedicated to the proposition\x0"
            "that all men are created equal.";
    cout << "SaveTextFile()" << endl;
    SaveTextFile( testTextFileName, testText );
    TESTCHECK( FileExists( testTextFileName ), true, &ok );
    cout << "LoadTextFile()" << endl;
    string testText1 = LoadTextFile( testTextFileName );
    TESTCHECK( (testText1 == testText), true, &ok );
    rmvRslt = remove( testTextFileName.c_str() );
    Assert( rmvRslt == 0 );
    TESTCHECK( FileExists( testTextFileName ), false, &ok );
    
    if ( ok )
        cout << "FileUtil PASSED." << endl << endl;
    else
        cout << "FileUtil FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

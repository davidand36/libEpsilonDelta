/*
  File.hpp
  Copyright (C) 2009 David M. Anderson

  File class: Basic file operations, wrapped for platform-independence.
*/


#include "File.hpp"
#include "FileException.hpp"
#include "Platform.hpp"

#if defined(OS_UNIX) || defined(OS_WINDOWS)
#define USE_CSTD_FILE
#endif

#ifdef USE_CSTD_FILE
#include <cstdio>
#include <sys/stat.h>
#include <ctime>
#include "Assert.hpp"
#endif
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Array.hpp"
#include <iostream>
#endif
using namespace std;




namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FileImpl
{
public:
    FileImpl( );
    virtual ~FileImpl( );
    virtual bool Open( const string & fileName,
                       File::Mode mode, File::Type type );
    virtual void Close( );
    virtual bool IsOpen( );
    virtual bool Read( char * buffer, int bufferSize, int * pBytesRead );
    virtual bool Write( const char * buffer, int bufferSize );
    virtual int Seek( int offset, File::Origin origin );

    static bool Exists( const std::string & fileName );
    static bool Delete( const std::string & fileName );
    static int Size( const std::string & fileName );
    static DateTime ModDate( const std::string & fileName );

private:
#ifdef USE_CSTD_FILE
    FILE *  m_file;
#endif
};


//=============================================================================


namespace
{                                                                   //namespace
//=============================================================================


bool SaveBuff( const string & fileName, const char * buffer, int bufferSize,
               File::Type type );
bool LoadBuff( const string & fileName, char ** pBuffer, int * pBufferSize,
               File::Type type );


//=============================================================================
}                                                                   //namespace


//*****************************************************************************


File::File( const string & fileName )
    :   m_fileName( fileName ),
        m_pImpl( new FileImpl )
{
}

//-----------------------------------------------------------------------------

File::~File( )
{
    if ( m_pImpl->IsOpen() )
        Close( );
}

//=============================================================================

bool 
File::Open( Mode mode, Type type )
{
    if ( m_pImpl->IsOpen() )
    {
        ms_log( Logger::Warning, "%s is already open", m_fileName.c_str() );
        return false;   //might have been opened in different mode
    }
    bool openRslt = m_pImpl->Open( m_fileName, mode, type );
    const char * modeNames[] = { "Reading", "Writing" };
    if ( openRslt )
        ms_log( Logger::Info, "Opened %s for %s",
                m_fileName.c_str(), modeNames[ mode ] );
    else
        ms_log( Logger::Error, "Unable to open %s for %s",
                m_fileName.c_str(), modeNames[ mode ] );
    return openRslt;
}

//=============================================================================

void 
File::Close( )
{
    m_pImpl->Close( );
    ms_log( Logger::Debug, "Closed %s", m_fileName.c_str() );
}

//=============================================================================

bool 
File::Read( char * buffer, int bufferSize, int * pBytesRead )
{
    bool readRslt = m_pImpl->Read( buffer, bufferSize, pBytesRead );
    if ( readRslt )
        ms_log( Logger::Debug, "Read %d bytes from %s",
               bufferSize, m_fileName.c_str() );
    else
        ms_log( Logger::Debug, "Unable to read %d bytes from %s",
               bufferSize, m_fileName.c_str() );
    return readRslt;
}

//-----------------------------------------------------------------------------

bool 
File::Write( const char * buffer, int bufferSize )
{
    bool writeRslt = m_pImpl->Write( buffer, bufferSize );
    if ( writeRslt )
        ms_log( Logger::Debug, "Wrote %d bytes to %s",
               bufferSize, m_fileName.c_str() );
    else
        ms_log( Logger::Debug, "Unable to write %d bytes to %s",
               bufferSize, m_fileName.c_str() );
    return writeRslt;
}

//=============================================================================

int 
File::Seek( int offset, Origin origin )
{
    int location = m_pImpl->Seek( offset, origin );
    const char * originNames[] = { "Beginning", "Current", "End" };
    if ( location >= 0 )
        ms_log( Logger::Debug, "Seeked to %d (%d from %s) in %s",
                location, offset, originNames[ origin ], m_fileName.c_str() );
    else
        ms_log( Logger::Debug, "Unable to seek to %d from %s in %s",
                offset, originNames[ origin ], m_fileName.c_str() );
    return location;
}

//=============================================================================

bool 
File::Exists( const std::string & fileName )
{
    return FileImpl::Exists( fileName );
}

//-----------------------------------------------------------------------------

bool 
File::Delete( const std::string & fileName )
{
    return FileImpl::Delete( fileName );
}

//-----------------------------------------------------------------------------

int 
File::Size( const std::string & fileName )
{
    return FileImpl::Size( fileName );
}

//-----------------------------------------------------------------------------

DateTime 
File::ModDate( const std::string & fileName )
{
    return FileImpl::ModDate( fileName );
}

//=============================================================================

bool 
File::Save( const std::string & fileName, const DataBuffer & buffer )
{
    const vector< char > & data = buffer.Buffer();
    return SaveBuff( fileName, &data[0], (int)data.size(), Binary );
}

//-----------------------------------------------------------------------------

bool 
File::Save( const std::string & fileName, const std::string & text )
{
    return SaveBuff( fileName, text.c_str(), (int)text.size(), Text );
}

//=============================================================================

bool 
File::Load( const std::string & fileName, DataBuffer * pBuffer )
{
    pBuffer->Clear( );
    char * buffer;
    int bufferSize;
    bool loadRslt = LoadBuff( fileName, &buffer, &bufferSize, Binary );
    if ( loadRslt )
    {        
        pBuffer->Add( buffer, bufferSize );
        delete[] buffer;
    }
    return loadRslt;
}

//-----------------------------------------------------------------------------

bool 
File::Load( const std::string & fileName, std::string * pText )
{
    char * buffer;
    int bufferSize;
    bool loadRslt = LoadBuff( fileName, &buffer, &bufferSize, Text );
    if ( loadRslt )
    {        
        pText->assign( buffer, buffer + bufferSize );
        delete[] buffer;
    }
    return loadRslt;
}

//=============================================================================

Logger & 
File::Log( )
{
    return ms_log;
}

//=============================================================================

Logger File::ms_log( "File" );


//*****************************************************************************


namespace
{                                                                   //namespace
//=============================================================================

bool 
SaveBuff( const string & fileName, const char * buffer, int bufferSize,
          File::Type type )
{
    File file( fileName );
    if ( ! file.Open( File::WriteMode, type ) )
         return false;
    if ( ! file.Write( buffer, bufferSize ) )
        return false;
    file.Close( );
    return true;
}

//-----------------------------------------------------------------------------

bool 
LoadBuff( const string & fileName, char ** pBuffer, int * pBufferSize,
          File::Type type )
{
    File file( fileName );
    if ( ! file.Open( File::ReadMode, type ) )
        return false;
    int fileSize = file.Seek( 0, File::End );
    if ( fileSize <= 0 )
        return false;
    if ( file.Seek( 0 ) != 0 )
        return false;
    *pBuffer = new char[ fileSize ];  //Not deleted by this function on success!
    memset( *pBuffer, 0, fileSize );
    int bytesRead = 0;
    if ( ! file.Read( *pBuffer, fileSize, &bytesRead ) )
    {
        delete[] *pBuffer;
        *pBuffer = 0;
        *pBufferSize = 0;
        return false;
    }
    *pBufferSize = bytesRead;   //In text mode, might be < fileSize.
    file.Close( );
    return true;
}

//=============================================================================
}                                                                   //namespace


//*****************************************************************************


#ifdef USE_CSTD_FILE

//=============================================================================


FileImpl::FileImpl( )
    :   m_file( 0 )
{
}

//-----------------------------------------------------------------------------

FileImpl::~FileImpl( )
{
}

//=============================================================================

bool 
FileImpl::Open( const string & fileName, File::Mode mode, File::Type type )
{
    Assert( m_file == 0 );
    char modeStr[3] = "\0\0";
    modeStr[0] = (mode == File::ReadMode  ?  'r'  :  'w');
    if ( type == File::Binary )
        modeStr[1] = 'b';
    m_file = fopen( fileName.c_str(), modeStr );
    return ( m_file != 0 );
}

//=============================================================================

void 
FileImpl::Close( )
{
    if ( m_file )
        fclose( m_file );
    m_file = 0;
}

//=============================================================================

bool 
FileImpl::IsOpen( )
{
    return (m_file != 0);
}

//=============================================================================

bool 
FileImpl::Read( char * buffer, int bufferSize, int * pBytesRead )
{
    if ( m_file == 0 )
        return false;
    size_t bytesRead = fread( buffer, 1, (size_t)bufferSize, m_file );
    if ( pBytesRead )
        *pBytesRead = bytesRead;
    if ( bytesRead == (size_t)bufferSize )
        return true;
    return ( ferror( m_file ) == 0 );
}

//-----------------------------------------------------------------------------

bool 
FileImpl::Write( const char * buffer, int bufferSize )
{
    if ( m_file == 0 )
        return false;
    size_t numWritten = fwrite( buffer, (size_t)bufferSize, 1, m_file );
    return (numWritten > 0);
}

//=============================================================================

int 
FileImpl::Seek( int offset, File::Origin origin )
{
    if ( m_file == 0 )
        return -1;
    int origins[3] = { SEEK_SET, SEEK_CUR, SEEK_END };
    int seekRslt = fseek( m_file, offset, origins[ origin ] );
    if ( seekRslt != 0 )
        return -1;
    return (int)ftell( m_file );
}

//=============================================================================

bool 
FileImpl::Exists( const std::string & fileName )
{
    FILE * file = fopen( fileName.c_str(), "r" );
    if ( file != 0 )
    {
        fclose( file );
        return true;
    }
    return false;
}

//-----------------------------------------------------------------------------

bool 
FileImpl::Delete( const std::string & fileName )
{
    int removeRslt = remove( fileName.c_str() );
    return (removeRslt == 0);
}

//-----------------------------------------------------------------------------

int 
FileImpl::Size( const std::string & fileName )
{
    struct ::stat fileStatus;
    int statRslt = ::stat( fileName.c_str(), &fileStatus );
    if ( statRslt != 0 )
        throw FileException( "Unable to get status of " + fileName );
    return static_cast< int >( fileStatus.st_size );
}

//-----------------------------------------------------------------------------

DateTime 
FileImpl::ModDate( const std::string & fileName )
{
    struct ::stat fileStatus;
    int statRslt = ::stat( fileName.c_str(), &fileStatus );
    if ( statRslt != 0 )
        throw FileException( "Unable to get status of " + fileName );
    std::time_t modDate = fileStatus.st_mtime;
    return DateTime( modDate );
}

//=============================================================================

#endif //USE_CSTD_FILE


//*****************************************************************************

#ifdef DEBUG

bool 
File::Test( )
{
    bool ok = true;
    cout << "Testing File" << endl;

    const string testDataFileName = "FileTest.dat";
    char testData[ 266 ]
            = { 0, '\r', '\n', '\r', '\n', '\n', '\r', '\r', 0, '\n' };
    for ( int i = 0; i < 256; ++i )
        testData[ i + 10 ] = static_cast< char >( i );
    DataBuffer buff;
    buff.Add( testData, 266 );
    TESTCHECK( File::Save( testDataFileName, buff ), true, &ok );
    TESTCHECK( File::Exists( testDataFileName ), true, &ok );
    TESTCHECK( File::Size( testDataFileName ),
               static_cast<int>(ARRAY_LENGTH( testData )), &ok );
    DateTime now( true );
    DateTime then = now;
    then.Increment( 0, 0, 0, 0, -1 );   //1 minute ago
    TESTCHECK( (now < File::ModDate( testDataFileName )), false, &ok );
    TESTCHECK( (then < File::ModDate( testDataFileName )), true, &ok );
    buff.Clear( );
    TESTCHECK( File::Load( testDataFileName, &buff ), true, &ok );
    const vector< char > & testData1 = buff.Buffer();
    TESTCHECK( testData1.size(), ARRAY_LENGTH( testData ), &ok );
    TESTCHECK( memcmp( &testData1[0], testData, ARRAY_LENGTH( testData ) ),
               0, &ok );
    TESTCHECK( File::Delete( testDataFileName ), true, &ok );
    TESTCHECK( File::Exists( testDataFileName ), false, &ok );

    const string testTextFileName = "FileTest.txt";
    const string testText = "Four score and seven years ago\n"
            "our fathers set forth upon this continent\n\r"
            "a new nation,\r\n"
            "conceived in liberty\r"
            "and dedicated to the proposition\x0"
            "that all men are created equal.";
    TESTCHECK( File::Save( testTextFileName, testText ), true, &ok );
    TESTCHECK( File::Exists( testTextFileName ), true, &ok );
    string testText1;
    TESTCHECK( File::Load( testTextFileName, &testText1 ), true, &ok );
    TESTCHECK( (testText1 == testText), true, &ok );
    TESTCHECK( File::Delete( testTextFileName ), true, &ok );
    TESTCHECK( File::Exists( testTextFileName ), false, &ok );
    
    if ( ok )
        cout << "File PASSED." << endl << endl;
    else
        cout << "File FAILED." << endl << endl;
    return ok;
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

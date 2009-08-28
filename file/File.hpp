#ifndef FILE_HPP
#define FILE_HPP
/*
  File.hpp
  Copyright (C) 2009 David M. Anderson

  File class: Basic file operations, wrapped for platform-independence.
*/


#include "DataBuffer.hpp"
#include "DateTime.hpp"
#include "Logger.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FileImpl;


//*****************************************************************************


class File
{
public:
    enum Mode { ReadMode, WriteMode };
    enum Type { Binary, Text };
    enum Origin { Beginning, Current, End };

    File( const std::string & fileName );
    virtual ~File( );
    virtual bool Open( Mode mode, Type type = Binary );
    virtual bool Read( char * buffer, int bufferSize, int * pBytesRead = 0 );
    template < typename T >
    bool Read( T * pT );
    virtual bool Write( const char * buffer, int bufferSize );
    template <typename T>
    bool Write( const T & t );
    virtual int Seek( int offset, Origin origin = Beginning );
    virtual void Close( );

    static bool Exists( const std::string & fileName );
    static bool Delete( const std::string & fileName );
    static int Size( const std::string & fileName );
    static DateTime ModDate( const std::string & fileName );
    static bool Save( const std::string & fileName, const DataBuffer & buffer );
    static bool Save( const std::string & fileName, const std::string & text );
    static bool Load( const std::string & fileName, DataBuffer * pBuffer );
    static bool Load( const std::string & fileName, std::string * pText );

    static Logger & Log( );
#ifdef DEBUG
    static bool Test( );
#endif

private:
    std::string             m_fileName;
    shared_ptr< FileImpl >  m_pImpl;

    static Logger           ms_log;
};


//*****************************************************************************


template < typename T >
bool 
File::Read( T * pT )
{
    return Read( reinterpret_cast< char * >( pT ), (int)sizeof( T ) );
}

//=============================================================================

template <typename T>
bool 
File::Write( const T & t )
{
    return Write( reinterpret_cast< const char * >( &t ), (int)sizeof( T ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILE_HPP

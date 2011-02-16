#ifndef FILE_HPP
#define FILE_HPP
/*
  File.hpp
  Copyright (C) 2011 David M. Anderson

  File class representing a file, wrapped for platform independence.
  Provides the basic operations of opening and closing through construction and
  destruction, as well as random access.
  NOTES:
  1. This is intended primarily as a base class for FileReader and FileWriter.
*/


#include "RandomAccess.hpp"
#include "Platform.hpp"
#include "Logger.hpp"
#include <string>
#if defined(OS_UNIX) || defined(OS_ANDROID) || defined(OS_WINDOWS)
#define USE_CSTD_FILE
#endif
#ifdef USE_CSTD_FILE
#include <cstdio>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class File
    :   public virtual RandomAccess
{
public:
    enum Mode { ReadMode, WriteMode };
    enum Type { Binary, Text };

    File( const std::string & fileName, Mode mode, Type type = Binary );
    virtual ~File( );

    virtual int Seek( int offset, Origin origin = Beginning );

    const std::string & FileName( ) const;
#ifdef USE_CSTD_FILE
    std::FILE * Handle( );
#endif
    static Logger & Log( );

#ifdef DEBUG
    static bool Test( );
#endif
    
private:
    std::string     m_fileName;
#ifdef USE_CSTD_FILE
    std::FILE *     m_file;
#endif
    
    static Logger   ms_log;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILE_HPP

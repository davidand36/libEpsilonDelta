#ifndef FILEEXCEPTION_HPP
#define FILEEXCEPTION_HPP
/*
  FileException.hpp
  Copyright (C) 2007 David M. Anderson

  FileException class, representing problems accessing files or directories.
*/


#include "Exception.hpp"
#include "File.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FileException
    :    public RuntimeError
{
public:
    explicit FileException( const std::string & description );
};


//*****************************************************************************


class FileOpenException
    :   public FileException
{
public:
    FileOpenException( const std::string & fileName, File::Mode mode );
};


//*****************************************************************************


class FileSeekException
    :   public FileException
{
public:
    FileSeekException( const std::string & fileName,
                       int offset, RandomAccess::Origin origin );
};


//*****************************************************************************


class FileReadException
    :   public FileException
{
public:
    FileReadException( const std::string & fileName, int bytesRequested );
};


//*****************************************************************************


class FileWriteException
    :   public FileException
{
public:
    FileWriteException( const std::string & fileName, int bytesRequested );
};


//*****************************************************************************


class FileDeleteException
    :   public FileException
{
public:
    FileDeleteException( const std::string & fileName );
};


//*****************************************************************************


class FileStatusException
    :   public FileException
{
public:
    FileStatusException( const std::string & fileName );
};


//*****************************************************************************


class ReadDirException
    :   public FileException
{
public:
    ReadDirException( const std::string & dirName );
};


//*****************************************************************************


class MkdirException
    :   public FileException
{
public:
    MkdirException( const std::string & dirName );
};


//*****************************************************************************


class RmdirException
    :   public FileException
{
public:
    RmdirException( const std::string & dirName );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILEEXCEPTION_HPP

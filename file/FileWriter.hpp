#ifndef FILEWRITER_HPP
#define FILEWRITER_HPP
/*
  FileWriter.hpp
  Copyright (C) 2011 David M. Anderson

  FileWriter class representing a file open for writing.
*/


#include "File.hpp"
#include "Writer.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FileWriter
    :   public File,
        public Writer
{
public:
    FileWriter( const std::string & fileName, Type type = File::Binary );
    virtual ~FileWriter( ) { }

    using Writer::Write;
    virtual void Write( const char * buffer, int bufferSize );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILEWRITER_HPP

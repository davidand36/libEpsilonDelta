#ifndef FILEREADER_HPP
#define FILEREADER_HPP
/*
  FileReader.hpp
  Copyright (C) 2011 David M. Anderson

  FileReader class representing a file open for reading.
*/


#include "File.hpp"
#include "Reader.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FileReader
    :   public File,
        public Reader
{
public:
    FileReader( const std::string & fileName, Type type = File::Binary );
    virtual ~FileReader( ) { }

    virtual void Read( char * buffer, int bufferSize );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILEREADER_HPP

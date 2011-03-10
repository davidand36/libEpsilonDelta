/*
  FileReader.cpp
  Copyright (C) 2011 David M. Anderson

  FileReader class representing a file open for reading.
*/


#include "FileReader.hpp"
#include "FileException.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


FileReader::FileReader( const std::string & fileName, Type type )
    :   File( fileName, File::ReadMode, type )
{
}

//=============================================================================

void
FileReader::Read( char * buffer, int bufferSize )
{
#ifdef USE_CSTD_FILE
    size_t bytesRead = fread( buffer, 1, (size_t) bufferSize, Handle() );
    if ( (ferror( Handle() ) == 0) && (bytesRead == (size_t) bufferSize) )
    {
        Log()( Logger::Debug, "Read %d bytes from %s",
               bufferSize, FileName().c_str() );
    }
    else
    {
        Log()( Logger::Error, "Unable to read %d bytes from %s",
               bufferSize, FileName().c_str() );
        throw FileReadException( FileName(), bufferSize );
    }
#endif
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

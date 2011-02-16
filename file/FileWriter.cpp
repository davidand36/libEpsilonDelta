/*
  FileWriter.cpp
  Copyright (C) 2011 David M. Anderson

  FileWriter class representing a file open for writing.
*/


#include "FileWriter.hpp"
#include "FileException.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


FileWriter::FileWriter( const std::string & fileName, Type type )
    :   File( fileName, File::WriteMode, type )
{
}

//=============================================================================

void
FileWriter::Write( const char * buffer, int bufferSize )
{
#ifdef USE_CSTD_FILE
    size_t bytesWritten = fwrite( buffer, 1, (size_t) bufferSize, Handle() );
    if ( bytesWritten == (size_t) bufferSize )
    {
        Log()( Logger::Info, "Wrote %d bytes to %s",
               bufferSize, FileName().c_str() );
    }
    else
    {
        Log()( Logger::Error, "Unable to write %d bytes to %s",
               bufferSize, FileName().c_str() );
        throw FileWriteException( FileName(), bufferSize );
    }
#endif
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

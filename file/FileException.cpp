/*
  FileException.cpp
  Copyright (C) 2007 David M. Anderson

  FileException class, representing problems accessing files or directories.
*/


#include "FileException.hpp"
#include "StringUtil.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


FileException::FileException( const string & description )
    :   RuntimeError( description )
{ 
}


//*****************************************************************************


FileOpenException::FileOpenException( const string & fileName,
                                      File::Mode mode )
    :   FileException( "" )
{
    const string modeNames[] = { "Reading", "Writing" };
    string description = "File open error: " + fileName
            + " for " + modeNames[ mode ];
    SetDescription( description );
}


//*****************************************************************************


FileSeekException::FileSeekException( const string & fileName,
                                      int offset, RandomAccess::Origin origin )
    :   FileException( "" )
{
    const string originNames[] = { "Beginning", "Current", "End" };
    string description = "File seek error: " + fileName
            + " to " + IntToString( offset ) + " from " + originNames[ origin ];
    SetDescription( description );
}


//*****************************************************************************


FileReadException::FileReadException( const std::string & fileName,
                                      int bytesRequested )
    :   FileException( "" )
{
    string description = "File read error: " + fileName
            + " " + IntToString( bytesRequested ) + " bytes";
    SetDescription( description );
}


//*****************************************************************************


FileWriteException::FileWriteException( const std::string & fileName,
                                        int bytesRequested )
    :   FileException( "" )
{
    string description = "File write error: " + fileName
            + " " + IntToString( bytesRequested ) + " bytes";
    SetDescription( description );
}


//*****************************************************************************


FileDeleteException::FileDeleteException( const std::string & fileName )
    :   FileException( "File delete error: " + fileName )
{
}


//*****************************************************************************


FileStatusException::FileStatusException( const std::string & fileName )
    :   FileException( "File status error: " + fileName )
{
}


//*****************************************************************************


ReadDirException::ReadDirException( const std::string & dirName )
    :   FileException( "Read directory error: " + dirName )
{
}


//*****************************************************************************


MkdirException::MkdirException( const std::string & dirName )
    :   FileException( "Make directory error: " + dirName )
{
}


//*****************************************************************************


RmdirException::RmdirException( const std::string & dirName )
    :   FileException( "Remove directory error: " + dirName )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta


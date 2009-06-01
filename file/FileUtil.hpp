#ifndef FILEUTIL_HPP
#define FILEUTIL_HPP
/*
  FileUtil.hpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for files.
*/


#include "DateTime.hpp"
#include <string>
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


bool FileExists( const std::string & fileName );
int FileSize( const std::string & fileName );
DateTime FileModDate( const std::string & fileName );
std::vector< char > LoadFile( const std::string & fileName );
std::string LoadTextFile( const std::string & fileName );
void SaveFile( const std::string & fileName, const char * data, int dataSize );
void SaveTextFile( const std::string & fileName, const std::string text );

#ifdef DEBUG
bool TestFileUtil( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILEUTIL_HPP

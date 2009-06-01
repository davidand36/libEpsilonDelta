#ifndef DIRUTIL_HPP
#define DIRUTIL_HPP
/*
  DirUtil.hpp
  Copyright (C) 2007 David M. Anderson

  Some useful utility functions for directories.
*/


#include <string>
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void MakeDirectory( const std::string & dirName, int mode = 0777 );
void RemoveDirectory( const std::string & dirName );
std::vector< std::string > ListFiles( const std::string & dirName );
std::vector< std::string > ListSubdirectories( const std::string & dirName );

#ifdef DEBUG
bool TestDirUtil( );
#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DIRUTIL_HPP
  

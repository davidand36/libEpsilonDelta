#ifndef FILENAME_HPP
#define FILENAME_HPP
/*
  FileName.hpp
  Copyright (C) 2009 David M. Anderson

  FileName handling routines
*/


#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


namespace FileName
{                                                          //namespace FileName

//-----------------------------------------------------------------------------

void Split( const std::string & fileSpec,
            std::string * pPath, std::string * pName,
            std::string * pExtension );
std::string Make( const std::string & path, const std::string & name,
                  const std::string & extension );

#ifdef DEBUG
bool Test( );
#endif

//-----------------------------------------------------------------------------

}                                                          //namespace FileName

//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILENAME_HPP

/*
  FTException.cpp
  Copyright (C) 2007 David M. Anderson

  FTException class: Exception class for FreeType errors.
*/


#ifdef USE_FREETYPE


#include "FTException.hpp"
#include "StringUtil.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


FTException::FTException( const std::string & ftFunction, int errorCode )
    :   Exception( "Error: " + ftFunction + ": " + IntToString( errorCode ) )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_FREETYPE

#ifndef FTEXCEPTION_HPP
#define FTEXCEPTION_HPP
/*
  FTException.hpp
  Copyright (C) 2007 David M. Anderson

  FTException class: Exception class for FreeType errors.
*/


#ifdef USE_FREETYPE


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FTException
    : public Exception
{
public:
    FTException( const std::string & ftFunction, int errorCode );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_FREETYPE
#endif //FTEXCEPTION_HPP

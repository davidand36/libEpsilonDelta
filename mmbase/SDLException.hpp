#ifndef SDLEXCEPTION_HPP
#define SDLEXCEPTION_HPP
/*
  SDLException.hpp
  Copyright (C) 2007 David M. Anderson

  SDLException class: Exception class for Simple DirectMedia Library (SDL)
  errors.
*/


#ifdef USE_SDL


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SDLException
    : public RuntimeError
{
public:
    SDLException( const std::string & sdlFunction );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL
#endif //SDLEXCEPTION_HPP

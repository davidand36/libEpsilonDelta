/*
  SDLException.cpp
  Copyright (C) 2007 David M. Anderson

  SDLException class: Exception class for Simple DirectMedia Library (SDL)
  errors.
*/


#ifdef USE_SDL


#include "SDLException.hpp"
#include <SDL.h>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


SDLException::SDLException( const std::string & sdlFunction )
    :   Exception( "Error: " + sdlFunction )
{
    const char * error = ::SDL_GetError( );
    if ( error )
    {
        m_description += "\n";
        m_description += error;
    }
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL

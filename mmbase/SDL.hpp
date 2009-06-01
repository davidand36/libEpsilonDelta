#ifndef SDL_HPP
#define SDL_HPP
/*
  SDL.hpp
  Copyright (C) 2007 David M. Anderson

  SDL class: Handles initialization and shutdown of the Simple DirectMedia
  Layer (SDL).
  NOTES:
  1. This does not initialize any of the subsystems, only the library as a
     whole.
  2. This class doesn't need to be used by the application. It is used by the
     classes representing subsystems, such as SDLGraphics2DSystem.
  3. SDL is a singleton with the accessor
     SDL & SDL::Instance().
*/


#ifdef USE_SDL


#include "Singleton.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SDL
    :   public Singleton< SDL >
{
public:
    void Init( );

#ifdef DEBUG
    bool Test( );
#endif

private:
    SDL( );
    ~SDL( );

    bool m_initialized;

    friend class Singleton< SDL >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL
#endif //SDL_HPP

#ifndef GRAPHICS2D_HPP
#define GRAPHICS2D_HPP
/*
  Graphics2D.hpp
  Copyright (C) 2007 David M. Anderson

  Graphics2D class: The 2-D graphics system.
  NOTES:
  1. Init() needs to be called before making any other calls.
  2. Shutdown() is optional; it will be called automatically if needed.
*/


#include "Pixel.hpp"
#include "Singleton.hpp"
#include "SmartPtr.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class Surface;

//*****************************************************************************


class Graphics2D
    :   public Singleton< Graphics2D >
{
public:
    void Init( );
    void Shutdown( );

    void SetupScreen( int width, int height,
                      const char * title, 
                      EPixelType pixelType = NativePixelType,
                      bool fullScreen = false, bool openGL = false );
    shared_ptr< Surface > Screen( );
    void ShowScreen( );

#ifdef DEBUG
    void PrintVideoInfo( );
#ifdef USE_SDL
    void PrintAvailableResolutions( const ::SDL_PixelFormat & format,
                                    Uint32 flags );
#endif
    bool Test( bool testResolutions );
#endif

private:
    Graphics2D( );
    ~Graphics2D( );

    shared_ptr< Surface > m_spScreenSurface;

    friend class Singleton< Graphics2D >;
};


//*****************************************************************************


inline 
shared_ptr< Surface >
Graphics2D::Screen( )
{
    return m_spScreenSurface;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GRAPHICS2D_HPP

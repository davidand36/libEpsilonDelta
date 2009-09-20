#ifndef IMAGE_HPP
#define IMAGE_HPP
/*
  Image.hpp
  Copyright (C) 2007 David M. Anderson

  Image class: Represents a 2-D graphical image, optimized for blitting.
  NOTES:
  1. Images should be loaded after Graphics2D::SetupScreen() has been called, 
     because then the surface will be converted for fast blitting.
  2. It may not make a lot of sense to use a color key if the image has alpha, 
     but if so, it is assumed that the alpha value of the color key is 0.
*/


#include "Surface.hpp"
#include "DataBuffer.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Image
    :   public Surface
{
public:
    Image( const DataBuffer & dataBuffer, bool alpha = false );
    Image( const DataBuffer & dataBuffer, const Color3B & transparentColor );
    Image( const std::string & fileSpec, bool alpha = false );
    Image( const std::string & fileSpec, const Color3B & transparentColor );
    virtual ~Image( );

#ifdef DEBUG
    static bool TestLoad( const std::string & testFileSpec );
    static void TestDraw( );
    static void TestUnload( );
#endif

private:
#ifdef USE_SDL
    void Init( ::SDL_Surface * pSDL_Surface, bool alpha );
    void Init( ::SDL_Surface * pSDL_Surface,
               const Color3B & transparentColor );
#endif
};


//*****************************************************************************


inline 
Image::~Image( )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //IMAGE_HPP

/*
  Surface.hpp
  Copyright (C) 2007 David M. Anderson

  Surface class: Represents a rectangular array of pixels.
  NOTES:
  1. The copy constructor and assignment operator behave differently depending
     on whether the rhs object owns its representation. Typically, own = true,
     in which case the target object gets its own independent copy of the
     structure and pixels in the rhs. If own = false, then the representation
     (e.g. m_pSDL_Surface) is shared.
  2. The destructor frees up the pixels if own = true.
  3. Lock() returns an untyped pointer to the pixels. It will need to be
     typecast based on PixelType().
*/


#ifndef SURFACE_HPP
#define SURFACE_HPP


#include "Region.hpp"
#include "DataBuffer.hpp"
#include "Pixel.hpp"
#include "Rectangle.hpp"
#include "DrawingSurface.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Surface
    :   public Region
{
public:
    Surface( );
    Surface( int width, int height, EPixelType pixelType = NativePixelType );
#ifdef USE_SDL
    Surface( ::SDL_Surface * sdl_Surface, bool own = true );
#endif
    Surface( const DataBuffer & dataBuffer, bool alpha = false );
    Surface( const DataBuffer & dataBuffer, const Color3B & transparentColor );
    Surface( const std::string & fileSpec, bool alpha = false );
    Surface( const std::string & fileSpec, const Color3B & transparentColor );
    Surface( const Surface & rhs );
    virtual ~Surface( );
    Surface & operator=( const Surface & rhs );
    bool operator==( const Surface & rhs ) const;
    bool operator!=( const Surface & rhs ) const;
    virtual bool operator==( const Region & rhs ) const;
    void SetTransparentColor( const Color3B & transparentColor );
    bool GetTransparentColor( Color3B * pTransparentColor ) const;
    Rectangle Extent( ) const;
    virtual bool Contains( const Point2I & point ) const;
    int Pitch( ) const;
    EPixelType PixelType( ) const;
    Rectangle ClippingRect( ) const;
    void SetClippingRect( const Rectangle & clippingRect );
    void * Lock( );
    void const * Lock( ) const;
    void Unlock( ) const;
    void Blit( const Point2I & destPos, Surface * pDest = ms_current );
    void Blit( const Rectangle & srcRect, const Point2I & destPos,
               Surface * pDest = ms_current );
    template < typename Pxl >
    DrawingSurface< Pxl > LockDrawingSurface( );
    template < typename Pxl >
    DrawingSurface< Pxl > const LockDrawingSurface( ) const;
    template < typename Shape >
    void Draw( const Shape & shape, uint32_t pxl );
    template < typename Shape >
    void Draw( const Shape & shape, const Color3B & color );
    template < typename Shape >
    void Draw( const Shape & shape, const Color4B & color );
    template < typename Shape >
    void Fill( const Shape & shape, uint32_t pxl );
    template < typename Shape >
    void Fill( const Shape & shape, const Color3B & color );
    template < typename Shape >
    void Fill( const Shape & shape, const Color4B & color );
#ifdef USE_SDL
    ::SDL_Surface * GetSDL_Surface( );
    const ::SDL_Surface * GetSDL_Surface( ) const;
#endif
#ifdef USE_PNG
    void SavePng( const std::string & fileSpec );
#endif
#ifdef USE_JPEG
    void SaveJpeg( const std::string & fileSpec );
#endif

#ifdef DEBUG
    void PrintInfo( ) const;
    static bool Test( );
    static bool TestLoad( const std::string & testFileSpec );
    static void TestDraw( );
    static void TestUnload( );
#endif

    void MakeCurrent( );
    static Surface * Current( );

protected:
    void CopySurface( const Surface & rhs );

#ifdef USE_SDL
    ::SDL_Surface * m_pSDL_Surface;
#endif
    bool m_own;
    EPixelType m_pixelType;

    static Surface * ms_current;

private:
#ifdef USE_SDL
    void Init( bool alpha );
    void Init( const Color3B & transparentColor );
    template < typename Pxl >
    bool Contains( const Point2I & point, uint32_t colorKey ) const;
    template < typename Pxl >
    bool Contains( const Point2I & point ) const;
    template < typename Shape, typename Pxl >
    void Draw( const Shape & shape, Pxl pxl );
    template < typename Shape, typename Pxl >
    void Fill( const Shape & shape, Pxl pxl );
#endif
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef USE_SDL

#include "SDLSurface.hpp"

#endif


//*****************************************************************************

#endif //SURFACE_HPP

/*
  SDLSurface.cpp
  Copyright (C) 2007 David M. Anderson

  Surface class: Represents a rectangular array of pixels. Implemented using
  the Simple DirectMedia Layer (SDL) library.
*/


#ifdef USE_SDL


#include "Surface.hpp"
#include "Graphics2D.hpp"
#include "SDLException.hpp"
#include <SDL_image.h>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "Array.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

Surface * Surface::ms_current = 0;

//=============================================================================


Surface::Surface( int width, int height, EPixelType pixelType )
    :   m_own( true ),
        m_pixelType( pixelType )
{
    Uint32 flags = 0;
    if ( Graphics2D::Instance().Screen()->PixelType() == pixelType )
        flags = SDL_HWSURFACE;
    SDL_PixelFormat pxlFmt = DetermineSDLPixelFormat( pixelType );
    m_pSDL_Surface = SDL_CreateRGBSurface( flags, width, height,
                                           pxlFmt.BitsPerPixel,
                                           pxlFmt.Rmask, pxlFmt.Gmask,
                                           pxlFmt.Bmask, pxlFmt.Amask );
    if ( m_pSDL_Surface == 0 )
        throw SDLException( "SDL_CreateRGBSurface" );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Surface::Surface( SDL_Surface * sdl_Surface, bool own )
    :   m_pSDL_Surface( sdl_Surface ),
        m_own( own )
{
    m_pixelType = DeterminePixelType( *(sdl_Surface->format) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Surface::Surface( const std::string & filespec, bool alpha )
{
    m_pSDL_Surface = IMG_Load( filespec.c_str() );
    if ( m_pSDL_Surface == 0 )
        throw SDLException( "IMG_Load" );
    if ( alpha )
    {
        int alphaRslt = SDL_SetAlpha( m_pSDL_Surface, SDL_SRCALPHA, 255 );
        if ( alphaRslt != 0 )
            throw SDLException( "SDL_SetAlpha" );
    }
    else
    {
        int alphaRslt = SDL_SetAlpha( m_pSDL_Surface, 0, 255 );
        if ( alphaRslt != 0 )
            throw SDLException( "SDL_SetAlpha" );
    }
    m_own = true;
    m_pixelType = DeterminePixelType( *(m_pSDL_Surface->format) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Surface::Surface( const std::string & filespec,
                  const Color3B & transparentColor )
{
    m_pSDL_Surface = IMG_Load( filespec.c_str() );
    if ( m_pSDL_Surface == 0 )
        throw SDLException( "IMG_Load" );
    Uint32 colorKey;
    if ( m_pSDL_Surface->format->Amask == 0 )
        colorKey = SDL_MapRGB( m_pSDL_Surface->format,
                               static_cast<Uint8>( transparentColor.Red() ),
                               static_cast<Uint8>( transparentColor.Green() ),
                               static_cast<Uint8>( transparentColor.Blue() ) );
    else
        colorKey = SDL_MapRGBA( m_pSDL_Surface->format,
                                static_cast<Uint8>( transparentColor.Red() ),
                                static_cast<Uint8>( transparentColor.Green() ),
                                static_cast<Uint8>( transparentColor.Blue() ),
                                0 );
    int alphaRslt = SDL_SetAlpha( m_pSDL_Surface, 0, 255 );
    if ( alphaRslt != 0 )
        throw SDLException( "SDL_SetAlpha" );
    int colorKeyRslt = SDL_SetColorKey( m_pSDL_Surface,
                                        SDL_SRCCOLORKEY | SDL_RLEACCEL,
                                        colorKey );
    if ( colorKeyRslt != 0 )
        throw SDLException( "SDL_SetColorKey" );
    m_own = true;
    m_pixelType = DeterminePixelType( *(m_pSDL_Surface->format) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Surface::Surface( const Surface & rhs )
{
    CopySurface( rhs );
}

//-----------------------------------------------------------------------------

Surface::~Surface( )
{
    if ( m_own && (m_pSDL_Surface != 0) )
        SDL_FreeSurface( m_pSDL_Surface );
}

//=============================================================================

Surface & 
Surface::operator=( const Surface & rhs )
{
    if ( &rhs == this )
        return *this;
    if ( m_own && (m_pSDL_Surface != 0) )
        SDL_FreeSurface( m_pSDL_Surface );
    CopySurface( rhs );
    return *this;
}

//-----------------------------------------------------------------------------

void 
Surface::CopySurface( const Surface & rhs )
{
    if ( rhs.m_own )
    {
        int width = rhs.m_pSDL_Surface->w;
        int height = rhs.m_pSDL_Surface->h;
        Uint32 flags = rhs.m_pSDL_Surface->flags;
        SDL_PixelFormat pxlFmt = *(rhs.m_pSDL_Surface->format);

        m_pSDL_Surface = SDL_CreateRGBSurface( flags, width, height, 
                                               pxlFmt.BitsPerPixel,
                                               pxlFmt.Rmask, pxlFmt.Gmask,
                                               pxlFmt.Bmask, pxlFmt.Amask );
        if ( m_pSDL_Surface == 0 )
            throw SDLException( "SDL_CreateRGBSurface" );
        SDL_Rect rect = { 0, 0, static_cast< Uint16 >( width ),
                          static_cast< Uint16 >( height ) };
        int blitRslt = SDL_BlitSurface( rhs.m_pSDL_Surface, &rect,
                                        m_pSDL_Surface, &rect );
        if ( blitRslt != 0 )
            throw SDLException( "SDL_BlitSurface" );
        m_own = true;
    }
    else
    {
        m_pSDL_Surface = rhs.m_pSDL_Surface;
        m_own = false;
    }
    m_pixelType = rhs.m_pixelType;
}

//=============================================================================

void * 
Surface::Lock( )
{
    int lockRslt = SDL_LockSurface( m_pSDL_Surface );
    if ( lockRslt != 0 )
        throw SDLException( "SDL_LockSurface" );
    return  m_pSDL_Surface->pixels;
}

//-----------------------------------------------------------------------------

void 
Surface::Unlock( )
{
    SDL_UnlockSurface( m_pSDL_Surface );
}

//=============================================================================

void 
Surface::Blit( const Point2I & destPos, Surface * pDest )
{
    Blit( Extent(), destPos, pDest );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Surface::Blit( const Rectangle & srcRect, const Point2I & destPos,
               Surface * pDest )
{
    SDL_Rect srcSDLRect = srcRect;
    SDL_Rect destSDLRect = srcSDLRect;
    destSDLRect.x = static_cast< Sint16 >( destPos.X() );
    destSDLRect.y = static_cast< Sint16 >( destPos.Y() );
    int blitRslt = SDL_BlitSurface( m_pSDL_Surface, &srcSDLRect,
                                    pDest->m_pSDL_Surface, &destSDLRect );
    if ( blitRslt != 0 )
        throw SDLException( "SDL_BlitSurface" );
}

//=============================================================================

#ifdef DEBUG

void 
Surface::PrintInfo( ) const
{
    cout << " Width: " << Extent().Width()
         << " Height: " << Extent().Height()
         << " Pitch: " << Pitch()
         << endl;

    Uint32 flags = m_pSDL_Surface->flags;
    cout.setf( ios_base::hex, ios_base::basefield );
    cout << " Flags: " << flags;
    cout.setf( ios_base::dec, ios_base::basefield );
    cout << " ( ";
    struct
    {
        Uint32 flag;
        const char * name;
    }
    flagTable[]
            = {
                { SDL_SWSURFACE, "SDL_SWSURFACE" },
                { SDL_HWSURFACE, "SDL_HWSURFACE" },
                { SDL_ASYNCBLIT, "SDL_ASYNCBLIT" },
                { SDL_ANYFORMAT, "SDL_ANYFORMAT" },
                { SDL_HWPALETTE, "SDL_HWPALETTE" },
                { SDL_DOUBLEBUF, "SDL_DOUBLEBUF" },
                { SDL_FULLSCREEN, "SDL_FULLSCREEN" },
                { SDL_OPENGL, "SDL_OPENGL" },
                { SDL_OPENGLBLIT, "SDL_OPENGLBLIT" },
                { SDL_RESIZABLE, "SDL_RESIZABLE" },
                { SDL_HWACCEL, "SDL_HWACCEL" },
                { SDL_SRCCOLORKEY, "SDL_SRCCOLORKEY" },
                { SDL_RLEACCEL, "SDL_RLEACCEL" },
                { SDL_SRCALPHA, "SDL_SRCALPHA" },
                { SDL_PREALLOC, "SDL_PREALLOC" },
            };
    bool first = true;
    for ( int i = 0; i < ARRAYSIZE( flagTable ); ++i )
        if ( flags & flagTable[i].flag )
        {
            if ( ! first )
                cout << " | ";
            cout << flagTable[i].name;
            first = false;
        }
    cout << " )" << endl;

    PrintPixelFormat( *(m_pSDL_Surface->format) );
}

#endif

//=============================================================================

//=============================================================================


#ifdef DEBUG

namespace 
{

shared_ptr< Surface > s_spSurfacePlain;
shared_ptr< Surface > s_spSurfaceColorKeyed;
shared_ptr< Surface > s_spSurfaceAlpha;

}

//-----------------------------------------------------------------------------

bool 
Surface::TestLoad( const std::string & testFileSpec )
{
    bool ok = true;
    cout << "Surface::TestLoad" << endl;

    try
    {
        cout << "Surface( \"" << testFileSpec << "\" )" << endl;
        s_spSurfacePlain.reset( new Surface( testFileSpec ) );
        cout << "Surface( \"" << testFileSpec
             << "\", Color3B( 0xFF, 0, 0xFF ) )" << endl;
        s_spSurfaceColorKeyed.reset( new Surface( testFileSpec,
                                         Color3B( 0xFF, 0, 0xFF ) ) );
        cout << "Surface( \"" << testFileSpec << "\", true )" << endl;
        s_spSurfaceAlpha.reset( new Surface( testFileSpec, true ) );
    }
    catch ( SDLException except )
    {
        ok = false;
        cout << except.Description( ) << endl;
    }

    if ( ok )
        cout << "Surface::TestLoad PASSED." << endl << endl;
    else
        cout << "Surface::TestLoad FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void 
Surface::TestDraw( )
{
    Rectangle oldClip = Surface::Current()->ClippingRect();

    Rectangle rect( 440, 0, 320, 300 );
    rect.Fill( Color3B( 0, 0, 0xFF ) );

    s_spSurfacePlain->Blit( Point2I( 450, 10 ) );
    s_spSurfaceColorKeyed->Blit( Point2I( 550, 10 ) );
    s_spSurfaceAlpha->Blit( Point2I( 650, 10 ) );
    Surface::Current()->SetClippingRect( Rectangle( 490, 240, 220, 100 ) );
    s_spSurfacePlain->Blit( Rectangle( 10, 10, 80, 80 ), Point2I( 460, 210 ) );
    s_spSurfaceColorKeyed->Blit( Rectangle( 10, 10, 80, 80 ), Point2I( 560, 210 ) );
    s_spSurfaceAlpha->Blit( Rectangle( 10, 10, 80, 80 ), Point2I( 660, 210 ) );

    Surface::Current()->SetClippingRect( oldClip );
}

//-----------------------------------------------------------------------------

void 
Surface::TestUnload( )
{
    s_spSurfacePlain.reset();
    s_spSurfaceColorKeyed.reset();
    s_spSurfaceAlpha.reset();
}

#endif

//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL

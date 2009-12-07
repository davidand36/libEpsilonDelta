/*
  SDLGraphics2D.cpp
  Copyright (C) 2007 David M. Anderson

  Graphics2D class: 2-D graphics system.
  Implementation based on the Simple DirectMedia Layer (SDL) library.
*/


#ifdef USE_SDL


#include "Graphics2D.hpp"
#include "SDL.hpp"
#include "SDLException.hpp"
#include "Pixel.hpp"
#include "Surface.hpp"
#include "Assert.hpp"
#include "Platform.hpp"
#include <SDL.h>
#include <SDL_video.h>
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Graphics2D::Graphics2D( )
{
    SDL::Instance();    //to force construction
}

//-----------------------------------------------------------------------------

Graphics2D::~Graphics2D( )
{
    Shutdown( );
}

//=============================================================================

void 
Graphics2D::Init( )
{
    SDL::Instance().Init( );
    if ( ::SDL_WasInit( SDL_INIT_VIDEO ) == 0 )
    {
        int initRslt = ::SDL_InitSubSystem( SDL_INIT_VIDEO );

        if ( initRslt != 0 )
            throw SDLException( "SDL_InitSubSystem( SDL_INIT_VIDEO )" );
    }
}

//-----------------------------------------------------------------------------

void 
Graphics2D::Shutdown( )
{
    m_spScreenSurface.reset();
    if ( ::SDL_WasInit( SDL_INIT_VIDEO ) != 0 )
        ::SDL_QuitSubSystem( SDL_INIT_VIDEO );
}

//=============================================================================

void 
Graphics2D::SetupScreen( int width, int height,
                               const char * title, 
                               EPixelType pixelType,
                               bool fullScreen, bool openGL )
{
    const ::SDL_VideoInfo * videoInfo = ::SDL_GetVideoInfo( );
    if ( videoInfo == 0 )
        throw SDLException( "SDL_GetVideoInfo" );
    if ( videoInfo->wm_available )
    {
        ::SDL_WM_SetCaption( title, 0 );
        //!!!SDL_WM_SetIcon(...);
    }

    Uint32 flags = 0;
    if ( fullScreen )
        flags |= SDL_FULLSCREEN;
    ::SDL_PixelFormat pxlFmt = DetermineSDLPixelFormat( pixelType );
    if ( openGL )
    {
        flags |= SDL_OPENGL;

        int glRslt = ::SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
        if ( glRslt != 0 )
            throw SDLException( "SDL_GL_SetAttribute(DOUBLEBUFFER)" );
        glRslt = ::SDL_GL_SetAttribute( SDL_GL_RED_SIZE, (8 - pxlFmt.Rloss) );
        if ( glRslt != 0 )
            throw SDLException( "SDL_GL_SetAttribute(RED_SIZE)" );
        glRslt = ::SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, (8 - pxlFmt.Gloss) );
        if ( glRslt != 0 )
            throw SDLException( "SDL_GL_SetAttribute(GREEN_SIZE)" );
        glRslt = ::SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, (8 - pxlFmt.Bloss) );
        if ( glRslt != 0 )
            throw SDLException( "SDL_GL_SetAttribute(BLUE_SIZE)" );
        glRslt = ::SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, (8 - pxlFmt.Aloss) );
        if ( glRslt != 0 )
            throw SDLException( "SDL_GL_SetAttribute(ALPHA_SIZE)" );
        glRslt = ::SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
        if ( glRslt != 0 )
            throw SDLException( "SDL_GL_SetAttribute(DEPTH_SIZE)" );
        //!!!SDL_GL_STENCIL_SIZE
        //!!!SDL_GL_ACCUM_RED_SIZE, ..., SDL_GL_ACCUM_ALPHA_SIZE
    }
    else
    {
        flags |= (SDL_HWSURFACE | SDL_DOUBLEBUF);
    }

    ::SDL_Surface * sdlScreenSurface
            = ::SDL_SetVideoMode( width, height, pxlFmt.BitsPerPixel, flags );
    if ( sdlScreenSurface == 0 )
        throw SDLException( "SDL_SetVideoMode" );

    m_spScreenSurface.reset( new Surface( sdlScreenSurface, false ) );
    m_spScreenSurface->MakeCurrent( );
}

//=============================================================================

void 
Graphics2D::ShowScreen( )
{
    if ( m_spScreenSurface->GetSDL_Surface()->flags & SDL_OPENGL )
        ::SDL_GL_SwapBuffers( );
    else
    {
        int flipRslt = ::SDL_Flip( m_spScreenSurface->GetSDL_Surface() );
        if ( flipRslt != 0 )
            throw SDLException( "SDL_Flip" );
    }
}

//=============================================================================

#ifdef DEBUG

void 
Graphics2D::PrintVideoInfo( )
{
    Assert( ::SDL_WasInit( SDL_INIT_VIDEO ) != 0 );

    char driverName[ 50 ] = "";
    char * name = ::SDL_VideoDriverName( driverName, 50 );
    Assert( name );
    cout << "Video driver: " << driverName << endl;

    bool videoModeSet = (m_spScreenSurface != 0);

    const ::SDL_VideoInfo * videoInfo = ::SDL_GetVideoInfo( );
    cout << "Can create hardware surfaces? "
         << (videoInfo->hw_available  ?  "yes"  :  "no") << endl;
    cout << "Window manager available? "
         << (videoInfo->wm_available  ?  "yes"  :  "no") << endl;
    cout << "HW to HW blits accelerated? "
         << (videoInfo->blit_hw  ?  "yes"  :  "no") << endl;
    cout << "HW to HW colorkey blits accelerated? "
         << (videoInfo->blit_hw_CC  ?  "yes"  :  "no") << endl;
    cout << "HW to HW alpha blits accelerated? "
         << (videoInfo->blit_hw_A  ?  "yes"  :  "no") << endl;
    cout << "SW to HW blits accelerated? "
         << (videoInfo->blit_sw  ?  "yes"  :  "no") << endl;
    cout << "SW to HW colorkey blits accelerated? "
         << (videoInfo->blit_sw_CC  ?  "yes"  :  "no") << endl;
    cout << "SW to HW alpha blits accelerated? "
         << (videoInfo->blit_sw_A  ?  "yes"  :  "no") << endl;
    cout << "Color fills accelerated? "
         << (videoInfo->blit_fill  ?  "yes"  :  "no") << endl;
    cout << "Video memory: " << videoInfo->video_mem << endl;

    if ( videoModeSet )
    {
        char * title;
        char * icon;
        ::SDL_WM_GetCaption( &title, &icon );
        if ( title )
            cout << "Window title: \"" << title << "\"" << endl;
        if ( icon )
            cout << "Window icon: \"" << icon << "\"" << endl;
        cout << "Screen surface:" << endl;
        Screen()->PrintInfo( );
    }
    else
    {
        PrintPixelFormat( *(videoInfo->vfmt) );
        cout << "Available resolutions for this pixel format" << endl;
        Uint32 flags = SDL_FULLSCREEN | SDL_HWSURFACE;
        cout << " (SDL_FULLSCREEN | SDL_HWSURFACE):" << endl;
        PrintAvailableResolutions( *(videoInfo->vfmt), flags );
        flags = SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF;
        cout << " (SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF):" << endl;
        PrintAvailableResolutions( *(videoInfo->vfmt), flags );
        flags = SDL_OPENGL;
        cout << " (SDL_OPENGL)" << endl;
        PrintAvailableResolutions( *(videoInfo->vfmt), flags );

        for ( int i = 0; i < NumPixelTypes; ++i )
        {
            ::SDL_PixelFormat format
                    = DetermineSDLPixelFormat( (EPixelType) i );
            cout << "Available resolutions for "
                 << GetPixelTypeName( (EPixelType) i ) << endl;
            flags = SDL_HWSURFACE;
            cout << " (SDL_HWSURFACE):" << endl;
            PrintAvailableResolutions( format, flags );
            flags = SDL_HWSURFACE | SDL_DOUBLEBUF;
            cout << " (SDL_HWSURFACE | SDL_DOUBLEBUF):" << endl;
            PrintAvailableResolutions( format, flags );
            flags = SDL_FULLSCREEN | SDL_HWSURFACE;
            cout << " (SDL_FULLSCREEN | SDL_HWSURFACE):" << endl;
            PrintAvailableResolutions( format, flags );
            flags = SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF;
            cout << " (SDL_FULLSCREEN | SDL_HWSURFACE | SDL_DOUBLEBUF):"
                 << endl;
            PrintAvailableResolutions( format, flags );
            flags = SDL_OPENGL;
            cout << " (SDL_OPENGL)" << endl;
            PrintAvailableResolutions( format, flags );
        }
    }
}

//.............................................................................

void 
Graphics2D::PrintAvailableResolutions( const ::SDL_PixelFormat & format,
                                             Uint32 flags )
{
    ::SDL_Rect ** resolutions
            = ::SDL_ListModes( &(const_cast< ::SDL_PixelFormat & >( format )),
                             flags );
    if ( resolutions == 0 )
        cout << " None available" << endl;
    else if ( resolutions == reinterpret_cast< ::SDL_Rect ** >( -1 ) )
        cout << " All resolutions available" << endl;
    else
    {
        for ( int i = 0; (resolutions[i] != 0); ++i )
        {
            cout << " " << resolutions[i]->w << "x"
                 << resolutions[i]->h;
            if ( (i % 8) == 7 )
                cout << endl;
        }
        cout << endl;
    }
}

//-----------------------------------------------------------------------------

bool 
Graphics2D::Test( bool testResolutions )
{
    bool ok = true;
    cout << "Testing Graphics2D" << endl;

    try
    {
        cout << "Init()" << endl;
        Graphics2D::Instance().Init( );

        PrintVideoInfo( );
        cout << endl;

        if ( testResolutions )
        {
            cout << "SetupScreen( 640, 480 )" << endl;
            SetupScreen( 640, 480, "Graphics2D Test 640x480" );
            PrintVideoInfo( );
            cout << endl;
            ::SDL_Delay( 1000 );

            cout << "SetupScreen( 320, 240, PixelType0RGB )" << endl;
            SetupScreen( 320, 240, "Graphics2D Test 320x240", PixelType0RGB );
            PrintVideoInfo( );
            cout << endl;
            ::SDL_Delay( 1000 );

            cout << "SetupScreen( 320, 240, PixelTypeRGB0 )" << endl;
            SetupScreen( 320, 240, "Graphics2D Test 320x240", PixelTypeRGB0 );
            PrintVideoInfo( );
            cout << endl;
            ::SDL_Delay( 1000 );

            cout << "SetupScreen( 640, 480, PixelType565 )" << endl;
            SetupScreen( 640, 480, "Graphics2D Test 640x480", PixelType565 );
            PrintVideoInfo( );
            cout << endl;
            ::SDL_Delay( 1000 );

            cout << "Shutdown()" << endl;
            Graphics2D::Instance().Shutdown( );

            cout << "Init()" << endl;
            Graphics2D::Instance().Init( );
            cout << "Init() again" << endl;
            Graphics2D::Instance().Init( );
            cout << endl;

            cout << "SetupScreen( 640, 480, NativePixelType, true )" << endl;
            SetupScreen( 640, 480, "Graphics2D Test 640x480", NativePixelType,
                         true );
            PrintVideoInfo( );
            cout << endl;
            ::SDL_Delay( 1500 );

            cout << "SetupScreen( 640, 480, NativePixelType, true, true )"
                 << endl;
            SetupScreen( 640, 480, "Graphics2D Test 640x480", NativePixelType,
                         true, true );
            PrintVideoInfo( );
            cout << endl;
            ::SDL_Delay( 1500 );

#ifndef OS_WINDOWS
            cout << "SetupScreen( 800, 600, PixelType565, true, true )" << endl;
            SetupScreen( 800, 600, "Graphics2D Test 800x600", PixelType565,
                         true, true );
            PrintVideoInfo( );
            cout << endl;
            ::SDL_Delay( 1500 );
#endif
        }

        cout << "Shutdown()" << endl;
        Graphics2D::Instance().Shutdown( );
        cout << "Shutdown() again" << endl;
        Graphics2D::Instance().Shutdown( );
    }
    catch ( SDLException & except )
    {
        cout << except.Description( ) << endl;
        ok = false;
    }

    if ( ok )
        cout << "Graphics2D PASSED." << endl << endl;
    else
        cout << "Graphics2D FAILED." << endl << endl;
    return ok;
}

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL

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
#if defined(USE_PNG) || defined(USE_JPEG)
#include <cstdio>
#include "File.hpp"
#include "FileException.hpp"
#endif
#ifdef USE_PNG
#include <png.h>
#include <setjmp.h>
#endif
#ifdef DEBUG
#include "TestCheck.hpp"
#include "File.hpp"
#include "Array.hpp"
#include "Circle.hpp"
using namespace std;
using namespace std::tr1;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Surface * Surface::ms_current = 0;

//=============================================================================


Surface::Surface( int width, int height, EPixelType pixelType )
    :   m_pSDL_Surface( 0 ),
        m_own( true ),
        m_pixelType( pixelType )
{
    Uint32 flags = 0;
    if ( Graphics2D::Instance().Screen()
         && Graphics2D::Instance().Screen()->PixelType() == pixelType )
        flags = SDL_HWSURFACE;
    ::SDL_PixelFormat pxlFmt = DetermineSDLPixelFormat( pixelType );
    m_pSDL_Surface = ::SDL_CreateRGBSurface( flags, width, height,
                                             pxlFmt.BitsPerPixel,
                                             pxlFmt.Rmask, pxlFmt.Gmask,
                                             pxlFmt.Bmask, pxlFmt.Amask );
    if ( m_pSDL_Surface == 0 )
        throw SDLException( "SDL_CreateRGBSurface" );
}

//.............................................................................

Surface::Surface( ::SDL_Surface * sdl_Surface, bool own )
    :   m_pSDL_Surface( sdl_Surface ),
        m_own( own )
{
    m_pixelType = DeterminePixelType( *(sdl_Surface->format) );
}

//.............................................................................

Surface::Surface( const DataBuffer & dataBuffer, bool alpha )
{
    ::SDL_RWops * pRWops
            = ::SDL_RWFromMem( const_cast< char * >( dataBuffer.Data() ),
                               dataBuffer.Size() );
    m_pSDL_Surface = ::IMG_Load_RW( pRWops, 0 );
    Init( alpha );
}

//.............................................................................

Surface::Surface( const DataBuffer & dataBuffer,
                  const Color3B & transparentColor )
{
    ::SDL_RWops * pRWops
            = ::SDL_RWFromMem( const_cast< char * >( dataBuffer.Data() ),
                               dataBuffer.Size() );
    m_pSDL_Surface = ::IMG_Load_RW( pRWops, 0 );
    Init( transparentColor );
}

//.............................................................................

Surface::Surface( const std::string & filespec, bool alpha )
{
    m_pSDL_Surface = ::IMG_Load( filespec.c_str() );
    if ( m_pSDL_Surface == 0 )
        throw SDLException( "IMG_Load" );
    Init( alpha );
}

//.............................................................................

Surface::Surface( const std::string & filespec,
                  const Color3B & transparentColor )
{
    m_pSDL_Surface = ::IMG_Load( filespec.c_str() );
    if ( m_pSDL_Surface == 0 )
        throw SDLException( "IMG_Load" );
    Init( transparentColor );
}

//.............................................................................

Surface::Surface( const Surface & rhs )
{
    CopySurface( rhs );
}

//-----------------------------------------------------------------------------

Surface::~Surface( )
{
    if ( m_own && (m_pSDL_Surface != 0) )
        ::SDL_FreeSurface( m_pSDL_Surface );
}

//=============================================================================

void 
Surface::Init( bool alpha )
{
    if ( alpha )
    {
        int alphaRslt = ::SDL_SetAlpha( m_pSDL_Surface, SDL_SRCALPHA, 255 );
        if ( alphaRslt != 0 )
            throw SDLException( "SDL_SetAlpha" );
    }
    else
    {
        int alphaRslt = ::SDL_SetAlpha( m_pSDL_Surface, 0, 255 );
        if ( alphaRslt != 0 )
            throw SDLException( "SDL_SetAlpha" );
    }
    m_own = true;
    m_pixelType = DeterminePixelType( *(m_pSDL_Surface->format) );
}

//-----------------------------------------------------------------------------

void 
Surface::Init( const Color3B & transparentColor )
{
    SetTransparentColor( transparentColor );
    m_own = true;
    m_pixelType = DeterminePixelType( *(m_pSDL_Surface->format) );
}

//*****************************************************************************

Surface & 
Surface::operator=( const Surface & rhs )
{
    if ( &rhs == this )
        return *this;
    if ( m_own && (m_pSDL_Surface != 0) )
        ::SDL_FreeSurface( m_pSDL_Surface );
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
        ::SDL_PixelFormat pxlFmt = *(rhs.m_pSDL_Surface->format);

        m_pSDL_Surface = ::SDL_CreateRGBSurface( flags, width, height, 
                                                 pxlFmt.BitsPerPixel,
                                                 pxlFmt.Rmask, pxlFmt.Gmask,
                                                 pxlFmt.Bmask, pxlFmt.Amask );
        if ( m_pSDL_Surface == 0 )
            throw SDLException( "SDL_CreateRGBSurface" );
        ::SDL_Rect rect = { 0, 0, static_cast< Uint16 >( width ),
                            static_cast< Uint16 >( height ) };
        int blitRslt = ::SDL_BlitSurface( rhs.m_pSDL_Surface, &rect,
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

bool 
Surface::operator==( const Region & rhs ) const
{
    const Surface * pSurface = dynamic_cast< Surface const * >( &rhs );
    return ( pSurface && (*this == *pSurface) );
}

//=============================================================================

void 
Surface::SetTransparentColor( const Color3B & transparentColor )
{
    Uint32 colorKey;
    if ( m_pSDL_Surface->format->Amask == 0 )
        colorKey = ::SDL_MapRGB( m_pSDL_Surface->format,
                                static_cast<Uint8>( transparentColor.Red() ),
                                static_cast<Uint8>( transparentColor.Green() ),
                                static_cast<Uint8>( transparentColor.Blue() ) );
    else
        colorKey = ::SDL_MapRGBA( m_pSDL_Surface->format,
                                 static_cast<Uint8>( transparentColor.Red() ),
                                 static_cast<Uint8>( transparentColor.Green() ),
                                 static_cast<Uint8>( transparentColor.Blue() ),
                                 0 );
    int alphaRslt = ::SDL_SetAlpha( m_pSDL_Surface, 0, 255 );
    if ( alphaRslt != 0 )
        throw SDLException( "SDL_SetAlpha" );
    int colorKeyRslt = ::SDL_SetColorKey( m_pSDL_Surface,
                                        SDL_SRCCOLORKEY | SDL_RLEACCEL,
                                        colorKey );
    if ( colorKeyRslt != 0 )
        throw SDLException( "SDL_SetColorKey" );
}

//-----------------------------------------------------------------------------

bool 
Surface::GetTransparentColor( Color3B * pTransparentColor ) const
{
    if ( m_pSDL_Surface->flags & SDL_SRCCOLORKEY )
    {
        uint32_t colorKey = m_pSDL_Surface->format->colorkey;
        uint8_t r, g, b;
        ::SDL_GetRGB( colorKey, m_pSDL_Surface->format, &r, &g, &b );
        pTransparentColor->Set( r, g, b );
        return true;
    }
    else
        return false;
}

//=============================================================================

bool 
Surface::Contains( const Point2I & point ) const
{
    if ( ! Extent().Contains( point ) )
        return false;
    bool transparent = false;
    if ( m_pSDL_Surface->flags & SDL_SRCCOLORKEY )
    {
        uint32_t colorKey = m_pSDL_Surface->format->colorkey;
        switch ( PixelType() )
        {
        case PixelType8:
        {
            DrawingSurface< Pixel8 > const drawSurf
                    = LockDrawingSurface< Pixel8 >( );
            transparent = (drawSurf.PixelAt( point )->Value()
                           == (uint8_t)colorKey);
            break;
        }
        case PixelType555:
        {
            DrawingSurface< Pixel555 > const drawSurf
                    = LockDrawingSurface< Pixel555 >( );
            transparent = (drawSurf.PixelAt( point )->Value()
                           == (uint16_t)colorKey);
            break;
        }
        case PixelType565:
        {
            DrawingSurface< Pixel565 > const drawSurf
                    = LockDrawingSurface< Pixel565 >( );
            transparent = (drawSurf.PixelAt( point )->Value()
                           == (uint16_t)colorKey);
            break;
        }
        case PixelType888:
        {
            DrawingSurface< Pixel888 > const drawSurf
                    = LockDrawingSurface< Pixel888 >( );
            transparent = (drawSurf.PixelAt( point )->Value() == colorKey);
            break;
        }
        case PixelType888Rev:
        {
            DrawingSurface< Pixel888Rev > const drawSurf
                    = LockDrawingSurface< Pixel888Rev >( );
            transparent = (drawSurf.PixelAt( point )->Value() == colorKey);
            break;
        }
        case PixelType0888:
        {
            DrawingSurface< Pixel0888 > const drawSurf
                    = LockDrawingSurface< Pixel0888 >( );
            transparent = (drawSurf.PixelAt( point )->Value() == colorKey);
            break;
        }
        case PixelType0888Rev:
        {
            DrawingSurface< Pixel0888Rev > const drawSurf
                    = LockDrawingSurface< Pixel0888Rev >( );
            transparent = (drawSurf.PixelAt( point )->Value() == colorKey);
            break;
        }
        case PixelType8888:
        {
            DrawingSurface< Pixel8888 > const drawSurf
                    = LockDrawingSurface< Pixel8888 >( );
            transparent = (drawSurf.PixelAt( point )->Value() == colorKey);
            break;
        }
        case PixelType8888Rev:
        {
            DrawingSurface< Pixel8888Rev > const drawSurf
                    = LockDrawingSurface< Pixel8888Rev >( );
            transparent = (drawSurf.PixelAt( point )->Value() == colorKey);
            break;
        }
        default:
            Assert( 0 && "Unknown pixel type" );
            return false;
        }
        Unlock( );
    }
    else if ( m_pSDL_Surface->flags & SDL_SRCALPHA )
    {
        switch ( PixelType() )
        {
        case PixelType8888:
        {
            DrawingSurface< Pixel8888 > const drawSurf
                    = LockDrawingSurface< Pixel8888 >( );
            transparent = (drawSurf.PixelAt( point )->Alpha() == 0);
            Unlock( );
            break;
        }
        case PixelType8888Rev:
        {
            DrawingSurface< Pixel8888Rev > const drawSurf
                    = LockDrawingSurface< Pixel8888Rev >( );
            bool transparent = (drawSurf.PixelAt( point )->Alpha() == 0);
            Unlock( );
            break;
        }
        default:
            break;
        }
    }
    return ! transparent;
}

//=============================================================================

void * 
Surface::Lock( )
{
    int lockRslt = ::SDL_LockSurface( m_pSDL_Surface );
    if ( lockRslt != 0 )
        throw SDLException( "SDL_LockSurface" );
    return  m_pSDL_Surface->pixels;
}

//.............................................................................

void const * 
Surface::Lock( ) const
{
    int lockRslt = ::SDL_LockSurface( m_pSDL_Surface );
    if ( lockRslt != 0 )
        throw SDLException( "SDL_LockSurface" );
    return  m_pSDL_Surface->pixels;
}

//-----------------------------------------------------------------------------

void 
Surface::Unlock( ) const
{
    ::SDL_UnlockSurface( m_pSDL_Surface );
}

//=============================================================================

void 
Surface::Blit( const Point2I & destPos, Surface * pDest )
{
    Blit( Extent(), destPos, pDest );
}

//.............................................................................

void 
Surface::Blit( const Rectangle & srcRect, const Point2I & destPos,
               Surface * pDest )
{
    ::SDL_Rect srcSDLRect = srcRect;
    ::SDL_Rect destSDLRect = srcSDLRect;
    destSDLRect.x = static_cast< Sint16 >( destPos.X() );
    destSDLRect.y = static_cast< Sint16 >( destPos.Y() );
    int blitRslt = ::SDL_BlitSurface( m_pSDL_Surface, &srcSDLRect,
                                      pDest->m_pSDL_Surface, &destSDLRect );
    if ( blitRslt != 0 )
        throw SDLException( "SDL_BlitSurface" );
}

//=============================================================================

#ifdef USE_PNG

void 
Surface::SavePng( const std::string & fileSpec )
{
    Rectangle extent = Extent();
    int width = extent.Width();
    int height = extent.Height();

    if ( (m_pixelType == PixelType888) || (m_pixelType == PixelType8888) )
    {
        File file( fileSpec );
        bool openRslt = file.Open( File::WriteMode );
        if ( ! openRslt )
            throw FileException( "Unable to open " + fileSpec
                                 + " for writing." );
        ::png_struct * pPng = ::png_create_write_struct( PNG_LIBPNG_VER_STRING,
                                                         0, 0, 0 );
        Assert( pPng != 0 );
        ::png_info * pInfo = ::png_create_info_struct( pPng );
        Assert( pInfo != 0 );
        if ( setjmp( pPng->jmpbuf ) )
        {
            ::png_destroy_write_struct( &pPng, &pInfo );
            throw FileException( "Error writing " + fileSpec + " as PNG." );
        }
        ::png_init_io( pPng, file.Handle() );
        int pitch = Pitch();
        int bytesPerPixel = (m_pixelType == PixelType888)  ?  3  :  4;
        pitch *= bytesPerPixel;
        int bitDepth = 8;
        int colorType = (m_pixelType == PixelType888)
                ?  PNG_COLOR_TYPE_RGB  :  PNG_COLOR_TYPE_RGB_ALPHA;
        int interlace = PNG_INTERLACE_ADAM7; //or PNG_INTERLACE_NONE
        ::png_set_IHDR( pPng, pInfo, width, height, bitDepth, colorType,
                        interlace, PNG_COMPRESSION_TYPE_DEFAULT,
                        PNG_FILTER_TYPE_DEFAULT );
        ::png_byte * rows[ height ];
        ::png_byte * pixels = (::png_byte *)Lock( );
        for ( int i = 0; i < height; ++i )
        {
            rows[ i ] = pixels;
            pixels += pitch;
        }
        ::png_set_rows( pPng, pInfo, rows );
        int transforms = (m_pixelType == PixelType888)
                ?  PNG_TRANSFORM_IDENTITY  :  PNG_TRANSFORM_SWAP_ALPHA;
        ::png_write_png( pPng, pInfo, transforms, 0 );
        Unlock( );
        ::png_destroy_write_struct( &pPng, &pInfo );
    }
    else if ( m_pixelType == PixelType8888Rev )
    {
        Surface surf( width, height, PixelType8888 );
        Blit( Point2I::Zero, &surf );
        surf.SavePng( fileSpec );
    }
    else
    {
        Surface surf( width, height, PixelType888 );
        Blit( Point2I::Zero, &surf );
        surf.SavePng( fileSpec );
    }
}

#endif //USE_PNG

//=============================================================================

#ifdef USE_JPEG

void 
Surface::SaveJpeg( const std::string & fileSpec )
{
    //!!!
}

#endif //USE_JPEG

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
    for ( int i = 0; i < ARRAY_LENGTH( flagTable ); ++i )
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


#ifdef DEBUG

namespace 
{

shared_ptr< Surface > s_spSurfacePlain;
shared_ptr< Surface > s_spSurfaceColorKeyed;
shared_ptr< Surface > s_spSurfaceAlpha;

}

//-----------------------------------------------------------------------------

bool
Surface::Test( )
{
    bool ok = true;
    cout << "Testing Surface" << endl;

    cout << "Surface( 100, 75, PixelType565 )" << endl;
    shared_ptr< Surface > pSurf565( new Surface( 100, 75, PixelType565 ) );
    cout << "Extent()" << endl;
    Rectangle ext = pSurf565->Extent();
    TESTCHECK( ext.Left(), 0, &ok );
    TESTCHECK( ext.Top(), 0, &ok );
    TESTCHECK( ext.Width(), 100, &ok );
    TESTCHECK( ext.Height(), 75, &ok );
    Color3B transparent( 255, 0, 132 );
    cout << "SetTransparentColor()" << endl;
    pSurf565->SetTransparentColor( transparent );
    TESTCHECK( pSurf565->GetTransparentColor( &transparent ), true, &ok );
    TESTCHECK( transparent.Red(), 255, &ok );
    TESTCHECK( transparent.Green(), 0, &ok );
    TESTCHECK( transparent.Blue(), 132, &ok );
    cout << "Fill( Rectangle, Color3B )" << endl;
    pSurf565->Fill( ext, transparent );
    Color3B color( 0, 0, 255 );
    Circle circle( Point2I( 37, 37 ), 37 );
    cout << "circle.Fill( Color3B )" << endl;
    circle.Fill( color, pSurf565.get() );
    TESTCHECK( pSurf565->Contains( Point2I( -1, -1 ) ), false, &ok );
    TESTCHECK( pSurf565->Contains( Point2I( 50, 100 ) ), false, &ok );
    TESTCHECK( pSurf565->Contains( Point2I( 37, 37 ) ), true, &ok );
    TESTCHECK( pSurf565->Contains( Point2I( 20, 60 ) ), true, &ok );
    TESTCHECK( pSurf565->Contains( Point2I( 8, 8 ) ), false, &ok );
    TESTCHECK( pSurf565->Contains( Point2I( 80, 37 ) ), false, &ok );

    cout << "Surface( 50, 75, PixelType888 )" << endl;
    shared_ptr< Surface > pSurf888( new Surface( 100, 75, PixelType888 ) );
    cout << "Extent()" << endl;
    ext = pSurf888->Extent();
    TESTCHECK( ext.Left(), 0, &ok );
    TESTCHECK( ext.Top(), 0, &ok );
    TESTCHECK( ext.Width(), 100, &ok );
    TESTCHECK( ext.Height(), 75, &ok );
    cout << "SetTransparentColor()" << endl;
    pSurf888->SetTransparentColor( transparent );
    TESTCHECK( pSurf888->GetTransparentColor( &transparent ), true, &ok );
    TESTCHECK( transparent.Red(), 255, &ok );
    TESTCHECK( transparent.Green(), 0, &ok );
    TESTCHECK( transparent.Blue(), 132, &ok );
    cout << "Fill( Rectangle, Color3B )" << endl;
    pSurf888->Fill( ext, transparent );
    cout << "circle.Fill( Color3B )" << endl;
    circle.Fill( color, pSurf888.get() );
    TESTCHECK( pSurf888->Contains( Point2I( -1, -1 ) ), false, &ok );
    TESTCHECK( pSurf888->Contains( Point2I( 50, 100 ) ), false, &ok );
    TESTCHECK( pSurf888->Contains( Point2I( 37, 37 ) ), true, &ok );
    TESTCHECK( pSurf888->Contains( Point2I( 20, 60 ) ), true, &ok );
    TESTCHECK( pSurf888->Contains( Point2I( 8, 8 ) ), false, &ok );
    TESTCHECK( pSurf888->Contains( Point2I( 80, 37 ) ), false, &ok );

    cout << "Surface( 75, 100, PixelType8888 )" << endl;
    shared_ptr< Surface > pSurf8888( new Surface( 75, 100, PixelType8888 ) );
    cout << "Extent()" << endl;
    ext = pSurf8888->Extent();
    TESTCHECK( ext.Left(), 0, &ok );
    TESTCHECK( ext.Top(), 0, &ok );
    TESTCHECK( ext.Width(), 75, &ok );
    TESTCHECK( ext.Height(), 100, &ok );
    cout << "Fill( Rectangle, Color4B )" << endl;
    pSurf8888->Draw( ext, Color4B( 255, 0, 0, 0 ) );
    cout << "circle.Fill( Color3B )" << endl;
    circle.Fill( color, pSurf8888.get() );
    TESTCHECK( pSurf8888->Contains( Point2I( -1, -1 ) ), false, &ok );
    TESTCHECK( pSurf8888->Contains( Point2I( 100, 50 ) ), false, &ok );
    TESTCHECK( pSurf8888->Contains( Point2I( 37, 37 ) ), true, &ok );
    TESTCHECK( pSurf8888->Contains( Point2I( 20, 60 ) ), true, &ok );
    TESTCHECK( pSurf8888->Contains( Point2I( 8, 8 ) ), false, &ok );
    TESTCHECK( pSurf8888->Contains( Point2I( 37, 80 ) ), false, &ok );

#ifdef USE_PNG
    cout << "Saving Test565.png" << endl;
    pSurf565->SavePng( "Test565.png" );
    cout << "Saving Test888.png" << endl;
    pSurf888->SavePng( "Test888.png" );
    cout << "Saving Test8888.png" << endl;
    pSurf8888->SavePng( "Test8888.png" );
#endif

    if ( ok )
        cout << "Surface PASSED." << endl << endl;
    else
        cout << "Surface FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

bool 
Surface::TestLoad( const std::string & testFileSpec )
{
    bool ok = true;
    cout << "Surface::TestLoad" << endl;

    try
    {
        DataBuffer dataBuff;
        if ( ! File::Load( testFileSpec, &dataBuff ) )
            throw Exception( "File::Load() failed" );
        cout << "Surface( DataBuffer )" << endl;
        s_spSurfacePlain.reset( new Surface( dataBuff ) );
        cout << "Surface( \"" << testFileSpec << "\" )" << endl;
        s_spSurfacePlain.reset( new Surface( testFileSpec ) );
        cout << "Surface( \"" << testFileSpec
             << "\", Color3B( 0xFF, 0, 0xFF ) )" << endl;
        s_spSurfaceColorKeyed.reset( new Surface( testFileSpec,
                                         Color3B( 0xFF, 0, 0xFF ) ) );
        cout << "Surface( \"" << testFileSpec << "\", true )" << endl;
        s_spSurfaceAlpha.reset( new Surface( testFileSpec, true ) );
        cout << "Surface( DataBuffer, Color3B )" << endl;
        s_spSurfaceColorKeyed.reset( new Surface( dataBuff,
                                                  Color3B( 0xFF, 0, 0xFF ) ) );
    }
    catch ( Exception except )
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

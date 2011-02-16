/*
  SDLImage.cpp
  Copyright (C) 2007 David M. Anderson

  Image class: Represents a 2-D graphical image, optimized for blitting.
  Implemented using the Simple DirectMedia Layer (SDL) library.
*/


#ifdef USE_SDL

#include "Image.hpp"
#include "SDLException.hpp"
#include <SDL_image.h>
#ifdef DEBUG
#include "TestCheck.hpp"
#include "FileReader.hpp"
#include <tr1/memory>
using namespace std;
using namespace std::tr1;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Image::Image( const DataBuffer & dataBuffer, bool alpha )
{
    ::SDL_RWops * pRWops
            = ::SDL_RWFromMem( const_cast< char * >( dataBuffer.Data() ),
                               dataBuffer.Size() );
    ::SDL_Surface * pSDL_Surface = ::IMG_Load_RW( pRWops, 0 );
    Init( pSDL_Surface, alpha );
}

//.............................................................................

Image::Image( const DataBuffer & dataBuffer, const Color3B & transparentColor )
{
    ::SDL_RWops * pRWops
            = ::SDL_RWFromMem( const_cast< char * >( dataBuffer.Data() ),
                               dataBuffer.Size() );
    ::SDL_Surface * pSDL_Surface = ::IMG_Load_RW( pRWops, 0 );
    Init( pSDL_Surface, transparentColor );
}

//.............................................................................

Image::Image( const std::string & filespec, bool alpha )
{
    ::SDL_Surface * pSDL_Surface = ::IMG_Load( filespec.c_str() );
    if ( pSDL_Surface == 0 )
        throw SDLException( "IMG_Load" );
    Init( pSDL_Surface, alpha );
}

//.............................................................................

Image::Image( const std::string & filespec, const Color3B & transparentColor )
{
    ::SDL_Surface * pSDL_Surface = ::IMG_Load( filespec.c_str() );
    if ( pSDL_Surface == 0 )
        throw SDLException( "IMG_Load" );
    Init( pSDL_Surface, transparentColor );
}

//-----------------------------------------------------------------------------

void 
Image::Init( ::SDL_Surface * pSDL_Surface, bool alpha )
{
    if ( alpha )
    {
        int alphaRslt = ::SDL_SetAlpha( pSDL_Surface, SDL_SRCALPHA, 255 );
        if ( alphaRslt != 0 )
            throw SDLException( "SDL_SetAlpha" );
        if ( ::SDL_GetVideoSurface() != 0 )
        {
            m_pSDL_Surface = ::SDL_DisplayFormatAlpha( pSDL_Surface );
            if ( m_pSDL_Surface == 0 )
                throw SDLException( "SDL_DisplayFormatAlpha" );
            ::SDL_FreeSurface( pSDL_Surface );
        }
        else
            m_pSDL_Surface = pSDL_Surface;
    }
    else
    {
        int alphaRslt = ::SDL_SetAlpha( pSDL_Surface, 0, 255 );
        if ( alphaRslt != 0 )
            throw SDLException( "SDL_SetAlpha" );
        if ( ::SDL_GetVideoSurface() != 0 )
        {
            m_pSDL_Surface = ::SDL_DisplayFormat( pSDL_Surface );
            if ( m_pSDL_Surface == 0 )
                throw SDLException( "SDL_DisplayFormat" );
            ::SDL_FreeSurface( pSDL_Surface );
        }
        else
            m_pSDL_Surface = pSDL_Surface;
    }
    m_own = true;
    m_pixelType = DeterminePixelType( *(m_pSDL_Surface->format) );
}

//.............................................................................

void 
Image::Init( ::SDL_Surface * pSDL_Surface, const Color3B & transparentColor )
{
    Uint32 colorKey;
    if ( pSDL_Surface->format->Amask == 0 )
        colorKey = ::SDL_MapRGB( pSDL_Surface->format,
                                static_cast<Uint8>( transparentColor.Red() ),
                                static_cast<Uint8>( transparentColor.Green() ),
                                static_cast<Uint8>( transparentColor.Blue() ) );
    else
        colorKey = ::SDL_MapRGBA( pSDL_Surface->format,
                                 static_cast<Uint8>( transparentColor.Red() ),
                                 static_cast<Uint8>( transparentColor.Green() ),
                                 static_cast<Uint8>( transparentColor.Blue() ),
                                 0 );
    int alphaRslt = ::SDL_SetAlpha( pSDL_Surface, 0, 255 );
    if ( alphaRslt != 0 )
        throw SDLException( "SDL_SetAlpha" );
    int colorKeyRslt = ::SDL_SetColorKey( pSDL_Surface,
                                          SDL_SRCCOLORKEY | SDL_RLEACCEL,
                                          colorKey );
    if ( colorKeyRslt != 0 )
        throw SDLException( "SDL_SetColorKey" );

    if ( ::SDL_GetVideoSurface() != 0 )
    {
        m_pSDL_Surface = ::SDL_DisplayFormat( pSDL_Surface );
        if ( m_pSDL_Surface == 0 )
            throw SDLException( "SDL_DisplayFormat" );
        ::SDL_FreeSurface( pSDL_Surface );
    }
    else
        m_pSDL_Surface = pSDL_Surface;
    m_own = true;
    m_pixelType = DeterminePixelType( *(m_pSDL_Surface->format) );
}

//=============================================================================


#ifdef DEBUG

namespace 
{

shared_ptr< Image > s_spImagePlain;
shared_ptr< Image > s_spImageColorKeyed;
shared_ptr< Image > s_spImageAlpha;

}

//-----------------------------------------------------------------------------

bool 
Image::TestLoad( const std::string & testFileSpec )
{
    bool ok = true;
    cout << "Image::TestLoad" << endl;

    try
    {
        cout << "Image( \"" << testFileSpec << "\" )" << endl;
        s_spImagePlain.reset( new Image( testFileSpec ) );
        DataBuffer dataBuff;
        FileReader file( testFileSpec );
        file.Load( &dataBuff );
        cout << "Image( DataBuffer, Color3B )" << endl;
        s_spImageColorKeyed.reset( new Image( dataBuff, 
                                              Color3B( 0xFF, 0, 0xFF ) ) );
        cout << "Image( \"" << testFileSpec << "\", Color3B( 0xFF, 0, 0xFF ) )"
             << endl;
        s_spImageColorKeyed.reset( new Image( testFileSpec,
                                     Color3B( 0xFF, 0, 0xFF ) ) );
        cout << "Image( \"" << testFileSpec << "\", true )" << endl;
        s_spImageAlpha.reset( new Image( testFileSpec, true ) );
        cout << "Image( DataBuffer, true )" << endl;
        s_spImageAlpha.reset( new Image( dataBuff, true ) );
    }
    catch ( Exception except )
    {
        ok = false;
        cout << except.Description( ) << endl;
    }

    if ( ok )
        cout << "Image::TestLoad PASSED." << endl << endl;
    else
        cout << "Image::TestLoad FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void 
Image::TestDraw( )
{
    Rectangle oldClip = Surface::Current()->ClippingRect();

    Rectangle rect( 440, 300, 320, 300 );
    rect.Fill( Color3B( 0, 0xFF, 0 ) );

    s_spImagePlain->Blit( Point2I( 450, 310 ) );
    s_spImageColorKeyed->Blit( Point2I( 550, 310 ) );
    s_spImageAlpha->Blit( Point2I( 650, 310 ) );
    Surface::Current()->SetClippingRect( Rectangle( 490, 540, 220, 100 ) );
    s_spImagePlain->Blit( Rectangle( 10, 10, 80, 80 ), Point2I( 460, 510 ) );
    s_spImageColorKeyed->Blit( Rectangle( 10, 10, 80, 80 ),
                                Point2I( 560, 510 ) );
    s_spImageAlpha->Blit( Rectangle( 10, 10, 80, 80 ), Point2I( 660, 510 ) );

    Surface::Current()->SetClippingRect( oldClip );
}

//-----------------------------------------------------------------------------

void 
Image::TestUnload( )
{
    s_spImagePlain.reset();
    s_spImageColorKeyed.reset();
    s_spImageAlpha.reset();
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif  //USE_SDL

/*
  Pixel.cpp
  Copyright (C) 2007 David M. Anderson

  Pixel classes.
*/


#include "Pixel.hpp"
#include "Assert.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

#ifdef DEBUG

static const char * const s_pixelTypeNames[ NumPixelTypes ]
    = {
        "Pixel8",
        "Pixel555",
        "Pixel565",
        "Pixel0888",
        "Pixel8888",
        "Pixel8888Rev"
    };

//.............................................................................

const char * 
GetPixelTypeName( EPixelType pixelType )
{
    Assert( pixelType < NumPixelTypes );
    return  s_pixelTypeNames[ pixelType ];
}

#endif

//=============================================================================


#ifdef USE_SDL


SDL_PixelFormat 
DetermineSDLPixelFormat( EPixelType pixelType )
{
    SDL_PixelFormat format;
    switch ( pixelType )
    {
    case PixelType8:
        format.palette = 0; //?
        format.BitsPerPixel = 8;
        format.BytesPerPixel = 1;
        format.Rmask = format.Gmask = format.Bmask = format.Amask
               = format.Rshift = format.Gshift = format.Bshift = format.Ashift
               = format.Rloss = format.Gloss = format.Bloss = format.Aloss = 0;
        format.colorkey = 0;
        format.alpha = 255;
        break;
    case PixelType555:
        format.palette = 0;
        format.BitsPerPixel = 16;
        format.BytesPerPixel = 2;
#if RGB_REDHIGH
        format.Rmask = 0xF800;
        format.Gmask = 0x07C0;
        format.Bmask = 0x003E;
        format.Amask = 0;
        format.Rshift = 11;
        format.Gshift = 6;
        format.Bshift = 1;
        format.Ashift = 0;
#else   //RGB_REDLOW
        format.Rmask = 0x001F;
        format.Gmask = 0x03E0;
        format.Bmask = 0x7C00;
        format.Amask = 0;
        format.Rshift = 0;
        format.Gshift = 5;
        format.Bshift = 10;
        format.Ashift = 0;
#endif  //RGB
        format.Rloss = 3;
        format.Gloss = 3;
        format.Bloss = 3;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;
    case PixelType565:
        format.palette = 0;
        format.BitsPerPixel = 16;
        format.BytesPerPixel = 2;
#if RGB_REDHIGH
        format.Rmask = 0xF800;
        format.Gmask = 0x07E0;
        format.Bmask = 0x001F;
        format.Amask = 0;
        format.Rshift = 11;
        format.Gshift = 5;
        format.Bshift = 0;
        format.Ashift = 0;
#else   //RGB_REDLOW
        format.Rmask = 0x001F;
        format.Gmask = 0x07E0;
        format.Bmask = 0xF800;
        format.Amask = 0;
        format.Rshift = 0;
        format.Gshift = 5;
        format.Bshift = 11;
        format.Ashift = 0;
#endif  //RGB
        format.Rloss = 3;
        format.Gloss = 2;
        format.Bloss = 3;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;
    case PixelType0888:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#if RGB_REDHIGH
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 0;
#else   //RGB_REDLOW
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 0;
#endif  //RGB
        format.Rloss = format.Gloss = format.Bloss = 0;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;
    case PixelType8888:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#if RGB_REDHIGH
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0xFF000000;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 24;
#else   //RGB_REDLOW
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0xFF000000;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 24;
#endif  //RGB
        format.Rloss = format.Gloss = format.Bloss = format.Aloss = 0;
        format.colorkey = 0;
        format.alpha = 255;
        break;
    case PixelType8888Rev:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#if RGB_REDHIGH
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0xFF000000;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 24;
#else   //RGB_REDLOW
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0xFF000000;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 24;
#endif  //RGB
        format.Rloss = format.Gloss = format.Bloss = format.Aloss = 0;
        format.colorkey = 0;
        format.alpha = 255;
        break;
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
    return format;
}

//-----------------------------------------------------------------------------

EPixelType 
DeterminePixelType( const SDL_PixelFormat & pixelFormat )
{
    if ( pixelFormat.BitsPerPixel == 32 )
    {
#if RGB_REDHIGH
        if ( pixelFormat.Amask == 0 )
        {
            Assert( pixelFormat.Rmask == 0x00FF0000 );
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x000000FF );
            Assert( pixelFormat.Amask == 0 );
            return PixelType0888;
        }
        else if ( pixelFormat.Rmask == 0x00FF0000 )
        {
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x000000FF );
            Assert( pixelFormat.Amask == 0xFF000000 );
            return PixelType8888;
        }
        else
        {
            Assert( pixelFormat.Rmask == 0x000000FF );
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x00FF0000 );
            Assert( pixelFormat.Amask == 0xFF000000 );
            return PixelType8888Rev;
        }
#else   //RGB_REDLOW
        if ( pixelFormat.Amask == 0 )
        {
            Assert( pixelFormat.Rmask == 0x000000FF );
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x00FF0000 );
            Assert( pixelFormat.Amask == 0 );
            return PixelType0888;
        }
        else
        {
            Assert( pixelFormat.Rmask == 0x000000FF );
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x00FF0000 );
            Assert( pixelFormat.Amask == 0xFF000000 );
            return PixelType8888;
        }
#endif  //RGB
    }
    else if ( pixelFormat.BitsPerPixel == 16 )
    {
#if RGB_REDHIGH
        if ( pixelFormat.Gmask == 0x07E0 )
        {
            Assert( pixelFormat.Rmask == 0xF800 );
            Assert( pixelFormat.Bmask == 0x001F );
            Assert( pixelFormat.Amask == 0 );
            return PixelType565;
        }
        else
        {
            Assert( pixelFormat.Rmask == 0xF800 );
            Assert( pixelFormat.Gmask == 0x07C0 );
            Assert( pixelFormat.Bmask == 0x003E );
            Assert( pixelFormat.Amask == 0 );
            return PixelType555;
        }
#else   //RGB_REDLOW
        if ( pixelFormat.Gmask == 0x07E0 )
        {
            Assert( pixelFormat.Rmask == 0x001F );
            Assert( pixelFormat.Bmask == 0xF800 );
            Assert( pixelFormat.Amask == 0 );
            return PixelType565;
        }
        else
        {
            Assert( pixelFormat.Rmask == 0x001F );
            Assert( pixelFormat.Gmask == 0x03E0 );
            Assert( pixelFormat.Bmask == 0x7C00 );
            Assert( pixelFormat.Amask == 0 );
            return PixelType555;
        }
#endif  //RGB
    }
    else if ( pixelFormat.BitsPerPixel == 8 )
    {
        return PixelType8;
    }
    else
    {
        Assert( 0 && "Unexpected pixel format" );
#ifndef DEBUG
        return UnknownPixelType;
#endif
    }
}

//=============================================================================

#ifdef DEBUG

void 
PrintPixelFormat( const SDL_PixelFormat & pixelFormat )
{
    cout << " BitsPerPixel: " << static_cast<int>( pixelFormat.BitsPerPixel )
         << " BytesPerPixel: " << static_cast<int>( pixelFormat.BytesPerPixel )
         << endl;
    cout.setf( ios_base::hex, ios_base::basefield );
    cout << " Rmask: " << pixelFormat.Rmask
         << " Gmask: " << pixelFormat.Gmask
         << " Bmask: " << pixelFormat.Bmask
         << " Amask: " << pixelFormat.Amask
         << endl;
    cout.setf( ios_base::dec, ios_base::basefield );
    cout << " Rshift: " << static_cast<int>( pixelFormat.Rshift )
         << " Gshift: " << static_cast<int>( pixelFormat.Gshift )
         << " Bshift: " << static_cast<int>( pixelFormat.Bshift )
         << " Ashift: " << static_cast<int>( pixelFormat.Ashift )
         << endl;
    cout << " Rloss: " << static_cast<int>( pixelFormat.Rloss )
         << " Gloss: " << static_cast<int>( pixelFormat.Gloss )
         << " Bloss: " << static_cast<int>( pixelFormat.Bloss )
         << " Aloss: " << static_cast<int>( pixelFormat.Aloss )
         << endl;
    cout << " colorkey: " << pixelFormat.colorkey
         << " alpha: " << static_cast<int>( pixelFormat.alpha )
         << endl;
#ifdef ASSERT_IS_EXCEPTION
    try
    {
        EPixelType pixelType = DeterminePixelType( pixelFormat );
        cout << " PixelType: " << GetPixelTypeName( pixelType ) << endl;
    }
    catch ( AssertException )
    {
        cout << " PixelType: Undefined" << endl;
    }
#else
    EPixelType pixelType = DeterminePixelType( pixelFormat );
    cout << " PixelType: " << GetPixelTypeName( pixelType ) << endl;
#endif
}

#endif  //DEBUG

#endif  //USE_SDL


//=============================================================================


#ifdef DEBUG

bool 
TestPixel( )
{
    bool ok = true;
    cout << "Testing Pixel" << endl;

    uint8_t u8 = 0xDA;
    TESTCHECK( sizeof( Pixel8 ), (size_t)1, &ok );
    cout << "Pixel8( " << u8 << " )" << endl;
    Pixel8 pxl8( u8 );
    TESTCHECK( pxl8.Value(), u8, &ok );

    uint16_t u16 = 0x1234;
    int r = 255;
    int g = 127;
    int b = 10;
    TESTCHECK( sizeof( Pixel555 ), (size_t)2, &ok );
    cout << "Pixel555( " << u16 << " )" << endl;
    Pixel555 pxl555v( u16 );
    TESTCHECK( pxl555v.Value(), u16, &ok );
    cout << "Pixel555 pxl555c( Color3B( " << r << ", " << g << ", " << b <<
            " ) )" << endl;
    Pixel555 pxl555c( Color3B( r, g, b ) );
    Color3B c555 = pxl555c.Color();
    TESTCHECK( c555.Red(), 255, &ok );
    TESTCHECK( c555.Green(), 123, &ok );
    TESTCHECK( c555.Blue(), 8, &ok );

    TESTCHECK( sizeof( Pixel565 ), (size_t)2, &ok );
    cout << "Pixel565( " << u16 << " )" << endl;
    Pixel565 pxl565v( u16 );
    TESTCHECK( pxl565v.Value(), u16, &ok );
    cout << "Pixel565 pxl565c( Color3B( " << r << ", " << g << ", " << b <<
            " ) )" << endl;
    Pixel565 pxl565c( Color3B( r, g, b ) );
    Color3B c565 = pxl565c.Color();
    TESTCHECK( c565.Red(), 255, &ok );
    TESTCHECK( c565.Green(), 125, &ok );
    TESTCHECK( c565.Blue(), 8, &ok );

    uint32_t u32 = 0x12345678;
    TESTCHECK( sizeof( Pixel0888 ), (size_t)4, &ok );
    cout << "Pixel0888( " << u32 << " )" << endl;
    Pixel0888 pxl0888v( u32 );
    TESTCHECK( pxl0888v.Value(), u32, &ok );
    cout << "Pixel0888 pxl0888c( Color3B( " << r << ", " << g << ", " << b <<
            " ) )" << endl;
    Pixel0888 pxl0888c3( Color3B( r, g, b ) );
    Color3B c0888 = pxl0888c3.Color();
    TESTCHECK( c0888.Red(), r, &ok );
    TESTCHECK( c0888.Green(), g, &ok );
    TESTCHECK( c0888.Blue(), b, &ok );

    TESTCHECK( sizeof( Pixel8888 ), (size_t)4, &ok );
    cout << "Pixel8888( " << u32 << " )" << endl;
    Pixel8888 pxl8888v( u32 );
    TESTCHECK( pxl8888v.Value(), u32, &ok );
    cout << "Pixel8888 pxl8888c( Color3B( " << r << ", " << g << ", " << b <<
            " ) )" << endl;
    Pixel8888 pxl8888c3( Color3B( r, g, b ) );
    Color3B c8888 = pxl8888c3.Color();
    TESTCHECK( c8888.Red(), r, &ok );
    TESTCHECK( c8888.Green(), g, &ok );
    TESTCHECK( c8888.Blue(), b, &ok );
    TESTCHECK( pxl8888c3.Alpha(), 255, &ok );
    int a = 127;
    cout << "Pixel8888 pxl8888c( Color4B( " << r << ", " << g << ", " << b <<
            ", " << a << " ) )" << endl;
    Pixel8888 pxl8888c4( Color4B( r, g, b, a ) );
    c8888 = pxl8888c4.Color();
    TESTCHECK( c8888.Red(), r, &ok );
    TESTCHECK( c8888.Green(), g, &ok );
    TESTCHECK( c8888.Blue(), b, &ok );
    TESTCHECK( pxl8888c4.Alpha(), a, &ok );

    TESTCHECK( sizeof( Pixel8888Rev ), (size_t)4, &ok );
    cout << "Pixel8888Rev( " << u32 << " )" << endl;
    Pixel8888Rev pxl8888rv( u32 );
    TESTCHECK( pxl8888rv.Value(), u32, &ok );
    cout << "Pixel8888Rev pxl8888c( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    Pixel8888Rev pxl8888rc3( Color3B( r, g, b ) );
    Color3B c8888r = pxl8888rc3.Color();
    TESTCHECK( c8888r.Red(), r, &ok );
    TESTCHECK( c8888r.Green(), g, &ok );
    TESTCHECK( c8888r.Blue(), b, &ok );
    TESTCHECK( pxl8888rc3.Alpha(), 255, &ok );
    a = 127;
    cout << "Pixel8888Rev pxl8888c( Color4B( " << r << ", " << g << ", " << b
         << ", " << a << " ) )" << endl;
    Pixel8888Rev pxl8888rc4( Color4B( r, g, b, a ) );
    c8888r = pxl8888rc4.Color();
    TESTCHECK( c8888r.Red(), r, &ok );
    TESTCHECK( c8888r.Green(), g, &ok );
    TESTCHECK( c8888r.Blue(), b, &ok );
    TESTCHECK( pxl8888rc4.Alpha(), a, &ok );

#ifdef USE_SDL

    cout << "DetermineSDLPixelFormat( PixelType8 )" << endl;
    SDL_PixelFormat pxlFmt8 = DetermineSDLPixelFormat( PixelType8 );
    TESTCHECK( DeterminePixelType( pxlFmt8 ), PixelType8, &ok );
    cout << "DetermineSDLPixelFormat( PixelType555 )" << endl;
    SDL_PixelFormat pxlFmt555 = DetermineSDLPixelFormat( PixelType555 );
    TESTCHECK( DeterminePixelType( pxlFmt555 ), PixelType555, &ok );
    cout << "DetermineSDLPixelFormat( PixelType565 )" << endl;
    SDL_PixelFormat pxlFmt565 = DetermineSDLPixelFormat( PixelType565 );
    TESTCHECK( DeterminePixelType( pxlFmt565 ), PixelType565, &ok );
    cout << "DetermineSDLPixelFormat( PixelType0888 )" << endl;
    SDL_PixelFormat pxlFmt0888 = DetermineSDLPixelFormat( PixelType0888 );
    TESTCHECK( DeterminePixelType( pxlFmt0888 ), PixelType0888, &ok );
    cout << "DetermineSDLPixelFormat( PixelType8888 )" << endl;
    SDL_PixelFormat pxlFmt8888 = DetermineSDLPixelFormat( PixelType8888 );
    TESTCHECK( DeterminePixelType( pxlFmt8888 ), PixelType8888, &ok );
    cout << "DetermineSDLPixelFormat( PixelType8888Rev )" << endl;
    SDL_PixelFormat pxlFmt8888Rev = DetermineSDLPixelFormat( PixelType8888Rev );
    TESTCHECK( DeterminePixelType( pxlFmt8888Rev ), PixelType8888Rev, &ok );

    Uint8 r8 = static_cast< Uint8 >( r );
    Uint8 g8 = static_cast< Uint8 >( g );
    Uint8 b8 = static_cast< Uint8 >( b );
    Uint8 a8 = static_cast< Uint8 >( a );
    TESTCHECK( SDL_MapRGB( &pxlFmt555, r8, g8, b8 ),
               static_cast< Uint32 >( pxl555c.Value() ), &ok );
    TESTCHECK( SDL_MapRGB( &pxlFmt565, r8, g8, b8 ),
               static_cast< Uint32 >( pxl565c.Value() ), &ok );
    TESTCHECK( SDL_MapRGB( &pxlFmt0888, r8, g8, b8 ), pxl0888c3.Value(), &ok );
    TESTCHECK( SDL_MapRGB( &pxlFmt8888, r8, g8, b8 ), pxl8888c3.Value(), &ok );
    TESTCHECK( SDL_MapRGBA( &pxlFmt8888, r8, g8, b8, a8 ), pxl8888c4.Value(),
               &ok );
    TESTCHECK( SDL_MapRGBA( &pxlFmt8888Rev, r8, g8, b8, a8 ),
               pxl8888rc4.Value(), &ok );

    cout << "SDL_GetRGB( " << u16 << ", pxlFmt555, ... )" << endl;
    SDL_GetRGBA( u16, &pxlFmt555, &r8, &g8, &b8, &a8 );
    r = r8; g = g8; b = b8; a = a8;
    TESTCHECK( pxl555v.Color().Red(), r, &ok );
    TESTCHECK( pxl555v.Color().Green(), g, &ok );
    TESTCHECK( pxl555v.Color().Blue(), b, &ok );
    TESTCHECK( a, 255, &ok );
    cout << "SDL_GetRGB( " << u16 << ", pxlFmt565, ... )" << endl;
    SDL_GetRGBA( u16, &pxlFmt565, &r8, &g8, &b8, &a8 );
    r = r8; g = g8; b = b8; a = a8;
    TESTCHECK( pxl565v.Color().Red(), r, &ok );
    TESTCHECK( pxl565v.Color().Green(), g, &ok );
    TESTCHECK( pxl565v.Color().Blue(), b, &ok );
    TESTCHECK( a, 255, &ok );
    cout << "SDL_GetRGB( " << u32 << ", pxlFmt0888, ... )" << endl;
    SDL_GetRGB( u32, &pxlFmt0888, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxl0888v.Color().Red(), r, &ok );
    TESTCHECK( pxl0888v.Color().Green(), g, &ok );
    TESTCHECK( pxl0888v.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmt8888, ... )" << endl;
    SDL_GetRGBA( u32, &pxlFmt8888, &r8, &g8, &b8, &a8 );
    r = r8; g = g8; b = b8; a = a8;
    TESTCHECK( pxl8888v.Color().Red(), r, &ok );
    TESTCHECK( pxl8888v.Color().Green(), g, &ok );
    TESTCHECK( pxl8888v.Color().Blue(), b, &ok );
    TESTCHECK( pxl8888v.Alpha(), a, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmt8888Rev, ... )" << endl;
    SDL_GetRGBA( u32, &pxlFmt8888Rev, &r8, &g8, &b8, &a8 );
    r = r8; g = g8; b = b8; a = a8;
    TESTCHECK( pxl8888rv.Color().Red(), r, &ok );
    TESTCHECK( pxl8888rv.Color().Green(), g, &ok );
    TESTCHECK( pxl8888rv.Color().Blue(), b, &ok );
    TESTCHECK( pxl8888rv.Alpha(), a, &ok );
    
#endif

    if ( ok )
        cout << "Pixel PASSED." << endl << endl;
    else
        cout << "Pixel FAILED." << endl << endl;
    return ok;
}

#endif  //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

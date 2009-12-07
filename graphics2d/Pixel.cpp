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
        "Pixel1555",
        "Pixel565",
        "PixelRGB",
        "PixelBGR",
        "Pixel0RGB",
        "Pixel0BGR",
        "PixelRGB0",
        "PixelBGR0",
        "PixelARGB",
        "PixelABGR",
        "PixelRGBA",
        "PixelBGRA"
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


::SDL_PixelFormat 
DetermineSDLPixelFormat( EPixelType pixelType )
{
    ::SDL_PixelFormat format;
    switch ( pixelType )
    {
    case PixelType8:
        format.palette = 0;
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
        format.Rmask = 0x7C00;
        format.Gmask = 0x03E0;
        format.Bmask = 0x001F;
        format.Amask = 0;
        format.Rshift = 10;
        format.Gshift = 5;
        format.Bshift = 0;
        format.Ashift = 0;
        format.Rloss = 3;
        format.Gloss = 3;
        format.Bloss = 3;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelType1555:
        format.palette = 0;
        format.BitsPerPixel = 16;
        format.BytesPerPixel = 2;
        format.Rmask = 0x7C00;
        format.Gmask = 0x03E0;
        format.Bmask = 0x001F;
        format.Amask = 0x8000;
        format.Rshift = 10;
        format.Gshift = 5;
        format.Bshift = 0;
        format.Ashift = 15;
        format.Rloss = 3;
        format.Gloss = 3;
        format.Bloss = 3;
        format.Aloss = 7;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelType565:
        format.palette = 0;
        format.BitsPerPixel = 16;
        format.BytesPerPixel = 2;
        format.Rmask = 0xF800;
        format.Gmask = 0x07E0;
        format.Bmask = 0x001F;
        format.Amask = 0;
        format.Rshift = 11;
        format.Gshift = 5;
        format.Bshift = 0;
        format.Ashift = 0;
        format.Rloss = 3;
        format.Gloss = 2;
        format.Bloss = 3;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeRGB:
        format.palette = 0;
        format.BitsPerPixel = 24;
        format.BytesPerPixel = 3;
#ifdef BIG_ENDIAN
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 0;
#else
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 0;
#endif
        format.Rloss = format.Gloss = format.Bloss = 0;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeBGR:
        format.palette = 0;
        format.BitsPerPixel = 24;
        format.BytesPerPixel = 3;
#ifdef BIG_ENDIAN
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 0;
#else
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 0;
#endif
        format.Rloss = format.Gloss = format.Bloss = 0;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelType0RGB:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 0;
#else
        format.Rmask = 0x0000FF00;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0xFF000000;
        format.Amask = 0;
        format.Rshift = 8;
        format.Gshift = 16;
        format.Bshift = 24;
        format.Ashift = 0;
#endif
        format.Rloss = format.Gloss = format.Bloss = 0;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelType0BGR:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 0;
#else
        format.Rmask = 0xFF000000;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0x0000FF00;
        format.Amask = 0;
        format.Rshift = 24;
        format.Gshift = 16;
        format.Bshift = 8;
        format.Ashift = 0;
#endif  //RGB
        format.Rloss = format.Gloss = format.Bloss = 0;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeRGB0:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0xFF000000;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0x0000FF00;
        format.Amask = 0;
        format.Rshift = 24;
        format.Gshift = 16;
        format.Bshift = 8;
        format.Ashift = 0;
#else
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 0;
#endif
        format.Rloss = format.Gloss = format.Bloss = 0;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeBGR0:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0x0000FF00;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0xFF000000;
        format.Amask = 0;
        format.Rshift = 8;
        format.Gshift = 16;
        format.Bshift = 24;
        format.Ashift = 0;
#else
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 0;
#endif  //RGB
        format.Rloss = format.Gloss = format.Bloss = 0;
        format.Aloss = 8;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeARGB:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0xFF000000;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 24;
#else
        format.Rmask = 0x0000FF00;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0xFF000000;
        format.Amask = 0x000000FF;
        format.Rshift = 8;
        format.Gshift = 16;
        format.Bshift = 24;
        format.Ashift = 0;
#endif
        format.Rloss = format.Gloss = format.Bloss = format.Aloss = 0;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeABGR:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0xFF000000;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 24;
#else
        format.Rmask = 0xFF000000;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0x0000FF00;
        format.Amask = 0x000000FF;
        format.Rshift = 24;
        format.Gshift = 16;
        format.Bshift = 8;
        format.Ashift = 0;
#endif
        format.Rloss = format.Gloss = format.Bloss = format.Aloss = 0;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeRGBA:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0xFF000000;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0x0000FF00;
        format.Amask = 0x000000FF;
        format.Rshift = 24;
        format.Gshift = 16;
        format.Bshift = 8;
        format.Ashift = 0;
#else
        format.Rmask = 0x000000FF;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x00FF0000;
        format.Amask = 0xFF000000;
        format.Rshift = 0;
        format.Gshift = 8;
        format.Bshift = 16;
        format.Ashift = 24;
#endif
        format.Rloss = format.Gloss = format.Bloss = format.Aloss = 0;
        format.colorkey = 0;
        format.alpha = 255;
        break;

    case PixelTypeBGRA:
        format.palette = 0;
        format.BitsPerPixel = 32;
        format.BytesPerPixel = 4;
#ifdef BIG_ENDIAN
        format.Rmask = 0x0000FF00;
        format.Gmask = 0x00FF0000;
        format.Bmask = 0xFF000000;
        format.Amask = 0x000000FF;
        format.Rshift = 8;
        format.Gshift = 16;
        format.Bshift = 24;
        format.Ashift = 0;
#else
        format.Rmask = 0x00FF0000;
        format.Gmask = 0x0000FF00;
        format.Bmask = 0x000000FF;
        format.Amask = 0xFF000000;
        format.Rshift = 16;
        format.Gshift = 8;
        format.Bshift = 0;
        format.Ashift = 24;
#endif
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
DeterminePixelType( const ::SDL_PixelFormat & pixelFormat )
{
    if ( pixelFormat.BitsPerPixel == 32 )
    {
#ifdef BIG_ENDIAN
        if ( pixelFormat.Amask == 0 )
        {
            if ( pixelFormat.Rmask == 0x00FF0000 )
            {
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x000000FF );
                Assert( pixelFormat.Amask == 0 );
                return PixelType0RGB;
            }
            else if ( pixelFormat.Rmask == 0x000000FF )
            {
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x00FF0000 );
                Assert( pixelFormat.Amask == 0 );
                return PixelType0BGR;
            }
            else if ( pixelFormat.Rmask == 0xFF000000 )
            {
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0x0000FF00 );
                Assert( pixelFormat.Amask == 0 );
                return PixelTypeRGB0;
            }
            else
            {
                Assert( pixelFormat.Rmask == 0x0000FF00 );
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0xFF000000 );
                return PixelTypeBGR0;
            }
        }
        else if ( pixelFormat.Amask == 0xFF000000 )
        {
            if ( pixelFormat.Rmask == 0x00FF0000 )
            {
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x000000FF );
                return PixelTypeARGB;
            }
            else
            {
                Assert( pixelFormat.Rmask == 0x000000FF );
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x00FF0000 );
                return PixelTypeABGR;
            }
        }
        else
        {
            Assert( pixelFormat.Amask == 0x000000FF );
            if ( pixelFormat.Rmask == 0xFF000000 )
            {
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0x0000FF00 );
                return PixelTypeRGBA;
            }
            else
            {
                Assert( pixelFormat.Rmask == 0x0000FF00 );
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0xFF000000 );
                return PixelTypeBGRA;
            }
        }
#else   //LITTLE_ENDIAN
        if ( pixelFormat.Amask == 0 )
        {
            if ( pixelFormat.Rmask == 0x0000FF00 )
            {
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0xFF000000 );
                return PixelType0RGB;
            }
            else if ( pixelFormat.Rmask == 0xFF000000 )
            {
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0x0000FF00 );
                return PixelType0BGR;
            }
            else if ( pixelFormat.Rmask == 0x000000FF )
            {
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x00FF0000 );
                return PixelTypeRGB0;
            }
            else
            {
                Assert( pixelFormat.Rmask == 0x00FF0000 );
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x000000FF );
                return PixelTypeBGR0;
            }
        }
        else if ( pixelFormat.Amask == 0x000000FF )
        {
            if ( pixelFormat.Rmask == 0x0000FF00 )
            {
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0xFF000000 );
                return PixelTypeARGB;
            }
            else
            {
                Assert( pixelFormat.Rmask == 0xFF000000 );
                Assert( pixelFormat.Gmask == 0x00FF0000 );
                Assert( pixelFormat.Bmask == 0x0000FF00 );
                return PixelTypeABGR;
            }
        }
        else
        {
            Assert( pixelFormat.Amask == 0xFF000000 );
            if ( pixelFormat.Rmask == 0x000000FF )
            {
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x00FF0000 );
                return PixelTypeRGBA;
            }
            else
            {
                Assert( pixelFormat.Rmask == 0x00FF0000 );
                Assert( pixelFormat.Gmask == 0x0000FF00 );
                Assert( pixelFormat.Bmask == 0x000000FF );
                return PixelTypeBGRA;
            }
        }
#endif
    }
    else if ( pixelFormat.BitsPerPixel == 24 )
    {
#ifdef BIG_ENDIAN
        if ( pixelFormat.Rmask == 0x00FF0000 )
        {
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x000000FF );
            Assert( pixelFormat.Amask == 0 );
            return PixelTypeRGB;
        }
        else
        {
            Assert( pixelFormat.Rmask == 0x000000FF );
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x00FF0000 );
            Assert( pixelFormat.Amask == 0 );
            return PixelTypeBGR;
        }
#else
        if ( pixelFormat.Rmask == 0x000000FF )
        {
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x00FF0000 );
            Assert( pixelFormat.Amask == 0 );
            return PixelTypeRGB;
        }
        else
        {
            Assert( pixelFormat.Rmask == 0x00FF0000 );
            Assert( pixelFormat.Gmask == 0x0000FF00 );
            Assert( pixelFormat.Bmask == 0x000000FF );
            Assert( pixelFormat.Amask == 0 );
            return PixelTypeBGR;
        }
#endif
    }
    else if ( pixelFormat.BitsPerPixel == 16 )
    {
        if ( pixelFormat.Gmask == 0x07E0 )
        {
            Assert( pixelFormat.Rmask == 0xF800 );
            Assert( pixelFormat.Bmask == 0x001F );
            Assert( pixelFormat.Amask == 0 );
            return PixelType565;
        }
        else
        {
            Assert( pixelFormat.Rmask == 0x7C00 );
            Assert( pixelFormat.Gmask == 0x03E0 );
            Assert( pixelFormat.Bmask == 0x001F );
            if ( pixelFormat.Amask == 0 )
                return PixelType555;
            else
            {
                Assert( pixelFormat.Amask == 0x8000 );
                return PixelType1555;
            }
        }
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
PrintPixelFormat( const ::SDL_PixelFormat & pixelFormat )
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

    TESTCHECK( sizeof( Pixel8 ), (size_t)1, &ok );
    uint8_t u8 = 0xDA;
    cout << "Pixel8( " << (int)u8 << " )" << endl;
    Pixel8 pxl8( u8 );
    TESTCHECK( (int)pxl8.Value(), (int)u8, &ok );

    TESTCHECK( sizeof( Pixel555 ), (size_t)2, &ok );
    int r = 255;
    int g = 127;
    int b = 10;
    cout << "Pixel555 pxl555c( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    Pixel555 pxl555c( Color3B( r, g, b ) );
    cout << "Color()" << endl;
    Color3B c555 = pxl555c.Color();
    TESTCHECK( c555.Red(), 248, &ok );
    TESTCHECK( c555.Green(), 120, &ok );
    TESTCHECK( c555.Blue(), 8, &ok );
    cout << "Color( true )" << endl;
    c555 = pxl555c.Color( true );
    TESTCHECK( c555.Red(), 255, &ok );
    TESTCHECK( c555.Green(), 123, &ok );
    TESTCHECK( c555.Blue(), 8, &ok );
    uint16_t u16 = 0x1234;
    cout << "Pixel555( " << u16 << " )" << endl;
    Pixel555 pxl555v( u16 );
    TESTCHECK( pxl555v.Value(), u16, &ok );

    TESTCHECK( sizeof( Pixel1555 ), (size_t)2, &ok );
    cout << "Pixel1555 pxl1555c0( Color4B( " << r << ", " << g << ", " << b
         << ", " << 0 << " ) )" << endl;
    Pixel1555 pxl1555c0( Color4B( r, g, b, 0 ) );
    cout << "Color()" << endl;
    Color4B c1555 = pxl1555c0.Color();
    TESTCHECK( c1555.Red(), 248, &ok );
    TESTCHECK( c1555.Green(), 120, &ok );
    TESTCHECK( c1555.Blue(), 8, &ok );
    TESTCHECK( c1555.Alpha(), 0, &ok );
    cout << "Color( true )" << endl;
    c1555 = pxl1555c0.Color( true );
    TESTCHECK( c1555.Red(), 255, &ok );
    TESTCHECK( c1555.Green(), 123, &ok );
    TESTCHECK( c1555.Blue(), 8, &ok );
    TESTCHECK( c1555.Alpha(), 0, &ok );
    cout << "Pixel1555 pxl1555c1( Color4B( " << r << ", " << g << ", " << b
         << ", " << 1 << " ) )" << endl;
    Pixel1555 pxl1555c1( Color4B( r, g, b, 1 ) );
    cout << "Color()" << endl;
    c1555 = pxl1555c1.Color();
    TESTCHECK( c1555.Red(), 248, &ok );
    TESTCHECK( c1555.Green(), 120, &ok );
    TESTCHECK( c1555.Blue(), 8, &ok );
    TESTCHECK( c1555.Alpha(), 255, &ok );
    cout << "Color( true )" << endl;
    c1555 = pxl1555c1.Color( true );
    TESTCHECK( c1555.Red(), 255, &ok );
    TESTCHECK( c1555.Green(), 123, &ok );
    TESTCHECK( c1555.Blue(), 8, &ok );
    TESTCHECK( c1555.Alpha(), 255, &ok );
    cout << "Pixel1555( " << u16 << " )" << endl;
    Pixel555 pxl1555v( u16 );
    TESTCHECK( pxl1555v.Value(), u16, &ok );

    TESTCHECK( sizeof( Pixel565 ), (size_t)2, &ok );
    cout << "Pixel565 pxl565c( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    Pixel565 pxl565c( Color3B( r, g, b ) );
    cout << "Color()" << endl;
    Color3B c565 = pxl565c.Color();
    TESTCHECK( c565.Red(), 248, &ok );
    TESTCHECK( c565.Green(), 124, &ok );
    TESTCHECK( c565.Blue(), 8, &ok );
    cout << "Color( true )" << endl;
    c565 = pxl565c.Color( true );
    TESTCHECK( c565.Red(), 255, &ok );
    TESTCHECK( c565.Green(), 125, &ok );
    TESTCHECK( c565.Blue(), 8, &ok );
    cout << "Pixel565( " << u16 << " )" << endl;
    Pixel565 pxl565v( u16 );
    TESTCHECK( pxl565v.Value(), u16, &ok );

    TESTCHECK( sizeof( PixelRGB ), (size_t)3, &ok );
    cout << "PixelRGB pxlRGBc( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelRGB pxlRGBc( Color3B( r, g, b ) );
    Color3B cRGB = pxlRGBc.Color();
    TESTCHECK( cRGB.Red(), r, &ok );
    TESTCHECK( cRGB.Green(), g, &ok );
    TESTCHECK( cRGB.Blue(), b, &ok );
    TESTCHECK( pxlRGBc.Bytes()[ 0 ], r, &ok );
    TESTCHECK( pxlRGBc.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlRGBc.Bytes()[ 2 ], b, &ok );
    uint32_t u32 = 0x12345678;
    cout << "PixelRGB( " << u32 << " )" << endl;
    PixelRGB pxlRGBv( u32 );
    TESTCHECK( pxlRGBv.Value(), (u32 & 0xFFFFFF), &ok );

    TESTCHECK( sizeof( PixelBGR ), (size_t)3, &ok );
    cout << "PixelRGB pxlBGRc( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelBGR pxlBGRc( Color3B( r, g, b ) );
    Color3B cBGR = pxlBGRc.Color();
    TESTCHECK( cBGR.Red(), r, &ok );
    TESTCHECK( cBGR.Green(), g, &ok );
    TESTCHECK( cBGR.Blue(), b, &ok );
    TESTCHECK( pxlBGRc.Bytes()[ 0 ], b, &ok );
    TESTCHECK( pxlBGRc.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlBGRc.Bytes()[ 2 ], r, &ok );
    cout << "PixelBGR( " << u32 << " )" << endl;
    PixelBGR pxlBGRv( u32 );
    TESTCHECK( pxlBGRv.Value(), (u32 & 0xFFFFFF), &ok );

    TESTCHECK( sizeof( Pixel0RGB ), (size_t)4, &ok );
    cout << "Pixel0RGB pxl0RGBc( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    Pixel0RGB pxl0RGBc( Color3B( r, g, b ) );
    Color3B c0RGB = pxl0RGBc.Color();
    TESTCHECK( c0RGB.Red(), r, &ok );
    TESTCHECK( c0RGB.Green(), g, &ok );
    TESTCHECK( c0RGB.Blue(), b, &ok );
    TESTCHECK( pxl0RGBc.Bytes()[ 0 ], 0, &ok );
    TESTCHECK( pxl0RGBc.Bytes()[ 1 ], r, &ok );
    TESTCHECK( pxl0RGBc.Bytes()[ 2 ], g, &ok );
    TESTCHECK( pxl0RGBc.Bytes()[ 3 ], b, &ok );
    cout << "Pixel0RGB( " << u32 << " )" << endl;
    Pixel0RGB pxl0RGBv( u32 );
    TESTCHECK( pxl0RGBv.Value(), u32, &ok );

    TESTCHECK( sizeof( Pixel0BGR ), (size_t)4, &ok );
    cout << "Pixel0BGR pxl0BGRc( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    Pixel0BGR pxl0BGRc( Color3B( r, g, b ) );
    Color3B c0BGR = pxl0BGRc.Color();
    TESTCHECK( c0BGR.Red(), r, &ok );
    TESTCHECK( c0BGR.Green(), g, &ok );
    TESTCHECK( c0BGR.Blue(), b, &ok );
    TESTCHECK( pxl0BGRc.Bytes()[ 0 ], 0, &ok );
    TESTCHECK( pxl0BGRc.Bytes()[ 1 ], b, &ok );
    TESTCHECK( pxl0BGRc.Bytes()[ 2 ], g, &ok );
    TESTCHECK( pxl0BGRc.Bytes()[ 3 ], r, &ok );
    cout << "Pixel0BGR( " << u32 << " )" << endl;
    Pixel0BGR pxl0BGRv( u32 );
    TESTCHECK( pxl0BGRv.Value(), u32, &ok );

    TESTCHECK( sizeof( PixelRGB0 ), (size_t)4, &ok );
    cout << "PixelRGB0 pxlRGB0c( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelRGB0 pxlRGB0c( Color3B( r, g, b ) );
    Color3B cRGB0 = pxlRGB0c.Color();
    TESTCHECK( cRGB0.Red(), r, &ok );
    TESTCHECK( cRGB0.Green(), g, &ok );
    TESTCHECK( cRGB0.Blue(), b, &ok );
    TESTCHECK( pxlRGB0c.Bytes()[ 0 ], r, &ok );
    TESTCHECK( pxlRGB0c.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlRGB0c.Bytes()[ 2 ], b, &ok );
    TESTCHECK( pxlRGB0c.Bytes()[ 3 ], 0, &ok );
    cout << "PixelRGB0( " << u32 << " )" << endl;
    PixelRGB0 pxlRGB0v( u32 );
    TESTCHECK( pxlRGB0v.Value(), u32, &ok );

    TESTCHECK( sizeof( PixelBGR0 ), (size_t)4, &ok );
    cout << "PixelBGR0 pxlBGR0c( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelBGR0 pxlBGR0c( Color3B( r, g, b ) );
    Color3B cBGR0 = pxlBGR0c.Color();
    TESTCHECK( cBGR0.Red(), r, &ok );
    TESTCHECK( cBGR0.Green(), g, &ok );
    TESTCHECK( cBGR0.Blue(), b, &ok );
    TESTCHECK( pxlBGR0c.Bytes()[ 0 ], b, &ok );
    TESTCHECK( pxlBGR0c.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlBGR0c.Bytes()[ 2 ], r, &ok );
    TESTCHECK( pxlBGR0c.Bytes()[ 3 ], 0, &ok );
    cout << "PixelBGR0( " << u32 << " )" << endl;
    PixelBGR0 pxlBGR0v( u32 );
    TESTCHECK( pxlBGR0v.Value(), u32, &ok );

    TESTCHECK( sizeof( PixelARGB ), (size_t)4, &ok );
    cout << "PixelARGB pxlARGBc3( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelARGB pxlARGBc3( Color3B( r, g, b ) );
    Color4B cARGB = pxlARGBc3.Color();
    TESTCHECK( cARGB.Red(), r, &ok );
    TESTCHECK( cARGB.Green(), g, &ok );
    TESTCHECK( cARGB.Blue(), b, &ok );
    TESTCHECK( cARGB.Alpha(), 255, &ok );
    TESTCHECK( pxlARGBc3.Bytes()[ 0 ], 255, &ok );
    TESTCHECK( pxlARGBc3.Bytes()[ 1 ], r, &ok );
    TESTCHECK( pxlARGBc3.Bytes()[ 2 ], g, &ok );
    TESTCHECK( pxlARGBc3.Bytes()[ 3 ], b, &ok );
    int a = 127;
    cout << "PixelARGB pxlARGBc4( Color4B( " << r << ", " << g << ", " << b
         << ", " << a << " ) )" << endl;
    PixelARGB pxlARGBc4( Color4B( r, g, b, a ) );
    cARGB = pxlARGBc4.Color();
    TESTCHECK( cARGB.Red(), r, &ok );
    TESTCHECK( cARGB.Green(), g, &ok );
    TESTCHECK( cARGB.Blue(), b, &ok );
    TESTCHECK( cARGB.Alpha(), a, &ok );
    TESTCHECK( pxlARGBc4.Bytes()[ 0 ], a, &ok );
    TESTCHECK( pxlARGBc4.Bytes()[ 1 ], r, &ok );
    TESTCHECK( pxlARGBc4.Bytes()[ 2 ], g, &ok );
    TESTCHECK( pxlARGBc4.Bytes()[ 3 ], b, &ok );
    cout << "PixelARGB( " << u32 << " )" << endl;
    PixelARGB pxlARGBv( u32 );
    TESTCHECK( pxlARGBv.Value(), u32, &ok );

    TESTCHECK( sizeof( PixelABGR ), (size_t)4, &ok );
    cout << "PixelABGR pxlABGRc3( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelABGR pxlABGRc3( Color3B( r, g, b ) );
    Color4B cABGR = pxlABGRc3.Color();
    TESTCHECK( cABGR.Red(), r, &ok );
    TESTCHECK( cABGR.Green(), g, &ok );
    TESTCHECK( cABGR.Blue(), b, &ok );
    TESTCHECK( cABGR.Alpha(), 255, &ok );
    TESTCHECK( pxlABGRc3.Bytes()[ 0 ], 255, &ok );
    TESTCHECK( pxlABGRc3.Bytes()[ 1 ], b, &ok );
    TESTCHECK( pxlABGRc3.Bytes()[ 2 ], g, &ok );
    TESTCHECK( pxlABGRc3.Bytes()[ 3 ], r, &ok );
    cout << "PixelABGR pxlABGRc4( Color4B( " << r << ", " << g << ", " << b
         << ", " << a << " ) )" << endl;
    PixelABGR pxlABGRc4( Color4B( r, g, b, a ) );
    cABGR = pxlABGRc4.Color();
    TESTCHECK( cABGR.Red(), r, &ok );
    TESTCHECK( cABGR.Green(), g, &ok );
    TESTCHECK( cABGR.Blue(), b, &ok );
    TESTCHECK( cABGR.Alpha(), a, &ok );
    TESTCHECK( pxlABGRc4.Bytes()[ 0 ], a, &ok );
    TESTCHECK( pxlABGRc4.Bytes()[ 1 ], b, &ok );
    TESTCHECK( pxlABGRc4.Bytes()[ 2 ], g, &ok );
    TESTCHECK( pxlABGRc4.Bytes()[ 3 ], r, &ok );
    cout << "PixelABGR( " << u32 << " )" << endl;
    PixelABGR pxlABGRv( u32 );
    TESTCHECK( pxlABGRv.Value(), u32, &ok );

    TESTCHECK( sizeof( PixelRGBA ), (size_t)4, &ok );
    cout << "PixelRGBA pxlRGBAc3( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelRGBA pxlRGBAc3( Color3B( r, g, b ) );
    Color4B cRGBA = pxlRGBAc3.Color();
    TESTCHECK( cRGBA.Red(), r, &ok );
    TESTCHECK( cRGBA.Green(), g, &ok );
    TESTCHECK( cRGBA.Blue(), b, &ok );
    TESTCHECK( cRGBA.Alpha(), 255, &ok );
    TESTCHECK( pxlRGBAc3.Bytes()[ 0 ], r, &ok );
    TESTCHECK( pxlRGBAc3.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlRGBAc3.Bytes()[ 2 ], b, &ok );
    TESTCHECK( pxlRGBAc3.Bytes()[ 3 ], 255, &ok );
    cout << "PixelRGBA pxlRGBAc4( Color4B( " << r << ", " << g << ", " << b
         << ", " << a << " ) )" << endl;
    PixelRGBA pxlRGBAc4( Color4B( r, g, b, a ) );
    cRGBA = pxlRGBAc4.Color();
    TESTCHECK( cRGBA.Red(), r, &ok );
    TESTCHECK( cRGBA.Green(), g, &ok );
    TESTCHECK( cRGBA.Blue(), b, &ok );
    TESTCHECK( cRGBA.Alpha(), a, &ok );
    TESTCHECK( pxlRGBAc4.Bytes()[ 0 ], r, &ok );
    TESTCHECK( pxlRGBAc4.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlRGBAc4.Bytes()[ 2 ], b, &ok );
    TESTCHECK( pxlRGBAc4.Bytes()[ 3 ], a, &ok );
    cout << "PixelRGBA( " << u32 << " )" << endl;
    PixelRGBA pxlRGBAv( u32 );
    TESTCHECK( pxlRGBAv.Value(), u32, &ok );

    TESTCHECK( sizeof( PixelBGRA ), (size_t)4, &ok );
    cout << "PixelBGRA pxlBGRAc3( Color3B( " << r << ", " << g << ", " << b
         << " ) )" << endl;
    PixelBGRA pxlBGRAc3( Color3B( r, g, b ) );
    Color4B cBGRA = pxlBGRAc3.Color();
    TESTCHECK( cBGRA.Red(), r, &ok );
    TESTCHECK( cBGRA.Green(), g, &ok );
    TESTCHECK( cBGRA.Blue(), b, &ok );
    TESTCHECK( cBGRA.Alpha(), 255, &ok );
    TESTCHECK( pxlBGRAc3.Bytes()[ 0 ], b, &ok );
    TESTCHECK( pxlBGRAc3.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlBGRAc3.Bytes()[ 2 ], r, &ok );
    TESTCHECK( pxlBGRAc3.Bytes()[ 3 ], 255, &ok );
    cout << "PixelBGRA pxlBGRAc4( Color4B( " << r << ", " << g << ", " << b
         << ", " << a << " ) )" << endl;
    PixelBGRA pxlBGRAc4( Color4B( r, g, b, a ) );
    cBGRA = pxlBGRAc4.Color();
    TESTCHECK( cBGRA.Red(), r, &ok );
    TESTCHECK( cBGRA.Green(), g, &ok );
    TESTCHECK( cBGRA.Blue(), b, &ok );
    TESTCHECK( cBGRA.Alpha(), a, &ok );
    TESTCHECK( pxlBGRAc4.Bytes()[ 0 ], b, &ok );
    TESTCHECK( pxlBGRAc4.Bytes()[ 1 ], g, &ok );
    TESTCHECK( pxlBGRAc4.Bytes()[ 2 ], r, &ok );
    TESTCHECK( pxlBGRAc4.Bytes()[ 3 ], a, &ok );
    cout << "PixelBGRA( " << u32 << " )" << endl;
    PixelBGRA pxlBGRAv( u32 );
    TESTCHECK( pxlBGRAv.Value(), u32, &ok );

#ifdef USE_SDL

    //!!!
    cout << "DetermineSDLPixelFormat( PixelType8 )" << endl;
    ::SDL_PixelFormat pxlFmt8 = DetermineSDLPixelFormat( PixelType8 );
    TESTCHECK( DeterminePixelType( pxlFmt8 ), PixelType8, &ok );
    cout << "DetermineSDLPixelFormat( PixelType555 )" << endl;
    ::SDL_PixelFormat pxlFmt555 = DetermineSDLPixelFormat( PixelType555 );
    TESTCHECK( DeterminePixelType( pxlFmt555 ), PixelType555, &ok );
    cout << "DetermineSDLPixelFormat( PixelType1555 )" << endl;
    ::SDL_PixelFormat pxlFmt1555 = DetermineSDLPixelFormat( PixelType1555 );
    TESTCHECK( DeterminePixelType( pxlFmt1555 ), PixelType1555, &ok );
    cout << "DetermineSDLPixelFormat( PixelType565 )" << endl;
    ::SDL_PixelFormat pxlFmt565 = DetermineSDLPixelFormat( PixelType565 );
    TESTCHECK( DeterminePixelType( pxlFmt565 ), PixelType565, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeRGB )" << endl;
    ::SDL_PixelFormat pxlFmtRGB = DetermineSDLPixelFormat( PixelTypeRGB );
    TESTCHECK( DeterminePixelType( pxlFmtRGB ), PixelTypeRGB, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeBGR )" << endl;
    ::SDL_PixelFormat pxlFmtBGR = DetermineSDLPixelFormat( PixelTypeBGR );
    TESTCHECK( DeterminePixelType( pxlFmtBGR ), PixelTypeBGR, &ok );
    cout << "DetermineSDLPixelFormat( PixelType0RGB )" << endl;
    ::SDL_PixelFormat pxlFmt0RGB = DetermineSDLPixelFormat( PixelType0RGB );
    TESTCHECK( DeterminePixelType( pxlFmt0RGB ), PixelType0RGB, &ok );
    cout << "DetermineSDLPixelFormat( PixelType0BGR )" << endl;
    ::SDL_PixelFormat pxlFmt0BGR = DetermineSDLPixelFormat( PixelType0BGR );
    TESTCHECK( DeterminePixelType( pxlFmt0BGR ), PixelType0BGR, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeRGB0 )" << endl;
    ::SDL_PixelFormat pxlFmtRGB0 = DetermineSDLPixelFormat( PixelTypeRGB0 );
    TESTCHECK( DeterminePixelType( pxlFmtRGB0 ), PixelTypeRGB0, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeBGR0 )" << endl;
    ::SDL_PixelFormat pxlFmtBGR0 = DetermineSDLPixelFormat( PixelTypeBGR0 );
    TESTCHECK( DeterminePixelType( pxlFmtBGR0 ), PixelTypeBGR0, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeARGB )" << endl;
    ::SDL_PixelFormat pxlFmtARGB = DetermineSDLPixelFormat( PixelTypeARGB );
    TESTCHECK( DeterminePixelType( pxlFmtARGB ), PixelTypeARGB, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeABGR )" << endl;
    ::SDL_PixelFormat pxlFmtABGR = DetermineSDLPixelFormat( PixelTypeABGR );
    TESTCHECK( DeterminePixelType( pxlFmtABGR ), PixelTypeABGR, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeRGBA )" << endl;
    ::SDL_PixelFormat pxlFmtRGBA = DetermineSDLPixelFormat( PixelTypeRGBA );
    TESTCHECK( DeterminePixelType( pxlFmtRGBA ), PixelTypeRGBA, &ok );
    cout << "DetermineSDLPixelFormat( PixelTypeBGRA )" << endl;
    ::SDL_PixelFormat pxlFmtBGRA = DetermineSDLPixelFormat( PixelTypeBGRA );
    TESTCHECK( DeterminePixelType( pxlFmtBGRA ), PixelTypeBGRA, &ok );

    Uint8 r8 = static_cast< Uint8 >( r );
    Uint8 g8 = static_cast< Uint8 >( g );
    Uint8 b8 = static_cast< Uint8 >( b );
    Uint8 a8 = static_cast< Uint8 >( a );
    TESTCHECK( ::SDL_MapRGB( &pxlFmt555, r8, g8, b8 ),
               (Uint32) pxl555c.Value(), &ok );
    TESTCHECK( ::SDL_MapRGBA( &pxlFmt1555, r8, g8, b8, 0 ),
               (Uint32) pxl1555c0.Value(), &ok );
    TESTCHECK( ::SDL_MapRGBA( &pxlFmt1555, r8, g8, b8, 255 ),
               (Uint32) pxl1555c1.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmt565, r8, g8, b8 ),
               (Uint32) pxl565c.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtRGB, r8, g8, b8 ),
               pxlRGBc.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtBGR, r8, g8, b8 ),
               pxlBGRc.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmt0RGB, r8, g8, b8 ),
               pxl0RGBc.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmt0BGR, r8, g8, b8 ),
               pxl0BGRc.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtRGB0, r8, g8, b8 ),
               pxlRGB0c.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtBGR0, r8, g8, b8 ),
               pxlBGR0c.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtARGB, r8, g8, b8 ),
               pxlARGBc3.Value(), &ok );
    TESTCHECK( ::SDL_MapRGBA( &pxlFmtARGB, r8, g8, b8, a8 ),
               pxlARGBc4.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtABGR, r8, g8, b8 ),
               pxlABGRc3.Value(), &ok );
    TESTCHECK( ::SDL_MapRGBA( &pxlFmtABGR, r8, g8, b8, a8 ),
               pxlABGRc4.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtRGBA, r8, g8, b8 ),
               pxlRGBAc3.Value(), &ok );
    TESTCHECK( ::SDL_MapRGBA( &pxlFmtRGBA, r8, g8, b8, a8 ),
               pxlRGBAc4.Value(), &ok );
    TESTCHECK( ::SDL_MapRGB( &pxlFmtBGRA, r8, g8, b8 ),
               pxlBGRAc3.Value(), &ok );
    TESTCHECK( ::SDL_MapRGBA( &pxlFmtBGRA, r8, g8, b8, a8 ),
               pxlBGRAc4.Value(), &ok );

    cout << "SDL_GetRGBA( " << u16 << ", pxlFmt555, ... )" << endl;
    ::SDL_GetRGBA( u16, &pxlFmt555, &r8, &g8, &b8, &a8 );
    r = r8; g = g8; b = b8; a = a8;
    TESTCHECK( pxl555v.Color( true ).Red(), r, &ok );
    TESTCHECK( pxl555v.Color( true ).Green(), g, &ok );
    TESTCHECK( pxl555v.Color( true ).Blue(), b, &ok );
    TESTCHECK( a, 255, &ok );
    cout << "SDL_GetRGBA( " << u16 << ", pxlFmt1555, ... )" << endl;
    ::SDL_GetRGBA( u16, &pxlFmt1555, &r8, &g8, &b8, &a8 );
    r = r8; g = g8; b = b8; a = a8;
    TESTCHECK( pxl1555v.Color( true ).Red(), r, &ok );
    TESTCHECK( pxl1555v.Color( true ).Green(), g, &ok );
    TESTCHECK( pxl1555v.Color( true ).Blue(), b, &ok );
    TESTCHECK( a, 0, &ok );
    cout << "SDL_GetRGBA( " << u16 << ", pxlFmt565, ... )" << endl;
    ::SDL_GetRGBA( u16, &pxlFmt565, &r8, &g8, &b8, &a8 );
    r = r8; g = g8; b = b8; a = a8;
    TESTCHECK( pxl565v.Color( true ).Red(), r, &ok );
    TESTCHECK( pxl565v.Color( true ).Green(), g, &ok );
    TESTCHECK( pxl565v.Color( true ).Blue(), b, &ok );
    TESTCHECK( a, 255, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtRGB, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtRGB, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlRGBv.Color().Red(), r, &ok );
    TESTCHECK( pxlRGBv.Color().Green(), g, &ok );
    TESTCHECK( pxlRGBv.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtBGR, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtBGR, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlBGRv.Color().Red(), r, &ok );
    TESTCHECK( pxlBGRv.Color().Green(), g, &ok );
    TESTCHECK( pxlBGRv.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmt0RGB, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmt0RGB, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxl0RGBv.Color().Red(), r, &ok );
    TESTCHECK( pxl0RGBv.Color().Green(), g, &ok );
    TESTCHECK( pxl0RGBv.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmt0BGR, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmt0BGR, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxl0BGRv.Color().Red(), r, &ok );
    TESTCHECK( pxl0BGRv.Color().Green(), g, &ok );
    TESTCHECK( pxl0BGRv.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtRGB0, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtRGB0, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlRGB0v.Color().Red(), r, &ok );
    TESTCHECK( pxlRGB0v.Color().Green(), g, &ok );
    TESTCHECK( pxlRGB0v.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtBGR0, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtBGR0, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlBGR0v.Color().Red(), r, &ok );
    TESTCHECK( pxlBGR0v.Color().Green(), g, &ok );
    TESTCHECK( pxlBGR0v.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtARGB, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtARGB, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlARGBv.Color().Red(), r, &ok );
    TESTCHECK( pxlARGBv.Color().Green(), g, &ok );
    TESTCHECK( pxlARGBv.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtABGR, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtABGR, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlABGRv.Color().Red(), r, &ok );
    TESTCHECK( pxlABGRv.Color().Green(), g, &ok );
    TESTCHECK( pxlABGRv.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtRGBA, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtRGBA, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlRGBAv.Color().Red(), r, &ok );
    TESTCHECK( pxlRGBAv.Color().Green(), g, &ok );
    TESTCHECK( pxlRGBAv.Color().Blue(), b, &ok );
    cout << "SDL_GetRGBA( " << u32 << ", pxlFmtBGRA, ... )" << endl;
    ::SDL_GetRGB( u32, &pxlFmtBGRA, &r8, &g8, &b8 );
    r = r8; g = g8; b = b8;
    TESTCHECK( pxlBGRAv.Color().Red(), r, &ok );
    TESTCHECK( pxlBGRAv.Color().Green(), g, &ok );
    TESTCHECK( pxlBGRAv.Color().Blue(), b, &ok );

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

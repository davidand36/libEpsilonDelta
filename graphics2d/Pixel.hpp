#ifndef PIXEL_HPP
#define PIXEL_HPP
/*
  Pixel.hpp
  Copyright (C) 2007 David M. Anderson

  Pixel classes.
  NOTES:
  1. i) Pixel8 represents an 8-bit pixel. Generally this will be an index into a
     256-color palette, but it could also be used for luminance, shadow, bump,
     etc., maps.
     ii) Pixel555, Pixel1555, and Pixel565 are 16-bit pixels with colors packed
     in native-endian order as RGB or ARGB.
     iii) PixelRGB,...,PixelBGRA are 24- or 32-bit pixels with 8 bits per
     component.
     The bytes are arranged in the order listed, regardless of platform.
  2. The value constructors and Value() methods deal directly with the
     underlying, if rather opaque, in-memory value of the pixel, so they are
     lossless.
     For 24- and 32-bit pixels, the color constructors and Color() methods are
     also lossless, but for the 16-bit pixels, this is not true. When
     converting from Color3B or Color4B, low-order bits are necessarily lost.
     For 16-bit pixels, Color() offers two options. The default is simply to
     shift the components back to 8 bits, setting the low bits to 0.
     Alternatively, an adjustment can be made by using high order bits to set
     the missing low ones. The advantage of this is that color components can
     range all the way from 0 to 255, indicating full saturation.
  4. The implementation uses bit fields, which are convenient, though rather
     platform-dependent. It appears that on big-endian platforms, the
     fields are arranged starting at the high end of the containing integer,
     while on little-endian platforms, fields start at the low end. (The bits
     within each field are not rearranged.) For the 32-bit pixel types, this
     means that in either case the fields are in the same order as the bytes.
*/


#include "Color.hpp"
#include "FixEndian.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


enum EPixelType
{
    PixelType8,
    PixelType555,
    PixelType1555,
    PixelType565,
    PixelTypeRGB,
    PixelTypeBGR,
    PixelType0RGB,
    PixelType0BGR,
    PixelTypeRGB0,
    PixelTypeBGR0,
    PixelTypeARGB,
    PixelTypeABGR,
    PixelTypeRGBA,
    PixelTypeBGRA,

    NumPixelTypes,

    UnknownPixelType = NumPixelTypes,

#if BITSPERPIXEL==8
    NativePixelType = PixelType8
#elif BITSPERPIXEL==16
    NativePixelType = PixelType565
#elif BITSPERPIXEL==32
    NativePixelType = PixelTypeRGB0 //???
#else
#error "Unexpected BITSPERPIXEL"
#endif
};


//*****************************************************************************


class Pixel8
{
public:
    Pixel8( );
    Pixel8( uint8_t value );
    uint8_t Value( ) const;

private:
    uint8_t m_value;
};


//*****************************************************************************


class Pixel555
{
public:
    Pixel555( );
    Pixel555( const Color3B & color );
    Pixel555( uint16_t value );
    Color3B Color( bool adjust = false ) const;
    uint16_t Value( ) const;
    
private:
    union
    {
        uint16_t m_value;
        struct
        {
#ifdef BIG_ENDIAN
            uint16_t : 1;
            uint16_t m_red : 5;
            uint16_t m_green : 5;
            uint16_t m_blue : 5;
#else
            uint16_t m_blue : 5;
            uint16_t m_green : 5;
            uint16_t m_red : 5;
            uint16_t : 1;
#endif
        } m_rgb;
    };
};


//*****************************************************************************


class Pixel1555
{
public:
    Pixel1555( );
    Pixel1555( const Color4B & color );
    Pixel1555( uint16_t value );
    Color4B Color( bool adjust = false ) const;
    uint16_t Value( ) const;
    
private:
    union
    {
        uint16_t m_value;
        struct
        {
#ifdef BIG_ENDIAN
            uint16_t m_alpha: 1;
            uint16_t m_red : 5;
            uint16_t m_green : 5;
            uint16_t m_blue : 5;
#else
            uint16_t m_blue : 5;
            uint16_t m_green : 5;
            uint16_t m_red : 5;
            uint16_t m_alpha: 1;
#endif
        } m_argb;
    };
};


//*****************************************************************************


class Pixel565
{
public:
    Pixel565( );
    Pixel565( const Color3B & color );
    Pixel565( uint16_t value );
    Color3B Color( bool adjust = false ) const;
    uint16_t Value( ) const;
    
private:
    union
    {
        uint16_t m_value;
        struct
        {
#ifdef BIG_ENDIAN
            uint16_t m_red : 5;
            uint16_t m_green : 6;
            uint16_t m_blue : 5;
#else
            uint16_t m_blue : 5;
            uint16_t m_green : 6;
            uint16_t m_red : 5;
#endif
        } m_rgb;
    };
};


//*****************************************************************************


class PixelRGB
{
public:
    PixelRGB( );
    PixelRGB( const Color3B & color );
    PixelRGB( uint32_t value );
    PixelRGB( char bytes[ 3 ] );
    Color3B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    uint8_t m_bytes[ 3 ];
};


//*****************************************************************************


class PixelBGR
{
public:
    PixelBGR( );
    PixelBGR( const Color3B & color );
    PixelBGR( uint32_t value );
    PixelBGR( char bytes[ 3 ] );
    Color3B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    uint8_t m_bytes[ 3 ];
};


//*****************************************************************************


class Pixel0RGB
{
public:
    Pixel0RGB( );
    Pixel0RGB( const Color3B & color );
    Pixel0RGB( uint32_t value );
    Color3B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t : 8;
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
        } m_rgb;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


class Pixel0BGR
{
public:
    Pixel0BGR( );
    Pixel0BGR( const Color3B & color );
    Pixel0BGR( uint32_t value );
    Color3B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t : 8;
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
        } m_rgb;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


class PixelRGB0
{
public:
    PixelRGB0( );
    PixelRGB0( const Color3B & color );
    PixelRGB0( uint32_t value );
    Color3B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
            uint32_t : 8;
        } m_rgb;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


class PixelBGR0
{
public:
    PixelBGR0( );
    PixelBGR0( const Color3B & color );
    PixelBGR0( uint32_t value );
    Color3B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
            uint32_t : 8;
        } m_rgb;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


class PixelARGB
{
public:
    PixelARGB( );
    PixelARGB( const Color3B & color );
    PixelARGB( const Color4B & color );
    PixelARGB( uint32_t value );
    Color4B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t m_alpha : 8;
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
        } m_rgba;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


class PixelABGR
{
public:
    PixelABGR( );
    PixelABGR( const Color3B & color );
    PixelABGR( const Color4B & color );
    PixelABGR( uint32_t value );
    Color4B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t m_alpha : 8;
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
        } m_rgba;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


class PixelRGBA
{
public:
    PixelRGBA( );
    PixelRGBA( const Color3B & color );
    PixelRGBA( const Color4B & color );
    PixelRGBA( uint32_t value );
    Color4B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
            uint32_t m_alpha : 8;
        } m_rgba;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


class PixelBGRA
{
public:
    PixelBGRA( );
    PixelBGRA( const Color3B & color );
    PixelBGRA( const Color4B & color );
    PixelBGRA( uint32_t value );
    Color4B Color( ) const;
    uint32_t Value( ) const;
    const uint8_t * Bytes( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
            uint32_t m_alpha : 8;
        } m_rgba;
        uint8_t m_bytes[ 4 ];
    };
};


//*****************************************************************************


#ifdef USE_SDL

::SDL_PixelFormat DetermineSDLPixelFormat( EPixelType pixelType );
EPixelType DeterminePixelType( const ::SDL_PixelFormat & pixelFormat );

#ifdef DEBUG
void PrintPixelFormat( const ::SDL_PixelFormat & pixelFormat );
#endif

#endif

//-----------------------------------------------------------------------------

#ifdef DEBUG

const char * GetPixelTypeName( EPixelType pixelType );
bool TestPixel( );

#endif


//*****************************************************************************
//*****************************************************************************


inline
Pixel8::Pixel8( )
{
}

//.............................................................................

inline
Pixel8::Pixel8( uint8_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
uint8_t 
Pixel8::Value( ) const
{
    return m_value;
}


//*****************************************************************************


inline
Pixel555::Pixel555( )
{
}

//.............................................................................

inline 
Pixel555::Pixel555( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint16_t)( color.Red() >> 3 );
    m_rgb.m_green = (uint16_t)( color.Green() >> 3 );
    m_rgb.m_blue = (uint16_t)( color.Blue() >> 3 );
}

//.............................................................................

inline
Pixel555::Pixel555( uint16_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color3B 
Pixel555::Color( bool adjust ) const
{
    if ( adjust )
    {
        //This allows the stored components to represent the full range 0-255.
        int r = (m_rgb.m_red << 3) | (m_rgb.m_red >> 2);
        int g = (m_rgb.m_green << 3) | (m_rgb.m_green >> 2);
        int b = (m_rgb.m_blue << 3) | (m_rgb.m_blue >> 2);
        return Color3B( r, g, b );
    }
    else
    {
        int r = m_rgb.m_red << 3;
        int g = m_rgb.m_green << 3;
        int b = m_rgb.m_blue << 3;
        return Color3B( r, g, b );
    }
}

//-----------------------------------------------------------------------------

inline
uint16_t 
Pixel555::Value( ) const
{
    return m_value;
}


//*****************************************************************************


inline
Pixel1555::Pixel1555( )
{
}

//.............................................................................

inline 
Pixel1555::Pixel1555( const Color4B & color )
    :   m_value( 0 )
{
    m_argb.m_alpha = (color.Alpha() == 0)  ?  0  :  1;
    m_argb.m_red = (uint16_t)( color.Red() >> 3 );
    m_argb.m_green = (uint16_t)( color.Green() >> 3 );
    m_argb.m_blue = (uint16_t)( color.Blue() >> 3 );
}

//.............................................................................

inline
Pixel1555::Pixel1555( uint16_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color4B 
Pixel1555::Color( bool adjust ) const
{
    if ( adjust )
    {
        //This allows the stored components to represent the full range 0-255.
        int r = (m_argb.m_red << 3) | (m_argb.m_red >> 2);
        int g = (m_argb.m_green << 3) | (m_argb.m_green >> 2);
        int b = (m_argb.m_blue << 3) | (m_argb.m_blue >> 2);
        int a = (m_argb.m_alpha == 0)  ?  0  :  255;
        return Color4B( r, g, b, a );
    }
    else
    {
        int r = m_argb.m_red << 3;
        int g = m_argb.m_green << 3;
        int b = m_argb.m_blue << 3;
        int a = (m_argb.m_alpha == 0)  ?  0  :  255;
        return Color4B( r, g, b, a );
    }
}

//-----------------------------------------------------------------------------

inline
uint16_t 
Pixel1555::Value( ) const
{
    return m_value;
}


//*****************************************************************************


inline
Pixel565::Pixel565( )
{
}

//.............................................................................

inline 
Pixel565::Pixel565( const Color3B & color )
{
    m_rgb.m_red = (uint16_t)( color.Red() >> 3 );
    m_rgb.m_green = (uint16_t)( color.Green() >> 2 );
    m_rgb.m_blue = (uint16_t)( color.Blue() >> 3 );
}

//.............................................................................

inline
Pixel565::Pixel565( uint16_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color3B 
Pixel565::Color( bool adjust ) const
{
    if ( adjust )
    {
        //This allows the stored components to represent the full range 0-255.
        int r = (m_rgb.m_red << 3) | (m_rgb.m_red >> 2);
        int g = (m_rgb.m_green << 2) | (m_rgb.m_green >> 4);
        int b = (m_rgb.m_blue << 3) | (m_rgb.m_blue >> 2);
        return Color3B( r, g, b );
    }
    else
    {
        int r = m_rgb.m_red << 3;
        int g = m_rgb.m_green << 2;
        int b = m_rgb.m_blue << 3;
        return Color3B( r, g, b );
    }
}

//-----------------------------------------------------------------------------

inline
uint16_t 
Pixel565::Value( ) const
{
    return m_value;
}


//*****************************************************************************


inline
PixelRGB::PixelRGB( )
{
}

//.............................................................................

inline
PixelRGB::PixelRGB( const Color3B & color )
{
    m_bytes[ 0 ] = (uint8_t) color.Red();
    m_bytes[ 1 ] = (uint8_t) color.Green();
    m_bytes[ 2 ] = (uint8_t) color.Blue();
}

//.............................................................................

inline
PixelRGB::PixelRGB( uint32_t value )
{
#ifdef BIG_ENDIAN
    m_bytes[ 0 ] = (uint8_t)((value >> 16) & 0xFF);
    m_bytes[ 1 ] = (uint8_t)((value >> 8) & 0xFF);
    m_bytes[ 2 ] = (uint8_t)(value & 0xFF);
#else
    m_bytes[ 0 ] = (uint8_t)(value & 0xFF);
    m_bytes[ 1 ] = (uint8_t)((value >> 8) & 0xFF);
    m_bytes[ 2 ] = (uint8_t)((value >> 16) & 0xFF);
#endif
}

//.............................................................................

inline
PixelRGB::PixelRGB( char bytes[ 3 ] )
{
    for ( int i = 0; i < 3; ++i )
        m_bytes[ i ] = bytes[ i ];
}

//=============================================================================

inline
Color3B 
PixelRGB::Color( ) const
{
    int r = m_bytes[ 0 ];
    int g = m_bytes[ 1 ];
    int b = m_bytes[ 2 ];
    return Color3B( r, g, b );
}

//-----------------------------------------------------------------------------

inline
uint32_t
PixelRGB::Value( ) const
{
#ifdef BIG_ENDIAN
    return (uint32_t)( (m_bytes[ 0 ] << 16)
                       | (m_bytes[ 1 ] << 8)
                       | m_bytes[ 2 ] );
#else
    return (uint32_t)( m_bytes[ 0 ]
                       | (m_bytes[ 1 ] << 8)
                       | (m_bytes[ 2 ] << 16) );
#endif
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelRGB::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline
PixelBGR::PixelBGR( )
{
}

//.............................................................................

inline
PixelBGR::PixelBGR( const Color3B & color )
{
    m_bytes[ 0 ] = (uint8_t) color.Blue();
    m_bytes[ 1 ] = (uint8_t) color.Green();
    m_bytes[ 2 ] = (uint8_t) color.Red();
}

//.............................................................................

inline
PixelBGR::PixelBGR( uint32_t value )
{
#ifdef BIG_ENDIAN
    m_bytes[ 0 ] = (uint8_t)((value >> 16) & 0xFF);
    m_bytes[ 1 ] = (uint8_t)((value >> 8) & 0xFF);
    m_bytes[ 2 ] = (uint8_t)(value & 0xFF);
#else
    m_bytes[ 0 ] = (uint8_t)(value & 0xFF);
    m_bytes[ 1 ] = (uint8_t)((value >> 8) & 0xFF);
    m_bytes[ 2 ] = (uint8_t)((value >> 16) & 0xFF);
#endif
}

//.............................................................................

inline
PixelBGR::PixelBGR( char bytes[ 3 ] )
{
    for ( int i = 0; i < 3; ++i )
        m_bytes[ i ] = bytes[ i ];
}

//=============================================================================

inline
Color3B 
PixelBGR::Color( ) const
{
    int r = m_bytes[ 2 ];
    int g = m_bytes[ 1 ];
    int b = m_bytes[ 0 ];
    return Color3B( r, g, b );
}

//-----------------------------------------------------------------------------

inline
uint32_t
PixelBGR::Value( ) const
{
#ifdef BIG_ENDIAN
    return (uint32_t)( (m_bytes[ 0 ] << 16)
                       | (m_bytes[ 1 ] << 8)
                       | m_bytes[ 2 ] );
#else
    return (uint32_t)( m_bytes[ 0 ]
                       | (m_bytes[ 1 ] << 8)
                       | (m_bytes[ 2 ] << 16) );
#endif
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelBGR::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
Pixel0RGB::Pixel0RGB( )
{
}

//.............................................................................

inline 
Pixel0RGB::Pixel0RGB( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint8_t) color.Red();
    m_rgb.m_green = (uint8_t) color.Green();
    m_rgb.m_blue = (uint8_t) color.Blue();
}

//.............................................................................

inline 
Pixel0RGB::Pixel0RGB( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================


inline
Color3B 
Pixel0RGB::Color( ) const
{
    return Color3B( m_rgb.m_red, m_rgb.m_green, m_rgb.m_blue );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
Pixel0RGB::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
Pixel0RGB::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
Pixel0BGR::Pixel0BGR( )
{
}

//.............................................................................

inline 
Pixel0BGR::Pixel0BGR( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint8_t) color.Red();
    m_rgb.m_green = (uint8_t) color.Green();
    m_rgb.m_blue = (uint8_t) color.Blue();
}

//.............................................................................

inline 
Pixel0BGR::Pixel0BGR( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color3B 
Pixel0BGR::Color( ) const
{
    return Color3B( m_rgb.m_red, m_rgb.m_green, m_rgb.m_blue );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
Pixel0BGR::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
Pixel0BGR::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
PixelRGB0::PixelRGB0( )
{
}

//.............................................................................

inline 
PixelRGB0::PixelRGB0( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint8_t) color.Red();
    m_rgb.m_green = (uint8_t) color.Green();
    m_rgb.m_blue = (uint8_t) color.Blue();
}

//.............................................................................

inline 
PixelRGB0::PixelRGB0( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color3B 
PixelRGB0::Color( ) const
{
    return Color3B( m_rgb.m_red, m_rgb.m_green, m_rgb.m_blue );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
PixelRGB0::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelRGB0::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
PixelBGR0::PixelBGR0( )
{
}

//.............................................................................

inline 
PixelBGR0::PixelBGR0( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint8_t) color.Red();
    m_rgb.m_green = (uint8_t) color.Green();
    m_rgb.m_blue = (uint8_t) color.Blue();
}

//.............................................................................

inline 
PixelBGR0::PixelBGR0( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color3B 
PixelBGR0::Color( ) const
{
    return Color3B( m_rgb.m_red, m_rgb.m_green, m_rgb.m_blue );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
PixelBGR0::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelBGR0::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
PixelARGB::PixelARGB( )
{
}

//.............................................................................

inline 
PixelARGB::PixelARGB( const Color3B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = 255;
}

//.............................................................................

inline
PixelARGB::PixelARGB( const Color4B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = color.Alpha();
}

//.............................................................................

inline 
PixelARGB::PixelARGB( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color4B 
PixelARGB::Color( ) const
{
    return Color4B( m_rgba.m_red, m_rgba.m_green, m_rgba.m_blue,
                    m_rgba.m_alpha );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
PixelARGB::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelARGB::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
PixelABGR::PixelABGR( )
{
}

//.............................................................................

inline 
PixelABGR::PixelABGR( const Color3B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = 255;
}

//.............................................................................

inline
PixelABGR::PixelABGR( const Color4B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = color.Alpha();
}

//.............................................................................

inline 
PixelABGR::PixelABGR( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color4B 
PixelABGR::Color( ) const
{
    return Color4B( m_rgba.m_red, m_rgba.m_green, m_rgba.m_blue,
                    m_rgba.m_alpha );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
PixelABGR::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelABGR::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
PixelRGBA::PixelRGBA( )
{
}

//.............................................................................

inline 
PixelRGBA::PixelRGBA( const Color3B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = 255;
}

//.............................................................................

inline
PixelRGBA::PixelRGBA( const Color4B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = color.Alpha();
}

//.............................................................................

inline 
PixelRGBA::PixelRGBA( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color4B 
PixelRGBA::Color( ) const
{
    return Color4B( m_rgba.m_red, m_rgba.m_green, m_rgba.m_blue,
                    m_rgba.m_alpha );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
PixelRGBA::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelRGBA::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************


inline 
PixelBGRA::PixelBGRA( )
{
}

//.............................................................................

inline 
PixelBGRA::PixelBGRA( const Color3B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = 255;
}

//.............................................................................

inline
PixelBGRA::PixelBGRA( const Color4B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = color.Alpha();
}

//.............................................................................

inline 
PixelBGRA::PixelBGRA( uint32_t value )
    :   m_value( value )
{
}

//=============================================================================

inline
Color4B 
PixelBGRA::Color( ) const
{
    return Color4B( m_rgba.m_red, m_rgba.m_green, m_rgba.m_blue,
                    m_rgba.m_alpha );
}

//-----------------------------------------------------------------------------

inline
uint32_t 
PixelBGRA::Value( ) const
{
    return m_value;
}

//-----------------------------------------------------------------------------

inline
const uint8_t * 
PixelBGRA::Bytes( ) const
{
    return m_bytes;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PIXEL_HPP

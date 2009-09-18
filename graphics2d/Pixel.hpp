#ifndef PIXEL_HPP
#define PIXEL_HPP
/*
  Pixel.hpp
  Copyright (C) 2007 David M. Anderson

  Pixel classes.
*/


#include "Color.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


#define BITFIELDS_HIGHFIRST  0
#define RGB_REDHIGH  1
#define BITFIELDS_REDFIRST  ( BITFIELDS_HIGHFIRST == RGB_REDHIGH )

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


enum EPixelType
{
    PixelType8,
    PixelType555,
    PixelType565,
    PixelType888,
    PixelType888Rev,
    PixelType0888,
    PixelType0888Rev,
    PixelType8888,
    PixelType8888Rev,

    NumPixelTypes,

    UnknownPixelType = NumPixelTypes,

#if BITSPERPIXEL==8
    NativePixelType = PixelType8
#elif BITSPERPIXEL==16
    NativePixelType = PixelType565
#elif BITSPERPIXEL==32
    NativePixelType = PixelType0888
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
    Pixel555( uint16_t value );
    Pixel555( const Color3B & color );
    uint16_t Value( ) const;
    Color3B Color( ) const;
    
private:
    union
    {
        uint16_t m_value;
        struct
        {
#if BITFIELDS_REDFIRST
            uint16_t m_red : 5;
            uint16_t m_green : 5;
            uint16_t m_blue : 5;
            uint16_t : 1;
#else
            uint16_t : 1;
            uint16_t m_blue : 5;
            uint16_t m_green : 5;
            uint16_t m_red : 5;
#endif
        } m_rgb;
    };
};


//*****************************************************************************


class Pixel565
{
public:
    Pixel565( );
    Pixel565( uint16_t value );
    Pixel565( const Color3B & color );
    uint16_t Value( ) const;
    Color3B Color( ) const;
    
private:
    union
    {
        uint16_t m_value;
        struct
        {
#if BITFIELDS_REDFIRST
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


class Pixel888
{
public:
    Pixel888( );
    Pixel888( char value[ 3 ] );
    Pixel888( const Color3B & color );
    const uint8_t * Value( ) const;
    Color3B Color( ) const;

private:
    uint8_t m_value[ 3 ];
};


//*****************************************************************************


class Pixel888Rev
{
public:
    Pixel888Rev( );
    Pixel888Rev( char value[ 3 ] );
    Pixel888Rev( const Color3B & color );
    const uint8_t * Value( ) const;
    Color3B Color( ) const;

private:
    uint8_t m_value[ 3 ];
};


//*****************************************************************************


class Pixel0888
{
public:
    Pixel0888( );
    Pixel0888( uint32_t value );
    Pixel0888( const Color3B & color );
    uint32_t Value( ) const;
    Color3B Color( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
#if BITFIELDS_REDFIRST
            uint32_t : 8;
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
#else
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
            uint32_t : 8;
#endif
        } m_rgb;
    };
};


//*****************************************************************************


class Pixel0888Rev
{
public:
    Pixel0888Rev( );
    Pixel0888Rev( uint32_t value );
    Pixel0888Rev( const Color3B & color );
    uint32_t Value( ) const;
    Color3B Color( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
#if BITFIELDS_REDFIRST
            uint32_t : 8;
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
#else
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
            uint32_t : 8;
#endif
        } m_rgb;
    };
};


//*****************************************************************************


class Pixel8888
{
public:
    Pixel8888( );
    Pixel8888( uint32_t value );
    Pixel8888( const Color3B & color );
    Pixel8888( const Color4B & color );
    uint32_t Value( ) const;
    Color3B Color( ) const;
    int Alpha( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
#if BITFIELDS_REDFIRST
            uint32_t m_alpha : 8;
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
#else
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
            uint32_t m_alpha : 8;
#endif
        } m_rgba;
    };
};


//*****************************************************************************


class Pixel8888Rev
{
public:
    Pixel8888Rev( );
    Pixel8888Rev( uint32_t value );
    Pixel8888Rev( const Color3B & color );
    Pixel8888Rev( const Color4B & color );
    uint32_t Value( ) const;
    Color3B Color( ) const;
    int Alpha( ) const;

private:
    union
    {
        uint32_t m_value;
        struct
        {
#if BITFIELDS_REDFIRST
            uint32_t m_alpha : 8;
            uint32_t m_blue : 8;
            uint32_t m_green : 8;
            uint32_t m_red : 8;
#else
            uint32_t m_red : 8;
            uint32_t m_green : 8;
            uint32_t m_blue : 8;
            uint32_t m_alpha : 8;
#endif
        } m_rgba;
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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

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

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel555::Pixel555( uint16_t value )
    :   m_value( value )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel555::Pixel555( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint16_t)( color.Red() >> 3 );
    m_rgb.m_green = (uint16_t)( color.Green() >> 3 );
    m_rgb.m_blue = (uint16_t)( color.Blue() >> 3 );
}

//=============================================================================

inline
uint16_t 
Pixel555::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel555::Color( ) const
{
    //This allows the stored components to represent the full range 0-255.
    int r = (m_rgb.m_red << 3) | (m_rgb.m_red >> 2);
    int g = (m_rgb.m_green << 3) | (m_rgb.m_green >> 2);
    int b = (m_rgb.m_blue << 3) | (m_rgb.m_blue >> 2);
    return Color3B( r, g, b );
}


//*****************************************************************************


inline
Pixel565::Pixel565( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel565::Pixel565( uint16_t value )
    :   m_value( value )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel565::Pixel565( const Color3B & color )
{
    m_rgb.m_red = (uint16_t)( color.Red() >> 3 );
    m_rgb.m_green = (uint16_t)( color.Green() >> 2 );
    m_rgb.m_blue = (uint16_t)( color.Blue() >> 3 );
}

//=============================================================================

inline
uint16_t 
Pixel565::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel565::Color( ) const
{
    //This allows the stored components to represent the full range 0-255.
    int r = (m_rgb.m_red << 3) | (m_rgb.m_red >> 2);
    int g = (m_rgb.m_green << 2) | (m_rgb.m_green >> 4);
    int b = (m_rgb.m_blue << 3) | (m_rgb.m_blue >> 2);
    return Color3B( r, g, b );
}


//*****************************************************************************


inline
Pixel888::Pixel888( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel888::Pixel888( char value[ 3 ] )
{
    for ( int i = 0; i < 3; ++i )
        m_value[ i ] = value[ i ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel888::Pixel888( const Color3B & color )
{
#if RBG_REDHIGH
    m_value[ 0 ] = (uint8_t) color.Red();
    m_value[ 1 ] = (uint8_t) color.Green();
    m_value[ 2 ] = (uint8_t) color.Blue();
#else
    m_value[ 0 ] = (uint8_t) color.Blue();
    m_value[ 1 ] = (uint8_t) color.Green();
    m_value[ 2 ] = (uint8_t) color.Red();
#endif
}

//=============================================================================

inline
const uint8_t * 
Pixel888::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel888::Color( ) const
{
#if RGB_REDHIGH
    int r = m_value[ 0 ];
    int g = m_value[ 1 ];
    int b = m_value[ 2 ];
#else
    int r = m_value[ 2 ];
    int g = m_value[ 1 ];
    int b = m_value[ 0 ];
#endif
    return Color3B( r, g, b );
}


//*****************************************************************************


inline
Pixel888Rev::Pixel888Rev( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel888Rev::Pixel888Rev( char value[ 3 ] )
{
    for ( int i = 0; i < 3; ++i )
        m_value[ i ] = value[ i ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel888Rev::Pixel888Rev( const Color3B & color )
{
#if RBG_REDHIGH
    m_value[ 0 ] = (uint8_t) color.Blue();
    m_value[ 1 ] = (uint8_t) color.Green();
    m_value[ 2 ] = (uint8_t) color.Red();
#else
    m_value[ 0 ] = (uint8_t) color.Red();
    m_value[ 1 ] = (uint8_t) color.Green();
    m_value[ 2 ] = (uint8_t) color.Blue();
#endif
}

//=============================================================================

inline
const uint8_t * 
Pixel888Rev::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel888Rev::Color( ) const
{
#if RGB_REDHIGH
    int r = m_value[ 2 ];
    int g = m_value[ 1 ];
    int b = m_value[ 0 ];
#else
    int r = m_value[ 0 ];
    int g = m_value[ 1 ];
    int b = m_value[ 2 ];
#endif
    return Color3B( r, g, b );
}


//*****************************************************************************


inline 
Pixel0888::Pixel0888( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel0888::Pixel0888( uint32_t value )
    :   m_value( value )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel0888::Pixel0888( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint8_t) color.Red();
    m_rgb.m_green = (uint8_t) color.Green();
    m_rgb.m_blue = (uint8_t) color.Blue();
}

//=============================================================================

inline
uint32_t 
Pixel0888::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel0888::Color( ) const
{
    return Color3B( m_rgb.m_red, m_rgb.m_green, m_rgb.m_blue );
}


//*****************************************************************************


inline 
Pixel0888Rev::Pixel0888Rev( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel0888Rev::Pixel0888Rev( uint32_t value )
    :   m_value( value )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel0888Rev::Pixel0888Rev( const Color3B & color )
    :   m_value( 0 )
{
    m_rgb.m_red = (uint8_t) color.Red();
    m_rgb.m_green = (uint8_t) color.Green();
    m_rgb.m_blue = (uint8_t) color.Blue();
}

//=============================================================================

inline
uint32_t 
Pixel0888Rev::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel0888Rev::Color( ) const
{
    return Color3B( m_rgb.m_red, m_rgb.m_green, m_rgb.m_blue );
}


//*****************************************************************************


inline 
Pixel8888::Pixel8888( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel8888::Pixel8888( uint32_t value )
    :   m_value( value )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel8888::Pixel8888( const Color3B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = 255;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel8888::Pixel8888( const Color4B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = color.Alpha();
}

//=============================================================================

inline
uint32_t 
Pixel8888::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel8888::Color( ) const
{
    return Color3B( m_rgba.m_red, m_rgba.m_green, m_rgba.m_blue );
}

//-----------------------------------------------------------------------------

inline 
int 
Pixel8888::Alpha( ) const
{
    return m_rgba.m_alpha;
}


//*****************************************************************************


inline 
Pixel8888Rev::Pixel8888Rev( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel8888Rev::Pixel8888Rev( uint32_t value )
    :   m_value( value )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Pixel8888Rev::Pixel8888Rev( const Color3B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = 255;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Pixel8888Rev::Pixel8888Rev( const Color4B & color )
{
    m_rgba.m_red = color.Red();
    m_rgba.m_green = color.Green();
    m_rgba.m_blue = color.Blue();
    m_rgba.m_alpha = color.Alpha();
}

//=============================================================================

inline
uint32_t 
Pixel8888Rev::Value( ) const
{
    return m_value;
}

//=============================================================================

inline
Color3B 
Pixel8888Rev::Color( ) const
{
    return Color3B( m_rgba.m_red, m_rgba.m_green, m_rgba.m_blue );
}

//-----------------------------------------------------------------------------

inline 
int 
Pixel8888Rev::Alpha( ) const
{
    return m_rgba.m_alpha;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //PIXEL_HPP

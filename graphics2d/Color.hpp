#ifndef COLOR_HPP
#define COLOR_HPP
/*
  Color.hpp
  Copyright (C) 2007 David M. Anderson

  Color classes.
  NOTES:
  1. Color3* stores red, green, and blue values.
     Color4* also stores alpha.
  2. Color*B stores each component in a byte with a range 0 to 255.
     Color*F stores each component in a float with a range 0.0f to 1.0f.
*/


#include "StdInt.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class Color3B;
class Color4B;
class Color3F;
class Color4F;

//*****************************************************************************


class Color3B
{
public:
    Color3B( );
    Color3B( int red, int green, int blue );
    Color3B( const Color3F & color3F );
    Color3B( const Color4B & color4B );
    void Set( int red, int green, int blue );
    void Set( const Color3F & color3F );
    void Set( const Color4B & color4B );
    int Red( ) const;
    int Green( ) const;
    int Blue( ) const;
    const uint8_t * Array( ) const;
    uint8_t * Array( );
    bool operator==( const Color3B & rhs ) const;
    bool operator!=( const Color3B & rhs ) const;

private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;

    friend class Color4B;
    friend class Color3F;
};


//*****************************************************************************


class Color4B
{
public:
    Color4B( );
    Color4B( int red, int green, int blue, int alpha = 255 );
    Color4B( const Color3B & color3B, int alpha = 255 );
    Color4B( const Color3F & color3F, float alpha = 1.f );
    Color4B( const Color4F & color4F );
    void Set( int red, int green, int blue, int alpha = 255 );
    void Set( const Color3B & color3B, int alpha = 255 );
    void Set( const Color3F & color3F, float alpha = 1.f );
    void Set( const Color4F & color4F );
    int Red( ) const;
    int Green( ) const;
    int Blue( ) const;
    int Alpha( ) const;
    const uint8_t * Array( ) const;
    uint8_t * Array( );
    bool operator==( const Color4B & rhs ) const;
    bool operator!=( const Color4B & rhs ) const;

private:
    uint8_t m_red;
    uint8_t m_green;
    uint8_t m_blue;
    uint8_t m_alpha;

    friend class Color3B;
    friend class Color4F;
};


//*****************************************************************************


class Color3F
{
public:
    Color3F( );
    Color3F( float red, float green, float blue );
    Color3F( const Color3B & color3B );
    Color3F( const Color4F & color4F );
    void Set( float red, float green, float blue );
    void Set( const Color3B & color3B );
    void Set( const Color4F & color4F );
    float Red( ) const;
    float Green( ) const;
    float Blue( ) const;
    const float * Array( ) const;
    float * Array( );
    bool operator==( const Color3F & rhs ) const;
    bool operator!=( const Color3F & rhs ) const;

private:
    float m_red;
    float m_green;
    float m_blue;

    friend class Color3B;
    friend class Color4B;
    friend class Color4F;
};


//*****************************************************************************


class Color4F
{
public:
    Color4F( );
    Color4F( float red, float green, float blue, float alpha = 1.f );
    Color4F( const Color3F & color3F, float alpha = 1.f );
    Color4F( const Color4B & color4B );
    void Set( float red, float green, float blue, float alpha = 1.f );
    void Set( const Color3F & color3F, float alpha = 1.f );
    void Set( const Color4B & color4B );
    float Red( ) const;
    float Green( ) const;
    float Blue( ) const;
    float Alpha( ) const;
    const float * Array( ) const;
    float * Array( );
    bool operator==( const Color4F & rhs ) const;
    bool operator!=( const Color4F & rhs ) const;

private:
    float m_red;
    float m_green;
    float m_blue;
    float m_alpha;

    friend class Color4B;
    friend class Color3F;
};


//*****************************************************************************

#ifdef DEBUG

bool TestColor( );

#endif


//#############################################################################


inline 
Color3B::Color3B( )
{
}

//.............................................................................

inline
Color3B::Color3B( int red, int green, int blue )
{
    Set( red, green, blue );
}

//.............................................................................

inline
Color3B::Color3B( const Color3F & color3F )
{
    Set( color3F );
}

//.............................................................................

inline
Color3B::Color3B( const Color4B & color4B )
{
    Set( color4B );
}

//-----------------------------------------------------------------------------

inline
void 
Color3B::Set( int red, int green, int blue )
{
    m_red = (uint8_t) red;
    m_green = (uint8_t) green;
    m_blue = (uint8_t) blue ;
}

//.............................................................................

inline
void 
Color3B::Set( const Color3F & color3F )
{
    m_red = (uint8_t)( color3F.m_red * 255.f  +  0.5f );
    m_green = (uint8_t)( color3F.m_green * 255.f  +  0.5f );
    m_blue = (uint8_t)( color3F.m_blue * 255.f  +  0.5f );
}

//.............................................................................

inline 
void 
Color3B::Set( const Color4B & color4B )
{
    m_red = color4B.m_red;
    m_green = color4B.m_green;
    m_blue = color4B.m_blue;
}

//=============================================================================

inline
int 
Color3B::Red( ) const
{
    return m_red;
}

//-----------------------------------------------------------------------------

inline
int 
Color3B::Green( ) const
{
    return m_green;
}

//-----------------------------------------------------------------------------

inline
int 
Color3B::Blue( ) const
{
    return m_blue;
}

//=============================================================================

inline
const uint8_t * 
Color3B::Array( ) const
{
    return  &m_red;
}

//.............................................................................

inline
uint8_t * 
Color3B::Array( )
{
    return  &m_red;
}

//=============================================================================

inline
bool 
Color3B::operator==( const Color3B & rhs ) const
{
    return ((m_red == rhs.m_red) && (m_green == rhs.m_green)
            && (m_blue == rhs.m_blue));
}

//-----------------------------------------------------------------------------

inline
bool 
Color3B::operator!=( const Color3B & rhs ) const
{
    return ! (*this == rhs);
}


//#############################################################################


inline 
Color4B::Color4B( )
{
}

//.............................................................................

inline
Color4B::Color4B( int red, int green, int blue, int alpha )
{
    Set( red, green, blue, alpha );
}

//.............................................................................

inline
Color4B::Color4B( const Color3B & color3B, int alpha )
{
    Set( color3B, alpha );
}

//.............................................................................

inline
Color4B::Color4B( const Color3F & color3F, float alpha )
{
    Set( color3F, alpha );
}

//.............................................................................

inline
Color4B::Color4B( const Color4F & color4F )
{
    Set( color4F );
}

//-----------------------------------------------------------------------------

inline
void 
Color4B::Set( int red, int green, int blue, int alpha )
{
    m_red = (uint8_t) red;
    m_green = (uint8_t) green;
    m_blue = (uint8_t) blue;
    m_alpha = (uint8_t) alpha;
}

//.............................................................................

inline 
void 
Color4B::Set( const Color3B & color3B, int alpha )
{
    Set( color3B.m_red, color3B.m_green, color3B.m_blue, alpha );
}

//.............................................................................

inline 
void 
Color4B::Set( const Color3F & color3F, float alpha )
{
    m_red = (uint8_t)( color3F.m_red * 255  +  0.5f );
    m_green = (uint8_t)( color3F.m_green * 255  +  0.5f );
    m_blue = (uint8_t)( color3F.m_blue * 255  +  0.5f );
    m_alpha = (uint8_t)( alpha * 255  +  0.5f );
}

//.............................................................................

inline
void 
Color4B::Set( const Color4F & color4F )
{
    m_red = (uint8_t)( color4F.m_red * 255  +  0.5f );
    m_green = (uint8_t)( color4F.m_green * 255  +  0.5f );
    m_blue = (uint8_t)( color4F.m_blue * 255  +  0.5f );
    m_alpha = (uint8_t)( color4F.m_alpha * 255  +  0.5f );
}

//=============================================================================

inline
int 
Color4B::Red( ) const
{
    return m_red;
}

//-----------------------------------------------------------------------------

inline
int 
Color4B::Green( ) const
{
    return m_green;
}

//-----------------------------------------------------------------------------

inline
int 
Color4B::Blue( ) const
{
    return m_blue;
}

//-----------------------------------------------------------------------------

inline
int 
Color4B::Alpha( ) const
{
    return m_alpha;
}

//=============================================================================

inline
const uint8_t * 
Color4B::Array( ) const
{
    return  &m_red;
}

//.............................................................................

inline
uint8_t * 
Color4B::Array( )
{
    return  &m_red;
}

//=============================================================================

inline
bool 
Color4B::operator==( const Color4B & rhs ) const
{
    return ((m_red == rhs.m_red) && (m_green == rhs.m_green)
            && (m_blue == rhs.m_blue) && (m_alpha == rhs.m_alpha));
}

//-----------------------------------------------------------------------------

inline
bool 
Color4B::operator!=( const Color4B & rhs ) const
{
    return ! (*this == rhs);
}


//#############################################################################


inline
Color3F::Color3F( )
{
}

//.............................................................................

inline
Color3F::Color3F( float red, float green, float blue )
{
    Set( red, green, blue );
}

//.............................................................................

inline
Color3F::Color3F( const Color3B & color3B )
{
    Set( color3B );
}

//.............................................................................

inline 
Color3F::Color3F( const Color4F & color4F )
{
    Set( color4F );
}

//-----------------------------------------------------------------------------

inline
void 
Color3F::Set( float red, float green, float blue )
{
    m_red = red;
    m_green = green;
    m_blue = blue;
}

//.............................................................................

inline
void 
Color3F::Set( const Color3B & color3B )
{
    m_red = (float) color3B.m_red / 255.f;
    m_green = (float) color3B.m_green / 255.f;
    m_blue = (float) color3B.m_blue / 255.f;
}

//.............................................................................

inline 
void 
Color3F::Set( const Color4F & color4F )
{
    m_red = color4F.m_red;
    m_green = color4F.m_green;
    m_blue = color4F.m_blue;
}

//=============================================================================

inline
float 
Color3F::Red( ) const
{
    return m_red;
}

//-----------------------------------------------------------------------------

inline
float 
Color3F::Green( ) const
{
    return m_green;
}

//-----------------------------------------------------------------------------

inline
float 
Color3F::Blue( ) const
{
    return m_blue;
}

//=============================================================================

inline
const float * 
Color3F::Array( ) const
{
    return  &m_red;
}

//.............................................................................

inline
float * 
Color3F::Array( )
{
    return  &m_red;
}

//=============================================================================

inline
bool 
Color3F::operator==( const Color3F & rhs ) const
{
    return ((m_red == rhs.m_red) && (m_green == rhs.m_green)
            && (m_blue == rhs.m_blue));
}

//-----------------------------------------------------------------------------

inline
bool 
Color3F::operator!=( const Color3F & rhs ) const
{
    return ! (*this == rhs);
}


//#############################################################################


inline
Color4F::Color4F( )
{
}

//.............................................................................

inline
Color4F::Color4F( float red, float green, float blue, float alpha )
{
    Set( red, green, blue, alpha );
}

//.............................................................................

inline
Color4F::Color4F( const Color3F & color3F, float alpha )
{
    Set( color3F, alpha );
}

//.............................................................................

inline
Color4F::Color4F( const Color4B & color4B )
{
    Set( color4B );
}

//-----------------------------------------------------------------------------

inline
void 
Color4F::Set( float red, float green, float blue, float alpha )
{
    m_red = red;
    m_green = green;
    m_blue = blue;
    m_alpha = alpha;
}

//.............................................................................

inline 
void 
Color4F::Set( const Color3F & color3F, float alpha )
{
    Set( color3F.m_red, color3F.m_green, color3F.m_blue, alpha );
}

//.............................................................................

inline
void 
Color4F::Set( const Color4B & color4B )
{
    m_red = (float) color4B.m_red / 255.f;
    m_green = (float) color4B.m_green / 255.f;
    m_blue = (float) color4B.m_blue / 255.f;
    m_alpha = (float) color4B.m_alpha / 255.f;
}

//=============================================================================

inline
float 
Color4F::Red( ) const
{
    return m_red;
}

//-----------------------------------------------------------------------------

inline
float 
Color4F::Green( ) const
{
    return m_green;
}

//-----------------------------------------------------------------------------

inline
float 
Color4F::Blue( ) const
{
    return m_blue;
}

//-----------------------------------------------------------------------------

inline
float 
Color4F::Alpha( ) const
{
    return m_alpha;
}

//=============================================================================

inline
const float * 
Color4F::Array( ) const
{
    return  &m_red;
}

//.............................................................................

inline
float * 
Color4F::Array( )
{
    return  &m_red;
}

//=============================================================================

inline
bool 
Color4F::operator==( const Color4F & rhs ) const
{
    return ((m_red == rhs.m_red) && (m_green == rhs.m_green)
            && (m_blue == rhs.m_blue) && (m_alpha == rhs.m_alpha));
}

//-----------------------------------------------------------------------------

inline
bool 
Color4F::operator!=( const Color4F & rhs ) const
{
    return ! (*this == rhs);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //COLOR_HPP

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
/*
  Rectangle.hpp
  Copyright (C) 2007 David M. Anderson

  Rectangle class: representing an axis-aligned rectangle.
*/


#include "Region.hpp"
#include "Color.hpp"
#include "Point2.hpp"
#include "Assert.hpp"
#ifdef USE_SDL
#include <SDL.h>
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename Pxl > class DrawingSurface;
class Surface;


//*****************************************************************************


class Rectangle
    :   public Region
{
public:
    Rectangle( );
    Rectangle( int left, int top, int width, int height );
    Rectangle( const Point2I & pt1, const Point2I & pt2 );
#if USE_SDL
    Rectangle( const SDL_Rect & sdlRect );
#endif
    void Set( int left, int top, int width, int height );
    void Set( const Point2I & pt1, const Point2I & pt2 );
#if USE_SDL
    void Set( const SDL_Rect & sdlRect );
#endif
    void SetLeft( int left );
    void SetTop( int top );
    void SetWidth( int width );
    void SetHeight( int height );
    int Left( ) const;
    int Right( ) const;
    int Top( ) const;
    int Bottom( ) const;
    int Width( ) const;
    int Height( ) const;
    Point2I TopLeft( ) const;
    Point2I TopRight( ) const;
    Point2I BottomLeft( ) const;
    Point2I BottomRight( ) const;
    bool Empty( ) const;
    int Area( ) const;
    bool operator==( const Rectangle & rhs ) const;
    bool operator!=( const Rectangle & rhs ) const;
    virtual bool operator==( const Region & rhs ) const;
#if USE_SDL
    operator SDL_Rect( ) const;
#endif
    virtual bool Contains( const Point2I & pt ) const;
    bool Contains( const Rectangle & rect ) const;
    bool Intersects( const Rectangle & rect ) const;
    bool Clip( const Rectangle & rect );
    template < typename Pxl >
    void Draw( Pxl pxl,
               DrawingSurface< Pxl > * pDrawingSurface ) const;
    template < typename Pxl >
    void Draw( const Color3B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const;
    template < typename Pxl >
    void Draw( const Color4B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const;
    template < typename Pxl >
    void Fill( Pxl pxl,
               DrawingSurface< Pxl > * pDrawingSurface ) const;
    template < typename Pxl >
    void Fill( const Color3B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const;
    template < typename Pxl >
    void Fill( const Color4B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const;
    void Draw( uint32_t pxl, Surface * pSurface = 0 ) const;
    void Draw( const Color3B & color, Surface * pSurface = 0 ) const;
    void Draw( const Color4B & color, Surface * pSurface = 0 ) const;
    void Fill( uint32_t pxl, Surface * pSurface = 0 ) const;
    void Fill( const Color3B & color, Surface * pSurface = 0 ) const;
    void Fill( const Color4B & color, Surface * pSurface = 0 ) const;

#ifdef DEBUG
    static bool Test( );
    static void TestDraw( );
#endif


private:
    int m_left;
    int m_top;
    int m_width;
    int m_height;
};

//.............................................................................

Rectangle Intersection( const Rectangle & rect1, const Rectangle & rect2 );
Rectangle BoundingRectangle( const Rectangle & rect1,
                             const Rectangle & rect2 );


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#include "Point2.hpp"
#include "LineSegment2.hpp"
#include "DrawingSurface.hpp"

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


inline 
void 
Rectangle::Set( int left, int top, int width, int height )
{
    m_left = left;
    m_top = top;
    m_width = width;
    m_height = height;
    Assert( width >= 0 );
    Assert( height >= 0 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#if USE_SDL

inline
void 
Rectangle::Set( const SDL_Rect & sdlRect )
{
    m_left = sdlRect.x;
    m_top = sdlRect.y;
    m_width = sdlRect.w;
    m_height = sdlRect.h;
}

#endif

//=============================================================================

inline 
Rectangle::Rectangle( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Rectangle::Rectangle( int left, int top, int width, int height )
{
    Set( left, top, width, height );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

#if USE_SDL

inline
Rectangle::Rectangle( const SDL_Rect & sdlRect )
{
    Set( sdlRect );
}

#endif

//=============================================================================

inline 
void 
Rectangle::SetLeft( int left )
{
    m_left = left;
}

//-----------------------------------------------------------------------------

inline
void 
Rectangle::SetTop( int top )
{
    m_top = top;
}

//-----------------------------------------------------------------------------

inline
void 
Rectangle::SetWidth( int width )
{
    m_width = width;
    Assert( width >= 0 );
}

//-----------------------------------------------------------------------------

inline
void 
Rectangle::SetHeight( int height )
{
    m_height = height;
    Assert( height >= 0 );
}

//=============================================================================

inline 
int 
Rectangle::Left( ) const
{
    return m_left;
}

//-----------------------------------------------------------------------------

inline
int 
Rectangle::Right( ) const
{
    return  (m_left + m_width - 1);
}

//-----------------------------------------------------------------------------

inline
int 
Rectangle::Top( ) const
{
    return m_top;
}

//-----------------------------------------------------------------------------

inline
int 
Rectangle::Bottom( ) const
{
    return  (m_top + m_height - 1);
}

//-----------------------------------------------------------------------------

inline
int 
Rectangle::Width( ) const
{
    return m_width;
}

//-----------------------------------------------------------------------------

inline
int 
Rectangle::Height( ) const
{
    return m_height;
}

//-----------------------------------------------------------------------------

inline 
Point2I 
Rectangle::TopLeft( ) const
{
    return Point2I( m_left, m_top );
}

//-----------------------------------------------------------------------------

inline 
Point2I 
Rectangle::TopRight( ) const
{
    return Point2I( Right(), m_top );
}

//-----------------------------------------------------------------------------

inline 
Point2I 
Rectangle::BottomLeft( ) const
{
    return Point2I( m_left, Bottom() );
}

//-----------------------------------------------------------------------------

inline 
Point2I 
Rectangle::BottomRight( ) const
{
    return Point2I( Right(), Bottom() );
}

//=============================================================================

inline 
bool 
Rectangle::Empty( ) const
{
    return ((m_width == 0) || (m_height == 0));
}

//=============================================================================

inline 
int 
Rectangle::Area( ) const
{
    return  m_width * m_height;
}

//=============================================================================

inline 
bool 
Rectangle::operator==( const Rectangle & rhs ) const
{
    return ( (m_left == rhs.m_left) && (m_top == rhs.m_top)
            && (m_width == rhs.m_width) && (m_height == rhs.m_height) );
}

//-----------------------------------------------------------------------------

inline 
bool 
Rectangle::operator!=( const Rectangle & rhs ) const
{
    return ! (*this == rhs);
}

//=============================================================================

#if USE_SDL

inline
Rectangle::operator SDL_Rect( ) const
{
    SDL_Rect sdlRect;
    sdlRect.x = static_cast< Sint16 >( m_left );
    sdlRect.y = static_cast< Sint16 >( m_top );
    sdlRect.w = static_cast< Uint16 >( m_width );
    sdlRect.h = static_cast< Uint16 >( m_height );
    return sdlRect;
}

#endif

//=============================================================================

inline 
bool 
Rectangle::Contains( const Point2I & pt ) const
{
    return ( (m_left <= pt.X()) && (pt.X() <= Right())
            && (m_top <= pt.Y()) && (pt.Y() <= Bottom()) );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
bool 
Rectangle::Contains( const Rectangle & rect ) const
{
    return ( (m_left <= rect.m_left) && (rect.Right() <= Right())
            && (m_top <= rect.m_top) && (rect.Bottom() <= Bottom()) );
}

//=============================================================================

template < typename Pxl >
void 
Rectangle::Draw( Pxl pxl, DrawingSurface< Pxl > * pDrawingSurface ) const
{
#if 0
    LineSegment2I( Left(), Top(), Width(), 0 ).Draw( pxl, pDrawingSurface );
    LineSegment2I( Left(), Top(), 0, Height() ).Draw( pxl, pDrawingSurface );
    LineSegment2I( Left(), Bottom(), Width(), 0 ).Draw( pxl, pDrawingSurface );
    LineSegment2I( Right(), Top(), 0, Height() ).Draw( pxl, pDrawingSurface );
#else
    int pitch = pDrawingSurface->Pitch();
    LineSegment2I ln( Left(), Top(), Width(), 0 );
    if ( ln.Clip( pDrawingSurface->ClippingRect() ) )
    {
        Pxl * pPxl = pDrawingSurface->PixelAt( ln.Endpoints().first );
        for ( int i = 0; i < ln.DX(); ++i )
            *pPxl++ = pxl;
    }
    ln.Set( Left(), Top(), 0, Height() );
    if ( ln.Clip( pDrawingSurface->ClippingRect() ) )
    {
        Pxl * pPxl = pDrawingSurface->PixelAt( ln.Endpoints().first );
        for ( int i = 0; i < ln.DY(); ++i )
        {
            *pPxl = pxl;
            pPxl += pitch;
        }
    }
    ln.Set( Left(), Bottom(), Width(), 0 );
    if ( ln.Clip( pDrawingSurface->ClippingRect() ) )
    {
        Pxl * pPxl = pDrawingSurface->PixelAt( ln.Endpoints().first );
        for ( int i = 0; i < ln.DX(); ++i )
            *pPxl++ = pxl;
    }
    ln.Set( Right(), Top(), 0, Height() );
    if ( ln.Clip( pDrawingSurface->ClippingRect() ) )
    {
        Pxl * pPxl = pDrawingSurface->PixelAt( ln.Endpoints().first );
        for ( int i = 0; i < ln.DY(); ++i )
        {
            *pPxl = pxl;
            pPxl += pitch;
        }
    }
#endif
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Rectangle::Draw( const Color3B & color,
                 DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Rectangle::Draw( const Color4B & color,
                 DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//-----------------------------------------------------------------------------

template < typename Pxl >
void 
Rectangle::Fill( Pxl pxl, DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Rectangle rect = *this;
    if ( ! rect.Clip( pDrawingSurface->ClippingRect() ) )
        return;

    Pxl * pLeft = pDrawingSurface->PixelAt( rect.TopLeft() );
    for ( int y = 0; y < rect.Height(); ++y )
    {
        Pxl * pPxl = pLeft;
        for ( int x = 0; x < rect.Width(); ++x )
            *pPxl++ = pxl;
        pLeft += pDrawingSurface->Pitch();
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Rectangle::Fill( const Color3B & color,
                 DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Fill( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Rectangle::Fill( const Color4B & color,
                 DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Fill( pxl, pDrawingSurface );
}

//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //RECTANGLE_HPP

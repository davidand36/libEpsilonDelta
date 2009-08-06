#ifndef DOT2_HPP
#define DOT2_HPP
/*
  Dot2.hpp
  Copyright (C) 2007 David M. Anderson

  Dot2I class: Two-dimensional dot, i.e. graphical representation of a point.
  NOTES:
  1. Coordinates are integers.
*/


#include "Point2.hpp"
#include "Color.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class Rectangle;
template < typename Pxl > class DrawingSurface;
class Surface;

//*****************************************************************************


class Dot2I
{
public:
    Dot2I( );
    Dot2I( const Point2I & pt );
    void Set( const Point2I & pt );
    Point2I & Point( );
    const Point2I & Point( ) const;
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
    void Draw( uint32_t pxl, Surface * pSurface = 0 ) const;
    void Draw( const Color3B & color, Surface * pSurface = 0 ) const;
    void Draw( const Color4B & color, Surface * pSurface = 0 ) const;

#ifdef DEBUG
    static bool Test( );
    static void TestDraw( );
#endif

private:
    Point2I m_coords;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#include "DrawingSurface.hpp"

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


inline 
Dot2I::Dot2I( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Dot2I::Dot2I( const Point2I & pt )
    :   m_coords( pt )
{
}

//=============================================================================

inline 
void 
Dot2I::Set( const Point2I & pt )
{
    m_coords = pt;
}

//=============================================================================

inline 
Point2I & 
Dot2I::Point( )
{
    return m_coords;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
const Point2I & 
Dot2I::Point( ) const
{
    return m_coords;
}

//=============================================================================

inline 
bool 
Dot2I::Clip( const Rectangle & rect )
{
    return  rect.Contains( m_coords );
}

//=============================================================================

template < typename Pxl >
void 
Dot2I::Draw( Pxl pxl, DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Dot2I dot = *this;
    Point2I pt = Point();
    if ( ! dot.Clip( pDrawingSurface->ClippingRect() ) )
        return;
    Pxl * pos = pDrawingSurface->PixelAt( pt );
    *pos = pxl;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Dot2I::Draw( const Color3B & color,
             DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Dot2I::Draw( const Color4B & color,
             DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}


/*****************************************************************************/

}                                                      //namespace EpsilonDelta

#endif //DOT2_HPP

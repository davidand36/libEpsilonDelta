#ifndef POINT2_HPP
#define POINT2_HPP
/*
  Point2.hpp
  Copyright (C) 2007 David M. Anderson

  Point2I class: Two-dimensional point.
  NOTES:
  1. Coordinates are integers. Typically used for screen coordinates and such.
*/


#include "Vector2.hpp"
#include "Color.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

class Rectangle;
template < typename Pxl > class DrawingSurface;
class Surface;

//*****************************************************************************


class Point2I
{
public:
    Point2I( );
    Point2I( int x, int y );
    Point2I( int * pCoords );
    Point2I( const Vector2I & vec );
    void Set( int x, int y );
    void Set( int * pCoords );
    void Set( const Vector2I & vec );
    int X( ) const;
    int Y( ) const;
    void SetX( int x );
    void SetY( int y );
    const int & operator[]( int index ) const;
    int & operator[]( int index );
    const int & At( int index ) const;
    int & At( int index );
    Vector2I & Vector( );
    const Vector2I & Vector( ) const;
    bool operator==( const Point2I & rhs ) const;
    bool operator!=( const Point2I & rhs ) const;
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
    Vector2I m_coords;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#include "DrawingSurface.hpp"

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


inline 
Point2I::Point2I( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Point2I::Point2I( int x, int y )
    :   m_coords( x, y )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Point2I::Point2I( const Vector2I & vec )
    :   m_coords( vec )
{
}

//=============================================================================

inline 
void 
Point2I::Set( int x, int y )
{
    m_coords.Set( x, y );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Point2I::Set( const Vector2I & vec )
{
    m_coords = vec;
}

//=============================================================================

inline 
int 
Point2I::X( ) const
{
    return  m_coords.X();
}

//-----------------------------------------------------------------------------

inline 
int 
Point2I::Y( ) const
{
    return  m_coords.Y();
}

//=============================================================================

inline
void 
Point2I::SetX( int x )
{
    m_coords.SetX( x );
}

//-----------------------------------------------------------------------------

inline
void 
Point2I::SetY( int y )
{
    m_coords.SetY( y );
}

//=============================================================================

inline 
const int & 
Point2I::operator[]( int index ) const
{
    return  m_coords[ index ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
int & 
Point2I::operator[]( int index )
{
    return  m_coords[ index ];
}

//=============================================================================

inline 
Vector2I & 
Point2I::Vector( )
{
    return m_coords;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
const Vector2I & 
Point2I::Vector( ) const
{
    return m_coords;
}

//=============================================================================

inline 
bool 
Point2I::operator==( const Point2I & rhs ) const
{
    return  (m_coords == rhs.m_coords);
}

//-----------------------------------------------------------------------------

inline 
bool 
Point2I::operator!=( const Point2I & rhs ) const
{
    return  (m_coords != rhs.m_coords);
}

//=============================================================================

inline 
bool 
Point2I::Clip( const Rectangle & rect )
{
    return  rect.Contains( *this );
}

//=============================================================================

template < typename Pxl >
void 
Point2I::Draw( Pxl pxl, DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Point2I pt = *this;
    if ( ! pt.Clip( pDrawingSurface->ClippingRect() ) )
        return;
    Pxl * pos = pDrawingSurface->PixelAt( pt );
    *pos = pxl;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Point2I::Draw( const Color3B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Point2I::Draw( const Color4B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}


/*****************************************************************************/

}                                                      //namespace EpsilonDelta

#endif //POINT2_HPP

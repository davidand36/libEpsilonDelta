#ifndef LINESEGMENT2_HPP
#define LINESEGMENT2_HPP
/*
  LineSegment2.hpp
  Copyright (C) 2007 David M. Anderson

  LineSegment2I class: Two-dimensional line segment.
  NOTES:
  1. Coordinates are integers. Typically used for screen coordinates and such.
*/


#include "Point2.hpp"
#include "Color.hpp"
#include <utility>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Rectangle;
template < typename Pxl > class DrawingSurface;
class Surface;


//*****************************************************************************


class LineSegment2I
{
public:
    LineSegment2I( );
    LineSegment2I( int x, int y, int dx, int dy );
    LineSegment2I( const Point2I & pt0, const Point2I & pt1 );
    void Set( int x, int y, int dx, int dy );
    void Set( const Point2I & pt0, const Point2I & pt1 );
    int X( ) const;
    int Y( ) const;
    int DX( ) const;
    int DY( ) const;
    void SetX( int x );
    void SetY( int y );
    void SetDX( int dx );
    void SetDY( int dy );
    typedef std::pair< Point2I, Point2I >  PointPair;
    PointPair Endpoints( ) const;
    bool operator==( const LineSegment2I & rhs ) const;
    bool operator!=( const LineSegment2I & rhs ) const;
    int LengthSquared( ) const;
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
    void Fixup( );

    int m_x;
    int m_y;
    int m_dx;
    int m_dy;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#include "DrawingSurface.hpp"

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


inline
void 
LineSegment2I::Fixup( )
{
    if ( m_dx < 0 )
    {
        m_x += m_dx;
        m_y += m_dy;
        m_dx = - m_dx;
        m_dy = - m_dy;
    }
    else if ( (m_dx == 0) && (m_dy < 0) )
    {
        m_y += m_dy;
        m_dy = - m_dy;
    }
}

//=============================================================================

inline 
void 
LineSegment2I::Set( int x, int y, int dx, int dy )
{
    m_x = x;
    m_y = y;
    m_dx = dx;
    m_dy = dy;
    Fixup( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
LineSegment2I::Set( const Point2I & pt0, const Point2I & pt1 )
{
    m_x = pt0.X();
    m_y = pt0.Y();
    m_dx = pt1.X() - pt0.X();
    m_dy = pt1.Y() - pt0.Y();
    Fixup( );
}

//=============================================================================

inline 
LineSegment2I::LineSegment2I( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
LineSegment2I::LineSegment2I( int x, int y, int dx, int dy )
    :   m_x( x ),
        m_y( y ),
        m_dx( dx ),
        m_dy( dy )
{
    Fixup( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
LineSegment2I::LineSegment2I( const Point2I & pt0, const Point2I & pt1 )
{
    Set( pt0, pt1 );
}

//=============================================================================

inline 
int 
LineSegment2I::X( ) const
{
    return m_x;
}

//-----------------------------------------------------------------------------

inline
int
LineSegment2I::Y( ) const
{
    return m_y;
}

//-----------------------------------------------------------------------------

inline 
int 
LineSegment2I::DX( ) const
{
    return m_dx;
}

//-----------------------------------------------------------------------------

inline
int 
LineSegment2I::DY( ) const
{
    return m_dy;
}

//=============================================================================

inline
void 
LineSegment2I::SetX( int x )
{
    m_x = x;
}

//-----------------------------------------------------------------------------

inline 
void 
LineSegment2I::SetY( int y )
{
    m_y = y;
}

//-----------------------------------------------------------------------------

inline
void 
LineSegment2I::SetDX( int dx )
{
    m_dx = dx;
    Fixup( );
}

//-----------------------------------------------------------------------------

inline
void 
LineSegment2I::SetDY( int dy )
{
    m_dy = dy;
    Fixup( );
}

//=============================================================================

inline 
LineSegment2I::PointPair  
LineSegment2I::Endpoints( ) const
{
    Point2I pt0( m_x, m_y );
    Point2I pt1( m_x + m_dx, m_y + m_dy );
    return PointPair( pt0, pt1 );
}

//=============================================================================

inline 
bool 
LineSegment2I::operator==( const LineSegment2I & rhs ) const
{
    return  ((m_x == rhs.m_x) && (m_y == rhs.m_y)
             && (m_dx == rhs.m_dx) && (m_dy == rhs.m_dy));
}

//-----------------------------------------------------------------------------

inline
bool 
LineSegment2I::operator!=( const LineSegment2I & rhs ) const
{
    return ! (*this == rhs);
}

//=============================================================================

inline
int 
LineSegment2I::LengthSquared( ) const
{
    return  (m_dx * m_dx  +  m_dy * m_dy);
}

//=============================================================================

template < typename Pxl >
void 
LineSegment2I::Draw( Pxl pxl, DrawingSurface< Pxl > * pDrawingSurface ) const
{                                                                      /*Draw*/
    LineSegment2I ln = *this;
    if ( ! ln.Clip( pDrawingSurface->ClippingRect() ) )
        return;

    Pxl * pPxl = pDrawingSurface->PixelAt( ln.Endpoints().first );

    if ( ln.m_dy == 0 )                 //horizontal
    {
        Assert( ln.m_dx >= 0 );
        for ( int i = 0; i < ln.m_dx; ++i )
            *pPxl++ = pxl;
    }
    else if ( ln.m_dx == 0 )            //vertical
    {
        Assert( ln.m_dy >= 0 );
        int pitch = pDrawingSurface->Pitch();
        for ( int i = 0; i < ln.m_dy; ++i )
        {
            *pPxl = pxl;
            pPxl += pitch;
        }
    }
    else
    {
        int pitch;
        Assert( ln.m_dx >= 0 );
        if ( ln.m_dy > 0 )
        {
            pitch = pDrawingSurface->Pitch();
        }
        else
        {
            ln.m_dy = - ln.m_dy;
            pitch = - pDrawingSurface->Pitch();
        }
        if ( ln.m_dy <= ln.m_dx )       //low slope
        {
            int incr1 = 2 * ln.m_dy;
            int d = incr1 - ln.m_dx;
            int incr2 = d - ln.m_dx;
            for ( int i = 0; i < ln.m_dx; ++i )
            {
                *pPxl++ = pxl;
                if ( d < 0 )
                    d += incr1;
                else
                {
                    d += incr2;
                    pPxl += pitch;
                }
            }
        }
        else                            //high slope
        {
            int incr1 = 2 * ln.m_dx;
            int d = incr1 - ln.m_dy;
            int incr2 = d - ln.m_dy;
            for ( int i = 0; i < ln.m_dy; ++i )
            {
                *pPxl = pxl;
                pPxl += pitch;
                if ( d < 0 )
                    d += incr1;
                else
                {
                    d += incr2;
                    ++pPxl;
                }
            }
        }
    }
}                                                                      /*Draw*/

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
LineSegment2I::Draw( const Color3B & color,
                     DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
LineSegment2I::Draw( const Color4B & color,
                     DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //LINE2_HPP

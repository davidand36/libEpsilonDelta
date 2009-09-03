#ifndef ELLIPSE_HPP
#define ELLIPSE_HPP
/*
  Ellipse.hpp
  Copyright (C) 2007 David M. Anderson

  Ellipse class: representing an axis-aligned ellipse.
*/


#include "Region.hpp"
#include "Point2.hpp"
#include "Rectangle.hpp"
#include "Dot2.hpp"
#include "Color.hpp"
#include "Assert.hpp"
#include <utility>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Ellipse
    :   public Region
{
public:
    Ellipse( );
    Ellipse( const Point2I & center, int semiWidth, int semiHeight );
    Ellipse( const Rectangle & bounds );
    void Set( const Point2I & center, int semiWidth, int semiHeight );
    void Set( const Rectangle & bounds );
    void SetCenter( Point2I & center );
    void SetAxes( int semiWidth, int semiHeight );
    Point2I Center( ) const;
    typedef std::pair< int, int > AxisPair;
    AxisPair SemiAxes( ) const;
    Rectangle Bounds( ) const;
    virtual bool Contains( const Point2I & point ) const;
    bool operator==( const Ellipse & rhs ) const;
    bool operator!=( const Ellipse & rhs ) const;
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
    template < typename Pxl >
    void Draw4Pixels( Pxl pxl, int x, int y,
                      DrawingSurface< Pxl > * pDrawingSurface ) const;
    
    Point2I m_center;
    int m_semiWidth;
    int m_semiHeight;
};


//*****************************************************************************


inline 
void 
Ellipse::Set( const Point2I & center, int semiWidth, int semiHeight )
{
    m_center = center;
    m_semiWidth = semiWidth;
    m_semiHeight = semiHeight;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
void 
Ellipse::Set( const Rectangle & bounds )
{
    m_semiWidth = bounds.Width() / 2;
    m_semiHeight = bounds.Height() / 2;
    m_center.Set( bounds.Left() + m_semiWidth, bounds.Top() + m_semiHeight );
}

//=============================================================================

inline 
Ellipse::Ellipse( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Ellipse::Ellipse( const Point2I & center, int semiWidth, int semiHeight )
{
    Set( center, semiWidth, semiHeight );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
Ellipse::Ellipse( const Rectangle & bounds )
{
    Set( bounds );
}

//=============================================================================

inline 
void 
Ellipse::SetCenter( Point2I & center )
{
    m_center = center;
}

//-----------------------------------------------------------------------------

inline 
void 
Ellipse::SetAxes( int semiWidth, int semiHeight )
{
    m_semiWidth = semiWidth;
    m_semiHeight = semiHeight;
}

//=============================================================================

inline 
Point2I 
Ellipse::Center( ) const
{
    return m_center;
}

//-----------------------------------------------------------------------------

inline 
Ellipse::AxisPair 
Ellipse::SemiAxes( ) const
{
    return AxisPair( m_semiWidth, m_semiHeight );
}

//-----------------------------------------------------------------------------

inline 
Rectangle 
Ellipse::Bounds( ) const
{
    return Rectangle( m_center.X() - m_semiWidth,
                      m_center.Y() - m_semiHeight,
                      2 * m_semiWidth + 1,
                      2 * m_semiHeight + 1 );
}

//=============================================================================

inline
bool 
Ellipse::operator==( const Ellipse & rhs ) const
{
    return  (m_center == rhs.m_center) && (m_semiWidth == rhs.m_semiWidth)
            && (m_semiHeight == rhs.m_semiHeight);
}

//-----------------------------------------------------------------------------

inline
bool 
Ellipse::operator!=( const Ellipse & rhs ) const
{
    return  ! (*this == rhs);
}

//=============================================================================

template < typename Pxl >
void 
Ellipse::Draw( Pxl pxl, DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Rectangle bounds = Bounds();
    if ( (bounds.Width() == 1) || (bounds.Height() == 1) )  //degenerate
    {
        LineSegment2I line( bounds.TopLeft(), bounds.BottomRight() );
        line.Draw( pxl, pDrawingSurface );
        return;
    }
    Rectangle clippingRect = pDrawingSurface->ClippingRect();
    if ( clippingRect.Contains( bounds ) )                  //not clipped
    {
        Point2I pt;
        pt.Set( m_center.X(), m_center.Y() + m_semiHeight );
        Pxl * pPxl0 = pDrawingSurface->PixelAt( pt );
        Pxl * pPxl1 = pPxl0;
        pt.Set( m_center.X(), m_center.Y() - m_semiHeight );
        Pxl * pPxl2 = pDrawingSurface->PixelAt( pt );
        Pxl * pPxl3 = pPxl2;
        int pitch = pDrawingSurface->Pitch();
        int y = m_semiHeight;
        const int A = m_semiWidth;
        const int B = y;
        const int ASq = A * A;
        const int ASq2 = 2 * ASq;
        const int BSq = B * B;
        const int BSq2 = 2 * BSq;
        int d = BSq  -  ASq * B  +  ASq / 4;
        int dx = 0;
        int dy = ASq2 * B;

        while ( dx < dy )
        {
            *pPxl0 = *pPxl1 = *pPxl2 = *pPxl3 = pxl;

            if ( d > 0 )
            {
                pPxl0 -= pitch;
                pPxl1 -= pitch;
                pPxl2 += pitch;
                pPxl3 += pitch;
                --y;
                dy -= ASq2;
                d -= dy;
            }
            ++pPxl0;
            --pPxl1;
            ++pPxl2;
            --pPxl3;
            dx += BSq2;
            d += BSq + dx;
        }

        d += (3 * (ASq - BSq) / 2  -  (dx + dy)) / 2;

        while ( y >= 0 )
        {
            *pPxl0 = *pPxl1 = *pPxl2 = *pPxl3 = pxl;

            if ( d < 0 )
            {
                ++pPxl0;
                --pPxl1;
                ++pPxl2;
                --pPxl3;
                dx += BSq2;
                d += dx;
            }
            pPxl0 -= pitch;
            pPxl1 -= pitch;
            pPxl2 += pitch;
            pPxl3 += pitch;
            --y;
            dy -= ASq2;
            d += ASq - dy;
        }
    }
    else                                                    //clipped
    {
        int x = 0;
        int y = m_semiHeight;
        const int A = m_semiWidth;
        const int B = y;
        const int ASq = A * A;
        const int ASq2 = 2 * ASq;
        const int BSq = B * B;
        const int BSq2 = 2 * BSq;
        int d = BSq  -  ASq * B  +  ASq / 4;
        int dx = 0;
        int dy = ASq2 * B;

        while ( dx < dy )
        {
            Draw4Pixels( pxl, x, y, pDrawingSurface );

            if ( d > 0 )
            {
                --y;
                dy -= ASq2;
                d -= dy;
            }
            ++x;
            dx += BSq2;
            d += BSq + dx;
        }

        d += (3 * (ASq - BSq) / 2  -  (dx + dy)) / 2;

        while ( y >= 0 )
        {
            Draw4Pixels( pxl, x, y, pDrawingSurface );

            if ( d < 0 )
            {
                ++x;
                dx += BSq2;
                d += dx;
            }
            --y;
            dy -= ASq2;
            d += ASq - dy;
        }
    }
}

//.............................................................................

template < typename Pxl >
void 
Ellipse::Draw4Pixels( Pxl pxl, int x, int y,
                      DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Dot2I dot( Point2I( m_center.X() - x,  m_center.Y() - y ) );
    dot.Draw( pxl, pDrawingSurface );
    dot.Point().Set( m_center.X() + x,  m_center.Y() - y );
    dot.Draw( pxl, pDrawingSurface );
    dot.Point().Set( m_center.X() - x,  m_center.Y() + y );
    dot.Draw( pxl, pDrawingSurface );
    dot.Point().Set( m_center.X() + x,  m_center.Y() + y );
    dot.Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Ellipse::Draw( const Color3B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Ellipse::Draw( const Color4B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//-----------------------------------------------------------------------------

template < typename Pxl >
void 
Ellipse::Fill( Pxl pxl,
               DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Rectangle bounds = Bounds();
    if ( (bounds.Width() == 1) || (bounds.Height() == 1) )  //degenerate
    {
        LineSegment2I line( bounds.TopLeft(), bounds.BottomRight() );
        line.Draw( pxl, pDrawingSurface );
        return;
    }
    int x = 0;
    int y = m_semiHeight;
    const int A = m_semiWidth;
    const int B = y;
    const int ASq = A * A;
    const int ASq2 = 2 * ASq;
    const int BSq = B * B;
    const int BSq2 = 2 * BSq;
    int d = BSq  -  ASq * B  +  ASq / 4;
    int dx = 0;
    int dy = ASq2 * B;
    LineSegment2I line;

    while ( dx < dy )
    {
        line.Set( m_center.X() - x,  m_center.Y() - y,  2 * x,  0 );
        line.Draw( pxl, pDrawingSurface );
        line.SetY( m_center.Y() + y );
        line.Draw( pxl, pDrawingSurface );

        if ( d > 0 )
        {
            --y;
            dy -= ASq2;
            d -= dy;
        }
        ++x;
        dx += BSq2;
        d += BSq + dx;
    }

    d += (3 * (ASq - BSq) / 2  -  (dx + dy)) / 2;

    while ( y >= 0 )
    {
        line.Set( m_center.X() - x,  m_center.Y() - y,  2 * x,  0 );
        line.Draw( pxl, pDrawingSurface );
        line.SetY( m_center.Y() + y );
        line.Draw( pxl, pDrawingSurface );

        if ( d < 0 )
        {
            ++x;
            dx += BSq2;
            d += dx;
        }
        --y;
        dy -= ASq2;
        d += ASq - dy;
    }
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Ellipse::Fill( const Color3B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Fill( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Ellipse::Fill( const Color4B & color,
               DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Fill( pxl, pDrawingSurface );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ELLIPSE_HPP

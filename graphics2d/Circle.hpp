#ifndef CIRCLE_HPP
#define CIRCLE_HPP
/*
  Circle.hpp
  Copyright (C) 2007 David M. Anderson

  Circle class.
*/


#include "Ellipse.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Circle
{
public:
    Circle( );
    Circle( const Point2I & center, int radius );
    void Set( const Point2I & center, int radius );
    void SetCenter( Point2I & center );
    void SetRadius( int radius );
    Point2I Center( ) const;
    int Radius( ) const;
    Rectangle Bounds( ) const;
    bool Contains( const Point2I & point ) const;
    bool operator==( const Circle & rhs ) const;
    bool operator!=( const Circle & rhs ) const;
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
    Point2I m_center;
    int m_radius;
};


//*****************************************************************************


inline
void 
Circle::Set( const Point2I & center, int radius )
{
    m_center = center;
    m_radius = radius;
}

//=============================================================================

inline
Circle::Circle( )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline
Circle::Circle( const Point2I & center, int radius )
{
    Set( center, radius );
}

//=============================================================================

inline
void 
Circle::SetCenter( Point2I & center )
{
    m_center = center;
}

//-----------------------------------------------------------------------------

inline
void 
Circle::SetRadius( int radius )
{
    m_radius = radius;
}

//=============================================================================

inline
Point2I 
Circle::Center( ) const
{
    return m_center;
}

//-----------------------------------------------------------------------------

inline
int 
Circle::Radius( ) const
{
    return m_radius;
}

//-----------------------------------------------------------------------------

inline
Rectangle 
Circle::Bounds( ) const
{
    return Rectangle( m_center.X() - m_radius,
                      m_center.Y() - m_radius,
                      2 * m_radius + 1,
                      2 * m_radius + 1 );
}

//=============================================================================

inline
bool 
Circle::Contains( const Point2I & point ) const
{
    int x = point.X() - m_center.X();
    int y = point.Y() - m_center.Y();
    return  ( x * x  +  y * y  <=  m_radius * m_radius );
}

//=============================================================================

inline
bool 
Circle::operator==( const Circle & rhs ) const
{
    return  (m_center == rhs.m_center) && (m_radius == rhs.m_radius);
}

//-----------------------------------------------------------------------------

inline
bool 
Circle::operator!=( const Circle & rhs ) const
{
    return  ! (*this == rhs);
}

//=============================================================================

template < typename Pxl >
void 
Circle::Draw( Pxl pxl,
              DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Ellipse ellipse( m_center, m_radius, m_radius );
    ellipse.Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Circle::Draw( const Color3B & color,
              DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Circle::Draw( const Color4B & color,
              DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Draw( pxl, pDrawingSurface );
}

//-----------------------------------------------------------------------------

template < typename Pxl >
void 
Circle::Fill( Pxl pxl,
              DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Ellipse ellipse( m_center, m_radius, m_radius );
    ellipse.Fill( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Circle::Fill( const Color3B & color,
              DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Fill( pxl, pDrawingSurface );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
void 
Circle::Fill( const Color4B & color,
              DrawingSurface< Pxl > * pDrawingSurface ) const
{
    Pxl pxl( color );
    Fill( pxl, pDrawingSurface );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CIRCLE_HPP

#ifndef DRAWINGSURFACE_HPP
#define DRAWINGSURFACE_HPP
/*
  DrawingSurface.hpp
  Copyright (C) 2007 David M. Anderson

  DrawingSurface template class: Represents a writable rectangle of pixels
  of a known type.
*/


#include "Pixel.hpp"
#include "Rectangle.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename Pxl >
class DrawingSurface
{
public:
    typedef Pxl  Pixel;

    DrawingSurface( Pxl * pixels, int pitch, const Rectangle & clippingRect );
    Pixel * PixelAt( const Point2I & pos );
    const Pixel * PixelAt( const Point2I & pos ) const;
    int Pitch( ) const;
    const Rectangle & ClippingRect( ) const;

private:
    Pxl * m_pixels;
    int m_pitch;
    Rectangle m_clippingRect;
};


//*****************************************************************************


template < typename Pxl >
inline 
DrawingSurface< Pxl >::DrawingSurface( Pxl * pixels, int pitch,
                                       const Rectangle & clippingRect )
    :   m_pixels( pixels ),
        m_pitch( pitch ),
        m_clippingRect( clippingRect )
{
}

//=============================================================================

template < typename Pxl >
inline 
Pxl * 
DrawingSurface< Pxl >::PixelAt( const Point2I & pos )
{
    return  m_pixels  +  pos.Y() * m_pitch  +  pos.X();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Pxl >
inline 
const Pxl * 
DrawingSurface< Pxl >::PixelAt( const Point2I & pos ) const
{
    return  m_pixels  +  pos.Y() * m_pitch  +  pos.X();
}

//=============================================================================

template < typename Pxl >
inline 
int 
DrawingSurface< Pxl >::Pitch( ) const
{
    return m_pitch;
}

//-----------------------------------------------------------------------------

template < typename Pxl >
inline 
const Rectangle & 
DrawingSurface< Pxl >::ClippingRect( ) const
{
    return m_clippingRect;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DRAWINGSURFACE_HPP

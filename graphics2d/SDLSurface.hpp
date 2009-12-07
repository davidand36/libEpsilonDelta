#ifndef SDLSURFACE_HPP
#define SDLSURFACE_HPP
/*
  SDLSurface.hpp
  Copyright (C) 2007 David M. Anderson

  For inclusion in Surface.hpp: Implementation using the Simple DirectMedia
  Layer (SDL) library.
*/


#ifdef USE_SDL


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


inline 
Surface::Surface( )
    :   m_pSDL_Surface( 0 ),
        m_own( true ),
        m_pixelType( UnknownPixelType )
{
}

//=============================================================================

inline
bool 
Surface::operator==( const Surface & rhs ) const
{
    return  (m_pSDL_Surface == rhs.m_pSDL_Surface);
}

//-----------------------------------------------------------------------------

inline
bool 
Surface::operator!=( const Surface & rhs ) const
{
    return  ! (*this == rhs);
}

//=============================================================================

inline
Rectangle 
Surface::Extent( ) const
{
    return  Rectangle( 0, 0, m_pSDL_Surface->w, m_pSDL_Surface->h );
}

//-----------------------------------------------------------------------------

inline
int 
Surface::Pitch( ) const
{
    Assert( m_pSDL_Surface->pitch
            % m_pSDL_Surface->format->BytesPerPixel == 0 );
    return  static_cast< int >( m_pSDL_Surface->pitch
                                / m_pSDL_Surface->format->BytesPerPixel );
}

//=============================================================================

inline 
EPixelType 
Surface::PixelType( ) const
{
    return  m_pixelType;
}

//=============================================================================

inline 
Rectangle 
Surface::ClippingRect( ) const
{
    return  Rectangle( m_pSDL_Surface->clip_rect );
}

//-----------------------------------------------------------------------------

inline 
void 
Surface::SetClippingRect( const Rectangle & clippingRect )
{
    ::SDL_Rect sdlRect = clippingRect;
    ::SDL_SetClipRect( m_pSDL_Surface, &sdlRect );
}

//=============================================================================

template < typename Pxl >
DrawingSurface< Pxl > 
Surface::LockDrawingSurface( )
{
    Pxl * pixels = reinterpret_cast< Pxl * >( Lock( ) );
    return DrawingSurface< Pxl >( pixels, Pitch(), ClippingRect() );
}

//.............................................................................

template < typename Pxl >
DrawingSurface< Pxl > const
Surface::LockDrawingSurface( ) const
{
    Pxl const * pixels = reinterpret_cast< Pxl const * >( Lock( ) );
    return DrawingSurface< Pxl >( const_cast< Pxl * >( pixels ),
                                  Pitch(), ClippingRect() );
}

//=============================================================================

template < typename Pxl >
bool 
Surface::Contains( const Point2I & point, uint32_t colorKey ) const
{
    DrawingSurface< Pxl > const drawSurf = LockDrawingSurface< Pxl >( );
    bool contained
            = ((uint32_t)(drawSurf.PixelAt( point )->Value()) != colorKey);
    Unlock( );
    return contained;
}

//.............................................................................

template < typename Pxl >
bool 
Surface::Contains( const Point2I & point ) const
{
    DrawingSurface< Pxl > const drawSurf = LockDrawingSurface< Pxl >( );
    bool contained = (drawSurf.PixelAt( point )->Color().Alpha() != 0);
    Unlock( );
    return contained;
}

//=============================================================================

template < typename Shape >
void
Surface::Draw( const Shape & shape, uint32_t pxl )
{
    switch ( PixelType() )
    {
    case PixelType8:
        return Draw( shape, Pixel8( (uint8_t) pxl ) );
    case PixelType555:
        return Draw( shape, Pixel555( (uint16_t) pxl ) );
    case PixelType1555:
        return Draw( shape, Pixel1555( (uint16_t) pxl ) );
    case PixelType565:
        return Draw( shape, Pixel565( (uint16_t) pxl ) );
    case PixelTypeRGB:
        return Draw( shape, PixelRGB( pxl ) );
    case PixelTypeBGR:
        return Draw( shape, PixelBGR( pxl ) );
    case PixelType0RGB:
        return Draw( shape, Pixel0RGB( pxl ) );
    case PixelType0BGR:
        return Draw( shape, Pixel0BGR( pxl ) );
    case PixelTypeRGB0:
        return Draw( shape, PixelRGB0( pxl ) );
    case PixelTypeBGR0:
        return Draw( shape, PixelBGR0( pxl ) );
    case PixelTypeARGB:
        return Draw( shape, PixelARGB( pxl ) );
    case PixelTypeABGR:
        return Draw( shape, PixelABGR( pxl ) );
    case PixelTypeRGBA:
        return Draw( shape, PixelRGBA( pxl ) );
    case PixelTypeBGRA:
        return Draw( shape, PixelBGRA( pxl ) );
    default:
        Assert( 0 && "Unexpected pixel type" );
        return;
    }
}

//.............................................................................

template < typename Shape >
void
Surface::Draw( const Shape & shape, const Color3B & color )
{
    switch ( PixelType() )
    {
    case PixelType555:
        return Draw( shape, Pixel555( color ) );
    case PixelType1555:
        return Draw( shape, Pixel1555( color ) );
    case PixelType565:
        return Draw( shape, Pixel565( color ) );
    case PixelTypeRGB:
        return Draw( shape, PixelRGB( color ) );
    case PixelTypeBGR:
        return Draw( shape, PixelBGR( color ) );
    case PixelType0RGB:
        return Draw( shape, Pixel0RGB( color ) );
    case PixelType0BGR:
        return Draw( shape, Pixel0BGR( color ) );
    case PixelTypeRGB0:
        return Draw( shape, PixelRGB0( color ) );
    case PixelTypeBGR0:
        return Draw( shape, PixelBGR0( color ) );
    case PixelTypeARGB:
        return Draw( shape, PixelARGB( color ) );
    case PixelTypeABGR:
        return Draw( shape, PixelABGR( color ) );
    case PixelTypeRGBA:
        return Draw( shape, PixelRGBA( color ) );
    case PixelTypeBGRA:
        return Draw( shape, PixelBGRA( color ) );
    default:
        Assert( 0 && "Unexpected pixel type" );
        return;
    }
}

//.............................................................................

template < typename Shape >
void
Surface::Draw( const Shape & shape, const Color4B & color )
{
    switch ( PixelType() )
    {
    case PixelType1555:
        return Draw( shape, Pixel1555( color ) );
    case PixelTypeARGB:
        return Draw( shape, PixelARGB( color ) );
    case PixelTypeABGR:
        return Draw( shape, PixelABGR( color ) );
    case PixelTypeRGBA:
        return Draw( shape, PixelRGBA( color ) );
    case PixelTypeBGRA:
        return Draw( shape, PixelBGRA( color ) );
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
}

//.............................................................................

template < typename Shape, typename Pxl >
void 
Surface::Draw( const Shape & shape, Pxl pxl )
{
    DrawingSurface< Pxl > drawSurf = LockDrawingSurface< Pxl >( );
    shape.Draw( pxl, &drawSurf );
    Unlock( );
}

//-----------------------------------------------------------------------------

template < typename Shape >
void
Surface::Fill( const Shape & shape, uint32_t pxl )
{
    switch ( PixelType() )
    {
    case PixelType8:
        return Fill( shape, Pixel8( (uint8_t) pxl ) );
    case PixelType555:
        return Fill( shape, Pixel555( (uint16_t) pxl ) );
    case PixelType1555:
        return Fill( shape, Pixel1555( (uint16_t) pxl ) );
    case PixelType565:
        return Fill( shape, Pixel565( (uint16_t) pxl ) );
    case PixelTypeRGB:
        return Fill( shape, PixelRGB( pxl ) );
    case PixelTypeBGR:
        return Fill( shape, PixelBGR( pxl ) );
    case PixelType0RGB:
        return Fill( shape, Pixel0RGB( pxl ) );
    case PixelType0BGR:
        return Fill( shape, Pixel0BGR( pxl ) );
    case PixelTypeRGB0:
        return Fill( shape, PixelRGB0( pxl ) );
    case PixelTypeBGR0:
        return Fill( shape, PixelBGR0( pxl ) );
    case PixelTypeARGB:
        return Fill( shape, PixelARGB( pxl ) );
    case PixelTypeABGR:
        return Fill( shape, PixelABGR( pxl ) );
    case PixelTypeRGBA:
        return Fill( shape, PixelRGBA( pxl ) );
    case PixelTypeBGRA:
        return Fill( shape, PixelBGRA( pxl ) );
    default:
        Assert( 0 && "Unexpected pixel type" );
        return;
    }
}

//.............................................................................

template < typename Shape >
void
Surface::Fill( const Shape & shape, const Color3B & color )
{
    switch ( PixelType() )
    {
    case PixelType555:
        return Fill( shape, Pixel555( color ) );
    case PixelType1555:
        return Fill( shape, Pixel1555( color ) );
    case PixelType565:
        return Fill( shape, Pixel565( color ) );
    case PixelTypeRGB:
        return Fill( shape, PixelRGB( color ) );
    case PixelTypeBGR:
        return Fill( shape, PixelBGR( color ) );
    case PixelType0RGB:
        return Fill( shape, Pixel0RGB( color ) );
    case PixelType0BGR:
        return Fill( shape, Pixel0BGR( color ) );
    case PixelTypeRGB0:
        return Fill( shape, PixelRGB0( color ) );
    case PixelTypeBGR0:
        return Fill( shape, PixelBGR0( color ) );
    case PixelTypeARGB:
        return Fill( shape, PixelARGB( color ) );
    case PixelTypeABGR:
        return Fill( shape, PixelABGR( color ) );
    case PixelTypeRGBA:
        return Fill( shape, PixelRGBA( color ) );
    case PixelTypeBGRA:
        return Fill( shape, PixelBGRA( color ) );
    default:
        Assert( 0 && "Unexpected pixel type" );
        return;
    }
}

//.............................................................................

template < typename Shape >
void
Surface::Fill( const Shape & shape, const Color4B & color )
{
    switch ( PixelType() )
    {
    case PixelType1555:
        return Fill( shape, Pixel1555( color ) );
    case PixelTypeARGB:
        return Fill( shape, PixelARGB( color ) );
    case PixelTypeABGR:
        return Fill( shape, PixelABGR( color ) );
    case PixelTypeRGBA:
        return Fill( shape, PixelRGBA( color ) );
    case PixelTypeBGRA:
        return Fill( shape, PixelBGRA( color ) );
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
}

//.............................................................................

template < typename Shape, typename Pxl >
void 
Surface::Fill( const Shape & shape, Pxl pxl )
{
    DrawingSurface< Pxl > drawSurf = LockDrawingSurface< Pxl >( );
    shape.Fill( pxl, &drawSurf );
    Unlock( );
}

//=============================================================================

inline 
::SDL_Surface * 
Surface::GetSDL_Surface( )
{
    return m_pSDL_Surface;
}

//.............................................................................

inline 
const ::SDL_Surface * 
Surface::GetSDL_Surface( ) const
{
    return m_pSDL_Surface;
}

//=============================================================================

inline 
void 
Surface::MakeCurrent( )
{
    ms_current = this;
}

//-----------------------------------------------------------------------------

inline 
Surface * 
Surface::Current( )
{
    return  ms_current;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //USE_SDL
#endif //SDLSURFACE_HPP

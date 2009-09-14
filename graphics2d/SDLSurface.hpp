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
    SDL_Rect sdlRect = clippingRect;
    SDL_SetClipRect( m_pSDL_Surface, &sdlRect );
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

template < typename Shape >
void
Surface::Draw( const Shape & shape, uint32_t pxl )
{
    switch ( PixelType() )
    {
    case PixelType8:
    {
        DrawingSurface< Pixel8 > drawSurf
                = LockDrawingSurface< Pixel8 >( );
        shape.Draw( Pixel8( static_cast< uint8_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType555:
    {
        DrawingSurface< Pixel555 > drawSurf
                = LockDrawingSurface< Pixel555 >( );
        shape.Draw( Pixel555( static_cast< uint16_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType565:
    {
        DrawingSurface< Pixel565 > drawSurf
                = LockDrawingSurface< Pixel565 >( );
        shape.Draw( Pixel565( static_cast< uint16_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType0888:
    {
        DrawingSurface< Pixel0888 > drawSurf
                = LockDrawingSurface< Pixel0888 >( );
        shape.Draw( Pixel0888( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType0888Rev:
    {
        DrawingSurface< Pixel0888Rev > drawSurf
                = LockDrawingSurface< Pixel0888Rev >( );
        shape.Draw( Pixel0888Rev( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType8888:
    {
        DrawingSurface< Pixel8888 > drawSurf
                = LockDrawingSurface< Pixel8888 >( );
        shape.Draw( Pixel8888( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType8888Rev:
    {
        DrawingSurface< Pixel8888Rev > drawSurf
                = LockDrawingSurface< Pixel8888Rev >( );
        shape.Draw( Pixel8888Rev( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
    Unlock( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Shape >
void
Surface::Draw( const Shape & shape, const Color3B & color )
{
    switch ( PixelType() )
    {
    case PixelType555:
    {
        DrawingSurface< Pixel555 > drawSurf
                = LockDrawingSurface< Pixel555 >( );
        shape.Draw( Pixel555( color ), &drawSurf );
        break;
    }
    case PixelType565:
    {
        DrawingSurface< Pixel565 > drawSurf
                = LockDrawingSurface< Pixel565 >( );
        shape.Draw( Pixel565( color ), &drawSurf );
        break;
    }
    case PixelType0888:
    {
        DrawingSurface< Pixel0888 > drawSurf
                = LockDrawingSurface< Pixel0888 >( );
        shape.Draw( Pixel0888( color ), &drawSurf );
        break;
    }
    case PixelType0888Rev:
    {
        DrawingSurface< Pixel0888Rev > drawSurf
                = LockDrawingSurface< Pixel0888Rev >( );
        shape.Draw( Pixel0888Rev( color ), &drawSurf );
        break;
    }
    case PixelType8888:
    {
        DrawingSurface< Pixel8888 > drawSurf
                = LockDrawingSurface< Pixel8888 >( );
        shape.Draw( Pixel8888( color ), &drawSurf );
        break;
    }
    case PixelType8888Rev:
    {
        DrawingSurface< Pixel8888Rev > drawSurf
                = LockDrawingSurface< Pixel8888Rev >( );
        shape.Draw( Pixel8888Rev( color ), &drawSurf );
        break;
    }
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
    Unlock( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Shape >
void
Surface::Draw( const Shape & shape, const Color4B & color )
{
    switch ( PixelType() )
    {
    case PixelType8888:
    {
        DrawingSurface< Pixel8888 > drawSurf
                = LockDrawingSurface< Pixel8888 >( );
        shape.Draw( Pixel8888( color ), &drawSurf );
        break;
    }
    case PixelType8888Rev:
    {
        DrawingSurface< Pixel8888Rev > drawSurf
                = LockDrawingSurface< Pixel8888Rev >( );
        shape.Draw( Pixel8888Rev( color ), &drawSurf );
        break;
    }
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
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
    {
        DrawingSurface< Pixel8 > drawSurf
                = LockDrawingSurface< Pixel8 >( );
        shape.Fill( Pixel8( static_cast< uint8_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType555:
    {
        DrawingSurface< Pixel555 > drawSurf
                = LockDrawingSurface< Pixel555 >( );
        shape.Fill( Pixel555( static_cast< uint16_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType565:
    {
        DrawingSurface< Pixel565 > drawSurf
                = LockDrawingSurface< Pixel565 >( );
        shape.Fill( Pixel565( static_cast< uint16_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType0888:
    {
        DrawingSurface< Pixel0888 > drawSurf
                = LockDrawingSurface< Pixel0888 >( );
        shape.Fill( Pixel0888( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType0888Rev:
    {
        DrawingSurface< Pixel0888Rev > drawSurf
                = LockDrawingSurface< Pixel0888Rev >( );
        shape.Fill( Pixel0888Rev( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType8888:
    {
        DrawingSurface< Pixel8888 > drawSurf
                = LockDrawingSurface< Pixel8888 >( );
        shape.Fill( Pixel8888( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    case PixelType8888Rev:
    {
        DrawingSurface< Pixel8888Rev > drawSurf
                = LockDrawingSurface< Pixel8888Rev >( );
        shape.Fill( Pixel8888Rev( static_cast< uint32_t >( pxl ) ), &drawSurf );
        break;
    }
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
    Unlock( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Shape >
void
Surface::Fill( const Shape & shape, const Color3B & color )
{
    switch ( PixelType() )
    {
    case PixelType555:
    {
        DrawingSurface< Pixel555 > drawSurf
                = LockDrawingSurface< Pixel555 >( );
        shape.Fill( Pixel555( color ), &drawSurf );
        break;
    }
    case PixelType565:
    {
        DrawingSurface< Pixel565 > drawSurf
                = LockDrawingSurface< Pixel565 >( );
        shape.Fill( Pixel565( color ), &drawSurf );
        break;
    }
    case PixelType0888:
    {
        DrawingSurface< Pixel0888 > drawSurf
                = LockDrawingSurface< Pixel0888 >( );
        shape.Fill( Pixel0888( color ), &drawSurf );
        break;
    }
    case PixelType0888Rev:
    {
        DrawingSurface< Pixel0888Rev > drawSurf
                = LockDrawingSurface< Pixel0888Rev >( );
        shape.Fill( Pixel0888Rev( color ), &drawSurf );
        break;
    }
    case PixelType8888:
    {
        DrawingSurface< Pixel8888 > drawSurf
                = LockDrawingSurface< Pixel8888 >( );
        shape.Fill( Pixel8888( color ), &drawSurf );
        break;
    }
    case PixelType8888Rev:
    {
        DrawingSurface< Pixel8888Rev > drawSurf
                = LockDrawingSurface< Pixel8888Rev >( );
        shape.Fill( Pixel8888Rev( color ), &drawSurf );
        break;
    }
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
    Unlock( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Shape >
void
Surface::Fill( const Shape & shape, const Color4B & color )
{
    switch ( PixelType() )
    {
    case PixelType8888:
    {
        DrawingSurface< Pixel8888 > drawSurf
                = LockDrawingSurface< Pixel8888 >( );
        shape.Fill( Pixel8888( color ), &drawSurf );
        break;
    }
    case PixelType8888Rev:
    {
        DrawingSurface< Pixel8888Rev > drawSurf
                = LockDrawingSurface< Pixel8888Rev >( );
        shape.Fill( Pixel8888Rev( color ), &drawSurf );
        break;
    }
    default:
        Assert( 0 && "Unexpected pixel type" );
        break;
    }
    Unlock( );
}

//=============================================================================

inline 
SDL_Surface * 
Surface::GetSDL_Surface( )
{
    return m_pSDL_Surface;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

inline 
const SDL_Surface * 
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

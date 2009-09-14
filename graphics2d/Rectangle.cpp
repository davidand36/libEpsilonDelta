/*
  Rectangle.cpp
  Copyright (C) 2007 David M. Anderson

  Rectangle class: representing an axis-aligned rectangle.
*/


#include "Rectangle.hpp"
#include "Point2.hpp"
#include "Surface.hpp"
#if USE_SDL
#include "SDLException.hpp"
#endif
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Rectangle::Rectangle( const Point2I & pt1, const Point2I & pt2 )
{
    Set( pt1, pt2 );
}

//=============================================================================

void 
Rectangle::Set( const Point2I & pt1, const Point2I & pt2 )
{
    m_left = std::min( pt1.X(), pt2.X() );
    m_top = std::min( pt1.Y(), pt2.Y() );
    m_width = std::abs( pt2.X() - pt1.X() ) + 1;
    m_height = std::abs( pt2.Y() - pt1.Y() ) + 1;
}

//=============================================================================

bool 
Rectangle::operator==( const Region & rhs ) const
{
    const Rectangle * pRectangle = dynamic_cast< Rectangle const * >( &rhs );
    return ( pRectangle && (*this == *pRectangle) );
}

//=============================================================================

bool 
Rectangle::Intersects( const Rectangle & rect ) const
{
    Rectangle intersection = Intersection( *this, rect );
    return  ! intersection.Empty();
}

//-----------------------------------------------------------------------------

bool 
Rectangle::Clip( const Rectangle & clipRect )
{
    *this = Intersection( *this, clipRect );
    return  ! Empty();
}

//-----------------------------------------------------------------------------

Rectangle 
Intersection( const Rectangle & rect1, const Rectangle & rect2 )
{
    int left = std::max( rect1.Left(), rect2.Left() );
    int top = std::max( rect1.Top(), rect2.Top() );
    int right = std::min( rect1.Right(), rect2.Right() );
    int bottom = std::min( rect1.Bottom(), rect2.Bottom() );
    return Rectangle( left, top,
            std::max( right - left + 1, 0 ),
            std::max( bottom - top + 1, 0 ) );
}

//-----------------------------------------------------------------------------

Rectangle 
BoundingRectangle( const Rectangle & rect1, const Rectangle & rect2 )
{
    int left = std::min( rect1.Left(), rect2.Left() );
    int top = std::min( rect1.Top(), rect2.Top() );
    int right = std::max( rect1.Right(), rect2.Right() );
    int bottom = std::max( rect1.Bottom(), rect2.Bottom() );
    int width = right - left + 1;
    int height = bottom - top + 1;
    return Rectangle( left, top, width, height );
}

//=============================================================================

void 
Rectangle::Draw( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Rectangle::Draw( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Rectangle::Draw( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//-----------------------------------------------------------------------------

void 
Rectangle::Fill( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
#if USE_SDL
    ::SDL_Surface * pSDL_Surf = pSurface->GetSDL_Surface();
    ::SDL_Rect sdl_Rect = *this;
    int fillRslt = ::SDL_FillRect( pSDL_Surf, &sdl_Rect, pxl );
    if ( fillRslt != 0 )
        throw SDLException( "SDL_FillRect" );
#else
    pSurface->Fill( *this, pxl );
#endif
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Rectangle::Fill( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
#if USE_SDL
    ::SDL_Surface * pSDL_Surf = pSurface->GetSDL_Surface();
    ::SDL_Rect sdl_Rect = *this;
    Uint32 pxl = ::SDL_MapRGB( pSDL_Surf->format,
                               static_cast< Uint8 >( color.Red() ),
                               static_cast< Uint8 >( color.Green() ),
                               static_cast< Uint8 >( color.Blue() ) );
    int fillRslt = ::SDL_FillRect( pSDL_Surf, &sdl_Rect, pxl );
    if ( fillRslt != 0 )
        throw SDLException( "SDL_FillRect" );
#else
    pSurface->Fill( *this, color );
#endif
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Rectangle::Fill( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
#if USE_SDL
    ::SDL_Surface * pSDL_Surf = pSurface->GetSDL_Surface();
    ::SDL_Rect sdl_Rect = *this;
    Uint32 pxl = ::SDL_MapRGBA( pSDL_Surf->format,
                                static_cast< Uint8 >( color.Red() ),
                                static_cast< Uint8 >( color.Green() ),
                                static_cast< Uint8 >( color.Blue() ),
                                static_cast< Uint8 >( color.Alpha() ) );
    int fillRslt = ::SDL_FillRect( pSDL_Surf, &sdl_Rect, pxl );
    if ( fillRslt != 0 )
        throw SDLException( "SDL_FillRect" );
#else
    pSurface->Fill( *this, color );
#endif
}

//=============================================================================

#ifdef DEBUG

bool 
Rectangle::Test( )
{
    bool ok = true;
    cout << "Testing Rectangle" << endl;

    int l = 200;
    int t = 30;
    int w = 100;
    int h = 80;
    cout << "Rectangle( " << l << ", " << t << ", " << w << ", " << h
         << " )" << endl;
    Rectangle rect1( l, t, w, h );
    TESTCHECK( rect1.Left(), l, &ok );
    TESTCHECK( rect1.Top(), t, &ok );
    TESTCHECK( rect1.Width(), w, &ok );
    TESTCHECK( rect1.Height(), h, &ok );
    TESTCHECK( rect1.Right(), (l + w - 1), &ok );
    TESTCHECK( rect1.Bottom(), (t + h - 1), &ok );
    TESTCHECK( rect1.TopLeft().X(), l, &ok );
    TESTCHECK( rect1.TopLeft().Y(), t, &ok );
    TESTCHECK( rect1.TopRight().X(), (l + w - 1), &ok );
    TESTCHECK( rect1.TopRight().Y(), t, &ok );
    TESTCHECK( rect1.BottomLeft().X(), l, &ok );
    TESTCHECK( rect1.BottomLeft().Y(), (t + h - 1), &ok );
    TESTCHECK( rect1.BottomRight().X(), (l + w - 1), &ok );
    TESTCHECK( rect1.BottomRight().Y(), (t + h - 1), &ok );
    TESTCHECK( rect1.Empty(), false, &ok );
    TESTCHECK( rect1.Area(), w * h, &ok );
#if USE_SDL
    cout << "SDL_Rect" << endl;
    ::SDL_Rect sdl_Rect = rect1;
    TESTCHECK( sdl_Rect.x, l, &ok );
    TESTCHECK( sdl_Rect.y, t, &ok );
    TESTCHECK( sdl_Rect.w, w, &ok );
    TESTCHECK( sdl_Rect.h, h, &ok );
#endif
    TESTCHECK( rect1.Contains( Point2I( 100, 40 ) ), false, &ok );
    TESTCHECK( rect1.Contains( Point2I( 200, 40 ) ), true, &ok );
    TESTCHECK( rect1.Contains( Point2I( 300, 40 ) ), false, &ok );
    TESTCHECK( rect1.Contains( Point2I( 250, 20 ) ), false, &ok );
    TESTCHECK( rect1.Contains( Point2I( 250, 30 ) ), true, &ok );
    TESTCHECK( rect1.Contains( Point2I( 250, 110 ) ), false, &ok );

    int x1 = 379;
    int y1 = 110;
    int x2 = 300;
    int y2 = 209;
    cout << "Rectangle( Point2I( " << x1 << ", " << y1 <<" ), "
         << "Point2I( " << x2 << ", " << y2 << " ) )" << endl;
    Rectangle rect2( Point2I( x1, y1 ), Point2I( x2, y2 ) );
    TESTCHECK( rect2.Left(), x2, &ok );
    TESTCHECK( rect2.Top(), y1, &ok );
    TESTCHECK( rect2.Width(), (x1 - x2 + 1), &ok );
    TESTCHECK( rect2.Height(), (y2 - y1 + 1), &ok );
    TESTCHECK( rect2.Right(), x1, &ok );
    TESTCHECK( rect2.Bottom(), y2, &ok );
    TESTCHECK( rect2.TopLeft().X(), x2, &ok );
    TESTCHECK( rect2.TopLeft().Y(), y1, &ok );
    TESTCHECK( rect2.TopRight().X(), x1, &ok );
    TESTCHECK( rect2.TopRight().Y(), y1, &ok );
    TESTCHECK( rect2.BottomLeft().X(), x2, &ok );
    TESTCHECK( rect2.BottomLeft().Y(), y2, &ok );
    TESTCHECK( rect2.BottomRight().X(), x1, &ok );
    TESTCHECK( rect2.BottomRight().Y(), y2, &ok );
    TESTCHECK( rect2.Empty(), false, &ok );
    TESTCHECK( rect2.Area(), 8000, &ok );
#if USE_SDL
    cout << "SDL_Rect" << endl;
    sdl_Rect = rect2;
    TESTCHECK( sdl_Rect.x, x2, &ok );
    TESTCHECK( sdl_Rect.y, y1, &ok );
    TESTCHECK( sdl_Rect.w, ( x1 - x2 + 1 ), &ok );
    TESTCHECK( sdl_Rect.h, ( y2 - y1 + 1 ), &ok );
#endif
    TESTCHECK( (rect1 == rect2), false, &ok );
    TESTCHECK( (rect2 == rect2), true, &ok );
    TESTCHECK( (rect1 != rect2), true, &ok );
    TESTCHECK( rect1.Contains( rect2 ), false, &ok );
    TESTCHECK( rect2.Contains( rect1 ), false, &ok );
    TESTCHECK( rect1.Intersects( rect2 ), false, &ok );
    TESTCHECK( rect2.Intersects( rect1 ), false, &ok );

    l = 230;
    t = 50;
    w = 40;
    h = 40;
#if USE_SDL
    ::SDL_Rect sdl_Rect3 = { static_cast< Sint16 >( l ),
                             static_cast< Sint16 >( t ),
                             static_cast< Uint16 >( w ),
                             static_cast< Uint16 >( h ) };
    cout << "Rectangle( SDL_Rect( " << l << ", " << t << ", " << w
         << ", " << h << " ) )" << endl;
    Rectangle rect3( sdl_Rect3 );
#else
    cout << "Rectangle( " << l << ", " << t << ", " << w << ", " << h
         << " )" << endl;
    Rectangle rect3( l, t, w, h );
#endif
    TESTCHECK( rect3.Left(), l, &ok );
    TESTCHECK( rect3.Top(), t, &ok );
    TESTCHECK( rect3.Width(), w, &ok );
    TESTCHECK( rect3.Height(), h, &ok );
    TESTCHECK( rect3.Right(), (l + w - 1), &ok );
    TESTCHECK( rect3.Bottom(), (t + h - 1), &ok );
    TESTCHECK( rect3.TopLeft().X(), l, &ok );
    TESTCHECK( rect3.TopLeft().Y(), t, &ok );
    TESTCHECK( rect3.TopRight().X(), (l + w - 1), &ok );
    TESTCHECK( rect3.TopRight().Y(), t, &ok );
    TESTCHECK( rect3.BottomLeft().X(), l, &ok );
    TESTCHECK( rect3.BottomLeft().Y(), (t + h - 1), &ok );
    TESTCHECK( rect3.BottomRight().X(), (l + w - 1), &ok );
    TESTCHECK( rect3.BottomRight().Y(), (t + h - 1), &ok );
    TESTCHECK( rect3.Empty(), false, &ok );
    TESTCHECK( rect3.Area(), w * h, &ok );
#if USE_SDL
    cout << "SDL_Rect" << endl;
    sdl_Rect = rect3;
    TESTCHECK( sdl_Rect.x, l, &ok );
    TESTCHECK( sdl_Rect.y, t, &ok );
    TESTCHECK( sdl_Rect.w, w, &ok );
    TESTCHECK( sdl_Rect.h, h, &ok );
#endif
    TESTCHECK( (rect1 == rect3), false, &ok );
    TESTCHECK( (rect3 == rect3), true, &ok );
    TESTCHECK( (rect2 != rect3), true, &ok );
    TESTCHECK( rect1.Contains( rect3 ), true, &ok );
    TESTCHECK( rect3.Contains( rect1 ), false, &ok );
    TESTCHECK( rect1.Intersects( rect3 ), true, &ok );
    TESTCHECK( rect3.Intersects( rect1 ), true, &ok );

    if ( ok )
        cout << "Rectangle PASSED." << endl << endl;
    else
        cout << "Rectangle FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void 
Rectangle::TestDraw( )
{
    Color3B gray( 100, 100, 100 );
    uint32_t white = 0xFFFFFFFF;
    Color3B brown( 100, 100, 0 );
    Color3B yellow( 255, 255, 0 );
    Color3B darkblue( 0, 0, 100 );
    Color3B blue( 0, 0, 255 );
    int l = 200;
    int t = 30;
    int w = 100;
    int h = 80;
    Rectangle rect1( l, t, w, h );
    int x1 = 379;
    int y1 = 110;
    int x2 = 300;
    int y2 = 209;
    Rectangle rect2( Point2I( x1, y1 ), Point2I( x2, y2 ) );
    l = 230;
    t = 50;
    w = 40;
    h = 40;
#if USE_SDL
    ::SDL_Rect sdl_Rect3 = { static_cast< Sint16 >( l ),
                             static_cast< Sint16 >( t ),
                             static_cast< Uint16 >( w ),
                             static_cast< Uint16 >( h ) };
    Rectangle rect3( sdl_Rect3 );
#else
    Rectangle rect3( l, t, w, h );
#endif

    Rectangle oldClip = Surface::Current()->ClippingRect();
    rect1.Fill( gray );
    rect1.Draw( white );
    rect2.Fill( gray );
    rect2.Draw( white );
    rect3.Fill( gray );
    rect3.Draw( white );
    Surface::Current()->SetClippingRect( Rectangle( 240, 60, 20, 20 ) );
    rect1.Fill( yellow );
    rect1.Draw( brown );
    rect2.Fill( yellow );
    rect2.Draw( brown );
    rect3.Fill( yellow );
    rect3.Draw( brown );
    Surface::Current()->SetClippingRect( Rectangle( 280, 90, 40, 40 ) );
    rect1.Fill( darkblue );
    rect1.Draw( blue );
    rect2.Fill( darkblue );
    rect2.Draw( blue );
    rect3.Fill( darkblue );
    rect3.Draw( blue );
    Surface::Current()->SetClippingRect( oldClip );
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  Line2.cpp
  Copyright (C) 2007 David M. Anderson

  Line2I class: Two-dimensional line.
*/


#include "Line2.hpp"
#include "Surface.hpp"
#ifdef DEBUG
#include "TestCheck.hpp"
using namespace std;
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


enum EClip { ClipNone = 0,
             ClipLeft = 1,
             ClipTop = 2,
             ClipRight = 4,
             ClipBottom = 8
           };
static EClip ComputeLocationCode( const Point2I & pt, const Rectangle & rect );

//.............................................................................

bool 
Line2I::Clip( const Rectangle & rect )
{                                                                      /*Clip*/
    PointPair endpoints = Endpoints();
    Point2I pt0 = endpoints.first;
    Point2I pt1 = endpoints.second;
    float left = static_cast< float >( rect.Left() );
    float top = static_cast< float >( rect.Top() );
    float right = static_cast< float >( rect.Right() );
    float bottom = static_cast< float >( rect.Bottom() );

    EClip code0 = ComputeLocationCode( pt0, rect );
    EClip code1 = ComputeLocationCode( pt1, rect );
    bool inside = ( (code0 | code1) == 0 );
    bool outside = ( (code0 & code1) != 0 );

    while ( ! inside && ! outside )
    {
        if ( code0 == ClipNone ) // Swap if necessary so pt0 needs clipping.
        {
            std::swap( pt0, pt1 );
            std::swap( code0, code1 );
        }

        float x, y;
        if ( code0 & ClipLeft )
        {
            y = pt0.Y()
                    +  ((pt1.Y() - pt0.Y())
                        * (left - pt0.X()) / (pt1.X() - pt0.X()));
            x = left;
        }
        else if ( code0 & ClipTop )
        {
            x = pt0.X()
                    +  ((pt1.X() - pt0.X()) 
                        * (top - pt0.Y()) / (pt1.Y() - pt0.Y()));
            y = top;
        }
        else if ( code0 & ClipRight )
        {
            y = pt0.Y()
                    +  ((pt1.Y() - pt0.Y())
                        * (right - pt0.X()) / (pt1.X() - pt0.X()));
            x = right;
        }
        else    //ClipBottom
        {
            x = pt0.X()
                    +  ((pt1.X() - pt0.X())
                        * (bottom - pt0.Y()) / (pt1.Y() - pt0.Y()));
            y = bottom;
        }
        pt0.Set( static_cast< int >( x + 0.5f ),
                 static_cast< int >( y + 0.5f ) );

        code0 = ComputeLocationCode( pt0, rect );
        inside = ( (code0 | code1) == 0 );
        outside = ( (code0 & code1) != 0 );
    }

    Set( pt0, pt1 );

    return inside;
}                                                                      /*Clip*/

//.............................................................................

static EClip ComputeLocationCode( const Point2I & pt, const Rectangle & rect )
{
    int code = ClipNone;

    if ( pt.X() < rect.Left() )
        code |= ClipLeft;
    if ( pt.Y() < rect.Top() )
        code |= ClipTop;
    if ( rect.Right() < pt.X() )
        code |= ClipRight;
    if ( rect.Bottom() < pt.Y() )
        code |= ClipBottom;

    return  EClip( code );
}

//=============================================================================

void 
Line2I::Draw( uint32_t pxl, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, pxl );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Line2I::Draw( const Color3B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

void 
Line2I::Draw( const Color4B & color, Surface * pSurface ) const
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    pSurface->Draw( *this, color );
}

//=============================================================================

#ifdef DEBUG

bool 
Line2I::Test( )
{
    bool ok = true;
    cout << "Testing Line2I" << endl;

    int x = 60;
    int y = 20;
    int dx = 100;
    int dy = 80;
    cout << "Line2I( " << x << ", " << y << ", " << dx << ", " << dy << " )"
         << endl; 
    Line2I ln1( x, y, dx, dy );
    TESTCHECK( ln1.X(), x, &ok );
    TESTCHECK( ln1.Y(), y, &ok );
    TESTCHECK( ln1.DX(), dx, &ok );
    TESTCHECK( ln1.DY(), dy, &ok );
    cout << "ln1.Endpoints()" << endl;
    PointPair endpoints = ln1.Endpoints();
    TESTCHECK( endpoints.first.X(), x, &ok );
    TESTCHECK( endpoints.first.Y(), y, &ok );
    TESTCHECK( endpoints.second.X(), (x + dx), &ok );
    TESTCHECK( endpoints.second.Y(), (y + dy), &ok );
    TESTCHECK( ln1.LengthSquared(), 16400, &ok );

    int x1 = 160;
    int y1 = 20;
    int x2 = 60;
    int y2 = 100;
    cout << "Line2I( Point2I( " << x1 << ", " << y1 <<" ), "
         << "Point2I( " << x2 << ", " << y2 << " ) )" << endl;
    Line2I ln2( Point2I( x1, y1 ), Point2I( x2, y2 ) );
    cout << "ln2.Endpoints()" << endl;
    endpoints = ln2.Endpoints();
    TESTCHECK( endpoints.first.X(), x2, &ok );
    TESTCHECK( endpoints.first.Y(), y2, &ok );
    TESTCHECK( endpoints.second.X(), x1, &ok );
    TESTCHECK( endpoints.second.Y(), y1, &ok );
    TESTCHECK( ln2.X(), x2, &ok );
    TESTCHECK( ln2.Y(), y2, &ok );
    TESTCHECK( ln2.DX(), (x1 - x2), &ok );
    TESTCHECK( ln2.DY(), (y1 - y2), &ok );
    TESTCHECK( ln2.LengthSquared(), 16400, &ok );
    TESTCHECK( (ln1 == ln2), false, &ok );
    TESTCHECK( (ln1 == ln1), true, &ok );
    TESTCHECK( (ln1 != ln2), true, &ok );

    if ( ok )
        cout << "Line2I PASSED." << endl << endl;
    else
        cout << "Line2I FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

void
Line2I::TestDraw( )
{
    Color3B gray( 100, 100, 100 );
    Color3B yellow( 255, 255, 0 );
    Color3B magenta( 255, 0, 255 );
    Color3B cyan( 0, 255, 255 );

    int x = 60;
    int y = 20;
    int dx = 100;
    int dy = 80;
    Line2I ln1( x, y, dx, dy );
    int x1 = 160;
    int y1 = 20;
    int x2 = 60;
    int y2 = 100;
    Line2I ln2( Point2I( x1, y1 ), Point2I( x2, y2 ) );

    Rectangle oldClip = Surface::Current()->ClippingRect();
    ln1.Draw( gray );
    ln2.Draw( gray );
    Surface::Current()->SetClippingRect( Rectangle( 100, 50, 20, 20 ) );
    ln1.Draw( yellow );
    ln2.Draw( yellow );
    Surface::Current()->SetClippingRect( Rectangle( 60, 10, 20, 200 ) );
    ln1.Draw( cyan );
    ln2.Draw( cyan );
    Surface::Current()->SetClippingRect( Rectangle( 140, 10, 20, 200 ) );
    ln1.Draw( magenta );
    ln2.Draw( magenta );
    Surface::Current()->SetClippingRect( oldClip );
}

#endif


//*****************************************************************************

}                                                      //namespace EpsilonDelta

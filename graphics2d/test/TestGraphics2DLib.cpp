/*
  TestGraphics2DLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta graphics2d library.
*/


#include "Exception.hpp"
#include "TestCheck.hpp"
#include "Color.hpp"
#include "Pixel.hpp"
#include "Point2.hpp"
#include "Line2.hpp"
#include "Rectangle.hpp"
#include "Ellipse.hpp"
#include "Circle.hpp"
#include "Graphics2D.hpp"
#include "Surface.hpp"
#include "Image.hpp"
#include "Platform.hpp"
#include <cstdio>
#include <iostream>
using namespace std;
using namespace EpsilonDelta;


int Main( int /*argc*/, char ** /*argv*/ );


//*****************************************************************************


int main( int argc, char ** argv )
{
    try
    {
        int res = Main( argc, argv );
        return res;
    }
    catch ( Exception & except )
    {
        cout << except.Description( ) << endl;
    }
    catch ( exception & except )
    {
        Exception exc( except );
        cout << exc.Description( ) << endl;
    }
    catch ( ... )
    {
        Exception exc( "Unknown exception!" ); //!!!
        cout << exc.Description( ) << endl;
    }
    return  2;
}

//-----------------------------------------------------------------------------

int Main( int /*argc*/, char ** argv )
{
    bool ok = true;

#ifdef DEBUG
    string libBasePath = argv[0];
    int slashPos = libBasePath.rfind( '/' );
    if ( slashPos == static_cast<int>( string::npos ) )
        libBasePath = "";
    else
        libBasePath.erase( slashPos + 1 );
    libBasePath += "../";
#ifdef COMPILER_MSC
    libBasePath += "../";
#endif

    if ( ! TestColor( ) )
        ok = false;
    if ( ! TestPixel( ) )
        ok = false;
    if ( ! Point2I::Test( ) )
        ok = false;
    if ( ! Line2I::Test( ) )
        ok = false;
    if ( ! Rectangle::Test( ) )
        ok = false;
    if ( ! Ellipse::Test( ) )
        ok = false;
    if ( ! Circle::Test( ) )
        ok = false;
    if ( ! Graphics2D::Instance().Test( ) )
        ok = false;
    if ( ok )
    {
        Graphics2D::Instance().Init( );
        Graphics2D::Instance().SetupScreen( 800, 600, "Test 2D Graphics Lib" );

        if ( ! Surface::TestLoad( libBasePath + "graphics2d/test/star.png" ) )
            ok = false;
        if ( ! Image::TestLoad( libBasePath + "graphics2d/test/star.png" ) )
            ok = false;
        if ( ok )
        {
            int frames = 0;
            Uint32 start = SDL_GetTicks();

            do
            {
                Point2I::TestDraw( );
                Line2I::TestDraw( );
                Rectangle::TestDraw( );
                Ellipse::TestDraw( );
                Circle::TestDraw( );
                Surface::TestDraw( );
                Image::TestDraw( );

                Graphics2D::Instance().ShowScreen( );
                ++frames;
            }
            while ( SDL_GetTicks() < start + 5000 );

            float fps = frames / 5.f;
            cout << "FPS: " << fps << endl;

            Image::TestUnload( );
            Surface::TestUnload( );
        }
    }
#endif //DEBUG

    if ( ok )
    {
        cout << endl << "All tests PASSED!" << endl;
        return  0;
    }
    else
    {
        cout << endl << "Some test(s) FAILED!" << endl;
        return  1;
    }
}


//*****************************************************************************

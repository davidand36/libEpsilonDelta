/*
  TestGraphics2DLib.cpp
  Copyright (C) 2007 David M. Anderson

  Exercises the EpsilonDelta graphics2d library.
*/


#include "Exception.hpp"
#include "TestCheck.hpp"
#include "Color.hpp"
#include "Pixel.hpp"
#include "Dot2.hpp"
#include "LineSegment2.hpp"
#include "Rectangle.hpp"
#include "Ellipse.hpp"
#include "Circle.hpp"
#include "Graphics2D.hpp"
#include "Surface.hpp"
#include "Image.hpp"
#include "Sprite.hpp"
#include "SimpleSpriteSystem.hpp"
#include "FontManager.hpp"
#include "Platform.hpp"
#include "StdClock.hpp"
#include "Array.hpp"
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

int Main( int argc, char ** argv )
{
    bool ok = true;
    bool testResolutions = false;

    for ( int i = 1; i < argc; ++i )
    {
        if ( strcmp( argv[ i ], "-r" ) == 0 )
            testResolutions = true;
    }

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
    if ( ! Dot2I::Test( ) )
        ok = false;
    if ( ! LineSegment2I::Test( ) )
        ok = false;
    if ( ! Rectangle::Test( ) )
        ok = false;
    if ( ! Ellipse::Test( ) )
        ok = false;
    if ( ! Circle::Test( ) )
        ok = false;
    if ( ! Surface::Test( ) )
        ok = false;
    if ( ! Sprite::Test( ) )
        ok = false;
    if ( ! SimpleSpriteSystem::Test( ) )
        ok = false;
    if ( ! Graphics2D::Instance().Test( testResolutions ) )
        ok = false;
    if ( ok )
    {
        Graphics2D::Instance().Init( );
        Graphics2D::Instance().SetupScreen( 800, 600, "Test 2D Graphics Lib" );

        string testDirectory = libBasePath + "graphics2d/test/";

        if ( ! Surface::TestLoad( testDirectory + "star.png" ) )
            ok = false;

        if ( ! Image::TestLoad( testDirectory + "star.png" ) )
            ok = false;

        string fontNames[] = { "Vera.ttf", "", "b018035l.pfb", "b018035l.pfm",
                               "helvR18.pcf", "" };
        vector< string > testFonts( fontNames,
                                    fontNames + ARRAY_LENGTH( fontNames ) );
        if ( ! FontManager::TestLoad( testDirectory, testFonts ) )
            ok = false;

        if ( ok )
        {
            int frames = 0;
            double start = StdClock::Instance().Seconds();

            bool done = false;
            do
            {
                Surface::Current()->Extent().Fill( Color3B( 0, 0, 0 ) );
                Dot2I::TestDraw( );
                LineSegment2I::TestDraw( );
                Rectangle::TestDraw( );
                Ellipse::TestDraw( );
                Circle::TestDraw( );
                Surface::TestDraw( );
                Image::TestDraw( );
                FontManager::TestDraw( );
                done = SimpleSpriteSystem::TestUpdate( );

                Graphics2D::Instance().ShowScreen( );
                ++frames;
            }
            while ( (! done)
                    || (StdClock::Instance().Seconds() <= start + 10.) );

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

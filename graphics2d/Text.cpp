/*
  Text.cpp
  Copyright (C) 2009 David M. Anderson

  Text namespace: routines for drawing text strings.
*/


#include "Text.hpp"
#include "FontManager.hpp"
#include "UnicodeUtil.hpp"
#include "CodePointData.hpp"
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace Text
{                                                              //namespace Text

//*****************************************************************************


Point2I
Draw( string strng, const Point2I & basePos,
      int fontID, const Color3B & color, Surface * pSurface )
{
    wstring wstrng = DecodeUTF8( strng );
    return Draw( wstrng, basePos, fontID, color, pSurface );
}

//.............................................................................

Point2I
Draw( wstring strng, const Point2I & basePos,
      int fontID, const Color3B & color, Surface * pSurface )
{
    FontManager & fontMgr = FontManager::Instance();
    Point2I pos = basePos;
    int length = (int)strng.length();
    for ( int i = 0; i < length; ++i )
    {
        shared_ptr< Glyph > pGlyph
                = fontMgr.GetGlyph( fontID, strng[ i ], color );
        pGlyph->Draw( pos, pSurface );
        Vector2F advance = pGlyph->GetAdvance();
        if ( i < length - 1 )
        {
            Vector2F kerning
                    = fontMgr.GetKerning( fontID, strng[ i ], strng[ i + 1 ] );
            advance += kerning;
        }
        pos += Vector2I( Round( advance ) );
    }
    return pos;
}

//-----------------------------------------------------------------------------

int 
Draw( string strng, const Rectangle & margins,
      int fontID, const Color3B & color, Surface * pSurface )
{
    wstring wstrng = DecodeUTF8( strng );
    return Draw( wstrng, margins, fontID, color, pSurface );
}

//.............................................................................

int 
Draw( wstring wstrng, const Rectangle & margins,
      int fontID, const Color3B & color, Surface * pSurface )
{
    return 0;   //!!!
}

//=============================================================================

int 
WordWrap( string strng, int width, int fontID, vector< int > * pLineStarts )
{
    wstring wstrng = DecodeUTF8( strng );
    return WordWrap( strng, width, fontID, pLineStarts );
}

//.............................................................................

int 
WordWrap( wstring strng, int width, int fontID, vector< int > * pLineStarts )
{
    return 0;   //!!!
}

//=============================================================================

#ifdef DEBUG

void 
TestDraw( )
{
    Color3B yellow( 255, 255, 0 );
    Color3B magenta( 255, 0, 255 );
    int fontID1 = 40;   //Created previously in FontManager::TestLoad().
    int fontID2 = 2040; //Ditto
    string testStrng1( "WAVY" );    //Excercises kerning
    string testStrng2( "üçüncü" );  //Excercises non-ASCII Unicode
    Point2I penPos( 20, 180 );
    penPos = Text::Draw( testStrng1, penPos, fontID1, yellow );
    penPos = Text::Draw( testStrng2, penPos, fontID2, magenta );
}

//-----------------------------------------------------------------------------

void 
FullTest( )
{
    //!!!
}

#endif


//*****************************************************************************

}                                                              //namespace Text

}                                                      //namespace EpsilonDelta

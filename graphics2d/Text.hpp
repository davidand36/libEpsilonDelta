#ifndef TEXT_HPP
#define TEXT_HPP
/*
  Text.hpp
  Copyright (C) 2009 David M. Anderson

  Text namespace: routines for drawing text strings.
  NOTES:
  1. Arguments of type std::string are presumed to be UTF-8 encoded.
  2. The Point2I basePos arguments are the initial position on the baseline of
     the text. When drawn, ascenders will rise above this point, and descenders
     will fall below it. These routines do not do any word wrapping. They do
     return the position where the next glyph would be drawn.
  3. The routines with Rectangle arguments do perform word wrapping and make
     every effort to keep the rendering within the margins. The value returned
     is the number of characters drawn.
*/


#include "Surface.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace Text
{                                                              //namespace Text

//*****************************************************************************


Point2I Draw( std::string strng, const Point2I & basePos,
              int fontID, const Color3B & color, Surface * pSurface = 0 );
Point2I Draw( std::wstring strng, const Point2I & basePos,
           int fontID, const Color3B & color, Surface * pSurface = 0 );
int Draw( std::string strng, const Rectangle & margins,
          int fontID, const Color3B & color, Surface * pSurface = 0 );
int Draw( std::wstring strng, const Rectangle & margins,
          int fontID, const Color3B & color, Surface * pSurface = 0 );

int WordWrap( std::string strng, int width, int fontID,
              std::vector< int > * pLineStarts );
int WordWrap( std::wstring strng, int width, int fontID,
              std::vector< int > * pLineStarts );

#ifdef DEBUG
void TestDraw( );
void FullTest( );
#endif


//*****************************************************************************


}                                                              //namespace Text

}                                                      //namespace EpsilonDelta

#endif //TEXT_HPP

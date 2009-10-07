#ifndef FONTMANAGER_HPP
#define FONTMANAGER_HPP
/*
  FontManager.hpp
  Copyright (C) 2009 David M. Anderson

  FontManager singleton class: Manages text fonts.
  NOTES:
  1. We use the following terminology:
     A FontSet is a collection of one or more typefaces in one or more sizes,
     whose glyph data are in a single file.
     A Face is a single style (typeface) in a FontSet.
     A Font is a single Face in a specific size.
     A Glyph is a bitmap representing a single character in a Font in a
     specific color (or colors), ready for drawing.
     (This usage is somewhat different from that used by, e.g., FreeType, but
     denotes the classes that seem most important to the user. This usage of
     "font" is close to the traditional typography term.)
  2. AddFontSet() and AddFace() return integers that identify the objects that
     have been added. So, for example, the first argument to AddFace() is the
     ID of the FontSet containing the face. Note that these IDs are not
     persistent across instantiations of the FontManager.
  3. In contrast, the first argument to AddFont() is an ID provided by the
     user. This allows the user to use the same font ID every time, and even
     to store it in text data, for example. Of course it is up to the user to
     maintain the correspondence.
  4. Font size is indicated by the height in pixels, i.e. the distance
     between consecutive baselines.
     Ascent() and Descent() give the declared length of the ascenders and
     descenders, respectively, from the baseline. Note that it is common that
     Height < Ascent + Descent, i.e. that the font is designed so that glyphs
     can overlap.
*/


#include "Singleton.hpp"
#include "DataBuffer.hpp"
#include "Glyph.hpp"
#include "Vector2.hpp"
#include "Color.hpp"
#include <string>
#include <vector>
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FontManagerImpl;


//*****************************************************************************


class FontManager
    :   public Singleton< FontManager >
{
public:
    void Init( );
    void Shutdown( );

    int AddFontSet( std::tr1::shared_ptr< DataBuffer > fontData,
                    std::tr1::shared_ptr< DataBuffer > auxData
                    = std::tr1::shared_ptr< DataBuffer >() );
    int AddFace( int fontSetID, int faceIndex );
    void AddFont( int fontID, int faceID, float height );

    int NumFaces( int fontSetID ) const;
    std::string FaceName( int faceID ) const;
    bool IsScalable( int faceID ) const;
    int GetFixedSizes( int faceID, std::vector< float > * pSizes ) const;
    float Height( int fontID );
    float Ascent( int fontID );
    float Descent( int fontID );

    std::tr1::shared_ptr< Glyph > GetGlyph( int fontID, wchar_t character,
                                            Color3B color,
                                            bool monochrome = false );

    Vector2F GetKerning( int fontID, wchar_t leftChar, wchar_t rightChar );

#ifdef DEBUG
    static bool TestLoad( const std::string & directory,
                          std::vector< std::string > const & testFileNames );
    static bool TestDraw( );
#endif

private:
    FontManager( );
    ~FontManager( );

    std::tr1::shared_ptr< FontManagerImpl >  m_pImpl;

    friend class Singleton< FontManager >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FONTMANAGER_HPP

/*
  FontManager.cpp
  Copyright (C) 2009 David M. Anderson

  FontManager singleton class: Manages text fonts.
*/


#include "FontManager.hpp"
#ifdef USE_FREETYPE
#include "FTException.hpp"
#include "Pixel.hpp"
#include "FixedPoint.hpp"
#include <vector>
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_CACHE_H
#endif //USE_FREETYPE
#ifdef DEBUG
#include "TestCheck.hpp"
#include "File.hpp"
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FontManagerImpl
{
public:
    FontManagerImpl( );
    ~FontManagerImpl( );

    void Init( );
    void Shutdown( );

    int AddFontSet( shared_ptr< DataBuffer > fontData,
                     shared_ptr< DataBuffer > auxData );
    int AddFace( int fontSetID, int faceIndex );
    void AddFont( int fontID, int faceID, float height );

    int NumFaces( int fontSetID ) const;
    string FaceName( int faceID ) const;
    bool IsScalable( int faceID ) const;
    int GetFixedSizes( int faceID, vector< float > * pSizes ) const;
    float Height( int fontID ) const;

    shared_ptr< Glyph > GetGlyph( int fontID, wchar_t character,
                                  Color3B color, bool monochrome );

#ifdef USE_FREETYPE
    static ::FT_Error FaceRequestHandler( ::FTC_FaceID facePtr,
                                          ::FT_Library ftLibrary,
                                          ::FT_Pointer userData,
                                          ::FT_Face * pFTFace );
#endif //USE_FREETYPE
    
private:
#ifdef USE_FREETYPE
    struct FontSet
    {
        shared_ptr< DataBuffer > fontBuffer;
        shared_ptr< DataBuffer > auxBuffer;
    };

    struct Face
    {
        int     fontSetID;
        int     faceIndex;
    };

    struct Font
    {
        int     fontID;
        int     faceID;
        float   height;
    };

    int FindFace( int fontSetID, int faceIndex = 0 ) const;
    const Font * FindFont( int fontID ) const;
    ::FT_Face GetFTFace( int faceID ) const;
    shared_ptr< Glyph > MakeGlyph( ::FTC_SBit sbit, Color3B color );
    shared_ptr< Glyph > MakeGlyph( ::FT_BitmapGlyph ftBitmapGlyph,
                                   Color3B color );

    ::FT_Library        m_ftLibrary;
    ::FTC_Manager       m_ftCacheMgr;
    ::FTC_CMapCache     m_ftCharmapCache;
    ::FTC_ImageCache    m_ftGlyphCache;
    ::FTC_SBitCache     m_ftSBitCache;
    vector< FontSet >   m_fontSets;
    vector< Face >      m_faces;
    vector< Font >      m_fonts;
#endif //USE_FREETYPE
};


//=============================================================================


namespace
{                                                                   //namespace

#ifdef USE_FREETYPE


#endif //USE_FREETYPE
}                                                                   //namespace


//*****************************************************************************


FontManager::FontManager( )
    :   m_pImpl( new FontManagerImpl )
{
}

//-----------------------------------------------------------------------------

FontManager::~FontManager( )
{
    try
    {
        Shutdown( );
    }
    catch ( ... )
    {
        //Destructors mustn't throw.
    }
}

//=============================================================================

void 
FontManager::Init( )
{
    m_pImpl->Init( );
}

//-----------------------------------------------------------------------------

void 
FontManager::Shutdown( )
{
    m_pImpl->Shutdown( );
}

//=============================================================================

int 
FontManager::AddFontSet( shared_ptr< DataBuffer > fontData,
                         shared_ptr< DataBuffer > auxData )
{
    return m_pImpl->AddFontSet( fontData, auxData );
}

//-----------------------------------------------------------------------------

int 
FontManager::AddFace( int fontSetID, int faceIndex )
{
    return m_pImpl->AddFace( fontSetID, faceIndex );
}

//-----------------------------------------------------------------------------

void 
FontManager::AddFont( int fontID, int faceID, float height )
{
    m_pImpl->AddFont( fontID, faceID, height );
}

//=============================================================================

int 
FontManager::NumFaces( int fontSetID ) const
{
    return m_pImpl->NumFaces( fontSetID );
}

//-----------------------------------------------------------------------------

string 
FontManager::FaceName( int faceID ) const
{
    return m_pImpl->FaceName( faceID );
}

//-----------------------------------------------------------------------------

bool 
FontManager::IsScalable( int faceID ) const
{
    return m_pImpl->IsScalable( faceID );
}

//-----------------------------------------------------------------------------

int 
FontManager::GetFixedSizes( int faceID, vector< float > * pSizes ) const
{
    return m_pImpl->GetFixedSizes( faceID, pSizes );
}

//-----------------------------------------------------------------------------

float 
FontManager::Height( int fontID )
{
    return m_pImpl->Height( fontID );
}

//=============================================================================

shared_ptr< Glyph > 
FontManager::GetGlyph( int fontID, wchar_t character,
                       Color3B color, bool monochrome )
{
    return m_pImpl->GetGlyph( fontID, character, color, monochrome );
}

//=============================================================================

#ifdef DEBUG

//-----------------------------------------------------------------------------

namespace
{                                                                   //namespace

vector< int > s_testFontIDs;

}                                                                   //namespace

//-----------------------------------------------------------------------------

bool 
FontManager::TestLoad( const ::string & directory,
                       vector< string > const & testFileNames )
{
    bool ok = true;
    cout << "Testing FontManager" << endl;

    FontManager & fontManager = FontManager::Instance();
    fontManager.Init( );

    try
    {
        int numFiles = (int)testFileNames.size();
        Assert( (numFiles % 2 == 0)
                && "FileNames must come in pairs for test." );
        for ( int i = 0; i < numFiles; i += 2 )
        {
            string const & fileName0 = testFileNames[ i ];
            string const & fileName1 = testFileNames[ i + 1 ];
            int fontSetID = -1;
            shared_ptr< DataBuffer > pBuff0( new DataBuffer );
            if ( ! File::Load( directory + fileName0, pBuff0.get() ) )
                throw Exception( "File::Load() failed: " + fileName0 );
            if ( fileName1.size() > 0 )
            {
                shared_ptr< DataBuffer > pBuff1( new DataBuffer );
                if ( ! File::Load( directory + fileName1, pBuff1.get() ) )
                    throw Exception( "File::Load() failed: " + fileName1 );
                fontSetID = fontManager.AddFontSet( pBuff0, pBuff1 );
            }
            else
            {
                fontSetID = fontManager.AddFontSet( pBuff0 );
            }
            cout << "FontSet (" << fileName0 << ") has ID " << fontSetID
                 << endl;

            int numFaces = fontManager.NumFaces( fontSetID );
            cout << "FontSet " << fontSetID << " has "
                 << numFaces << " face(s)." << endl;
            for ( int j = 0; j < numFaces; ++j )
            {
                int faceID = fontManager.AddFace( fontSetID, j );
                cout << "Face (fontSet " << fontSetID
                     << ", faceIndex " << j << ") has ID " << faceID << endl;
                cout << "Face " << faceID << " is \""
                     << fontManager.FaceName( faceID ) << "\"" << endl;
                bool scalable = fontManager.IsScalable( faceID );
                if ( scalable )
                {
                    cout << "Face " << faceID << " is scalable." << endl;
                    float sizes[] = { 10.f, 20.f, 40.f };
                    for ( int k = 0; k < ARRAY_LENGTH( sizes ); ++k )
                    {
                        int fontID = faceID * 2000  +  (int)sizes[ k ];
                        fontManager.AddFont( fontID, faceID, sizes[ k ] );
                        s_testFontIDs.push_back( fontID );
                        cout << "Font (face "<< faceID << ", size "
                             << sizes[ k ] << ") has ID " << fontID << endl;
                    }
                }
                else
                {
                    vector< float > sizes;
                    int numSizes = fontManager.GetFixedSizes( faceID, &sizes );
                    cout << "Face " << faceID << " has "
                         << numSizes << " fixed size(s): ";
                    for ( int s = 0; s < numSizes; ++s )
                    {
                        if ( s > 0 )
                            cout << ", ";
                        cout << sizes[ s ];
                    }
                    cout << endl;
                    for ( int k = 0; k < sizes.size(); ++k )
                    {
                        int fontID = faceID * 2000  +  (int)sizes[ k ];
                        fontManager.AddFont( fontID, faceID, sizes[ k ] );
                        s_testFontIDs.push_back( fontID );
                        cout << "Font (face "<< faceID << ", size "
                             << sizes[ k ] << ") has ID " << fontID << endl;
                    }
                }
            }
        }
    }    
    catch ( Exception except )
    {
        ok = false;
        cout << except.Description( ) << endl;
    }

    if ( ok )
        cout << "FontManager PASSED." << endl << endl;
    else
        cout << "FontManager FAILED." << endl << endl;
    return ok;
}

//-----------------------------------------------------------------------------

bool 
FontManager::TestDraw( )
{
    FontManager & fontManager = FontManager::Instance();

    Color3B yellow( 255, 255, 0 );
    Point2F penPos( 480, 180 );
    for ( int i = 0; i < s_testFontIDs.size(); ++i )//!!!
    {
        int fontID = s_testFontIDs[ i ];
        shared_ptr< Glyph > pGlyph
                = fontManager.GetGlyph( fontID, 'M', yellow, true );
        Point2I pos( (int)penPos.X(), (int)penPos.Y() );
        pGlyph->Draw( pos );
        penPos += pGlyph->GetAdvance();
        pGlyph = fontManager.GetGlyph( fontID, 'A', yellow, false );
        pos.Set( (int)penPos.X(), (int)penPos.Y() );
        pGlyph->Draw( pos );
        penPos += pGlyph->GetAdvance();
    }
}

//-----------------------------------------------------------------------------

#endif


//*****************************************************************************


#ifdef USE_FREETYPE

//=============================================================================

FontManagerImpl::FontManagerImpl( )
    :   m_ftLibrary( 0 ),
        m_ftCacheMgr( 0 ),
        m_ftCharmapCache( 0 ),
        m_ftGlyphCache( 0 ),
        m_ftSBitCache( 0 )
{
}

//-----------------------------------------------------------------------------

FontManagerImpl::~FontManagerImpl( )
{
}

//=============================================================================

void 
FontManagerImpl::Init( )
{
    if ( m_ftLibrary == 0 )
    {
        ::FT_Error rslt = ::FT_Init_FreeType( &m_ftLibrary );
        if ( rslt != 0 )
            throw FTException( "FT_Init_FreeType", rslt );
    }
    if ( m_ftCacheMgr == 0 )
    {
        //At least for now, we use the default cache size, etc.
        ::FT_Error rslt = ::FTC_Manager_New( m_ftLibrary, 0, 0, 0,
                                             FaceRequestHandler,
                                             (FT_Pointer)this,
                                             &m_ftCacheMgr );
        if ( rslt != 0 )
            throw FTException( "FTC_Manager_New", rslt );
    }
    if ( m_ftCharmapCache == 0 )
    {
        ::FT_Error rslt = ::FTC_CMapCache_New( m_ftCacheMgr,
                                               &m_ftCharmapCache );
        if ( rslt != 0 )
            throw FTException( "FTC_CMapCache_New", rslt );
    }
    if ( m_ftGlyphCache == 0 )
    {
        ::FT_Error rslt = ::FTC_ImageCache_New( m_ftCacheMgr, &m_ftGlyphCache );
        if ( rslt != 0 )
            throw FTException( "FTC_ImageCache_New", rslt );
    }
    if ( m_ftSBitCache == 0 )
    {
        ::FT_Error rslt = ::FTC_SBitCache_New( m_ftCacheMgr, &m_ftSBitCache );
        if ( rslt != 0 )
            throw FTException( "FTC_SBitCache_New", rslt );
    }
}

//-----------------------------------------------------------------------------

void 
FontManagerImpl::Shutdown( )
{
    m_fonts.clear();
    m_faces.clear();
    m_fontSets.clear();
    if ( m_ftCacheMgr != 0 )
    {
        ::FTC_Manager_Done( m_ftCacheMgr );
        m_ftCacheMgr = 0;
        m_ftCharmapCache = 0;
        m_ftGlyphCache = 0;
        m_ftSBitCache = 0;
    }
    if ( m_ftLibrary != 0 )
    {
        ::FT_Error rslt = ::FT_Done_FreeType( m_ftLibrary );
        m_ftLibrary = 0;
        if ( rslt != 0 )
            throw FTException( "FT_Done_FreeType", rslt );
    }
}

//=============================================================================

int 
FontManagerImpl::AddFontSet( shared_ptr< DataBuffer > fontData,
                             shared_ptr< DataBuffer > auxData )
{
    int fontSetID = (int)m_fontSets.size(); //ID is next vector index
    FontSet fontSet = { fontData, auxData };
    m_fontSets.push_back( fontSet );
    AddFace( fontSetID, 0 );  //Automatically add the first face.
    return fontSetID;
}

//-----------------------------------------------------------------------------

int 
FontManagerImpl::AddFace( int fontSetID, int faceIndex )
{
    int numFaces = (int)m_faces.size();
    for ( int i = 0; i < numFaces; ++i )
        if ( (m_faces[ i ].fontSetID == fontSetID)
             && (m_faces[ i ].faceIndex == faceIndex) )
            return i;
    Face face = { fontSetID, faceIndex };
    m_faces.push_back( face );
    return numFaces;
}

//-----------------------------------------------------------------------------

void 
FontManagerImpl::AddFont( int fontID, int faceID, float height )
{
    Font font = { fontID, faceID, height };
    m_fonts.push_back( font );
}

//=============================================================================

int 
FontManagerImpl::NumFaces( int fontSetID ) const
{
    Assert( m_ftCacheMgr );
    int faceID = FindFace( fontSetID );
    ::FT_Face ftFace = GetFTFace( faceID );
    return (int)ftFace->num_faces;
}

//-----------------------------------------------------------------------------

string 
FontManagerImpl::FaceName( int faceID ) const
{
    ::FT_Face ftFace = GetFTFace( faceID );
    string name;
    if ( ftFace->family_name )
        name += ftFace->family_name;
    if ( ftFace->style_name )
        name += string( " " ) + ftFace->style_name;
    return name;
}

//-----------------------------------------------------------------------------

bool 
FontManagerImpl::IsScalable( int faceID ) const
{
    ::FT_Face ftFace = GetFTFace( faceID );
    return ((ftFace->face_flags & FT_FACE_FLAG_SCALABLE) != 0);
}

//-----------------------------------------------------------------------------

int 
FontManagerImpl::GetFixedSizes( int faceID, vector< float > * pSizes ) const
{
    pSizes->clear();
    ::FT_Face ftFace = GetFTFace( faceID );
    int numFixedSizes = ftFace->num_fixed_sizes;
    for ( int i = 0; i < numFixedSizes; ++i )
    {
        Fixed32_6 fxSize( ftFace->available_sizes[ i ].y_ppem, true );
        pSizes->push_back( fxSize.ToFloat() );
    }
    return numFixedSizes;
}

//-----------------------------------------------------------------------------

float 
FontManagerImpl::Height( int fontID ) const
{
    return FindFont( fontID )->height;
}

//=============================================================================

shared_ptr< Glyph > 
FontManagerImpl::GetGlyph( int fontID, wchar_t character,
                           Color3B color, bool monochrome )
{
    const Font * pFont = FindFont( fontID );
    const Face * pFace = &m_faces[ pFont->faceID ];
    int glyphIndex
            = ::FTC_CMapCache_Lookup( m_ftCharmapCache,
                                      const_cast< Face *>( pFace ), -1,
                                      character ); //-1=default charmap
    int height = (int)floor( pFont->height + 0.5 );
#if 0
    if ( height <= 255 )
    {
        ::FTC_ImageTypeRec imageType;
        imageType.face_id = const_cast< Face *>( pFace );
        imageType.width = 0;
        imageType.height = height;
        imageType.flags = FT_LOAD_RENDER;
        if ( monochrome )
            imageType.flags |= FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO;
        ::FTC_SBit ftSBit;
        ::FT_Error rslt = ::FTC_SBitCache_Lookup( m_ftSBitCache, &imageType,
                                                  glyphIndex, &ftSBit, 0 );
        if ( rslt != 0 )
            throw FTException( "FTC_SBitCache_Lookup", rslt );
        shared_ptr< Glyph > pGlyph = MakeGlyph( sbit, color );
        //!!!cache this
        return pGlyph;
    }
    else
#endif
    {
        ::FTC_ImageTypeRec imageType;
        imageType.face_id = const_cast< Face *>( pFace );
        imageType.width = 0;
        imageType.height = height;
        imageType.flags = FT_LOAD_RENDER;
        if ( monochrome )
            imageType.flags |= FT_LOAD_MONOCHROME | FT_LOAD_TARGET_MONO;
        ::FT_Glyph ftGlyph;
        ::FT_Error rslt = ::FTC_ImageCache_Lookup( m_ftGlyphCache, &imageType,
                                                   glyphIndex, &ftGlyph, 0 );
        if ( rslt != 0 )
        {
            cout << "GetGlyph: fontID=" << fontID << " faceID=" << pFont->faceID
                 << " height=" << height << endl;
            throw FTException( "FTC_ImageCache_Lookup", rslt );
        }
        ::FT_BitmapGlyph ftBitmap
                  = reinterpret_cast< ::FT_BitmapGlyph >( ftGlyph );
        shared_ptr< Glyph > pGlyph = MakeGlyph( ftBitmap, color );
        //!!!cache this
        return pGlyph;
    }
}

//-----------------------------------------------------------------------------

shared_ptr< Glyph > 
FontManagerImpl::MakeGlyph( ::FTC_SBit ftSBit, Color3B color )
{
    Vector2F offset( (float)ftSBit->left, -(float)ftSBit->top );
    Vector2F advance( (float)ftSBit->xadvance, -(float)ftSBit->yadvance );
    bool monochrome = (ftSBit->format == 0); //?!!!
    EPixelType pixelType = monochrome  ?  PixelType888  :  PixelType8888;
    shared_ptr< Surface > pSurface( new Surface( ftSBit->width, ftSBit->height,
                                                 pixelType ) );
    //!!!Blt to surface
    return shared_ptr< Glyph >( new Glyph( pSurface, offset, advance ) );
}

//-----------------------------------------------------------------------------

shared_ptr< Glyph > 
FontManagerImpl::MakeGlyph( ::FT_BitmapGlyph ftBitmapGlyph, Color3B color )
{
    Vector2F offset( (float)ftBitmapGlyph->left, -(float)ftBitmapGlyph->top );
    Fixed32_16 advX( ftBitmapGlyph->root.advance.x, true );
    Fixed32_16 advY( ftBitmapGlyph->root.advance.y, true );
    Vector2F advance( advX.ToFloat(), advY.ToFloat() );

    bool monochrome
            = ((ftBitmapGlyph->bitmap.pixel_mode & FT_PIXEL_MODE_MONO) != 0);
    if ( ! monochrome )
        Assert( (ftBitmapGlyph->bitmap.pixel_mode & FT_PIXEL_MODE_GRAY) != 0 );
    int width = ftBitmapGlyph->bitmap.width;
    int height = ftBitmapGlyph->bitmap.rows;
    EPixelType pixelType = monochrome  ?  PixelType0888  :  PixelType8888;
    shared_ptr< Surface > pSurface( new Surface( width, height, pixelType ) );

    unsigned char * srcPixels = ftBitmapGlyph->bitmap.buffer;
    int srcPitch = ftBitmapGlyph->bitmap.pitch;
    if ( srcPitch < 0 )
        srcPixels += -srcPitch * (height - 1);   //start at last row (=top)

    if ( monochrome )
    {
        Color3B transparentColor;
        if ( color.Red() == 0 )
            transparentColor.Set( 255, 255, 255 );
        else
            transparentColor.Set( 0, 0, 0 );
        pSurface->SetTransparentColor( transparentColor );

        Pixel0888 fg( color );
        Pixel0888 bg( transparentColor );

        int destPitch = pSurface->Pitch();
        Pixel0888 * destPixels
                = reinterpret_cast< Pixel0888 * >( pSurface->Lock( ) );

        for ( int y = 0; y < height; ++y )
        {
            unsigned char * pSrc = srcPixels;
            Pixel0888 * pDest = destPixels;
            unsigned char c = 0;
            for ( int x = 0; x < width; ++x )
            {
                if ( (x & 7) == 0 ) //x%8==0
                    c = *pSrc++;
                bool on = ((c & 0x80) != 0);
                c <<= 1;
                *pDest++ = (on  ?  fg  :  bg);
            }
            srcPixels += srcPitch;
            destPixels += destPitch;
        }
    }
    else
    {
        int destPitch = pSurface->Pitch();
        Pixel8888 * destPixels
                = reinterpret_cast< Pixel8888 * >( pSurface->Lock( ) );

        for ( int y = 0; y < height; ++y )
        {
            unsigned char * pSrc = srcPixels;
            Pixel8888 * pDest = destPixels;
            for ( int x = 0; x < width; ++x )
            {
                Pixel8888 pxl( Color4B( color, (int)(*pSrc++) ) );
                *pDest++ = pxl;
            }
            srcPixels += srcPitch;
            destPixels += destPitch;
        }
    }
    pSurface->Unlock( );

    return shared_ptr< Glyph >( new Glyph( pSurface, offset, advance ) );
}

//=============================================================================

int
FontManagerImpl::FindFace( int fontSetID, int faceIndex ) const
{
    for ( int i = 0; i < (int)m_faces.size(); ++i )
        if ( (m_faces[ i ].fontSetID == fontSetID)
             && (m_faces[ i ].faceIndex == faceIndex) )
            return i;
    Assert( 0 && "FindFace failed" );
    return -1;
}

//-----------------------------------------------------------------------------

const FontManagerImpl::Font * 
FontManagerImpl::FindFont( int fontID ) const
{
    for ( size_t i = 0; i < m_fonts.size(); ++i )
        if ( m_fonts[ i ].fontID == fontID )
            return &m_fonts[ i ];
    Assert( 0 && "FindFont" );
    return 0;
}

//-----------------------------------------------------------------------------

::FT_Face 
FontManagerImpl::GetFTFace( int faceID ) const
{
    Assert( (faceID >= 0) && (faceID < (int)m_faces.size()) );
    const Face * pFace = &m_faces[ faceID ];
    ::FT_Face ftFace = 0;
    ::FT_Error rslt = ::FTC_Manager_LookupFace( m_ftCacheMgr,
                                                const_cast< Face * >( pFace ),
                                                &ftFace );
    if ( rslt != 0 )
        throw FTException( "FTC_Manager_LookupFace", rslt );
    Assert( ftFace != 0 );
    return ftFace;
}

//=============================================================================

::FT_Error 
FontManagerImpl::FaceRequestHandler( ::FTC_FaceID facePtr,
                                     ::FT_Library ftLibrary,
                                     ::FT_Pointer userData,
                                     ::FT_Face * pFTFace )
{
    FontManagerImpl * pImpl = reinterpret_cast< FontManagerImpl * >( userData );
    const Face * pFace = reinterpret_cast< Face * >( facePtr );
    size_t fontSetID = pFace->fontSetID;
    Assert( fontSetID < pImpl->m_fontSets.size() );
    FontSet & fontSet = pImpl->m_fontSets[ fontSetID ];
    int faceIndex = pFace->faceIndex;
    shared_ptr< DataBuffer > fontBuffer = fontSet.fontBuffer;
    const FT_Byte * buffer
            = reinterpret_cast< const FT_Byte * >( fontBuffer->Data() );
    int size = fontBuffer->Size();
    ::FT_Face ftFace;
    ::FT_Error rslt = ::FT_New_Memory_Face( ftLibrary,
                                            buffer, size,
                                            faceIndex,
                                            &ftFace );
    if ( rslt != 0 )
        throw FTException( "FT_New_Memory_Face", rslt );
    shared_ptr< DataBuffer > auxBuffer = fontSet.auxBuffer;
    if ( auxBuffer )
    {
        ::FT_Open_Args openArgs;
        openArgs.flags = FT_OPEN_MEMORY;
        openArgs.memory_base
                = reinterpret_cast< const FT_Byte * >( auxBuffer->Data() );
        openArgs.memory_size = auxBuffer->Size();
        ::FT_Error rslt = ::FT_Attach_Stream( ftFace, &openArgs );
        if ( rslt != 0 )
            throw FTException( "FT_Attach_Stream", rslt );
    }
    Assert( (ftFace->charmap != 0) && "No Uniicode charmap in font" );
    if ( pFTFace )
        *pFTFace = ftFace;
    return 0;
}

//=============================================================================

#endif //USE_FREETYPE


//*****************************************************************************

}                                                      //namespace EpsilonDelta

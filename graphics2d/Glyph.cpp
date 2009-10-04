/*
  Glyph.cpp
  Copyright (C) 2009 David M. Anderson

  Glyph class: the graphic representation of a text character in a particular
  font, color, etc.
*/


#include "Glyph.hpp"
using namespace std;
using namespace std::tr1;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


Glyph::Glyph( shared_ptr< Surface > image, Vector2F offset, Vector2F advance )
    :   m_image( image ),
        m_offset( offset ),
        m_advance( advance )
{
    Assert( m_image );
}

//=============================================================================

void 
Glyph::Draw( const Point2I & destPos, Surface * pSurface )
{
    if ( pSurface == 0 )
        pSurface = Surface::Current();
    Point2I pos = destPos + Vector2I( (int)m_offset.X(), (int)m_offset.Y() );
    m_image->Blit( pos, pSurface );
}

//-----------------------------------------------------------------------------

Vector2F 
Glyph::GetAdvance( ) const
{
    return m_advance;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

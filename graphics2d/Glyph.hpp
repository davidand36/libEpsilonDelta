#ifndef GLYPH_HPP
#define GLYPH_HPP
/*
  Glyph.hpp
  Copyright (C) 2009 David M. Anderson

  Glyph class: the graphic representation of a text character in a particular
  font, color, etc.
*/


#include "Surface.hpp"
#include "SmartPtr.hpp"
#include "Vector2.hpp"
#include "Point2.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Glyph
{
public:
    Glyph( shared_ptr< Surface > image, Vector2F offset, Vector2F advance );

    void Draw( const Point2I & destPos, Surface * pSurface = 0 );
    Vector2F GetAdvance( ) const;
    
private:
    shared_ptr< Surface >   m_image;
    Vector2F                m_offset;
    Vector2F                m_advance;
};



//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //GLYPH_HPP

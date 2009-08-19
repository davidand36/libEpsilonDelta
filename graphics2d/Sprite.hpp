#ifndef SPRITE_HPP
#define SPRITE_HPP
/*
  Sprite.hpp
  Copyright (C) 2007 David M. Anderson

  Sprite class: A graphic element (surface) to be rendered at a specific
  position.
*/


#include "Surface.hpp"
#include "SmartPtr.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta
//*****************************************************************************


class Sprite
{
public:
    Sprite( shared_ptr< Surface > graphic, const Point2I & position );
    virtual ~Sprite( );

    shared_ptr< Surface > Graphic( ) const;
    const Point2I & Position( ) const;
    void SetGraphic( shared_ptr< Surface > graphic );
    void SetPosition( const Point2I & position );

#ifdef DEBUG
    static bool Test( );
#endif

private:
    shared_ptr< Surface >   m_graphic;
    Point2I                 m_position;
};


//*****************************************************************************


inline
Sprite::Sprite( shared_ptr< Surface > graphic, const Point2I & position )
    :   m_graphic( graphic ),
        m_position( position )
{
}

//-----------------------------------------------------------------------------

inline
Sprite::~Sprite( )
{
}

//=============================================================================

inline
shared_ptr< Surface > 
Sprite::Graphic( ) const
{
    return m_graphic;
}

//-----------------------------------------------------------------------------

inline
const Point2I & 
Sprite::Position( ) const
{
    return m_position;
}

//=============================================================================

inline
void 
Sprite::SetGraphic( shared_ptr< Surface > graphic )
{
    m_graphic = graphic;
}

//-----------------------------------------------------------------------------

inline
void 
Sprite::SetPosition( const Point2I & position )
{
    m_position = position;
}


//*****************************************************************************
}                                                      //namespace EpsilonDelta

#endif //SPRITE_HPP

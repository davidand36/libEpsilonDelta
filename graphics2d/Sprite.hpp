#ifndef SPRITE_HPP
#define SPRITE_HPP
/*
  Sprite.hpp
  Copyright (C) 2007 David M. Anderson

  Sprite class: A graphic element (surface) to be rendered at a specific
  position.
*/


#include "Region.hpp"
#include "Surface.hpp"
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta
//*****************************************************************************


class Sprite
    :   public Region
{
public:
    Sprite( std::tr1::shared_ptr< Surface > graphic, const Point2I & position,
            bool visible = true );
    virtual ~Sprite( );

    std::tr1::shared_ptr< Surface > Graphic( ) const;
    const Point2I & Position( ) const;
    void SetGraphic( std::tr1::shared_ptr< Surface > graphic );
    void SetPosition( const Point2I & position );
    void Show( );
    void Hide( );
    bool IsVisible( ) const;

    virtual bool Contains( const Point2I & point ) const;
    bool operator==( const Sprite & rhs ) const;
    bool operator!=( const Sprite & rhs ) const;
    virtual bool operator==( const Region & rhs ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    std::tr1::shared_ptr< Surface >     m_graphic;
    Point2I                             m_position;
    bool                                m_visible;
};


//*****************************************************************************


inline
Sprite::Sprite( std::tr1::shared_ptr< Surface > graphic,
                const Point2I & position, bool visible )
    :   m_graphic( graphic ),
        m_position( position ),
        m_visible( visible )
{
}

//-----------------------------------------------------------------------------

inline
Sprite::~Sprite( )
{
}

//=============================================================================

inline
std::tr1::shared_ptr< Surface > 
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
Sprite::SetGraphic( std::tr1::shared_ptr< Surface > graphic )
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

//=============================================================================

inline
void 
Sprite::Show( )
{
    m_visible = true;
}

//-----------------------------------------------------------------------------

inline
void 
Sprite::Hide( )
{
    m_visible = false;
}

//-----------------------------------------------------------------------------

inline
bool 
Sprite::IsVisible( ) const
{
    return m_visible;
}

//=============================================================================

inline
bool 
Sprite::operator==( const Sprite & rhs ) const
{
    return  (m_graphic == rhs.m_graphic) && (m_position == rhs.m_position);
}

//-----------------------------------------------------------------------------

inline
bool 
Sprite::operator!=( const Sprite & rhs ) const
{
    return  ! (*this == rhs);
}


//*****************************************************************************
}                                                      //namespace EpsilonDelta

#endif //SPRITE_HPP

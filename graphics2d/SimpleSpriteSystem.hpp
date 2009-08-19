#ifndef SIMPLESPRITESYSTEM_HPP
#define SIMPLESPRITESYSTEM_HPP
/*
  SimpleSpriteSystem.hpp
  Copyright (C) David M. Anderson

  SimpleSpriteSystem class: Sprite system that redraws background and all
  sprites each frame.
  NOTES:
  1. This system does not do any erasing, so if no background is set, sprites
     may leave trails.
*/


#include "SpriteSystem.hpp"
#include <list>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta
//*****************************************************************************


class SimpleSpriteSystem
    :   public SpriteSystem
{
public:
    SimpleSpriteSystem( );
    virtual ~SimpleSpriteSystem( );

    void SetBackground( shared_ptr< Surface > background );

    virtual bool AddSprite( shared_ptr< Sprite > sprite, int level = 0 );
    virtual bool RemoveSprite( shared_ptr< Sprite > sprite );
    virtual bool SetLevel( shared_ptr< Sprite > sprite, int level );

    void Render( );

#ifdef DEBUG
    static bool Test( );
    static bool TestUpdate( );
#endif

private:
#ifdef DEBUG
    static bool CheckSorted( const SimpleSpriteSystem & sss );
#endif

    struct SpriteData
    {
        shared_ptr< Sprite >    m_sprite;
        int                     m_level;
    };

    shared_ptr< Surface >   m_background;
    typedef std::list< SpriteData > SpriteList;
    SpriteList              m_sprites;
};


//*****************************************************************************
}                                                      //namespace EpsilonDelta

#endif //SIMPLESPRITESYSTEM_HPP

#ifndef SPRITESYSTEM_HPP
#define SPRITESYSTEM_HPP
/*
  SpriteSystem.hpp
  Copyright (C) 2007 David M. Anderson

  SpriteSystem abstract base class: Manages and displays background and sprites.
*/


#include "Sprite.hpp"
#include "SmartPtr.hpp"
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta
//*****************************************************************************


class SpriteSystem
{
public:
    virtual ~SpriteSystem( ) { };

    virtual bool AddSprite( shared_ptr< Sprite > sprite, int level = 0 ) = 0;
    virtual bool RemoveSprite( shared_ptr< Sprite > sprite ) = 0;
    virtual bool SetLevel( shared_ptr< Sprite > sprite, int level ) = 0;

    virtual void Render( ) = 0;
};


//*****************************************************************************
}                                                      //namespace EpsilonDelta

#endif //SPRITESYSTEM_HPP

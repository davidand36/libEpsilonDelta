#ifndef REGIONMAP_HPP
#define REGIONMAP_HPP
/*
  RegionMap.hpp
  Copyright (C) 2009 David M. Anderson

  RegionMap class: Associates regions with actions.
  1. The mapping is from regions, which are presumed, but not required, to
     be areas of the screen that can be pointed to, and integers, which
     are presumed to represent actions the program should perform.
  2. The use of shared_ptr is a bit clumsy, but it allows for polymorphic
     regions while providing safety through (shared) ownership.
  3. Regions are examined in the order they were added, so in the case of
     overlap, the first region takes precedence.
  4. This class is copy constructable and assignable, so maps can be saved,
     replaced, and restored, when moving between modes of interaction, for
     example.
*/


#include "InputDevice.hpp"
#include "Region.hpp"
#include <tr1/memory>
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class RegionMap
{
public:
    void Set( std::tr1::shared_ptr< Region > pRegion, int action );
    void Remove( const Region & region );
    void Reset( );

    int Action( const Region & region ) const;
    int Action( const Point2I & point ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    struct RegAct
    {
        std::tr1::shared_ptr< Region >  m_pRegion;
        int                             m_action;
    };

    std::vector< RegAct >   m_regActs;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //REGIONMAP_HPP

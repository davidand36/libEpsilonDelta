#ifndef INPUTEVENTMAP_HPP
#define INPUTEVENTMAP_HPP
/*
  InputEventMap.hpp
  Copyright (C) 2009 David M. Anderson

  InputEventMap class: Associates input events with actions.
  1. The mapping is to integers, which are presumed, but not required, to
     represent actions the program should perform.
  2. The values used are up to the application. They are not required to be
     nonnegative, but note that "no action" is denoted by -1000.
  3. This class is copy constructable and assignable, so maps can be saved,
     replaced, and restored, when moving between modes of interaction, for
     example.
*/


#include "InputEvent.hpp"
#include "VMap.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputEventMap
{
public:
    InputEventMap( );

    void Set( const InputEvent & event, int action );
    void Remove( const InputEvent & event );
    void Reset( );

    int Action( const InputEvent & event ) const;

    static const int NoAction = -1000;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    typedef bool (*IELess)( const InputEvent & lhs, const InputEvent & rhs );
    typedef VMap< InputEvent, int, IELess >  MapType;

    MapType     m_map;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTEVENTMAP_HPP

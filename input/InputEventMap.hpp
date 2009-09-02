#ifndef INPUTEVENTMAP_HPP
#define INPUTEVENTMAP_HPP
/*
  InputEventMap.hpp
  Copyright (C) 2009 David M. Anderson

  InputEventMap class: Maps input events to user-defined values.
  1. The values used are up to the application. They are not required to be
     nonnegative, but note that "no value" is denoted by -1000.
  2. This class can (by default) be copied and assigned, so maps can be saved,
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

    void Set( const InputEvent & event, int value );
    void Remove( const InputEvent & event );

    int Value( const InputEvent & event ) const;

    static const int NoValue = -1000;

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

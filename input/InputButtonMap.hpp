#ifndef INPUTBUTTONMAP_HPP
#define INPUTBUTTONMAP_HPP
/*
  InputButtonMap.hpp
  Copyright (C) 2009 David M. Anderson

  InputButtonMap class: Associates button presses (on input devices)
  with actions.
  1. The mapping is to integers, which are presumed, but not required, to
     represent actions the program should perform.
  2. The values used are up to the application. They are not required to be
     nonnegative, but note that "no action" is denoted by -1000.
  3. This class is copy constructable and assignable, so maps can be saved,
     replaced, and restored, when moving between modes of interaction, for
     example.
*/


#include "InputDevice.hpp"
#include "SmartPtr.hpp"
#include "VMap.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputButtonMap
{
public:
    InputButtonMap( );

    void Set( shared_ptr< InputDevice const > device, int button, int action );
    void Remove( shared_ptr< InputDevice const > device, int button );
    void Reset( );

    int Action( shared_ptr< InputDevice const > device, int button ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    struct DevButton
    {
        shared_ptr< InputDevice const > m_device;
        int                             m_button;
    };

    typedef bool (*ButtonLessFunc)( const DevButton & lhs,
                                    const DevButton & rhs );
    typedef VMap< DevButton, int, ButtonLessFunc >  MapType;

    static bool ButtonLess( const DevButton & lhs, const DevButton & rhs );

    MapType     m_map;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTBUTTONMAP_HPP

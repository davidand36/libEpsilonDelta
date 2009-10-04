#ifndef INPUTDEVICEMAP_HPP
#define INPUTDEVICEMAP_HPP
/*
  InputDeviceMap.hpp
  Copyright (C) 2009 David M. Anderson

  InputDeviceMap class: Associates input devices with owners.
  NOTES:
  1. The mapping is to integers, which are presumed, but not required, to
     represent users (owners) of the devices, mainly for multiplayer games,
     and such.
  2. Each device may only have one owner, but each owner may own multiple
     devices.
  3. Owner IDs are not required to be nonnegative, but note that "no owner"
     is represented here by -1000.
*/


#include "InputDevice.hpp"
#include "Singleton.hpp"
#include <vector>
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class InputDeviceMap
    :   public Singleton< InputDeviceMap >
{
public:
    void Set( std::tr1::shared_ptr< InputDevice const > pDevice, int owner );
    void Remove( std::tr1::shared_ptr< InputDevice const > pDevice );
    void Reset( );

    int Owner( std::tr1::shared_ptr< InputDevice const > pDevice ) const;
    std::vector< std::tr1::shared_ptr< InputDevice const > >
            Devices( int owner ) const;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    InputDeviceMap( );
    ~InputDeviceMap( );

    struct OwnedDevice
    {
        std::tr1::shared_ptr< InputDevice const >   m_pDevice;
        int                                         m_owner;
    };

    std::vector< OwnedDevice >  m_ownedDevices;

    friend class Singleton< InputDeviceMap >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INPUTDEVICEMAP_HPP

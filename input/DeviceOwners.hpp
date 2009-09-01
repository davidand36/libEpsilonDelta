#ifndef DEVICEOWNERS_HPP
#define DEVICEOWNERS_HPP
/*
  DeviceOwners.hpp
  Copyright (C) 2009 David M. Anderson

  DeviceOwners class: Associates input devices with owners.
  NOTES:
  1. Owners are simply identified by integers. It is up to the application to
     give these integers additional meaning.
  2. Each device may only have one owner, but each owner may own multiple
     devices.
  3. Owner IDs are not required to be nonnegative, but note that "no owner"
     is represented here by -1000.
*/


#include "InputDevice.hpp"
#include "Singleton.hpp"
#include "SmartPtr.hpp"
#include <vector>

namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class DeviceOwners
    :   public Singleton< DeviceOwners >
{
public:
    void Set( shared_ptr< InputDevice const > pDevice, int owner );

    int Owner( shared_ptr< InputDevice const > pDevice ) const;
    std::vector< shared_ptr< InputDevice const > > Devices( int owner ) const;

    void Disown( shared_ptr< InputDevice const > pDevice );
    void Reset( );

    static const int NoOwner = -1000;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    DeviceOwners( );
    ~DeviceOwners( );

    struct OwnedDevice
    {
        shared_ptr< InputDevice const >     m_pDevice;
        int                                 m_owner;
    };

    std::vector< OwnedDevice >  m_ownedDevices;

    friend class Singleton< DeviceOwners >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DEVICEOWNERS_HPP

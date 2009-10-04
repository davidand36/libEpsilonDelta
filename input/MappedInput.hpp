#ifndef MAPPEDINPUT_HPP
#define MAPPEDINPUT_HPP
/*
  MappedInput.hpp
  Copyright (C) 2009 David M. Anderson

  MappedInput class: Maps input events to owners and actions.
*/


#include "InputButtonMap.hpp"
#include "RegionMap.hpp"
#include "InputEvent.hpp"
#include "Singleton.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class MappedInput
    :   public Singleton< MappedInput >
{
public:
    void SetButtonMap(
        std::tr1::shared_ptr< InputButtonMap const > pButtonMap );
    void SetRegionMap( std::tr1::shared_ptr< RegionMap const > pRegionMap,
                       std::tr1::shared_ptr< InputDevice const > pDevice
                       = std::tr1::shared_ptr< InputDevice const >(),
                       int button = 0, int pointer = 0 );
    void RemoveRegionMap( std::tr1::shared_ptr< InputDevice const > pDevice
                          = std::tr1::shared_ptr< InputDevice const >(),
                          int button = 0, int pointer = 0 );

    bool CheckEvent( int * pAction, int * pOwner = 0 ) const;

    static const int NoAction = -1000;
    static const int NoOwner = -1000;

#ifdef DEBUG
    static bool Test( );
#endif

private:
    MappedInput( );
    ~MappedInput( );
    bool MapEvent( std::tr1::shared_ptr< InputEvent const > pEvent,
                   int * pAction, int * pOwner ) const;


    struct RegMapItem
    {
        bool Matches( std::tr1::shared_ptr< InputDevice const > pDevice,
                      int button, int pointer = -1 ) const;

        std::tr1::shared_ptr< InputDevice const >   m_device;
        int                                         m_button;
        int                                         m_pointer;
        std::tr1::shared_ptr< RegionMap const >     m_pRegionMap;
    };

    std::tr1::shared_ptr< InputButtonMap const >    m_pButtonMap;
    std::vector< RegMapItem >                       m_regionMaps;

    friend class Singleton< MappedInput >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //MAPPEDINPUT_HPP

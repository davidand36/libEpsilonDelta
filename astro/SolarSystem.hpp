#ifndef SOLARSYSTEM_HPP
#define SOLARSYSTEM_HPP
/*
  SolarSystem.hpp
  Copyright (C) 2007 David M. Anderson

  EBody enum of major bodies in the solar system.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

namespace SolarSystem
{                                                       //namespace SolarSystem

//*****************************************************************************


enum EBody
    { Sun, Moon,
      Mercury, Venus, Earth, Mars, Jupiter, Saturn, Uranus, Neptune,
      Pluto,
      NumBodies
    };


//*****************************************************************************

}                                                       //namespace SolarSystem

}                                                      //namespace EpsilonDelta

#endif //SOLARSYSTEM_HPP

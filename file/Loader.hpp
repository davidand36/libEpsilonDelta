#ifndef LOADER_HPP
#define LOADER_HPP
/*
  Loader.hpp
  Copyright (C) 2011 David M. Anderson

  Loader abstract base class representing a file or device from which data
  can be loaded.
  NOTES:
  1. Load() is a one-shot operation that reads all of the data at once.
*/


#include "DataBuffer.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Loader
{
public:
    virtual ~Loader( ) { }

    virtual void Load( DataBuffer * pBuffer ) = 0;
    virtual void Load( std::string * pText );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //LOADER_HPP

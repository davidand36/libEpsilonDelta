#ifndef READER_HPP
#define READER_HPP
/*
  Reader.hpp
  Copyright (C) 2011 David M. Anderson

  Reader abstract base class representing a file or similar object
  from which data can be read and within which the read position can be set.
  NOTES:
  1. Load() is a one-shot operation that reads the entire file into memory.
*/


#include "BasicReader.hpp"
#include "RandomAccess.hpp"
#include "Loader.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Reader
    :   public BasicReader,
        public virtual RandomAccess,
        public Loader
{
public:
    virtual ~Reader( ) { }

    using Loader::Load;
    virtual void Load( DataBuffer * pBuffer );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //READER_HPP

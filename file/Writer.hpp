#ifndef WRITER_HPP
#define WRITER_HPP
/*
  Writer.hpp
  Copyright (C) 2011 David M. Anderson

  Writer abstract base class representing a file or similar object
  to which data can be written and within which the write position can be set.
*/


#include "BasicWriter.hpp"
#include "RandomAccess.hpp"
#include "Saver.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Writer
    :   public BasicWriter,
        public virtual RandomAccess,
        public Saver
{
public:
    virtual ~Writer( ) { }

    using Saver::Save;
    virtual void Save( const DataBuffer & buffer );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //WRITER_HPP

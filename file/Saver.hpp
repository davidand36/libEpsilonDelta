#ifndef SAVER_HPP
#define SAVER_HPP
/*
  Saver.hpp
  Copyright (C) 2011 David M. Anderson

  Saver abstract base class representing a file or device to which data
  can be saved.
  NOTES:
  1. Save() is a one-shot operation that writes all of the data at once.
     It may overwrite any previously saved data.
*/


#include "DataBuffer.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class Saver
{
public:
    virtual ~Saver( ) { }

    virtual void Save( const DataBuffer & buffer ) = 0;
    virtual void Save( const std::string & text );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SAVER_HPP

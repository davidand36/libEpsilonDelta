#ifndef RANDOMACCESS_HPP
#define RANDOMACCESS_HPP
/*
  RandomAccess.hpp
  Copyright (C) 2011 David M. Anderson

  RandomAccess abstract base class representing a file or similar object
  within which a position indicator may be moved.
  NOTES:
  1. Seek() combines the standard fseek() and ftell() operations, in that it
     returns the resulting file position.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class RandomAccess
{
public:
    enum Origin { Beginning, Current, End };

    virtual ~RandomAccess( ) { }

    virtual int Seek( int offset, Origin origin = Beginning ) = 0;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //RANDOMACCESS_HPP

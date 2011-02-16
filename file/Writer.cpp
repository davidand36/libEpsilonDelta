/*
  Writer.cpp
  Copyright (C) 2011 David M. Anderson

  Writer abstract base class representing a file or similar object
  to which data can be written and within which the write position can be set.
*/


#include "Writer.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void
Writer::Save( const DataBuffer & buffer )
{
    Seek( 0 );
    Write( buffer.Data(), buffer.Size() );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

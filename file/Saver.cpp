/*
  Saver.cpp
  Copyright (C) 2011 David M. Anderson

  Saver abstract base class representing a file or device to which data
  can be saved.
*/


#include "Saver.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void
Saver::Save( const std::string & text )
{
    DataBuffer buffer;
    buffer.Add( text.c_str(), (int)text.size() );
    Save( buffer );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

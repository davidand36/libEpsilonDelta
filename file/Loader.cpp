/*
  Loader.cpp
  Copyright (C) 2011 David M. Anderson

  Loader abstract base class representing a file or device from which data
  can be loaded.
*/


#include "Loader.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void
Loader::Load( string * pText )
{
    DataBuffer buffer;
    Load( &buffer );
    const vector< char > & vc = buffer.Buffer();
    pText->assign( vc.begin(), vc.end() );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

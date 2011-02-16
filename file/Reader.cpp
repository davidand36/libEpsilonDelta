/*
  Reader.hpp
  Copyright (C) 2011 David M. Anderson

  Reader abstract base class representing a file or similar object
  from which data can be read and the read pointer can be positioned.
*/


#include "Reader.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


void
Reader::Load( DataBuffer * pBuffer )
{
    int fileSize = Seek( 0, RandomAccess::End );
    Seek( 0 );
    pBuffer->Clear( );
    vector< char > & vc = pBuffer->GetBuffer();
    vc.resize( fileSize, 0 );
    Read( &(vc.at(0)), fileSize );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

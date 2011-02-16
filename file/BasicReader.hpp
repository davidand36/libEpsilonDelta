#ifndef BASICREADER_HPP
#define BASICREADER_HPP
/*
  BasicReader.hpp
  Copyright (C) 2011 David M. Anderson

  BasicReader abstract base class representing a file or similar object
  from which data may be read.
  NOTES:
  1. For our purposes Read() is considered a failure if the number of bytes
     read is different than that requested, whether because the end-of-file
     was encountered or for another reason. An exception should be thrown.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class BasicReader
{
public:
    virtual ~BasicReader( ) { }

    virtual void Read( char * buffer, int bufferSize ) = 0;
    template < typename T >
    void Read( T * pT );
};

//#############################################################################

template < typename T >
void 
BasicReader::Read( T * pT )
{
    Read( reinterpret_cast< char * >( pT ), (int) sizeof( T ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BASICREADER_HPP

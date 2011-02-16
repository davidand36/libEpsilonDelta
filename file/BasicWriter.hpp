#ifndef BASICWRITER_HPP
#define BASICWRITER_HPP
/*
  BasicWriter.hpp
  Copyright (C) 2011 David M. Anderson

  BasicWriter abstract base class representing a file or similar object
  to which data may be written.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class BasicWriter
{
public:
    virtual ~BasicWriter( ) { }

    virtual void Write( const char * buffer, int bufferSize ) = 0;
    template <typename T>
    void Write( const T & t );
};

//#############################################################################

template <typename T>
void 
BasicWriter::Write( const T & t )
{
    Write( reinterpret_cast< const char * >( &t ), (int)sizeof( T ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //BASICWRITER_HPP

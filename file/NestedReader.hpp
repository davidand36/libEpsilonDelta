#ifndef NESTEDREADER_HPP
#define NESTEDREADER_HPP
/*
  NestedReader.hpp
  Copyright (C) 2011 David M. Anderson

  NestedReader class: a "file" that is embedded inside another "file" at some
  specified offset.
*/

#include "Reader.hpp"
#include <tr1/memory>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class NestedReader
    :   public Reader
{
public:
    NestedReader( std::tr1::shared_ptr< Reader > spReader,
                  int offset, int size,
                  const std::string & name = "" );
    virtual ~NestedReader( ) { }

    virtual int Seek( int offset, Origin origin = Beginning );
    using Reader::Read;
    virtual void Read( char * buffer, int bufferSize );

#ifdef DEBUG
    static bool Test( );
#endif
    
//=============================================================================

private:
    std::tr1::shared_ptr< Reader > m_spReader;
    int m_offset;
    int m_size;
    int m_curPos;
    std::string m_name;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //NESTEDREADER_HPP

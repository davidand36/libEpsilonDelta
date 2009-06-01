#ifndef CGIRAWINPUT_HPP
#define CGIRAWINPUT_HPP
/*
  CGIRawInput.hpp
  Copyright (C) 2007 David M. Anderson
  
  CGIRawInput class:  Reads and parses CGI input, but allows access to raw data
*/


#include "CGIInput.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class CGIRawInput
    :   public Singleton< CGIRawInput >
{
public:
    bool ReadInput( int maxInputLength = 10000 );
    const std::string & RawInput( ) const;
    std::vector< std::string > Values( const std::string & name );
    const std::multimap< std::string, std::string > & Pairs( );

private:
    CGIRawInput( );
    virtual ~CGIRawInput( );

    bool m_parsed;

    friend class Singleton< CGIRawInput >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CGIRAWINPUT_HPP

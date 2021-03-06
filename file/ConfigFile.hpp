#ifndef CONFIGFILE_HPP
#define CONFIGFILE_HPP
/*
  ConfigFile.hpp
  Copyright (C) 2007 David M. Anderson

  ConfigFile class:  Reads, parses config file.
*/


#include "Reader.hpp"
#include <string>
#include <vector>
#include <map>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ConfigFile
{
public:
    explicit ConfigFile( Reader & reader );
    std::vector< std::string > Values( const std::string & name ) const;
    std::string Value( const std::string & name ) const;
    const std::multimap< std::string, std::string > & Pairs( );
#ifdef DEBUG
    static bool Test( );
#endif

private:
    void ParseInput( const std::string & rawInput  );

    std::multimap< std::string, std::string > m_pairs;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CONFIGFILE_HPP

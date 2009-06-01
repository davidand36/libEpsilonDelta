#ifndef CGIINPUT_HPP
#define CGIINPUT_HPP
/*
  CGIInput.hpp
  Copyright (C) 2007 David M. Anderson

  CGIInput class:  Reads and parses CGI input.
  NOTES:
  1. The CGI program should be a loop like this:
     int main( ... )
     {
         bool firstTime = true;
         while ( CGIInput::Instance().ReadInput( ) )
         {
             if ( firstTime )
             {
                 firstTime = false;
                 //Input-independent initialization steps
             }
             CGIOutput::Instance().Reset( );
             //Process input
             CGIOutput::Instance().Send(...);
         }
         return 0;
     }

  2. See tests/TestCGILib.cpp for a fuller example, including exception
     reporting.
*/


#include "Singleton.hpp"
#include <string>
#include <vector>
#include <map>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class CGIInput
    :   public Singleton< CGIInput >
{
public:
    bool ReadInput( int maxInputLength = 10000 );
    std::vector< std::string > Values( const std::string & name ) const;
    std::string operator[]( const std::string & name ) const;

private:
    CGIInput( );
    virtual ~CGIInput( );
    void SetParseAndFree( bool parseAndFree );
    void ReadRawInput( int maxInputLength );
    void ParseRawInput( );
    void ParseURLEncodedInput( const std::string & input );
    void ParseMultipartFormData( const std::string & input );
    void FreeRawInput( );
    const std::string & RawInput( ) const;
    const std::multimap< std::string, std::string > & Pairs( );

    bool m_initialized;
    bool m_parseAndFree;
    std::string m_rawInput;
    std::multimap< std::string, std::string > m_pairs;

    friend class Singleton< CGIInput >;
    friend class CGIRawInput;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CGIINPUT_HPP

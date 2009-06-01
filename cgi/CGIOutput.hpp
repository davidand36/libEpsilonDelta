#ifndef CGIOUTPUT_HPP
#define CGIOUTPUT_HPP
/*
  CGIOutput.hpp
  Copyright (C) 2007 David M. Anderson

  CGIOutput class: Outputs content, etc., to the client.
  NOTES:
  1. By default, output will be sent with content type "text/html" and
     character set UTF-8. These can be changed with the Set... commands.
     Reset() restores these defaults.
  2. SetFileName() with a nonempty file name causes the content to be marked
     as an attachment so that the browser will (typically) allow the user to
     save the file (with the given file name).
     Reset() also restores this to the empty default.
*/


#include "Singleton.hpp"
#include <string>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class CGIOutput
    :   public Singleton< CGIOutput >
{
public:
    void Send( const std::string & content ) const;
    void SetContentType( const std::string & contentType = "text/html" );
    void SetCharSet( const std::string & charSet = "UTF-8" );
    void SetFileName( const std::string & fileName = "" );
    void Reset( );

private:
    CGIOutput( );
    ~CGIOutput( );

    std::string m_contentType;
    std::string m_charSet;
    std::string m_fileName;

    friend class Singleton< CGIOutput >;
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CGIOUTPUT_HPP

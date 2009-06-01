/*
  CGIOutput.cpp
  Copyright (C) 2007 David M. Anderson

  CGIOutput class: Outputs content, etc., to the client.
*/


#include "CGIOutput.hpp"
#include "Assert.hpp"
#ifdef USE_FASTCGI
#include <fcgi_stdio.h>
#else
#include <iostream>
#endif
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


CGIOutput::CGIOutput( )
{
    Reset( );
}

//-----------------------------------------------------------------------------

CGIOutput::~CGIOutput( )
{
}

//=============================================================================

void
CGIOutput::Send( const string & content ) const
{
    if ( content.length() > 0 )
    {
        Assert( m_contentType.length() > 0 );
        printf( "Content-type: %s; charset=%s\n",
                m_contentType.c_str(), m_charSet.c_str() );
        if ( ! m_fileName.empty() )
            printf( "Content-disposition: attachment; filename=%s\n",
                    m_fileName.c_str() );
        printf( "\n%s", content.c_str() );
    }
}

//=============================================================================

void 
CGIOutput::SetContentType( const std::string & contentType )
{
    m_contentType = contentType;
}

//-----------------------------------------------------------------------------

void 
CGIOutput::SetCharSet( const std::string & charSet )
{
    m_charSet = charSet;
}

//-----------------------------------------------------------------------------

void 
CGIOutput::SetFileName( const std::string & fileName )
{
    m_fileName = fileName;
}

//-----------------------------------------------------------------------------

void 
CGIOutput::Reset( )
{
    m_contentType = "text/html";
    m_charSet = "UTF-8";
    m_fileName.clear();
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

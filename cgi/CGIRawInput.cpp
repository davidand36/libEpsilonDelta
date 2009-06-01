/*
  CGIRawInput.cpp
  Copyright (C) 2007 David M. Anderson
  
  CGIRawInput class:  Reads and parses CGI input, but allows access to raw data
  NOTES:
  1. This is primarily for testing purposes, to determine what data are being
     received by the client.
  2. It can be used instead of CGIInput; they won't coexist well.
  3. As with CGIInput, the program should loop until Init() returns false.
*/


#include "CGIRawInput.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


CGIRawInput::CGIRawInput( )
    :   m_parsed( false )
{
    CGIInput::Instance().SetParseAndFree( false );
}

//-----------------------------------------------------------------------------


CGIRawInput::~CGIRawInput( )
{
}

//=============================================================================

bool 
CGIRawInput::ReadInput( int maxInputLength )
{
    m_parsed = false;
    return CGIInput::Instance().ReadInput( maxInputLength );
}

//=============================================================================

const string & 
CGIRawInput::RawInput( ) const
{
    return CGIInput::Instance().RawInput();
}

//-----------------------------------------------------------------------------

vector< string > 
CGIRawInput::Values( const string & name )
{
    if ( ! m_parsed )
    {
        CGIInput::Instance().ParseRawInput( );
        m_parsed = true;
    }
    return CGIInput::Instance().Values( name );
}

//-----------------------------------------------------------------------------

const multimap< string, string > & 
CGIRawInput::Pairs( )
{
    if ( ! m_parsed )
    {
        CGIInput::Instance().ParseRawInput( );
        m_parsed = true;
    }
    return CGIInput::Instance().Pairs();
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

/*
  Exception.cpp
  Copyright (C) 2007 David M. Anderson

  Exception class:  Base exception class for my library.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

using namespace std;

//*****************************************************************************


Exception::Exception( const string & description )
    :   m_description( description )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Exception::Exception( const char * description )
    :   m_description( string( description ) )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

Exception::Exception( const exception & exceptn )
    :   m_description( exceptn.what( ) )
{
}

//-----------------------------------------------------------------------------

Exception::~Exception( )
{
}

//=============================================================================

string 
Exception::Description( ) const
{
    return m_description;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

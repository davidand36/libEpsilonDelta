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

//.............................................................................

Exception::Exception( const exception & exceptn )
    :   m_description( exceptn.what( ) )
{
}

//-----------------------------------------------------------------------------

Exception::~Exception( ) throw ()
{
}

//=============================================================================

void
Exception::SetDescription( const std::string & description )
{
    m_description = description;
}

//-----------------------------------------------------------------------------

string 
Exception::Description( ) const
{
    return m_description;
}

//-----------------------------------------------------------------------------

const char*
Exception::what() const throw()
{
    return m_description.c_str();
}


//*****************************************************************************


LogicError::LogicError( const std::string & description )
    :   Exception( description ),
        std::logic_error( description )
{
}


//*****************************************************************************


RuntimeError::RuntimeError( const std::string & description )
    :   Exception( description ),
        std::runtime_error( description )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

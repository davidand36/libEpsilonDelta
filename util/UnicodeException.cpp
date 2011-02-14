/*
  UnicodeException.cpp
  Copyright (C) 2007 David M. Anderson

  UnicodeException class: exception parsing Unicode string.
*/


#include "UnicodeException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


UnicodeException::UnicodeException( const std::string & description )
    :   RuntimeError( description )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

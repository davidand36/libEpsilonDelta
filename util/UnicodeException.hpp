#ifndef UNICODEEXCEPTION_HPP
#define UNICODEEXCEPTION_HPP
/*
  UnicodeException.hpp
  Copyright (C) 2007 David M. Anderson

  UnicodeException class: exception parsing Unicode string.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class UnicodeException
    :   public RuntimeError
{
public:
    UnicodeException( const std::string & description );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //UNICODEEXCEPTION_HPP

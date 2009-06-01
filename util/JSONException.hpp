#ifndef JSONEXCEPTION_HPP
#define JSONEXCEPTION_HPP
/*
  JSONException.hpp
  Copyright (C) 2007 David M. Anderson

  JSONException class: exception parsing JSON string.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class JSONException
        :   public Exception
{
public:
    JSONException( const std::string & description );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //JSONEXCEPTION_HPP

/*
  JSONException.cpp
  Copyright (C) 2007 David M. Anderson

  JSONException class: exception parsing JSON string.
*/


#include "JSONException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


JSONException::JSONException( const std::string & description )
    :   RuntimeError( description )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

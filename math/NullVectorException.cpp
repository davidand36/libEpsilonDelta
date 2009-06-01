/*
  NullVectorException.cpp
  Copyright (C) 2007 David M. Anderson

  NullVectorException class:  Exception class for attempts to invert,
    etc., a singular matrix.
*/


#include "NullVectorException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


NullVectorException::NullVectorException( )
    :   Exception( "Vector is null." )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

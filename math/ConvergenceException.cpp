/*
  ConvergenceException.cpp
  Copyright (C) 2007 David M. Anderson

  ConvergenceException class:  Exception class for attempts to invert,
    etc., a singular matrix.
*/


#include "ConvergenceException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


ConvergenceException::ConvergenceException( const std::string & description )
    :   RuntimeError( description )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

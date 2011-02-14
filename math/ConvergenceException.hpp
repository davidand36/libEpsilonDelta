#ifndef CONVERGENCEEXCEPTION_HPP
#define CONVERGENCEEXCEPTION_HPP
/*
  ConvergenceException.hpp
  Copyright (C) 2007 David M. Anderson

  ConvergenceException class:  Exception class for attempts to normalize, etc.,
    a null vector.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class ConvergenceException
    : public RuntimeError
{
public:
    ConvergenceException( const std::string & description );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //CONVERGENCEEXCEPTION_HPP

#ifndef SINGULARMATRIXEXCEPTION_HPP
#define SINGULARMATRIXEXCEPTION_HPP
/*
  SingularMatrixException.hpp
  Copyright (C) 2007 David M. Anderson

  SingularMatrixException class:  Exception class for attempts to invert,
    etc., a singular matrix.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SingularMatrixException
    : public Exception
{
public:
    SingularMatrixException( );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SINGULARMATRIXEXCEPTION_HPP

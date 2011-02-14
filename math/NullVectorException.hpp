#ifndef NULLVECTOREXCEPTION_HPP
#define NULLVECTOREXCEPTION_HPP
/*
  NullVectorException.hpp
  Copyright (C) 2007 David M. Anderson

  NullVectorException class:  Exception class for attempts to normalize, etc.,
    a null vector.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class NullVectorException
    : public LogicError
{
public:
    NullVectorException( );
};


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //NULLVECTOREXCEPTION_HPP

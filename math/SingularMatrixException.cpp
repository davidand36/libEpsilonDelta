/*
  SingularMatrixException.cpp
  Copyright (C) 2007 David M. Anderson

  SingularMatrixException class:  Exception class for attempts to invert,
    etc., a singular matrix.
*/


#include "SingularMatrixException.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


SingularMatrixException::SingularMatrixException( )
    :   Exception( "Matrix is singular." )
{
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

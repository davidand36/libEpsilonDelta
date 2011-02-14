#ifndef FILEEXCEPTION_HPP
#define FILEEXCEPTION_HPP
/*
  FileException.hpp
  Copyright (C) 2007 David M. Anderson

  FileException class, representing problems accessing files or directories.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class FileException
    :    public RuntimeError
{
public:
    explicit FileException( const std::string & description );
};

//#############################################################################

inline
FileException::FileException( const std::string & description )
    :    RuntimeError( description )
{ 
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FILEEXCEPTION_HPP

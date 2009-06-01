#ifndef SQLEXCEPTION_HPP
#define SQLEXCEPTION_HPP
/*
  SQLException.hpp
  Copyright (C) 2007 David M. Anderson

  SQLException class, representing problems accessing an SQL database.
*/


#include "Exception.hpp"


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


class SQLException
    :    public Exception
{
public:
    explicit SQLException( const std::string & description );
    virtual ~SQLException( ) { }
};

//#############################################################################

inline
SQLException::SQLException( const std::string & description )
    :    Exception( description )
{ 
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SQLEXCEPTION_HPP

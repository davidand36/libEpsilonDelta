#ifndef ASSERT_HPP
#define ASSERT_HPP
/*
  Assert.hpp
  Copyright (C) 2007 David M. Anderson

  Assert macro. Implemented using:
  AssertException class:  Exception class for assertion failures.
*/


#define ASSERT_IS_EXCEPTION  1

#ifdef DEBUG
#ifdef ASSERT_IS_EXCEPTION
#include "Exception.hpp"
#endif
#endif


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG
#ifdef ASSERT_IS_EXCEPTION

//.............................................................................

class AssertException
    : public Exception
{
public:
    AssertException( const std::string & description,
                     const std::string & sourceFile, int sourceLine );
};

//.............................................................................

#define Assert( Expr )                                                  \
{                                                                       \
    if ( ! (Expr) )                                                     \
        throw EpsilonDelta::AssertException( std::string( #Expr ),      \
                                             std::string( __FILE__ ),   \
                                             __LINE__ );                \
}

//.............................................................................

#endif //ASSERT_IS_EXCEPTION
#else //!DEBUG

#define Assert( Expr )  ((void)0)

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ASSERT_HPP

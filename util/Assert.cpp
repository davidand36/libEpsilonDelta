/*
  Assert.cpp
  Copyright (C) 2007 David M. Anderson

  AssertException class:  Exception class for assertion failures.
*/


#ifdef DEBUG


#include "Assert.hpp"
#include <stdio.h>
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef ASSERT_IS_EXCEPTION

AssertException::AssertException( const string & description,
				    const string & sourceFile, int sourceLine )
    :   Exception( "Assertion failure: " + description )
{
    if ( sourceFile.length() > 0 )
    {
        m_description += "\nFile: " + sourceFile;
        char lineStr[ 16 ];
        sprintf( lineStr, "%d", sourceLine );
        m_description += "  Line: ";
        m_description += lineStr;
    }
}

#endif //ASSERT_IS_EXCEPTION

//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //DEBUG

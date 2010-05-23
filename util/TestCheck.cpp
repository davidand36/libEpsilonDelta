/*
  TestCheck.cpp
  Copyright (C) 2007 David M. Anderson

  TESTCHECK macro/template function that compares an expression to its
  expected value and prints the comparison and result.
*/


#include "TestCheck.hpp"
using namespace std;


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

//=============================================================================


bool
TestCheckImpl< string >::Check( string value, string expected,
                                const char * valStr, bool * pOK,
                                TestCheck::EDetails details )
{
    if ( value == expected )
    {
        if ( details >= TestCheck::PrintAll )
        {
            cout << valStr << "==" << "\"" << value << "\"";
            cout << "\tOK" << endl;
        }
        return true;
    }
    else
    {
        if ( details >= TestCheck::PrintFailure )
        {
            cout << valStr << "==" << "\"" << value << "\"";
            cout << " should be \"" << expected << "\"\tFAILED" << endl;
        }
        if ( pOK )
            *pOK = false;
        return false;
    }
}

//-----------------------------------------------------------------------------

bool
TestCheckImpl< wstring >::Check( wstring value,
                                 wstring expected,
                                 const char * valStr, bool * pOK,
                                 TestCheck::EDetails details )
{
    if ( value == expected )
    {
        if ( details >= TestCheck::PrintAll )
        {
            cout << valStr << "==";
            wcout << L"\"" << value << L"\"" << L"\tOK" << endl;
        }
        return true;
    }
    else
    {
        if ( details >= TestCheck::PrintFailure )
        {
            cout << valStr << "==";
            wcout << L"\"" << value << L"\"" << L" should be \"" << expected
                  << L"\"\tFAILED" << endl;
        }
        if ( pOK )
            *pOK = false;
        return false;
    }
}

//-----------------------------------------------------------------------------

bool
TestCheckImpl< const char * >::Check( const char * value,
                                      const char * expected,
                                      const char * valStr, bool * pOK,
                                      TestCheck::EDetails details )
{
    return TestCheckImpl< string >::Check( string( value ), string( expected ),
                                           valStr, pOK, details );
}

//-----------------------------------------------------------------------------

bool
TestCheckImpl< const wchar_t * >::Check( const wchar_t * value,
                                         const wchar_t * expected,
                                         const char * valStr, bool * pOK,
                                         TestCheck::EDetails details )
{
    return TestCheckImpl< wstring >::Check( wstring( value ),
                                            wstring( expected ),
                                            valStr, pOK, details );
}

//=============================================================================

bool
DoTestCheck( int value, int expected, const char * valStr, bool * pOK,
             TestCheck::EDetails details )
{
    return TestCheckImpl< int >::Check( value, expected, valStr, pOK,
                                        details );
}

//-----------------------------------------------------------------------------

bool 
DoTestCheck( unsigned int value, unsigned int expected, const char * valStr,
             bool * pOK, TestCheck::EDetails details )
{
    return TestCheckImpl< unsigned int >::Check( value, expected, valStr,
                                                 pOK, details );
}

//-----------------------------------------------------------------------------

bool 
DoTestCheck( unsigned int value, int expected, const char * valStr,
             bool * pOK, TestCheck::EDetails details )
{
    return TestCheckImpl< int >::Check( static_cast< int >( value ), expected,
                                        valStr, pOK, details );
}

//-----------------------------------------------------------------------------

bool 
DoTestCheck( unsigned long value, int expected, const char * valStr, bool * pOK,
             TestCheck::EDetails details )
{
    return TestCheckImpl< int >::Check( static_cast< int >( value ), expected,
                                        valStr, pOK, details );
}

//-----------------------------------------------------------------------------

bool 
DoTestCheckF( double value, double expected, const char * valStr, bool * pOK,
              double epsilon, TestCheck::EDetails details )
{
    return TestCheckFImpl< double >::Check( value, expected, valStr, pOK,
                                            epsilon, details );
}

//=============================================================================

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

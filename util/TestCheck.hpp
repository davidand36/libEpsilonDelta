#ifndef TESTCHECK_HPP
#define TESTCHECK_HPP
/*
  TestCheck.hpp
  Copyright (C) 2007 David M. Anderson

  TESTCHECK macro/template function that compares an expression to its
  expected value and prints the comparison and result.
*/


#include "Platform.hpp"
#include "StringUtil.hpp"
#include <iostream>
#include <string>
#include <cmath>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#ifdef DEBUG

//=============================================================================

namespace TestCheck
{

enum EDetails { NoDetails, PrintFailure, PrintAll };

}

//-----------------------------------------------------------------------------

template < typename T >
class TestCheckImpl
{
public:
    static bool Check( T value, T expected, const char * valStr, bool * pOK,
                       TestCheck::EDetails details );
};

//-----------------------------------------------------------------------------

template<>
class TestCheckImpl< std::string >
{
public:
    static bool Check( std::string value, std::string expected,
                       const char * valStr, bool * pOK,
                       TestCheck::EDetails details );
};

//-----------------------------------------------------------------------------

template<>
class TestCheckImpl< std::wstring >
{
public:
    static bool Check( std::wstring value, std::wstring expected,
                       const char * valStr, bool * pOK,
                       TestCheck::EDetails details );
};

//-----------------------------------------------------------------------------

template<>
class TestCheckImpl< const char * >
{
public:
    static bool Check( const char * value, const char * expected,
                       const char * valStr, bool * pOK,
                       TestCheck::EDetails details );
};

//-----------------------------------------------------------------------------

template<>
class TestCheckImpl< const wchar_t * >
{
public:
    static bool Check( const wchar_t * value, const wchar_t * expected,
                       const char * valStr, bool * pOK,
                       TestCheck::EDetails details );
};

//-----------------------------------------------------------------------------

template< typename T >
class TestCheckFImpl
{
public:
    static bool Check( T value, T expected, const char * valStr,
                       bool * pOK, T epsilon, TestCheck::EDetails details );
};


//-----------------------------------------------------------------------------


template < typename T >
bool DoTestCheck( T value, T expected, const char * valStr, bool * pOK,
                  TestCheck::EDetails details = TestCheck::PrintAll );
template < typename T >
bool DoTestCheck( T * value, const T * expected, const char * valStr,
                  bool * pOK,
                  TestCheck::EDetails details = TestCheck::PrintAll );

bool DoTestCheck( int value, int expected, const char * valStr, bool * pOK,
                  TestCheck::EDetails details = TestCheck::PrintAll );
bool DoTestCheck( unsigned int value, unsigned int expected,
                  const char * valStr, bool * pOK,
                  TestCheck::EDetails details = TestCheck::PrintAll );
bool DoTestCheck( unsigned int value, int expected, const char * valStr,
                  bool * pOK,
                  TestCheck::EDetails details = TestCheck::PrintAll );
bool DoTestCheck( unsigned long value, int expected, const char * valStr,
                  bool * pOK,
                  TestCheck::EDetails details = TestCheck::PrintAll );

template < typename T >
bool DoTestCheckF( T value, T expected, const char * valStr, bool * pOK,
                 T epsilon, TestCheck::EDetails details = TestCheck::PrintAll );
bool DoTestCheckF( double value, double expected, const char * valStr,
                 bool * pOK, double epsilon,
                 TestCheck::EDetails details = TestCheck::PrintAll );


//#############################################################################


template < typename T >
bool 
TestCheckImpl<T>::Check( T value, T expected, const char * valStr, bool * pOK,
                         TestCheck::EDetails details )
{
    if ( value == expected )
    {
        if ( details >= TestCheck::PrintAll )
        {
            std::cout << valStr << "==" << value;
            std::cout << "\tOK" << std::endl;
        }
        return true;
    }
    else
    {
        if ( details >= TestCheck::PrintFailure )
        {
            std::cout << valStr << "==" << value;
            std::cout << " should be " << expected << "\tFAILED"
                      << std::endl;
        }
        if ( pOK )
            *pOK = false;
        return false;
    }
}

//-----------------------------------------------------------------------------

template< typename T >
bool
TestCheckFImpl<T>::Check( T value, T expected, const char * valStr, bool * pOK,
                          T epsilon, TestCheck::EDetails details )
{
    bool ok;
    if ( std::fabs( expected ) < 1.0 )
        ok = std::fabs( value - expected ) < epsilon;
    else
        ok = (std::fabs( (value - expected) / expected )) < epsilon;
    if ( ok )
    {
        if ( details >= TestCheck::PrintAll )
        {
            std::streamsize oldPrecision = std::cout.precision( 12 );
            std::cout << valStr << "==" << value;
            std::cout << "\tOK" << std::endl;
            std::cout.precision( oldPrecision );
        }
        return true;
    }
    else
    {
        if ( details >= TestCheck::PrintFailure )
        {
            std::streamsize oldPrecision = std::cout.precision( 12 );
            std::cout << valStr << "==" << value;
            std::cout << " should be " << expected << "\tFAILED"
                      << std::endl;
            std::cout.precision( oldPrecision );
        }
        if ( pOK )
            *pOK = false;
        return false;
    }
}


//=============================================================================


template < typename T >
bool
DoTestCheck( T value, T expected, const char * valStr, bool * pOK,
             TestCheck::EDetails details )
{
    return TestCheckImpl< T >::Check( value, expected, valStr, pOK, details );
}

//-----------------------------------------------------------------------------

template < typename T >
bool 
DoTestCheck( T * value, const T * expected, const char * valStr, bool * pOK,
             TestCheck::EDetails details )
{
    return TestCheckImpl< const T * >::Check( const_cast< const T * >( value ),
                                              expected, valStr, pOK, details );
}

//-----------------------------------------------------------------------------

template < typename T >
bool
DoTestCheckF( T value, T expected, const char * valStr, bool * pOK, T epsilon,
              TestCheck::EDetails details )
{
    return TestCheckFImpl< T >::Check( value, expected, valStr, pOK, epsilon,
                                       details);
}


//=============================================================================


#define TESTCHECK( val, exp, ok )  (DoTestCheck( (val), (exp), #val, (ok) ))
#define TESTCHECKF( val, exp, ok )  \
        (DoTestCheckF( (val), (exp), #val, (ok), 1e-6 ))
#define TESTCHECKFE( val, exp, ok, eps )  \
        (DoTestCheckF( (val), (exp), #val, (ok), (eps) ))
#define TESTCHECKD( val, exp, ok, details )  \
        (DoTestCheck( (val), (exp), #val, (ok), (details) ))

#else //!DEBUG

#define TESTCHECK( val, exp, ok )  ((void)0)
#define TESTCHECKF( val, exp, ok )  ((void)0)
#define TESTCHECKFE( val, exp, ok, eps )  ((void)0)
#define TESTCHECKD( val, exp, ok, details )  ((void)0)

#endif //DEBUG


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //TESTCHECK_HPP

#ifndef FACTORIAL_HPP
#define FACTORIAL_HPP
/*
  Factorial.hpp
  Copyright (C) 2007 David M. Anderson

  Factorial template function: computes n!
  LogFactorial: (natural) log of n!
  BinomialCoefficient: n! / k!(n-k)!, 0 <= k < n.
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename T >
T Factorial( T n );
double LogFactorial( int n );
double BinomialCoefficient( int n, int k );

#ifdef DEBUG
bool TestFactorial( );
#endif


//#############################################################################


template < typename T >
inline
T 
Factorial( T n )
{
    static T fact[33];  //cache
    if ( n <= 1 )
        return T( 1 );
    else if ( n < 33 )
    {
        int nn = static_cast< int >( n );
        if ( fact[nn] != T( 0 ) )
            return fact[nn];
        else
            return (fact[nn] = n * Factorial( n - T( 1 ) ));
    }
    else
        return n * Factorial( n - T( 1 ) );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //FACTORIAL_HPP

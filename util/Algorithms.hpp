#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP
/*
  Algorithms.hpp
  Copyright (C) 2007 David M. Anderson

  Some basic algorithms not in the STL.
  NOTES:
  1. MinIter() and MaxIter() return the iterator that points to the smallest
     or largest item in the sequence.
  2. MinIndex() and MaxIndex() return the index of the smallest or largest
     item in the sequence, that is, the difference between the corresponding
     iterator and begin. This is meaningful only for containers like arrays
     and vectors.
*/


#include <iterator>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename Iter >
Iter MinIter( Iter begin, Iter end );
template < typename Iter >
Iter MaxIter( Iter begin, Iter end );
template < typename Iter >
int MinIndex( Iter begin, Iter end );
template < typename Iter >
int MaxIndex( Iter begin, Iter end );

#ifdef DEBUG
bool TestAlgorithms( );
#endif


//#############################################################################


template < typename Iter >
Iter
MinIter( Iter begin, Iter end )
{
    Iter minIter = begin;
    typename std::iterator_traits<Iter>::value_type  minItem = *begin;
    for ( ++begin; begin != end; ++begin )
        if ( *begin < minItem )
        {
            minIter = begin;
            minItem = *begin;
        }
    return minIter;
}

//-----------------------------------------------------------------------------

template < typename Iter >
Iter
MaxIter( Iter begin, Iter end )
{
    Iter maxIter = begin;
    typename std::iterator_traits<Iter>::value_type  maxItem = *begin;
    for ( ++begin; begin != end; ++begin )
        if ( *begin > maxItem )
        {
            maxIter = begin;
            maxItem = *begin;
        }
    return maxIter;
}

//-----------------------------------------------------------------------------

template < typename Iter >
int 
MinIndex( Iter begin, Iter end )
{
    Iter minIter = MinIter( begin, end );
    return  static_cast<int>( minIter - begin );
}

//-----------------------------------------------------------------------------

template < typename Iter >
int 
MaxIndex( Iter begin, Iter end )
{
    Iter maxIter = MaxIter( begin, end );
    return  static_cast<int>( maxIter - begin );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ALGORITHMS_HPP

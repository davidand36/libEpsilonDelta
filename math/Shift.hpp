#ifndef SHIFT_HPP
#define SHIFT_HPP
/*
  Shift.hpp
  Copyright (C) 2009 David M. Anderson

  Template functions for arithmetic shift operations.
  NOTES:
  1. These are provided primarily to avoid compiler warnings about negative
     shift counts when the shift counts are template parameters.
  2. The "public" functions here are simply ShiftLeft<T,N>( T ) and
     ShiftRight<T,N>( T ).
     The structs are merely implementation details. (For one thing, default
     template parameters are not allowed for functions.)
*/


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


template < typename T, int N >  T ShiftLeft( T t );
template < typename T, int N >  T ShiftRight( T t );

#ifdef DEBUG
bool TestShift( );
#endif

//=============================================================================

template < typename T, int N, bool Pos = (N >= 0) >
struct Shifter
{
    static T ShiftLeft( T t );
    static T ShiftRight( T t );
};

//.............................................................................

template < typename T, int N >
struct Shifter< T, N, true >
{
    static T ShiftLeft( T t );
    static T ShiftRight( T t );
};

//.............................................................................

template < typename T, int N >
struct Shifter< T, N, false >
{
    static T ShiftLeft( T t );
    static T ShiftRight( T t );
};


//*****************************************************************************


template < typename T, int N >  
inline
T ShiftLeft( T t )
{
    return Shifter< T, N >::ShiftLeft( t );
}

//-----------------------------------------------------------------------------

template < typename T, int N >  
inline
T ShiftRight( T t )
{
    return Shifter< T, N >::ShiftRight( t );
}

//=============================================================================

template < typename T, int N >
inline
T
Shifter< T, N, true >::ShiftLeft( T t )
{
    return t << N;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
inline
T
Shifter< T, N, true >::ShiftRight( T t )
{
    return t >> N;
}

//=============================================================================

template < typename T, int N >
inline
T
Shifter< T, N, false >::ShiftLeft( T t )
{
    return t >> -N;
}

//-----------------------------------------------------------------------------

template < typename T, int N >
inline
T
Shifter< T, N, false >::ShiftRight( T t )
{
    return t << -N;
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //SHIFT_HPP

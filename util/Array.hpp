#ifndef ARRAY_HPP
#define ARRAY_HPP
/*
  Array.hpp
  Copyright (C) 2007 David M. Anderson

  ARRAYSIZE macro for built-in arrays.
  array template class: STL-compatible container using a built-in array.
  TwoDArray and ThreeDArray template classes: simple, heap-allocated
  arrays.
  NOTES:
  1. array can be initialized almost like built-in arrays, albeit with an
     extra set of braces to avoid warnings, e.g.,
     array<int, 2> a = {{ 0, 1 }};
  2. Because, in order to allow for array-style initialization, m_array is
     public, these operators are automatically implicitly defined:
     reference operator[]( size_t index );
     const_reference operator[]( size_t index ) const;
  3. The at() methods return the same results as operator[], but throw an
     exception if the index is out of range.
*/


#if defined( USE_BOOST )

#include <boost/array.hpp>
using boost::array;

#else

#include <iterator>
#include <stdexcept>
#include <utility>

#endif

#include <SmartPtr.hpp>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#define ARRAYSIZE( array )  (sizeof(array) / sizeof(array[0]))


//#############################################################################


#if ! defined( USE_BOOST )

//.............................................................................

template < typename T, size_t N >
class array
{
public:
    typedef T  value_type;
    typedef value_type *  pointer;
    typedef const value_type *  const_pointer;
    typedef value_type *  iterator;
    typedef const value_type *  const_iterator;
    typedef value_type &  reference;
    typedef const value_type &  const_reference;
    typedef std::reverse_iterator<iterator>  reverse_iterator;
    typedef std::reverse_iterator<const_iterator>  const_reverse_iterator;
    typedef size_t  size_type;
    typedef ptrdiff_t  difference_type;

    reference at( size_t index );
    const_reference at( size_t index ) const;
    reference front( );
    const_reference front( ) const;
    reference back( );
    const_reference back( ) const;

    iterator begin( );
    const_iterator begin( ) const;
    iterator end( );
    const_iterator end( ) const;
    reverse_iterator rbegin( );
    const_reverse_iterator rbegin( ) const;
    reverse_iterator rend( );
    const_reverse_iterator rend( ) const;

    operator T*( );
    operator const T*( ) const;

    size_t size( ) const;

    T m_array[ N ]; //public to allow {{...}} initialization
};

//.............................................................................

template < typename T, size_t N >
bool operator==( const array< T, N > & rhs, const array< T, N > & lhs );
template < typename T, size_t N >
bool operator<( const array< T, N > & rhs, const array< T, N > & lhs );

//.............................................................................

#endif //!USE_BOOST

//.............................................................................

#ifdef DEBUG
bool TestArray( );
#endif


//#############################################################################


template < typename T >
class TwoDArray
{
public:
    TwoDArray( int numRows, int numColumns );
    virtual ~TwoDArray( );
    int NumRows( ) const;
    int NumColumns( ) const;
    const T * operator[]( int row ) const;
    T * operator[]( int row );
    const T & operator()( int row, int column ) const;
    T & operator()( int row, int column );
    const T & at( int row, int column ) const;
    T & at( int row, int column );

private:
    int m_numRows;
    int m_numColumns;
    shared_array< T* > m_elements;
    shared_array< T > m_elements0;
};


#ifdef DEBUG
bool TestTwoDArray( );
#endif


//#############################################################################


template < typename T >
class ThreeDArray
{
public:
    ThreeDArray( int numSlices, int numRows, int numColumns );
    virtual ~ThreeDArray( );
    int NumSlices( ) const;
    int NumRows( ) const;
    int NumColumns( ) const;
    const T * const * operator[]( int slice ) const;
    T * const * operator[]( int slice );
    const T & operator()( int slice, int row, int column ) const;
    T & operator()( int slice, int row, int column );
    const T & at( int slice, int row, int column ) const;
    T & at( int slice, int row, int column );

private:
    int m_numSlices;
    int m_numRows;
    int m_numColumns;
    shared_array< T** > m_elements;
    shared_array< T* > m_elements0;
    shared_array< T > m_elements00;
};


#ifdef DEBUG
bool TestThreeDArray( );
#endif


//#############################################################################


#if ! defined( USE_BOOST )

//.............................................................................

template < typename T, size_t N >
typename array<T,N>::reference 
array<T,N>::at( size_t index )
{
    if ( index >= N )
        throw  std::out_of_range( "array" );
    return  m_array[ index ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
typename array<T,N>::const_reference 
array<T,N>::at( size_t index ) const
{
    if ( index >= N )
        throw  std::out_of_range( "array" );
    return  m_array[ index ];
}

//=============================================================================

template < typename T, size_t N >
typename array<T,N>::reference 
array<T,N>::front( )
{
    return  m_array[ 0 ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
typename array<T,N>::const_reference 
array<T,N>::front( ) const
{
    return  m_array[ 0 ];
}

//-----------------------------------------------------------------------------

template < typename T, size_t N >
typename array<T,N>::reference 
array<T,N>::back( )
{
    return  m_array[ N - 1 ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
typename array<T,N>::const_reference 
array<T,N>::back( ) const
{
    return  m_array[ N - 1 ];
}

//-----------------------------------------------------------------------------

template < typename T, size_t N >
typename array<T,N>::iterator 
array<T,N>::begin( )
{
    return  &m_array[ 0 ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
typename array<T,N>::const_iterator 
array<T,N>::begin( ) const
{
    return  &m_array[ 0 ];
}

//-----------------------------------------------------------------------------

template < typename T, size_t N >
typename array<T,N>::iterator 
array<T,N>::end( )
{
    return  &m_array[ N ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
typename array<T,N>::const_iterator 
array<T,N>::end( ) const
{
    return  &m_array[ N ];
}

//-----------------------------------------------------------------------------

template < typename T, size_t N >
typename array<T,N>::reverse_iterator 
array<T,N>::rbegin( )
{
    return  static_cast< reverse_iterator >( end() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
typename array<T,N>::const_reverse_iterator 
array<T,N>::rbegin( ) const
{
    return  static_cast< reverse_iterator >( end() );
}

//-----------------------------------------------------------------------------

template < typename T, size_t N >
typename array<T,N>::reverse_iterator 
array<T,N>::rend( )
{
    return  static_cast< reverse_iterator >( begin() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
typename array<T,N>::const_reverse_iterator 
array<T,N>::rend( ) const
{
    return  static_cast< reverse_iterator >( begin() );
}

//=============================================================================

template < typename T, size_t N >
array<T,N>::operator T*( )
{
    return  m_array;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, size_t N >
array<T,N>::operator const T*( ) const
{
    return  m_array;
}

//=============================================================================

template < typename T, size_t N >
size_t 
array<T,N>::size( ) const
{
    return  N;
}

//=============================================================================

template < typename T, size_t N >
bool 
operator==( const array< T, N > & lhs, const array< T, N > & rhs )
{
    for ( int i = 0; i < N; ++i )
        if ( ! (lhs[i] == rhs[i]) )
            return  false;
    return  true;
}

//-----------------------------------------------------------------------------

template < typename T, size_t N >
bool
operator<( const array< T, N > & lhs, const array< T, N > & rhs )
{
    for ( int i = 0; i < N; ++i )
        if ( lhs[i] < rhs[i] )
            return  true;
    return  false;
}

//.............................................................................

#endif //!USE_BOOST


//#############################################################################


template < typename T >
TwoDArray<T>::TwoDArray( int numRows, int numColumns )
    :   m_numRows( numRows ),
        m_numColumns( numColumns ),
        m_elements( new T*[ numRows ] ),
        m_elements0( new T[ numRows * numColumns ] )
{
    int numElements = numRows * numColumns;
    std::memset( m_elements0.get(), 0, numElements * sizeof( T ) );
    for ( int r = 0; r < numRows; ++r )
        m_elements[ r ] = &(m_elements0[ r * numColumns ]);
}

//-----------------------------------------------------------------------------

template < typename T >
TwoDArray<T>::~TwoDArray( )
{
}

//=============================================================================

template < typename T >
inline
int 
TwoDArray<T>::NumRows( ) const
{
    return m_numRows;
}

//-----------------------------------------------------------------------------

template < typename T >
inline
int 
TwoDArray<T>::NumColumns( ) const
{
    return m_numColumns;
}

//=============================================================================

template < typename T >
const T * 
TwoDArray<T>::operator[]( int row ) const
{
    return m_elements[ row ];
}

//-----------------------------------------------------------------------------

template < typename T >
T * 
TwoDArray<T>::operator[]( int row )
{
    return m_elements[ row ];
}

//=============================================================================

template < typename T >
inline
const T & 
TwoDArray<T>::operator()( int row, int column ) const
{
    return m_elements[ row ][ column ];
}

//-----------------------------------------------------------------------------

template < typename T >
inline
T & 
TwoDArray<T>::operator()( int row, int column )
{
    return m_elements[ row ][ column ];
}

//=============================================================================

template < typename T >
const T & 
TwoDArray<T>::at( int row, int column ) const
{
    if ( (row < 0) || (row >= m_numRows)
         || (column < 0) || (column >= m_numColumns) )
        throw  std::out_of_range( "TwoDArray" );
    return m_elements[ row ][ column ];
}

//-----------------------------------------------------------------------------

template < typename T >
T & 
TwoDArray<T>::at( int row, int column )
{
    if ( (row < 0) || (row >= m_numRows)
         || (column < 0) || (column >= m_numColumns) )
        throw  std::out_of_range( "TwoDArray" );
    return m_elements[ row ][ column ];
}


//#############################################################################


template < typename T >
ThreeDArray<T>::ThreeDArray( int numSlices, int numRows, int numColumns )
    :   m_numSlices( numSlices ),
        m_numRows( numRows ),
        m_numColumns( numColumns ),
        m_elements( new T**[ numSlices ] ),
        m_elements0( new T*[ numSlices * numRows ] ),
        m_elements00( new T[ numSlices * numRows * numColumns ] )
{
    int numElements = numSlices * numRows * numColumns;
    std::memset( m_elements00.get(), 0, numElements * sizeof( T ) );
    for ( int s = 0; s < numSlices; ++s )
    {
        m_elements[s] = &(m_elements0[ s * numRows ]);
        for ( int r = 0; r < numRows; ++r )
            m_elements[s][r]
                    = &(m_elements00[ (s * numRows + r) * numColumns ]);
    }
}

//-----------------------------------------------------------------------------

template < typename T >
ThreeDArray<T>::~ThreeDArray( )
{
}

//=============================================================================

template < typename T >
inline
int 
ThreeDArray<T>::NumSlices( ) const
{
    return m_numSlices;
}

//-----------------------------------------------------------------------------

template < typename T >
inline
int 
ThreeDArray<T>::NumRows( ) const
{
    return m_numRows;
}

//-----------------------------------------------------------------------------

template < typename T >
inline
int 
ThreeDArray<T>::NumColumns( ) const
{
    return m_numColumns;
}

//=============================================================================

template < typename T >
const T * const * 
ThreeDArray<T>::operator[]( int slice ) const
{
    return m_elements[ slice ];
}

//-----------------------------------------------------------------------------

template < typename T >
T * const * 
ThreeDArray<T>::operator[]( int slice )
{
    return m_elements[ slice ];
}

//=============================================================================

template < typename T >
inline
const T & 
ThreeDArray<T>::operator()( int slice, int row, int column ) const
{
    return m_elements[ slice ][ row ][ column ];
}

//-----------------------------------------------------------------------------

template < typename T >
inline
T & 
ThreeDArray<T>::operator()( int slice, int row, int column )
{
    return m_elements[ slice ][ row ][ column ];
}

//=============================================================================

template < typename T >
const T & 
ThreeDArray<T>::at( int slice, int row, int column ) const
{
    if ( (slice < 0) || (slice >= m_numSlices)
         || (row < 0) || (row >= m_numRows)
         || (column < 0) || (column >= m_numColumns) )
        throw  std::out_of_range( "ThreeDArray" );
    return m_elements[ slice ][ row ][ column ];
}

//-----------------------------------------------------------------------------

template < typename T >
T & 
ThreeDArray<T>::at( int slice, int row, int column )
{
    if ( (slice < 0) || (slice >= m_numSlices)
         || (row < 0) || (row >= m_numRows)
         || (column < 0) || (column >= m_numColumns) )
        throw  std::out_of_range( "ThreeDArray" );
    return m_elements[ slice ][ row ][ column ];
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //ARRAY_HPP

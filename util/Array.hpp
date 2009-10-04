#ifndef ARRAY_HPP
#define ARRAY_HPP
/*
  Array.hpp
  Copyright (C) 2007 David M. Anderson

  ARRAY_LENGTH macro for built-in arrays.
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


#include <iterator>
#include <stdexcept>
#include <utility>
#include <vector>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************


#define ARRAY_LENGTH( array )  (sizeof(array) / sizeof(array[0]))


//=============================================================================


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
    std::vector< T* > m_elements;
    std::vector< T > m_elements0;
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
    std::vector< T** > m_elements;
    std::vector< T* > m_elements0;
    std::vector< T > m_elements00;
};


#ifdef DEBUG
bool TestThreeDArray( );
#endif


//#############################################################################


template < typename T >
TwoDArray<T>::TwoDArray( int numRows, int numColumns )
    :   m_numRows( numRows ),
        m_numColumns( numColumns ),
        m_elements( numRows ),
        m_elements0( numRows * numColumns, 0 )
{
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
        m_elements( numSlices ),
        m_elements0( numSlices * numRows ),
        m_elements00( numSlices * numRows * numColumns, 0 )
{
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

#ifndef INDEXEDVECTOR_HPP
#define INDEXEDVECTOR_HPP
/*
  IndexedVector.hpp
  Copyright (C) 2007 David M. Anderson

  IndexedVector template class: a utility class for accessing elements of a
  vector (or other random-access container, such as an array) through a
  separate vector of indices.
  NOTE:
  1. This class is not a container but rather a link between containers that
     provides STL-compatible operations.
*/


#include <vector>
#include <iterator>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

template < typename T, typename I, typename CT, typename CI >
class IndexedVectorIterator;

//#############################################################################


template < typename T, typename I = int,
           typename CT = std::vector<T>, typename CI = std::vector<I> >
class IndexedVector
{
public:
    IndexedVector( CT & items, const CI & indices );

    size_t size( ) const;

    T & operator[]( size_t index );
    const T & operator[]( size_t index ) const;

    typedef T  value_type;

    typedef IndexedVectorIterator< T, I, CT, CI >  iterator;
    typedef IndexedVectorIterator< const T, I, CT, CI > const_iterator;

    iterator begin( );
    const_iterator begin( ) const;
    iterator end( );
    const_iterator end( ) const;

private:
    //Undefined, to avoid warning:
    IndexedVector & operator=( const IndexedVector & );

    CT & m_items;
    const CI & m_indices;
};


//*****************************************************************************


template < typename T, typename I, typename CT, typename CI >
class IndexedVectorIterator
{
public:
    IndexedVectorIterator( CT & items, const CI & indices, size_t index );
    IndexedVectorIterator & operator=( const IndexedVectorIterator & rhs );
    T & operator*( );
    const T & operator*( ) const;
    T * operator->( );
    const T * operator->( ) const;
    IndexedVectorIterator & operator++( );
    IndexedVectorIterator operator++( int );
    IndexedVectorIterator & operator--( );
    IndexedVectorIterator operator--( int );
    bool operator==( const IndexedVectorIterator & rhs );
    bool operator!=( const IndexedVectorIterator & rhs );

    typedef std::random_access_iterator_tag  iterator_category;
    typedef T  value_type;
    typedef T*  pointer;
    typedef T&  reference;
    typedef std::ptrdiff_t  difference_type;

private:
    CT & m_items;
    const CI & m_indices;
    size_t m_index;
};

//*****************************************************************************


#ifdef DEBUG
bool TestIndexedVector( );
#endif


//#############################################################################


template < typename T, typename I, typename CT, typename CI >
IndexedVector< T, I, CT, CI >::IndexedVector( CT & items, const CI & indices )
    :   m_items( items ),
        m_indices( indices )
{
}

//=============================================================================

template < typename T, typename I, typename CT, typename CI >
size_t 
IndexedVector< T, I, CT, CI >::size( ) const
{
    return  m_indices.size();
}

//=============================================================================

template < typename T, typename I, typename CT, typename CI >
T & 
IndexedVector< T, I, CT, CI >::operator[]( size_t index )
{
    return  m_items[ m_indices[ index ] ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, typename I, typename CT, typename CI >
const T & 
IndexedVector< T, I, CT, CI >::operator[]( size_t index ) const
{
    return  m_items[ m_indices[ index ] ];
}

//=============================================================================

template < typename T, typename I, typename CT, typename CI >
typename IndexedVector< T, I, CT, CI >::iterator 
IndexedVector< T, I, CT, CI >::begin( )
{
    return  iterator( m_items, m_indices, 0 );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, typename I, typename CT, typename CI >
typename IndexedVector< T, I, CT, CI >::const_iterator 
IndexedVector< T, I, CT, CI >::begin( ) const
{
    return  const_iterator( m_items, m_indices, 0 );
}

//-----------------------------------------------------------------------------

template < typename T, typename I, typename CT, typename CI >
typename IndexedVector< T, I, CT, CI >::iterator 
IndexedVector< T, I, CT, CI >::end( )
{
    return  iterator( m_items, m_indices, m_indices.size() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, typename I, typename CT, typename CI >
typename IndexedVector< T, I, CT, CI >::const_iterator 
IndexedVector< T, I, CT, CI >::end( ) const
{
    return  const_iterator( m_items, m_indices, m_indices.size() );
}


//#############################################################################


template < typename T, typename I, typename CT, typename CI >
IndexedVectorIterator< T, I, CT, CI >
        ::IndexedVectorIterator( CT & items, const CI & indices, size_t index )
    :   m_items( items ),
        m_indices( indices ),
        m_index( index )
{
}

//-----------------------------------------------------------------------------

template < typename T, typename I, typename CT, typename CI >
IndexedVectorIterator< T, I, CT, CI > & 
IndexedVectorIterator< T, I, CT, CI >::operator=(
    const IndexedVectorIterator & rhs )
{
    m_index = rhs.m_index;
    return  *this;
}

//=============================================================================

template < typename T, typename I, typename CT, typename CI >
T &
IndexedVectorIterator< T, I, CT, CI >::operator*( )
{
    return  m_items[ m_indices[ m_index ] ];
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, typename I, typename CT, typename CI >
const T &
IndexedVectorIterator< T, I, CT, CI >::operator*( ) const
{
    return  m_items[ m_indices[ m_index ] ];
}

//-----------------------------------------------------------------------------

template < typename T, typename I, typename CT, typename CI >
T * 
IndexedVectorIterator< T, I, CT, CI >::operator->( )
{
    return  &(m_items[ m_indices[ m_index ] ]);
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, typename I, typename CT, typename CI >
const T * 
IndexedVectorIterator< T, I, CT, CI >::operator->( ) const
{
    return  &(m_items[ m_indices[ m_index ] ]);
}

//=============================================================================

template < typename T, typename I, typename CT, typename CI >
IndexedVectorIterator< T, I, CT, CI > & 
IndexedVectorIterator< T, I, CT, CI >::operator++( )
{
    ++m_index;
    return  *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, typename I, typename CT, typename CI >
IndexedVectorIterator< T, I, CT, CI > 
IndexedVectorIterator< T, I, CT, CI >::operator++( int )
{
    IndexedVectorIterator old = *this;
    ++m_index;
    return  old;
}

//-----------------------------------------------------------------------------

template < typename T, typename I, typename CT, typename CI >
IndexedVectorIterator< T, I, CT, CI > & 
IndexedVectorIterator< T, I, CT, CI >::operator--( )
{
    --m_index;
    return  *this;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename T, typename I, typename CT, typename CI >
IndexedVectorIterator< T, I, CT, CI > 
IndexedVectorIterator< T, I, CT, CI >::operator--( int )
{
    IndexedVectorIterator old = *this;
    --m_index;
    return  old;
}

//=============================================================================

template < typename T, typename I, typename CT, typename CI >
bool 
IndexedVectorIterator< T, I, CT, CI >::operator==(
    const IndexedVectorIterator & rhs )
{
    return  (m_index == rhs.m_index);
}

//-----------------------------------------------------------------------------

template < typename T, typename I, typename CT, typename CI >
bool 
IndexedVectorIterator< T, I, CT, CI >::operator!=(
    const IndexedVectorIterator & rhs )
{
    return  (m_index != rhs.m_index);
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //INDEXEDVECTOR_HPP

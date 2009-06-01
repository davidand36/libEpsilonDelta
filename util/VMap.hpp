#ifndef VMAP_HPP
#define VMAP_HPP
/*
  VMap.hpp
  Copyright (C) 2007 David M. Anderson

  VMap template container class: like std::map, but implemented using a
  sorted std::vector.
  This will be more efficient than std::map when there are distinct phases of
  population and lookup.
  NOTES:
  1. VMap has the same interface and semantics as std::map, with a few
     exceptions, listed below.
  2. The value_type is std::pair< Key, T >, rather than pair< const Key, T >.
     (I'm not clever enough to get insert() to work with const.) 
  3. An additional method, reserve(), allows the user to optimize memory
     allocation.
  4. When items are added through the constructor and insert() functions whose
     arguments are a pair of iterators, uniqueness of elements is maintained
     directly through a call to to the std::unique algorithm. I have been
     unable to get this to work with an equivalence relation based on the
     key_compare member, so I've used equality (operator==).
  5. Vide Scott Meyers, "Effective STL", Item 23.
*/


#include <vector>
#include <algorithm>
#include <functional>


namespace EpsilonDelta
{                                                      //namespace EpsilonDelta

//*****************************************************************************

template < typename Key, typename T, typename KeyCompare, typename Alloc >
class VMap;
template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool operator==( const VMap< Key, T, KeyCompare, Alloc > & lhs,
                 const VMap< Key, T, KeyCompare, Alloc > & rhs );
template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool operator<( const VMap< Key, T, KeyCompare, Alloc > & lhs,
                const VMap< Key, T, KeyCompare, Alloc > & rhs );

//*****************************************************************************


template < typename Key, typename T, typename KeyCompare = std::less< Key >,
           typename Alloc = std::allocator< std::pair< Key, T > > >
class VMap
{                                                                        //VMap
public:
    VMap( );
    explicit VMap( const KeyCompare & keyCompare,
                   const Alloc & alloc = Alloc() );
    template < typename InputIterator >
    VMap( InputIterator first, InputIterator last,
          const KeyCompare & keyCompare = KeyCompare(),
          const Alloc & alloc = Alloc() );
    VMap( const VMap & rhs );
    ~VMap( );
    VMap & operator=( const VMap & rhs );

    T & operator[]( const Key & key );

private:
    typedef std::pair< Key, T >  PairType;
    typedef std::vector< PairType, Alloc >  PairVec;

    PairVec m_pairVec;
    KeyCompare m_keyCompare;

public:
    typedef Key  key_type;
    typedef T  data_type;
    typedef T  mapped_type;
    typedef PairType  value_type;
    typedef KeyCompare  key_compare;
    typedef Alloc  allocator_type;
    typedef typename PairVec::pointer  pointer;
    typedef typename PairVec::const_pointer  const_pointer;
    typedef typename PairVec::reference  reference;
    typedef typename PairVec::const_reference  const_reference;
    typedef typename PairVec::iterator  iterator;
    typedef typename PairVec::const_iterator  const_iterator;
    typedef typename PairVec::reverse_iterator  reverse_iterator;
    typedef typename PairVec::const_reverse_iterator  const_reverse_iterator;
    typedef typename PairVec::size_type  size_type;
    typedef typename PairVec::difference_type  difference_type;

    iterator begin( );
    const_iterator begin( ) const;
    iterator end( );
    const_iterator end( ) const;
    reverse_iterator rbegin( );
    const_reverse_iterator rbegin( ) const;
    reverse_iterator rend( );
    const_reverse_iterator rend( ) const;
    bool empty( ) const;
    size_type size( ) const;
    size_type max_size( ) const;
    void reserve( size_type n );
    allocator_type get_allocator() const;

    iterator find( const key_type & key );
    const_iterator find( const key_type & key ) const;
    size_type count( const key_type & key ) const;
    iterator lower_bound( const key_type & key );
    const_iterator lower_bound( const key_type & key ) const;
    iterator upper_bound( const key_type & key );
    const_iterator upper_bound( const key_type & key ) const;
    std::pair< iterator, iterator > equal_range( const key_type & key );
    std::pair< const_iterator, const_iterator >
            equal_range( const key_type & key ) const;

    std::pair< iterator, bool > insert( const value_type & val );
    iterator insert( iterator pos, const value_type & val );
    template < class InputIterator >
    void insert( InputIterator first, InputIterator last );
    void erase( iterator pos );
    size_type erase( const key_type & key );
    void erase( iterator first, iterator last );
    void clear( );

    void swap( VMap & vmap );

//.............................................................................

    class value_compare
        :   public std::binary_function< value_type, value_type, bool >
    {
    public:
        bool operator( )( const value_type & a, const value_type & b ) const;
        bool operator( )( const value_type & a, const key_type & b ) const;
        bool operator( )( const key_type & a, const value_type & b ) const;
        bool operator( )( const key_type & a, const key_type & b ) const;

    protected:
        value_compare( KeyCompare keyCompare );

        KeyCompare m_keyCompare;

        friend class VMap< Key, T, KeyCompare, Alloc >;
    };

//.............................................................................

    key_compare key_comp( ) const;
    value_compare value_comp( ) const;

    friend 
    bool operator==<>( const VMap & lhs, const VMap & rhs );
    friend 
    bool operator< <>( const VMap & lhs, const VMap & rhs );

private:
    static bool value_equal( const value_type & a, const value_type & b );
    void fixup( );
};                                                                       //VMap

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void swap( const VMap< Key, T, KeyCompare, Alloc > & a,
           const VMap< Key, T, KeyCompare, Alloc > & b );


#ifdef DEBUG
bool TestVMap( );
#endif


//*****************************************************************************


template < typename Key, typename T, typename KeyCompare, typename Alloc >
VMap< Key, T, KeyCompare, Alloc >::VMap( )
    :   m_pairVec( Alloc() ),
        m_keyCompare( KeyCompare() )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
VMap< Key, T, KeyCompare, Alloc >::VMap( const KeyCompare & keyCompare,
                                         const Alloc & alloc )
    :   m_pairVec( alloc ),
        m_keyCompare( keyCompare )
{
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
template < typename InputIterator >
VMap< Key, T, KeyCompare, Alloc >::VMap( InputIterator first,
                                         InputIterator last,
                                         const KeyCompare & keyCompare,
                                         const Alloc & alloc )
    :   m_pairVec( first, last, alloc ),
        m_keyCompare( keyCompare )
{
    fixup( );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
VMap< Key, T, KeyCompare, Alloc >::VMap( const VMap & rhs )
    :   m_pairVec( rhs.m_pairVec ),
        m_keyCompare( rhs.m_keyCompare )
{
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
VMap< Key, T, KeyCompare, Alloc >::~VMap( )
{
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
VMap< Key, T, KeyCompare, Alloc > & 
VMap< Key, T, KeyCompare, Alloc >::operator=( const VMap & rhs )
{
    if ( this == &rhs )
        return *this;
    m_pairVec = rhs.m_pairVec;
    m_keyCompare = rhs.m_keyCompare;
    return *this;
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
T & 
VMap< Key, T, KeyCompare, Alloc >::operator[]( const Key & key )
{
    iterator pos = lower_bound( key );
    if ( (pos == end()) || m_keyCompare( key, pos->first ) )
        pos = insert( pos, PairType( key, T() ) );
    return pos->second;
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::iterator 
VMap< Key, T, KeyCompare, Alloc >::begin( )
{
    return m_pairVec.begin();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::const_iterator 
VMap< Key, T, KeyCompare, Alloc >::begin( ) const
{
    return m_pairVec.begin();
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::iterator 
VMap< Key, T, KeyCompare, Alloc >::end( )
{
    return m_pairVec.end();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::const_iterator 
VMap< Key, T, KeyCompare, Alloc >::end( ) const
{
    return m_pairVec.end();
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::reverse_iterator 
VMap< Key, T, KeyCompare, Alloc >::rbegin( )
{
    return m_pairVec.rbegin();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::const_reverse_iterator 
VMap< Key, T, KeyCompare, Alloc >::rbegin( ) const
{
    return m_pairVec.rbegin();
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::reverse_iterator 
VMap< Key, T, KeyCompare, Alloc >::rend( )
{
    return m_pairVec.rend();
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::const_reverse_iterator 
VMap< Key, T, KeyCompare, Alloc >::rend( ) const
{
    return m_pairVec.rend();
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool 
VMap< Key, T, KeyCompare, Alloc >::empty( ) const
{
    return m_pairVec.empty();
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::size_type 
VMap< Key, T, KeyCompare, Alloc >::size( ) const
{
    return m_pairVec.size();
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::size_type 
VMap< Key, T, KeyCompare, Alloc >::max_size( ) const
{
    return m_pairVec.max_size();
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void 
VMap< Key, T, KeyCompare, Alloc >::reserve( size_type n )
{
    m_pairVec.reserve( n );
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::allocator_type 
VMap< Key, T, KeyCompare, Alloc >::get_allocator() const
{
    return m_pairVec.get_allocator();
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::iterator 
VMap< Key, T, KeyCompare, Alloc >::find( const key_type & key )
{
    PairType val( key, T() );
    iterator pos = std::lower_bound( begin(), end(), val, value_comp() );
    if ( (pos == end()) || m_keyCompare( key, pos->first ) )
        return end();
    return pos;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::const_iterator 
VMap< Key, T, KeyCompare, Alloc >::find( const key_type & key ) const
{
    PairType val( key, T() );
    const_iterator pos = std::lower_bound( begin(), end(), val, value_comp() );
    if ( (pos == end()) || m_keyCompare( key, pos->first ) )
        return end();
    return pos;
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::size_type 
VMap< Key, T, KeyCompare, Alloc >::count( const key_type & key ) const
{
    return ( std::binary_search( begin(), end(), key, value_comp() )
             ?  1  :  0 );
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::iterator 
VMap< Key, T, KeyCompare, Alloc >::lower_bound( const key_type & key )
{
    return std::lower_bound( begin(), end(), key, value_comp() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::const_iterator 
VMap< Key, T, KeyCompare, Alloc >::lower_bound( const key_type & key ) const
{
    return std::lower_bound( begin(), end(), key, value_comp() );
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::iterator 
VMap< Key, T, KeyCompare, Alloc >::upper_bound( const key_type & key )
{
    return std::upper_bound( begin(), end(), key, value_comp() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::const_iterator 
VMap< Key, T, KeyCompare, Alloc >::upper_bound( const key_type & key ) const
{
    return std::upper_bound( begin(), end(), key, value_comp() );
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
std::pair< typename VMap< Key, T, KeyCompare, Alloc >::iterator,
           typename VMap< Key, T, KeyCompare, Alloc >::iterator > 
VMap< Key, T, KeyCompare, Alloc >::equal_range( const key_type & key )
{
    return std::equal_range( begin(), end(), key, value_comp() );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
std::pair< typename VMap< Key, T, KeyCompare, Alloc >::const_iterator,
           typename VMap< Key, T, KeyCompare, Alloc >::const_iterator >
VMap< Key, T, KeyCompare, Alloc >::equal_range( const key_type & key ) const
{
    return std::equal_range( begin(), end(), key, value_comp() );
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
std::pair< typename VMap< Key, T, KeyCompare, Alloc >::iterator, bool > 
VMap< Key, T, KeyCompare, Alloc >::insert( const value_type & val )
{
    iterator pos = lower_bound( val.first );
    if ( (pos != end()) && (! m_keyCompare( val.first, pos->first )) )
    {
        pos->second = val.second;
        return std::make_pair( pos, false );
    }
    pos = m_pairVec.insert( pos, val );
    return std::make_pair( pos, true );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::iterator 
VMap< Key, T, KeyCompare, Alloc >::insert( iterator pos,
                                           const value_type & val )
{
    if ( (pos == end()) || m_keyCompare( val.first, pos->first ) )
        if ( (pos == begin()) || m_keyCompare( (pos - 1)->first, val.first ) )
            //pos really is where val belongs:
            return  m_pairVec.insert( pos, val );
    //pos was a poor hint:
    return  insert( val ).first;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
template < class InputIterator >
void 
VMap< Key, T, KeyCompare, Alloc >::insert( InputIterator first,
                                           InputIterator last )
{
    m_pairVec.insert( end(), first, last );
    fixup( );
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void 
VMap< Key, T, KeyCompare, Alloc >::erase( iterator pos )
{
    m_pairVec.erase( pos );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::size_type 
VMap< Key, T, KeyCompare, Alloc >::erase( const key_type & key )
{
    iterator pos = find( key );
    if ( pos == end() )
        return 0;
    m_pairVec.erase( pos );
    return 1;
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void 
VMap< Key, T, KeyCompare, Alloc >::erase( iterator first, iterator last )
{
    m_pairVec.erase( first, last );
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void 
VMap< Key, T, KeyCompare, Alloc >::clear( )
{
    m_pairVec.clear( );
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void 
VMap< Key, T, KeyCompare, Alloc >::swap( VMap & vmap )
{
    m_pairVec.swap( vmap.m_pairVec );
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::key_compare
VMap< Key, T, KeyCompare, Alloc >::key_comp( ) const
{
    return m_keyCompare;
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
typename VMap< Key, T, KeyCompare, Alloc >::value_compare
VMap< Key, T, KeyCompare, Alloc >::value_comp( ) const
{
    return value_compare( m_keyCompare );
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool 
VMap< Key, T, KeyCompare, Alloc >::value_equal( const value_type & a,
                                                const value_type & b )
{
    return (a.first == b.first);
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void 
VMap< Key, T, KeyCompare, Alloc >::fixup( )
{
    std::sort( begin(), end(), value_comp() );
    iterator uniqueEnd = std::unique( begin(), end(), value_equal );
    m_pairVec.erase( uniqueEnd, end() );
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool operator==( const VMap< Key, T, KeyCompare, Alloc > & lhs,
                 const VMap< Key, T, KeyCompare, Alloc > & rhs )
{
    return  (lhs.m_pairVec == rhs.m_pairVec);
}

//-----------------------------------------------------------------------------

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool operator<( const VMap< Key, T, KeyCompare, Alloc > & lhs,
                const VMap< Key, T, KeyCompare, Alloc > & rhs )
{
    return  (lhs.m_pairVec < rhs.m_pairVec);
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
void swap( const VMap< Key, T, KeyCompare, Alloc > & a,
           const VMap< Key, T, KeyCompare, Alloc > & b )
{
    a.swap( b );
}

//*****************************************************************************

template < typename Key, typename T, typename KeyCompare, typename Alloc >
VMap< Key, T, KeyCompare, Alloc >::value_compare::value_compare(
    KeyCompare keyCompare )
    :   m_keyCompare( keyCompare )
{
}

//=============================================================================

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool 
VMap< Key, T, KeyCompare, Alloc >::value_compare::operator( )(
    const value_type & a,
    const value_type & b ) const
{
    return m_keyCompare( a.first, b.first );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool 
VMap< Key, T, KeyCompare, Alloc >::value_compare::operator( )(
    const value_type & a,
    const key_type & b ) const
{
    return m_keyCompare( a.first, b );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool 
VMap< Key, T, KeyCompare, Alloc >::value_compare::operator( )(
    const key_type & a,
    const value_type & b ) const
{
    return m_keyCompare( a, b.first );
}

//:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::

template < typename Key, typename T, typename KeyCompare, typename Alloc >
bool 
VMap< Key, T, KeyCompare, Alloc >::value_compare::operator( )(
    const key_type & a,
    const key_type & b ) const
{
    return m_keyCompare( a, b );
}


//*****************************************************************************

}                                                      //namespace EpsilonDelta

#endif //VMAP_HPP
